/********************************************************************************
** Form generated from reading ui file 'TexturesForm.ui'
**
** Created: Wed 26. May 02:16:38 2010
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_TEXTURESFORM_H
#define UI_TEXTURESFORM_H

#include <QtCore/QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QHeaderView>
#include <QListWidget>
#include <QVBoxLayout>
#include <QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QVBoxLayout *vboxLayout;
    QListWidget *texturesList;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QString::fromUtf8("Form"));
        Form->resize(357, 365);
        vboxLayout = new QVBoxLayout(Form);
#ifndef Q_OS_MAC
        vboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        vboxLayout->setMargin(9);
#endif
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        texturesList = new QListWidget(Form);
        texturesList->setObjectName(QString::fromUtf8("texturesList"));
        texturesList->setProperty("showDropIndicator", QVariant(false));
        texturesList->setFlow(QListView::LeftToRight);
        texturesList->setProperty("isWrapping", QVariant(true));
        texturesList->setResizeMode(QListView::Adjust);
        texturesList->setLayoutMode(QListView::Batched);
        texturesList->setViewMode(QListView::ListMode);
        texturesList->setUniformItemSizes(true);
        texturesList->setWordWrap(true);
        texturesList->setSortingEnabled(true);

        vboxLayout->addWidget(texturesList);


        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QApplication::translate("Form", "Form", 0));
        Q_UNUSED(Form);
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TEXTURESFORM_H
