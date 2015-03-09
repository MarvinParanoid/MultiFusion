/********************************************************************************
** Form generated from reading ui file 'infowindow.ui'
**
** Created: Mon 17. May 02:58:13 2010
**      by: Qt User Interface Compiler version 4.4.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_INFOWINDOW_H
#define UI_INFOWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>

QT_BEGIN_NAMESPACE

class Ui_InfoWindowClass
{
public:

    void setupUi(QDialog *InfoWindowClass)
    {
    if (InfoWindowClass->objectName().isEmpty())
        InfoWindowClass->setObjectName(QString::fromUtf8("InfoWindowClass"));
    InfoWindowClass->resize(400, 300);

    retranslateUi(InfoWindowClass);

    QMetaObject::connectSlotsByName(InfoWindowClass);
    } // setupUi

    void retranslateUi(QDialog *InfoWindowClass)
    {
    InfoWindowClass->setWindowTitle(QApplication::translate("InfoWindowClass", "InfoWindow", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(InfoWindowClass);
    } // retranslateUi

};

namespace Ui {
    class InfoWindowClass: public Ui_InfoWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INFOWINDOW_H
