#include "Plugin.h"
#include "InterfacePlugin.h"

#include <QLibrary>
#include <QDir>
#include <QtGui>
#include <QMessageBox>

namespace plugin
{

	QDomDocument* PluginsManager::getPluginsDom()
	{
		return pluginsDom;
	}

	QObject* PluginsManager::getPluginById(const QString &Id)
	{
		int countPlugin = pluginsContainer.count();
		for(int i=0; i<countPlugin; i++)
		{
			QString n = pluginsContainer.value(i)->getPluginId();
			if(Id.contains(n))
			{
				return pluginsContainer.value(i)->getPlugin();
			}
		}
		return 0;
	}

	QObject* PluginsManager::createFromPath( QString path )
	{
		QStringList files = QDir( path ).entryList( QDir::Files );
		int countPaths = files.size();
		for( int i = 0; i < countPaths; i++ )
		{
			QString fileName = path + "/" + files[i];
			if( PluginsManager::isPlugin( fileName ) )
				return PluginsManager::createPluginInstance( fileName, this );
		}
		return 0;
	}

	void PluginsManager::addPlugins(QObject* parent, QString idParent)
	{
		QObject *p = 0;
		PluginContainer* plgn;
		InterfacePlugin *plugin;

		for( int i = 0; i < pluginsContainer.count() ; i++ )
		{

			plgn = pluginsContainer.value(i);
			if(plgn->isWidget(idParent))
			{
				p = plgn->getPlugin();

				if( PLUGIN( p ) == 0 )
				{
					delete p;
					continue;
				}

				plugin = PLUGIN( p );
				plugin->createPlugin(parent ,idParent, this);

			}
		}
	}

	bool PluginsManager::writeXml(const QString &fileName)
	{
		QFile file(fileName);
		if (!file.open(QFile::WriteOnly | QFile::Text)) {

			return false;
		}

		int Indent = 4;
		QTextStream out(&file);
		pluginsDom->save(out,Indent);

		file.close();
		if (file.error()) {
			return false;
		}
		return true;
	}

	QObject* PluginsManager::copy( QString IdPlugin )
	{
		int countPlugin = pluginsContainer.count();
		for(int i=0; i<countPlugin; i++)
		{
			QString n = pluginsContainer.value(i)->getPluginId();
			if(IdPlugin.contains(n))
			{
				QObject *plugin = createFromPath( pluginsContainer[i]->getPath() );
				PLUGIN(plugin)->createPlugin(plugin ,"Main", this);
				return plugin;
			}
		}
		return 0;
	}

	QObject* PluginsManager::create( QString name )
	{
		if( !findedPlugins.contains( name ) )
			return 0;
		return PluginsManager::createPluginInstance( findedPlugins[ name ], this );
	}

	QStringList PluginsManager::supportedInterfaces( QString name ) const
	{
		if( !findedPlugins.contains( name ) )
			return QStringList();
		return PluginsManager::getSupportedPluginInterfaces( findedPlugins[ name ] );
	}

	void PluginsManager::research()
	{
		findedPlugins.clear();

		int countPaths = pathToFind.size();
		for( int i = 0; i < countPaths; i++ )
		{
			QStringList files = QDir( pathToFind[i] ).entryList( QDir::Files );

			files.sort();
			int countFiles = files.size();
			for( int j = 0; j < countFiles; j++ )
			{
				QString fileName = pathToFind[i] + "/" + files[j];
				if( PluginsManager::isPlugin( fileName ) )
					findedPlugins.insert( PluginsManager::getPluginName( fileName ), fileName );
			}
		}
	}


	QStringList PluginsManager::pluginsList() const
	{
		return findedPlugins.keys();
	}

	PluginsManager::PluginsManager( QString pathToPlugins )
	{
		pathToFind << pathToPlugins;
		research();
	}

	PluginsManager::PluginsManager( QStringList pathToPlugins )
	{
		pathToFind = pathToPlugins;
		research();
	}

	PluginsManager::~PluginsManager()
	{
	}

	PluginsManager::PluginsManager(QString pathToXMl, QWidget* main)
	{
		QFile xmlFile(pathToXMl);

		if(xmlFile.open(QIODevice::ReadWrite))
		{
			QString errorStr;
			int errorLine;
			int errorColumn;
			pluginsDom = new QDomDocument();
			if(!pluginsDom->setContent(&xmlFile, false, &errorStr, &errorLine, &errorColumn))
			{
				QMessageBox::warning(main,"Error Message","Error: Can't parse XML:" + errorStr + ", " + QString::number(errorLine) +", "+ QString::number(errorColumn));
			}
			else
			{
				QDomNodeList plugs = pluginsDom->elementsByTagName("plugin");

				for(int i=0; i<plugs.count(); i++)
				{
					QDomElement elm = plugs.item(i).toElement();

					if(elm.attribute("included")=="true")
					{
						QString Id;
						QStringList idsWidget;
						QString pathPlugin;
						QObject* plugin;
						Id = elm.firstChildElement("Id").text();

						QDomNodeList widgets = elm.firstChildElement("Widgets").elementsByTagName("Widget");
						for(int j=0; j<widgets.count(); j++)
						{
							idsWidget << widgets.item(j).toElement().text();
						}

						pathPlugin = elm.firstChildElement("path").text();

						plugin = createFromPath( pathPlugin );
						if(plugin)
						{
							PluginContainer* con = new PluginContainer(plugin, Id, idsWidget,pathPlugin);
							pluginsContainer << con;
						}

					}
				}
			}
		}
		else
		{
			QMessageBox::warning(main,"Error Message","Error: Can't open file");
		}

	}

	bool PluginsManager::isPlugin( QString pluginFile )
	{
		CreatePluginInstanceProc createProc = (CreatePluginInstanceProc)
			QLibrary::resolve( pluginFile, CREATE_PLUGIN_PROC_NAME_STR );

		GetPluginSupportedInterfacesProc getInterfacesProc = ( GetPluginSupportedInterfacesProc )
			QLibrary::resolve( pluginFile, GET_PLUGIN_SUPPORTED_INTERFACES_PROC_NAME_STR );

		GetPluginNameProc getNameProc = ( GetPluginNameProc )
			QLibrary::resolve( pluginFile, GET_PLUGIN_NAME_PROC_NAME_STR );

		return ( ( createProc != 0 ) && ( getInterfacesProc != 0 ) && ( getNameProc != 0 ) );
	}

	QString PluginsManager::getPluginName( QString pluginFile )
	{
		if( !isPlugin( pluginFile ) )
			return QString();

		GetPluginNameProc getNameProc = ( GetPluginNameProc )
			QLibrary::resolve( pluginFile, GET_PLUGIN_NAME_PROC_NAME_STR );

		return getNameProc();
	}

	QStringList PluginsManager::getSupportedPluginInterfaces( QString pluginFile )
	{
		if( !isPlugin( pluginFile ) )
			return QStringList();

		GetPluginSupportedInterfacesProc getInterfacesProc = ( GetPluginSupportedInterfacesProc )
			QLibrary::resolve( pluginFile, GET_PLUGIN_SUPPORTED_INTERFACES_PROC_NAME_STR );

		return getInterfacesProc().split( QRegExp( "\\s+" ) );
	}

	QObject *PluginsManager::createPluginInstance( QString pluginFile, PluginsManager *manager )
	{
		if( !isPlugin( pluginFile ) )
			return 0;

		CreatePluginInstanceProc createProc = (CreatePluginInstanceProc)
			QLibrary::resolve( pluginFile, CREATE_PLUGIN_PROC_NAME_STR );

		return createProc( manager );
	}

} /* namespace plugin */
