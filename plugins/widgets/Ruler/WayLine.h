// Demidov Alexey, 2015
// Класс, реализующий направляющие

#ifndef WAYLINE_H
#define WAYLINE_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

#define HEIGHT 1080
#define WIDTH 1920

class WayLine : public QWidget
{
    Q_OBJECT

public:
    enum WayLineType { Horizontal, Vertical };

    explicit WayLine(QWidget *parent = 0);

    void setType(WayLine::WayLineType w_type);
    WayLine::WayLineType getType();
    bool getMousePress();
    void setMousePress(bool b);
    int getCoord();
    void setCoord(int c);

private:
    int coord;
    WayLineType waylineType;
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
