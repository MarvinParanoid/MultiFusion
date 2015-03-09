#include <QPointF>
#include <QLineF>
#include <math.h>

template < class T > inline T interpolate( const T from, const T to, const qreal width, const qreal pos )
{
	return ( from + ( ( to - from ) / width ) * pos );
	//return from - ( to - from ) * ( pos ) * ( pos - width * 2.0 );
}

inline QPointF projectPointToLine( const QPointF &point, const QLineF &line )
{
	QLineF n = line.normalVector();
	n.translate( -line.p1() );
	n.translate( point );

	QPointF intersectPoint;
	line.intersect( n, &intersectPoint );

	return intersectPoint;
}

inline float lengthFromPointToLine( const QPointF &point, const QLineF &line )
{
	QLineF n = line.normalVector();
	n.translate( -line.p1() );
	n.translate( point );

	QPointF intersectPoint;
	line.intersect( n, &intersectPoint );

	QPointF dp = intersectPoint - line.p1();
	QLineF u = line.unitVector();

	u.translate( -u.p1() );

	qreal t = dp.x() * u.p2().x() + dp.y() * u.p2().y();

	if( t < 0 )
		intersectPoint = line.p1();
	else if( t > line.length() )
		intersectPoint = line.p2();

	return QLineF( intersectPoint, point ).length();
}
