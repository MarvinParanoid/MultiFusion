//#include <QMessageBox>
#include <QUrl>
#include <QFile>
#include <QWebFrame>
#include <QTextCodec>
#include <QSize>
#include <QDir>
#include "helpwindow.h"

HelpWindow::HelpWindow(QString caption, QString defaultContent, QString rootUrl, QWidget *parent)
    : QDialog(parent)
{
	QTextCodec::setCodecForCStrings( QTextCodec::codecForName("utf8") );
	this->defaultContent = defaultContent;

	this->basepath = QUrl::fromLocalFile(QDir::current().absoluteFilePath(rootUrl));

	ui.setupUi(this);

    //this->setCaption(caption.toUtf8());
	this->setContentsMargins(0, 0, 0, 0);

	ui.verticalLayout->setSpacing(0);
	ui.verticalLayout->setMargin(0);

	setContent(defaultContent);
}

HelpWindow::~HelpWindow()
{
}

void HelpWindow::setUrl(QString url) {
	ui.view->load(url);
}

void HelpWindow::setContent(QString contents) {
	QString current = ui.view->page()->currentFrame()->toHtml();
	QTextCodec::setCodecForCStrings( QTextCodec::codecForName("utf8") );
	ui.view->setHtml(contents.toUtf8(), basepath);
}
