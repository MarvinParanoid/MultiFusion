//#include <QMessageBox>
#include <QUrl>
#include <QFile>
#include <QWebFrame>
#include <QTextCodec>
#include <QSize>
#include <QDir>
#include "HelpWindow.h"

HelpWindow::HelpWindow(QString caption, QString defaultContent, QString rootUrl, QWidget *parent)
    : QDialog(parent)
{
    #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        QTextCodec::setCodecForCStrings( QTextCodec::codecForName("utf8") );
    #endif
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
    #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        QTextCodec::setCodecForCStrings( QTextCodec::codecForName("utf8") );
    #endif
	ui.view->setHtml(contents.toUtf8(), basepath);
}
