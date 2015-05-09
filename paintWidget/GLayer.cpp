/**
* GLayer.cpp
*
*  Created on: 08.06.2009
*      Author: Alexey Kukushkin
*/
#include <QtAlgorithms>
#include <QMessageBox>
#include "GLayer.h"
#include "../pluginTool/Plugin.h"
#include "GVectorFigure.h"

GLayer::~GLayer(){};

GLayer* GLayer::copyLayer()
{
	GLayer* l = new GLayer();
	l->setObjectName(objectName());
	int count = frames.size();
	for(int i = 0; i < count; i++)
		l->frames.append(frames[i]);

	//QMessageBox::warning(WIDGET(this),"Trace1","2");
	count = countObjects();
	for( int i = 0; i < count; i++ )
		l->objects.append(GOBJECT(objects[i]->copyObject()));

	return l;
}

void GLayer::setHideFigures(bool value)
{
	hideFigures = value;
}

bool GLayer::getHideFigures()
{
	return hideFigures;
}

void GLayer::saveLayer( QDataStream &stream ) const
{
	int countFrames = frames.size();
	stream << countFrames;
	for( int i = 0; i < countFrames; i++ )
	{
		stream << frames[i];
	}
	save(stream);
}

void GLayer::loadLayer( QDataStream &stream )
{

	int countFrames = 0;
	stream >> countFrames;
	int value = 0;

	while(frames.size()>0)
		frames.remove(0);

	while(countFrames>0)
	{
		stream >> value;
		frames.append(value);
		countFrames--;
	}
	load(stream);
}

int GLayer::maxFrame()
{
	return frames[frames.size()-1];
}

void GLayer::paintLayer( QPainter &p )
{
	int countObjects = objects.size();
    for( int i = countObjects - 1; i >= 0 ; i-- )
        objects[i]->paint( p );

	int countTransform = transformed.size();
	for( int i = countTransform - 1; i >= 0 ; i-- )
	{
		transformed[i]->paint( p );
	}

}

QPoint GLayer::getIndexFrames(int frame)
{
	QPoint item;
	item.setX(-1);
	item.setY(-1);
	int count = frames.size();
	int index = -1;

	for(int i=0; i<count; i++)
	{
		int p = frames[i];
		if(p == frame)
		{
			item.setX(p);
			item.setY(p);
			return item;
		}
		if(p < frame && p > item.x())
		{
			item.setX(p);
			index = i;
		}
	}

	if(item.x() != -1 && index < count-1)
		item.setY(frames[index+1]);

	return item;
}

QPolygonF GLayer::transformation(QPolygonF p1, QPolygonF p2)
{
	int count1 = (p1.count() + 2) / 3;
	int count2 = (p2.count() + 2) / 3;
	int ost = count2 % count1;
	int div =  (count2 - ost) / count1;
	QPolygonF newP;


	for(int j=0; j<div; j++)
	{
		if(j == 0 && j == (div-1) && ost == 0 )
		{
		  newP << p1[0] << p1[1];
		  break;
		}

		if(j == (div-1) && ost == 0 )
		{
		  newP << p1[0] << p1[0] << p1[1];
		  break;
		}

		if(j == 0 )
		{
			newP << p1[0] << p1[0];
			continue;
		}

		newP << p1[0] << p1[0] << p1[0];
	}

	if(ost!=0)
	{
		newP << p1[0] << p1[0] << p1[1];
		ost--;
	}

	for(int i=2; i< p1.count() - 2; i+=3)
	{
		for(int j=0; j<div; j++)
		{
			if(j == 0 && j == (div-1))
				newP << p1[i] << p1[i+1] << p1[i+2];
			else if(j == 0)
			{
				newP << p1[i] << p1[i+1] << p1[i+1];
			}
			else if(j == (div-1))
			{
				newP << p1[i+1] << p1[i+1] << p1[i+2];
			}
			else{
				newP << p1[i+1] << p1[i+1] << p1[i+1];
			}
		}

		if(ost!=0)
		{
			newP << p1[i] << p1[i+1] << p1[i+2];
			ost--;
		}
	}

	for(int j=0; j<div; j++)
	{
		if(j == 0 && j == (div-1))
		{
		  newP << p1[p1.count()-2] << p1[p1.count()-1];
		  break;
		}

		if(j != div-1)
			newP << p1[p1.count()-2] << p1[p1.count()-1] << p1[p1.count()-1];
		else
			newP << p1[p1.count()-1] << p1[p1.count()-1];
	}

	return newP;
}

