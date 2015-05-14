#ifndef __ALIGNANDDISTRIBUTE_H__
#define __ALIGNANDDISTRIBUTE_H__

#include <QDockWidget>

#include "./../../../pluginTool/Plugin.h"
#include "./../../../pluginTool/InterfacePlugin.h"
#include "./../../../interfaces/MainWindowInterface.h"
#include "./../../../paintWidget/GContainer.h"
#include "./../../../interfaces/GSRInterface.h"
#include "./../../../interfaces/RPWInterface.h"
#include "./../../../interfaces/PaintWidgetInterface.h"
#include "ui_AlignAndDistribute.h"

enum { LEFT, CENTERX, RIGHT, TOP, CENTERY, BOTTOM, GAPSX, GAPSY};

class AlignAndDistribute:public QWidget, public InterfacePlugin
{
    Q_OBJECT
    Q_INTERFACES( InterfacePlugin )

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

            alignSignalMapper = new QSignalMapper(this);
            distributeSignalMapper = new QSignalMapper(this);
            ui.setupUi( this );

            connect(alignSignalMapper, SIGNAL(mapped(int)),this, SLOT(align(int)));
            connect(distributeSignalMapper, SIGNAL(mapped(int)),this, SLOT(distribute(int)));

            alignSignalMapper->setMapping(ui.AlignButton_01, LEFT);
            connect(ui.AlignButton_01, SIGNAL(clicked()), alignSignalMapper, SLOT(map()));
            alignSignalMapper->setMapping(ui.AlignButton_02, CENTERX);
            connect(ui.AlignButton_02, SIGNAL(clicked()), alignSignalMapper, SLOT(map()));
            alignSignalMapper->setMapping(ui.AlignButton_03, RIGHT);
            connect(ui.AlignButton_03, SIGNAL(clicked()), alignSignalMapper, SLOT(map()));
            alignSignalMapper->setMapping(ui.AlignButton_04, TOP);
            connect(ui.AlignButton_04, SIGNAL(clicked()), alignSignalMapper, SLOT(map()));
            alignSignalMapper->setMapping(ui.AlignButton_05, CENTERY);
            connect(ui.AlignButton_05, SIGNAL(clicked()), alignSignalMapper, SLOT(map()));
            alignSignalMapper->setMapping(ui.AlignButton_06, BOTTOM);
            connect(ui.AlignButton_06, SIGNAL(clicked()), alignSignalMapper, SLOT(map()));

            distributeSignalMapper->setMapping(ui.DistributeButton_1, LEFT);
            connect(ui.DistributeButton_1, SIGNAL(clicked()), distributeSignalMapper, SLOT(map()));
            distributeSignalMapper->setMapping(ui.DistributeButton_2, CENTERX);
            connect(ui.DistributeButton_2, SIGNAL(clicked()), distributeSignalMapper, SLOT(map()));
            distributeSignalMapper->setMapping(ui.DistributeButton_3, RIGHT);
            connect(ui.DistributeButton_3, SIGNAL(clicked()), distributeSignalMapper, SLOT(map()));
            distributeSignalMapper->setMapping(ui.DistributeButton_5, TOP);
            connect(ui.DistributeButton_5, SIGNAL(clicked()), distributeSignalMapper, SLOT(map()));
            distributeSignalMapper->setMapping(ui.DistributeButton_6, CENTERY);
            connect(ui.DistributeButton_6, SIGNAL(clicked()), distributeSignalMapper, SLOT(map()));
            distributeSignalMapper->setMapping(ui.DistributeButton_7, BOTTOM);
            connect(ui.DistributeButton_7, SIGNAL(clicked()), distributeSignalMapper, SLOT(map()));

