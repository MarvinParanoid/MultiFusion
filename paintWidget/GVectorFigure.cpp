#include <QMessageBox>
#include "BrushEditorInterface.h"
#include "GVectorFigure.h"
#include "../PluginTool/Plugin.h"
#include <QMessageBox>
#include "Utils.h"

void GVectorFigure::setHideLines(int value)
{
	hideLines = value;
}

int GVectorFigure::getHideLines()
{
	return hideLines;
}


void GVectorFigure::toNormal()
{
	int countFrames = properties.countFrames();
	for(int index=0; index<countFrames; index++ )
	{
		int countPoints = properties[index].points.size();
		properties[index].points[1] = properties[index].points[0];
		properties[index].points[countPoints - 2] = properties[index].points[countPoints -1];
		for(int i = 2; i<countPoints - 2; i+=3)
			properties[index].points[i] = properties[index].points[i+2] = properties[index].points[i+1];
	}
	_linesType = gNormal;
	showBiese = false;
}

void GVectorFigure::toSpline()
{
	_linesType = gSplines;
	showBiese = true;
}

bool GVectorFigure::isSpline()
{
	return (_linesType == gSplines)?(true):(false);
}

void GVectorFigure::setShowBezier(bool value)
{
	showBiese = value;
}

bool GVectorFigure::isShowBezier()
{
	return showBiese;
}

GObjectInterface* GVectorFigure::copyObject()
{
	GVectorFigure* f = new GVectorFigure(this);
	f->setObjectName(objectName());
	return f;
}

bool GVectorFigure::isContainer() const
{
	return false;
}

void GVectorFigure::deleteFrame( int position )
{
	properties.remove( getIndexFrameByPosition((qreal)(position)) );
}

void GVectorFigure::deleteFrames( int frame )
{
	int index = getIndexFrameByPosition((qreal)(frame));
	for(int i=0; i<properties.countFrames(); i++)
	{
		if(i != index)
			properties.remove(i);
	}
	if(properties.countFrames()>1)
	{
		while(properties.countFrames()!=1)
		{
			properties.remove(0);
		}
	}
}

void GVectorFigure::addFrame( int position ,bool visible)
{
	int count = countFrames();
	int pos = -1;
	int item = 0;
	for(int i=0; i<count; i++)
	{
		int p = properties[i].position;
		if(p < position && p > pos)
		{
			item = i;
			pos = p;
		}
	}


	if(position == 0)
	{
		properties.insert( properties[0], 0);
		properties[0].visible = false;
		properties[0].isTransform = false;
		properties[0].position = position;
	}
	else
	{
		properties.insert( properties[ item ], item + 1 );
		properties[ item + 1 ].visible = visible;
		properties[ item + 1 ].isTransform = false;
		properties[ item + 1 ].position = position;
	}
}

void GVectorFigure::addFrameWithPoints( int position, QPolygonF polF)
{
	GProperties props;
	props.isTransform = false;
	props.visible = true;
	props.blocked = false;
	props.alpha = 255;
	props.points = polF;
	props.position = position;

	if(position > properties[0].position)
		properties.append(props);
	else
		properties.insert(props, 0);
}

void GVectorFigure::cloneFrameBefore( int frame, int pasteTo )
{
	if( ( frame < 0 ) || ( frame >= countFrames() ) )
		return;

	if( ( pasteTo < 0 ) || ( pasteTo >= countFrames() ) )
		return;

	properties.insert( properties[ frame ], pasteTo );

	if( pasteTo <= _frame )
		_frame += 1.0;
}

void GVectorFigure::cloneFrameAfter( int frame, int pasteTo )
{
	if( ( frame < 0 ) || ( frame >= countFrames() ) )
		return;

	if( ( pasteTo < 0 ) || ( pasteTo >= countFrames() ) )
		return;

	properties.insert( properties[ frame ], pasteTo + 1 );

	if( ( pasteTo + 1 ) <= _frame )
		_frame += 1.0;
}

GVectorFigure::GLinesType GVectorFigure::type() const
{
	return _linesType;
}

bool GVectorFigure::isEditable() const
{
	if( getIndexFrameByPosition(_frame) == -1 )
		return false;

	return true;
}

int GVectorFigure::getAlpha()
{
	return properties[ getIndexFrameByPosition(_frame) ].alpha;
}

