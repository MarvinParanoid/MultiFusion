/**
 * savesvg.h
 *
 *  Created on: 4.11.2009
 *      Author: Irina Latysheva
 */


#ifndef SAVESVG_H
#define SAVESVG_H

#include <QWidget>
#include <QObject>
#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QSvgGenerator>
#include <QPainter>
#include <QFileDialog>
#include <QImage>
#include <qfile.h>
#include <qtextstream.h>
#include <qregexp.h>
#include "./../../pluginTool/Plugin.h"
#include "./../../pluginTool/InterfacePlugin.h"
#include "./../../interfaces/MainWindowInterface.h"
#include "./../../interfaces/SaveSVGInterface.h"
#include "./../../interfaces/PaintWidgetInterface.h"
#include "./../../interfaces/RPWInterface.h"
#include "./../../interfaces/BackgroundInterface.h"
#include "./../../interfaces/TimelineInterface.h"

class SaveSVG: public SaveSVGInterface, public InterfacePlugin
{
	Q_OBJECT
	Q_INTERFACES( SaveSVGInterface )
	Q_INTERFACES( InterfacePlugin )

	private:
		QAction* Export;
		QObject *timeline;
		MainWindowInterface* mainWin;
		QString lastTo;
		double fps;

	public slots:
        void ShowWindow(){
			QString fileName = QFileDialog::getSaveFileName( this,
                    tr("Export to SVG ..."),
					lastTo,
					tr( "SVG graphics (*.svg)" ) );
			if( fileName.isNull() ) {
				return;
			} else {
				lastTo = fileName;
			}

			int pointIndex = lastTo.lastIndexOf( '.' );
			if( pointIndex == -1 )
				lastTo += ".svg";

			if (SaveFile(lastTo)) {
                QMessageBox::information( 0, "SVG", tr("Animation has been saved successfully.") );
			} else {
                QMessageBox::information( 0, tr("Error!"), tr("Unable to save the animation.") );
			}
		};

	public:
		bool SaveFile(QString to) {
			// Preparing the whole thing...
			if (timeline != NULL) {
				//QMessageBox::information( 0, "Yeeah!", "Timeline plugin available." );
				fps = (TIMELINE(timeline))->getFps();
			} else
				fps = 24.0;
			PaintWidgetInterface *pw = PAINTWIDGETINTERFACE(mainWin->getPaintWidget());
			RPWInterface *rpw = RPWINTEFACE(pw->getRealPaintWidget());
			QSize sz = rpw->getSize();
			QRect size(0, 0, sz.width(), sz.height());
			QSvgGenerator *generator = new QSvgGenerator();
			generator->setSize(sz);
			QString tempfile = to + ".tmp";

			// Drawing the background...
			generator->setFileName(tempfile);
			QPainter painter;
			painter.begin(generator);
			FILL(rpw->getBackground(), painter, size);
			painter.end();

			// Stripping from last "</svg>"...
			QFile file(to);
			if (file.exists())
				file.remove();
            if (!file.open (QIODevice::Append))
				return false;
			QTextStream stream ( &file );

			QFile tmpfile(tempfile);
            if (!tmpfile.open (QIODevice::ReadOnly))
				return false;
			QTextStream tmpstream ( &tmpfile );
			QString line;
			while(!tmpstream.atEnd()) {
				line = tmpstream.readLine();
				if (!line.startsWith("</svg>"))
					stream << line << '\n';
			}
			stream << "<!-- fps: " << fps << " -->\n";
			stream.flush();
			tmpfile.close();
			tmpfile.remove();

			delete generator;

			// Drawing the frames...
			int layers = pw->countLayers();

			if ((layers == 1) && (pw->countFrames() == 1)) {
				// Only one static image is present
				generator = new QSvgGenerator();
				generator->setSize(sz);
				generator->setFileName(tempfile);
				QPainter p;
				p.begin(generator);
				pw->paintFrameTo(p, size, 0.0);
				p.end();
				delete generator;

				QStringList frameContents = stripeFrame(tempfile, 0, 0, true);
				for (int k = 0; k < frameContents.size(); k++)
					stream << frameContents.at(k) << '\n';
				stream.flush();
			} else {
				// Many frames can be extracted
				int currentLayer = pw->layer();
				for (int j = 0; j < layers; j++) {
					pw->setLayer(j);
					int frames = pw->countFrames();
					for (int i = 0; i < pw->getPositionFrame(frames - 1); i++) {
						stream << "<!-- Layer #" << j << "; Frame #" << i
						<< "; Position on timeline: "
						<< pw->getPositionFrame(i) << " -->\n";

						generator = new QSvgGenerator();
						generator->setSize(sz);
						generator->setFileName(tempfile);
						QPainter p;
						p.begin(generator);
						pw->paintFrameTo(p, size, i);
						p.end();
						delete generator;

						QStringList frameContents = stripeFrame(tempfile, i / fps, 1 / fps);
						for (int k = 0; k < frameContents.size(); k++)
							stream << frameContents.at(k) << '\n';
						stream.flush();
					}
				}
				pw->setLayer(currentLayer);
			}

			// Cleaning up...
			stream << "</svg>";
			file.close();
			return true;
		}

		virtual void createPlugin(QObject *parent, QString idParent,plugin::PluginsManager *manager)
		{
			if (idParent == "Main") {
				mainWin = MAINWINDOW(parent);
				if(mainWin!=0)
				{
					QMenu* fileMenu = mainWin->getFileMenu();
                    QAction* Export = fileMenu->addAction( tr("Export to SVG ...") );
					Export->setShortcut( tr( "Ctrl+E" ) );
					Export->setEnabled(true);

					connect(Export,SIGNAL( triggered( bool ) ), this, SLOT( ShowWindow() ) );
					manager->addPlugins(this, "SaveSVG");				}
			}
		}

		virtual QString getName()const
		{
			return "SaveSVG";
		}

		SaveSVG(plugin::PluginsManager *manager) {
			lastTo = "/";
			QString timelineStr = "Timeline";
			timeline = manager->getPluginById(timelineStr);
		}

		~SaveSVG() {}

	private:
		QStringList stripeFrame(QString tempfile, double startTime, double duration, bool onlyFrame = false) {
			QStringList frameContents;
			QFile file(tempfile);
            if (file.open (QIODevice::ReadOnly)) {
				QTextStream stream ( &file );
				if (!stream.atEnd()) {
					QString line;
					do {
						line = stream.readLine();
					}
					while(!stream.atEnd() && !line.startsWith("<g"));

					// The frame contents start here.
					if(!onlyFrame) {
						line.replace(QRegExp(">$"), "display=\"none\">");
						frameContents.append(line);
						QString animParam = "<set attributeName=\"display\" to=\"inline\" begin=\""
							+ QString::number(startTime, 'g', 3) + "s\" dur=\""
							+ QString::number(duration, 'g', 3) + "s\" fill=\"remove\" />";
						frameContents.append(animParam);
					} else {
						frameContents.append(line);
					}

					while(!stream.atEnd()) {
						line = stream.readLine();
						if (!line.startsWith("</svg>"))
							frameContents.append(line);
					}
				}

				file.close();
				file.remove();
			}
			return frameContents;
		}
};


#endif // SAVESVG_H
