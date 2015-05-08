// Demidov Alexey
// Класс, реализующий направляющие

#ifndef WAYLINE_H
#define WAYLINE_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

class WayLine : public QWidget
{
    Q_OBJECT

public:
    explicit WayLine(int coord, QWidget *parent = 0);

private:
    int coord;
    void paintEvent(QPaintEvent* event);
    void mouseMoveEvent(QMouseEvent *event);

signals:

public slots:

};

#endif // WAYLINE_H