            distributeSignalMapper->setMapping(ui.DistributeButton_4, GAPSX);
            connect(ui.DistributeButton_4, SIGNAL(clicked()), distributeSignalMapper, SLOT(map()));
            distributeSignalMapper->setMapping(ui.DistributeButton_8, GAPSY);
            connect(ui.DistributeButton_8, SIGNAL(clicked()), distributeSignalMapper, SLOT(map()));
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
            int objectCount = container->countObjects();
            if(objectCount < 2) return;
            QRect rect = selection->getPosition();
            bool isX = true;
            qreal x = rect.left(),
                y = rect.top(),
                coeff = 0, // 0 - левая/верхняя сторона  |  0.5 - центр  |  1 - правая/нижняя сторона
                selectionW = rect.width(),
                selectionH = rect.height();
            switch(direction){
                case CENTERX:
                    coeff = 0.5; break;
                case RIGHT:
                    coeff = 1; break;
                case TOP:
                    isX = false; break;
                case CENTERY:
                    isX = false; coeff = 0.5; break;
                case BOTTOM:
                    isX = false; coeff = 1; break;
            }
            for(int i=0;i<objectCount;i++){
                GObject *obj = container->object(i);
                QRectF objectRect = obj->boundingRect();
                if(isX)
                    obj->move((x + selectionW * coeff) - (objectRect.x() + objectRect.width() * coeff) , 0);
                else
                    obj->move(0 , (y + selectionH * coeff) - (objectRect.y() + objectRect.height() * coeff));
            }
            // что бы рамка изменила свой размер имитируем отпускание кнопки мыши по ней
            selection->mouseRelease(Qt::LeftButton,rect.center(),Qt::NoModifier);
            // вызываем перерисовку и сохраняем в историю изменений
            selection->emitChanged();
            selection->emitStateChanged("Align");
        }

        void distribute(int direction){
            container = selection->getSelectedAsGContainer();
            int objectCount = container->countObjects();
            if(objectCount < 3) return;
            bool isX = true,
                isGaps = false;
            qreal coeff = 0, // 0 - левая/верхняя сторона  |  0.5 - центр  |  1 - правая/нижняя сторона
                sum = 0; // сумма ширин/высот объектов выделения
            QRect rect = selection->getPosition();
            switch(direction){
                case CENTERX:
                    coeff = 0.5; break;
                case RIGHT:
                    coeff = 1; break;
                case TOP:
                    isX = false; break;
                case CENTERY:
                    isX = false; coeff = 0.5; break;
                case BOTTOM:
                    isX = false; coeff = 1; break;
                case GAPSX:
                    isGaps = true; coeff = 0.5; sum = rect.width(); break;
                case GAPSY:
                    isGaps = true; isX = false; coeff = 0.5; sum = rect.height(); break;
            }

            // сделать массив индексов выделения и массив координат фигур, по которому будет сортировка
            int* indicesArray = new int[objectCount];
            qreal* figureSizes = new qreal[objectCount];
            for(int i=0;i<objectCount;i++){
                indicesArray[i] = i;

                QRectF objectRect = container->object(i)->boundingRect();
                if(isX){
                    figureSizes[i] = objectRect.x() + objectRect.width() * coeff;
                    sum -= objectRect.width();
                }else{
                    figureSizes[i] = objectRect.y() + objectRect.height() * coeff;
                    sum -= objectRect.height();
                }
            }

            // отсортировать индексы так, что бы фигуры, вызываемые из выделения по индексам, были по порядку
            for(int i=0;i<objectCount-1;i++){
                int min = i;
                for(int j=i;j<objectCount;j++)
                    if(figureSizes[ indicesArray[j] ] < figureSizes[ indicesArray[min] ])
                        min = j;
                if(min != i){
                    //swap index[i],index[min]
                    int tmp = indicesArray[i];
                    indicesArray[i] = indicesArray[min];
                    indicesArray[min] = tmp;
                }
            }

            // считаем расстояние между 1й и последней фигурами
            qreal distance;
            if( ! isGaps )
                distance = figureSizes[ indicesArray[objectCount-1] ]
                    - figureSizes[ indicesArray[0] ];
            else
                distance = sum;

            // делим расстояние на количество промежутков между фигурами
            qreal difference = distance / (objectCount-1);

            // по порядку расставляем объкты выделения
            if( ! isGaps )
                for(int i=1;i<objectCount-1;i++){
                    GObject *obj = container->object(indicesArray[i]);
                    if(isX)
                        obj->move(figureSizes[ indicesArray[0] ] + i*difference - figureSizes[ indicesArray[i] ],0);
                    else
                        obj->move(0,figureSizes[ indicesArray[0] ] + i*difference - figureSizes[ indicesArray[i] ]);
                }
            else{
                int prevObjIndex = 0;
                for(int i=1;i<objectCount-1;i++){
                    GObject *obj = container->object(indicesArray[i]);
                    if(isX)
                        obj->move(container->object(indicesArray[prevObjIndex])->boundingRect().right() + difference - obj->boundingRect().x(),0);
                    else
                        obj->move(0,container->object(indicesArray[prevObjIndex])->boundingRect().bottom() + difference - obj->boundingRect().y());
                    prevObjIndex = i;
                }
            }

            // что бы рамка изменила свой размер имитируем отпускание кнопки мыши по ней
            selection->mouseRelease(Qt::LeftButton,rect.center(),Qt::NoModifier);
            // вызываем перерисовку и сохраняем в историю изменений
            selection->emitChanged();
            selection->emitStateChanged("Distribute");
        }

    private:

		MainWindowInterface* mainWin;
        PaintWidgetInterface* painter;
        QDockWidget *AlignAndDistributeWindow;
        Ui::AlignAndDistributeForm ui;

        GSRInterface* selection;
        RPWInterface* realPainter;
        GContainer* container;

        QSignalMapper *alignSignalMapper;
        QSignalMapper *distributeSignalMapper;

};

#endif /* __AlignAndDistribute_H__ */
