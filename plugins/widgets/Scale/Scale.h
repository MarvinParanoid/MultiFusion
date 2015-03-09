#ifndef __SCALE_H__
#define __SCALE_H__

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
#include "./../../../interfaces/ScaleInterface.h"
#include "./../../../interfaces/MainWindowInterface.h"
#include "./../../../interfaces/PaintWidgetInterface.h"


class Scale:public QWidget, public ScaleInterface, public InterfacePlugin
{
	Q_OBJECT
    Q_INTERFACES( ScaleInterface )
	Q_INTERFACES( InterfacePlugin )

	signals:

	public:

		virtual void createPlugin(QObject *parent, QString idParent,plugin::PluginsManager *manager)
		{
			mainWin = MAINWINDOW(parent);
			if(mainWin!=0)
			{
				painter = PAINTWIDGETINTERFACE(mainWin->getPaintWidget());


                scaleWindow = new QDockWidget(mainWin);
                scaleWindow->setWindowTitle( tr( "Scale" ) );
                this->setParent( scaleWindow );
                scaleWindow->setWidget(this);
                mainWin->addDockWidget( Qt::BottomDockWidgetArea, scaleWindow );

                manager->addPlugins(this, "Scale");
			}

		}

		virtual QString getName()const
		{
            return "Scale";
		}

        Scale( plugin::PluginsManager *manager )
		{

            //fr = new QFrame(this);
            //fr->setMinimumWidth(100);
            //fr->setMinimumHeight(40);


		}

        virtual ~Scale()
		{
		}



	private slots:


	private:

		MainWindowInterface* mainWin;
		PaintWidgetInterface* painter;
		QFrame* fr;

        QDockWidget *scaleWindow;


};

#endif /* __SCALE_H__ */
