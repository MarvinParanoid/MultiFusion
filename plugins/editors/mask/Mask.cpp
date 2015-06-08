#include "Mask.h"

void Mask::mask_slot(){
    int countSel = gsri->countSelected();
    GObjectInterface *iObj, *endObj=gsri->selected(gsri->countSelected()-1);//создаём контейнер с выделенными рамкой объектами
    qreal k_h, k_w, width_iObj, width_endObj = endObj->boundingRect().width(),
    height_iObj, height_endObj = endObj->boundingRect().height();
    QBrush endObjBrush = endObj->brush();
    if(endObjBrush.style() == Qt::RadialGradientPattern
            || endObjBrush.style() == Qt::ConicalGradientPattern
                || endObjBrush.style() == Qt::LinearGradientPattern){
        for (int i=0; i<countSel-1; i++){
            iObj = gsri->selected(i);
            QBrush inters = iObj->brush();
            qreal iObjAlpha = inters.color().alphaF();
            QGradient qg = *endObjBrush.gradient();
            height_iObj = iObj->boundingRect().height();
            width_iObj = iObj->boundingRect().width();
            k_h = height_endObj/height_iObj;//считаем коэффициенты пропорциональности фигур для умножения-->
            k_w = width_endObj/width_iObj;//-->на точки градиента(чтобы градиенты не зависели от размеров фигур).

            int d_x = iObj->boundingRect().x() - endObj->boundingRect().x();//вычисляем расстояния между левыми краями фигур
            int d_y = iObj->boundingRect().y() - endObj->boundingRect().y();
            qreal k_shift_x = ( d_x / width_endObj) * k_w,//вычисляем коэффициенты сдвига градиента по ширине-->
                    k_shift_y = (d_y / height_endObj) * k_h;//--> и по высотке

            QGradientStops grStops(qg.stops());//копируем точки/цвета задаваемого градиента.

            QColor BrushColor = inters.gradient()->stops()[0].second;

            for(int j=0; j<grStops.count(); j++){
                grStops[j].second.setRgbF( BrushColor.redF(), BrushColor.greenF(), BrushColor.blueF(),
                                           grStops[j].second.alphaF()*iObjAlpha);
            }

            if(qg.type() == QGradient::LinearGradient){
                QLinearGradient qlg = *static_cast<const QLinearGradient *>(endObjBrush.gradient());
                QLinearGradient qlgLinear(qlg.start().x() * k_w - k_shift_x, qlg.start().y() * k_h - k_shift_y,
                                         qlg.finalStop().x() * k_w - k_shift_x, qlg.finalStop().y() * k_h - k_shift_y);
                qlgLinear.setStops(grStops);
                iObj->setBrush(qlgLinear);
            }

            if(qg.type() == QGradient::RadialGradient){
                for(int j=0; j<grStops.count(); j++)
                    grStops[j].first *= k_w;
                QRadialGradient qlg = *static_cast<const QRadialGradient *>(endObjBrush.gradient());
                QRadialGradient qlgRadial(QPointF(qlg.center().x()*k_w - k_shift_x, qlg.center().y()*k_h - k_shift_y),
                     qlg.radius(), QPointF(qlg.focalPoint().x()*k_w - k_shift_x, qlg.focalPoint().y()*k_h - k_shift_y),
                     qlg.focalRadius());
                qlgRadial.setStops(grStops);
                iObj->setBrush(qlgRadial);
            }

            if(qg.type() == QGradient::ConicalGradient){
                QConicalGradient qlg = *static_cast<const QConicalGradient *>(endObjBrush.gradient());
                QConicalGradient qlgConical(QPointF(qlg.center().x()*k_w - k_shift_x, qlg.center().y()*k_h - k_shift_y),
                                          qlg.angle());
                qlgConical.setStops(grStops);
                iObj->setBrush(qlgConical);
            }
        }
        endObj->setVisible(false);
    }
    else{
        QMessageBox m;
        m.setText(tr("upper figure does not contain a gradient"));
        m.exec();
    }
    gsri->reset();

}

void Mask::createPlugin(QObject *parent, QString idParent,plugin::PluginsManager *manager)
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
            ActionMask = new QAction(tr("&Mask"), this);
            ActionMask->setEnabled(true);
            ObjectMenu->addAction( ActionMask );
            connect(ActionMask,SIGNAL( triggered( bool ) ),
                    this, SLOT( mask_slot() ) );
        }
    }
    connect( this, SIGNAL( setActive( QString ) ), getBeep(), SIGNAL( setActive( QString ) ) );
    /**/manager->addPlugins(this, "Mask");
}

QString Mask::getName() const
{
    return "Mask";
}

QGradient Mask::gradient() const
{
    //return range->gradient();
    return QGradient();
}

void Mask::setGradient( const QGradient &g){

}

Mask::Mask( plugin::PluginsManager *_manager ){
    //range = new GradientRangeEditor( this );
    //stops = new GradientStopsEditor( 0 );

}

Mask::~Mask(){
    //delete range;
    //delete stops;
}


EXPORT_PLUGIN( Mask, GradientEditorInterface )
