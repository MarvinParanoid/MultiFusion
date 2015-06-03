#include "Ruler.h"

EXPORT_QOBJECT_PLUGIN( Ruler )

Ruler::Ruler(plugin::PluginsManager *manager)
{
    scale = 1;
}

void Ruler::createPlugin(QObject *parent, QString idParent,plugin::PluginsManager *manager)
{
    if(idParent == "Main")
    {
        mainWin = MAINWINDOW(parent);
        if(mainWin!=0)
        {
            painter = PAINTWIDGETINTERFACE(mainWin->getPaintWidget());

            realPainter = RPWINTEFACE(painter->getRealPaintWidget());
            selection = GSRINTEFACE(realPainter->getSelection());
            connect(selection,SIGNAL(moved(qreal,qreal)),this,SLOT(moveSelection(qreal,qreal)));

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

            for(int i=0; i<W_COUNT; i++)
            {
                WayLine *w = new WayLine(painter->viewport());
                w->setVisible(false);
                waylines.append(w);
            }

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
    this->scale = scale;
    mHorzRuler->setRulerZoom(scale);
    mVertRuler->setRulerZoom(scale);
}

void Ruler::mouseMoveOrigin(QPoint origin)
{
    mHorzRuler->setOrigin(origin.x());
    mVertRuler->setOrigin(origin.y());

    // передвигаем направляющие, если сменились origin'ы или был зум
    for(int i=0; i<W_COUNT; i++)
    {
        if (waylines[i]->isVisible()==true)
        {
            if (waylines[i]->getType()==WayLine::Horizontal)
            {
                waylines[i]->setGeometry( 0, (waylines[i]->getCoord()) * scale + mVertRuler->origin(), waylines[i]->width(), 1 );
            }
            else
            {
                waylines[i]->setGeometry( (waylines[i]->getCoord()) * scale + mHorzRuler->origin(), 0, 1, waylines[i]->height() );
            }
        }
    }
}

void Ruler::mouseMoveCoords(QPoint origin, QPoint global, qreal scale)
{
    // здесь будем смотреть не зажата ли направляющая
    for (int i=0; i<W_COUNT; i++)
    {
        // если зажата, то передвигаем
        if (waylines[i]->getMousePress())
        {
            if (waylines[i]->getType()==WayLine::Vertical)
            {
                waylines[i]->setGeometry(global.x(),0,1,HEIGHT);
                waylines[i]->setCoord((global.x()-mHorzRuler->origin())/scale);
            }
            else
            {
                waylines[i]->setGeometry(0,global.y(),WIDTH,1);
                waylines[i]->setCoord((global.y()-mVertRuler->origin())/scale);
            }
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
        w->setType(WayLine::Vertical);
        w->setGeometry(point.x(),0,1,HEIGHT);
        w->setCoord((point.x()-mHorzRuler->origin())/scale);
    }
}

void Ruler::rulerClickedV(QPoint point)
{
    WayLine *w = getFreeWayline();
    if (w)
    {
        w->setVisible(true);
        w->setType(WayLine::Horizontal);//
        w->setGeometry(0,point.y(),WIDTH,1);
        w->setCoord((point.y()-mVertRuler->origin())/scale);
    }
}

// метод вызывается при перемещении рамки выделения
void Ruler::moveSelection(qreal dx, qreal dy)
{
    // прямоугольник выделения
    QRect sRect = selection->getPosition();

    // ищем есть ли рядом рамка к которой можно присосаться
    for (int i=0; i<W_COUNT; i++)
    {
        if (waylines[i]->isVisible())
        {
            if (waylines[i]->getType()==WayLine::Horizontal)
            {
                int y = waylines[i]->geometry().y() - mVertRuler->origin();
                int h = sRect.top()-y;
                if (h>0 && h<S_DIST)
                {
                    QRect t( sRect.x() , sRect.y()-h+1, sRect.width(), sRect.height() );
                    selection->setPosition(t);
                    return;
                }
                h = sRect.bottom()-y;
                if (h>-S_DIST && h<0)
                {
                    QRect t( sRect.x() , sRect.y()-h-1, sRect.width(), sRect.height() );
                    selection->setPosition(t);
                    return;
                }
            }
            else
            {
                int x = waylines[i]->geometry().x() - mHorzRuler->origin();
                int w = sRect.right()-x;
                if (w<0 && w>-S_DIST)
                {
                    QRect t( sRect.x()-w-1 , sRect.y(), sRect.width(), sRect.height() );
                    selection->setPosition(t);
                    return;
                }
                w = sRect.left()-x;
                if (w>0 && w<S_DIST)
                {
                    QRect t( sRect.x()-w+1 , sRect.y(), sRect.width(), sRect.height() );
                    selection->setPosition(t);
                    return;
                }
            }
        }
    }
}
