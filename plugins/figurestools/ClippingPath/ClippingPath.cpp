#include "ClippingPath.h"

bool J_ClippingPath::resourcesInited = false;

void J_ClippingPath::clip(){
    GVectorFigure *endObj=(GVectorFigure*)(gsri->selected(gsri->countSelected()-1));//создаём контейнер с выделенными рамкой объектами

    for (int i=0; i<gsri->countSelected(); i++)
        J_ClippingPath::cutFigure((GVectorFigure*)gsri->selected(i), endObj);

    endObj->setVisible(false);
    gsri->reset();

}

void J_ClippingPath::cutFigure(GVectorFigure *iObj, GVectorFigure *endObj){
    QPolygonF pp = iObj->points(iObj->frame());
    QPolygonF pp2 = endObj->points(endObj->frame());
    int qr = iObj->frame();
    QPainterPath pone,ptwo;
    pone.moveTo( pp[0] );
    ptwo.moveTo( pp2[0] );
    int countSplinePointsOne = ( ( pp.size() - 1 ) / 3 ) * 3;
    int countSplinePointsTwo = ( ( pp2.size() - 1 ) / 3 ) * 3;
    for( int i = 1; i < ( countSplinePointsOne + 1 ); i += 3 )
        pone.cubicTo(pp[i], pp[i + 1], pp[i + 2] );
    if(iObj->isClosed())
        pone.closeSubpath();
    for( int i = 1; i < ( countSplinePointsTwo + 1 ); i += 3 )
        ptwo.cubicTo(pp2[i], pp2[i + 1], pp2[i + 2] );
    if(endObj->isClosed())
        ptwo.closeSubpath();
    QPainterPath unioin=pone.intersected(ptwo);
    pone=unioin;
    QMatrix qm;
    QPolygonF pmain=pone.toFillPolygon(qm);
    QPolygonF newMain;
    int countPoints=pmain.size();
    for( int i = 0; i < countPoints; i++ )
    {
        QPointF curPoint=pmain[i];
        if(i == 0 || i == (countPoints-1))
            newMain << curPoint<< curPoint;
        else
            newMain << curPoint<< curPoint<< curPoint;
    }
    iObj->setPoints(newMain, qr);
}

void J_ClippingPath::createPlugin(QObject *parent, QString idParent,
        plugin::PluginsManager *manager)
{
    if(idParent == "Main")
    {
        mainWin = MAINWINDOW(parent);
        if(mainWin!=0)
        {
            painter = PAINTWIDGETINTERFACE(mainWin->getPaintWidget());
            rpwi = RPWINTEFACE(painter->getRealPaintWidget());
            gsri = GSRINTEFACE(rpwi->getSelection());
            QMenu* ObjectMenu = mainWin->getObjectMenu();
            ActionClippingPath = new QAction(tr("&Clip"), this);
            ActionClippingPath->setEnabled(true);
            ObjectMenu->addAction( ActionClippingPath );
            connect(ActionClippingPath,SIGNAL( triggered( bool ) ),
                    this, SLOT( clip() ) );
        }
    }

    connect( this, SIGNAL( setActive( QString ) ), getBeep(), SIGNAL( setActive( QString ) ) );

    manager->addPlugins(this,"ClippingPath");



}

QWidget* J_ClippingPath::getWidget() {
    return 0;
}

QString J_ClippingPath::getName()const
{
    return "ClippingPath";
}

QIcon J_ClippingPath::icon()
{
    return QIcon( ":/clippingpath/images/clippingpath.png" );
}

FigureToolInterface::CreateStyle J_ClippingPath::createStyle() const
{
    return createAndResize;
}

FigureToolInterface::FiguresInfo J_ClippingPath::figure() const
{
    FigureInfo info;
    return ( FiguresInfo() << info );
}

QString J_ClippingPath::description() const
{
    return tr( "ClippingPath" );
}

QString J_ClippingPath::figureName() const
{
    return tr( "ClippingPath" );
}

void J_ClippingPath::toolSelected()
{
    emit setActive(getName());
}

J_ClippingPath::J_ClippingPath(const plugin::PluginsManager *manager )
{
    if( !resourcesInited )
    {
        Q_INIT_RESOURCE( ClippingPath );
        resourcesInited = true;
    }

}

J_ClippingPath::~J_ClippingPath()
{
}

EXPORT_PLUGIN( J_ClippingPath, FigureToolInterface )


