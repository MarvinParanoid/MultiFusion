/**
* BrushBoxInterface.h
*
*  Created on: 04.05.2009
*      Author: Alexey Kukushkin
*/

#ifndef BRUSHBOXINTERFACE_H_
#define BRUSHBOXINTERFACE_H_

#include <QWidget>
#include <QObject>
#include <QApplication>

class BrushBoxInterface:public QWidget
{
	signals:
		void brushChanged( const QBrush &b );

	public slots:
		virtual void setBrush( const QBrush &b ) = 0;

	public:
		virtual QBrush brush() const = 0;

		BrushBoxInterface():QWidget(0){};

		virtual ~BrushBoxInterface(){};

		virtual void paintEvent( QPaintEvent *event ) = 0;
};

Q_DECLARE_INTERFACE( BrushBoxInterface, "com.Tailer.MultiFusion.BrushBoxInterface/1.0" )

template < class T > inline BrushBoxInterface * BRUSHBOXINTERFACE( T *o )
{
	return qobject_cast< BrushBoxInterface* >( o );
}
#endif
