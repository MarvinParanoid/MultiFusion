/**
*	@mainpage Документация по PluginSDK
*
*		В данной справке вы найдёте документацию по PluginSDK - SDK, который позволит вам создавать
*	без излишних усилий программы любой сложности, функциональные возможности которых впоследствии смогут
*	быть расширены за счёт использования различных систем плагинов. Рекомендуется начать с просмотра примеров
*	(в особенности этого: @ref FirstPlugin ).
*/

/**
*	@file Plugin.h Заголовочный файл, в котором находится описание всего, что может понадобится
*			для создания разнообразных плагинов. Т.е. заголовочный файл к pluginSDK :)
*
*	@author Серков Александр
*
*	@example FirstPlugin пример создания и использования простого плагина (он ничего не делает толком,
*					но для примера сойдёт :) )
*
*		Для начала нужно придумать интерфейс плагина. Т.е. те методы, что нам будут необходимы в классе-плагине.
*	Данный интерфейс описыватся в отдельном заголовочном файле и все методы его должны быть абстрактными и
*	виртуальными. Вот пример:
*
*	<b>файл <i>FirstPluginInterface.h</i></b>
*
*	@code
*
*	#ifndef __FIRST_PLUGIN_INTERFACE__
*	#define __FIRST_PLUGIN_INTERFACE__
*
*	#include <QString>
*
*	class FirstPluginInterface
*	{
*		public:
*			virtual QString getTestString( QString param ) const = 0;
*			virtual void printMessage( QString message ) = 0;
*
*					 FirstPluginInterface() {};
*			virtual ~FirstPluginInterface() {};
*	};
*
*	//И, что главное, не забудьте обьявить ваш интерфейс интерфейсом:
*	Q_DECLARE_INTERFACE( FirstPluginInterface,
*                     "com.alex.test.FirstPluginInterface/1.0")
*
*	#endif // __FIRST_PLUGIN_INTERFACE__
*
*	@endcode
*
*		Далее, пишется сам плагин, который реализует данный интерфейс. Он должен обязательно быть наследником
*	класса QObject или любого его производного класса (например, QWidget). Так же данный плагин обязан реализовывать
*	все методы, указанные в классе-интерфейсе. Ну, и в дополнение, он в конструкторе должен принимать один
*	параметр - указатель на plugin::PluginsManager. Это указатель на тот самый менеджер плагинов, который
*	загружает данный плагин.
*
*	<b>файл <i>FirstPlugin.h</i></b>
*
*	@code
*
*	#ifndef __FIRST_PLUGIN__
*	#define __FIRST_PLUGIN__
*
*	#include <QObject>
*	#include "Plugin.h" //	Заголовочный файл, в котором определены всякие
*						//нужные для создания плагинов макросы и классы :)
*	#include "FirstPluginInterface.h"
*
*	class FirstPlugin:public QObject, public FirstPluginInterface
*	{
*		Q_OBJECT	//	Данный макрос тут необходим, так как наш плагин
*					//наследуется от класса QObject (ну, или от потомка класса QObject - это не так важно).
*		Q_INTERFACES( FirstPluginInterface ) //		С помощью данного макроса мы обьявляем, что используем
*											 //те или иные интерфейсы. Ну, в этом примере это FirstPluginInterface.
*		public:
*			virtual QString getTestString( QString param ) const;
*			virtual void printMessage( QString message );
*
					 // Главное не забыть про этот единственный параметр. А то компилироваться оно не будет.
*					 FirstPlugin( const plugin::PluginsManager *manager ) {};
*			virtual ~FirstPlugin() {};
*	};
*
*	#endif // __FIRST_PLUGIN__
*
*	@endcode
*
*	<b>файл <i>FirstPlugin.cpp</i></b>
*
*	@code
*
*	#include <QMessageBox>
*	#include "FirstPlugin.h"
*
*	//	Тут приведены самые простые реализации данного плагина. Но, вообще, никаких ограничений на сложность
*	//и использование тех или иных вещей нет.
*
*	QString FirstPlugin::getTestString( QString param ) const
*	{
*		return param + " :)";
*	}
*
*	void FirstPlugin::printMessage( QString message )
*	{
*		QMessageBox::information( 0, "Message", message );
*	}
*
*	EXPORT_PLUGIN( FirstPlugin, FirstPluginInterface ) 	//	Ну, и наконец необходимо применить и этот макрос.
*														//Он создаст много чего необходимого для функционирования
*														//плагина, что вручную бы писать было очень скучно каждый раз.
*
*	@endcode
*
*		Ну, собственно всё. Теперь нужно скомпилировать плагин. Для этого можно написать примерно следующий файл проекта
*	для сборки:
*
*	@code
*
*	TEMPLATE = lib
*	TARGET = FirstPlugin
*	DEPENDPATH += .
*	INCLUDEPATH += .
*	HEADERS += FirstPluginInterface.h FirstPlugin.h Plugin.h
*	SOURCES += FirstPlugin.cpp
*
*	@endcode
*
*		Данный файл нужно сохранить с расширением *.pro и положить рядом со всеми файлами проекта. Туда же необходимо
*	скопировать и файл Plugin.h. После чего можно набрать следующие команды в нижеуказанном порядке:
*
*	@code
*	qmake
*	make (или nmake для Microsoft Visual Studio)
*	@endcode
*
*	Всё, ваш первый плагин готов :)
*
*		Осталось только написать программу, которая использовала бы плагины данного типа. Это тоже довольно просто.
*	Примерно так:
*
*	<b>файл <i>FirstPluginTest.cpp</i></b>
*
*	@code
*
*	#include "FirstPluginInterface.h"
*	#include "Plugin.h"
*
*	int main( int argC, char *argV[] )
*	{
*		plugin::PluginsManager manager( "./" );	//Создаём обьект, который поможет нам згрузить наш плагин.
*												//Его можно загружать и вручную, но так намного удобнее.
*
*		FirstPluginInterface *testPlugin =
*			qobject_cast< FirstPluginInterface* >( manager.create( "FirstPlugin" ) );
*					//Создаём экземпляр нашего плагина. Так как при создании плагина мы получаем указатель на
*					//обьект QObject, мы сразу приводим данный указатель к типу указателя на наш интерфейс.
*					//Если произошла ошибка (например, мы загрузили не тот плагин случайно) и данный плагин
*					//не поддерживает запрашиваемый интерфейс, то qobject_cast вернёт нам 0.
*
*		if( testPlugin != 0 ) //Проверяем то, что плагин загрузился и поддерживает нужный нам интерфейс.
*		{
*			//Вызываем методы интерфейса. Естественно, что будут вызваны методы реального плгина.
*			testPlugin->printMessage( testPlugin->getTestString( "Hello wordl!" ) );
*			delete testPlugin; //Удаляем уже не нужный нам плагин.
*		}
*
*		return 0;
*	}
*
*	@endcode
*
*		Теперь положите файлы FirstPluginInterface.h, Plugin.h, libplugin.a (plugin.lib для Windows) и FirstPluginTest.cpp в одну папку
*	и скопируйте туда же получившийся плагин. Так же напишите следующий файл с правилами для компиляции:
*
*	<b>файл <i>FirstPluginTest.pro</i></b>
*
*	@code
*
*	TEMPLATE = app
*	TARGET = FirstPluginTest
*	DEPENDPATH += .
*	INCLUDEPATH += .
*	HEADERS += FirstPluginInterface.h Plugin.h
*	SOURCES += FirstPluginTest.cpp
*	LIB += -L./ -lplugin
*
*	@endcode
*
*	И снова наберите командочки для компиляции программы:
*
*	@code
*	qmake
*	make (или nmake для Microsoft Visual Studio)
*	@endcode
*
*		Запустите полученную программу. Она должна загрузить плагин и показать окошко с сообщением и следующим текстом:
*	"Hello wordl! :)"
*
*		Хочется добавить, что в случае, если вам не нужно использовать какие-либо интерфейсы и
*	вам достаточно будет стандартного интерфейса любого класса, происходящего откласса QObject,
*	то вы можете не обьявлять свой интерфейс. И в этом случае вы просто не используете макрос
*	Q_INTERFACES и вместо макроса EXPORT_PLUGIN используете EXPORT_QOBJECT_PLUGIN.
*/
#ifndef __PLUGIN_H__
#define __PLUGIN_H__