void GVectorFigure::setAlpha( int _alpha )
{
	if( !isEditable() )
		return;

	properties[ getIndexFrameByPosition(_frame) ].alpha = _alpha;
}

bool GVectorFigure::isBlocked() const
{
	int i = getIndexFrameByPosition(_frame);
	return properties[i].blocked;
}

void GVectorFigure::setBlocked( bool blocked )
{
	if( !isEditable() )
		return;

	properties[getIndexFrameByPosition(_frame)].blocked = blocked;
}

bool GVectorFigure::isTransformed(int frame) const
{
	int i = getIndexFrameByPosition(frame);
	if(i != -1)
		return properties[i].isTransform;
	return false;
}

void GVectorFigure::setTransformed(int frame, bool transform)
{
	int i = getIndexFrameByPosition(frame);
	if(i != -1)
		properties[i].isTransform = transform;
}

bool GVectorFigure::isVisible() const
{
	int i = getIndexFrameByPosition(_frame);
	return properties[i].visible;
}

void GVectorFigure::setVisible( bool visible )
{
	if( !isEditable() )
		return;

	properties[getIndexFrameByPosition(_frame)].visible = visible;
}

GObject* GVectorFigure::contains( const QPoint &p )
{
	if( !isEditable() )
		return 0;

	const GProperties &props = properties[ getIndexFrameByPosition(_frame) ];
	QPainterPath path;

	path.setFillRule( Qt::WindingFill );

	if( _linesType == gSplines )
	{
		if( props.points.size() < 4 )
			return 0;

		int countSplinePoints = ( ( props.points.size() - 1 ) / 3 ) * 3;
		path.moveTo( props.points[0] );

		for( int i = 1; i < ( countSplinePoints + 1 ); i += 3 )
			path.cubicTo( props.points[i], props.points[i + 1], props.points[i + 2] );

		if( closed )
			path.closeSubpath();
	}
	else
		path.addPolygon( props.points );

	if( path.contains( p ) )
		return this;

	return 0;
}

qreal GVectorFigure::frame() const
{
	return _frame;
}

void GVectorFigure::setFrame( qreal f )
{
	_frame = f;
}

int GVectorFigure::countFrames() const
{
	return properties.countFrames();
}

QRectF GVectorFigure::boundingRect()
{
	if( countFrames() <= 0 )
		return QRectF();

	getIndexFrames();
	QPolygonF points = currentFrame().points;


	QRectF rect = points.boundingRect();
	if( rect.isNull() )
		return QRectF( points[0] - QPointF( 10, 10 ), QSize( 10, 10 ) );

	return rect;
}

int GVectorFigure::addPoint( const QPoint &p )
{
	if( !isEditable() )
		return -1;

	if( _linesType == gNormal )
		return addPointToPolygon( p );

	return addPointToSpline( p );
}

int GVectorFigure::addPointToEnd( const QPoint &p )
{
	if( !isEditable() )
		return -1;

	if( _linesType == gNormal )
		return addPointToEndOfPolygon( p );

	return addPointToSpline( p );
}

bool GVectorFigure::deletePoint( int index )
{
	if( !isEditable() )
		return false;

	int countFrames = properties.countFrames();
	int countPoints = properties[ 0 ].points.size();

	if( ( index < 0 ) || ( index >= countPoints ) )
		return false;

	if( ( countPoints <= 4 ) || ( ( index % 3 ) != 0 ) )
		return false;

	int firstIndex;
	if(index == 0)
		firstIndex = 0;
	else if(index == countPoints-1)
		firstIndex = -2;
	else
		firstIndex = -1;

	for( int i = 0; i < countFrames; i++ )
		for( int j = 0; j < 3; j++ )
		{
			properties[i].points.remove( index + firstIndex);
		}


	return true;
}

int GVectorFigure::getIndexFrameByPosition(qreal frame) const
{
	for(int i = 0; i<properties.countFrames();i++)
	{
		if(properties[i].position == int(frame))
		{
			return i;
		}
	}
	return -1;
}

void GVectorFigure::move( qreal dx, qreal dy )
{
	if( !isEditable() )
		return;

	properties[ getIndexFrameByPosition(_frame)].points.translate( dx, dy );
}

