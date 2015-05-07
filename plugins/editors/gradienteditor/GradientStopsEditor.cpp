#include "GradientStopsEditor.h"

/************************************************************
*	GradientStopEditor class implementation
*/

int GradientStopEditor::addStop( int newStopValue, qreal newStopPosition )
{
	return addStop( newStopValue, newStopPosition, true );
}

int GradientStopEditor::moveStop( int stop, int newStopValue, qreal newStopPosition )
{
	if( ( stop < 0 ) || ( stop >= values.size() ) )
		return -1;

	if( newStopValue < 0 )
		newStopValue = 0;

	if( newStopValue > 255 )
		newStopValue = 255;

	if( stop != 0 && stop != ( values.size() - 1 ) )
	{
		if( newStopPosition < 0 )
			newStopPosition = 0;

		if( newStopPosition > 1.0 )
			newStopPosition = 1.0;
	}
	else
		newStopPosition = positions[ stop ];

	if( ( stop == 0 ) || ( stop == ( values.size() - 1 ) ) )
	{
		values[ stop ] = newStopValue;
		if( stopChannel == composite )
			generateBackground( true );
		update();
		emit stopMoved( stop, newStopValue, newStopPosition, stop );
		return stop;
	}

	values.remove( stop );
	positions.remove( stop );

	int newIndex = addStop( newStopValue, newStopPosition, false );
	if( currentMovedStop == stop )
		currentMovedStop = newIndex;

	emit stopMoved( stop, newStopValue, newStopPosition, newIndex );
	if( stopChannel == composite )
		generateBackground( true );
	update();

	return newIndex;
}

bool GradientStopEditor::deleteStop( int stop )
{
	if( ( stop <= 0 ) || ( stop >= ( values.size() - 1 ) ) )
		return false;

	values.remove( stop );
	positions.remove( stop );

	emit stopDeleted( stop );
	if( stopChannel == composite )
		generateBackground( true );
	update();
	return true;
}

int GradientStopEditor::countStops() const
{
	return values.size();
}

int GradientStopEditor::stopValue( int stop ) const
{
	if( ( stop < 0 ) || ( stop >= values.size() ) )
		return -1;

	return values[ stop ];
}

qreal GradientStopEditor::stopPosition( int stop ) const
{
	if( ( stop < 0 ) || ( stop >= values.size() ) )
		return -1;

	return positions[ stop ];
}

bool GradientStopEditor::setStops( const QGradientStops &s )
{
	int countStops = s.size();

	if( countStops < 2 )
		return false;

	values.clear();
	positions.clear();

	switch( stopChannel )
	{
		case redChannel:
			for( int i = 0; i < countStops; i++ )
			{
				positions << s[ i ].first;
				values << s[ i ].second.red();
			}
			break;

		case greenChannel:
			for( int i = 0; i < countStops; i++ )
			{
				positions << s[ i ].first;
				values << s[ i ].second.green();
			}
			break;

		case blueChannel:
			for( int i = 0; i < countStops; i++ )
			{
				positions << s[ i ].first;
				values << s[ i ].second.blue();
			}
			break;

		case composite:
			for( int i = 0; i < countStops; i++ )
			{
				positions << s[ i ].first;
				values << s[ i ].second.alpha();
			}
			break;
	}

	if( positions[ 0 ] != 0.0 )
		positions[ 0 ] = 0.0;

	if( positions[ countStops - 1 ] != 1.0 )
		positions[ countStops - 1 ] = 1.0;

	generateBackground( true );
	update();
	return true;
}

void GradientStopEditor::setCompositeGradientStops( const QGradientStops &s )
{
	if( stopChannel == composite )
	{
		compositeGradient = QLinearGradient( 0, 0, width(), 0 );

		QGradientStops newStops;
		int countStops = s.size();
		for( int i = 0; i < countStops; i++ )
		{
			QColor c( s[ i ].second );
			c.setAlpha( 255 );
			newStops << QGradientStop( s[ i ].first, c );
		}

		//newStops << compositeGradient.stops();
		compositeGradient.setStops( newStops );
		generateBackground( true );
		update();
	}
}

