#ifndef __ALIGNANDDISTRIBUTE_H__
#define __ALIGNANDDISTRIBUTE_H__

#include <QDockWidget>

#include "./../../../pluginTool/Plugin.h"
#include "./../../../pluginTool/InterfacePlugin.h"
#include "./../../../interfaces/MainWindowInterface.h"
#include "./../../../interfaces/AlignAndDistributeInterface.h"
#include "./../../../paintWidget/GContainer.h"
#include "./../../../interfaces/GSRInterface.h"
#include "./../../../interfaces/RPWInterface.h"
#include "./../../../interfaces/PaintWidgetInterface.h"
#include "ui_AlignAndDistribute.h"

enum { LEFT, CENTERX, RIGHT, TOP, CENTERY, BOTTOM};

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
                realPainter = RPWINTEFACE(painter->getRealPaintWidget());
                selection = GSRINTEFACE(realPainter->getSelection());

                AlignAndDistributeWindow = new QDockWidget(mainWin);
                AlignAndDistributeWindow->setWindowTitle( tr( "Align And Distribute" ) );
                this->setParent( AlignAndDistributeWindow );
                AlignAndDistributeWindow->setWidget(this);
                mainWin->addDockWidget( Qt::RightDockWidgetArea, AlignAndDistributeWindow );

                QMenu* objectMenu = mainWin->getObjectMenu();
                QAction* showAlignAndDistribute = objectMenu->addAction(  tr( "&Align and Distribute..." ) );
                objectMenu->addSeparator();
                showAlignAndDistribute->setIcon( QIcon( ":/main/images/dialog-align-and-distribute.png" ) );
                connect( showAlignAndDistribute, SIGNAL( triggered( bool ) ), this, SLOT( showAlignAndDistribute() ) );

                manager->addPlugins(this, "Align And Distribute");
            }
		}

		virtual QString getName()const
		{
            return "Align And Distribute";
		}

        AlignAndDistribute( plugin::PluginsManager *manager )
        {

            signalMapper = new QSignalMapper(this);
            ui.setupUi( this );

            connect(signalMapper, SIGNAL(mapped(int)),this, SLOT(align(int)));

            signalMapper->setMapping(ui.AlignButton_01, LEFT);
            connect(ui.AlignButton_01, SIGNAL(clicked()), signalMapper, SLOT(map()));
            signalMapper->setMapping(ui.AlignButton_02, CENTERX);
            connect(ui.AlignButton_02, SIGNAL(clicked()), signalMapper, SLOT(map()));
            signalMapper->setMapping(ui.AlignButton_03, RIGHT);
            connect(ui.AlignButton_03, SIGNAL(clicked()), signalMapper, SLOT(map()));
            signalMapper->setMapping(ui.AlignButton_04, TOP);
            connect(ui.AlignButton_04, SIGNAL(clicked()), signalMapper, SLOT(map()));
            signalMapper->setMapping(ui.AlignButton_05, CENTERY);
            connect(ui.AlignButton_05, SIGNAL(clicked()), signalMapper, SLOT(map()));
            signalMapper->setMapping(ui.AlignButton_06, BOTTOM);
            connect(ui.AlignButton_06, SIGNAL(clicked()), signalMapper, SLOT(map()));
        }

        virtual ~AlignAndDistribute()
        {
        }


    public slots:

        void showAlignAndDistribute()
        {
            AlignAndDistributeWindow->show();
        }


    private slots:

        void align(int direction){
            container = selection->getSelectedAsGContainer();
            if(container->countObjects() == 0) return;
            QRect rect = selection->getPosition();
            qreal x = rect.left(),
                y = rect.top(),
                isX = 0, isY = 0,// по какой из осей идет выравнивание
                xCoeff = 0, yCoeff = 0, // по какой из по центру или по какой то из сторон ( 0 / 0,5 / 1)
                selectionW = rect.width(),
                selectionH = rect.height();
            switch(direction){
                case LEFT:
                    isX = 1;  break;
                case CENTERX:
                    isX = 1; xCoeff = 0.5; break;
                case RIGHT:
                    isX = 1; xCoeff = 1; break;
                case TOP:
                    isY = 1; break;
                case CENTERY:
                    isY = 1; yCoeff = 0.5; break;
                case BOTTOM:
                    isY = 1; yCoeff = 1; break;
            }
            for(int i=0;i<container->countObjects();i++){
                GObject *obj = container->object(i);
                obj->move(
                            isX * ( ( x + selectionW * xCoeff) - (obj->boundingRect().x() + obj->boundingRect().width() * xCoeff ) ),
                            isY * ( ( y + selectionH * yCoeff) - (obj->boundingRect().y() + obj->boundingRect().height() * yCoeff ) )
                         );
            }
            // что бы рамка изменила свой размер имитируем отпускание кнопки мыши по ней
            selection->mouseRelease(Qt::LeftButton,rect.center(),Qt::NoModifier);
            // вызываем перерисовку и сохраняем в историю изменений
            selection->emitChanged();
            selection->emitStateChanged("Align");
        }

    private:

		MainWindowInterface* mainWin;
        PaintWidgetInterface* painter;
        QDockWidget *AlignAndDistributeWindow;
        Ui::AlignAndDistributeForm ui;

        GSRInterface* selection;
        RPWInterface* realPainter;
        GContainer* container;

        QSignalMapper *signalMapper;

};

#endif /* __AlignAndDistribute_H__ */