#include <QObject>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QtXml>
#include <QtXmlPatterns>

#define CREATE_PLUGIN_PROC_NAME createPluginInstance
#define GET_PLUGIN_SUPPORTED_INTERFACES_PROC_NAME getSupportedInterfaces
#define GET_PLUGIN_NAME_PROC_NAME getPluginName
#define CREATE_PLUGIN_PROC_NAME_STR "createPluginInstance"
#define GET_PLUGIN_SUPPORTED_INTERFACES_PROC_NAME_STR "getSupportedInterfaces"
#define GET_PLUGIN_NAME_PROC_NAME_STR "getPluginName"

namespace plugin
{
	class PluginsManager;
} /** namespace plugin **/


typedef QObject* (*CreatePluginInstanceProc)( plugin::PluginsManager *manager );
typedef QString (*GetPluginSupportedInterfacesProc)();
typedef QString (*GetPluginNameProc)();

/**
*	@name EXPORT_PLUGIN
*
*		Макрос EXPORT_PLUGIN обязан быть применен к тому классу, который должен стать в будущем
*	плагином. Он, собственно, создает все то, что нужно для плагина, но что было бы неинтересно
*	каждый раз писать самостоятельно, своими руками.
*
*	@param className имя класса, экспортируемого из плагина. В каждом из плагинов может находится только
*				один класс. Будьте внимательны :)
*
*	@param supportedInterfaces список поддерживаемых интерфейсов. В целом не особенно нужен, но может и
*				пригодиться для особо сложных систем. Данный список представляет собой просто перечисление
*				поддерживаемых интерфейсов, разделенных пробелами
*
*	@note ни один из параметров не должен быть заключен в кавычки.
*
*	@example SimplePlugin Простой пример использования данного макроса EXPORT_PLUGIN:
*	@code
*
*	// Файл SimplePlugin.h
*	class SimplePlugin:public QObject, public SomeInterface, public NextSomeInterface
*	{
*		Q_OBJECT
*		Q_INTERFACES( SomeInterface NextSomeInterface )
*
*		public:
*			...
*	};
*
*	//Файл SimplePlugin.cpp
*
*	SimplePlugin::SimplePlugin():QObject( 0 )
*	{
*	}
*
*	...
*
*	EXPORT_PLUGIN( SimplePlugin, SomeInterface NextSomeInterface )
*
*	@endcode
*/
#define EXPORT_PLUGIN( className, supportedInterfaces )\
	extern "C"\
	{\
		Q_DECL_EXPORT QObject* CREATE_PLUGIN_PROC_NAME( plugin::PluginsManager *manager )\
		{\
			return new className( manager );\
		}\
		\
		Q_DECL_EXPORT QString GET_PLUGIN_SUPPORTED_INTERFACES_PROC_NAME()\
		{\
			return #supportedInterfaces;\
		}\
		\
		Q_DECL_EXPORT QString GET_PLUGIN_NAME_PROC_NAME()\
		{\
			return #className;\
		}\
	}

