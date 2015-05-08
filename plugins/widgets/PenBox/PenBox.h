/**
* PenBox.h
*
*  Created on: 04.05.2009
*      Author: Alexey Kukushin
*/

#ifndef PENBOX_H_
#define PENBOX_H_

#include <QtGui>
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
#include "./../../../interfaces/PenBoxInterface.h"
#include "./../../../interfaces/MainWindowInterface.h"
#include "./../../../interfaces/ToolBoxInterface.h"
#include "./../../../interfaces/PaintWidgetInterface.h"
#include "./../../../interfaces/PenEditorInterface.h"
#include "./../../../interfaces/BrushEditorInterface.h"
#include "./../../../interfaces/GSRInterface.h"

class PenBox:public PenBoxInterface, public InterfacePlugin
{
	Q_OBJECT
	Q_INTERFACES( PenBoxInterface )
	Q_INTERFACES( InterfacePlugin )

	signals:
		void penChanged( const QPen &p );
		void setActive(QString);

	public slots:
		void setPen( const QPen &p )
		{
			if( p == _pen )
				return;

			_pen = p;
			update();

		}

		void onObjectCreated()
		{
			if( !mainWin->getProcessSignals() ) return;

			painter->setPen( this->pen() );
		}

		void onStateChanged( )
		{
			if( !mainWin->getProcessSignals() ) return;

			setPen( painter->pen());
		}

	public:

		virtual void createPlugin(QObject *parent, QString idParent,plugin::PluginsManager *manager)
		{
			connect( this, SIGNAL( setActive( QString ) ), getBeep(), SIGNAL( setActive( QString ) ) );
			if(idParent.contains("Main"))
			{

					mainWin = MAINWINDOW(parent);
					if(mainWin!=0)
					{
						painter = PAINTWIDGETINTERFACE( mainWin->getPaintWidget() );

						connect( this, SIGNAL( penChanged( const QPen& ) ),
								mainWin, SLOT( onPenChanged( const QPen& ) ) );

						connect( painter, SIGNAL( objectCreated() ),
								this, SLOT( onObjectCreated() ) );

				        connect(painter,SIGNAL(figureSelected(int,int)),
				        		this,  SLOT( onStateChanged() ) );
					}
			}
			else if(idParent.contains("ToolBox"))
			{
					_manager = manager;
					penEditor = manager->getPluginById("PenEditor");
					ToolBoxInterface* tool = TOOLBOX(parent);
					if(tool!=0 && penEditor!=0)
					{
						WIDGET( penEditor )->setParent( &dialog );
						l->addWidget( WIDGET( penEditor ) );
						this->setParent(tool);

						tool->connect( this, SIGNAL( penChanged( const QPen& ) ),
								tool, SIGNAL( penChanged( const QPen& ) ) );

						this->setToolTip( tr( "Current pen" ) );
						QHBoxLayout* hl = new QHBoxLayout;
						QVBoxLayout *mainLayout = &(tool->getMainLayout());
						mainLayout->addLayout(hl );
						hl->addWidget( this );
						manager->addPlugins(this, "PenBox");

					}
			}
			else if(idParent.contains("Selection"))
			{
				selection = GSRINTEFACE(parent);
			}

		}

		virtual QString getName()const
		{
			return "PenBox";
		}

		virtual void mousePressEvent( QMouseEvent *event )
		{
			selectPen();
		}

		QPen pen() const
		{
			return _pen;
		}

		PenBox( plugin::PluginsManager * manager ):PenBoxInterface(),dialog( this )
		{
			l = new QVBoxLayout( &dialog );

			QPixmap pm( 64, 64 );

			QPainter pmp( &pm );

			pmp.fillRect( 0, 0, 64, 64, Qt::white );
			pmp.end();

			QPalette pal = palette();
			pal.setBrush( backgroundRole(), QBrush( pm ) );
			setAutoFillBackground( true );
			setPalette( pal );

			setMinimumSize( QSize( 64, 64 ) );
			setMaximumSize( QSize( 64, 64 ) );
		}

		virtual ~PenBox()
		{
		}

		virtual void paintEvent( QPaintEvent *event )
		{
			QPainter p( this );
			QRect r = rect();

			p.setRenderHints( QPainter::Antialiasing |
							  QPainter::TextAntialiasing |
							  QPainter::SmoothPixmapTransform );

			p.fillRect( rect(), prepareBrushToPaint( QBrush(Qt::white), rect() ) );
			qDrawShadePanel( &p, 0, 0, width() + 1, height() + 1, palette(), true, 3 );


			p.setPen( _pen );

			QPoint points[] =
			{
				QPoint( 0, 32 ),
				QPoint( width() , 32 ),

			};

			p.setClipRect( 3, 3, width() - 6, height() - 6 );
			p.drawPolyline( points, 2 );
		}


	private:
		QPen _pen; /// Перо
		QObject* penEditor; /// Указатель на плагин изменения пера(PenEditor)

		QDialog dialog; ///Диалог в который загружается плагин изменения пера(PenEditor)
		QVBoxLayout *l;

		GSRInterface* selection;
		MainWindowInterface* mainWin;/// Указатель на область рисования(PaintWidget)
		PaintWidgetInterface* painter;/// Указатель на панель инструментов(ToolBox)
		plugin::PluginsManager *_manager;

		void selectPen()
		{
			emit setActive(getName());

			SET_PEN( penEditor, _pen );

			QDialogButtonBox buttons( QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
							Qt::Horizontal, &dialog );
			l->addWidget( &buttons );

			connect( &buttons, SIGNAL( accepted() ), &dialog, SLOT( accept() ) );
			connect( &buttons, SIGNAL( rejected() ), &dialog, SLOT( reject() ) );

			dialog.setWindowTitle( tr( "Selection of pen" ) );

			if( dialog.exec() != QDialog::Accepted )
				return;

			setPen( GET_PEN( penEditor ) );
			if(selection->countSelected()>0)
				emit penChanged( _pen );

		}
};


#endif
