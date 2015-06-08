#ifndef CLIPPINGPATH_H
#define CLIPPINGPATH_H


#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QVector>
#include "./../../../pluginTool/InterfacePlugin.h"
#include "./../../../pluginTool/Plugin.h"
#include "./../../../interfaces/ToolBoxInterface.h"
#include "./../../../interfaces/FigureToolInterface.h"
#include "./../../../interfaces/MainWindowInterface.h"
#include "./../../../interfaces/PaintWidgetInterface.h"
#include "./../../../interfaces/RPWInterface.h"
#include "./../../../interfaces/GSRInterface.h"
#include "./../../../interfaces/GContainerInterface.h"
#include "./../../../interfaces/GObjectInterface.h"
#include "./../../../interfaces/ToolButtonInterface.h"
#include "./../../../paintWidget/GVectorFigure.h"
#include "./../../../paintWidget/GObject.h"
#include "./../../../paintWidget/GContainer.h"

#include <QToolButton>
#include <QWidget>
#include <QObject>
#include <QMouseEvent>

class ToolButton:public ToolButtonInterface
{
    Q_OBJECT
    Q_INTERFACES( ToolButtonInterface )

    signals:
        void toolSelected( FigureToolInterface *tool );

    public:
        QObject* getTool()
        {
            return tool;
        }

        void setTool(QObject* t)
        {
            tool = FIGURETOOL(t);
        }

        ToolButton( FigureToolInterface *_tool, QWidget *parent ):
            ToolButtonInterface( parent ), tool( _tool )
        {
            setIcon( tool->icon() );
            setToolTip( tool->description() );

            connect( this, SIGNAL( clicked( bool ) ),
                    this, SLOT( onClicked( bool ) ) );
        }

        virtual ~ToolButton()
        {
            delete tool;
        }

    private slots:
        void onClicked( bool checked )
        {
            if( !checked ) return;

            tool->toolSelected();
            emit toolSelected( tool );
        }

    private:
        FigureToolInterface *tool;
};

class J_ClippingPath:public FigureToolInterface, public InterfacePlugin
{
    Q_OBJECT
    Q_INTERFACES( FigureToolInterface )
    Q_INTERFACES( InterfacePlugin )

    signals:
        void setActive(QString);
    public slots:
        void clip();

    public:
        virtual void cutFigure(GVectorFigure*, GVectorFigure*);

        virtual void createPlugin(QObject *parent, QString idParent,
                                  plugin::PluginsManager *manager);

        virtual QWidget* getWidget();

        virtual QString getName() const;

        virtual QIcon icon();

        virtual CreateStyle createStyle() const;

        virtual FiguresInfo figure() const;

        virtual QString description() const;

        virtual QString figureName() const;

        virtual void toolSelected();

        J_ClippingPath( const plugin::PluginsManager *manager );

        virtual ~J_ClippingPath();

    private:
        MainWindowInterface *mainWin;
        PaintWidgetInterface *painter;
        RPWInterface *rpwi;
        GSRInterface *gsri;
        QAction *ActionClippingPath;
        ToolButton *button;
        static bool resourcesInited;
};

#endif // CLIPPINGPATH_H
