/**
* GVFInterface.h
*
*  Created on: 27.04.2009
*      Author: Alexey Kukushkin
*/

#ifndef GVFINTERFACE_H_
#define GVFINTERFACE_H_

#include <QObject>
#include "../gobject.h"

/**
*		Класс, реализующий векторную фигуру,
*			сотоящую из точек.
*/
class GVFInterface:public GObject
{

	public:
		/**
		*	Удаляет кадр с требуемым номером, если этот кадр существует.
		*
		*	@param frame номер удаляемого кадра.
		*/
		virtual void deleteFrame( int frame );

		/**
		*	Добавляет кадр в самое начало.
		*
		*	@param empty если параметр установлен в true, то будет добавлен пустой кадр,
		*		в противном случае будет скопирован предыдущий первый кадр (если он есть).
		*/
		virtual void addFrameToBegin( bool empty = false );

		/**
		*	Добавляет кадр в самый конец.
		*
		*	@param empty если параметр установлен в true, то будет добавлен пустой кадр,
		*		в противном случае будет скопирован предыдущий последний кадр (если он есть).
		*/
		virtual void addFrameToEnd( bool empty = false );

		/**
		*	Добавляет кадр после некоторого существуюшего кадра.
		*
		*	@param frame кадр, после которого нужно вставить новый.
		*	@param empty если параметр установлен в true, то будет добавлен пустой кадр,
		*		в противном случае будет скопирован указанный кадр (если он есть).
		*/
		virtual void addFrameAfter( int frame, bool empty = false );

		/**
		*	Добавляет кадр перед некоторым существуюшим кадром.
		*
		*	@param frame кадр, перед которым нужно вставить новый.
		*	@param empty если параметр установлен в true, то будет добавлен пустой кадр,
		*		в противном случае будет скопирован указанный кадр (если он есть).
		*/
		virtual void addFrameBefore( int frame, bool empty = false );

		/**
		*	Клонирует выбранный кадр и вставляет его после указанного.
		*
		*	@param frame кадр, который необходимо склонировать.
		*	@param pasteTo кадр, после которого будет вставлен новый кадр.
		*/
		virtual void cloneFrameBefore( int frame, int pasteTo );

		/**
		*	Клонирует выбранный кадр и вставляет его перед указанным.
		*
		*	@param frame кадр, который необходимо склонировать.
		*	@param pasteTo кадр, перед которым будет вставлен новый кадр.
		*/
		virtual void cloneFrameAfter( int frame, int pasteTo );

		/**
		*	Возвращает тип соединительных линий обьекта.
		*
		*	@return тип соединительных линий.
		*/
		virtual GLinesType type() const;


		/**
		*	Возвращает true, если в данный момент обьект можно редактировать.
		*
		*	@return true, если в данный момент обьект можно редактировать.
		*/
		virtual bool isEditable() const;


		/**
		*	Возвращает true, если обьект является контейнером.
		*
		*	@return true, если обьект является контейнером.
		*/
		virtual bool isContainer() const;

		/**
		*	Возвращает true, если обьект в данный момент можно видеть на экране.
		*
		*	@return true, если обьект в данный момент можно видеть на экране.
		*/
		virtual bool isVisible() const;

		/**
		*	Устанавливает видимость обьекта.
		*
		*	@param visible видимость обьекта.
		*/
		virtual void setVisible( bool visible );


		/**
		*	Возвращает указатель на обьект, который содержит данную точку.
		*
		*	@param p кординаты точки, по которой необходимо определить обьект.
		*
		*	@return указатель на обьект, который содержит данную точку.
		*
		*	@warning если ни один обьект в контейнере не содержит эту точку,
		*		то будет возвращён 0.
		*/
		virtual GObject* contains( const QPoint &p );


		/**
		*	Возвращает текущую позицию обьекта в анимации (текущий кадр).
		*
		*	@return текущую позицию обьекта в анимации (текущий кадр).
		*/
		virtual qreal frame() const;

		/**
		*	Устанвливает текущий кадр для обьекта.
		*
		*	@param f устанавливаемый кадр.
		*/
		virtual void setFrame( qreal f );

		/**
		*	Возвращает общее количество кадров, которое уже заданно для этого обьекта.
		*
		*	@return общее количество кадров, которое уже заданно для этого обьекта.
		*/
		virtual int countFrames() const;


		/**
		*	Возвращает координаты прямоугольника, в который вписан данный обьект.
		*
		*	@return координаты прямоугольника, в который вписан данный обьект.
		*/
		virtual QRectF boundingRect() const;

		/**
		*	Возвращает все точки, из которых состоит данная фигура.
		*
		*	@return точки, из которых состоит данная фигура.
		*/
		virtual QPolygonF points() const;

		/**
		*	Добавляет новую точку в фигуру.
		*
		*	@param p координаты добавляемой точки.
		*
		*	@return индекс новодобавленной точки.
		*/
		virtual int addPoint( const QPoint &p );

