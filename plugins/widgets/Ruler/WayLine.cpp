#include "WayLine.h"

WayLine::WayLine(int coord, QWidget *parent) :
    QWidget(parent),
    coord(coord)
{
    setMouseTracking(true);
}

void WayLine::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPen pen(Qt::black,0);
    QRectF rulerRect = this->rect();
    painter.setPen(pen);
    painter.fillRect(rulerRect,QColor(236,233,216));

}

void WayLine::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);
}
