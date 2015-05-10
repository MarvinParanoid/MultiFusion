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

            //WayLine *w1 = new WayLine(WayLine::Horizontal,100,p);
            w1 = new WayLine(WayLine::Horizontal,100,painter->viewport());
            w1->setVisible(false);

            painter->setLayout(gridLayout);

            // сигналы из внешнего мира
            connect(painter,SIGNAL(mouseMoveEvent(QPoint,QPoint,qreal)),this,SLOT(mouseMoveCoords(QPoint,QPoint,qreal)));
            connect(painter,SIGNAL(paintEvent(QPoint)),this,SLOT(mouseMoveOrigin(QPoint)));
            connect(painter,SIGNAL(zoomEvent(qreal)),this,SLOT(zoomEvent(qreal)));

            // коннекты для направляющих
            connect(mHorzRuler,SIGNAL(rulerClick(QPoint)),this,SLOT(rulerClicked(QPoint)));
            connect(mVertRuler,SIGNAL(rulerClick(QPoint)),this,SLOT(rulerClicked(QPoint)));

            manager->addPlugins(this, "Scale");
        }
    }
}

QString Ruler::getName()const
{
    return "Ruler";
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
    // здесь будем смотреть не выбрана ли направляющая
    if (w1->getMousePress())
        w1->setGeometry(global.x(),0,1,painter->viewport()->height());

    mHorzRuler->setCursorPos(global);
    mVertRuler->setCursorPos(global);
}

void Ruler::rulerClicked(QPoint point)
{

    w1->setVisible(true);
    w1->setGeometry(point.x(),0,1,painter->viewport()->height());

}