		/**
		*	Добавляет новую точку в фигуру, не используя различный алгоритмы выбора позиции -
		*		т.е. точка всегда добавляется в конец фигуры.
		*
		*	@param p координаты добавляемой точки.
		*
		*	@return индекс новодобавленной точки.
		*/
		virtual int addPointToEnd( const QPoint &p );

		/**
		*	Удаляет точку по её индексу из обьекта.
		*
		*	@param index индекс удаляемой точки.
		*
		*	@return true, если точка была удалена.
		*/
		virtual bool deletePoint( int index );

		/**
		*	Перемещает одну из точек фигуры на новое место.
		*
		*	@param index индекс перемещаемой точки.
		*	@param newPosition новое положение точки.
		*/
		virtual void movePoint( int index, const QPointF &newPosition );

		/**
		*	Клонирует один из кадров таким образом, что он замещает все остальные кадры.
		*
		*	@param frame клонируемый кадр.
		*/
		virtual void cloneFrameToAll( int frame );


		/**
		*	Производит паралельный перенос обьекта.
		*
		*	@param dx смещение по оси X.
		*	@param dy смещение по оси Y.
		*/
		virtual void move( qreal dx, qreal dy );

		/**
		*	Производит масштабирование обьекта.
		*
		*	@param sx коэффициент масштабирования по оси X.
		*	@param sy коэффициент масштабирования по оси Y.
		*	@param scaleCenter точка, относительно которой производится масштаирование.
		*/
		virtual void scale( qreal sx, qreal sy, const QPointF &scaleCenter );

		/**
		*	Производит паралельный сдвиг некоторой грани обьекта.
		*
		*	@param sx коэффициент сдвига по оси X.
		*	@param sy коэффициент сдвига по оси Y.
		*	@param scaleCenter точка, относительно которой производится сдвиг
		*					(т.е., собственно, точка, лежащая на сдвигаемой грани).
		*/
		virtual void sheare( qreal sx, qreal sy, const QPointF &shearPoint );

		/**
		*	Производит вращение фигуры.
		*
		*	@param angle угол, на который производится поворот.
		*	@param center точка, вокруг которой производится поворот.
		*/
		virtual void rotate( qreal angle, const QPointF &center );

		/**
		*	Возвращает тип и параметры линий обьекта.
		*
		*	@return тип и параметры линий обьекта.
		*/
		virtual QPen pen() const;

		/**
		*	Возвращает тип и параметры заливки обьекта.
		*
		*	@return тип и параметры заливки обьекта.
		*/
		virtual QBrush brush() const;

		/**
		*	Устанавливает тип и параметры линий обьекта.
		*
		*	@param p тип и параметры линий обьекта.
		*/
		virtual void setPen( const QPen &p );

		/**
		*	Устанавливает тип и параметры заливки обьекта.
		*
		*	@param p тип и параметры заливки обьекта.
		*/
		virtual void setBrush( const QBrush &b );

		/**
		*	Проверяет фигуру на замкнутость.
		*
		*	@return true, если фигура замкнута.
		*/
		virtual bool isClosed() const;

		/**
		*	Устанавливает замкнутость фигуры.
		*
		*	@param closed замкнутость фигуры.
		*/
		virtual void setClosed( bool _closed );

		/**
		*	Получение значения прозрачности фигуры.
		*
		*	@return значение прозрачности(от 0 до 255%).
		*/
		virtual int getAlpha();

		/**
		*	Задаёт прозрачность фигуры.
		*
		*	@param _alpha значение прозрачности(от 0 до 255).
		*/
		virtual void setAlpha( int _alpha );

		/**
		*	Отрисовывает целиком весь обьект.
		*
		*	@param p то, куда будет производится отрисовка.
		*/
		virtual void paint( QPainter &p ) const;

		/**
		*	Конструктор.
		*/
		GVFInterface( ) = 0;

		/**
		*	Деструктор.
		*/
		virtual ~GVFInterface();

	protected:

		/**
		*	Сохраняет весь обьект целиком в поток.
		*
		*	@param stream поток, в который сохраняется обьект.
		*/
		virtual void save( QDataStream &stream ) const;

		/**
		*	Читает весь обьект целиком из потока.
		*
		*	@param stream поток, из которого происходит чтение.
		*/
		virtual void load( QDataStream &stream );

	private:
		GFramesProperties properties;
		qreal _frame;
		GLinesType	_linesType;
		QPen alphaPen;
		QBrush alphaBrush;
		QPen _pen;
		QBrush _brush;
		bool closed;

		virtual GProperties currentFrame() const;
		virtual void paintObject( QPainter &p, const GProperties &props ) const;

		void drawSplines( QPainter &p, const GProperties &props ) const;

		QPen interpolatedPen() const;
		QBrush interpolatedBrush() const;

		int addPointToPolygon( const QPointF &p );
		int addPointToEndOfPolygon( const QPointF &p );
		int addPointToSpline( const QPointF &p );
};


Q_DECLARE_INTERFACE( GVFInterface, "com.Tailer.MultiFusion.GVFInterface/1.0" )


#endif /* GVFINTERFACE_H_ */
