// Demidov Alexey, 2015
// Класс реализующий линейку

/* Добавление линеек в виджет
    setViewportMargins(RULER_BREADTH,RULER_BREADTH,0,0);
    QGridLayout* gridLayout = new QGridLayout();
    gridLayout->setSpacing(0);
    gridLayout->setMargin(0);
    QDRuler *mHorzRuler, *mVertRuler;
    mHorzRuler = new QDRuler(QDRuler::Horizontal,this);
    mVertRuler = new QDRuler(QDRuler::Vertical,this);
    QWidget* fake = new QWidget();
    fake->setBackgroundRole(QPalette::Window);
    fake->setFixedSize(RULER_BREADTH,RULER_BREADTH);
    gridLayout->addWidget(fake,0,0);
    gridLayout->addWidget(mHorzRuler,0,1);
    gridLayout->addWidget(mVertRuler,1,0);
    gridLayout->addWidget(this->viewport(),1,1);
    this->setLayout(gridLayout);
*/

#ifndef QDRULER_H
#define QDRULER_H

// ширина линейки
#define RULER_BREADTH 20

#include <QFont>
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>

class QDRuler : public QWidget
{
    Q_OBJECT
    Q_ENUMS(RulerType)
    Q_PROPERTY(qreal origin READ origin WRITE setOrigin)
    Q_PROPERTY(qreal rulerUnit READ rulerUnit WRITE setRulerUnit)
    Q_PROPERTY(qreal rulerZoom READ rulerZoom WRITE setRulerZoom)

signals:
    void rulerClick(QPoint);

public:
    enum RulerType { Horizontal, Vertical };

    QDRuler(QDRuler::RulerType rulerType, QWidget* parent);
    QSize minimumSizeHint() const;
    QDRuler::RulerType rulerType() const;
    qreal origin() const;
    qreal rulerUnit() const;
    qreal rulerZoom() const;
    QPoint getCursorPos() const;

public slots:

    void setOrigin(const qreal origin);
    void setRulerUnit(const qreal rulerUnit);

    // изменение масштаба линейки
    void setRulerZoom(const qreal rulerZoom);

    void setCursorPos(const QPoint cursorPos);

protected:

    // мышиные события
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

    // отрисовка всего
    void paintEvent(QPaintEvent* event);

private:

    // отрисовка рисок на шкале
    void drawAScaleMeter(QPainter* painter, QRectF rulerRect, qreal scaleMeter, qreal startPositoin);

    void drawFromOriginTo(QPainter* painter, QRectF rulerRect, qreal startMark, qreal endMark, int startTickNo, qreal step, qreal startPosition);

    // отрисовка риски на линейке
    void drawMousePosTick(QPainter* painter);

    // отрисовка треугольника на верхней линейке
    void drawDownTriangle(QPainter* painter, qreal a, qreal x);

    // отрисовка треугольника на левой линейке
    void drawRigthTriangle(QPainter* painter, qreal a, qreal y);

private:

    RulerType mRulerType;   // тип
    qreal mOrigin;          // начальная точка
    qreal mRulerUnit;       // расстояние между рисками
    qreal mRulerZoom;       // масштаб
    qreal mStep;
    QPoint mCursorPos;
    bool mDrawText;
    bool isMousePress;

};

#endif // QDRULER_H