GradientStopEditor::GradientStopEditor( StopChannel _stopChannel, QWidget *parent ):
	QWidget( parent ), stopChannel( _stopChannel ),
	pointPen( QPen( QColor( 255, 255, 255, 191 ), 1 ) ),
	connectionPen( QPen( QColor( 255, 255, 255, 127 ), 2 ) ),
	pointBrush( QBrush( QColor( 191, 191, 191, 127 ) ) ),
	linesPen( QColor( 120, 120, 120, 150 ), 5 ),
	compositeGradient( 0, 0, width(), 0 ),
	currentMovedStop( -1 )
{
	stopChannel = _stopChannel;
	values << 0 << 255;
	positions << 0 << 1;

	if( stopChannel == composite )
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
	}
	else
		setAttribute( Qt::WA_NoBackground );
}

GradientStopEditor::~GradientStopEditor()
{
}

void GradientStopEditor::mousePressEvent( QMouseEvent * event )
{
	switch( event->button() )
	{
		case Qt::LeftButton:
			currentMovedStop = stopFromPoint( event->pos() );
			if( currentMovedStop == -1 )
				currentMovedStop = addStop( event->pos() );
			break;

		case Qt::RightButton:
			deleteStop( stopFromPoint( event->pos() ) );
			break;

		default:
			break;
	}
}

void GradientStopEditor::mouseMoveEvent( QMouseEvent * event )
{
	if( currentMovedStop != -1 )
		moveStop( currentMovedStop, event->pos() );
}

void GradientStopEditor::mouseReleaseEvent( QMouseEvent * event )
{
	switch( event->button() )
	{
		case Qt::LeftButton:
			currentMovedStop = -1;
			break;

		default:
			break;
	}
}

void GradientStopEditor::paintEvent( QPaintEvent * event )
{
	QPainter p( this );
	generateBackground( false );

	p.drawImage( 0, 0, bg );

	p.setPen( QColor( 146, 146, 146 ) );
	p.drawRect( 0, 0, width() - 1, height() - 1 );

	int countStops = values.size();
	if( countStops <= 0 )
		return;

	p.setRenderHints( QPainter::Antialiasing |
						QPainter::TextAntialiasing |
						QPainter::SmoothPixmapTransform );

	p.setPen( connectionPen );
	QPainterPath path;
	path.moveTo( getStopPosition( 0 ) );

	for( int i = 1; i < countStops; i++ )
	{
		QPointF p1 = getStopPosition( i-1 );
		QPointF p2 = getStopPosition( i );
		double distance = p2.x() - p1.x();

		path.cubicTo( p1.x() + distance / 2, p1.y(),
						p1.x() + distance / 2, p2.y(),
						p2.x(), p2.y() );
	}

	p.drawPath( path );

	QPointF stopPosition;
	p.setBrush( pointBrush );
	for( int i = 0; i < countStops; i++ )
	{
		stopPosition = getStopPosition( i );
		QRectF cirleRect = QRectF(
			stopPosition - QPointF( 6, 6 ),
			QSizeF( 12, 12 ) );

		p.setPen( linesPen );
		p.drawLine( (int)(stopPosition.x()), 0, (int)(stopPosition.x()), height() - 1 );

		p.setPen( pointPen );
		p.drawEllipse( cirleRect );
	}
}

QPointF GradientStopEditor::getStopPosition( int stop )
{
	return QPointF( positions[ stop ] * qreal( width() -1 ),
		qreal( 255 - values[ stop ] ) * qreal( height() - 1 ) / 255.0 );
}

