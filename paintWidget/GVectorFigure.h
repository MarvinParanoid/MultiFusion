#ifndef __GVECTOR_FIGURE_H__
#define __GVECTOR_FIGURE_H__

#include <QObject>
#include <QGradient>
#include <QPainter>
#include <QPainterPath>

#include "GObject.h"
#include "GFramesProperties.h"
/**
*	@file gvectorfigure.h Данный файл содержит описание класса, реализующего векторную фигуру,
*			сотоящую из точек.
*/

/**
*		Класс, реализующий векторную фигуру,
*			сотоящую из точек.
*/
class GVectorFigure:public GObject
{
	Q_OBJECT

	public:

		virtual void setHideLines(int value);

		virtual int getHideLines();

		/**
		 * Переводит фигуру в несплайновый режим
		 */
		virtual void toNormal();
		/**
		 * Переводит фигуру в сплайновый режим
		 */
		virtual void toSpline();

		/**
		 * Проверяет является ли фигура в сплайновой
		 */
		virtual bool isSpline();

		/**
		 * Задаёт отбражать для фигуры линии Бизье или нет
		 * @param value true если отображать
		 */
		virtual void setShowBezier(bool value);

        /**
         *  Установление точек фигуре
         *
         *  p - устанавливаемый набор точек
         *  position - кадр, в который устанавливаются точки
         */
        virtual void setPoints(QPolygonF p, int position);

        /**
        *	Заменяет кадр по позиции c определёнными точками.
        *	@param polF - точки
        *	@param position - позиция на таймлайне;
        */
        virtual void setPoints( int position, QPolygonF polF);

		/**
		 * Проверяет отбражать для фигуры линии Бизье или нет
		 * @return true если отображать
		 */
		virtual bool isShowBezier();

		/**
		 *  Копирование текущего объекта
		 */
		virtual GObjectInterface* copyObject();

		/**
		*	Удаляет кадр с требуемым номером, если этот кадр существует.
		*
		*	@param frame номер удаляемого кадра.
		*/
		virtual void deleteFrame( int position );

		/**
		 * Удаляет все кадры кроме frame
		 * @param frame, если равен -1 удаляет абсолютно все кадры
		 */
		virtual void deleteFrames( int frame );

		/**
		*	Добавляет кадр по позиции.
		*
		*	@param position - позиция на таймлайне;
		*/
		virtual void addFrame( int position, bool visible );

		/**
		*	Добавляет кадр по позиции c определёнными точками.
		*	@param polF - точки
		*	@param position - позиция на таймлайне;
		*/
		virtual void addFrameWithPoints( int position, QPolygonF polF);

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
		 * Возвращает индекс кадра в списке по позиции в таймлайне
		 */
		virtual int getIndexFrameByPosition(qreal frame) const;

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
		*	Возвращает true, если обьект в данном кадре является трансформируемым.
		*	@param frame данный кадр
		*	@return true, если обьект в данный момент блокирован
		*/
		virtual bool isTransformed(int frame) const;

		/**
		*	Устанавливает обьект в данном кадре трансформируемым если transform равен true.
		*	@param frame данный кадр
		*/
		virtual void setTransformed(int frame, bool transform);

		/**
		*	Возвращает true, если обьект в данный момент блокирован.
		*
		*	@return true, если обьект в данный момент блокирован
		*/
		virtual bool isBlocked() const;

		/**
		*	Устанавливает блокировку объекта.
		*
		*	@param blocked блокировка объекта.
		*/
		virtual void setBlocked( bool blocked );

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
		virtual QRectF boundingRect();

		/**
		*	Возвращает все точки, из которых состоит данная фигура.
		*
		*	@return точки, из которых состоит данная фигура.
		*/
		virtual QPolygonF points(int frame);

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
		 * Определяет индексы фреймов между которыми находится курсор
		 */
		virtual bool getIndexFrames();

		/**
		*	Отрисовывает целиком весь обьект.
		*
		*	@param p то, куда будет производится отрисовка.
		*/
		virtual void paint( QPainter &p );

				/**
				*	Конструктор.
				*
				*	@param points точки, из которых будет состоят новосозданный обьект.
				*		Значение по умолчанию - пустой QPolygonF (т.е. фигура не будет содержать ни одной точки).
				*
				*	@param _spline отвечает за то, будут ли соединены точки сплайнами или линиями.
				*		Значение по умолчанию - false (т.е. фигура будет соединена линиями).
				*
				*	@param _closed отвечает за замкнутость фигуры.
				*		Значение по умолчанию - false (т.е. фигура не будет замкнута).
				*/
				GVectorFigure( const QPolygonF &points = QPolygonF(),
								bool _spline = false, bool _closed = false, int position = 0);

				/**
				 * Копирующий конструктор
				 */
				GVectorFigure( GVectorFigure* vf );

		/**
		*	Деструктор.
		*/
		virtual ~GVectorFigure();

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
		bool showBiese;
		qreal _frame;
		int itemMax;
		int itemMin;
		GLinesType	_linesType;
		QPen alphaPen;
		QBrush alphaBrush;
		QPen _pen;
		int hideLines;
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

template < class T > inline GVectorFigure* GVECTORFIGURE( T *o )
{
	return qobject_cast< GVectorFigure* >( o );
}


#endif /* __GVECTOR_FIGURE_H__ */