void GVectorFigure::scale( qreal sx, qreal sy, const QPointF &scaleCenter )
{
	if( !isEditable() )
		return;

	QMatrix m;

	m.translate( scaleCenter.x(), scaleCenter.y() );
	m.scale( sx, sy );
	m.translate( -scaleCenter.x(), -scaleCenter.y() );
	int i = getIndexFrameByPosition(_frame);
	properties[i].points =
		m.map( properties[i].points );
}

void GVectorFigure::sheare( qreal sx, qreal sy, const QPointF &shearPoint )
{
	if( !isEditable() )
		return;

	QMatrix m;

	m.translate( shearPoint.x(), shearPoint.y() );
	m.shear( sx, sy );
	m.translate( -shearPoint.x(), -shearPoint.y() );

	int i = getIndexFrameByPosition(_frame);
	properties[ i ].points =
		m.map( properties[ i ].points );
}

void GVectorFigure::rotate( qreal angle, const QPointF &center )
{
	if( !isEditable() )
		return;

	QMatrix m;

	m.translate( center.x(), center.y() );
	m.rotate( angle );
	m.translate( -center.x(), -center.y() );

	int i = getIndexFrameByPosition(_frame);
	properties[ i ].points =
		m.map( properties[ i ].points );
}

QPolygonF GVectorFigure::points(int frame)
{
	int i = getIndexFrameByPosition(frame);
	if(i != -1)
		return properties[ i ].points;

	return QPolygonF();
}

void GVectorFigure::movePoint( int index, const QPointF &newPosition )
{
	if( !isEditable() )
		return;
	int i = getIndexFrameByPosition(_frame);
	int countPoints = properties[ i ].points.size();
	if( ( index < 0 ) || ( index >= countPoints ) )
		return;

	properties[ i ].points[ index ] = newPosition;
}

void GVectorFigure::cloneFrameToAll( int frame )
{
	if( !isEditable() ) return;

	int j = getIndexFrameByPosition(qreal(frame));
	int count = countFrames();

	for( int i = 0; i < count; i++ )
	{
		if( properties[i].position != frame )
		{
			int pos = properties[i].position;
			properties[i] = properties[ j ];
			properties[i].position = pos;
		}
	}
}

QPen GVectorFigure::pen() const
{
	return _pen;
}

QBrush GVectorFigure::brush() const
{
	return _brush;
}

void GVectorFigure::setPen( const QPen &p )
{
	if( _pen != p )
	{
		alphaPen = p;
		_pen = p;
	}
}

void GVectorFigure::setBrush( const QBrush &b )
{
	if( _brush != b )
	{
		alphaBrush = b;
		_brush = b;
	}
}

bool GVectorFigure::isClosed() const
{
	return closed;
}

void GVectorFigure::setClosed( bool _closed )
{
	closed = _closed;
}

void GVectorFigure::save( QDataStream &stream ) const
{
	stream << objectName();
	stream << _frame;
	stream << (int) _linesType;
	stream << _pen;
	stream << _brush;
	stream << closed;
	stream << showBiese;
	stream << hideLines;
	properties.save( stream );
}

void GVectorFigure::load( QDataStream &stream )
{
	QString name;
	int linesType = 0;
	stream >> name;

	setObjectName( name );

	stream >> _frame;
	stream >> linesType;
	_linesType = ( GLinesType ) linesType;
	stream >> _pen;
	stream >> _brush;
	stream >> closed;
	stream >> showBiese;
	stream >> hideLines;
	properties.load( stream );
}

void GVectorFigure::paint( QPainter &p )
{

	if( countFrames() <= 0 ) return;


	if(!getIndexFrames()) return;
	GProperties props = currentFrame();

	if( !props.visible ) return;

	p.setPen( interpolatedPen() );
	p.setBrush( prepareBrushToPaint( interpolatedBrush(),
				props.points.boundingRect().toRect() ) );
	paintObject( p, props );
}

GVectorFigure::GVectorFigure(  const QPolygonF &points, bool _spline, bool _closed, int position  ):
	_frame( 0 ),
	_linesType( ( _spline ) ? ( gSplines ) : ( gNormal ) ),
	closed( _closed ), hideLines(0)
{
	GProperties props;
	props.isTransform = false;
	props.visible = true;
	props.blocked = false;
	props.alpha = 255;
	props.points = points;
	props.position = position;
	properties.append( props );
}

