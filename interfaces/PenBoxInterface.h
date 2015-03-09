/**
* PenBoxInterface.h
*
*  Created on: 04.05.2009
*      Author: Alexey kukushkin
*/

#ifndef PENBOXINTERFACE_H_
#define PENBOXINTERFACE_H_
#include <QObject>
#include <QApplication>

class PenBoxInterface:public QWidget
{
	signals:
		void penChanged( const QPen &p );

	public slots:

		void setPen( const QPen &p );

	public:

		virtual QPen pen() const = 0;

		PenBoxInterface():QWidget(0){};

		virtual ~PenBoxInterface(){};

		virtual void paintEvent( QPaintEvent *event ) = 0;


};

Q_DECLARE_INTERFACE( PenBoxInterface, "com.Tailer.MultiFusion.PenBoxInterface/1.0" )

template < class T > inline PenBoxInterface * PENBOXINTERFACE( T *o )
{
	return qobject_cast< PenBoxInterface* >( o );
}

#endif
