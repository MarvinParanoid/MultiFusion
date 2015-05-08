/********************************************************************************
** Form generated from reading ui file 'helpwindow.ui'
**
** Created: Thu 10. Jun 06:38:46 2010
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_HELPWINDOW_H
#define UI_HELPWINDOW_H

#include <QtCore/QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QDialog>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QWebView>

QT_BEGIN_NAMESPACE

class Ui_HelpWindowClass
{
public:
    QVBoxLayout *verticalLayout;
    QWebView *view;

    void setupUi(QDialog *HelpWindowClass)
    {
        if (HelpWindowClass->objectName().isEmpty())
            HelpWindowClass->setObjectName(QString::fromUtf8("HelpWindowClass"));
        HelpWindowClass->resize(588, 584);
        verticalLayout = new QVBoxLayout(HelpWindowClass);
        verticalLayout->setSpacing(6);
        verticalLayout->setMargin(11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        view = new QWebView(HelpWindowClass);
        view->setObjectName(QString::fromUtf8("view"));

        verticalLayout->addWidget(view);

        view->raise();

        retranslateUi(HelpWindowClass);

        QMetaObject::connectSlotsByName(HelpWindowClass);
    } // setupUi

    void retranslateUi(QDialog *HelpWindowClass)
    {
        HelpWindowClass->setWindowTitle(QApplication::translate("HelpWindowClass", "HelpWindow", 0));
        Q_UNUSED(HelpWindowClass);
    } // retranslateUi

};

namespace Ui {
    class HelpWindowClass: public Ui_HelpWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HELPWINDOW_H
