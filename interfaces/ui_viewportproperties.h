/********************************************************************************
** Form generated from reading ui file 'viewportproperties.ui'
**
** Created: Fri 20. Mar 06:37:58 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_VIEWPORTPROPERTIES_H
#define UI_VIEWPORTPROPERTIES_H

#include <QtCore/QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QComboBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QRadioButton>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>

QT_BEGIN_NAMESPACE

class Ui_viewportPropsForm
{
public:
    QVBoxLayout *vboxLayout;
    QTabWidget *propertiesTabs;
    QWidget *brushTab;
    QWidget *viewportTab;
    QVBoxLayout *vboxLayout1;
    QGroupBox *windowBgOptions;
    QHBoxLayout *hboxLayout;
    QGroupBox *bgFillStyle;
    QVBoxLayout *vboxLayout2;
    QRadioButton *colorFillBg;
    QRadioButton *transparencyFillBg;
    QGroupBox *viewportStyleOptions;
    QGridLayout *gridLayout;
    QRadioButton *sizeHintStyle;
    QRadioButton *fullWindowStyle;
    QRadioButton *FixedSizeStyle;
    QComboBox *viewportWidth;
    QComboBox *viewportHeight;
    QLabel *label;

    void setupUi(QWidget *viewportPropsForm)
    {
        if (viewportPropsForm->objectName().isEmpty())
            viewportPropsForm->setObjectName(QString::fromUtf8("viewportPropsForm"));
        viewportPropsForm->resize(437, 432);
        vboxLayout = new QVBoxLayout(viewportPropsForm);
#ifndef Q_OS_MAC
        vboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        vboxLayout->setMargin(9);
#endif
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        propertiesTabs = new QTabWidget(viewportPropsForm);
        propertiesTabs->setObjectName(QString::fromUtf8("propertiesTabs"));
        brushTab = new QWidget();
        brushTab->setObjectName(QString::fromUtf8("brushTab"));
        propertiesTabs->addTab(brushTab, QString());
        viewportTab = new QWidget();
        viewportTab->setObjectName(QString::fromUtf8("viewportTab"));
        vboxLayout1 = new QVBoxLayout(viewportTab);
#ifndef Q_OS_MAC
        vboxLayout1->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        vboxLayout1->setMargin(9);
#endif
        vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
        windowBgOptions = new QGroupBox(viewportTab);
        windowBgOptions->setObjectName(QString::fromUtf8("windowBgOptions"));
        hboxLayout = new QHBoxLayout(windowBgOptions);
#ifndef Q_OS_MAC
        hboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        hboxLayout->setMargin(9);
#endif
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        bgFillStyle = new QGroupBox(windowBgOptions);
        bgFillStyle->setObjectName(QString::fromUtf8("bgFillStyle"));
        vboxLayout2 = new QVBoxLayout(bgFillStyle);
#ifndef Q_OS_MAC
        vboxLayout2->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        vboxLayout2->setMargin(9);
#endif
        vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
        colorFillBg = new QRadioButton(bgFillStyle);
        colorFillBg->setObjectName(QString::fromUtf8("colorFillBg"));
        colorFillBg->setChecked(true);

        vboxLayout2->addWidget(colorFillBg);

        transparencyFillBg = new QRadioButton(bgFillStyle);
        transparencyFillBg->setObjectName(QString::fromUtf8("transparencyFillBg"));

        vboxLayout2->addWidget(transparencyFillBg);


        hboxLayout->addWidget(bgFillStyle);


        vboxLayout1->addWidget(windowBgOptions);

        viewportStyleOptions = new QGroupBox(viewportTab);
        viewportStyleOptions->setObjectName(QString::fromUtf8("viewportStyleOptions"));
        gridLayout = new QGridLayout(viewportStyleOptions);
#ifndef Q_OS_MAC
        gridLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        gridLayout->setMargin(9);
#endif
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        sizeHintStyle = new QRadioButton(viewportStyleOptions);
        sizeHintStyle->setObjectName(QString::fromUtf8("sizeHintStyle"));

        gridLayout->addWidget(sizeHintStyle, 2, 0, 1, 1);

        fullWindowStyle = new QRadioButton(viewportStyleOptions);
        fullWindowStyle->setObjectName(QString::fromUtf8("fullWindowStyle"));
        fullWindowStyle->setChecked(true);

        gridLayout->addWidget(fullWindowStyle, 0, 0, 1, 1);

        FixedSizeStyle = new QRadioButton(viewportStyleOptions);
        FixedSizeStyle->setObjectName(QString::fromUtf8("FixedSizeStyle"));

        gridLayout->addWidget(FixedSizeStyle, 1, 0, 1, 1);

        viewportWidth = new QComboBox(viewportStyleOptions);
        viewportWidth->setObjectName(QString::fromUtf8("viewportWidth"));
        viewportWidth->setEditable(true);

        gridLayout->addWidget(viewportWidth, 0, 1, 1, 1);

        viewportHeight = new QComboBox(viewportStyleOptions);
        viewportHeight->setObjectName(QString::fromUtf8("viewportHeight"));
        viewportHeight->setEditable(true);
        viewportHeight->setMinimumContentsLength(1);

        gridLayout->addWidget(viewportHeight, 2, 1, 1, 1);

        label = new QLabel(viewportStyleOptions);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 1, 1, 1, 1);

        sizeHintStyle->raise();
        fullWindowStyle->raise();
        FixedSizeStyle->raise();
        viewportWidth->raise();
        viewportHeight->raise();
        label->raise();
        windowBgOptions->raise();

        vboxLayout1->addWidget(viewportStyleOptions);

        propertiesTabs->addTab(viewportTab, QString());

        vboxLayout->addWidget(propertiesTabs);


        retranslateUi(viewportPropsForm);

        propertiesTabs->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(viewportPropsForm);
    } // setupUi

    void retranslateUi(QWidget *viewportPropsForm)
    {
        viewportPropsForm->setWindowTitle(QApplication::translate("viewportPropsForm", "Viewport properties", 0));
        propertiesTabs->setTabText(propertiesTabs->indexOf(brushTab), QApplication::translate("viewportPropsForm", "Viewport background", 0));
        windowBgOptions->setTitle(QApplication::translate("viewportPropsForm", "Window background", 0));
        bgFillStyle->setTitle(QApplication::translate("viewportPropsForm", "Style", 0));
        colorFillBg->setText(QApplication::translate("viewportPropsForm", "Colored background", 0));
        transparencyFillBg->setText(QApplication::translate("viewportPropsForm", "Transparency", 0));
        viewportStyleOptions->setTitle(QApplication::translate("viewportPropsForm", "Viewport style", 0));
        sizeHintStyle->setText(QApplication::translate("viewportPropsForm", "Size hints", 0));
        fullWindowStyle->setText(QApplication::translate("viewportPropsForm", "Full window (resizable)", 0));
        FixedSizeStyle->setText(QApplication::translate("viewportPropsForm", "Fixed size", 0));
        viewportWidth->clear();
        viewportWidth->insertItems(0, QStringList()
         << QApplication::translate("viewportPropsForm", "320", 0)
         << QApplication::translate("viewportPropsForm", "640", 0)
         << QApplication::translate("viewportPropsForm", "800", 0)
         << QApplication::translate("viewportPropsForm", "1024", 0)
         << QApplication::translate("viewportPropsForm", "1280", 0)
        );
        viewportHeight->clear();
        viewportHeight->insertItems(0, QStringList()
         << QApplication::translate("viewportPropsForm", "240", 0)
         << QApplication::translate("viewportPropsForm", "480", 0)
         << QApplication::translate("viewportPropsForm", "600", 0)
         << QApplication::translate("viewportPropsForm", "768", 0)
         << QApplication::translate("viewportPropsForm", "1024", 0)
        );
        label->setText(QApplication::translate("viewportPropsForm", "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal; text-decoration:none;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:20pt;\">X</span></p></body></html>", 0));
        propertiesTabs->setTabText(propertiesTabs->indexOf(viewportTab), QApplication::translate("viewportPropsForm", "Viewport style", 0));
        Q_UNUSED(viewportPropsForm);
    } // retranslateUi

};

namespace Ui {
    class viewportPropsForm: public Ui_viewportPropsForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIEWPORTPROPERTIES_H
