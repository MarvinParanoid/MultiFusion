#ifndef __ALIGNANDDISTRIBUTE_H__
#define __ALIGNANDDISTRIBUTE_H__

#include <QWidget>
#include <QFrame>

#include <QDockWidget>
#include <QMainWindow>
#include <QMenuBar>
#include <QLayout>
#include <QPainter>
#include <QToolButton>
#include <QMessageBox>
#include <QMouseEvent>
#include <QLabel>
#include <QPixmap>
#include <QSpinBox>
#include <QBitmap>
#include <QTimer>

#include "./../../../pluginTool/Plugin.h"
#include "./../../../pluginTool/InterfacePlugin.h"
#include "./../../../interfaces/AlignAndDistributeInterface.h"
#include "./../../../interfaces/MainWindowInterface.h"
#include "./../../../interfaces/PaintWidgetInterface.h"

class AlignAndDistribute:public QWidget, public AlignAndDistributeInterface, public InterfacePlugin
{
	Q_OBJECT
    Q_INTERFACES( AlignAndDistributeInterface )
	Q_INTERFACES( InterfacePlugin )

	signals:

	public:

		virtual void createPlugin(QObject *parent, QString idParent,plugin::PluginsManager *manager)
		{
			mainWin = MAINWINDOW(parent);
			if(mainWin!=0)
			{
				painter = PAINTWIDGETINTERFACE(mainWin->getPaintWidget());


                AlignAndDistributeWindow = new QDockWidget(mainWin);
                AlignAndDistributeWindow->setWindowTitle( tr( "AlignAndDistribute" ) );
                this->setParent( AlignAndDistributeWindow );
                AlignAndDistributeWindow->setWidget(this);
                mainWin->addDockWidget( Qt::BottomDockWidgetArea, AlignAndDistributeWindow );

                manager->addPlugins(this, "AlignAndDistribute");

                //QLabel *q = new QLabel("test");
                //QStatusBar *w = mainWin->statusBar();
                //w->addWidget(q);

			}

		}

		virtual QString getName()const
		{
            return "AlignAndDistribute";
		}

        AlignAndDistribute( plugin::PluginsManager *manager )
		{


		}

        virtual ~AlignAndDistribute()
		{
		}



	private slots:


	private:

		MainWindowInterface* mainWin;
		PaintWidgetInterface* painter;
		QFrame* fr;

        QDockWidget *AlignAndDistributeWindow;


};

#endif /* __AlignAndDistribute_H__ */
