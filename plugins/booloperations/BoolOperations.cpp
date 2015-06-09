/**
 * BoolOperations.cpp
 *
 *  Created on: 20.05.2015
 *      Author: Sergey Korolev
 */

#include "BoolOperations.h"

EXPORT_PLUGIN(BoolOperations, BoolOperationsInterface InterfacePlugin)

QPainterPath pathFromPolygon(QPolygonF &poly, bool closed = true)
{
	int countPoints = ((poly.size() - 1 ) / 3 ) * 3;
	QPainterPath path;
	path.moveTo(poly[0]);

	for ( int i = 1; i < ( countPoints + 1 ); i += 3 )
		path.cubicTo(poly[i], poly[i + 1], poly[i + 2]);

	if (closed)
		path.closeSubpath();

	return path;
}

bool getPolygonFromPath(QPainterPath &path, int &pos, QPolygonF &poly)
{
	int count = path.elementCount();
	if (pos >= count || !path.elementAt(pos).isMoveTo()) return false;

	poly << path.elementAt(pos);
	++pos;
	while (pos < count) {
		switch (path.elementAt(pos).type) {
		case QPainterPath::MoveToElement:
			return true;
		case QPainterPath::LineToElement:
			poly << path.elementAt(pos - 1) << path.elementAt(pos) << path.elementAt(pos);
			++pos;
			break;
		case QPainterPath::CurveToElement:
			poly << path.elementAt(pos) << path.elementAt(pos + 1) << path.elementAt(pos + 2);
			pos += 3;
			break;
		}
	}
	return true;
}

QList<QPolygonF> polygonsFromPath(QPainterPath &path)
{
	QList<QPolygonF> polys;
	int pos = 0;
	while (pos < path.elementCount()) {
		QPolygonF poly;
		if (!getPolygonFromPath(path, pos, poly))
			break;
		polys << poly;
	}
	return polys;
}

QPolygonF expandedPolygon(const QPolygonF& poly)
{
	QPolygonF p;
	for (int i = 0; i < poly.size(); ++i)
		if (i == 0 || i == poly.size() - 1)
			p << poly[i] << poly[i];
		else
			p << poly[i] << poly[i] << poly[i];
	return p;
}

QPolygonF squeezePolygon(const QPolygonF& poly)
{
	QPolygonF p;
	int c = ((poly.size() - 1 ) / 3 ) * 3;
	for (int i = 0; i < c; i += 3) {
		p << poly[i];
	}
	return p;
}

QPolygonF substractPolygonsFromPolygon(const QList<QPolygonF> &polygons, const QPolygonF &poly)
{
	int count = polygons.size();
	if (count == 0) return expandedPolygon(poly);

	QPolygonF p = squeezePolygon(poly);
	for (int i = 0; i < count; ++i)
		p = p.subtracted(squeezePolygon(polygons[i]));
	return expandedPolygon(p);
}

GVectorFigure* figureFromPolygon(const QPolygonF &poly, QPen pen = QPen(), QBrush brush = QBrush(), qreal curFrame=0)
{
	if (poly.empty()) return 0;

    GVectorFigure* f = new GVectorFigure(poly, false, true, 1);
	f->setBrush(brush);
	f->setPen(pen);
	f->setObjectName("Complex");
	return f;
}

bool unionWithPolygons(QPainterPath &path, QList<GObject *> &objects)
{
	for (QList<GObject *>::iterator it = objects.begin(); it != objects.end(); ++it) {
		QPolygonF poly = (*it)->points((*it)->frame());
		QPainterPath path2 = pathFromPolygon(poly);
		if (!path.intersects(path2)) continue;
		if (path.contains(path2) || path2.contains(path)) continue;
		path = path.united(path2);
		objects.erase(it);
		return true;
	}
	return false;
}

QList<GObject *> BoolOperations::getSelectionObjects()//Возврат списка с выделенными объектами
{
    QList<GObject *> objects;
    container = selection->getSelectedAsGContainer();
    int c = container->countObjects();
    for (int i = 0; i < c; ++i) {
        GObject* f = container->object(i);
        if (f->isClosed() && f->objectName() != tr("Line") && f->objectName() != tr("Spline"))
            objects.push_back(f);
    }
    return objects;
}

