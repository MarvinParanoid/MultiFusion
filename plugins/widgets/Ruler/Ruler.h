#ifndef __RULER_H__
#define __RULER_H__

#include "QDRuler.h"
#include "Wayline.h"
#include <QWidget>
#include <QLayout>
#include <QGridLayout>
#include <QDebug>

#include "./../../../pluginTool/Plugin.h"
#include "./../../../pluginTool/InterfacePlugin.h"
#include "./../../../interfaces/RulerInterface.h"
#include "./../../../interfaces/MainWindowInterface.h"
#include "./../../../interfaces/PaintWidgetInterface.h"

#define W_COUNT 10

class Ruler:public QWidget, public RulerInterface, public InterfacePlugin
{
	Q_OBJECT
    Q_INTERFACES( RulerInterface )
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

	private:

        MainWindowInterface* mainWin;
		PaintWidgetInterface* painter;
        QDRuler *mHorzRuler;
        QDRuler *mVertRuler;

        QList <WayLine*> waylines;
        WayLine *w1;


};

#endif /* __SCALE_H__ */
