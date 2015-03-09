#ifndef __ALIGNANDDISTRIBUTE_H__
#define __ALIGNANDDISTRIBUTE_H__

#include <QDockWidget>

#include "./../../../pluginTool/Plugin.h"
#include "./../../../pluginTool/InterfacePlugin.h"
#include "./../../../interfaces/AlignAndDistributeInterface.h"
#include "./../../../interfaces/MainWindowInterface.h"
#include "./../../../interfaces/PaintWidgetInterface.h"
#include "ui_AlignAndDistribute.h"

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
                AlignAndDistributeWindow->setWindowTitle( tr( "Align And Distribute" ) );
                this->setParent( AlignAndDistributeWindow );
                AlignAndDistributeWindow->setWidget(this);
                mainWin->addDockWidget( Qt::RightDockWidgetArea, AlignAndDistributeWindow );

                manager->addPlugins(this, "Align And Distribute");
			}

		}

		virtual QString getName()const
		{
            return "Align And Distribute";
		}

        AlignAndDistribute( plugin::PluginsManager *manager )
        {
            ui.setupUi( this );
        }

        virtual ~AlignAndDistribute()
        {
		}


	private slots:

	private:

		MainWindowInterface* mainWin;
        PaintWidgetInterface* painter;
        QDockWidget *AlignAndDistributeWindow;
        Ui::AlignAndDistributeForm ui;

};

#endif /* __AlignAndDistribute_H__ */