QList<GObject *> BoolOperations::setPolygonsToObjects(QList<QPolygonF> polygons, QList<GObject *> &objects)//Размещение Полигонов в фигуры
{
    if(objects.size()==0||polygons.size()==0)
        return objects;

    for(int i=0;i<polygons.size()&&i<objects.size();i++)
    {
            GVectorFigure* curFigure=(GVectorFigure*)objects[i];
            curFigure->setPoints(curFigure->frame(),polygons[i]);
    }
    return objects;
}


QList<GObject *> BoolOperations::delFramesInObjects(QList<QPolygonF> polygons, QList<GObject *> &objects)//удаление фигур в текущем слайде
{
    if(objects.size()==0||polygons.size()==0)
        return objects;

    for(int i=polygons.size();i<objects.size();i++)
    {
        GVectorFigure* curFigure=(GVectorFigure*)objects[i];
        curFigure->deleteFrame(curFigure->frame());
    }
    return objects;
}

QList<GObject *> BoolOperations::checkAndChangeListUnion(QList<GObject *> &objects)//проверка фигуры на соответствии операции и выполнение операции
{
    if(objects.size()<2)
        return objects;

    QPolygonF poly1 = objects.front()->points(objects.front()->frame());
    QPainterPath path = pathFromPolygon(poly1);

    int size=objects.size();
    for(int i=1;i<size;i++)
    {
        QPolygonF curPolygonF=objects[i]->points(objects[i]->frame());
        QPainterPath curPainterPath = pathFromPolygon(curPolygonF);
        if (!path.intersects(curPainterPath)||path.contains(curPainterPath) || curPainterPath.contains(path))
        {
            objects.removeAt(i);
            i--;
            size--;
        }
        else
            path=path.united(curPainterPath);

    }
    return objects;
}


void BoolOperations::createUnion()
{
	if (selection == 0) return;

    QList<GObject *> objects=getSelectionObjects();

	if (objects.size() < 2) {
		QMessageBox::information(0, tr("Error!"), tr("The count of selected and closed objects are less then two objects."));
		return;
	}


	QPen pen = objects.front()->pen();
	QBrush brush = objects.front()->brush();
	QPolygonF poly1 = objects.front()->points(objects.front()->frame());
	QPainterPath path = pathFromPolygon(poly1);
	objects.pop_front();
    while (unionWithPolygons(path, objects));
    QList<QPolygonF> polygons = polygonsFromPath(path);
	if (polygons.empty()) return;


    objects=getSelectionObjects();//полчение выделенных фигур
    objects=checkAndChangeListUnion(objects);//проверка на соответствии фигур
    if (objects.size()<2) return;

    setPolygonsToObjects(polygons,objects);//размещение полученных полигонов в существующих фигурах

    objects.front()->setObjectName("Complex union");

    delFramesInObjects(polygons,objects);//удаление фигур в текущем слайде

    // вызываем перерисовку и сохраняем в историю изменений
    selection->emitChanged();
    selection->emitStateChanged("Change Objects");

}

void BoolOperations::createDifference()
{
	if (selection == 0) return;
	container = selection->getSelectedAsGContainer();
	if (container->countObjects() != 2) {
		QMessageBox::information(0, tr("Error!"), tr("The count of selected objects are not equal two objects."));
		return;
	}
	GObject* fig1 = container->object(0);
	if (!fig1->isClosed() || fig1->objectName() == tr("Line") || fig1->objectName() == tr("Spline")) {
		QMessageBox::information(0, tr("Error!"), tr("First selected object is not closed."));
		return;
	}
	GObject* fig2 = container->object(1);
	if (!fig2->isClosed() || fig2->objectName() == tr("Line") || fig2->objectName() == tr("Spline")) {
		QMessageBox::information(0, tr("Error!"), tr("Second selected object is not closed."));
		return;
	}
	QPolygonF poly1 = fig1->points(fig1->frame());
	QPainterPath path1 = pathFromPolygon(poly1);
	QPolygonF poly2 = fig2->points(fig2->frame());
	QPainterPath path2 = pathFromPolygon(poly2);
	if (!path1.intersects(path2)) return;
	if (path1.contains(path2) || path2.contains(path1)) return;

	QPen pen = fig2->pen();
	QBrush brush = fig2->brush();
    QPainterPath path = path1.subtracted(path2).simplified();
	QList<QPolygonF> polygons = polygonsFromPath(path);
	if (polygons.empty()) return;

    QList<GObject *> objects=getSelectionObjects();//полчение выделенных фигур
    setPolygonsToObjects(polygons,objects);//размещение полученных полигонов в существующих фигурах
    objects.front()->setObjectName("Complex difference");
    delFramesInObjects(polygons,objects);//удаление фигур в текущем слайде

    painter->update();
    // вызываем перерисовку и сохраняем в историю изменений
    selection->emitChanged();
    selection->emitStateChanged("Change Objects");


    //painter->deleteSelected();

    /*for (QList<QPolygonF>::const_iterator it = polygons.begin(); it != polygons.end(); ++it)
        painter->addFigure(painter->selectedLayer(), figureFromPolygon(*it, pen, brush), true);*/
}

