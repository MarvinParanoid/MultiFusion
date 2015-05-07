#include "GObject.h"
#include "GContainer.h"
#include "GVectorFigure.h"
#include "../pluginTool/Plugin.h"
#include <QMessageBox>

static const QString SIGNATURE = "SACS2_GOBJECT_1.0";

void GObject::saveObject( QDataStream &stream, const GObjectInterface *o )
{
	if( o == 0 ) return;

	stream << SIGNATURE;
	stream << o->isContainer();
	o->save( stream );
}

GObjectInterface *GObject::loadObject( QDataStream &stream )
{
	QString s;
	stream >> s;
	if( s != SIGNATURE )
		return 0;

	bool container;
	stream >> container;

	GObjectInterface *o = 0;
	if( container )
		o = new GContainer();
	else
		o = new GVectorFigure();

	o->load( stream );
	return o;
}

GObjectInterface *GObject::create( QObject* figureTool,
		const QPointF &pos, int number, int position)
{

	FigureToolInterface* ft = FIGURETOOL(figureTool);
	FigureToolInterface::FiguresInfo info = ft->figure();
	GVectorFigure *f = new GVectorFigure( info[number].points, info[number].spline, info[number].closed, position );
	f->setFrame(position);
	f->move( pos.x(), pos.y() );
	//QMessageBox::warning(WIDGET(f->parent()),"Trace",QString::number(pos.x()));
	f->setObjectName( info[number].name );
	return f;
}

GObjectInterface *GObject::create( QObject* figureTool,
					const QPointF &pos, const QString &name, int positon )
{
	FigureToolInterface* ft = FIGURETOOL(figureTool);
	FigureToolInterface::FiguresInfo info = ft->figure();
	int countObjects = info.size();
	if( countObjects <= 0 )
		return 0;

	if( countObjects == 1 )
		return create(figureTool, pos, 0, positon);

	GContainer *c = new GContainer;
	for( int i = 0; i < countObjects; i++ )
		c->add( GOBJECT(create(figureTool, pos, i , positon)), true );

	c->setObjectName( name );
	return GOBJECTINTEFACE(c);
}
