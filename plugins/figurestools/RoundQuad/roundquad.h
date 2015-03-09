#ifndef __ROUNDQUAD_H__
#define __ROUNDQUAD_H__

#include "./../../../pluginTool/Plugin.h"
#include "./../../../pluginTool/InterfacePlugin.h"
#include "./../../../interfaces/FigureToolInterface.h"

class D_RoundQuad: public FigureToolInterface
{
	Q_OBJECT
	Q_INTERFACES( FigureToolInterface )

	public:
		virtual QWidget* getWidget() {
			return NULL;
		}

		virtual QIcon icon()
		{
			return QIcon( ":/roundquad/images/roundquad.png" );
		}

		virtual CreateStyle createStyle() const
		{
			return createAndResize;
		}

		virtual FiguresInfo figure() const
		{
			FigureInfo info;

			info.closed = true;
			info.spline = true;
			info.points << QPointF( 0.5, 0.0 ) << QPointF( 0.1, 0.0 ) << QPointF( 0.0, 0.1 ) <<
							QPointF( 0.0, 0.5 ) << QPointF( 0.0, 0.9 ) << QPointF( 0.1, 1.0 ) <<
							QPointF( 0.5, 1.0 ) << QPointF( 0.9, 1.0 ) << QPointF( 1.0, 0.9 ) <<
							QPointF( 1.0, 0.5 ) << QPointF( 1.0, 0.1 ) << QPointF( 0.9, 0.0 ) <<
							QPointF( 0.5, 0.0 );

			info.name = figureName();

			return ( FiguresInfo() << info );
		}

		virtual QString description() const
		{
			return tr( "Rounded quad" );
		}

		virtual QString figureName() const
		{
			return tr( "Rounded quad" );
		}

		virtual void toolSelected()
		{
		}

				D_RoundQuad( const plugin::PluginsManager *manager )
				{
					if( !resourcesInited )
					{
						Q_INIT_RESOURCE( roundquad );
						resourcesInited = true;
					}

				}

		virtual ~D_RoundQuad()
		{
		}

	private:
		static bool resourcesInited;
};

#endif /* __ROUNDQUAD_H__ */
