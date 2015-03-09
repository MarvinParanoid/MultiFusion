#ifndef __GRADIENT_STOPS_EDITOR_H__
#define __GRADIENT_STOPS_EDITOR_H__

#include <QImage>
#include <QWidget>
#include <QPainter>
#include <QGradient>
#include <QBoxLayout>
#include <QMouseEvent>
#include <QPainterPath>

class GradientStopEditor:public QWidget
{
	Q_OBJECT

	signals:
		void stopMoved( int stop, int value, qreal position, int newIndex );
		void stopAdded( int value, qreal position, int index );
		void stopDeleted( int stop );

	public slots:
		virtual int addStop( int newStopValue, qreal newStopPosition );
		virtual int moveStop( int stop, int newStopValue, qreal newStopPosition );
		virtual bool deleteStop( int stop );

	public:
		enum StopChannel
		{
			redChannel,
			greenChannel,
			blueChannel,
			composite
		};

		virtual int countStops() const;
		virtual int stopValue( int stop ) const;
		virtual qreal stopPosition( int stop ) const;

		virtual bool setStops( const QGradientStops &s );
		virtual void setCompositeGradientStops( const QGradientStops &s );

				GradientStopEditor( StopChannel _stopChannel, QWidget *parent = 0 );
		virtual ~GradientStopEditor();

	protected:
		virtual void mousePressEvent( QMouseEvent * event );
		virtual void mouseMoveEvent( QMouseEvent * event );
		virtual void mouseReleaseEvent( QMouseEvent * event );

		virtual void paintEvent( QPaintEvent * event );

	private:
		QVector< int > values;
		QVector< qreal > positions;
		QImage bg;
		StopChannel stopChannel;

		QPen pointPen;
    	QPen connectionPen;
    	QBrush pointBrush;
		QPen linesPen;
		QLinearGradient compositeGradient;

		int currentMovedStop;

		QPointF getStopPosition( int stop );
		int stopFromPoint( const QPoint &point );
		int addStop( int newStopValue, qreal newStopPosition, bool emitSignalsAndUpdate );
		int addStop( const QPointF &position );
		int moveStop( int stop, const QPointF &newPos );
		void generateBackground( bool forceGenerate );
};

class GradientStopsEditor:public QWidget
{
	Q_OBJECT

	signals:
		void stopsChanged( const QGradientStops &stops );

	public:
		virtual const QGradientStops stops() const;
		virtual bool setStops( const QGradientStops &s );

				GradientStopsEditor( QWidget *parent = 0 );
		virtual ~GradientStopsEditor();

	private:
		GradientStopEditor r;
		GradientStopEditor g;
		GradientStopEditor b;
		GradientStopEditor c;

		bool handleSignals;

	private slots:
		void redStopMoved( int stop, int value, qreal position, int newIndex );
		void redStopAdded( int value, qreal position, int index );
		void redStopDeleted( int stop );

		void greenStopMoved( int stop, int value, qreal position, int newIndex );
		void greenStopAdded( int value, qreal position, int index );
		void greenStopDeleted( int stop );

		void blueStopMoved( int stop, int value, qreal position, int newIndex );
		void blueStopAdded( int value, qreal position, int index );
		void blueStopDeleted( int stop );

		void compositeStopMoved( int stop, int value, qreal position, int newIndex );
		void compositeStopAdded( int value, qreal position, int index );
		void compositeStopDeleted( int stop );
};

#endif /* __GRADIENT_STOPS_EDITOR_H__ */
