// Demidov Alexey
// Класс, реализующий направляющие

#ifndef WAYLINE_H
#define WAYLINE_H

#define WIDTH 1920
#define HEIGHT 1080

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

class WayLine : public QWidget
{
    Q_OBJECT

public:
    enum WayLineType { Horizontal, Vertical };
    explicit WayLine(WayLine::WayLineType waylineType, int coord, QWidget *parent = 0);

private:
    WayLineType waylineType;
    int coord;
    bool isMousePress;
    virtual void paintEvent(QPaintEvent* event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);

signals:
    void moveWayline(QPoint);

public slots:

};

#endif // WAYLINE_H
