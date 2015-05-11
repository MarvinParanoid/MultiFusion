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

    explicit WayLine(QWidget *parent = 0);

    void setType(WayLine::WayLineType w_type);
    WayLine::WayLineType getType();
    bool getMousePress();

private:
    WayLineType waylineType;
    int coord;
    bool isMousePress;
    virtual void paintEvent(QPaintEvent* event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);

signals:
    void moveWayline();

public slots:

};

#endif // WAYLINE_H
