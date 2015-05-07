#include "GradientRangeEditor.h"

QPointF GradientRangeEditor::startPoint() const
{
	return _startPoint;
}

void GradientRangeEditor::setStartPoint( const QPointF &p )
{
	_startPoint = p;

	if( _startPoint.x() < 0.0 )
		_startPoint.rx() = 0.0;

	if( _startPoint.x() > 1.0 )
		_startPoint.rx() = 1.0;

	if( _startPoint.y() < 0.0 )
		_startPoint.ry() = 0.0;

	if( _startPoint.y() > 1.0 )
		_startPoint.ry() = 1.0;

	emit startPointChanged( _startPoint );
	update();
}

QPointF GradientRangeEditor::endPoint() const
{
	return _endPoint;
}

void GradientRangeEditor::setEndPoint( const QPointF &p )
{
	_endPoint = p;

	if( _endPoint.x() < 0.0 )
		_endPoint.rx() = 0.0;

	if( _endPoint.x() > 1.0 )
		_endPoint.rx() = 1.0;

	if( _endPoint.y() < 0.0 )
		_endPoint.ry() = 0.0;

	if( _endPoint.y() > 1.0 )
		_endPoint.ry() = 1.0;

	emit endPointChanged( _endPoint );
	update();
}

qreal GradientRangeEditor::angle() const
{
	QLineF l( _startPoint, _endPoint );
	qreal a = l.angle( QLineF( 0.0, 0.0, 1.0, 0.0 ) );
	if( l.dy() > 0 )
		a = 360 - a;
	return ( -a );
}

void GradientRangeEditor::setAngle( const qreal a )
{
	QLineF l = QLineF( _startPoint, _endPoint ) * QMatrix().rotate( a - angle() );
	setStartPoint( l.p1() );
	setEndPoint( l.p2() );
}

qreal GradientRangeEditor::radius() const
{
	return QLineF( _startPoint, _endPoint ).length();
}

void GradientRangeEditor::setRadius( const qreal rad )
{
	QLineF l( _startPoint, _endPoint );
	l.setLength( rad );
	setStartPoint( l.p1() );
	setEndPoint( l.p2() );
}

QGradient GradientRangeEditor::gradient() const
{
	QGradient g;

	switch( gradientType )
	{
		case QGradient::LinearGradient:
			g = QLinearGradient( _startPoint, _endPoint );
			break;

		case QGradient::RadialGradient:
		{
			QLineF l( _startPoint, _endPoint );
			if( l.length() > 0.5 )
				l.setLength( 0.5 - 0.001 );
			g = QRadialGradient( l.p1(), 0.5, l.p2() );
			break;
		}

		default:
			g = QConicalGradient( _startPoint, angle() );
			break;
	}

	g.setStops( stops );
	g.setSpread( spreadType );
	return g;
}

void GradientRangeEditor::setGradient( const QGradient &g )
{
	gradientType = g.type();
	stops = g.stops();
	spreadType = g.spread();
	switch( gradientType )
	{
		case QGradient::LinearGradient:
			setStartPoint( ( ( QLinearGradient& )g ).start() );
			setEndPoint( ( ( QLinearGradient& )g ).finalStop() );
			break;

		case QGradient::RadialGradient:
			setStartPoint( ( ( QRadialGradient& )g ).center() );
			setEndPoint( ( ( QRadialGradient& )g ).focalPoint() );
			break;

		default:
			setStartPoint( ( ( QConicalGradient& )g ).center() );
			qreal a = ( ( QConicalGradient& )g ).angle();
			QLineF l( QLineF( 0.0, 0.0, 0.5, 0.0 ) *
							QMatrix().rotate( a ) );
			l.translate( _startPoint );
			setEndPoint( l.p2() );
			break;
	}
}

GradientRangeEditor::GradientRangeEditor( QWidget *parent ):QWidget( parent ),
	pointPen( QPen( QColor( 255, 255, 255, 191 ), 1 ) ),
	pointBrush( QBrush( QColor( 191, 191, 191, 127 ) ) ),
	movedPoint( nothingMoved )
{
	QPixmap pm( 20, 20 );

	QPainter pmp( &pm );
		pmp.fillRect( 0, 0, 10, 10, Qt::lightGray );
		pmp.fillRect( 10, 10, 10, 10, Qt::lightGray );
		pmp.fillRect( 0, 10, 10, 10, Qt::darkGray );
		pmp.fillRect( 10, 0, 10, 10, Qt::darkGray );
	pmp.end();

	QPalette pal = palette();
	pal.setBrush( backgroundRole(), QBrush( pm ) );
	setAutoFillBackground( true );
	setPalette( pal );

	setGradient( QLinearGradient( QPointF( 0.1, 0.1 ), QPointF( 0.9, 0.9 ) ) );
	setMinimumSize( QSize( 64, 64 ) );
}

GradientRangeEditor::~GradientRangeEditor()
{
}

void GradientRangeEditor::paintEvent( QPaintEvent * event )
{
	QPainter p( this );

	p.fillRect( 0, 0, width(), height(), prepareGradientToPaint( gradient(), rect() ) );

	p.setRenderHints( QPainter::Antialiasing |
						QPainter::TextAntialiasing |
						QPainter::SmoothPixmapTransform );

	p.setBrush( pointBrush );
	p.setPen( pointPen );

	QPointF start = QPointF( _startPoint.x() * width(), _startPoint.y() * height() );
	QPointF end = QPointF( _endPoint.x() * width(), _endPoint.y() * height() );

	p.drawEllipse( QRectF( start - QPointF( 12, 12 ), QSizeF( 24, 24 ) ) );
	p.drawEllipse( QRectF( end - QPointF( 12, 12 ), QSizeF( 24, 24 ) ) );
}

void GradientRangeEditor::mousePressEvent( QMouseEvent * event )
{
	switch( event->button() )
	{
		case Qt::LeftButton:
		{
			{
				QPainterPath path;
				path.addEllipse( QRectF(
					QPointF( _startPoint.x() * width(), _startPoint.y() * height() ) -
						QPointF( 12, 12 ), QSizeF( 24, 24 ) ) );
				if( path.contains( event->pos() ) )
				{
					movedPoint = startMoved;
					break;
				}
			}

			{
				QPainterPath path;
				path.addEllipse( QRectF(
					QPointF( _endPoint.x() * width(), _endPoint.y() * height() ) -
						QPointF( 12, 12 ), QSizeF( 24, 24 ) ) );
				if( path.contains( event->pos() ) )
				{
					movedPoint = endMoved;
					break;
				}
			}
			break;
		}

		default:
			break;
	}
}

void GradientRangeEditor::mouseMoveEvent( QMouseEvent * event )
{
	QPointF pos = QPointF(  qreal( event->pos().x() ) / ( width() - 1 ),
							qreal( event->pos().y() ) / ( height() - 1) );

	switch( movedPoint )
	{
		case startMoved:
			setStartPoint( pos );
			break;

		case endMoved:
			setEndPoint( pos );
			break;

		default:
			break;
	}
}

void GradientRangeEditor::mouseReleaseEvent( QMouseEvent * event )
{
	switch( event->button() )
	{
		case Qt::LeftButton:
			movedPoint = nothingMoved;
			break;

		default:
			break;
	}
}
