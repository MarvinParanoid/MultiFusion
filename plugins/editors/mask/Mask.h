#ifndef MASK_H
#define MASK_H

#include <QWidget>
#include <QHBoxLayout>
#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QMessageBox>

#include "./../../../pluginTool/Plugin.h"
#include "./../../../pluginTool/InterfacePlugin.h"

#include "./../../../interfaces/GradientEditorInterface.h"
#include "./../../../interfaces/MainWindowInterface.h"
#include "./../../../interfaces/PaintWidgetInterface.h"
#include "./../../../interfaces/RPWInterface.h"
#include "./../../../interfaces/GSRInterface.h"

class Mask: public GradientEditorInterface, public InterfacePlugin
{
    Q_OBJECT
    Q_INTERFACES( GradientEditorInterface )
    Q_INTERFACES( InterfacePlugin )
public slots:
    void mask_slot();

public:
    QString getName() const;
    QGradient gradient() const;
    void setGradient( const QGradient &g);
    void createPlugin(QObject *parent, QString idParent,plugin::PluginsManager *manager);
    Mask(plugin::PluginsManager *_manager);
    ~Mask();

    private:
        //Ui::Form ui;
        //GradientRangeEditor *range;
        //GradientStopsEditor *stops;
        bool handleSignals;
        bool forceUpdate;


        MainWindowInterface *mainWin;
        PaintWidgetInterface *painter;
        RPWInterface *rpwi;
        GSRInterface *gsri;
        QAction *ActionMask;
        /*
        */
};



#endif // MASK_H