void GLayer::setTransformations(int itemMin, int itemMax)
{
	QVector < GVectorFigure* > first;
	QVector < GVectorFigure* > last;

	int count = objects.size();
	for(int i=0; i<count; i++)
	{
		if(!objects[i]->isContainer())
		{
			GVectorFigure *o = qobject_cast< GVectorFigure* >( objects[i] );

			if(o->isTransformed(itemMin))
				first.append(o);

			if(o->isTransformed(itemMax))
				last.append(o);

		}
	}

	QPolygonF pointsFirst;
	QPolygonF pointsLast;
	GVectorFigure *f;

	//QMessageBox::warning(WIDGET(this),"Trace(Message0):",QString::number(first.size())+", "+QString::number(last.size()));
	for(int i=0; i<first.size(); i++)
	{
		for(int j=0; j<last.size(); j++)
		{
			if(first[i] == last[j])
				continue;

			pointsFirst = first[i]->points(itemMin);
			pointsLast = last[j]->points(itemMax);

			if(pointsFirst.count() > pointsLast.count())
			{
				f = new GVectorFigure( pointsFirst, true, last[j]->isClosed(), itemMin );
				pointsLast = transformation(pointsLast,pointsFirst);
				f->addFrameWithPoints(itemMax, pointsLast);
			}
			else
			{
				f = new GVectorFigure( pointsLast, true, last[j]->isClosed(), itemMax );
				pointsFirst = transformation(pointsFirst, pointsLast);
				f->addFrameWithPoints(itemMin, pointsFirst);
			}
			f->setBrush(first[i]->brush());
			f->setPen(first[i]->pen());
			//QMessageBox::warning(WIDGET(this),"Trace(Message1):",QString::number(pointsFirst.size())+", "+QString::number(pointsFirst.size()));
			transformed.append(f);
		}
	}
}

void GLayer::setCurrentFrameForLayer(int frame, bool isPlayed)
{
	if(isPlayed)
	{

		int count = transformed.size();
		bool conFrame = isContainsFrame(frame);

		if(conFrame)
		{
			for(int i=0; i<count; i++)
			{
				delete transformed[i];
			}
			transformed.clear();
		}

		if(count == 0 && !conFrame)
		{

			QPoint p = getIndexFrames(frame);
			if(p.x() != p.y())
			{
				setTransformations(p.x(),p.y());
			}
		}

		for(int i=0; i<transformed.size(); i++)
			transformed[i]->setFrame(qreal(frame));

		setFrame(frame);
	}
	else
	{

		for(int i=0; i<transformed.size(); i++)
		{
			delete transformed[i];
		}
		transformed.clear();
		setFrame(frame);
	}
}

bool GLayer::isContainsFrame(int pos)
{
	if( frames.indexOf(pos)== -1 )
		return false;
	return true;
}

bool GLayer::objectIsTransformed(int object, int frame)
{
	if(objects[object]->isContainer())
		return false;

	GVectorFigure *o = qobject_cast< GVectorFigure* >( objects[object] );
	return o->isTransformed(frame);
}

void GLayer::setObjectTransformed(int object,int frame, bool transform)
{
	if(objects[object]->isContainer())
		return;
	GVectorFigure *o = qobject_cast< GVectorFigure* >( objects[object] );
	o->setTransformed(frame, transform);
}

bool GLayer::objectIsContainer(int object)
{
	if(object < objects.size() && object >= 0)
		return objects[object]->isContainer();
	return true;
}

int GLayer::countFramesForLayer()
{
	return frames.size();
}

void GLayer::addFrameForLayer(int pos, bool visible)
{
	frames.append(pos);
	qSort(frames);
	addFrame(pos,visible);
}

void GLayer::deleteFrameForLayer(int pos)
{
	frames.remove(frames.indexOf(pos));
	deleteFrame(pos);
}

int GLayer::getPositionFrame(int i)
{
	return frames[i];
}

QVector <int> GLayer::getFrames()
{
	return frames;
}
