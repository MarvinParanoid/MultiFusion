#ifndef __GRADIENT_EDITOR_H__
#define __GRADIENT_EDITOR_H__

#include <QWidget>
#include <QHBoxLayout>
#include "ui_gradientform.h"
#include "GradientStopsEditor.h"
#include "GradientRangeEditor.h"

#include "./../../../pluginTool/Plugin.h"
#include "./../../../pluginTool/InterfacePlugin.h"
#include "./../../../interfaces/GradientEditorInterface.h"

class GradientEditor:public GradientEditorInterface, public InterfacePlugin
{
	Q_OBJECT
	Q_INTERFACES( GradientEditorInterface )
	Q_INTERFACES( InterfacePlugin )

	signals:
		void gradientChanged( const QGradient &g );
		void setActive(QString);

	public slots:
		virtual void setGradient( const QGradient &g );

	public:
		virtual void createPlugin(QObject *parent, QString idParent,plugin::PluginsManager *manager);

		virtual QString getName()const;

		virtual QGradient gradient() const;

		GradientEditor(plugin::PluginsManager *_manager );
		virtual ~GradientEditor();

	private:
		Ui::Form ui;
		GradientRangeEditor *range;
		GradientStopsEditor *stops;
		bool handleSignals;
		bool forceUpdate;

		void syncSpread(  const QGradient &g );

	private slots:
		void onStopsChanged( const QGradientStops &s );
		void onLinearGradientSelected( bool checked );
		void onRadialGradientSelected( bool checked );
		void onConicalGradientSelected( bool checked );
		void onPadSpreadSelected( bool checked );
		void onRepeatSpreadSelected( bool checked );
		void onReflectSpreadSelected( bool checked );
		void onStartStopPointChanged( const QPointF &p );
};

#endif /* __GRADIENT_EDITOR_H__ */
