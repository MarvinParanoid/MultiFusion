#include "Ruler.h"

EXPORT_QOBJECT_PLUGIN( Ruler )

Ruler::Ruler(plugin::PluginsManager *manager)
{
}

void Ruler::createPlugin(QObject *parent, QString idParent,plugin::PluginsManager *manager)
{
    if(idParent == "Main")
    {
        mainWin = MAINWINDOW(parent);
        if(mainWin!=0)
        {
            painter = PAINTWIDGETINTERFACE(mainWin->getPaintWidget());

            // добавление линеек
            painter->mySetViewportMargins(RULER_BREADTH,RULER_BREADTH,0,0);
            QGridLayout* gridLayout = new QGridLayout();
            gridLayout->setSpacing(0);
            gridLayout->setMargin(0);
            mHorzRuler = new QDRuler(QDRuler::Horizontal,this);
            mVertRuler = new QDRuler(QDRuler::Vertical,this);
            QWidget* fake = new QWidget();
            fake->setBackgroundRole(QPalette::Window);
            fake->setFixedSize(RULER_BREADTH,RULER_BREADTH);
            gridLayout->addWidget(fake,0,0);
            gridLayout->addWidget(mHorzRuler,0,1);
            gridLayout->addWidget(mVertRuler,1,0);
            gridLayout->addWidget(painter->viewport(),1,1);
            painter->setLayout(gridLayout);

            //WayLine *w1 = new WayLine(WayLine::Horizontal,100,p);
            //w1 = new WayLine(WayLine::Horizontal,100,painter->viewport());
            //w1->setVisible(false);

            for(int i=0; i<W_COUNT; i++)
            {
                WayLine *w = new WayLine(painter->viewport());
                w->setVisible(false);
                waylines.append(w);
            }

            w1 = waylines[0];


            // сигналы из внешнего мира
            connect(painter,SIGNAL(mouseMoveEvent(QPoint,QPoint,qreal)),this,SLOT(mouseMoveCoords(QPoint,QPoint,qreal)));
            connect(painter,SIGNAL(paintEvent(QPoint)),this,SLOT(mouseMoveOrigin(QPoint)));
            connect(painter,SIGNAL(zoomEvent(qreal)),this,SLOT(zoomEvent(qreal)));

            // коннекты для направляющих
            connect(mHorzRuler,SIGNAL(rulerClick(QPoint)),this,SLOT(rulerClickedH(QPoint)));
            connect(mVertRuler,SIGNAL(rulerClick(QPoint)),this,SLOT(rulerClickedV(QPoint)));

            manager->addPlugins(this, "Scale");
        }
    }
}

QString Ruler::getName()const
{
    return "Ruler";
}

WayLine *Ruler::getFreeWayline()
{
    for(int i=0; i<W_COUNT; i++)
        if (waylines[i]->isVisible()==false)
            return waylines[i];
    return NULL;
}

void Ruler::zoomEvent(qreal scale)
{
    mHorzRuler->setRulerZoom(scale);
    mVertRuler->setRulerZoom(scale);
}

void Ruler::mouseMoveOrigin(QPoint origin)
{
    mHorzRuler->setOrigin(origin.x());
    mVertRuler->setOrigin(origin.y());
}

void Ruler::mouseMoveCoords(QPoint origin, QPoint global, qreal scale)
{
    // здесь будем смотреть не зажата ли направляющая
    for (int i=0; i<W_COUNT; i++)
    {
        // если зажата, то передвигаем
        if (waylines[i]->getMousePress())
        {
            if (waylines[i]->getType()==WayLine::Horizontal)
                waylines[i]->setGeometry(global.x(),0,1,painter->viewport()->height());
            else
                waylines[i]->setGeometry(0,global.y(),painter->viewport()->width(),1);
            break;
        }
    }


    mHorzRuler->setCursorPos(global);
    mVertRuler->setCursorPos(global);
}

void Ruler::rulerClickedH(QPoint point)
{
    WayLine *w = getFreeWayline();
    if (w)
    {
        w->setVisible(true);
        w->setType(WayLine::Horizontal);
        w->setGeometry(point.x(),0,1,painter->viewport()->height());
    }
}

void Ruler::rulerClickedV(QPoint point)
{
    WayLine *w = getFreeWayline();
    if (w)
    {
        w->setVisible(true);
        w->setType(WayLine::Vertical);
        w->setGeometry(0,point.y(),painter->viewport()->width(),1);
    }
}
