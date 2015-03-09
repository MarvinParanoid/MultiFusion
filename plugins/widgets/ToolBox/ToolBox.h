#ifndef __TOOL_BOX_H__
#define __TOOL_BOX_H__

#include <QWidget>
#include <QDialog>
#include <QToolButton>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QDockWidget>
#include <QSpinBox>
#include <QMessageBox>

#include "./../../../pluginTool/Plugin.h"
#include "./../../../pluginTool/InterfacePlugin.h"
#include "./../../../interfaces/ToolBoxInterface.h"
#include "./../../../interfaces/MainWindowInterface.h"
#include "./../../../interfaces/PaintWidgetInterface.h"
#include "./../../../interfaces/FigureToolInterface.h"
#include "./../../../interfaces/GSRInterface.h"


class ToolBox: public ToolBoxInterface, public InterfacePlugin
{
	Q_OBJECT
	Q_INTERFACES( ToolBoxInterface )
	Q_INTERFACES( InterfacePlugin )

	private:
		static bool resourcesInited;
		QDockWidget* toolsWindow;
		QGridLayout* figureLayout;
		QVBoxLayout mainLayout;
		QSpinBox* posHideLines;
		QToolButton selectBtn;
		QToolButton addPointBtn;
		QToolButton zoomInBtn;
		QToolButton zoomOutBtn;
		QToolButton closedCheckBtn;

		GSRInterface* selection;
		MainWindowInterface* mainWin;
		PaintWidgetInterface* painter;

	signals:

		 /**
		 * Отправляется после выполнения события, которое нам нужно сохранить в истории
		 * @param event имя события
		 */
		void StateChanged(QString event);
		void selectionToolSelected();
		void addPointToolSelected();
		void zoomInToolSelected();
		void zoomOutToolSelected();
		void brushChanged( const QBrush &b );
		void penChanged( const QPen &p );
		void closedChanged( bool closed );
		void toolSelected( FigureToolInterface *tool );

		/**
		 * Сигнал, сообщающий о том, что данный инструмент был использован
		 */
		void setActive(QString name);

	public:

		virtual void createPlugin(QObject *parent, QString idParent,plugin::PluginsManager *manager)
		{
			if(idParent == "Main")
			{
				mainWin = MAINWINDOW(parent);
				if(mainWin!=0)
				{
					painter = PAINTWIDGETINTERFACE(mainWin->getPaintWidget());
					toolsWindow = new QDockWidget(mainWin);
					toolsWindow->setWindowTitle( tr( "Tools" ) );
					this->setParent( toolsWindow );
					toolsWindow->setWidget( this );
					toolsWindow->setFeatures(QDockWidget::DockWidgetVerticalTitleBar);
					toolsWindow->setAllowedAreas(Qt::LeftDockWidgetArea);
					mainWin->addDockWidget( Qt::LeftDockWidgetArea, toolsWindow );

					connect(painter,SIGNAL(figureSelected(int,int)),
							this,  SLOT( onStateChanged() ) );

					connect( &addPointBtn, SIGNAL( clicked( bool ) ),
							this, SLOT( onAddPointBtnClicked( bool ) ) );

					connect( &selectBtn, SIGNAL( clicked( bool ) ),
							this, SLOT( onSelectBtnClicked( bool ) ) );

					connect( &addPointBtn, SIGNAL( clicked( bool ) ),
							this, SLOT( onAddPointBtnClicked( bool ) ) );

					connect( &zoomInBtn, SIGNAL( clicked( bool ) ),
							mainWin, SLOT( onZoomInPressed()));

					connect( &zoomOutBtn, SIGNAL( clicked( bool ) ),
							mainWin, SLOT( onZoomOutPressed() ) );

					connect( &closedCheckBtn, SIGNAL( toggled ( bool ) ),
							this, SLOT( onClosedChanged( bool ) ) );


					connect( &addPointBtn, SIGNAL( clicked( bool ) ),
							this, SLOT( onTouched() ) );

					connect( &selectBtn, SIGNAL( clicked( bool ) ),
							this, SLOT( onTouched() ) );

					connect( &addPointBtn, SIGNAL( clicked( bool ) ),
							this, SLOT( onTouched() ) );

					connect( &zoomInBtn, SIGNAL( clicked( bool ) ),
							this, SLOT( onTouched() ) );

					connect( &zoomOutBtn, SIGNAL( clicked( bool ) ),
							this, SLOT( onTouched() ) );

					connect( &closedCheckBtn, SIGNAL( toggled ( bool ) ),
							this, SLOT( onTouched() ) );

					connect( this, SIGNAL( selectionToolSelected() ), mainWin, SLOT( onSelectionToolPressed() ) );
					connect( this, SIGNAL( addPointToolSelected() ), mainWin, SLOT( onAddPointToolPressed() ) );

					connect( this, SIGNAL( toolSelected( FigureToolInterface* ) ),
							mainWin, SLOT( onToolSelected( FigureToolInterface* ) ) );

					connect( painter, SIGNAL( isFrame( bool ) ),
							this, SLOT( onEnabled( bool) ) );

					/**
					 * Здесь мы соединяем сигнал, сообщающий о том, что инструмент использован,
					 * с сигналом класса SignalHolder.
					 */
					connect(this, SIGNAL(setActive(QString)), getBeep(), SIGNAL(setActive(QString)));

					manager->addPlugins(this, "ToolBox");
				}
			}
			else if(idParent == "Selection")
			{
				selection = GSRINTEFACE(parent);
			}
		}