GVectorFigure::GVectorFigure( GVectorFigure* vf )
{

	_frame = vf->_frame;
	_linesType = vf->_linesType;
	closed = vf->closed;
	alphaPen = vf->alphaPen;
	alphaBrush = vf->alphaBrush;
	_pen = vf->_pen;
	_brush = vf->_brush;
	showBiese = vf->showBiese;
	hideLines = vf->hideLines;
	int count = vf->properties.countFrames();
	for(int i=0; i<count; i++)
	{
		properties.append(vf->properties[i]);
	}

}

GVectorFigure::~GVectorFigure()
{
}


bool GVectorFigure::getIndexFrames()
{

	itemMin = -1;
	itemMax = -1;
	int count = properties.countFrames();

	int posMin = -1;
	for(int i=0; i<count; i++)
	{
		int p = properties[i].position;

		if(p == _frame)
		{
			itemMin = itemMax = i ;
		}

		if(p < _frame && p > posMin)
		{
			itemMin = i;
			posMin = p;
		}
	}

	if(itemMin != -1 && itemMin < count-1)
	{
		itemMax = itemMin+1;
	}

	if(itemMin == -1 || itemMax == -1)
		return false;

	return true;


}


GProperties GVectorFigure::currentFrame() const
{

	if(itemMin == itemMax)
		return properties[itemMin];

	const GProperties &frame1 = properties[itemMin];
	const GProperties &frame2 = properties[itemMax];

	qreal inFramePos = qreal(frame1.position -  int( _frame ));

	if(( !frame1.visible ) || ( !frame2.visible ) )
			return frame1;


	GProperties newFrame = frame1;
	newFrame.visible = true;
	int countPoints = frame1.points.size();
	for( int i = 0; i < countPoints; i++ )
	{
		if( frame1.points[i] == frame2.points[i] )
			continue;

		newFrame.points[i] = interpolate(
			frame1.points[i], frame2.points[i], qreal(frame1.position - frame2.position), inFramePos );
	}

	return newFrame;
}


void GVectorFigure::paintObject( QPainter &p, const GProperties &props ) const
{
	if( !props.visible )
		return;

	drawSplines( p, props );
}


void GVectorFigure::drawSplines( QPainter &p, const GProperties &props ) const
{
	if( props.points.size() < 4 )
		return;

	if(hideLines!=0)
	{
		int countPoints = props.points.size();
		QBrush br = p.brush();
		p.setBrush( QBrush() );
		for( int i = 1; i < countPoints - 1 ; i += 3 )
		{
			if(((i/3)+1) % (hideLines+1) == 0 )
				continue;
			QPainterPath path;
			path.moveTo( props.points[i-1] );
			path.cubicTo( props.points[i], props.points[i + 1], props.points[i + 2] );
			p.drawPath( path );
		}


		QPen pen = p.pen();
		p.setPen(QColor(0,0,0,0));
		p.setBrush(br);

		int countSplinePoints = ( ( props.points.size() - 1 ) / 3 ) * 3;
		QPainterPath path;
		path.moveTo( props.points[0] );

		for( int i = 1; i < ( countSplinePoints + 1 ); i += 3 )
			path.cubicTo( props.points[i], props.points[i + 1], props.points[i + 2] );

		p.drawPath( path );
		p.setPen(pen);
		if( closed )
			p.drawLine(props.points[0], props.points[countPoints-1]);
	}
	else
	{
		int countSplinePoints = ( ( props.points.size() - 1 ) / 3 ) * 3;
		QPainterPath path;
		path.moveTo( props.points[0] );

		for( int i = 1; i < ( countSplinePoints + 1 ); i += 3 )
			path.cubicTo( props.points[i], props.points[i + 1], props.points[i + 2] );

		if( closed )
			path.closeSubpath();

		p.drawPath( path );
	}

}


QPen GVectorFigure::interpolatedPen() const
{
	const GProperties &frame1 = properties[itemMin];
	const GProperties &frame2 = properties[itemMax];

	if( ( !frame1.visible ) && ( !frame2.visible ) )
		return QPen( Qt::NoPen );

	qreal inFramePos = qreal(frame1.position -  int( _frame ));

	QColor color = _pen.color();
	QPen newPen = _pen;

	if( inFramePos == 0.0 || ( frame1.alpha == frame2.alpha))
	{
		color.setAlpha(color.alpha() + frame1.alpha - 255);
		newPen.setColor( color );
		return newPen;
	}

	int a1 = (color.alpha() + frame1.alpha - 255);
	int a2  = (color.alpha() + frame2.alpha - 255);

	if(a1<0)
		a1 = 0;

	if(a2<0)
		a2 = 0;

	color.setAlpha( int( interpolate( qreal( a1 ), qreal( a2 ), frame1.position - frame2.position, inFramePos ) ) );

	newPen.setColor( color );
	return newPen;
}


