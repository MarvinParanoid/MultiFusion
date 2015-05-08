/********************************************************************************
** Form generated from reading ui file 'BrushEditor.ui'
**
** Created: Wed 26. May 02:16:38 2010
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_BRUSHEDITOR_H
#define UI_BRUSHEDITOR_H

#include <QtCore/QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QGridLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QRadioButton>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>

QT_BEGIN_NAMESPACE

class Ui_BrushEditorForm
{
public:
    QVBoxLayout *vboxLayout;
    QGroupBox *PreviewBox;
    QTabWidget *BrushPropsTab;
    QWidget *patternBrushTab;
    QVBoxLayout *vboxLayout1;
    QGroupBox *styleBox;
    QGridLayout *gridLayout;
    QRadioButton *dense4PatternButton;
    QRadioButton *dense1PatternButton;
    QRadioButton *noBrushButton;
    QRadioButton *dense5PatternButton;
    QRadioButton *FDiagPatternButton;
    QRadioButton *solidPatternButton;
    QRadioButton *dense2PatternButton;
    QRadioButton *dense6PatternButton;
    QRadioButton *dense7PatternButton;
    QRadioButton *VerPatternButton;
    QRadioButton *CrossPatternButton;
    QRadioButton *BDiagPatternButton;
    QRadioButton *dense3PatternButton;
    QRadioButton *HorPatternButton;
    QRadioButton *DiagCrossPatternButton;
    QGroupBox *colorBox;
    QWidget *gradientBrushTab;
    QGridLayout *gridLayout1;
    QGroupBox *gradientBox;
    QWidget *textureBrushTab;

    void setupUi(QWidget *BrushEditorForm)
    {
        if (BrushEditorForm->objectName().isEmpty())
            BrushEditorForm->setObjectName(QString::fromUtf8("BrushEditorForm"));
        BrushEditorForm->resize(538, 395);
        vboxLayout = new QVBoxLayout(BrushEditorForm);
#ifndef Q_OS_MAC
        vboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        vboxLayout->setMargin(9);
#endif
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        PreviewBox = new QGroupBox(BrushEditorForm);
        PreviewBox->setObjectName(QString::fromUtf8("PreviewBox"));

        vboxLayout->addWidget(PreviewBox);

        BrushPropsTab = new QTabWidget(BrushEditorForm);
        BrushPropsTab->setObjectName(QString::fromUtf8("BrushPropsTab"));
        patternBrushTab = new QWidget();
        patternBrushTab->setObjectName(QString::fromUtf8("patternBrushTab"));
        vboxLayout1 = new QVBoxLayout(patternBrushTab);
#ifndef Q_OS_MAC
        vboxLayout1->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        vboxLayout1->setMargin(9);
#endif
        vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
        styleBox = new QGroupBox(patternBrushTab);
        styleBox->setObjectName(QString::fromUtf8("styleBox"));
        gridLayout = new QGridLayout(styleBox);
#ifndef Q_OS_MAC
        gridLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        gridLayout->setMargin(9);
#endif
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        dense4PatternButton = new QRadioButton(styleBox);
        dense4PatternButton->setObjectName(QString::fromUtf8("dense4PatternButton"));

        gridLayout->addWidget(dense4PatternButton, 0, 1, 1, 1);

        dense1PatternButton = new QRadioButton(styleBox);
        dense1PatternButton->setObjectName(QString::fromUtf8("dense1PatternButton"));

        gridLayout->addWidget(dense1PatternButton, 2, 0, 1, 1);

        noBrushButton = new QRadioButton(styleBox);
        noBrushButton->setObjectName(QString::fromUtf8("noBrushButton"));

        gridLayout->addWidget(noBrushButton, 0, 0, 1, 1);

        dense5PatternButton = new QRadioButton(styleBox);
        dense5PatternButton->setObjectName(QString::fromUtf8("dense5PatternButton"));

        gridLayout->addWidget(dense5PatternButton, 1, 1, 1, 1);

        FDiagPatternButton = new QRadioButton(styleBox);
        FDiagPatternButton->setObjectName(QString::fromUtf8("FDiagPatternButton"));

        gridLayout->addWidget(FDiagPatternButton, 3, 2, 1, 1);

        solidPatternButton = new QRadioButton(styleBox);
        solidPatternButton->setObjectName(QString::fromUtf8("solidPatternButton"));

        gridLayout->addWidget(solidPatternButton, 1, 0, 1, 1);

        dense2PatternButton = new QRadioButton(styleBox);
        dense2PatternButton->setObjectName(QString::fromUtf8("dense2PatternButton"));

        gridLayout->addWidget(dense2PatternButton, 3, 0, 1, 1);

        dense6PatternButton = new QRadioButton(styleBox);
        dense6PatternButton->setObjectName(QString::fromUtf8("dense6PatternButton"));

        gridLayout->addWidget(dense6PatternButton, 2, 1, 1, 1);

        dense7PatternButton = new QRadioButton(styleBox);
        dense7PatternButton->setObjectName(QString::fromUtf8("dense7PatternButton"));

        gridLayout->addWidget(dense7PatternButton, 3, 1, 1, 1);

        VerPatternButton = new QRadioButton(styleBox);
        VerPatternButton->setObjectName(QString::fromUtf8("VerPatternButton"));

        gridLayout->addWidget(VerPatternButton, 0, 2, 1, 1);

        CrossPatternButton = new QRadioButton(styleBox);
        CrossPatternButton->setObjectName(QString::fromUtf8("CrossPatternButton"));

        gridLayout->addWidget(CrossPatternButton, 1, 2, 1, 1);

        BDiagPatternButton = new QRadioButton(styleBox);
        BDiagPatternButton->setObjectName(QString::fromUtf8("BDiagPatternButton"));

        gridLayout->addWidget(BDiagPatternButton, 2, 2, 1, 1);

        dense3PatternButton = new QRadioButton(styleBox);
        dense3PatternButton->setObjectName(QString::fromUtf8("dense3PatternButton"));

        gridLayout->addWidget(dense3PatternButton, 4, 0, 1, 1);

        HorPatternButton = new QRadioButton(styleBox);
        HorPatternButton->setObjectName(QString::fromUtf8("HorPatternButton"));

        gridLayout->addWidget(HorPatternButton, 4, 1, 1, 1);

        DiagCrossPatternButton = new QRadioButton(styleBox);
        DiagCrossPatternButton->setObjectName(QString::fromUtf8("DiagCrossPatternButton"));

        gridLayout->addWidget(DiagCrossPatternButton, 4, 2, 1, 1);


        vboxLayout1->addWidget(styleBox);

        colorBox = new QGroupBox(patternBrushTab);
        colorBox->setObjectName(QString::fromUtf8("colorBox"));

        vboxLayout1->addWidget(colorBox);

        BrushPropsTab->addTab(patternBrushTab, QString());
        gradientBrushTab = new QWidget();
        gradientBrushTab->setObjectName(QString::fromUtf8("gradientBrushTab"));
        gridLayout1 = new QGridLayout(gradientBrushTab);
#ifndef Q_OS_MAC
        gridLayout1->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        gridLayout1->setMargin(9);
#endif
        gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
        gradientBox = new QGroupBox(gradientBrushTab);
        gradientBox->setObjectName(QString::fromUtf8("gradientBox"));

        gridLayout1->addWidget(gradientBox, 0, 0, 1, 1);

        BrushPropsTab->addTab(gradientBrushTab, QString());
        textureBrushTab = new QWidget();
        textureBrushTab->setObjectName(QString::fromUtf8("textureBrushTab"));
        BrushPropsTab->addTab(textureBrushTab, QString());

        vboxLayout->addWidget(BrushPropsTab);


        retranslateUi(BrushEditorForm);

        BrushPropsTab->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(BrushEditorForm);
    } // setupUi

    void retranslateUi(QWidget *BrushEditorForm)
    {
        BrushEditorForm->setWindowTitle(QApplication::translate("BrushEditorForm", "Brush properties", 0));
        PreviewBox->setTitle(QApplication::translate("BrushEditorForm", "Preview", 0));
        styleBox->setTitle(QApplication::translate("BrushEditorForm", "Style", 0));
        dense4PatternButton->setText(QApplication::translate("BrushEditorForm", "Dense 4", 0));
        dense1PatternButton->setText(QApplication::translate("BrushEditorForm", "Dense 1", 0));
        noBrushButton->setText(QApplication::translate("BrushEditorForm", "No brush", 0));
        dense5PatternButton->setText(QApplication::translate("BrushEditorForm", "Dense 5", 0));
        FDiagPatternButton->setText(QApplication::translate("BrushEditorForm", "Forward diagonal lines", 0));
        solidPatternButton->setText(QApplication::translate("BrushEditorForm", "Solid", 0));
        dense2PatternButton->setText(QApplication::translate("BrushEditorForm", "Dense 2", 0));
        dense6PatternButton->setText(QApplication::translate("BrushEditorForm", "Dense 6", 0));
        dense7PatternButton->setText(QApplication::translate("BrushEditorForm", "Dense 7", 0));
        VerPatternButton->setText(QApplication::translate("BrushEditorForm", "Verital lines", 0));
        CrossPatternButton->setText(QApplication::translate("BrushEditorForm", "Cross lines", 0));
        BDiagPatternButton->setText(QApplication::translate("BrushEditorForm", "Backward diagonal lines", 0));
        dense3PatternButton->setText(QApplication::translate("BrushEditorForm", "Dense 3", 0));
        HorPatternButton->setText(QApplication::translate("BrushEditorForm", "Horizontal lines", 0));
        DiagCrossPatternButton->setText(QApplication::translate("BrushEditorForm", "Crossing diagonal lines", 0));
        colorBox->setTitle(QApplication::translate("BrushEditorForm", "Color", 0));
        BrushPropsTab->setTabText(BrushPropsTab->indexOf(patternBrushTab), QApplication::translate("BrushEditorForm", "Pattern brush", 0));
        gradientBox->setTitle(QApplication::translate("BrushEditorForm", "Gradient", 0));
        BrushPropsTab->setTabText(BrushPropsTab->indexOf(gradientBrushTab), QApplication::translate("BrushEditorForm", "Gradient brush", 0));
        BrushPropsTab->setTabText(BrushPropsTab->indexOf(textureBrushTab), QApplication::translate("BrushEditorForm", "Texture brush", 0));
        Q_UNUSED(BrushEditorForm);
    } // retranslateUi

};

namespace Ui {
    class BrushEditorForm: public Ui_BrushEditorForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BRUSHEDITOR_H
