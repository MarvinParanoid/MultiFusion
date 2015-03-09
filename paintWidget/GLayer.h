#ifndef GLAYER__H
#define GLAYER__H

#include "GObject.h"
#include "GContainer.h"
#include <QVector>
#include "GVectorFigure.h"
#include "../interfaces/GObjectInterface.h"

class GLayer: public GContainer
{
	Q_OBJECT
	public:

		GLayer():hideFigures(false){}

		~GLayer();

		virtual void saveLayer( QDataStream &stream ) const;

		virtual void loadLayer( QDataStream &stream );

		virtual GLayer* copyLayer();

		virtual int maxFrame();

		virtual void paintLayer( QPainter &p );

		virtual QPolygonF transformation(QPolygonF p1, QPolygonF p2);

		virtual int countFramesForLayer();

		virtual bool objectIsContainer(int object);

		virtual bool objectIsTransformed(int object, int frame);

		virtual void setObjectTransformed(int object,int frame, bool transform);

		virtual void setTransformations(int itemMin, int itemMax);

		virtual void setCurrentFrameForLayer(int frame, bool isPlayed);

		virtual void addFrameForLayer(int pos, bool visible);

		virtual	bool isContainsFrame(int pos);

		virtual void deleteFrameForLayer(int pos);

		virtual int getPositionFrame(int i);

		virtual QPoint getIndexFrames(int frame);

		virtual QVector <int> getFrames();

		virtual void setHideFigures(bool value);

		virtual bool getHideFigures();

	private:

		QVector <int> frames;
		bool hideFigures;
		QVector < GVectorFigure* > transformed;
		friend class PaintWidget;
};

#endif
