/********************************************************************************
** Form generated from reading ui file 'savesvg.ui'
**
** Created: Thu 5. Nov 00:39:05 2009
**      by: Qt User Interface Compiler version 4.4.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SAVESVG_H
#define UI_SAVESVG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>

QT_BEGIN_NAMESPACE

class Ui_SaveSVGClass
{
public:

    void setupUi(QDialog *SaveSVGClass)
    {
    if (SaveSVGClass->objectName().isEmpty())
        SaveSVGClass->setObjectName(QString::fromUtf8("SaveSVGClass"));
    SaveSVGClass->setWindowModality(Qt::NonModal);
    SaveSVGClass->resize(400, 300);
    SaveSVGClass->setMaximumSize(QSize(1024, 768));

    retranslateUi(SaveSVGClass);

    QMetaObject::connectSlotsByName(SaveSVGClass);
    } // setupUi

    void retranslateUi(QDialog *SaveSVGClass)
    {
    SaveSVGClass->setWindowTitle(QApplication::translate("SaveSVGClass", "Export to SVG...", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(SaveSVGClass);
    } // retranslateUi

};

namespace Ui {
    class SaveSVGClass: public Ui_SaveSVGClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAVESVG_H