/**
*	@name EXPORT_QOBJECT_PLUGIN
*
*		Макрос EXPORT_QOBJECT_PLUGIN обязан быть применен к тому классу, который должен стать в будущем
*	плагином, но который не реализует никаких дополнительных интерфейсов
*	(кроме интерфейса любого наследника QObject).
*	Он, собственно, создает все то, что нужно для плагина, но что было бы неинтересно
*	каждый раз писать самостоятельно, своими руками.
*
*	@param className имя класса, экспортируемого из плагина. В каждом из плагинов может находится только
*				один класс. Будьте внимательны :)
*
*	@note имя класса не должено быть заключено в кавычки.
*
*	@example SimplePlugin2 Простой пример использования макроса EXPORT_QOBJECT_PLUGIN:
*	@code
*
*	// Файл SimplePlugin2.h
*	class SimplePlugin2:public QWidget
*	{
*		Q_OBJECT
*		public:
*			...
*	};
*
*	//Файл SimplePlugin2.cpp
*
*	SimplePlugin2::SimplePlugin2():QObject( 0 )
*	{
*	}
*
*	...
*
*	EXPORT_QOBJECT_PLUGIN( SimplePlugin2 )
*
*	@endcode
*/
#define EXPORT_QOBJECT_PLUGIN( className )\
				EXPORT_PLUGIN( className,  )


class PluginContainer /**Класс содержащий объект плагина и информацию о нём**/
{	public:
		PluginContainer(QObject* _plugin,QString _idPlugin,QStringList _idsWidget,QString _path):
			plugin(_plugin), idPlugin(_idPlugin), idsWidget(_idsWidget), path(_path){}

		bool isWidget(QString idWidget) /**Проверяет работает ли с данным виджетом плагин**/
		{
			return idsWidget.contains(idWidget);
		}

		QObject* getPlugin() /** Возвращает указатель на плагин**/
		{
			return plugin;
		}

		QString getPluginId()
		{
			return idPlugin;
		}

		QString getPath()
		{
			return path;
		}
	private:
		QObject* plugin;
		QString idPlugin; /** Id плагина **/
		QStringList idsWidget;/**Id виджетов с которыми работает плагин**/
		QStringList interfaces;
		QString path;
};

