/********************************************************************************
** Form generated from reading ui file 'PenEditor.ui'
**
** Created: Wed 26. May 03:19:18 2010
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_PENEDITOR_H
#define UI_PENEDITOR_H

#include <QtCore/QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QComboBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QWidget>

QT_BEGIN_NAMESPACE

class Ui_PenEditorForm
{
public:
    QVBoxLayout *vboxLayout;
    QGroupBox *previewBox;
    QGroupBox *lineStyleBox;
    QHBoxLayout *hboxLayout;
    QVBoxLayout *vboxLayout1;
    QRadioButton *noneLineStyleBtn;
    QRadioButton *solidLineStyleBtn;
    QRadioButton *dashLineStyleBtn;
    QVBoxLayout *vboxLayout2;
    QRadioButton *dotLineStyleBtn;
    QRadioButton *dashDotLineStyleBtn;
    QRadioButton *dashDotDotLineStyleBtn;
    QHBoxLayout *hboxLayout1;
    QLabel *label;
    QComboBox *lineWidthCombo;
    QHBoxLayout *hboxLayout2;
    QGroupBox *capStyleBox;
    QHBoxLayout *hboxLayout3;
    QVBoxLayout *vboxLayout3;
    QRadioButton *squareCapStyleBtn;
    QRadioButton *flatCapStyleBtn;
    QRadioButton *roundCapStyleBtn;
    QGroupBox *joinStyleBox;
    QHBoxLayout *hboxLayout4;
    QVBoxLayout *vboxLayout4;
    QRadioButton *bevelJoinStyleBtn;
    QRadioButton *miterJoinStyleBtn;
    QRadioButton *roundJoinStyleBtn;
    QGroupBox *penColorBox;

    void setupUi(QWidget *PenEditorForm)
    {
        if (PenEditorForm->objectName().isEmpty())
            PenEditorForm->setObjectName(QString::fromUtf8("PenEditorForm"));
        PenEditorForm->resize(300, 391);
        vboxLayout = new QVBoxLayout(PenEditorForm);
#ifndef Q_OS_MAC
        vboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        vboxLayout->setMargin(9);
#endif
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        previewBox = new QGroupBox(PenEditorForm);
        previewBox->setObjectName(QString::fromUtf8("previewBox"));

        vboxLayout->addWidget(previewBox);

        lineStyleBox = new QGroupBox(PenEditorForm);
        lineStyleBox->setObjectName(QString::fromUtf8("lineStyleBox"));
        hboxLayout = new QHBoxLayout(lineStyleBox);
#ifndef Q_OS_MAC
        hboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        hboxLayout->setMargin(9);
#endif
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        vboxLayout1 = new QVBoxLayout();
#ifndef Q_OS_MAC
        vboxLayout1->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        vboxLayout1->setMargin(0);
#endif
        vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
        noneLineStyleBtn = new QRadioButton(lineStyleBox);
        noneLineStyleBtn->setObjectName(QString::fromUtf8("noneLineStyleBtn"));
        noneLineStyleBtn->setChecked(true);

        vboxLayout1->addWidget(noneLineStyleBtn);

        solidLineStyleBtn = new QRadioButton(lineStyleBox);
        solidLineStyleBtn->setObjectName(QString::fromUtf8("solidLineStyleBtn"));

        vboxLayout1->addWidget(solidLineStyleBtn);

        dashLineStyleBtn = new QRadioButton(lineStyleBox);
        dashLineStyleBtn->setObjectName(QString::fromUtf8("dashLineStyleBtn"));

        vboxLayout1->addWidget(dashLineStyleBtn);


        hboxLayout->addLayout(vboxLayout1);

        vboxLayout2 = new QVBoxLayout();
#ifndef Q_OS_MAC
        vboxLayout2->setSpacing(6);
#endif
        vboxLayout2->setMargin(0);
        vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
        dotLineStyleBtn = new QRadioButton(lineStyleBox);
        dotLineStyleBtn->setObjectName(QString::fromUtf8("dotLineStyleBtn"));

        vboxLayout2->addWidget(dotLineStyleBtn);

        dashDotLineStyleBtn = new QRadioButton(lineStyleBox);
        dashDotLineStyleBtn->setObjectName(QString::fromUtf8("dashDotLineStyleBtn"));

        vboxLayout2->addWidget(dashDotLineStyleBtn);

        dashDotDotLineStyleBtn = new QRadioButton(lineStyleBox);
        dashDotDotLineStyleBtn->setObjectName(QString::fromUtf8("dashDotDotLineStyleBtn"));

        vboxLayout2->addWidget(dashDotDotLineStyleBtn);


        hboxLayout->addLayout(vboxLayout2);


        vboxLayout->addWidget(lineStyleBox);

        hboxLayout1 = new QHBoxLayout();
#ifndef Q_OS_MAC
        hboxLayout1->setSpacing(6);
#endif
        hboxLayout1->setMargin(0);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        label = new QLabel(PenEditorForm);
        label->setObjectName(QString::fromUtf8("label"));

        hboxLayout1->addWidget(label);

        lineWidthCombo = new QComboBox(PenEditorForm);
        lineWidthCombo->setObjectName(QString::fromUtf8("lineWidthCombo"));
        lineWidthCombo->setEditable(true);
        lineWidthCombo->setMaxVisibleItems(12);
        lineWidthCombo->setMinimumContentsLength(1);

        hboxLayout1->addWidget(lineWidthCombo);


        vboxLayout->addLayout(hboxLayout1);

        hboxLayout2 = new QHBoxLayout();
#ifndef Q_OS_MAC
        hboxLayout2->setSpacing(6);
#endif
        hboxLayout2->setMargin(0);
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        capStyleBox = new QGroupBox(PenEditorForm);
        capStyleBox->setObjectName(QString::fromUtf8("capStyleBox"));
        hboxLayout3 = new QHBoxLayout(capStyleBox);
#ifndef Q_OS_MAC
        hboxLayout3->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        hboxLayout3->setMargin(9);
#endif
        hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
        vboxLayout3 = new QVBoxLayout();
#ifndef Q_OS_MAC
        vboxLayout3->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        vboxLayout3->setMargin(0);
#endif
        vboxLayout3->setObjectName(QString::fromUtf8("vboxLayout3"));
        squareCapStyleBtn = new QRadioButton(capStyleBox);
        squareCapStyleBtn->setObjectName(QString::fromUtf8("squareCapStyleBtn"));
        squareCapStyleBtn->setChecked(true);

        vboxLayout3->addWidget(squareCapStyleBtn);

        flatCapStyleBtn = new QRadioButton(capStyleBox);
        flatCapStyleBtn->setObjectName(QString::fromUtf8("flatCapStyleBtn"));

        vboxLayout3->addWidget(flatCapStyleBtn);

        roundCapStyleBtn = new QRadioButton(capStyleBox);
        roundCapStyleBtn->setObjectName(QString::fromUtf8("roundCapStyleBtn"));

        vboxLayout3->addWidget(roundCapStyleBtn);


        hboxLayout3->addLayout(vboxLayout3);


        hboxLayout2->addWidget(capStyleBox);

        joinStyleBox = new QGroupBox(PenEditorForm);
        joinStyleBox->setObjectName(QString::fromUtf8("joinStyleBox"));
        hboxLayout4 = new QHBoxLayout(joinStyleBox);
#ifndef Q_OS_MAC
        hboxLayout4->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        hboxLayout4->setMargin(9);
#endif
        hboxLayout4->setObjectName(QString::fromUtf8("hboxLayout4"));
        vboxLayout4 = new QVBoxLayout();
#ifndef Q_OS_MAC
        vboxLayout4->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        vboxLayout4->setMargin(0);
#endif
        vboxLayout4->setObjectName(QString::fromUtf8("vboxLayout4"));
        bevelJoinStyleBtn = new QRadioButton(joinStyleBox);
        bevelJoinStyleBtn->setObjectName(QString::fromUtf8("bevelJoinStyleBtn"));
        bevelJoinStyleBtn->setChecked(true);

        vboxLayout4->addWidget(bevelJoinStyleBtn);

        miterJoinStyleBtn = new QRadioButton(joinStyleBox);
        miterJoinStyleBtn->setObjectName(QString::fromUtf8("miterJoinStyleBtn"));

        vboxLayout4->addWidget(miterJoinStyleBtn);

        roundJoinStyleBtn = new QRadioButton(joinStyleBox);
        roundJoinStyleBtn->setObjectName(QString::fromUtf8("roundJoinStyleBtn"));

        vboxLayout4->addWidget(roundJoinStyleBtn);


        hboxLayout4->addLayout(vboxLayout4);


        hboxLayout2->addWidget(joinStyleBox);


        vboxLayout->addLayout(hboxLayout2);

        penColorBox = new QGroupBox(PenEditorForm);
        penColorBox->setObjectName(QString::fromUtf8("penColorBox"));

        vboxLayout->addWidget(penColorBox);


        retranslateUi(PenEditorForm);

        lineWidthCombo->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(PenEditorForm);
    } // setupUi

    void retranslateUi(QWidget *PenEditorForm)
    {
        PenEditorForm->setWindowTitle(QApplication::translate("PenEditorForm", "Pen properties", 0));
        previewBox->setTitle(QApplication::translate("PenEditorForm", "Preview", 0));
        lineStyleBox->setTitle(QApplication::translate("PenEditorForm", "Line style", 0));
        noneLineStyleBtn->setText(QApplication::translate("PenEditorForm", "None", 0));
        solidLineStyleBtn->setText(QApplication::translate("PenEditorForm", "Solid", 0));
        dashLineStyleBtn->setText(QApplication::translate("PenEditorForm", "Dash", 0));
        dotLineStyleBtn->setText(QApplication::translate("PenEditorForm", "Dot", 0));
        dashDotLineStyleBtn->setText(QApplication::translate("PenEditorForm", "Dash && dot", 0));
        dashDotDotLineStyleBtn->setText(QApplication::translate("PenEditorForm", "Dash && double dot", 0));
        label->setText(QApplication::translate("PenEditorForm", "Line width", 0));
        lineWidthCombo->clear();
        lineWidthCombo->insertItems(0, QStringList()
         << QApplication::translate("PenEditorForm", "1", 0)
         << QApplication::translate("PenEditorForm", "2", 0)
         << QApplication::translate("PenEditorForm", "3", 0)
         << QApplication::translate("PenEditorForm", "4", 0)
         << QApplication::translate("PenEditorForm", "5", 0)
         << QApplication::translate("PenEditorForm", "6", 0)
         << QApplication::translate("PenEditorForm", "7", 0)
         << QApplication::translate("PenEditorForm", "8", 0)
         << QApplication::translate("PenEditorForm", "9", 0)
         << QApplication::translate("PenEditorForm", "10", 0)
         << QApplication::translate("PenEditorForm", "11", 0)
         << QApplication::translate("PenEditorForm", "12", 0)
         << QApplication::translate("PenEditorForm", "13", 0)
         << QApplication::translate("PenEditorForm", "14", 0)
         << QApplication::translate("PenEditorForm", "15", 0)
         << QApplication::translate("PenEditorForm", "16", 0)
         << QApplication::translate("PenEditorForm", "17", 0)
         << QApplication::translate("PenEditorForm", "18", 0)
         << QApplication::translate("PenEditorForm", "19", 0)
         << QApplication::translate("PenEditorForm", "20", 0)
         << QApplication::translate("PenEditorForm", "21", 0)
         << QApplication::translate("PenEditorForm", "22", 0)
         << QApplication::translate("PenEditorForm", "23", 0)
         << QApplication::translate("PenEditorForm", "24", 0)
         << QApplication::translate("PenEditorForm", "25", 0)
         << QApplication::translate("PenEditorForm", "26", 0)
         << QApplication::translate("PenEditorForm", "27", 0)
         << QApplication::translate("PenEditorForm", "28", 0)
         << QApplication::translate("PenEditorForm", "29", 0)
         << QApplication::translate("PenEditorForm", "30", 0)
         << QApplication::translate("PenEditorForm", "35", 0)
         << QApplication::translate("PenEditorForm", "40", 0)
         << QApplication::translate("PenEditorForm", "45", 0)
         << QApplication::translate("PenEditorForm", "50", 0)
         << QApplication::translate("PenEditorForm", "55", 0)
         << QApplication::translate("PenEditorForm", "60", 0)
         << QApplication::translate("PenEditorForm", "70", 0)
         << QApplication::translate("PenEditorForm", "80", 0)
         << QApplication::translate("PenEditorForm", "90", 0)
         << QApplication::translate("PenEditorForm", "100", 0)
        );
        capStyleBox->setTitle(QApplication::translate("PenEditorForm", "Cap style", 0));
        squareCapStyleBtn->setText(QApplication::translate("PenEditorForm", "Square", 0));
        flatCapStyleBtn->setText(QApplication::translate("PenEditorForm", "Flat", 0));
        roundCapStyleBtn->setText(QApplication::translate("PenEditorForm", "Round", 0));
        joinStyleBox->setTitle(QApplication::translate("PenEditorForm", "Join style", 0));
        bevelJoinStyleBtn->setText(QApplication::translate("PenEditorForm", "Bevel", 0));
        miterJoinStyleBtn->setText(QApplication::translate("PenEditorForm", "Miter", 0));
        roundJoinStyleBtn->setText(QApplication::translate("PenEditorForm", "Round", 0));
        penColorBox->setTitle(QApplication::translate("PenEditorForm", "Pen color", 0));
        Q_UNUSED(PenEditorForm);
    } // retranslateUi

};

namespace Ui {
    class PenEditorForm: public Ui_PenEditorForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PENEDITOR_H