		virtual QString getName()const
		{
			return "ToolBox";
		}

		virtual bool isClosedChecked() const
		{
			return closedCheckBtn.isChecked();
		}

		virtual void setClosedChecked( bool closed )
		{
			closedCheckBtn.setChecked(closed );
		}

		virtual QVBoxLayout& getMainLayout()
		{
			return mainLayout;
		}

		virtual QGridLayout* getFigureLayout()
		{
			return figureLayout;
		}

		ToolBox( plugin::PluginsManager * manager ):
			mainLayout( this ), selectBtn( this ),
			addPointBtn( this ), zoomInBtn( this ), zoomOutBtn( this ), closedCheckBtn(this)
		{
			if( !resourcesInited )
			{
				Q_INIT_RESOURCE( ToolBox );
				resourcesInited = true;
			}

			mainLayout.setContentsMargins(1,1,1,1);
			mainLayout.setAlignment(Qt::AlignTop);

			figureLayout = new QGridLayout;

			mainLayout.setSpacing(2);
			mainLayout.addLayout( figureLayout );

			figureLayout->addWidget( &selectBtn, 1, 1, Qt::AlignTop);
			figureLayout->addWidget( &addPointBtn, 1, 2, Qt::AlignTop);

			selectBtn.setToolTip( tr( "Selection" ) );
			selectBtn.setAutoRaise(true);
			addPointBtn.setToolTip( tr( "Add or delete point" ) );
			addPointBtn.setAutoRaise(true);

			QHBoxLayout* l = new QHBoxLayout;
			mainLayout.addLayout( l );

			l->addWidget( &zoomOutBtn);
			l->addWidget( &zoomInBtn);

			zoomInBtn.setToolTip( tr( "Zoom in frame" ) );
			zoomInBtn.setAutoRaise(true);
			zoomOutBtn.setToolTip( tr( "Zoom out frame" ) );
			zoomOutBtn.setAutoRaise(true);

			QHBoxLayout* hl = new QHBoxLayout;
			mainLayout.addLayout( hl );
			hl->addWidget( &closedCheckBtn );
			closedCheckBtn.setToolTip( tr( "Figure is closed" ) );
			closedCheckBtn.setAutoRaise(true);
			hl = new QHBoxLayout;

			mainLayout.addLayout( hl );
			posHideLines = new QSpinBox(this);
			posHideLines->setMaximum(9);
			posHideLines->setMinimum(0);
			posHideLines->setEnabled(false);
			connect( posHideLines, SIGNAL( valueChanged(int) ), this, SLOT( onHideLinesChanged(int) ) );
			hl->addWidget( posHideLines );

			selectBtn.setIcon( QIcon( ":/toolbox/images/selection.png" ) );
			addPointBtn.setIcon( QIcon( ":/toolbox/images/addpoint.png" ) );
			zoomInBtn.setIcon( QIcon( ":/toolbox/images/zoomin.png" ) );
			zoomOutBtn.setIcon( QIcon( ":/toolbox/images/zoomout.png" ) );
			closedCheckBtn.setIcon(QIcon( ":/toolbox/images/close_figure.png" ) );

			selectBtn.setAutoExclusive( true );
			addPointBtn.setAutoExclusive( true );
			selectBtn.setCheckable( true );
			closedCheckBtn.setCheckable( true );
			addPointBtn.setCheckable( true );
			selectBtn.setChecked( true );
		}

		virtual ~ToolBox(){};



	private slots:

		void onTouched() {
			emit setActive(getName());
		}

		void onSelectBtnClicked( bool checked )
		{
			if( !checked ) return;
			emit selectionToolSelected();
		}

		void onAddPointBtnClicked( bool checked )
		{
			if( !checked ) return;
			emit addPointToolSelected();
		}

		void onHideLinesChanged( int value )
		{
			if( !mainWin->getProcessSignals() ) return;

			if(selection->countSelected()!=0)
				painter->setFigureHideLines( value );
		}

		void onClosedChanged( bool closed )
		{
			if( !mainWin->getProcessSignals() ) return;

			//if(selection->countSelected()>0)
			{
				painter->setClosed( closed );
				setClosedChecked( painter->isClosed() );
			}
		}

		void onEnabled(bool value)
		{
			this->setEnabled(value);
		}

		void onStateChanged( )
		{
			if( !mainWin->getProcessSignals() ) return;
			int countObject = selection->countSelected();
			if(countObject == 1)
			{
				posHideLines->setEnabled(true);
				posHideLines->setValue(painter->getFigureHideLines());
			}
			else
			{
				posHideLines->setEnabled(false);
			}
			setClosedChecked( painter->isClosed() );
		}
};

#endif /* __TOOL_BOX_H__ */
