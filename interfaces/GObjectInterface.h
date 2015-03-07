/**
 * GObjectInterface.h
 *
 *  Created on: 24.04.2009
 *      Author: Alexey Kukushkin
 */

#ifndef GOBJECTINTERFACE_H_
#define GOBJECTINTERFACE_H_
#include <QApplication>
#include <QObject>
#include "FigureToolInterface.h"

class GObjectInterface:public QObject
{
	public:

		/**
		*	Определяет тип соединения точек обьекта.
		*/
		enum GLinesType
		{
			gNormal, /** точки будут соединены линиями */
			gSplines /** точки будут соединены сплайнами */
		};

		/**
		 *  Копирование текущего объекта
		 */
		virtual GObjectInterface* copyObject()=0;

		/**
		*	Добавляет кадр по позиции.
		*
		*	@param position - позиция на таймлайне;
		*/
		virtual void addFrame( int position ,bool visible) = 0;

		/**
		 * Удаляет все кадры кроме frame
		 * @param frame, если равен -1 удаляет абсолютно все кадры
		 */
		virtual void deleteFrames( int frame ) = 0;

		/**
		*	Удаляет кадр с требуемым номером, если этот кадр существует.
		*
		*	@param frame номер удаляемого кадра.
		*/
		virtual void deleteFrame( int position ) = 0;

		/**
		*	Клонирует выбранный кадр и вставляет его после указанного.
		*
		*	@param frame кадр, который необходимо склонировать.
		*	@param pasteTo кадр, после которого будет вставлен новый кадр.
		*/
		virtual void cloneFrameBefore( int frame, int pasteTo ) = 0;

		/**
		*	Клонирует выбранный кадр и вставляет его перед указанным.
		*
		*	@param frame кадр, который необходимо склонировать.
		*	@param pasteTo кадр, перед которым будет вставлен новый кадр.
		*/
		virtual void cloneFrameAfter( int frame, int pasteTo ) = 0;


		/**
		*	Возвращает тип соединительных линий обьекта.
		*
		*	@return тип соединительных линий.
		*/
		virtual GLinesType type() const = 0;

		/**
		*	Возвращает true, если в данный момент обьект можно редактировать.
		*
		*	@return true, если в данный момент обьект можно редактировать.
		*/
		virtual bool isEditable() const = 0;


		/**
		*	Возвращает true, если обьект является контейнером.
		*
		*	@return true, если обьект является контейнером.
		*/
		virtual bool isContainer() const = 0;

		/**
		*	Возвращает true, если обьект в данный момент можно видеть на экране.
		*
		*	@return true, если обьект в данный момент можно видеть на экране.
		*/
		virtual bool isVisible() const = 0;

		/**
		*	Устанавливает видимость обьекта.
		*
		*	@param visible видимость обьекта.
		*/
		virtual void setVisible( bool visible ) = 0;

		/**
		*	Возвращает true, если обьект в данный момент блокирован.
		*
		*	@return true, если обьект в данный момент блокирован
		*/
		virtual bool isBlocked() const = 0;

		/**
		*	Устанавливает блокировку объекта.
		*
		*	@param blocked блокировка объекта.
		*/
		virtual void setBlocked( bool blocked ) = 0;

		/**
		*	Возвращает текущую позицию обьекта в анимации (текущий кадр).
		*
		*	@return текущую позицию обьекта в анимации (текущий кадр).
		*/
		virtual qreal frame() const = 0;

		/**
		*	Устанвливает текущий кадр для обьекта.
		*
		*	@param f устанавливаемый кадр.
		*/
		virtual void setFrame( qreal f ) = 0;


		/**
		*	Возвращает общее количество кадров, которое уже заданно для этого обьекта.
		*
		*	@return общее количество кадров, которое уже заданно для этого обьекта.
		*/
		virtual int countFrames() const = 0;


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
		virtual GObjectInterface* contains( const QPoint &p ) = 0;

		/**
		*	Возвращает координаты прямоугольника, в который вписан данный обьект.
		*
		*	@return координаты прямоугольника, в который вписан данный обьект.
		*/
		virtual QRectF boundingRect() = 0;
		/**
		*	Получение значения прозрачности фигуры.
		*
		*	@return значение прозрачности(от 0 до 255%).
		*/
		virtual int getAlpha() = 0;

		/**
		*	Задаёт прозрачность фигуры.
		*
		*	@param _alpha значение прозрачности(от 0 до 255).
		*/
		virtual void setAlpha( int _alpha ) = 0;
		/**
		*	Производит паралельный перенос обьекта.
		*
		*	@param dx смещение по оси X.
		*	@param dy смещение по оси Y.
		*/

		virtual void move( qreal dx, qreal dy ) = 0;

		/**
		*	Производит масштабирование обьекта.
		*
		*	@param sx коэффициент масштабирования по оси X.
		*	@param sy коэффициент масштабирования по оси Y.
		*	@param scaleCenter точка, относительно которой производится масштаирование.
		*/
		virtual void scale( qreal sx, qreal sy, const QPointF &scaleCenter ) = 0;

		/**
		*	Производит паралельный сдвиг некоторой грани обьекта.
		*
		*	@param sx коэффициент сдвига по оси X.
		*	@param sy коэффициент сдвига по оси Y.
		*	@param scaleCenter точка, относительно которой производится сдвиг
		*					(т.е., собственно, точка, лежащая на сдвигаемой грани).
		*/
		virtual void sheare( qreal sx, qreal sy, const QPointF &shearPoint ) = 0;

		/**
		*	Производит вращение фигуры.
		*
		*	@param angle угол, на который производится поворот.
		*	@param center точка, вокруг которой производится поворот.
		*/
		virtual void rotate( qreal angle, const QPointF &center ) = 0;