QBrush GVectorFigure::interpolatedBrush() const
{
	const GProperties &frame1 = properties[itemMin];
	const GProperties &frame2 = properties[itemMax];

	if( ( !frame1.visible ) && ( !frame2.visible ) )
		return QBrush( Qt::NoBrush );

	int alpha1, alpha2;

	qreal inFramePos = qreal(frame1.position -  int( _frame ));
	QBrush newBrush = _brush;

	if( inFramePos == 0.0 || ( frame1.alpha == frame2.alpha))
	{
		switch( newBrush.style() )
			{
				case Qt::LinearGradientPattern:
				case Qt::ConicalGradientPattern:
				case Qt::RadialGradientPattern:
				{
					QGradient g = ( *newBrush.gradient() );
					QGradientStops stops = g.stops();

					int countStops = stops.size();

						for( int i = 0; i < countStops; i++ )
						{
							alpha1 = stops[i].second.alpha() + frame1.alpha  - 255;

							stops[i].second = QColor( stops[i].second.red(),
									stops[i].second.green(), stops[i].second.blue(),
								int( alpha1 ) );
						}

					g.setStops( stops );
					newBrush = QBrush( g );
					break;
				}

				case Qt::TexturePattern:
				{
					QPixmap pixmap = newBrush.texture();
					QPixmap alpha = pixmap.alphaChannel();
					int alphaValue = qAlpha( pixmap.toImage().pixel( 0, 0 ) );

					alpha1 = alphaValue + frame1.alpha  - 255;

					alphaValue = int( alpha1 );

					alpha.fill( QColor( alphaValue, alphaValue, alphaValue ) );
					pixmap.setAlphaChannel( alpha );
					newBrush.setTexture( pixmap );
					break;
				}

				default:
				{
					QColor c = _brush.color();

					alpha1 = c.alpha() + frame1.alpha  - 255;
					c.setAlpha( int(alpha1) );

					newBrush.setColor( c );
					break;
				}
			}
		return newBrush;
	}

	switch( newBrush.style() )
	{
		case Qt::LinearGradientPattern:
		case Qt::ConicalGradientPattern:
		case Qt::RadialGradientPattern:
		{
			QGradient g = ( *newBrush.gradient() );
			QGradientStops stops = g.stops();

			int countStops = stops.size();

				for( int i = 0; i < countStops; i++ )
				{
					alpha1 = stops[i].second.alpha() + frame1.alpha  - 255;
					alpha2 = stops[i].second.alpha() + frame2.alpha  - 255;

					stops[i].second = QColor( stops[i].second.red(),
							stops[i].second.green(), stops[i].second.blue(),
						int( interpolate( qreal( alpha1 ), qreal( alpha2 ), frame1.position - frame2.position, inFramePos ) ) );
				}

			g.setStops( stops );
			newBrush = QBrush( g );
			break;
		}

		case Qt::TexturePattern:
		{
			QPixmap pixmap = newBrush.texture();
			QPixmap alpha = pixmap.alphaChannel();
			int alphaValue = qAlpha( pixmap.toImage().pixel( 0, 0 ) );

			alpha1 = alphaValue + frame1.alpha  - 255;
			alpha2 = alphaValue + frame2.alpha  - 255;

			alphaValue = int( interpolate(
				qreal( alpha1 ),qreal( alpha2 ), frame1.position - frame2.position, inFramePos ) );

			alpha.fill( QColor( alphaValue, alphaValue, alphaValue ) );
			pixmap.setAlphaChannel( alpha );
			newBrush.setTexture( pixmap );
			break;
		}

		default:
		{
			QColor c = newBrush.color();

			alpha1 = c.alpha() + frame1.alpha  - 255;
			alpha2 = c.alpha() + frame2.alpha  - 255;

			c.setAlpha( int( interpolate(
				qreal( alpha1 ), qreal( alpha2 ), frame1.position - frame2.position, inFramePos ) ) );

			newBrush.setColor( c );
			break;
		}
	}

	return newBrush;
}


