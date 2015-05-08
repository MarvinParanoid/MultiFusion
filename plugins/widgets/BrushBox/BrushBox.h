#ifndef BrushBox_H
#define BrushBox_H

#include <QWidget>
#include <QDialog>
#include <QPainter>
#include <QToolButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QCheckBox>
#include <QDialogButtonBox>
#include "qdrawutil.h"


#include "./../../../pluginTool/Plugin.h"
#include "./../../../pluginTool/InterfacePlugin.h"
#include "./../../../interfaces/BrushBoxInterface.h"
#include "./../../../interfaces/PaintWidgetInterface.h"
#include "./../../../interfaces/BrushEditorInterface.h"
#include "./../../../interfaces/MainWindowInterface.h"
#include "./../../../interfaces/ToolBoxInterface.h"
#include "./../../../interfaces/GSRInterface.h"

class BrushBox: public BrushBoxInterface, public InterfacePlugin
{
	Q_OBJECT
	Q_INTERFACES( BrushBoxInterface )
	Q_INTERFACES( InterfacePlugin )
	signals:
		void brushChanged( const QBrush &b );
		void setActive(QString);

	public slots:
		void setBrush( const QBrush &b )
		{
			if( b == _brush )
				return;

			_brush = b;
			update();
		}

		void onObjectCreated()
		{
			if( !mainWin->getProcessSignals() ) return;
			painter->setBrush( this->brush() );
		}

		void onStateChanged( )
		{
			if( !mainWin->getProcessSignals() ) return;

			setBrush( painter->brush());
		}
	public:

		virtual QBrush brush() const
		{
			return _brush;
		}

		virtual void createPlugin(QObject *parent, QString idParent,plugin::PluginsManager *manager)
		{

			if(idParent.contains("Main"))
			{
				mainWin = MAINWINDOW(parent);
				if(mainWin)
				{

					painter = PAINTWIDGETINTERFACE( mainWin->getPaintWidget() );

					connect(painter,SIGNAL(figureSelected(int,int)),
			        		this,  SLOT( onStateChanged() ) );

					connect( this, SIGNAL( brushChanged( const QBrush & ) ),
					 	mainWin, SLOT( onBrushChanged( const QBrush & ) ) );

					//connect( painter, SIGNAL( objectCreated() ),
						//	this, SLOT( onObjectCreated() ) );
				}
			}
			else if(idParent.contains("ToolBox"))
			{
				_manager = manager;
				brushEditor = manager->getPluginById("BrushEditor");
				ToolBoxInterface* tool = TOOLBOX(parent);
				if(tool!=0 && brushEditor!=0)
				{
					WIDGET( brushEditor )->setParent( &dialog );
					l->addWidget( WIDGET( brushEditor ) );
					this->setParent(tool);
					tool->connect( this, SIGNAL( brushChanged( const QBrush& ) ),
							tool, SIGNAL( brushChanged( const QBrush& ) ) );
					this->setToolTip( tr( "Current brush" ) );
					QHBoxLayout* hl = new QHBoxLayout;
					QVBoxLayout *mainLayout = &(tool->getMainLayout());
					mainLayout->addLayout(hl );
					hl->addWidget( this );


				}
			}
			else if(idParent.contains("Selection"))
			{
				selection = GSRINTEFACE(parent);
			}

			connect( this, SIGNAL( setActive( QString ) ), getBeep(), SIGNAL( setActive( QString ) ) );
			manager->addPlugins(this, "BrushBox");
		}

		virtual void mousePressEvent( QMouseEvent *event )
		{
			emit setActive(getName());
			selectBrush();
		}

		virtual QString getName()const
		{
			return "BrushBox";
		}

		BrushBox(plugin::PluginsManager *manager):BrushBoxInterface(),dialog( this )
		{
			l = new QVBoxLayout( &dialog );
			QPixmap pm( 20, 20 );

			QPainter pmp( &pm );

			pmp.fillRect( 0, 0, 10, 10, Qt::lightGray );
			pmp.fillRect( 10, 10, 10, 10, Qt::lightGray );
			pmp.fillRect( 0, 10, 10, 10, Qt::darkGray );
			pmp.fillRect( 10, 0, 10, 10, Qt::darkGray );
			pmp.end();

			QPalette pal = palette();
			pal.setBrush( backgroundRole(), QBrush( pm ) );
			setAutoFillBackground( true );
			setPalette( pal );

			setMinimumSize( QSize( 64, 64 ) );
			setMaximumSize( QSize( 64, 64 ) );
		}

		virtual ~BrushBox()
		{
		}

		virtual void paintEvent( QPaintEvent *event )
		{
			QPainter p( this );
			QRect r = rect();

			p.setRenderHints( QPainter::Antialiasing |
							  QPainter::TextAntialiasing |
							  QPainter::SmoothPixmapTransform );

			p.fillRect( rect(), prepareBrushToPaint( _brush, rect() ) );
			qDrawShadePanel( &p, 0, 0, width() + 1, height() + 1, palette(), true, 3 );

		}

	private:
		QObject* brushEditor; /// Указатель на плагин изменения кисти(BrushEditor)

		QDialog dialog; ///Диалог в который загружается плагин изменения кисти(BrushEditor)
		QVBoxLayout *l;

		GSRInterface* selection;
		MainWindowInterface* mainWin;/// Указатель на область рисования(PaintWidget)
		PaintWidgetInterface* painter;/// Указатель на панель инструментов(ToolBox)

		plugin::PluginsManager *_manager;
		QBrush _brush;

		void selectBrush()
		{
			SET_BRUSH( brushEditor, _brush );
			QDialogButtonBox buttons( QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
							Qt::Horizontal, &dialog );

			l->addWidget( &buttons );

			connect( &buttons, SIGNAL( accepted() ), &dialog, SLOT( accept() ) );
			connect( &buttons, SIGNAL( rejected() ), &dialog, SLOT( reject() ) );

			dialog.setWindowTitle( tr( "Selection of brush" ) );

			if( dialog.exec() != QDialog::Accepted )
				return;


				setBrush( GET_BRUSH( brushEditor ) );
			if(selection->countSelected()>0)
				emit brushChanged( _brush );
		}
};

#endif
