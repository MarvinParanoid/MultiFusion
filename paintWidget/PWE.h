/*
 * PWE.h
 *
 *  Created on: 23.04.2009
 *      Author: Admin
 */

#ifndef PWE_H_
#define PWE_H_

#include <QApplication>
#include "../interfaces/PWEInterface.h"
#include "../interfaces/FigureToolInterface.h"
#include "../interfaces/BackgroundInterface.h"
#include "GSelectionRect.h"
#include "GVectorFigure.h"
#include "GContainer.h"
#include "ColorBox.h"
#include "ui_viewportproperties.h"

class PaintWidget;

class PaintWidgetEditor:public QWidget, public PWEIntreface
{
	Q_OBJECT
	Q_INTERFACES(PWEIntreface)

	public:
		PaintWidgetEditor(PaintWidget &paintWidget);
		virtual ~PaintWidgetEditor();
		virtual QObject* getColorBox();
		virtual void setColorBox(QObject clb);
		virtual QObject* getPaintWidget();
		virtual void setPaintWidget(QObject pw);
		virtual bool getUpdateViews();
		virtual void setUpdateViews(bool uv);
		virtual Ui::viewportPropsForm getUI();

	public slots:
		virtual void onBackgroundChanged( QWidget *from );
		virtual void onColorChanged();
		virtual void onColorFillBgButton( bool checked );
		virtual void onTransparencyFillBgButton( bool checked );
		virtual void onSizeHintStyle( bool checked );
		virtual void onFullWindowStyle( bool checked );
		virtual void onFixedSizeStyle( bool checked );
		virtual void onViewportWidthChanged( const QString &text );
		virtual void onViewportWidthEdited( const QString &text );
		virtual void onViewportHeightChanged( const QString &text );
		virtual void onViewportHeightEdited( const QString &text );
		virtual void destroy();

    private:
		Ui::viewportPropsForm ui;
        PaintWidget &_paintWidget;
		ColorBox*	colorBox;
		bool updateViews;

		void updateBackgroundViews();
        void syncWithPainter();
        friend class PaintWidget;
};

template < class T > inline PaintWidgetEditor * PAINTWIDGETEDITOR (T *o )
{
	return qobject_cast< PaintWidgetEditor* >( o );
}

#endif /* PWE_H_ */
