#include "WayLine.h"
#include <QDebug>

WayLine::WayLine(QWidget *parent) :
    QWidget(parent), isMousePress(false)
{
    setMouseTracking(true);
}

void WayLine::setType(WayLine::WayLineType w_type)
{
    waylineType = w_type;
}

WayLine::WayLineType WayLine::getType()
{
    return waylineType;
}

void WayLine::setMousePress(bool b)
{
    isMousePress = b;
}

int WayLine::getCoord()
{
    return coord;
}

void WayLine::setCoord(int c)
{
    coord = c;
}

bool WayLine::getMousePress()
{
    return isMousePress;
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
}

void WayLine::mouseReleaseEvent(QMouseEvent *event)
{
    isMousePress = false;
}

void WayLine::mouseMoveEvent(QMouseEvent *event)
{
    if ( (waylineType == Vertical && event->y()<0) || (waylineType == Horizontal && event->x()<0) )
    {
        setVisible(false);
        isMousePress = false;
    }
    QWidget::mouseMoveEvent(event);
    setCursor(Qt::PointingHandCursor);
}

void WayLine::mouseDoubleClickEvent(QMouseEvent *event)
{
    setVisible(false);
}