int GradientStopEditor::stopFromPoint( const QPoint &point )
{
	int countStops = values.size();
	for( int i = 0; i < countStops; i++ )
	{
		QPainterPath path;
		path.addEllipse( QRectF(
			getStopPosition( i ) - QPointF( 6, 6 ),
			QSizeF( 12, 12 ) ) );
		if( path.contains( point ) )
			return i;
	}

	return -1;
}

int GradientStopEditor::addStop( int newStopValue, qreal newStopPosition, bool emitSignalsAndUpdate )
{
	if( newStopValue < 0 )
		newStopValue = 0;

	if( newStopValue > 255 )
		newStopValue = 255;

	if( newStopPosition < 0 )
		newStopPosition = 0;

	if( newStopPosition > 1.0 )
		newStopPosition = 1.0;

	int countStops = values.size();
	for( int i = 1; i < countStops - 1; i++ )
	{
		if( positions[i] < newStopPosition )
			continue;

		values.insert( i, newStopValue );
		positions.insert( i, newStopPosition );
		if( emitSignalsAndUpdate )
		{
			emit stopAdded( newStopValue, newStopPosition, i );
			if( stopChannel == composite )
				generateBackground( true );
			update();
		}
		return i;
		break;
	}

	values.insert( values.size() - 1, newStopValue );
	positions.insert( positions.size() - 1, newStopPosition );
	if( emitSignalsAndUpdate )
	{
		emit stopAdded( newStopValue, newStopPosition, values.size() - 2 );
		if( stopChannel == composite )
			generateBackground( true );
		update();
	}
	return ( values.size() - 2 );
}

int GradientStopEditor::addStop( const QPointF &position )
{
	int newStopValue = 255 - int( ( qreal( position.y() ) /
									qreal( height() - 1 ) ) * 255.0  );
	qreal newStopPosition = qreal( position.x() ) / qreal( width() - 1 );
	return addStop( newStopValue, newStopPosition );
}

int GradientStopEditor::moveStop( int stop, const QPointF &newPos )
{
	return moveStop( stop, 255 - int( ( qreal( newPos.y() ) / qreal( height() - 1 ) ) * 255.0  ),
		qreal( newPos.x() ) / qreal( width() - 1 ) );
}

void GradientStopEditor::generateBackground( bool forceGenerate )
{
	if( forceGenerate || bg.isNull() || ( bg.size() != size() ) )
	{
		if (stopChannel == composite )
		{
			bg = QImage( size(), QImage::Format_ARGB32_Premultiplied );
			bg.fill(0);

			QPainter p( &bg );
			p.fillRect( rect(), compositeGradient );
			p.setCompositionMode( QPainter::CompositionMode_DestinationIn );
			QLinearGradient fade( 0, 0, 0, height() );
			fade.setColorAt( 0, QColor(0, 0, 0, 255) );
			fade.setColorAt( 1, QColor(0, 0, 0, 0) );
			p.fillRect( rect(), fade );

		}
		else
		{
			bg = QImage( size(), QImage::Format_RGB32 );

			QLinearGradient shade( 0, 0, 0, height() );
			shade.setColorAt( 1, Qt::black );

			if( stopChannel == redChannel )
				shade.setColorAt( 0, Qt::red );
			else if( stopChannel == greenChannel )
				shade.setColorAt( 0, Qt::green );
			else
				shade.setColorAt( 0, Qt::blue );

			QPainter p( &bg );
			p.fillRect( rect(), shade );
		}
	}
}

/************************************************************
*	GradientStopsEditor class implementation
*/

const QGradientStops GradientStopsEditor::stops() const
{
	QGradientStops s;

	int countStops = r.countStops();
	for( int i = 0; i < countStops; i++ )
	{
		s << QGradientStop( r.stopPosition( i ),
				QColor( r.stopValue( i ), g.stopValue( i ),
						b.stopValue( i ) ,c.stopValue( i ) ) );
	}

	return s;
}

