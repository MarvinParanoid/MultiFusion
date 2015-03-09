/**
 * UndoStructure.h
 *
 *  Created on: 13.06.2009
 *      Author: Kukushkin Alexey
 */
#ifndef UNDOSTRUCTURE_H_
#define UNDOSTRUCTURE_H_

#include <QObject>
#include "RPW.h"
#include "../interfaces/BackgroundInterface.h"


class UndoStructure:public QObject
{
	Q_OBJECT
	public:

		UndoStructure(QObject* realPainter)
		{

			RealPaintWidget* rpw = REALPAINTWIDGET(realPainter);
			//QMessageBox::warning(WIDGET(this),"Trace2","2");
			fixedSize = rpw->fixedSize;
			size = rpw->size;

			for(int i=0; i<rpw->layers.size(); i++)
				layers.append(rpw->layers[i]->copyLayer());

			//background = BACKGROUND(rpw->background)->copyBackground();
			currentLayer = rpw->currentLayer;
			currentFrame = rpw->currentFrame;
			inKeyPressedHandler = rpw->inKeyPressedHandler;
			inSelectionMode = rpw->inSelectionMode;
			selectionRect = rpw->selectionRect;
		}

		~UndoStructure(){}

	private:

		QObject *background;
		bool fixedSize;
		QSize size;
		QVector <GLayer*> layers;
		int currentLayer;
		int currentFrame;
		bool inKeyPressedHandler;
		bool inSelectionMode;
		QRect selectionRect;

	friend class RealPaintWidget;
};

template < class T > inline UndoStructure * UNDO( T *o )
{
	return qobject_cast< UndoStructure* >( o );
}

#endif /* UNDOSTRUCTURE_H_ */
