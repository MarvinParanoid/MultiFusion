#include "GSelectionRect.h"
#include "GObject.h"
#include "GVectorFigure.h"

const int GSelectionRect::marksRadius = 4;
const int GSelectionRect::displacement = 11;


void GSelectionRect::mouseDoubleClick( const int button, const QPoint &pos, Qt::KeyboardModifiers modifiers )
{
	if( ( !visible ) || addPointMode ) return;

	if( button == Qt::RightButton )
		return;

    if( ( modifiers & Qt::ShiftModifier ) == 0 )
		createFigureMode = false;

	movedMark = selectedMark( pos );
	if( ( movedMark != rotateCenter ) && ( movedMark != center ) )
		return;

	if( movedMark == rotateCenter )
	{
		centerPoint = QPointF( position.width() / 2,
					position.height() / 2 );
		emit changed();
	}
	else
		setInputMode( ( _inputMode ==  GSRInterface::moveResize ) ?
				(  GSRInterface::sheareRotate ) : (  GSRInterface::moveResize ) );
}

bool GSelectionRect::mousePress( const int button, const QPoint &pos, Qt::KeyboardModifiers modifiers )
{
	if( !visible ) return false;

	if( isNewFigure ) visible = false;
    if( ( modifiers & Qt::ShiftModifier ) == 0 )
		createFigureMode = false;

	switch( button )
	{
		case Qt::LeftButton:
			selectedObjectPoint = -1;
			movedMark = selectedMark( pos );

			if( ( movedMark ==  GSRInterface::center || movedMark == GSRInterface::objectPoint ) && addPointMode && ( _inputMode ==  GSRInterface::moveResize ) )
			{
				if( selectedObjects->addPoint( pos ) != -1 )
				{
					position = selectedObjects->boundingRect().toRect();
					emit changed();
					emit StateChanged("Add point");
					return true;
				}
			}

			switch( movedMark )
			{
				case topLeft:
				case bottomRight:
					_parent->setCursor( Qt::SizeFDiagCursor );
					break;

				case topCenter:
				case bottomCenter:
					_parent->setCursor( Qt::SizeVerCursor );
					break;

				case bottomLeft:
				case topRight:
					_parent->setCursor( Qt::SizeBDiagCursor );
					break;

				case leftCenter:
				case rightCenter:
					_parent->setCursor( Qt::SizeHorCursor );
					break;

				case objectPoint:
				case center:
				case rotateCenter:
					_parent->setCursor( Qt::SizeAllCursor );
					break;

				case shearTop:
				case shearBottom:
				case shearLeft:
				case shearRight:

				case rotateLeftTop:
				case rotateRightTop:
				case rotateRightBottom:
				case rotateLeftBottom:
					_parent->setCursor( Qt::ClosedHandCursor );
					break;

				default:
					break;
			}

			lastHitPoint = pos;
			if( movedMark != notSelected )
				return true;
			break;

		case Qt::RightButton:
			if( ( !addPointMode ) || ( _inputMode !=  GSRInterface::moveResize ) )
				break;

			selectedObjectPoint = -1;
			movedMark = selectedMark( pos );

			if( selectedObjectPoint == -1 || selectedObjectPoint%3 !=0)
				break;

			if( QMessageBox::question( _parent,
				tr( "Deleting point from layer." ),
				tr( "Are you sure want to delete this point? " ) +
				tr( "It delete point from all frames in animation." ),
				QMessageBox::Ok | QMessageBox::Cancel,
				QMessageBox::Cancel ) == QMessageBox::Cancel )
			{
				return true;
			}

			if( selectedObjects->deletePoint( selectedObjectPoint ) )
			{
				position = selectedObjects->boundingRect().toRect();
				emit changed();
				emit StateChanged("Delete point");
				return true;
			}
			break;

		default:
			break;
	}

	return false;
}

