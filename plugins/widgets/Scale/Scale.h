#ifndef __SCALE_H__
#define __SCALE_H__

#include "QDRuler.h"
#include <QWidget>
#include <QGridLayout>

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
            if(idParent == "Main")
            {
                mainWin = MAINWINDOW(parent);
                if(mainWin!=0)
                {
                    painter = PAINTWIDGETINTERFACE(mainWin->getPaintWidget());

                    // добавление линеек
                    //painter->setViewportMargins(RULER_BREADTH,RULER_BREADTH,0,0);
                    QGridLayout* gridLayout = new QGridLayout();
                    gridLayout->setSpacing(0);
                    gridLayout->setMargin(0);
                    QDRuler *mHorzRuler, *mVertRuler;
                    mHorzRuler = new QDRuler(QDRuler::Horizontal,painter);
                    mVertRuler = new QDRuler(QDRuler::Vertical,painter);
                    QWidget* fake = new QWidget();
                    fake->setBackgroundRole(QPalette::Window);
                    fake->setFixedSize(RULER_BREADTH,RULER_BREADTH);
                    gridLayout->addWidget(fake,0,0);
                    gridLayout->addWidget(mHorzRuler,0,1);
                    gridLayout->addWidget(mVertRuler,1,0);
                    gridLayout->addWidget(painter->viewport(),1,1);
                    painter->setLayout(gridLayout);

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
            return "Scale";
		}

        Scale( plugin::PluginsManager *manager )
        {

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
