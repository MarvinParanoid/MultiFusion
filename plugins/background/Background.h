#ifndef __BACKGROUND_H__
#define __BACKGROUND_H__

#include <QWidget>
#include <QObject>
#include <QBrush>
#include <QMainWindow>
#include <QMessageBox>
#include <QGLWidget>
#include "./../../interfaces/BackgroundInterface.h"
#include "./../../interfaces/BrushEditorInterface.h"
#include "./../../interfaces/PaintWidgetInterface.h"
#include "./../../interfaces/RPWInterface.h"
#include "./../../pluginTool/InterfacePlugin.h"
#include "./../../pluginTool/Plugin.h"

class Background:public QObject, public BackgroundInterface, public InterfacePlugin
{
	Q_OBJECT
	Q_INTERFACES( BackgroundInterface )
	Q_INTERFACES( InterfacePlugin )

	signals:

		void changed();

		void setActive(QString);

		void brushChanged( const QBrush &b );

	public:

		virtual QWidget *editor()
		{
			if(BrushEditor !=0)
				return BrushEditor ;
			return 0;
		}

		virtual void reset()
		{
			brush = QColor( 255, 255 ,255 );
			emit brushChanged( brush );
			emit changed();
		}

		virtual void save( QDataStream &stream ) const
		{
			stream << brush;
		}

		virtual void load( QDataStream &stream )
		{
			stream >> brush;
			emit brushChanged( brush );
			emit changed();
		}

		virtual QObject* copyBackground()
		{
			Background* back = new Background(_manager);
			back->brush = brush;
			return back;
		}

		virtual void fill( QPainter &p, QRect rect ) const
		{
			p.fillRect( rect, prepareBrushToPaint( brush, rect ) );
		}

		Background( plugin::PluginsManager *manager ):
					QObject( 0 ), brush( QColor( 255, 255 ,255 ) ), _manager( manager )
		{
		}

		virtual ~Background()
		{
		}

		virtual void createPlugin(QObject *parent, QString idParent, plugin::PluginsManager *manager)
		{
			if( PAINTWIDGETINTERFACE( parent ) != 0 )
			{
				QObject* editor =  manager->copy("BrushEditor");

				if(editor!=0)
				{
					SET_BRUSH( editor, brush );
					BrushEditor = BRUSHEDITOR(editor);

					connect( BrushEditor, SIGNAL( brushChanged( const QBrush& ) ),
						this, SLOT( onBrushChanged( const QBrush& ) ) );

					connect( this, SIGNAL( brushChanged( const QBrush& ) ),
							 BrushEditor, SLOT( setBrush( const QBrush& ) ) );
				}

				connect( this, SIGNAL( setActive( QString ) ), getBeep(), SIGNAL( setActive( QString ) ) );

				manager->addPlugins(this, "Background");
				paintWidget = PAINTWIDGETINTERFACE( parent );
				RPWInterface* realPainter = RPWINTEFACE(paintWidget->getRealPaintWidget());
				realPainter->setBackground(this);
				paintWidget->connect( this, SIGNAL( changed() ),
							realPainter, SLOT( update() ) );

				//QMessageBox::about(paintWidget, "Background", QString::number((int)(background)));
			}

		}

		virtual QString getName()const
		{
			return "Background";
		}

	private slots:

		void onBrushChanged( const QBrush &b )
		{
			if( b == brush ) return;
			brush = b;
			emit brushChanged( brush );
			emit changed();
		}

	private:
		QBrush brush;
		BrushEditorInterface* BrushEditor;
		PaintWidgetInterface * paintWidget;
		plugin::PluginsManager *_manager;
};

#endif /* __BACKGROUND_H__ */
