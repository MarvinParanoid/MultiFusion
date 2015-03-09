/**
* PWEInterface.h
*
*  Created on: 23.04.2009
*      Author: Alexey Kukushkin
*/

#ifndef PWEINTERFACE_H_
#define PWEINTERFACE_H_

#include "ui_viewportproperties.h"
#include <QApplication>

class PWEIntreface
{
	public:
		PWEIntreface(){};
		virtual ~PWEIntreface(){};
		virtual QObject* getColorBox() = 0;
		virtual void setColorBox(QObject clb) = 0;
		virtual QObject* getPaintWidget() = 0;
		virtual void setPaintWidget(QObject pw) = 0;
		virtual bool getUpdateViews() = 0;
		virtual void setUpdateViews(bool uv) = 0;
		virtual Ui::viewportPropsForm getUI() = 0;

	public slots:

		virtual void onBackgroundChanged( QWidget *from ) = 0;
		virtual void onColorChanged() = 0;
		virtual void onColorFillBgButton( bool checked ) = 0;
		virtual void onTransparencyFillBgButton( bool checked ) = 0;
		virtual void onSizeHintStyle( bool checked ) = 0;
		virtual void onFullWindowStyle( bool checked ) = 0;
		virtual void onFixedSizeStyle( bool checked ) = 0;
		virtual void onViewportWidthChanged( const QString &text ) = 0;
		virtual void onViewportWidthEdited( const QString &text ) = 0;
		virtual void onViewportHeightChanged( const QString &text ) = 0;
		virtual void onViewportHeightEdited( const QString &text )= 0;
		virtual void destroy() = 0;

	private:
		void updateBackgroundViews();
		void syncWithPainter();

};

Q_DECLARE_INTERFACE( PWEIntreface, "com.alxn1.sacs2.PWEIntreface/1.0" )

template < class T > inline PWEIntreface * PWEINTEFACE( T *o )
{
	return qobject_cast< PWEIntreface* >( o );
}

#endif /* PWEINTERFACE_H_ */
