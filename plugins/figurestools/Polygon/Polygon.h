#ifndef __POLYGON_H__
#define __POLYGON_H__

#include <QInputDialog>
#include <QApplication>
#include <QMatrix>
#include <QLineF>

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

class F_Polygon:public FigureToolInterface, public InterfacePlugin
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

				QGridLayout* gridLayout= tool->getFigureLayout();
				button->setAutoExclusive( true );
				button->setCheckable( true );
				button->setAutoRaise(true);
				tool->connect( button, SIGNAL( toolSelected( FigureToolInterface* ) ),
						tool, SIGNAL( toolSelected( FigureToolInterface* ) ) );
				gridLayout->addWidget( button, 4, 2, Qt::AlignTop);
			}
			connect( this, SIGNAL( setActive( QString ) ), getBeep(), SIGNAL( setActive( QString ) ) );
			manager->addPlugins(this,"Polygon");
		}

		virtual QWidget* getWidget() {
			return button;
		}

		virtual QString getName()const
		{
			return "Polygon";
		}

		virtual QIcon icon()
		{
			return QIcon( ":/polygon/images/polygon.png" );
		}

		virtual CreateStyle createStyle() const
		{
			return createAndResize;
		}

		virtual FiguresInfo figure() const
		{
			FigureInfo info;

			info.closed = true;
			info.spline = false;

			qreal angle = 0.0;
			qreal dangle = 360.0 / qreal( countVertexes );
			for( int i = 0; i < countVertexes; i++ )
			{
				QLineF line = QLineF( 0.0, 0.0, 0.5, 0.0 ) * QMatrix().rotate( angle );
				line.translate( 0.5, 0.5 );

				if(i == 0 || i == countVertexes)
					info.points << line.p2()<< line.p2();
				else
					info.points << line.p2()<< line.p2()<< line.p2();

				angle += dangle;
			}

			info.name = figureName();

			return ( FiguresInfo() << info );
		}

		virtual QString description() const
		{
			return tr( "Polygon" );
		}

		virtual QString figureName() const
		{
			return tr( "Polygon" );
		}

		virtual void toolSelected()
		{
			emit setActive(getName());

			bool ok = false;
     		int value;

            value = QInputDialog::getInt( QApplication::activeWindow(),
								tr( "Input count vertexes:" ),
                                tr( "Count vertexes:" ), countVertexes, 3, 50, 1, &ok );

			if( !ok ) return;

			countVertexes = value;
		}

				F_Polygon( plugin::PluginsManager *manager ):
					countVertexes( 8 )
				{
					if( !resourcesInited )
					{
						Q_INIT_RESOURCE( Polygon );
						resourcesInited = true;
					}

				}

		virtual ~F_Polygon()
		{
		}

	private:
		static bool resourcesInited;
		ToolButton *button;
		int countVertexes;
};

#endif /* __POLYGON_H__ */
