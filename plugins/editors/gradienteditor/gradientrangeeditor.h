#ifndef __GRADIENT_RANGE_EDITOR__
#define __GRADIENT_RANGE_EDITOR__

#include <QWidget>
#include <QMatrix>
#include <QPainter>
#include <QGradient>
#include <QMouseEvent>
#include <QPainterPath>

#include "./../../../interfaces/GradientEditorInterface.h"

class GradientRangeEditor:public QWidget
{
	Q_OBJECT

	signals:
		void startPointChanged( const QPointF &p );
		void endPointChanged( const QPointF &p );

	public:
		virtual QPointF startPoint() const;
		virtual void setStartPoint( const QPointF &p );

		virtual QPointF endPoint() const;
		virtual void setEndPoint( const QPointF &p );

		virtual qreal angle() const;
		virtual void setAngle( const qreal a );

		virtual qreal radius() const;
		virtual void setRadius( const qreal rad );

		virtual QGradient gradient() const;
		virtual void setGradient( const QGradient &g );

				GradientRangeEditor( QWidget *parent = 0 );
		virtual ~GradientRangeEditor();

	protected:
		virtual void paintEvent( QPaintEvent * event );

		virtual void mousePressEvent( QMouseEvent * event );
		virtual void mouseMoveEvent( QMouseEvent * event );
		virtual void mouseReleaseEvent( QMouseEvent * event );

	private:
		enum MovedPoint
		{
			startMoved,
			endMoved,
			nothingMoved
		};

		QPointF _startPoint, _endPoint;
		QPen pointPen;
    	QBrush pointBrush;
		MovedPoint movedPoint;
		QGradient::Type gradientType;
		QGradient::Spread spreadType;
		QGradientStops stops;
};

#endif /* __GRADIENT_RANGE_EDITOR__ */