void BoolOperations::createIntersection()
{
	if (selection == 0) return;
	container = selection->getSelectedAsGContainer();
	if (container->countObjects() != 2) {
		QMessageBox::information(0, tr("Error!"), tr("The count of selected objects are not equal two objects."));
		return;
	}
	GObject* fig1 = container->object(0);
	if (!fig1->isClosed() || fig1->objectName() == tr("Line") || fig1->objectName() == tr("Spline")) {
		QMessageBox::information(0, tr("Error!"), tr("First selected object is not closed."));
		return;
	}
	GObject* fig2 = container->object(1);
	if (!fig2->isClosed() || fig2->objectName() == tr("Line") || fig2->objectName() == tr("Spline")) {
		QMessageBox::information(0, tr("Error!"), tr("Second selected object is not closed."));
		return;
	}
	QPolygonF poly1 = fig1->points(fig1->frame());
	QPainterPath path1 = pathFromPolygon(poly1);
	QPolygonF poly2 = fig2->points(fig2->frame());
	QPainterPath path2 = pathFromPolygon(poly2);
	if (!path1.intersects(path2)) return;
	if (path1.contains(path2) || path2.contains(path1)) return;

	QPen pen = fig2->pen();
	QBrush brush = fig2->brush();
    QPainterPath path = path1.intersected(path2);
	QList<QPolygonF> polygons = polygonsFromPath(path);
	if (polygons.empty()) return;

    QList<GObject *> objects=getSelectionObjects();//полчение выделенных фигур
    setPolygonsToObjects(polygons,objects);//размещение полученных полигонов в существующих фигурах
    objects.front()->setObjectName("Complex intersected");
    delFramesInObjects(polygons,objects);//удаление фигур в текущем слайде

    painter->update();
    // вызываем перерисовку и сохраняем в историю изменений
    selection->emitChanged();
    selection->emitStateChanged("Change Objects");

    /*painter->deleteSelected();

	for (QList<QPolygonF>::const_iterator it = polygons.begin(); it != polygons.end(); ++it)
        painter->addFigure(painter->selectedLayer(), figureFromPolygon(*it, pen, brush), true);*/
}

