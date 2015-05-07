﻿//#include <QMessageBox>
#include <QUrl>
#include <QFile>
#include <QWebFrame>
#include <QTextCodec>
#include <QSize>
#include <QDir>
#include <QMessageBox>
#include "HelpWindow.h"

HelpWindow::HelpWindow(QWidget *parent)
    : QDialog(parent)
{
	QTextCodec::setCodecForCStrings( QTextCodec::codecForName("utf8") );
	ui.setupUi(this);

	this->setContentsMargins(0, 0, 0, 0);

	ui.verticalLayout->setSpacing(0);
	ui.verticalLayout->setMargin(0);
}

HelpWindow::~HelpWindow()
{
}

void HelpWindow::setContent(QString url, QString caption, QString contents) {
	QTextCodec::setCodecForCStrings( QTextCodec::codecForName("utf8") );
	QUrl myUrl = QUrl::fromLocalFile(QDir::current().absoluteFilePath(url));
	//QMessageBox::information(0, "!", myUrl.toLocalFile());
	ui.view->setHtml(contents.toUtf8(), myUrl);
	this->setWindowTitle(caption);
}
