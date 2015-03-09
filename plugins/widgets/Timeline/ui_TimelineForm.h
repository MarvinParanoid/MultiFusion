/********************************************************************************
** Form generated from reading ui file 'TimelineForm.ui'
**
** Created: Wed 26. May 04:01:23 2010
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_TIMELINEFORM_H
#define UI_TIMELINEFORM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLCDNumber>
#include <QtGui/QLabel>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_timelineForm
{
public:
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout;
    QLCDNumber *currentFrameLcd;
    QLabel *label;
    QLCDNumber *countFramesLcd;
    QSlider *frameSlider;
    QHBoxLayout *hboxLayout1;
    QSpacerItem *spacerItem;
    QToolButton *firstFrameBtn;
    QToolButton *prevFrameBtn;
    QToolButton *playBtn;
    QToolButton *pauseBtn;
    QToolButton *stopBtn;
    QToolButton *nextFrameBtn;
    QToolButton *lastFrameBtn;
    QSpacerItem *spacerItem1;

    void setupUi(QWidget *timelineForm)
    {
        if (timelineForm->objectName().isEmpty())
            timelineForm->setObjectName(QString::fromUtf8("timelineForm"));
        timelineForm->resize(335, 90);
        vboxLayout = new QVBoxLayout(timelineForm);
#ifndef Q_OS_MAC
        vboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        vboxLayout->setMargin(9);
#endif
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        hboxLayout = new QHBoxLayout();
#ifndef Q_OS_MAC
        hboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        hboxLayout->setMargin(0);
#endif
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        currentFrameLcd = new QLCDNumber(timelineForm);
        currentFrameLcd->setObjectName(QString::fromUtf8("currentFrameLcd"));

        hboxLayout->addWidget(currentFrameLcd);

        label = new QLabel(timelineForm);
        label->setObjectName(QString::fromUtf8("label"));

        hboxLayout->addWidget(label);

        countFramesLcd = new QLCDNumber(timelineForm);
        countFramesLcd->setObjectName(QString::fromUtf8("countFramesLcd"));

        hboxLayout->addWidget(countFramesLcd);

        frameSlider = new QSlider(timelineForm);
        frameSlider->setObjectName(QString::fromUtf8("frameSlider"));
        frameSlider->setMinimumSize(QSize(164, 0));
        frameSlider->setMaximum(0);
        frameSlider->setPageStep(1);
        frameSlider->setOrientation(Qt::Horizontal);
        frameSlider->setTickPosition(QSlider::TicksBothSides);
        frameSlider->setTickInterval(1);

        hboxLayout->addWidget(frameSlider);


        vboxLayout->addLayout(hboxLayout);

        hboxLayout1 = new QHBoxLayout();
#ifndef Q_OS_MAC
        hboxLayout1->setSpacing(6);
#endif
        hboxLayout1->setMargin(0);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout1->addItem(spacerItem);

        firstFrameBtn = new QToolButton(timelineForm);
        firstFrameBtn->setObjectName(QString::fromUtf8("firstFrameBtn"));
        const QIcon icon = QIcon(QString::fromUtf8(":/timeline/images/first.png"));
        firstFrameBtn->setIcon(icon);

        hboxLayout1->addWidget(firstFrameBtn);

        prevFrameBtn = new QToolButton(timelineForm);
        prevFrameBtn->setObjectName(QString::fromUtf8("prevFrameBtn"));
        const QIcon icon1 = QIcon(QString::fromUtf8(":/timeline/images/back.png"));
        prevFrameBtn->setIcon(icon1);

        hboxLayout1->addWidget(prevFrameBtn);

        playBtn = new QToolButton(timelineForm);
        playBtn->setObjectName(QString::fromUtf8("playBtn"));
        const QIcon icon2 = QIcon(QString::fromUtf8(":/timeline/images/play.png"));
        playBtn->setIcon(icon2);
        playBtn->setCheckable(true);
        playBtn->setAutoExclusive(true);

        hboxLayout1->addWidget(playBtn);

        pauseBtn = new QToolButton(timelineForm);
        pauseBtn->setObjectName(QString::fromUtf8("pauseBtn"));
        const QIcon icon3 = QIcon(QString::fromUtf8(":/timeline/images/pause.png"));
        pauseBtn->setIcon(icon3);
        pauseBtn->setCheckable(true);
        pauseBtn->setAutoExclusive(true);

        hboxLayout1->addWidget(pauseBtn);

        stopBtn = new QToolButton(timelineForm);
        stopBtn->setObjectName(QString::fromUtf8("stopBtn"));
        const QIcon icon4 = QIcon(QString::fromUtf8(":/timeline/images/stop.png"));
        stopBtn->setIcon(icon4);
        stopBtn->setCheckable(true);
        stopBtn->setChecked(true);
        stopBtn->setAutoExclusive(true);

        hboxLayout1->addWidget(stopBtn);

        nextFrameBtn = new QToolButton(timelineForm);
        nextFrameBtn->setObjectName(QString::fromUtf8("nextFrameBtn"));
        const QIcon icon5 = QIcon(QString::fromUtf8(":/timeline/images/next.png"));
        nextFrameBtn->setIcon(icon5);

        hboxLayout1->addWidget(nextFrameBtn);

        lastFrameBtn = new QToolButton(timelineForm);
        lastFrameBtn->setObjectName(QString::fromUtf8("lastFrameBtn"));
        const QIcon icon6 = QIcon(QString::fromUtf8(":/timeline/images/last.png"));
        lastFrameBtn->setIcon(icon6);

        hboxLayout1->addWidget(lastFrameBtn);

        spacerItem1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout1->addItem(spacerItem1);


        vboxLayout->addLayout(hboxLayout1);


        retranslateUi(timelineForm);
        QObject::connect(frameSlider, SIGNAL(valueChanged(int)), currentFrameLcd, SLOT(display(int)));

        QMetaObject::connectSlotsByName(timelineForm);
    } // setupUi

    void retranslateUi(QWidget *timelineForm)
    {
        timelineForm->setWindowTitle(QApplication::translate("timelineForm", "Timeline", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("timelineForm", "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal; text-decoration:none;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt;\">/</span></p></body></html>", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        firstFrameBtn->setToolTip(QApplication::translate("timelineForm", "To first frame", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        firstFrameBtn->setText(QString());
#ifndef QT_NO_TOOLTIP
        prevFrameBtn->setToolTip(QApplication::translate("timelineForm", "To previous frame", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        prevFrameBtn->setText(QString());
#ifndef QT_NO_TOOLTIP
        playBtn->setToolTip(QApplication::translate("timelineForm", "Play", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        playBtn->setText(QString());
#ifndef QT_NO_TOOLTIP
        pauseBtn->setToolTip(QApplication::translate("timelineForm", "Pause", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        pauseBtn->setText(QString());
#ifndef QT_NO_TOOLTIP
        stopBtn->setToolTip(QApplication::translate("timelineForm", "Stop", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        stopBtn->setText(QString());
#ifndef QT_NO_TOOLTIP
        nextFrameBtn->setToolTip(QApplication::translate("timelineForm", "To next frame", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        nextFrameBtn->setText(QString());
#ifndef QT_NO_TOOLTIP
        lastFrameBtn->setToolTip(QApplication::translate("timelineForm", "To last frame", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        lastFrameBtn->setText(QString());
        Q_UNUSED(timelineForm);
    } // retranslateUi

};

namespace Ui {
    class timelineForm: public Ui_timelineForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TIMELINEFORM_H
