#ifndef __RULER_H__
#define __RULER_H__

#include "QDRuler.h"
#include "WayLine.h"
#include <QWidget>
#include <QLayout>
#include <QGridLayout>
#include <QDebug>

#include "./../../../pluginTool/Plugin.h"
#include "./../../../pluginTool/InterfacePlugin.h"
#include "./../../../interfaces/MainWindowInterface.h"
#include "./../../../interfaces/PaintWidgetInterface.h"
#include "./../../../interfaces/GSRInterface.h"
#include "./../../../interfaces/RPWInterface.h"

// максимальное кол-во направляющих
#define W_COUNT 10

// расстояние на котором рамка начинает присасываться к направляющей
#define S_DIST 5

class Ruler:public QWidget, public InterfacePlugin
{
    Q_OBJECT
	Q_INTERFACES( InterfacePlugin )

	signals:

	public:

        Ruler(plugin::PluginsManager *manager);
        virtual void createPlugin(QObject *parent, QString idParent,plugin::PluginsManager *manager);
        virtual QString getName()const;
        virtual ~Ruler(){}
        WayLine *getFreeWayline();

	private slots:

        // изменение масштаба
        void zoomEvent(qreal scale);

        // изменение точки отсчета
        void mouseMoveOrigin(QPoint origin);

        // изменение позиций маркеров курсора
        void mouseMoveCoords(QPoint origin, QPoint global, qreal scale);

        void rulerClickedH(QPoint);
        void rulerClickedV(QPoint);
        void moveSelection(qreal,qreal);

	private:

        qreal scale;
        GSRInterface* selection;
        RPWInterface* realPainter;
        MainWindowInterface* mainWin;
		PaintWidgetInterface* painter;
        QDRuler *mHorzRuler;
        QDRuler *mVertRuler;
        QList <WayLine*> waylines;

};

#endif /* __SCALE_H__ */
