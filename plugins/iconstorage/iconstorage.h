#ifndef __ICON_STORAGE_H__
#define __ICON_STORAGE_H__

#include <QObject>

#include "./../../pluginTool/Plugin.h"
#include "./../../pluginTool/InterfacePlugin.h"
#include "./../../interfaces/iconstorageinterface.h"

class IconStorage:public QObject, public IconStorageInterface
{
	Q_OBJECT
	Q_INTERFACES( IconStorageInterface )

	public:
		virtual QIcon icon( int index ) const
		{
			switch( index )
			{
				case appIcon:
					break;

				case fileNew:
					return QIcon( ":/iconstorage/images/new.png" );
					break;

				case fileOpen:
					return QIcon( ":/iconstorage/images/open.png" );
					break;

				case fileSave:
					return QIcon( ":/iconstorage/images/save.png" );
					break;

				case fileSaveAs:
					return QIcon( ":/iconstorage/images/save.png" );
					break;

				case fileExit:
					return QIcon( ":/iconstorage/images/exit.png" );
					break;


				case editDelete:
					return QIcon( ":/iconstorage/images/delete.png" );
					break;

				case editCut:
					return QIcon( ":/iconstorage/images/cut.png" );
					break;

				case editCopy:
					return QIcon( ":/iconstorage/images/copy.png" );
					break;

				case editPaste:
					return QIcon( ":/iconstorage/images/paste.png" );
					break;

				case editGroup:
					break;

				case editUngroup:
					break;


				case frameDeleteCurrent:
					break;

				case frameAddEmptyToBegin:
					break;

				case frameAddEmptyBefore:
					break;

				case frameAddEmptyAfter:
					break;

				case frameAddEmptyToEnd:
					break;


				case frameCloneBefore:
					break;

				case frameCloneAfter:
					break;

				case frameCloneTo:
					break;


				case toolsTools:
					break;

				case toolsTimeline:
					break;

				case toolsLayers:
					break;

				case toolsMainToolbar:
					break;


				case toolsConfig:
					break;


				case helpHelp:
					break;

				case helpAbout:
					break;

				case helpAboutQt:
					return QIcon( ":/iconstorage/images/aboutQt.png" );
					break;

				default:
					break;
			}

			return QIcon();
		}

				IconStorage( const plugin::PluginsManager *manager )
				{
					if( !resourcesInited )
					{
						Q_INIT_RESOURCE( iconstorage );
						resourcesInited = true;
					}
				}

		virtual ~IconStorage()
		{
		}

	private:
		static bool resourcesInited;
};

#endif /* __ICON_STORAGE_H__ */
