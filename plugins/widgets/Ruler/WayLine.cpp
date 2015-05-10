#include "WayLine.h"
#include <QDebug>

WayLine::WayLine(WayLine::WayLineType waylineType, int coord, QWidget *parent) :
    QWidget(parent), waylineType(waylineType), coord(coord), isMousePress(false)
{
    setMouseTracking(true);
}

void WayLine::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPen pen(Qt::black,0);
    QRectF rulerRect = this->rect();
    painter.setPen(pen);
    painter.fillRect(rulerRect,QColor("blue"));
}

void WayLine::mousePressEvent(QMouseEvent *event)
{
    isMousePress = true;
    //QWidget::mousePressEvent(event);
}

void WayLine::mouseReleaseEvent(QMouseEvent *event)
{
    isMousePress = false;
    //QWidget::mouseReleaseEvent(event);
}

void WayLine::mouseMoveEvent(QMouseEvent *event)
{
    setCursor(Qt::PointingHandCursor);
    if(isMousePress)
    {
        emit moveWayline(event->pos());
    }

    QWidget::mouseMoveEvent(event);

}
