#ifndef __CIRCLE_H__
#define __CIRCLE_H__

#include <QHBoxLayout>
#include <QVBoxLayout>
#include "./../../../pluginTool/Plugin.h"
#include "./../../../pluginTool/InterfacePlugin.h"
#include "./../../../interfaces/ToolBoxInterface.h"
#include "./../../../interfaces/FigureToolInterface.h"

#include <QToolButton>
#include <QWidget>
#include <QObject>
#include "./../../../interfaces/ToolButtonInterface.h"

class ToolButton:public ToolButtonInterface
{
	Q_OBJECT
	Q_INTERFACES( ToolButtonInterface )

	signals:
		void toolSelected( FigureToolInterface *tool );

	public:
		QObject* getTool()
		{
			return tool;
		}

		void setTool(QObject* t)
		{
			tool = FIGURETOOL(t);
		}

		ToolButton( FigureToolInterface *_tool, QWidget *parent ):
			ToolButtonInterface( parent ), tool( _tool )
		{
			setIcon( tool->icon() );
			setToolTip( tool->description() );

			connect( this, SIGNAL( clicked( bool ) ),
					this, SLOT( onClicked( bool ) ) );
		}

		virtual ~ToolButton()
		{
			delete tool;
		}

	private slots:
		void onClicked( bool checked )
		{
			if( !checked ) return;

			tool->toolSelected();
			emit toolSelected( tool );
		}

	private:
		FigureToolInterface *tool;
};

class C_Circle:public FigureToolInterface, public InterfacePlugin
{
	Q_OBJECT
	Q_INTERFACES( FigureToolInterface )
	Q_INTERFACES( InterfacePlugin )

	signals:
		void setActive(QString);

	public:

		virtual void createPlugin(QObject *parent, QString idParent,
				plugin::PluginsManager *manager)
		{
			ToolBoxInterface* tool = TOOLBOX(parent);
			if(tool!=0)
			{
				button = new ToolButton( FIGURETOOL(this), tool);

				QGridLayout* gridLayout = tool->getFigureLayout();

				button->setAutoExclusive( true );
				button->setCheckable( true );
				button->setAutoRaise(true);

				tool->connect( button, SIGNAL( toolSelected( FigureToolInterface* ) ),
						tool, SIGNAL( toolSelected( FigureToolInterface* ) ) );
				gridLayout->addWidget( button, 3, 2, Qt::AlignTop);
			}

			connect( this, SIGNAL( setActive( QString ) ), getBeep(), SIGNAL( setActive( QString ) ) );

			manager->addPlugins(this,"Circle");
		}

		virtual QWidget* getWidget() {
			return button;
		}

		virtual QString getName()const
		{
			return "Circle";
		}

		virtual QIcon icon()
		{
			return QIcon( ":/circle/images/circle.png" );
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
			info.points 		    << QPointF( 0.5, 0.0 ) << QPointF( 0.21, 0.0 )
		    << QPointF( 0.0, 0.21 ) << QPointF( 0.0, 0.5 ) << QPointF( 0.0, 0.79 )
			<< QPointF( 0.21, 1.0 ) << QPointF( 0.5, 1.0 ) << QPointF( 0.79, 1.0 )
			<< QPointF( 1.0, 0.79 ) << QPointF( 1.0, 0.5 ) << QPointF( 1.0, 0.21 )
			<< QPointF( 0.79, 0.0 ) << QPointF( 0.5, 0.0 );

			info.name = figureName();

			return ( FiguresInfo() << info );
		}

		virtual QString description() const
		{
			return tr( "Circle" );
		}

		virtual QString figureName() const
		{
			return tr( "Circle" );
		}

		virtual void toolSelected()
		{
			emit setActive(getName());
		}

		C_Circle( const plugin::PluginsManager *manager )
		{
			if( !resourcesInited )
			{
				Q_INIT_RESOURCE( Circle );
				resourcesInited = true;
			}

		}

		virtual ~C_Circle()
		{
		}

	private:
		ToolButton *button;
		static bool resourcesInited;
};

#endif /* __CIRCLE_H__ */