void GSelectionRect::mouseMove( const int button, const QPoint &pos, Qt::KeyboardModifiers modifiers )
{
	if( button == Qt::RightButton )
		return;

    if( ( modifiers & Qt::ShiftModifier ) == 0 )
		createFigureMode = false;

	QPoint p = pos;

	if( p.x() < maxRect.x() )
		p.setX( maxRect.x() );
	if( p.x() >= ( maxRect.x() + maxRect.width() ) )
		p.setX( maxRect.x() + maxRect.width() - 1 );

	if( p.y() < maxRect.y() )
		p.setY( maxRect.y() );
	if( p.y() >= ( maxRect.y() + maxRect.height() ) )
		p.setY( maxRect.y() + maxRect.height() - 1 );

	if( movedMark != notSelected )
		moveMark( movedMark, p, modifiers );
}

void GSelectionRect::mouseRelease( const int button, const QPoint &pos, Qt::KeyboardModifiers modifiers )
{
	setVisible(true);
	if( !visible ) return;

	createFigureMode = false;

	switch( button )
	{
		case Qt::LeftButton:
		{
			movedMark = notSelected;

			QRect oldPosition = position;

			position = selectedObjects->boundingRect().toRect();
			normalize();

			if( rotateAngle == 0.0 )
			{
				qreal sx = qreal( position.width() ) / qreal( oldPosition.width() );
				qreal sy = qreal( position.height() ) / qreal( oldPosition.height() );

				centerPoint.setX( centerPoint.x() * sx );
				centerPoint.setY( centerPoint.y() * sy );
			}
			else
			{
				centerPoint += oldPosition.topLeft();
				centerPoint -= position.topLeft();
			}

			rotateAngle = 0.0;
			shearOffsets = QPointF( 0.0, 0.0 );
			_parent->unsetCursor();
			emit changed();
			break;
		}

		default:
			break;
	}

	if(isMove)
		 emit StateChanged("Move");
	if(isShear)
		 emit StateChanged("Shear");
	if(isRotate)
		 emit StateChanged("Rotate");
	if(isScale)
		 emit StateChanged("Scale");
	if(isPoint)
		 emit StateChanged("Move point");

	isMove = false;
	isShear = false;
	isRotate = false;
	isScale = false;
	isPoint = false;
}

