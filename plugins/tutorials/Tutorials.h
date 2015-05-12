/**
 * tutorials.h
 *
 *  Created on: 18.04.2009
 *      Author: Irina Latysheva
 */


#ifndef TUTORIALS_H
#define TUTORIALS_H

#include <QWidget>
#include <QObject>
#include <QMainWindow>
#include <QStringList>
#include <QDomNodeList>
#include <QDomElement>
#include <QDomDocument>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QList>
#include <QFile>
#include <QTextStream>
#include <QXmlQuery>
#include <QUrl>
#include "./../../pluginTool/Plugin.h"
#include "./../../pluginTool/InterfacePlugin.h"
#include "./../../pluginTool/SignalHolder.h"
#include "./../../interfaces/MainWindowInterface.h"
#include "./../../interfaces/TutorialsInterface.h"

#include "Tutorial.h"
#include "HelpWindow.h"

class Tutorials: public TutorialsInterface, public InterfacePlugin
{
	Q_OBJECT
	Q_INTERFACES( TutorialsInterface )
	Q_INTERFACES( InterfacePlugin )

	public slots:
		void showTutorial(QString id, QString caption) {
			show(id, caption);
		}

	public:
		void show(QString id, QString caption) {
            #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
                QTextCodec::setCodecForCStrings( QTextCodec::codecForName("utf8") );
            #endif
			QString style = styleMock;
			style.replace("%id%", id);
			QString tutorialMessage;
			QXmlQuery query(QXmlQuery::XSLT20);
			query.setFocus(QUrl(pathToTutorials));
			query.setQuery(style);
			query.evaluateTo(&tutorialMessage);
			window->setContent(pathToTutorials, caption, tutorialMessage);
			window->show();
		}

		virtual void createPlugin(QObject *parent, QString idParent, plugin::PluginsManager *manager)
		{
			this->manager = manager;
			if (idParent == "Main") {
				mainWin = MAINWINDOW(parent);
				if(mainWin!=0)
                {

					QMenu *help = mainWin->getHelpMenu();
                    QAction *tutorialMenuAction = help->addAction( tr("Training manuals ...") );
					tutorialMenuAction->setEnabled(true);
					help->addAction(tutorialMenuAction);

					parseTutorials();
					tutorialMenuAction->setMenu(tutorialMenu);

					if (tutorialMenu->isEmpty())
						tutorialMenuAction->setEnabled(false);

					loadMock(pathToStyle);

					manager->addPlugins(this, "Tutorials");
				}
			}
		}

		virtual QString getName()const
		{
			return "Tutorials";
		}

		Tutorials(plugin::PluginsManager *manager) {
			tutorialMenu = new QMenu();
			plugins = new QStringList();
			window = new HelpWindow(0);

			pathToTutorials = "./tutorials/tutorials.xml";
			pathToStyle = "./tutorials/tutorials_view.xslt";
		}

		~Tutorials() {
			delete tutorialMenu;
			delete plugins;
			delete window;
		}

	private:
			MainWindowInterface* mainWin;
			plugin::PluginsManager *manager;

			QMenu *tutorialMenu;
			QStringList *plugins;
			HelpWindow *window;

			QString pathToTutorials;
			QString pathToStyle;
			QString styleMock;

			void parsePlugins() {
				QDomNodeList pluginDom = manager->getPluginsDom()->elementsByTagName("plugin");
				for(int i = 0; i < pluginDom.count(); i++)
				{
					QDomElement element = pluginDom.item(i).toElement();
					QString currentID = element.firstChildElement("Id").text();
					if (manager->getPluginById(currentID) != NULL)
						plugins->append(currentID);
				}
			}

			void parseTutorials() {
				parsePlugins();
                #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
                    QTextCodec::setCodecForCStrings( QTextCodec::codecForName("utf8") );
                #endif

				QFile xmlFile(pathToTutorials);
				if(xmlFile.open(QIODevice::ReadWrite))
				{
					QString errorStr;
					int errorLine;
					int errorColumn;
					QDomDocument *dom = new QDomDocument();
					if(!dom->setContent(&xmlFile, false))
					{
						QMessageBox::warning(0, "Tutorials Error", "Error: Can't parse XML:" + errorStr + ", " + QString::number(errorLine) +", "+ QString::number(errorColumn));
					}

					QDomNodeList tutorialDom = dom->elementsByTagName("Tutorial");
					for(int i = 0; i < tutorialDom.count(); i++)
					{
						QDomElement element = tutorialDom.item(i).toElement();
						QString currentID = element.firstChildElement("Id").text();

						bool shouldRender = true;
						QDomNodeList pluginsForThis = element.firstChildElement("plugins").elementsByTagName("Id");
						for (int j = 0; shouldRender && (j < pluginsForThis.count()); j++) {
							QString pluginId = pluginsForThis.item(j).toElement().text();
							if (!plugins->contains(pluginId)) {
								shouldRender = false;
							}
						}

						if(shouldRender) {
                            #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
                                QTextCodec::setCodecForCStrings( QTextCodec::codecForName("utf8") );
                            #endif
							QString currentCaption = element.firstChildElement("Caption").text().toUtf8();

							Tutorial *showThis = new Tutorial(0, currentID, currentCaption);
							showThis->setText(currentCaption);
							showThis->setEnabled(true);
							connect(showThis, SIGNAL( shouldShow( QString, QString ) ), this, SLOT( showTutorial( QString, QString ) ) );

							tutorialMenu->addAction(showThis);
						}
					}
				}
			}

			void loadMock(QString path) {
				QFile file(path);
				QString style = "";
                if (!file.open(QIODevice::ReadOnly))
				{
					QMessageBox::warning(0, "Tutorial Error", "Error: Can't load single message stylesheet at " + path);
				}
				QTextStream stream ( &file );
				stream.setCodec("utf8");
				QString line;
				while( !stream.atEnd() ) {
					 line = stream.readLine();
					 style.append(line);
				}
				file.close();
				styleMock = style;
			}
};


#endif // TUTORIALS_H