/**
*		В данном пространстве имен находятся все классы, отвечающие за работу с плагинами.
*	Их немного и они совсем не страшные :)
*/
namespace plugin
{
	/**
	*		Данный класс отвечает за загрузку плагинов и получения дополнительной информации о них,
	*	такой как список поддерживаемых интерфейсов какого-либо плагина, имена плагинов и прочего.
	*	Так же он может клонировать экземпляры плагинов, если они были загружены с его помощью.
	*/
	class PluginsManager
	{
		public:
			virtual QDomDocument* getPluginsDom();

			virtual QObject* createFromPath( QString path );

			virtual void addPlugins(QObject* parent, QString idParent);

			virtual bool writeXml(const QString &fileName);

			virtual QObject* getPluginById(const QString &Id);

			/**
			*	Создает новый экземпляр (если может) плагина. Причем не копирует, а именно создает.
			*
			*	@param plugin уже созданный экземпляр плагина
			*
			*	@return новый экземпляр плагина
			*/
			virtual QObject* copy( QString IdPlugin ) ;

			/**
			*	Создает новый экземпляр плагина по его имени.
			*
			*	@param name имя создаваемого плагина
			*
			*	@return новосозданный экземпляр плагина
			*/
			virtual QObject* create( QString name ) ;

			/**
			*	Возвращает список поддерживаемых плагином интерфейсов.
			*
			*	@param name имя плагина
			*
			*	@return список имен поддерживаемых интерфейсов
			*/
			virtual QStringList supportedInterfaces( QString name )const ;

			/**
			*		Повторяет поиск плагинов по тем путям, которые были указаны в конструкторе.
			*	Может быть использован в том случае, если есть вероятность того, что какие-то плагины
			*	были удалены или наоборот, добавлены в те папки, в которых до этого производился поиск.
			*/
			virtual void research();



			/**
			*	Возвращает список имен найденных плагинов.
			*
			*	@return список имен найденных плагинов
			*/
			virtual QStringList pluginsList()const;

				/**
				*		Конструктор номер раз. Сразу же составляет список плагинов, которые находятся по
				*	указанному пути.
				*
				*	@param pathToPlugins путь, по которому можно найти плагины
				*/
				PluginsManager( QString pathToPlugins );

				/**
				*		Конструктор номер два. Сразу же составляет список плагинов, которые находятся по
				*	указанным путям, перечисленным в списке.
				*
				*	@param pathToPlugins список путей, по которым можно найти плагины
				*/
				PluginsManager( QStringList pathToPlugins );

				/**
				*		Конструктор номер три. Сразу содаёт все плагины по информации из XML
				*
				*	@param pathToXMl путь к XML-файлу
				*/
				PluginsManager( QString pathToXMl, QWidget* main);

			/**
			*	Просто деструктор.
			*
			*	@note загруженные плагины можно использовать и после того, как объект данного класса,
			*		загрузивший плагин, был уничтожен.
			*/
			virtual ~PluginsManager();

		private:
			/**
			*	@internal
			*/
			QDomDocument* pluginsDom;
			/**
			*	@internal
			*/
			QVector <PluginContainer* > pluginsContainer;

			/**
			*	@internal
			*/
			QStringList pathToFind;

			/**
			*	@internal
			*/
			QMap< QString, QString > findedPlugins;

			/**
			*	@internal
			*/
			Q_DISABLE_COPY( PluginsManager )

			/**
			*	@internal
			*/
			static bool isPlugin( QString pluginFile );

			/**
			*	@internal
			*/
			static QString getPluginName( QString pluginFile );

			/**
			*	@internal
			*/
			static QStringList getSupportedPluginInterfaces( QString pluginFile );

			/**
			*	@internal
			*/
			static QObject *createPluginInstance( QString pluginFile, PluginsManager *manager );
	};

} /* namespace plugin */

/**
*		Пытается преобразовать любой указатель на обьект, являющийся наследником QObject
*	в указатель на обьект типа QWidget. Если это сделать невозможно (обьект виджетом не является),
*	то функция возвращает 0.
*
*	@param o указатель на обьект, который может являться ещё и виджетом.
*			Должен быть как минимум прозводным классом от QObject, а, что
*			ещё лучше, от QWidget.
*
*	@return указатель на тот-же обьект, но уже как указатель на QWidget.
*			В случае, если преобразование не возможно, бужет возвращён 0.
*/
template < class T > inline QWidget* WIDGET( T *o )
{
	return qobject_cast< QWidget* >( o );
}

#endif /* __PLUGIN_H__ */
