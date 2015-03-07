#include "GContainer.h"
#include "GVectorfigure.h"
#include "../PluginTool/Plugin.h"
#include <QMessageBox>

GObjectInterface* GContainer::copyObject()
{
	GContainer* c = new GContainer();
	c->setObjectName(objectName());
	int count = countObjects();
	for( int i = 0; i < count; i++ )
		c->objects.append(GOBJECT(objects[i]->copyObject()));
	return c;
}

bool GContainer::isContainer() const
{
	return true;
}

void GContainer::deleteFrames( int frame )
{
	int count = countObjects();
	for( int i = 0; i < count; i++ )
		objects[i]->deleteFrames( frame );
}

void GContainer::deleteFrame( int position )
{
	int count = countObjects();
	for( int i = 0; i < count; i++ )
		objects[i]->deleteFrame( position );
}

void GContainer::addFrame( int position ,bool visible)
{
	int count = countObjects();
	for( int i = 0; i < count; i++ )
		objects[i]->addFrame( position, visible );
}

void GContainer::cloneFrameBefore( int frame, int pasteTo )
{
	if( ( frame < 0 ) || ( frame >= countFrames() ) )
		return;

	if( ( pasteTo < 0 ) || ( pasteTo >= countFrames() ) )
		return;

	int count = countObjects();
	for( int i = 0; i < count; i++ )
		objects[i]->cloneFrameBefore( frame, pasteTo );
}

void GContainer::cloneFrameAfter( int frame, int pasteTo )
{
	int count = countObjects();
	for( int i = 0; i < count; i++ )
		objects[i]->cloneFrameAfter( frame, pasteTo );
}

QString GContainer::objectName( int index ) const
{
	if( ( index < 0 ) || ( index >= objects.size() ) )
		return QString();

	return objects[ index ]->objectName();
}

void GContainer::setObjectName( int index, const QString &newName )
{
	if( ( index < 0 ) || ( index >= objects.size() ) )
		return;

	objects[ index ]->setObjectName( newName );
}

int GContainer::add( GObject *o, bool realChild )
{
	objects.prepend( o );
	if( realChild ) o->setParent( this );
	return ( objects.size() - 1 );
}

GObject* GContainer::remove( int index )
{
	if( ( index < 0 ) || ( index >= objects.size() ) )
		return 0;

	GObject * o = objects[ index ];
	objects.remove( index );
	return o;
}

QVector< GObject* > GContainer::removeAll()
{
	QVector< GObject* > ret = objects;

	objects.clear();
	return ret;
}

bool GContainer::isInside( GObject *o )
{
	return objects.contains( o );
}

int GContainer::objectIndex( GObject *o )
{
	return objects.indexOf( o );
}

GObject* GContainer::object( int index )
{
	if( ( index < 0 ) || ( index >= objects.size() ) )
		return 0;

	return objects[ index ];
}

int GContainer::moveDown( int index )
{
	if( ( index < 0 ) || ( index >= ( objects.size() - 1 ) ) )
		return -1;

	GObject * o = objects[ index ];
	objects.remove( index );
	objects.insert( index + 1, o );

	return ( index + 1 );
}

int GContainer::moveUp( int index )
{
	if( ( index <= 0 ) || ( index >= objects.size() ) )
		return -1;

	GObject * o = objects[ index ];
	objects.remove( index );
	objects.insert( index - 1, o );

	return ( index - 1 );
}

int GContainer::moveToFirst( int index )
{
	if( ( index <= 0 ) || ( index >= objects.size() ) )
		return -1;

	GObject * o = objects[ index ];
	objects.remove( index );
	objects.prepend( o );

	return 0;
}

int GContainer::moveToLast( int index )
{
	if( ( index < 0 ) || ( index >= ( objects.size() - 1 ) ) )
		return -1;

	GObject * o = objects[ index ];
	objects.remove( index );
	objects.append( o );

	return ( objects.size() - 1 );
}

int GContainer::move( int index, int to )
{
	if( index == to ) return -1;

	if( ( index < 0 ) || ( index >= objects.size() ) )
		return -1;

	if( ( to < 0 ) || ( to >= objects.size() ) )
		return -1;

	GObject * o = objects[ index ];
	objects.remove( index );

	if( to == objects.size() )
	{
		objects.append( o );
		return to;
	}

	//int i = to;
	//if( index < to ) i--;
	objects.insert( to, o );
	return to;
	//return i;
}

GObject::GLinesType GContainer::type() const
{
	if( countVisibleObjects() != 1 )
		return GObject::gNormal;

	int countObjects = objects.size();
	for( int i = 0; i < countObjects; i++ )
	{
		if( !objects[i]->isVisible() )
			continue;

		return objects[i]->type();
	}

	return GObject::gNormal;
}