void BoolOperations::createExclusion()
{
	if (selection == 0) return;
	container = selection->getSelectedAsGContainer();
	if (container->countObjects() != 2) {
		QMessageBox::information(0, tr("Error!"), tr("The count of selected objects are not equal two objects."));
		return;
	}
	GObject* fig1 = container->object(0);
	if (!fig1->isClosed() || fig1->objectName() == tr("Line") || fig1->objectName() == tr("Spline")) {
		QMessageBox::information(0, tr("Error!"), tr("First selected object is not closed."));
		return;
	}
	GObject* fig2 = container->object(1);
	if (!fig2->isClosed() || fig2->objectName() == tr("Line") || fig2->objectName() == tr("Spline")) {
		QMessageBox::information(0, tr("Error!"), tr("Second selected object is not closed."));
		return;
	}
	QPolygonF poly1 = fig1->points(fig1->frame());
	QPainterPath path1 = pathFromPolygon(poly1);
	QPolygonF poly2 = fig2->points(fig2->frame());
	QPainterPath path2 = pathFromPolygon(poly2);
	if (!path1.intersects(path2)) return;

	QPen pen = fig2->pen();
	QBrush brush = fig2->brush();

	bool contains = false;
	QPainterPath united_path, intersect_path;
	if (path1.contains(path2)) {
		united_path = path1;
		intersect_path = path2;
		contains = true;
	} else if (path2.contains(path1)) {
		united_path = path2;
		intersect_path = path1;
		contains = true;
	} else {
		united_path = path1.united(path2);
	}
	QList<QPolygonF> united_polygons = polygonsFromPath(united_path);
	if (united_polygons.size() != 1) return;

	if (!contains)
		intersect_path = path1.intersected(path2);
	QList<QPolygonF> inter_polygons = polygonsFromPath(intersect_path);

	QPolygonF united_poly = squeezePolygon(united_polygons.front());
	QPolygonF inter_poly = squeezePolygon(inter_polygons.front());
	QPolygonF exclude_poly = expandedPolygon(united_poly.subtracted(inter_poly));


    QList<QPolygonF> polygons;
    polygons.append(exclude_poly);

    QList<GObject *> objects=getSelectionObjects();//полчение выделенных фигур
    setPolygonsToObjects(polygons,objects);//размещение полученных полигонов в существующих фигурах
    objects.front()->setObjectName("Complex exclusion");
    delFramesInObjects(polygons,objects);//удаление фигур в текущем слайде

    painter->update();
    // вызываем перерисовку и сохраняем в историю изменений
    selection->emitChanged();
    selection->emitStateChanged("Change Objects");

/*
	painter->deleteSelected();
    painter->addFigure(painter->selectedLayer(), figureFromPolygon(exclude_poly, pen, brush), true);*/
}

void BoolOperations::createDivision()
{
	if (selection == 0) return;
	container = selection->getSelectedAsGContainer();
	if (container->countObjects() != 2) {
		QMessageBox::information(0, tr("Error!"), tr("The count of selected objects are not equal two objects."));
		return;
	}
	GObject* fig1 = container->object(0);
	if (!fig1->isClosed() || fig1->objectName() == tr("Line") || fig1->objectName() == tr("Spline")) {
		QMessageBox::information(0, tr("Error!"), tr("First selected object is not closed."));
		return;
	}
	GObject* fig2 = container->object(1);
	if (!fig2->isClosed() || fig2->objectName() == tr("Line") || fig2->objectName() == tr("Spline")) {
		QMessageBox::information(0, tr("Error!"), tr("Second selected object is not closed."));
		return;
	}
	QPen pen = fig2->pen();
	QBrush brush = fig2->brush();
	QPolygonF poly1 = fig1->points(fig1->frame());
	QPainterPath path1 = pathFromPolygon(poly1);
	QPolygonF poly2 = fig2->points(fig2->frame());
	QPainterPath path2 = pathFromPolygon(poly2);

	if (!path1.intersects(path2)) return;
    //if (path1.contains(path2) || path2.contains(path1)) return;

    QPainterPath path3 = path1.subtracted(path2);
    QPainterPath path4 = path1.intersected(path2);

	QList<QPolygonF> polygons1 = polygonsFromPath(path3);
	QList<QPolygonF> polygons2 = polygonsFromPath(path4);
    polygons1.append(polygons2);
    if (polygons1.empty()) return;

    QList<QPolygonF> polygons=polygons1;

    QList<GObject *> objects=getSelectionObjects();//полчение выделенных фигур
    setPolygonsToObjects(polygons,objects);//размещение полученных полигонов в существующих фигурах
    objects.front()->setObjectName("Complex division 1");
    objects[1]->setObjectName("Complex division 2");
    delFramesInObjects(polygons,objects);//удаление фигур в текущем слайде

    painter->update();
    // вызываем перерисовку и сохраняем в историю изменений
    selection->emitChanged();
    selection->emitStateChanged("Change Objects");


    /*painter->deleteSelected();

	for (QList<QPolygonF>::const_iterator it = polygons2.begin(); it != polygons2.end(); ++it)
        painter->addFigure(painter->selectedLayer(), figureFromPolygon(*it, pen, brush), true);*/
}
