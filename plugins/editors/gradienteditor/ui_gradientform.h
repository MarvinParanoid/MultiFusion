/********************************************************************************
** Form generated from reading ui file 'gradientform.ui'
**
** Created: Wed 26. May 03:09:29 2010
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_GRADIENTFORM_H
#define UI_GRADIENTFORM_H

#include <QtCore/QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QVBoxLayout *vboxLayout;
    QGroupBox *gradientOptions;
    QHBoxLayout *hboxLayout;
    QGroupBox *gradientColorsBox;
    QGroupBox *gradientTypeBox;
    QVBoxLayout *vboxLayout1;
    QRadioButton *linearTypeButton;
    QRadioButton *radialTypeButton;
    QRadioButton *conicalTypeButton;
    QGroupBox *spreadMethodBox;
    QVBoxLayout *vboxLayout2;
    QRadioButton *padSpreadButton;
    QRadioButton *repeatSpreadButton;
    QRadioButton *reflectSpreadButton;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QString::fromUtf8("Form"));
        Form->resize(546, 546);
        vboxLayout = new QVBoxLayout(Form);
#ifndef Q_OS_MAC
        vboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        vboxLayout->setMargin(9);
#endif
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        gradientOptions = new QGroupBox(Form);
        gradientOptions->setObjectName(QString::fromUtf8("gradientOptions"));
        hboxLayout = new QHBoxLayout(gradientOptions);
#ifndef Q_OS_MAC
        hboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        hboxLayout->setMargin(9);
#endif
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        gradientColorsBox = new QGroupBox(gradientOptions);
        gradientColorsBox->setObjectName(QString::fromUtf8("gradientColorsBox"));

        hboxLayout->addWidget(gradientColorsBox);

        gradientTypeBox = new QGroupBox(gradientOptions);
        gradientTypeBox->setObjectName(QString::fromUtf8("gradientTypeBox"));
        vboxLayout1 = new QVBoxLayout(gradientTypeBox);
#ifndef Q_OS_MAC
        vboxLayout1->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        vboxLayout1->setMargin(9);
#endif
        vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
        linearTypeButton = new QRadioButton(gradientTypeBox);
        linearTypeButton->setObjectName(QString::fromUtf8("linearTypeButton"));

        vboxLayout1->addWidget(linearTypeButton);

        radialTypeButton = new QRadioButton(gradientTypeBox);
        radialTypeButton->setObjectName(QString::fromUtf8("radialTypeButton"));

        vboxLayout1->addWidget(radialTypeButton);

        conicalTypeButton = new QRadioButton(gradientTypeBox);
        conicalTypeButton->setObjectName(QString::fromUtf8("conicalTypeButton"));

        vboxLayout1->addWidget(conicalTypeButton);


        hboxLayout->addWidget(gradientTypeBox);

        spreadMethodBox = new QGroupBox(gradientOptions);
        spreadMethodBox->setObjectName(QString::fromUtf8("spreadMethodBox"));
        vboxLayout2 = new QVBoxLayout(spreadMethodBox);
#ifndef Q_OS_MAC
        vboxLayout2->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        vboxLayout2->setMargin(9);
#endif
        vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
        padSpreadButton = new QRadioButton(spreadMethodBox);
        padSpreadButton->setObjectName(QString::fromUtf8("padSpreadButton"));

        vboxLayout2->addWidget(padSpreadButton);

        repeatSpreadButton = new QRadioButton(spreadMethodBox);
        repeatSpreadButton->setObjectName(QString::fromUtf8("repeatSpreadButton"));

        vboxLayout2->addWidget(repeatSpreadButton);

        reflectSpreadButton = new QRadioButton(spreadMethodBox);
        reflectSpreadButton->setObjectName(QString::fromUtf8("reflectSpreadButton"));

        vboxLayout2->addWidget(reflectSpreadButton);


        hboxLayout->addWidget(spreadMethodBox);


        vboxLayout->addWidget(gradientOptions);


        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QApplication::translate("Form", "Form", 0));
        gradientOptions->setTitle(QString());
        gradientColorsBox->setTitle(QApplication::translate("Form", "Colors", 0));
        gradientTypeBox->setTitle(QApplication::translate("Form", "Type", 0));
        linearTypeButton->setText(QApplication::translate("Form", "Linear", 0));
        radialTypeButton->setText(QApplication::translate("Form", "Radial", 0));
        conicalTypeButton->setText(QApplication::translate("Form", "Conical", 0));
        spreadMethodBox->setTitle(QApplication::translate("Form", "Spread method", 0));
        padSpreadButton->setText(QApplication::translate("Form", "Pad", 0));
        repeatSpreadButton->setText(QApplication::translate("Form", "Repeat", 0));
        reflectSpreadButton->setText(QApplication::translate("Form", "Reflect", 0));
        Q_UNUSED(Form);
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRADIENTFORM_H