bool GContainer::isEditable() const
{
	if( countVisibleObjects() <= 0 ) return false;

	int countObjects = objects.size();
	for( int i = 0; i < countObjects; i++ )
	{
		if( !objects[i]->isVisible() )
			continue;

		return objects[i]->isEditable();
	}

	return false;
}

bool GContainer::isBlocked() const
{
	int countObjects = objects.size();
	for( int i = 0; i < countObjects; i++ )
	{
		if( !objects[i]->isBlocked() )
			return false;
	}

	return true;
}

void GContainer::setBlocked( bool blocked )
{
	int countObjects = objects.size();
	for( int i = 0; i < countObjects; i++ )
		objects[i]->setBlocked( blocked );
}

bool GContainer::isObjectBlocked( int index ) const
{
	if( ( index < 0 ) || ( index >= objects.size() ) )
		return false;

	return objects[ index ]->isBlocked();
}

void GContainer::setObjectBlocked( int index, bool blocked )
{
	if( ( index < 0 ) || ( index >= objects.size() ) )
		return;

	objects[ index ]->setBlocked( blocked );
}

bool GContainer::isObjectVisible( int index ) const
{
	if( ( index < 0 ) || ( index >= objects.size() ) )
		return false;

	return objects[ index ]->isVisible();
}

void GContainer::setObjectVisible( int index, bool visible )
{
	if( ( index < 0 ) || ( index >= objects.size() ) )
		return;

	objects[ index ]->setVisible( visible );
}

bool GContainer::isVisible() const
{
	int countObjects = objects.size();
	for( int i = 0; i < countObjects; i++ )
	{
		if( objects[i]->isVisible() )
			return true;
	}

	return false;
}

void GContainer::setVisible( bool visible )
{
	int countObjects = objects.size();
	for( int i = 0; i < countObjects; i++ )
		objects[i]->setVisible( visible );
}

GObject* GContainer::contains( const QPoint &p )
{
	if( objects.size() <= 0 ) return 0;

	int countObjects = objects.size();
	for( int i = 0; i < countObjects; i++ )
	{
		if( !objects[i]->isVisible() || objects[i]->isBlocked())
			continue;

		if( objects[i]->contains( p ) != 0 )
			return objects[i];
	}

	return 0;
}

qreal GContainer::frame() const
{
	if( objects.size() <= 0 ) return 0.0;
	return objects[ 0 ]->frame();
}

void GContainer::setFrame( const qreal f )
{
	int countObjects = objects.size();
	for( int i = 0; i < countObjects; i++ )
		objects[i]->setFrame( f );
}

int GContainer::countFrames() const
{
	if( objects.size() <= 0 ) return 0;
	return objects[ 0 ]->countFrames();
}

int GContainer::countObjects() const
{
	return objects.size();
}

QRectF GContainer::boundingRect()
{
	if( ( countFrames() <= 0 ) || ( countVisibleObjects() <= 0 ) )
		return QRectF();


	int countObjects = objects.size();
	QRectF bound;

	for( int i = 0; i < countObjects; i++ )
	{
		if( !objects[i]->isVisible() )
			continue;
		bound |= objects[i]->boundingRect();

	}

	return bound;
}

bool GContainer::deletePoint( int index )
{
	if( !isEditable() || ( countVisibleObjects() > 1 ) )
		return -1;

	return objects[0]->deletePoint( index );
}

int GContainer::addPoint( const QPoint &p )
{
	if( !isEditable() || ( countVisibleObjects() > 1 ) )
		return -1;

	return objects[0]->addPoint( p );
}

int GContainer::addPointToEnd( const QPoint &p )
{
	if( !isEditable() || ( countVisibleObjects() > 1 ) )
		return -1;

	return objects[0]->addPointToEnd( p );
}

void GContainer::setAlpha( int alpha)
{
	if( !isEditable() )
		return;

	objects[0]->setAlpha(alpha);
}

int GContainer::getAlpha()
{
	if( !isEditable() )
		return -1;

	return objects[0]->getAlpha();
}

void GContainer::move( qreal dx, qreal dy )
{
	if( !isEditable() )
		return;

	int countObjects = objects.size();
	for( int i = 0; i < countObjects; i++ )
		objects[i]->move( dx, dy );
}

void GContainer::scale( qreal sx, qreal sy, const QPointF &scaleCenter )
{
	if( !isEditable() )
		return;

	int countObjects = objects.size();
	for( int i = 0; i < countObjects; i++ )
		objects[i]->scale( sx, sy, scaleCenter );
}

