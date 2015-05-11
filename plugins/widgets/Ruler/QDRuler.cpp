#include "QDRuler.h"

QDRuler::QDRuler(QDRuler::RulerType rulerType, QWidget* parent):
    QWidget(parent), mRulerType(rulerType), mRulerUnit(1.0),
    mRulerZoom(1.0), mOrigin(0.0), mStep(mRulerUnit*100)
{
    isMousePress = false;
    setMouseTracking(true);
    QFont txtFont("Goudy Old Style", 8, 20);
    txtFont.setStyleHint(QFont::TypeWriter,QFont::PreferOutline);
    setFont(txtFont);
}

QSize QDRuler::minimumSizeHint() const
{
  return QSize(RULER_BREADTH,RULER_BREADTH);
}

QDRuler::RulerType QDRuler::rulerType() const
{
  return mRulerType;
}

qreal QDRuler::origin() const
{
  return mOrigin;
}

qreal QDRuler::rulerUnit() const
{
  return mRulerUnit;
}

qreal QDRuler::rulerZoom() const
{
    return mRulerZoom;
}

QPoint QDRuler::getCursorPos() const
{
    return mCursorPos;
}

void QDRuler::setOrigin(const qreal origin)
{
  if (mOrigin != origin)
  {
    mOrigin = origin;
    update();
  }
}

void QDRuler::setRulerUnit(const qreal rulerUnit)
{
  if (mRulerUnit != rulerUnit)
  {
    mRulerUnit = rulerUnit;
    mStep = mRulerUnit*100;
    update();
  }
}

// изменение масштаба линейки
void QDRuler::setRulerZoom(const qreal rulerZoom)
{
  if (mRulerZoom != rulerZoom)
  {
    mRulerZoom = rulerZoom;
    update();
  }
}

void QDRuler::setCursorPos(const QPoint cursorPos)
{
    mCursorPos = cursorPos;
    update();
}

void QDRuler::mouseMoveEvent(QMouseEvent *event)
{
    if(isMousePress)
    {
        if ( (mRulerType==Horizontal && event->y()>RULER_BREADTH) || (mRulerType==Vertical && event->x()>RULER_BREADTH) )
        {
            isMousePress = false;
            rulerClick(QPoint(event->pos()));
        }
    }
    mCursorPos = event->pos();
    update();
    QWidget::mouseMoveEvent(event);
}

void QDRuler::mousePressEvent(QMouseEvent *event)
{
    isMousePress = true;
}

// отрисовка всего
void QDRuler::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::TextAntialiasing | QPainter::HighQualityAntialiasing);
    QPen pen(Qt::black,0);
    pen.setCosmetic(true);
    painter.setPen(pen);
    // прямоугольник в котором будет рисоваться сама линейка
    QRectF rulerRect = this->rect();

    // заполним его
    //painter.fillRect(rulerRect,QColor(220,200,180));
    painter.fillRect(rulerRect,QColor(236,233,216));

    // отрисовка рисок на шкале
    // 25
    drawAScaleMeter(&painter,rulerRect,25,(Horizontal == mRulerType ? rulerRect.height() : rulerRect.width())/2);
    // 50
    drawAScaleMeter(&painter,rulerRect,50,(Horizontal == mRulerType ? rulerRect.height() : rulerRect.width())/4);
    // 100 (с цифрами)
    mDrawText = true;
    drawAScaleMeter(&painter,rulerRect,100,0);
    mDrawText = false;

    // отрисовка индикатора курсора
    painter.setOpacity(0.7);
    drawMousePosTick(&painter);
    painter.setOpacity(1.0);

    // отрисовка линии между ruler и view
    QPointF starPt = Horizontal == mRulerType ? rulerRect.bottomLeft() : rulerRect.topRight();
    QPointF endPt = Horizontal == mRulerType ? rulerRect.bottomRight() : rulerRect.bottomRight();
    painter.setPen(QPen(Qt::black,2));
    painter.drawLine(starPt,endPt);
}

// отрисовка рисок на шкале
void QDRuler::drawAScaleMeter(QPainter* painter, QRectF rulerRect, qreal scaleMeter, qreal startPositoin)
{
    bool isHorzRuler = Horizontal == mRulerType;

    scaleMeter  = scaleMeter * mRulerUnit * mRulerZoom;

    // Ruler rectangle starting mark
    qreal rulerStartMark = isHorzRuler ? rulerRect.left() : rulerRect.top();
    // Ruler rectangle ending mark
    qreal rulerEndMark = isHorzRuler ? rulerRect.right() : rulerRect.bottom();

    // If origin point is between the start & end mard, we have to draw both from origin to left mark & origin to right mark.
    if (mOrigin >= rulerStartMark && mOrigin <= rulerEndMark)
    {
        drawFromOriginTo(painter, rulerRect, mOrigin, rulerEndMark, 0, scaleMeter, startPositoin);
        drawFromOriginTo(painter, rulerRect, mOrigin, rulerStartMark, 0, -scaleMeter, startPositoin);
    }
    // If origin point is left of the start mark, we have to draw from origin to end mark.
    else if (mOrigin < rulerStartMark)
    {
        int tickNo = int((rulerStartMark - mOrigin) / scaleMeter);
        drawFromOriginTo(painter, rulerRect, mOrigin + scaleMeter * tickNo, rulerEndMark, tickNo, scaleMeter, startPositoin);
    }
    // If origin point is right of the end mark, we have to draw from origin to start mark.
    else if (mOrigin > rulerEndMark)
    {
        int tickNo = int((mOrigin - rulerEndMark) / scaleMeter);
        drawFromOriginTo(painter, rulerRect, mOrigin - scaleMeter * tickNo, rulerStartMark, tickNo, -scaleMeter, startPositoin);
    }
}

void QDRuler::drawFromOriginTo(QPainter* painter, QRectF rulerRect, qreal startMark, qreal endMark, int startTickNo, qreal step, qreal startPosition)
{
    bool isHorzRuler = Horizontal == mRulerType;
    qreal x1,x2,y1,y2;

    for (qreal current = startMark; (step < 0 ? current >= endMark : current <= endMark); current += step)
    {
        if (isHorzRuler)
        {
            x1 = current;
            y1 = rulerRect.top() + startPosition;
            x2 = current;
            y2 = rulerRect.bottom();
        }
        else
        {
            x1 = rulerRect.left() + startPosition;
            y1 = current;
            x2 = rulerRect.right();
            y2 = current;
        }
        painter->drawLine(QLineF(x1,y1,x2,y2));
        // отрисовка цифр
        if (mDrawText)
        {
            QPainterPath txtPath;
            txtPath.addText(x1 + 1,y1 + (isHorzRuler ? 7 : -2),this->font(),QString::number((mStep * startTickNo++)));
            painter->drawPath(txtPath);
        }
    }
}

// отрисовка риски на линейке
void QDRuler::drawMousePosTick(QPainter* painter)
{
    if (Horizontal == mRulerType)
    {
        drawDownTriangle(painter, RULER_BREADTH, mCursorPos.x());
    }
    else
    {
        drawRigthTriangle(painter, RULER_BREADTH, mCursorPos.y());
    }
}

// отрисовка треугольника на верхней линейке
void QDRuler::drawDownTriangle(QPainter* painter, qreal a, qreal x)
{
    QPainterPath path;
    path.moveTo( x, a-1);
    path.lineTo( x-a/3, a/2);
    path.lineTo( x+a/3, a/2);
    path.lineTo( x, a-1);
    painter->fillPath(path, QBrush(QColor ("blue")));
}

// отрисовка треугольника на левой линейке
void QDRuler::drawRigthTriangle(QPainter* painter, qreal a, qreal y)
{
    QPainterPath path;
    path.moveTo( a-1, y);
    path.lineTo( a/2, y-a/3);
    path.lineTo( a/2, y+a/3);
    path.lineTo( a-1, y);
    painter->fillPath(path, QBrush(QColor ("blue")));
}
