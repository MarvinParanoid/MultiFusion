#ifndef __RULER_H__
#define __RULER_H__

#include "QDRuler.h"
#include <QWidget>
#include <QGridLayout>
#include <QGLWidget>
#include <QDebug>

#include "./../../../pluginTool/Plugin.h"
#include "./../../../pluginTool/InterfacePlugin.h"
#include "./../../../interfaces/RulerInterface.h"
#include "./../../../interfaces/MainWindowInterface.h"
#include "./../../../interfaces/PaintWidgetInterface.h"
#include "./../../../interfaces/RPWInterface.h"


class Ruler:public QWidget, public RulerInterface, public InterfacePlugin
{
	Q_OBJECT
    Q_INTERFACES( RulerInterface )
	Q_INTERFACES( InterfacePlugin )

	signals:

	public:

		virtual void createPlugin(QObject *parent, QString idParent,plugin::PluginsManager *manager)
		{
            if(idParent == "Main")
            {
                mainWin = MAINWINDOW(parent);
                if(mainWin!=0)
                {
                    painter = PAINTWIDGETINTERFACE(mainWin->getPaintWidget());
                    realPainter = RPWINTEFACE(painter->getRealPaintWidget());

                    // добавление линеек
                    painter->mySetViewportMargins(RULER_BREADTH,RULER_BREADTH,0,0);
                    QGridLayout* gridLayout = new QGridLayout();
                    gridLayout->setSpacing(0);
                    gridLayout->setMargin(0);
                    mHorzRuler = new QDRuler(QDRuler::Horizontal,painter);
                    mVertRuler = new QDRuler(QDRuler::Vertical,painter);
                    QWidget* fake = new QWidget();
                    fake->setBackgroundRole(QPalette::Window);
                    fake->setFixedSize(RULER_BREADTH,RULER_BREADTH);
                    gridLayout->addWidget(fake,0,0);
                    gridLayout->addWidget(mHorzRuler,0,1);
                    gridLayout->addWidget(mVertRuler,1,0);
                    //gridLayout->addWidget(painter->viewport(),1,1);
                    painter->setLayout(gridLayout);

                    connect(realPainter,SIGNAL(mouseMoveEvent(QPoint,QPoint)),this,SLOT(mouseMoveOrigin(QPoint,QPoint)));
                    connect(painter,SIGNAL(mouseMoveEvent(QPoint)),this,SLOT(mouseMoveCoords(QPoint)));

                    manager->addPlugins(this, "Scale");
                }
            }

//			mainWin = MAINWINDOW(parent);
//			if(mainWin!=0)
//			{
//				painter = PAINTWIDGETINTERFACE(mainWin->getPaintWidget());
//                scaleWindow = new QDockWidget(mainWin);
//                scaleWindow->setWindowTitle( tr( "Scale" ) );
//                this->setParent( scaleWindow );
//                scaleWindow->setWidget(this);
//                mainWin->addDockWidget( Qt::BottomDockWidgetArea, scaleWindow );
//                manager->addPlugins(this, "Scale");
//			}


		}

		virtual QString getName()const
		{
            return "Ruler";
		}

        Ruler( plugin::PluginsManager *manager )
        {

        }

        virtual ~Ruler()
		{
		}



	private slots:

        void mouseMoveOrigin(QPoint global, QPoint rpw)
        {
            QPoint p = (global-rpw);
            mVertRuler->setOrigin(p.y());
            mHorzRuler->setOrigin(p.x());
            mHorzRuler->setCursorPos(global);
            mVertRuler->setCursorPos(global);
        }

        void mouseMoveCoords(QPoint global)
        {
            mHorzRuler->setCursorPos(global);
            mVertRuler->setCursorPos(global);
        }

	private:

		MainWindowInterface* mainWin;
		PaintWidgetInterface* painter;
        RPWInterface* realPainter;
        QDRuler *mHorzRuler, *mVertRuler;


};

#endif /* __SCALE_H__ */