bool GradientStopsEditor::setStops( const QGradientStops &s )
{
	int countStops = s.size();
	if( countStops < 2 ) return false;
	bool oldValueOfHandleSignals = handleSignals;
	handleSignals = false;
	r.setStops( s );
	g.setStops( s );
	b.setStops( s );
	c.setStops( s );
	c.setCompositeGradientStops( s );
	handleSignals = oldValueOfHandleSignals;
	emit stopsChanged( s );
	return true;
}

GradientStopsEditor::GradientStopsEditor( QWidget *parent ):QWidget( parent ),
	r( GradientStopEditor::redChannel, this ),
	g( GradientStopEditor::greenChannel, this ),
	b( GradientStopEditor::blueChannel, this ),
	c( GradientStopEditor::composite, this ),
	handleSignals( true )
{

	QBoxLayout *layout = new QBoxLayout( QBoxLayout::TopToBottom, this );
	layout->setSpacing( 1 );
	layout->setMargin( 1 );
	layout->addWidget( &r );
	layout->addWidget( &g );
	layout->addWidget( &b );
	layout->addWidget( &c );

	setMinimumSize( QSize( 192, 128 ) );

	connect( &r, SIGNAL( stopMoved( int, int, qreal, int ) ),
			this, SLOT( redStopMoved(  int, int, qreal, int ) ) );
	connect( &r, SIGNAL( stopAdded( int, qreal, int ) ),
			this, SLOT( redStopAdded( int, qreal, int ) ) );
	connect( &r, SIGNAL( stopDeleted( int ) ),
			this, SLOT( redStopDeleted( int ) ) );

	connect( &g, SIGNAL( stopMoved( int, int, qreal, int ) ),
			this, SLOT( greenStopMoved(  int, int, qreal, int ) ) );
	connect( &g, SIGNAL( stopAdded( int, qreal, int ) ),
			this, SLOT( greenStopAdded( int, qreal, int ) ) );
	connect( &g, SIGNAL( stopDeleted( int ) ),
			this, SLOT( greenStopDeleted( int ) ) );

	connect( &b, SIGNAL( stopMoved( int, int, qreal, int ) ),
			this, SLOT( blueStopMoved(  int, int, qreal, int ) ) );
	connect( &b, SIGNAL( stopAdded( int, qreal, int ) ),
			this, SLOT( blueStopAdded( int, qreal, int ) ) );
	connect( &b, SIGNAL( stopDeleted( int ) ),
			this, SLOT( blueStopDeleted( int ) ) );

	connect( &c, SIGNAL( stopMoved( int, int, qreal, int ) ),
			this, SLOT( compositeStopMoved(  int, int, qreal, int ) ) );
	connect( &c, SIGNAL( stopAdded( int, qreal, int ) ),
			this, SLOT( compositeStopAdded( int, qreal, int ) ) );
	connect( &c, SIGNAL( stopDeleted( int ) ),
			this, SLOT( compositeStopDeleted( int ) ) );
}

GradientStopsEditor::~GradientStopsEditor()
{
}

void GradientStopsEditor::redStopMoved( int stop, int value, qreal position, int newIndex )
{
	if( !handleSignals ) return;
	handleSignals = false;
	g.moveStop( stop, g.stopValue( stop ), position );
	b.moveStop( stop, b.stopValue( stop ), position );
	c.moveStop( stop, c.stopValue( stop ), position );

	const QGradientStops &s = stops();
	c.setCompositeGradientStops( s );
	handleSignals = true;
	emit stopsChanged( s );
}

void GradientStopsEditor::redStopAdded( int value, qreal position, int index )
{
	if( !handleSignals ) return;
	handleSignals = false;
	g.addStop( value, position );
	b.addStop( value, position );
	c.addStop( value, position );

	const QGradientStops &s = stops();
	c.setCompositeGradientStops( s );
	handleSignals = true;
	emit stopsChanged( s );
}

