#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include <QDialog>
#include <QWebView>
#include <QStringList>
#include "ui_helpwindow.h"

class HelpWindow : public QDialog
{
    Q_OBJECT

public:
    HelpWindow(QWidget *parent = 0);
    ~HelpWindow();

    void setContent(QString url, QString caption, QString contents);

private:
    Ui::HelpWindowClass ui;
};

#endif // HELPWINDOW_H