int GVectorFigure::addPointToEndOfPolygon( const QPointF &p )
{
	int countFrames = properties.countFrames();
	int index = getIndexFrameByPosition(_frame);
	int countPoints = properties[ index ].points.size();

	properties[ index ].points.append( properties[ index ].points[ countPoints - 1 ]);

	for(int j=0;j<2;j++)
	{
		properties[ index ].points.append( p );
	}

	for( int i = 0; i < countFrames; i++ )
	{

		if( properties[ i ].position == _frame )
			continue;

		properties[ i ].points.append( properties[ i ].points[ countPoints - 1 ]);

		for(int j=0;j<2;j++)
		{
			properties[ i ].points.append(
				properties[ i ].points[ 0 ] +
				( ( properties[ i ].points[ 0 ] -
					properties[ i ].points[ countPoints - 1 ] ) / 2.0 )
			);
		}
	}

	return countPoints;
}


int GVectorFigure::addPointToPolygon( const QPointF &p )
{
	int countFrames = properties.countFrames();
	int index = getIndexFrameByPosition(_frame);
	int countPoints = properties[ index ].points.size();

	qreal minLenToLine = 65535;
	qreal lenToLine = 0;
	int lineBegin = 0;
	int lineEnd = 0;

	for( int i = 0; i < countPoints; i ++ )
	{
		lenToLine = lengthFromPointToLine( p,
			QLineF( properties[ index ].points[ i ],
					properties[ index ].points[ ( i + 1 ) % countPoints ] ) );

		if( lenToLine < minLenToLine )
		{
			minLenToLine = lenToLine;
			lineBegin = i;
		}
	}

	qreal lenToBegin = QLineF( properties[ index ].points[ 0 ], p ).length();
	qreal lenToEnd = QLineF( properties[ index ].points[ countPoints - 1 ], p ).length();

	lineEnd = ( lineBegin + 1 )  % countPoints;
	int insertTo = lineEnd;

	if( minLenToLine >= lenToBegin || minLenToLine >= lenToEnd )
	{
		if( lenToBegin <= lenToEnd )
			insertTo = 0;
		else
			insertTo = countPoints;

		lineBegin = 0;
		lineEnd = countPoints - 1;
	}

	int count;
	int insertToEdge = -1;
	if(insertTo==0 || insertTo == countPoints)
	{
		if(insertTo == 0)
		{
			properties[ index ].points.insert( 0, properties[ index ].points[ 0 ]);
			insertToEdge = 0;
		}
		else
		{
			properties[ index ].points.insert( countPoints-1, properties[ index ].points[ countPoints-1 ]);
			insertToEdge = countPoints-1;
			insertTo++;
		}
		count=2;
	}
	else
		count=3;



	for(int j=0;j<count;j++)
	{
		properties[ index ].points.insert( insertTo, p );
	}

	for( int i = 0; i < countFrames; i++ )
	{
		if( properties[ i ].position == _frame )
			continue;

		if(insertToEdge!=-1)
			properties[ i ].points.insert( insertToEdge, properties[ i ].points[ insertToEdge ]);

		for(int j=0;j<count;j++)
		{
			properties[ i ].points.insert( insertTo,
					properties[ i ].points[ lineBegin ] +
					( ( properties[ i ].points[ lineEnd ] -
							properties[ i ].points[ lineBegin ] ) / 2.0 )
					);
		}
	}

	return insertTo;
}


int GVectorFigure::addPointToSpline( const QPointF &p )
{
	int countFrames = properties.countFrames();
	int index = getIndexFrameByPosition(_frame);
	int countPoints = properties[ index ].points.size();

	for( int i = 0; i < countFrames; i++ )
	{
		if( properties[ i ].position == _frame  )
		{
			properties[ i ].points.append(
				( properties[ i ].points[ countPoints - 1 ] * 2 ) -
					properties[ i ].points[ countPoints - 2 ] );
			properties[ i ].points.append( p );
			properties[ i ].points.append( p );
		}
		else
		{
			QPointF point = properties[ i ].points[ countPoints - 1 ];
			for( int j = 0; j < 3; j++ )
				properties[ i ].points.append( point );
		}
	}

	return properties[ index ].points.size() - 2;
}