void GSelectionRect::paint( QPainter &p )
{
	if(!visible && _inputMode ==  GSRInterface::sheareRotate) return;

	QPen selectionRectPen( Qt::DashLine );
	selectionRectPen.setColor( QColor( 50, 50, 50, 200 ) );


	QPen marksPen( QColor( 0, 0, 0 ) );
	QBrush marksBrush( QColor( 120, 240, 120, 255 ) );
	QBrush centerPointBrush( QColor( 240, 240, 80, 255 ) );

	if( _inputMode ==  GSRInterface::sheareRotate )
	{
		p.save();

		QMatrix m;

		m.translate( shearPoint.x(), shearPoint.y() );
		m.shear( shearOffsets.x(), shearOffsets.y() );
		m.translate( -shearPoint.x(), -shearPoint.y() );

		m.translate( position.x() + centerPoint.x(),
					position.y() + centerPoint.y() );
		m.rotate( rotateAngle );
		m.translate( -position.x() - centerPoint.x(),
					-position.y() - centerPoint.y() );

		p.setWorldMatrix( m, true );
	}
	else
	{
		QPolygonF points = selectedObjects->points(selectedObjects->frame());

		int countPoints = points.size();
		if( countPoints > 0 && selectedObjects->countObjects() == 1)
		{

			GVectorFigure* v = GVECTORFIGURE(selectedObjects->object(0));

			if( v->isSpline() && v->isShowBezier())
			{
				p.setPen( selectionRectPen );

				for( int i = 0; i < countPoints - 1; i += 3 )
					p.drawLine( points[i], points[i + 1] );

				for( int i = 2; i < countPoints - 1; i += 3 )
					p.drawLine( points[i], points[i + 1] );

				marksPen.setCapStyle(Qt::FlatCap);
				marksPen.setWidth(1);
				p.setPen( selectionRectPen );

				p.setBrush( QBrush() );
				for( int i = 0; i < countPoints; i++ )
				{
					p.drawRect((int)(points[i].x() - 3),(int)(points[i].y() - 3), 6, 6);
				}
			}
			else
			{
				p.setPen( selectionRectPen );
				p.setBrush( QBrush() );
				for( int i = 0; i < countPoints; i+=3 )
				{
					p.drawRect((int)(points[i].x() - 3),(int)(points[i].y() - 3), 6, 6);
				}
			}
		}
	}

	if(visible)
	{
		p.setBrush(QBrush());
		p.setPen( selectionRectPen );
		p.drawRect( position.x(), position.y(),
			position.width() - 1, position.height() - 1 );

		p.setBrush(QColor( 51, 51, 51, 255));
		p.setPen(marksPen);

		if( _inputMode !=  GSRInterface::sheareRotate )
		{
			QVector< QRect > marks = marksRects();

			int countMarks = marks.size();
			for( int i = 0; i < countMarks; i++ )
				p.drawRect( marks[i] );
		}

		p.drawLine( position.x() + position.width() / 2 - 10,
					position.y() + position.height() / 2,
					position.x() + position.width() / 2 + 10,
					position.y() + position.height() / 2 );

		p.drawLine( position.x() + position.width() / 2,
					position.y() + position.height() / 2 - 10,
					position.x() + position.width() / 2,
					position.y() + position.height() / 2 + 10 );
	}

		if( _inputMode ==  GSRInterface::sheareRotate )
		{
			p.setBrush( QBrush() );
			drawRotateArrow( p, position.topLeft() );
			drawRotateArrow( p, position.topRight() );
			drawRotateArrow( p, position.bottomLeft() );
			drawRotateArrow( p, position.bottomRight() );

			p.setBrush( centerPointBrush );
			p.drawEllipse( (int)(position.x() + centerPoint.x() - marksRadius),
					(int)(position.y() + centerPoint.y() - marksRadius),
						marksRadius * 2, marksRadius * 2 );

			p.drawEllipse((int)(position.x() + centerPoint.x() - 1),
					(int)(position.y() + centerPoint.y() - 1), 2, 2 );

			drawShiftArrow( p, QPoint( position.x() + position.width() / 2, position.y() - 11 ), arrowHorizontal );
			drawShiftArrow( p, QPoint( position.x() + position.width() / 2, position.y() + position.height() + 10 ), arrowHorizontal );

			drawShiftArrow( p, QPoint( position.x() - 11, position.y() + position.height() / 2 ), arrowVertical );
			drawShiftArrow( p, QPoint( position.x() + position.width() + 10, position.y() + position.height() / 2 ), arrowVertical );

			p.restore();
		}

}

bool GSelectionRect::isInCreateFigureMode() const
{
	return createFigureMode;
}

void GSelectionRect::setCreateFigureMode( bool enabled )
{
	createFigureMode = enabled;
}

void GSelectionRect::setIsNewFigure(bool inf)
{
	isNewFigure = inf;
}

bool GSelectionRect::getIsNewFigure()
{
	return this->isNewFigure;
}

QRect GSelectionRect::getViewport() const
{
	return maxRect;
}

void GSelectionRect::setViewport( const QRect &v )
{
	maxRect = v;
}

GSRInterface::InputMode GSelectionRect::getInputMode() const
{
	return _inputMode;
}

void GSelectionRect::setInputMode( const GSRInterface::InputMode m )
{
	if( _inputMode == m )
		return;

	_inputMode = m;
	emit changed();
}

bool GSelectionRect::isVisible() const
{
	return visible;
}

void GSelectionRect::setVisible( bool v )
{
	visible = v;
	emit changed();
}

QPen GSelectionRect::pen()
{
	return selectedObjects->pen();
}

QBrush GSelectionRect::brush()
{
	return selectedObjects->brush();
}

void GSelectionRect::setPen( const QPen &p )
{
	selectedObjects->setPen( p );
}

void GSelectionRect::setBrush( const QBrush &b )
{
	selectedObjects->setBrush( b );
}

bool GSelectionRect::isClosed() const
{
	return selectedObjects->isClosed();
}

void GSelectionRect::setClosed( bool closed )
{
	selectedObjects->setClosed( closed );
}

int GSelectionRect::countSelected() const
{
	return selectedObjects->countObjects();
}

bool GSelectionRect::isInside( GObjectInterface *o )
{
	return selectedObjects->isInside( GOBJECT(o));
}

GObjectInterface *GSelectionRect::selected( int index )
{
	return selectedObjects->object( index );
}

void GSelectionRect::setSelected( GObjectInterface *o )
{
	setInputMode(  GSRInterface::moveResize );
	selectedObjects->removeAll();
	if( o != 0 )
		addSelected( o );
}

QObject* GSelectionRect::getSelected()
{
	return selectedObjects;
}

void GSelectionRect::addSelected( GObjectInterface *o )
{
	if( selectedObjects->isInside(GOBJECT(o)) )
		return;
	selectedObjects->add(GOBJECT(o));
	position = selectedObjects->boundingRect().toRect();
	centerPoint = ( ( position.topLeft() + position.bottomRight() ) / 2 ) -
					position.topLeft();

	normalize();
	setVisible( true );
}

void GSelectionRect::reset()
{
	selectedObjects->removeAll();
	setVisible( false );
}

bool GSelectionRect::isInAddPointMode() const
{
	return addPointMode;
}

void GSelectionRect::enableAddPointMode( bool enabled )
{
	addPointMode = enabled;
	setInputMode(  GSRInterface::moveResize );
}

GSelectionRect::GSelectionRect( plugin::PluginsManager *manager, QWidget *parent, const QRect viewport, const QRect &pos ):
	position( pos ), maxRect( viewport ),
	movedMark( notSelected ), centerPoint( 60, 50 ),
	rotateAngle( 0.0 ), _inputMode( moveResize ),
	visible( false ), addPointMode( false ),
	_parent( parent ), createFigureMode( false ),isNewFigure(false)
{
	selectedObjects = new GContainer();
	if( position.width() == 0 )
		position.setWidth( 1 );

	if( position.height() == 0 )
		position.setHeight( 1 );

	connect( this, SIGNAL( moved( const qreal, const qreal ) ),
			this, SLOT( onMoved( const qreal, const qreal ) ) );

	connect( this, SIGNAL( scaled( const qreal, const qreal, const QPointF& ) ),
			this, SLOT( onScaled( const qreal, const qreal, const QPointF& ) ) );

	connect( this, SIGNAL( sheared( const qreal, const qreal, const QPointF& ) ),
			this, SLOT( onSheared( const qreal, const qreal, const QPointF& ) ) );

	connect( this, SIGNAL( rotated( const qreal, const QPointF& ) ),
			this, SLOT( onRotated( const qreal, const QPointF& ) ) );

	manager->addPlugins(this, "Selection");
}

GSelectionRect::~GSelectionRect()
{
}

void GSelectionRect::drawShiftArrow( QPainter &p, const QPoint &pos, const ShiftArrowDirection dir )
{
	if( dir == arrowHorizontal )
	{
		p.drawLine( pos.x() - 8, pos.y() - 2, pos.x() + 7, pos.y() - 2 );
		p.drawLine( pos.x() + 7, pos.y() - 2, pos.x() + 3, pos.y() - 4 );

		p.drawLine( pos.x() - 8, pos.y() + 2, pos.x() + 7, pos.y() + 2 );
		p.drawLine( pos.x() - 8, pos.y() + 2, pos.x() - 4, pos.y() + 4 );
	}
	else
	{
		p.drawLine( pos.x() - 2, pos.y() - 8, pos.x() - 2, pos.y() + 7 );
		p.drawLine( pos.x() - 2, pos.y() + 7, pos.x() - 4, pos.y() + 3 );

		p.drawLine( pos.x() + 2, pos.y() - 8, pos.x() + 2, pos.y() + 7 );
		p.drawLine( pos.x() + 2, pos.y() - 8, pos.x() + 4, pos.y() - 4 );
	}
}

void GSelectionRect::drawRotateArrow( QPainter &p, const QPoint &pos )
{
	p.drawEllipse( pos.x() - 10, pos.y() - 10, 20, 20 );

	p.drawLine( pos.x() - 10, pos.y(), pos.x() - 13, pos.y() + 3 );
	p.drawLine( pos.x() - 10, pos.y(), pos.x() - 7, pos.y() + 3 );

	p.drawLine( pos.x() + 10, pos.y(), pos.x() + 13, pos.y() - 3 );
	p.drawLine( pos.x() + 10, pos.y(), pos.x() + 7, pos.y() - 3 );
}

qreal GSelectionRect::calcSheareAngle( qreal height, qreal width )
{
	QLineF l( 0.0, height, width, 0.0 );
	qreal a = l.angle( QLineF( 0.0, height, 0.0, 0.0 ) );
	if( width < 0 )
		a = -a;
	return ( a / 180.0 ) * 3.14159265358979;
}

qreal GSelectionRect::calcRotateAngle( const QPointF &center, const QPoint &beginPoint, const QPoint &endPoint )
{
	QLineF l( center, endPoint );
	qreal a = l.angle( QLineF( center, beginPoint ) );
	qreal t = qreal( beginPoint.x() - center.x() ) *
			qreal( endPoint.y() - center.y() ) -
			qreal( beginPoint.y() - center.y() ) *
			qreal( endPoint.x() - center.x() );

	if( t < 0 ) a = -a;
	return a;
}

void GSelectionRect::onMoved( const qreal dx, const qreal dy )
{

	selectedObjects->move( dx, dy );
}

void GSelectionRect::onScaled( const qreal sx, const qreal sy, const QPointF &scaleCenter )
{
	selectedObjects->scale( sx, sy, scaleCenter );
}

void GSelectionRect::onSheared( const qreal sx, const qreal sy, const QPointF &shearPoint )
{
	selectedObjects->sheare( sx, sy, shearPoint );
}

void GSelectionRect::onRotated( const qreal angle, const QPointF &center )
{
	selectedObjects->rotate( angle, center );
}

QRect GSelectionRect::getPosition() const
{
	return position;
}

void GSelectionRect::setPosition( const QRect &p )
{
	if( p == position )
		return;

	QRect oldPosition = position;
	position = p;

	if( position.width() == 0 )
		position.setWidth( 1 );

	if( position.height() == 0 )
		position.setHeight( 1 );

	recalcCenter( oldPosition );
	normalize();
}


GSelectionRect::MarksPositions GSelectionRect::selectedMark( const QPoint &p )
{
	if( _inputMode !=  GSRInterface::sheareRotate )
	{
		if(isNewFigure)
		{
			isNewFigure = false;
			return (MarksPositions)4;
		}
		QVector< QRect > marks = marksRects();
		int countMarks = marks.size();
		for( int i = 0; i < countMarks; i++ )
		{
			QPainterPath path;
			path.addRect(marks[i]);
			if( path.contains( p ) )
				return ( MarksPositions ) i;

		}
	}

	if( _inputMode ==  GSRInterface::sheareRotate )
	{
		QPainterPath path;
		path.addEllipse( position.x() + centerPoint.x() - marksRadius,
					position.y() + centerPoint.y() - marksRadius,
					marksRadius * 2, marksRadius * 2 );

		if( path.contains( p ) )
			return rotateCenter;

		if( QRect( position.x() + position.width() / 2 - 8,
				position.y() - 19, 16, 16 ).contains( p ) )
		{
			shearPoint = position.bottomRight();
			return shearTop;
		}

		if( QRect( position.x() + position.width() / 2 - 8,
			position.y() + position.height() + 2, 16 ,16 ).contains( p ) )
		{
			shearPoint = position.topLeft();
			return shearBottom;
		}

		if( QRect( position.x() - 19,
			position.y() + position.height() / 2 - 8, 16, 16 ).contains( p ) )
		{
			shearPoint = position.topRight();
			return shearLeft;
		}

		if( QRect( position.x() + position.width() + 2,
				position.y() + position.height() / 2 - 8, 16, 16 ).contains( p ) )
		{
			shearPoint = position.topLeft();
			return shearRight;
		}

		if( QRect( position.topLeft() - QPoint( 10, 10 ), QSize( 20, 20 ) ).contains( p ) )
			return rotateLeftTop;

		if( QRect( position.topRight() - QPoint( 10, 10 ), QSize( 20, 20 ) ).contains( p ) )
			return rotateRightTop;

		if( QRect( position.bottomLeft() - QPoint( 10, 10 ), QSize( 20, 20 ) ).contains( p ) )
			return rotateLeftBottom;

		if( QRect( position.bottomRight() - QPoint( 10, 10 ), QSize( 20, 20 ) ).contains( p ) )
			return rotateRightBottom;
	}
	else
	{
		QPolygonF points = selectedObjects->points(selectedObjects->frame());
		int countPoints = points.size();
		if( countPoints > 0 && selectedObjects->countObjects() == 1)
		{
			GVectorFigure* v = GVECTORFIGURE(selectedObjects->object(0));
			if( v->isSpline() && v->isShowBezier())
			{
				for( int i = countPoints - 1; i >= 1; i-- )
				{
					QPainterPath path;

					if(i%3 == 0)
						continue;

					path.addEllipse( QRect( (int)(points[i].x() - 10), (int)(points[i].y() - 10), 20, 20 ) );
					if( path.contains( p ) )
					{
						selectedObjectPoint = i;
						return objectPoint;
					}
				}
				for( int i = countPoints - 1; i >= 0; i-=3 )
				{
					QPainterPath path;
					path.addEllipse( QRect( (int)(points[i].x() - 10), (int)(points[i].y() - 10), 20, 20 ) );
					if( path.contains( p ) )
					{
						selectedObjectPoint = i;
						return objectPoint;
					}
				}

			}
			else
			{
				for( int i = countPoints - 1; i >= 0; i-=3 )
				{
					QPainterPath path;
					path.addEllipse( QRect( (int)(points[i].x() - 10), (int)(points[i].y() - 10), 20, 20 ) );
					if( path.contains( p ) )
					{
						selectedObjectPoint = i;
						return objectPoint;
					}
				}
			}
		}
	}

	if( position.contains( p ) )
		return center;

	return notSelected;
}

void GSelectionRect::moveMark( const MarksPositions mark, QPoint p, Qt::KeyboardModifiers modifiers )
{
	QRect oldPosition = position;
	bool forceUpdate = false;
	qreal shearAngle = 0.0;
	QPointF lastSheareOffsets =shearOffsets;
	qreal lastRotateAngle = rotateAngle;

    if( createFigureMode && ( ( modifiers & Qt::ShiftModifier ) != 0 ) )
	{
		QPoint tmp = p - lastHitPoint;
		int t = ( tmp.x() > tmp.y() ) ? ( tmp.x() ) : ( tmp.y() );
		p = QPoint( t, t ) + lastHitPoint;
	}

	switch( mark )
	{
		case topLeft:
			setVisible(false);
			position.setTopLeft( position.topLeft() + p - lastHitPoint );
			break;

		case topCenter:
			setVisible(false);
			position.setTop( position.top() + p.y() - lastHitPoint.y() );
			break;

		case topRight:
			setVisible(false);
			position.setTopRight( position.topRight() + p - lastHitPoint );
			break;

		case rightCenter:
			setVisible(false);
			position.setRight( position.right() + p.x() - lastHitPoint.x() );
			break;

		case bottomRight:
			setVisible(false);
			position.setBottomRight( position.bottomRight() + p - lastHitPoint );
			break;

		case bottomCenter:
			setVisible(false);
			position.setBottom( position.bottom() + p.y() - lastHitPoint.y() );
			break;

		case bottomLeft:
			setVisible(false);
			position.setBottomLeft( position.bottomLeft() + p - lastHitPoint );
			break;

		case leftCenter:
			setVisible(false);
			position.setLeft( position.left() + p.x() - lastHitPoint.x() );
			break;

		case center:
			setVisible(false);
			position.moveTo( position.topLeft() + p - lastHitPoint );
			break;

		case rotateCenter:
			centerPoint += ( p - lastHitPoint );
			forceUpdate = true;
			break;

		case shearTop:
			shearAngle = -calcSheareAngle( position.height(), ( p - lastHitPoint ).x() ) * 2.0;
			shearOffsets.setX( shearAngle );
			isShear = true;
			emit sheared( shearAngle - lastSheareOffsets.x(), 0.0, shearPoint );
			emit changed();
			break;

		case shearBottom:
			shearAngle = calcSheareAngle( position.height(), ( p - lastHitPoint ).x() ) * 2.0;
			shearOffsets.setX( shearAngle );
			isShear = true;
			emit sheared( shearAngle - lastSheareOffsets.x(), 0.0, shearPoint );
			emit changed();
			break;

		case shearLeft:
			shearAngle = -calcSheareAngle( position.width(), ( p - lastHitPoint ).y() ) * 2.0;
			shearOffsets.setY( shearAngle );
			isShear = true;
			emit sheared( 0.0, shearAngle - lastSheareOffsets.y(), shearPoint );
			emit changed();
			break;

		case shearRight:
			shearAngle = calcSheareAngle( position.width(), ( p - lastHitPoint ).y() ) * 2.0;
			shearOffsets.setY( shearAngle );
			isShear = true;
			emit sheared( 0.0, shearAngle - lastSheareOffsets.y(), shearPoint );
			emit changed();
			break;

		case rotateLeftTop:
		case rotateRightTop:
		case rotateRightBottom:
		case rotateLeftBottom:
			rotateAngle += calcRotateAngle(
				position.topLeft() + centerPoint, lastHitPoint, p );
			lastHitPoint = p;
			isRotate = true;
			emit rotated( rotateAngle - lastRotateAngle,
				position.topLeft() + centerPoint );
			emit changed();
			break;

		case objectPoint:
		{
			setVisible(false);
			GVectorFigure* v = GVECTORFIGURE(selectedObjects->object(0));
			if( v->isSpline())
			{
				QPolygonF points = selectedObjects->points(selectedObjects->frame());
				selectedObjects->movePoint( selectedObjectPoint,
						points[ selectedObjectPoint ] + p - lastHitPoint );
				lastHitPoint = p;
				isPoint = true;
			}
			else
			{
				QPolygonF points = selectedObjects->points(selectedObjects->frame());
				selectedObjects->movePoint( selectedObjectPoint,
						points[ selectedObjectPoint ] + p - lastHitPoint );

				points = selectedObjects->points(selectedObjects->frame());
				if(selectedObjectPoint != points.size()-1)
				{
					selectedObjects->movePoint( selectedObjectPoint+1,
							points[ selectedObjectPoint ]);
				}

				if(selectedObjectPoint != 0)
				{
					selectedObjects->movePoint( selectedObjectPoint-1,
							points[ selectedObjectPoint ] );
				}
				lastHitPoint = p;
				isPoint = true;
			}


			QRect oldPosition = position;
			position = selectedObjects->boundingRect().toRect();

			qreal sx = qreal( position.width() ) / qreal( oldPosition.width() );
			qreal sy = qreal( position.height() ) / qreal( oldPosition.height() );

			centerPoint.setX( centerPoint.x() * sx );
			centerPoint.setY( centerPoint.y() * sy );
			emit changed();
			return;
			break;
		}

		case notSelected:
			break;
	}

	if( position.width() == 0 )
		position.setWidth( 1 );

	if( position.height() == 0 )
		position.setHeight( 1 );

	if( forceUpdate || ( position != oldPosition ) )
	{
		lastHitPoint = p;
		recalcCenter( oldPosition );
		emit changed();
	}

}

void GSelectionRect::recalcCenter( const QRect &oldPosition )
{

	qreal sx = qreal( position.width() ) / qreal( oldPosition.width() );
	qreal sy = qreal( position.height() ) / qreal( oldPosition.height() );

	qreal dx = position.x() - oldPosition.x();
	qreal dy = position.y() - oldPosition.y();

	centerPoint.setX( centerPoint.x() * sx );
	centerPoint.setY( centerPoint.y() * sy );

	if( ( dx != 0.0 ) || ( dy != 0.0 ) )
	{
		emit moved( dx, dy );
		isMove = true;
	}


	if( ( sx != 1.0 ) || ( sy != 1.0 ) )
	{
		emit scaled( sx, sy, position.topLeft() );
		isScale = true;
	}
}

QVector< QRect > GSelectionRect::marksRects()
{
	QVector< QRect > rects;

	rects << QRect( position.x() - marksRadius - displacement,
					position.y() - marksRadius - displacement,
					marksRadius * 2, marksRadius * 2 );

	rects << QRect( position.x() + position.width() / 2 - marksRadius - 1,
					position.y() - marksRadius - displacement,
					marksRadius * 2, marksRadius * 2 );

	rects << QRect( position.x() + position.width() - marksRadius - 1 + displacement,
					position.y() - marksRadius - displacement,
					marksRadius * 2, marksRadius * 2 );

	rects << QRect( position.x() + position.width() - marksRadius - 1 + displacement,
					position.y() + position.height() / 2 - marksRadius - 1,
					marksRadius * 2, marksRadius * 2 );

	rects << QRect( position.x() + position.width() - marksRadius - 1 + displacement,
					position.y() + position.height() - marksRadius - 1 + displacement,
					marksRadius * 2, marksRadius * 2 );

	rects << QRect( position.x() + position.width() / 2 - marksRadius - 1,
					position.y() + position.height() - marksRadius - 1 + displacement,
					marksRadius * 2, marksRadius * 2 );

	rects << QRect( position.x() - marksRadius - displacement,
					position.y() + position.height() - marksRadius - 1 + displacement,
					marksRadius * 2, marksRadius * 2 );

	rects << QRect( position.x() - marksRadius - displacement,
					position.y() + position.height() / 2 - marksRadius - 1,
					marksRadius * 2, marksRadius * 2 );

	return rects;
}

void GSelectionRect::normalize()
{
	QRect oldPosition = position;
	position = position.normalized();
	recalcCenter( oldPosition );
}

/**TODO Дописать функции */
 QPoint GSelectionRect::getShearPoint()
 {
	 return this->shearPoint;
 }

 void GSelectionRect::setShearPoint(QPoint sp)
 {
	 this->shearPoint = sp;
 }

 QPointF GSelectionRect::getShearOffsets()
 {
	 return this->shearOffsets;
 }

 void GSelectionRect::setShearOffsets(QPointF so)
 {
	 this->shearOffsets = so;
 }

 qreal GSelectionRect::getRotateAngle()
 {
	 return this->rotateAngle;
 }

 void GSelectionRect::setScale(qreal dx, qreal dy)
 {
     QPoint *p = new QPoint(getPosition().center().x(),getPosition().center().y());
     if(dx<0)
         p->setX(p->x()+1);
     if(dy<0)
         p->setY(p->y()+1);
     selectedObjects->scale(dx,dy,*p);
     position = selectedObjects->boundingRect().toRect();
     emit changed();
     emit StateChanged(QString("Scale"));
 }

 void GSelectionRect::setRotateAngle(qreal ra)
 {
     selectedObjects->rotate(ra,getPosition().center());
     position = selectedObjects->boundingRect().toRect();
     emit changed();
     emit StateChanged(QString("Rotate %1").arg(ra));
 }

 int GSelectionRect::getSelectedObjectPoint()
 {
	 return this->selectedObjectPoint;
 }

 void GSelectionRect::setSelectedObjectPoint(int sop)
 {
	 this->selectedObjectPoint = sop;
 }

 QWidget * GSelectionRect::getParent()
 {
	 return this->_parent;
 }

 void GSelectionRect::setParent(QWidget * p)
 {
	 this->_parent = p;
 }
