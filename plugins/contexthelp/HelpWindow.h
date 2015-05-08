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
    HelpWindow(QString caption, QString defaultContent, QString rootUrl, QWidget *parent = 0);
    ~HelpWindow();

    void setUrl(QString url);
    void setContent(QString contents);

private slots:
	void goToContents() {
		setContent(defaultContent);
	}

private:
    Ui::HelpWindowClass ui;
    QUrl basepath;
    QString defaultContent;
};

#endif // HELPWINDOW_H