void GContainer::sheare( qreal sx, qreal sy, const QPointF &shearPoint )
{
	if( !isEditable() )
		return;

	int countObjects = objects.size();
	for( int i = 0; i < countObjects; i++ )
		objects[i]->sheare( sx, sy, shearPoint );
}

void GContainer::rotate( qreal angle, const QPointF &center )
{
	if( !isEditable() )
		return;

	int countObjects = objects.size();
	for( int i = 0; i < countObjects; i++ )
		objects[i]->rotate( angle, center );
}

QPolygonF GContainer::points(int frame)
{
	if( !isEditable() || ( countVisibleObjects() > 1 ) )
		return QPolygonF();

	return objects[0]->points(frame);
}

void GContainer::movePoint( int index, const QPointF &newPosition )
{
	if( !isEditable() || ( countVisibleObjects() > 1 ) )
		return;

	objects[0]->movePoint( index, newPosition );
}

void GContainer::cloneFrameToAll( int frame )
{
	if( ( frame < 0 ) || ( frame >= countFrames() ) )
		return;

	int countObjects = objects.size();
	if( !isEditable() || ( countObjects <= 0 ) )
		return;

	for( int i = 0; i < countObjects; i++ )
		objects[i]->cloneFrameToAll( frame );
}

QPen GContainer::pen() const
{
	if( ( countVisibleObjects() <= 0 ) || ( !isEditable() ) ) return QPen();

	int countObjects = objects.size();
	for( int i = 0; i < countObjects; i++ )
	{
		if( !objects[i]->isVisible() )
			continue;

		return objects[i]->pen();
	}

	return QPen();
}

QBrush GContainer::brush() const
{
	if( ( countVisibleObjects() <= 0 ) || ( !isEditable() ) ) return QBrush();

	int countObjects = objects.size();
	for( int i = 0; i < countObjects; i++ )
	{
		if( !objects[i]->isVisible() )
			continue;

		return objects[i]->brush();
	}

	return QBrush();
}

void GContainer::setPen( const QPen &p )
{
	if( ( countVisibleObjects() <= 0 ) || ( !isEditable() ) ) return;

	int countObjects = objects.size();
	for( int i = 0; i < countObjects; i++ )
	{
		if( !objects[i]->isVisible() )
			continue;

		objects[i]->setPen( p );
	}
}

void GContainer::setBrush( const QBrush &b )
{
	if( ( countVisibleObjects() <= 0 ) || ( !isEditable() ) ) return;

	int countObjects = objects.size();
	for( int i = 0; i < countObjects; i++ )
	{
		if( !objects[i]->isVisible() )
			continue;

		objects[i]->setBrush( b );
	}
}

bool GContainer::isClosed() const
{
	if( ( countVisibleObjects() <= 0 ) || ( !isEditable() ) ) return false;

	int countObjects = objects.size();
	for( int i = 0; i < countObjects; i++ )
	{
		if( !objects[i]->isVisible() )
			continue;

		return objects[i]->isClosed();
	}

	return false;
}

void GContainer::setClosed( bool closed )
{
	if( ( countVisibleObjects() <= 0 ) || ( !isEditable() ) ) return;

	int countObjects = objects.size();
	for( int i = 0; i < countObjects; i++ )
	{
		if( !objects[i]->isVisible() )
			continue;

		objects[i]->setClosed( closed );
	}
}

void GContainer::save( QDataStream &stream ) const
{
	int count = objects.size();
	stream << objectName();
	stream << count;
	for( int i = 0; i < count; i++ )
	{
		stream << objects[i]->isContainer();
		objects[i]->save( stream );
	}
}

void GContainer::load( QDataStream &stream )
{
	int count = objects.size();

	QString name;
	stream >> name;
	setObjectName( name );

	count = 0;
	stream >> count;

	bool container = false;
	GObject *o = 0;
	for( int i = 0; i < count; i++ )
	{
		stream >> container;
		if( container )
			o = new GContainer();
		else
			o = new GVectorFigure();

		o->load( stream );
		objects.append( o );
		o->setParent( this );
	}
}

void GContainer::paint( QPainter &p )
{
	int countObjects = objects.size();
	for( int i = countObjects - 1; i >= 0 ; i-- )
	{
		if( !objects[i]->isVisible() )
			continue;

		objects[i]->paint( p );
	}
}

GContainer::GContainer()
{
}

GContainer::~GContainer()
{
}

int GContainer::countVisibleObjects() const
{
	int countObjects = objects.size();
	int count = 0;
	for( int i = 0; i < countObjects; i++ )
		if( objects[i]->isVisible() ) count++;

	return count;
}