void GradientStopsEditor::redStopDeleted( int stop )
{
	if( !handleSignals ) return;
	handleSignals = false;
	g.deleteStop( stop );
	b.deleteStop( stop );
	c.deleteStop( stop );

	const QGradientStops &s = stops();
	c.setCompositeGradientStops( s );
	handleSignals = true;
	emit stopsChanged( s );
}

void GradientStopsEditor::greenStopMoved( int stop, int value, qreal position, int newIndex )
{
	if( !handleSignals ) return;
	handleSignals = false;
	r.moveStop( stop, r.stopValue( stop ), position );
	b.moveStop( stop, b.stopValue( stop ), position );
	c.moveStop( stop, c.stopValue( stop ), position );

	const QGradientStops &s = stops();
	c.setCompositeGradientStops( s );
	handleSignals = true;
	emit stopsChanged( s );
}

void GradientStopsEditor::greenStopAdded( int value, qreal position, int index )
{
	if( !handleSignals ) return;
	handleSignals = false;
	r.addStop( value, position );
	b.addStop( value, position );
	c.addStop( value, position );

	const QGradientStops &s = stops();
	c.setCompositeGradientStops( s );
	handleSignals = true;
	emit stopsChanged( s );
}

void GradientStopsEditor::greenStopDeleted( int stop )
{
	if( !handleSignals ) return;
	handleSignals = false;
	r.deleteStop( stop );
	b.deleteStop( stop );
	c.deleteStop( stop );

	const QGradientStops &s = stops();
	c.setCompositeGradientStops( s );
	handleSignals = true;
	emit stopsChanged( s );
}

void GradientStopsEditor::blueStopMoved( int stop, int value, qreal position, int newIndex )
{
	if( !handleSignals ) return;
	handleSignals = false;
	r.moveStop( stop, r.stopValue( stop ), position );
	g.moveStop( stop, g.stopValue( stop ), position );
	c.moveStop( stop, c.stopValue( stop ), position );

	const QGradientStops &s = stops();
	c.setCompositeGradientStops( s );
	handleSignals = true;
	emit stopsChanged( s );
}

void GradientStopsEditor::blueStopAdded( int value, qreal position, int index )
{
	if( !handleSignals ) return;
	handleSignals = false;
	r.addStop( value, position );
	g.addStop( value, position );
	c.addStop( value, position );

	const QGradientStops &s = stops();
	c.setCompositeGradientStops( s );
	handleSignals = true;
	emit stopsChanged( s );
}

void GradientStopsEditor::blueStopDeleted( int stop )
{
	if( !handleSignals ) return;
	handleSignals = false;
	r.deleteStop( stop );
	g.deleteStop( stop );
	c.deleteStop( stop );

	const QGradientStops &s = stops();
	c.setCompositeGradientStops( s );
	handleSignals = true;
	emit stopsChanged( s );
}

void GradientStopsEditor::compositeStopMoved( int stop, int value, qreal position, int newIndex )
{
	if( !handleSignals ) return;
	handleSignals = false;
	r.moveStop( stop, r.stopValue( stop ), position );
	g.moveStop( stop, g.stopValue( stop ), position );
	b.moveStop( stop, b.stopValue( stop ), position );

	const QGradientStops &s = stops();
	c.setCompositeGradientStops( s );
	handleSignals = true;
	emit stopsChanged( s );
}

void GradientStopsEditor::compositeStopAdded( int value, qreal position, int index )
{
	if( !handleSignals ) return;
	handleSignals = false;
	r.addStop( value, position );
	g.addStop( value, position );
	b.addStop( value, position );

	const QGradientStops &s = stops();
	c.setCompositeGradientStops( s );
	handleSignals = true;
	emit stopsChanged( s );
}

void GradientStopsEditor::compositeStopDeleted( int stop )
{
	if( !handleSignals ) return;
	handleSignals = false;
	r.deleteStop( stop );
	g.deleteStop( stop );
	b.deleteStop( stop );

	const QGradientStops &s = stops();
	c.setCompositeGradientStops( s );
	handleSignals = true;
	emit stopsChanged( s );
}
