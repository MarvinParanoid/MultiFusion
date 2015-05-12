/**
 * contexthelp.h
 *
 *  Created on: 18.04.2009
 *      Author: Irina Latysheva
 */


#ifndef CONTEXTHELP_H
#define CONTEXTHELP_H

#include <QWidget>
#include <QObject>
#include <QMainWindow>
#include <QStringList>
#include <QWhatsThis>
#include <QDomNodeList>
#include <QDomElement>
#include <QMenuBar>
#include <QToolButton>
#include <QAction>
#include <QMessageBox>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QXmlQuery>
#include <QUrl>
#include <QVariant>
#include "./../../pluginTool/Plugin.h"
#include "./../../pluginTool/InterfacePlugin.h"
#include "./../../pluginTool/SignalHolder.h"
#include "./../../interfaces/FigureToolInterface.h"
#include "./../../interfaces/MainWindowInterface.h"
#include "./../../interfaces/ContextHelpInterface.h"

#include "HelpWindow.h"

class ContextHelp: public ContextHelpInterface, public InterfacePlugin
{
	Q_OBJECT
	Q_INTERFACES( ContextHelpInterface )
	Q_INTERFACES( InterfacePlugin )

	public slots:
		void activeChanged(QString id) {
			activePlugin = id;
			//QMessageBox::information(0, "Active changed!", id);
		}

		void showHelp() {
			show();
		}

		void showContextHelp() {
			showContext();
		}

	public:
		void show() {
			helpWindow->setContent(helpHTML);
			helpWindow->show();
		}

		void showContext() {
            #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
                QTextCodec::setCodecForCStrings( QTextCodec::codecForName("utf8") );
            #endif
			if (!activePlugin.isEmpty() && haveHelp->contains(activePlugin)) {
				QString style = contextStylesheetMock;
				style.replace("%helpId%", activePlugin);
				QString contextHTML;
				QXmlQuery query(QXmlQuery::XSLT20);
				query.setFocus(QUrl(helpXML));
				query.setQuery(style);
				//query.bindVariable("helpId", QVariant(activePlugin));
				query.evaluateTo(&contextHTML);
				helpWindow->setContent(contextHTML);
				helpWindow->show();
			} else {
				helpWindow->setContent(helpHTML);
				helpWindow->show();
			}
		}

		virtual void createPlugin(QObject *parent, QString idParent, plugin::PluginsManager *manager)
		{
			this->manager = manager;
			if (idParent == "Main") {
				mainWin = MAINWINDOW(parent);
				if(mainWin!=0)
				{
					QMenu *menu = mainWin->getHelpMenu();

                    QAction* help = menu->addAction( tr("Content ...") );
					help->setShortcut( tr( "F1" ) );
					help->setEnabled(true);

                    QAction* contextHelp = menu->addAction( tr("Help!") );
					contextHelp->setShortcut( tr( "Shift+F1" ) );
					contextHelp->setEnabled(true);

					parseHelpMessages();

					connect(help, SIGNAL( triggered( bool ) ), this, SLOT( showHelp() ) );
					connect(contextHelp, SIGNAL( triggered( bool ) ), this, SLOT( showContextHelp() ) );

					manager->addPlugins(this, "ContextHelp");
				}
			}
		}

		virtual QString getName()const
		{
			return "ContextHelp";
		}

		ContextHelp(plugin::PluginsManager *manager) {
            #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
                QTextCodec::setCodecForCStrings( QTextCodec::codecForName("utf8") );
            #endif

			haveHelp = new QStringList();

			pathToHelp = "./help/";
			helpXML = "./help/help.xml";
			pathToStylesheet = "./help/help_view.xslt";
			pathToContextStylesheet = "./help/help_show_one.xslt";

			wasInit = false;

			QXmlQuery query(QXmlQuery::XSLT20);
			query.setFocus(QUrl(helpXML));
			query.setQuery(QUrl(pathToStylesheet));
			query.evaluateTo(&helpHTML);

			QString helpTitle = "Справка";

			helpWindow = new HelpWindow(helpTitle, helpHTML, pathToHelp, 0);
		}

		~ContextHelp() {
			delete helpDom;
			delete helpWindow;
			delete haveHelp;
		}

	private:
		QString pathToHelp;
		QString helpXML;
		QString helpHTML;
		QString pathToStylesheet;
		QString pathToContextStylesheet;
		QString contextStylesheetMock;

		QStringList *haveHelp;

		QDomDocument *helpDom;
		HelpWindow *helpWindow;
		MainWindowInterface* mainWin;
		plugin::PluginsManager *manager;
		bool wasInit;
		QString activePlugin;

		void parseHelpMessages() {
			loadHelp();
			QDomNodeList plugins = manager->getPluginsDom()->elementsByTagName("plugin");
			for(int i = 0; i < plugins.count(); i++)
			{
				QDomElement element = plugins.item(i).toElement();
				QString currentID = element.firstChildElement("Id").text();
				haveHelp->append(currentID);
				QObject *temp = manager->getPluginById(currentID);
				if (temp != NULL) {
					InterfacePlugin *plugin = PLUGIN(temp);
					connect(plugin->getBeep(), SIGNAL(setActive(QString)), this, SLOT(activeChanged(QString)));
				}
			}
			wasInit = true;
		}

		void loadHelp() {
            #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
                QTextCodec::setCodecForCStrings( QTextCodec::codecForName("utf8") );
            #endif
			QFile xmlFile(pathToHelp);
			if(xmlFile.open(QIODevice::ReadWrite))
			{
				QString errorStr;
                int errorLine;
                int errorColumn;
				helpDom = new QDomDocument();
				if(!helpDom->setContent(&xmlFile, false))
				{
					QMessageBox::warning(0, "Help Error", "Error: Can't parse XML:" + errorStr + ", " + QString::number(errorLine) +", "+ QString::number(errorColumn));
				}
			}

			QFile file(pathToContextStylesheet);
			QString style = "";
            if (!file.open(QIODevice::ReadOnly)) // Open the file
			{
				QMessageBox::warning(0, "Help Error", "Error: Can't load single message stylesheet at " + pathToContextStylesheet);
			}
			QTextStream stream ( &file );
			stream.setCodec("utf8");
			QString line;
			while( !stream.atEnd() ) {
			     line = stream.readLine();
			     style.append(line);
			}
			file.close(); // when your done.
			contextStylesheetMock = style;
			//QMessageBox::information(0, "Active changed!", style);
		}
};


#endif // CONTEXTHELP_H