		/**
		*	Возвращает все точки, из которых состоит данная фигура.
		*
		*	@return точки, из которых состоит данная фигура.
		*/
		virtual QPolygonF points(int frame) = 0;

		/**
		*	Добавляет новую точку в фигуру.
		*
		*	@param p координаты добавляемой точки.
		*
		*	@return индекс новодобавленной точки.
		*/
		virtual int addPoint( const QPoint &p ) = 0;

		/**
		*	Добавляет новую точку в фигуру, не используя различный алгоритмы выбора позиции -
		*		т.е. точка всегда добавляется в конец фигуры.
		*
		*	@param p координаты добавляемой точки.
		*
		*	@return индекс новодобавленной точки.
		*/
		virtual int addPointToEnd( const QPoint &p ) = 0;

		/**
		*	Удаляет точку по её индексу из обьекта.
		*
		*	@param index индекс удаляемой точки.
		*
		*	@return true, если точка была удалена.
		*/
		virtual bool deletePoint( int index ) = 0;

		/**
		*	Перемещает одну из точек фигуры на новое место.
		*
		*	@param index индекс перемещаемой точки.
		*	@param newPosition новое положение точки.
		*/
		virtual void movePoint( int index, const QPointF &newPosition ) = 0;

		/**
		*	Клонирует один из кадров таким образом, что он замещает все остальные кадры.
		*
		*	@param frame клонируемый кадр.
		*/
		virtual void cloneFrameToAll( int frame ) = 0;


		/**
		*	Возвращает тип и параметры линий обьекта.
		*
		*	@return тип и параметры линий обьекта.
		*/
		virtual QPen pen() const = 0;

		/**
		*	Возвращает тип и параметры заливки обьекта.
		*
		*	@return тип и параметры заливки обьекта.
		*/
		virtual QBrush brush() const = 0;

		/**
		*	Устанавливает тип и параметры линий обьекта.
		*
		*	@param p тип и параметры линий обьекта.
		*/
		virtual void setPen( const QPen &p ) = 0;

		/**
		*	Устанавливает тип и параметры заливки обьекта.
		*
		*	@param p тип и параметры заливки обьекта.
		*/
		virtual void setBrush( const QBrush &b ) = 0;

		/**
		*	Проверяет фигуру на замкнутость.
		*
		*	@return true, если фигура замкнута.
		*/
		virtual bool isClosed() const = 0;

		/**
		*	Устанавливает замкнутость фигуры.
		*
		*	@param closed замкнутость фигуры.
		*/
		virtual void setClosed( bool closed ) = 0;

		/**
		*	Отрисовывает целиком весь обьект.
		*
		*	@param p то, куда будет производится отрисовка.
		*/
		virtual void paint( QPainter &p ) = 0;

		/**
		*	Сохраняет в поток любое дерево графических обьектов.
		*
		*	@param stream поток, в который происходит запись.
		*	@param o обьект, который сохраняется в поток. Он может быть любым графическим обьектом,
		*			даже контейнером. Даже контейнером, в котором находятся контейнеры. И т.д.
		*			Глубина вложенности неограничена.
		*/
		static void saveObject( QDataStream &stream, const GObjectInterface *o );

		/**
		*		Загружает и создаёт из потока ранее сохранённый графический обьект. (Или дерево
		*	обьектов.)
		*
		*	@param stream поток, из которого происходит чтение.
		*
		*	@return новосозданный обьект, который был до этого сохранён в том, откуда происходит
		*		теперь чтение. Если поток не содержит обьектов, которые можно загрузить,
		*		возвращается 0.
		*/
		static GObjectInterface *loadObject( QDataStream &stream );

		/**
		*		Создаёт новый графический обьект по информации об обьекте с нужным количеством кадров
		*	и в нужном месте.
		*
		*	@param info информация о фигуре, на основе которой создаётся обьект.
		*	@param countFrames то количество кадров, с которыми будет создан обьект.
		*	@param pos позиция, в которой будет создан обьект.
		*/
		static GObjectInterface *create( QObject* figureTool,
				 const QPointF &pos, int number, int positon);

		/**
		*		Создаёт новый графический обьект по информации об обьекте с нужным количеством кадров
		*	и в нужном месте. Если информация содержит описание нескольких обьектов, то они
		*	будут созданы и помещены в контейнер. И будет возвращен указатель на него.
		*
		*	@param info информация о фигуре, на основе которой создаётся обьект.
		*	@param countFrames то количество кадров, с которыми будет создан обьект.
		*	@param pos позиция, в которой будет создан обьект.
		*	@param name имя контейнера, если он будет создан.
		*/
		static GObjectInterface *create( QObject* figureTool,
				 const QPointF &pos,const QString &name, int position );
		/**
		*	Сохраняет весь обьект целиком в поток.
		*
		*	@param stream поток, в который сохраняется обьект.
		*/
		virtual void save( QDataStream &stream ) const = 0;

		/**
		*	Читает весь обьект целиком из потока.
		*
		*	@param stream поток, из которого происходит чтение.
		*/
		virtual void load( QDataStream &stream ) = 0;

		/**
		*	Конструктор.
		*/
		GObjectInterface(){};

		/**
		*	Деструктор
		*/
		virtual ~GObjectInterface() {};

};

Q_DECLARE_INTERFACE( GObjectInterface, "com.Tailer.MultiFusion.GObjectInterface/1.0" )

template < class T > inline GObjectInterface * GOBJECTINTEFACE( T *o )
{
	return qobject_cast< GObjectInterface* >( o );
}
#endif /* GOBJECTINTERFACE_H_ */
