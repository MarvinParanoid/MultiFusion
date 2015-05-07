#ifndef __GSELECTION_RECT_H__
#define __GSELECTION_RECT_H__

#include <QObject>
#include <QPainter>
#include <QPainterPath>
#include <QMessageBox>
#include "../pluginTool/Plugin.h"
#include "../interfaces/GSRInterface.h"
#include "../interfaces/GObjectInterface.h"
#include "GContainer.h"


/**
*	@file gselectionrect.h Данный файл содержит описание класса, реализующего рамку выделения
*		графических обьектов. Он так же позволяет призводить различные операции с выделенными
*		обьектами, такие как их перемещение, изменение из размера и прочее.
*/

/**
*		Этот класс реализует рамку выделения графических обьектов. Он так же позволяет призводить
*	различные операции с выделенными обьектами, такие как их перемещение, изменение из размера и
*	прочее.
*/
class GSelectionRect: public GSRInterface
{
	Q_OBJECT
	Q_INTERFACES(GSRInterface)

    signals:

		/**
		 * Отправляется после выполнения события, которое нам нужно сохранить в истории
		 * @param event имя события
		 */
		void StateChanged(QString event);

		/**
		*		Данный сигнал генерируется при изменении состояния рамки выделения. Например,
		*	если пользователь её растянул. Или дважды по ней кликнул, и рамка перешла в другое
		*	состояние (см. InputMode ). Ожидается, что в ответ на это событие рамку перересуют
		*	(вызовут у неё метод paint( QPainter& ), так как сама себя она перерисовать не может.
		*/
		void changed();

		/**
		*	Этот сигнал генерируется в случае, если пользователь перемести рамку.
		*
		*	@param dx смещение рамки относительно прошлой её позиции по оси X.
		*	@param dy смещение рамки относительно прошлой её позиции по оси Y.
		*/
		void moved( const qreal dx, const qreal dy );

		/**
		*	Данный сигнал генерируется на изменение пользователем размера рамки.
		*
		*	@param sx коэффициент, на который пользователь смаштабировал рамку по оси X.
		*	@param sy коэффициент, на который пользователь смаштабировал рамку по оси X.
		*	@param scaleCenter точка, от которой и происходит изменение размера рамки.
		*/
		void scaled( const qreal sx, const qreal sy, const QPointF &scaleCenter );

		/**
		*		Сигнал генерируется на паралельный перенос пользователем одной из сторон
		*	прямоугольника рамки.
		*
		*	@param sx коэффициент сдвига по оси X.
		*	@param sy коэффициент сдвига по оси Y.
		*	@param scaleCenter точка, от которой и происходит изменение рамки.
		*/
		void sheared( const qreal sx, const qreal sy, const QPointF &shearPoint );

		/**
		*	Сигнал генерируется на поворот рамки вокруг некоторой точки.
		*
		*	@param angle угол поворота рамки.
		*	@param center точка, вокруг которой поворот и совершился.
		*/
		void rotated( const qreal angle, const QPointF &center );

    public:


        virtual void emitChanged();
        virtual void emitStateChanged(QString event);

		/**
		*		Так, так данная рамка не является полноценным элементом управления и может существовать только
		*	как часть некоторого виджета, то, соответственно, она и на может сама реагировать на различные
		*	события. Посему, этот класс имеет несколько методов, которые необходимо вызывать при наступлении
		*	тех или иных событий, произошедших с тем виджетом, частью которого она является. Данный
		*	метод необходимо вызывать при двойном щелчке мышью по элементу управления.
		*
		*	@param button кнопка, которой пользователь щёлкнул по виджету. Определяется константами
		*			Qt::LeftButton, Qt::RightButton и т.д. (см. справку по Qt)
		*	@param position положение курсора мыши пи щелчке в системе координат виджета.
		*	@param modifiers модификаторы, которые при этом удерживались пользователем (константы
		*			Qt::ControlModifier, Qt::AltModifier и т.д. (см. справку по Qt))
		*/
		virtual void mouseDoubleClick( const int button, const QPoint &pos, Qt::KeyboardModifiers modifiers );

		/**
		*		Так, так данная рамка не является полноценным элементом управления и может существовать только
		*	как часть некоторого виджета, то, соответственно, она и на может сама реагировать на различные
		*	события. Посему, этот класс имеет несколько методов, которые необходимо вызывать при наступлении
		*	тех или иных событий, произошедших с тем виджетом, частью которого она является. Данный
		*	метод необходимо вызывать при щелчке мышью по элементу управления.
		*
		*	@param button кнопка, которой пользователь щёлкнул по виджету. Определяется константами
		*			Qt::LeftButton, Qt::RightButton и т.д. (см. справку по Qt)
		*	@param position положение курсора мыши пи щелчке в системе координат виджета.
		*	@param modifiers модификаторы, которые при этом удерживались пользователем (константы
		*			Qt::ControlModifier, Qt::AltModifier и т.д. (см. справку по Qt))
		*
		*	@return true, если событие было обработано рамкой выделения и false, если щелчёк пришёлся мимо.
		*/
		virtual bool mousePress( const int button, const QPoint &pos, Qt::KeyboardModifiers modifiers );

		/**
		*		Так, так данная рамка не является полноценным элементом управления и может существовать только
		*	как часть некоторого виджета, то, соответственно, она и на может сама реагировать на различные
		*	события. Посему, этот класс имеет несколько методов, которые необходимо вызывать при наступлении
		*	тех или иных событий, произошедших с тем виджетом, частью которого она является. Данный
		*	метод необходимо вызывать при перемещении мыши над элементом управления.
		*
		*	@param button кнопка, которой пользователь щёлкнул по виджету. Определяется константами
		*			Qt::LeftButton, Qt::RightButton и т.д. (см. справку по Qt)
		*	@param position положение курсора мыши пи щелчке в системе координат виджета.
		*	@param modifiers модификаторы, которые при этом удерживались пользователем (константы
		*			Qt::ControlModifier, Qt::AltModifier и т.д. (см. справку по Qt))
		*/
		virtual void mouseMove( const int button, const QPoint &pos, Qt::KeyboardModifiers modifiers );

		/**
		*		Так, так данная рамка не является полноценным элементом управления и может существовать только
		*	как часть некоторого виджета, то, соответственно, она и на может сама реагировать на различные
		*	события. Посему, этот класс имеет несколько методов, которые необходимо вызывать при наступлении
		*	тех или иных событий, произошедших с тем виджетом, частью которого она является. Данный
		*	метод необходимо вызывать в момент, когда пользователь отпускил клавишу мыши, в случае,
		*	если она была нажата над элементом управления.
		*
		*	@param button кнопка, которой пользователь щёлкнул по виджету. Определяется константами
		*					Qt::LeftButton, Qt::RightButton и т.д. (см. справку по Qt)
		*	@param position положение курсора мыши пи щелчке в системе координат виджета.
		*	@param modifiers модификаторы, которые при этом удерживались пользователем (константы
		*					Qt::ControlModifier, Qt::AltModifier и т.д. (см. справку по Qt))
		*/
		virtual void mouseRelease( const int button, const QPoint &pos, Qt::KeyboardModifiers modifiers );

		/**
		*		Этот метод необходимо вызывать при отрисовке виджета, в котором должна находится данная рамка.
		*	Он её, эту рамку, как раз и отрисовывает.
		*
		*	@param p обьект класса QPainter, которым данная рамка и будет отрисована.
		*
		*	@warning данный метод может изменять состояние обьекта p.
		*/
		virtual void paint( QPainter &p );

		/**
		*		Если перед созданием некоторой фигуры зажать клавишу Ctrl и, не отпуская её,
		*	протянуть новую фигуру, то она будет принимать исключительно квадратную форму
		*	(это удобно при создании правильных окружностей).
		*	Так вот, в этот момент создания фигуры до момента отпускания клавиши Ctrl этот
		*	метод будет возвращать true - т.е. рамка находится в режиме создания квадратной
		*	фигуры.
		*
		*	@return если в момент создания фигуры была зажата клавиша Ctrl,
		*	то до момента её отпускания этот метод будет возвращать true.
		*
		*	@todo Провести рефакторинг и переименовать метод во что-нибудь более понятное.
		*/
		virtual bool isInCreateFigureMode() const;

		/**
		*		Если перед созданием некоторой фигуры зажать клавишу Ctrl и, не отпуская её,
		*	протянуть новую фигуру, то она будет принимать исключительно квадратную форму
		*	(это удобно при создании правильных окружностей).
		*	Так вот, что бы это происходило именно так, перед созданием фигуры необходимо вызвать
		*	данный метод со значением параметра true. Если этого не сделать, то на клавишу Ctrl
		*	рамка реагировать никак не будет.
		*
		*	@param enabled вот этот параметр и нужно устанавливать в true перед созданием какой-либо
		*				фигуры и добавлением её в данную рамку.
		*
		*	@todo Провести рефакторинг и переименовать метод во что-нибудь более понятное.
		*/
		virtual void setCreateFigureMode( bool enabled );

		/**
		*	Возвращает текущую позицию рамки в системе координат родительского виджета.
		*
		*	@return текущая позиция рамки в системе координат родительского виджета.
		*/
		virtual QRect getPosition() const;

		virtual void setPosition( const QRect &p );

		/**
		*	Говорит о том что создается новая фигура
		*/
		virtual void setIsNewFigure(bool inf);

		virtual bool getIsNewFigure();

		/**
		*	Возвращает координаты прямоугольной области, в которой может находится рамка выделения.
		*
		*	@return координаты прямоугольной области, в которой может находится рамка выделения.
		*/
		virtual QRect getViewport() const;

		/**
		*		С помощью данного метода можно установить прямоугольную область в которой и только в
		*	которой может находится данная рамка выделения.
		*
		*	@param v координаты прямоугольной области.
		*/
		virtual void setViewport( const QRect &v );

		/**
		*	Возвращает состояние, в котором на данный момет находится рамка выделения. (см. InputMode)
		*
		*	@return сотояние, в котором на данный момент находится рамка выделения.
		*/
		virtual GSRInterface::InputMode getInputMode() const;

		virtual void setInputMode( const GSRInterface::InputMode m );

		/**
		*	Возвращает true, если в данный момент рамка должна быть видна на экране.
		*
		*	@return true, если рамка должна быть видна на экране и false в противоположном случае.
		*/
		virtual bool isVisible() const;

		/**
		*	Управлят видимостью рамки на экране.
		*
		*	@param v определяет видимость рамки на экране.
		*/
		virtual void setVisible( bool v );

		/**
		*	Возвращает тип и параметры линий первого из выделенных обьектов.
		*
		*	@return тип и параметры линий первого из выделенных обьектов.
		*/
		virtual QPen pen();

		/**
		*	Возвращает тип и параметры заливки первого из выделенных обьектов.
		*
		*	@return тип и параметры заливки первого из выделенных обьектов.
		*/
		virtual QBrush brush();

		/**
		*	Устанавливает тип и параметры линий всем выделенным обьктам сразу.
		*
		*	@param p тип и параметры линий, которые будут установленны всем выделенным объектам.
		*/
		virtual void setPen( const QPen &p );

		/**
		*	Устанавливает тип и параметры заливки всем выделенным обьктам сразу.
		*
		*	@param p тип и параметры заливки, которые будут установленны всем выделенным объектам.
		*/
		virtual void setBrush( const QBrush &b );

		/**
		*	Проверяет на замкнутость первую из выделенных фигур.
		*
		*	@return true, если первая из выделенных фигур замкнута.
		*/
		virtual bool isClosed() const;

		/**
		*	Устанавливает замкнутость для всех выделенных фигур.
		*
		*	@param closed должен быть установленн в true, если вы хотите замкнуть все выделеные
		*			фигуры, и false, если разомкнуть.
		*/
		virtual void setClosed( bool closed );
		/**
		*	Устанавливает отрисовку выделения.
		*
		*	@param closed должен быть установленн в true, если вы хотите отрисовать выделение
		*		, и false, если нет.
		*/

		virtual int countSelected() const;

		/**
		*	Проверяет находится ли некоторый обьект выделенным с помощью данной рамки выделения.
		*
		*	@return true, если обьект выделен и false в противном случае.
		*/

		virtual bool isInside( GObjectInterface *o );

		/**
		*	Возвращает указатель на один из выбранных обьектов по его индексу.
		*
		*	@param index порядковый номер (индекс) выделенного в данный момент обьекта.
		*			Обьекты нумеруются в порядке их выделения.
		*
		*	@return указатель на запрошенный обьект, или 0, если обьекта с таким номером нет.
		*/
		virtual GObjectInterface *selected( int index );

		/**
		*		Устанавливает выделенным один некоторый обьект. Если до этого были выделенны другие
		*	обьекты, они исключаются из выделения.
		*
		*	@param o обьект, который необходимо выделить рамкой выделения.
		*/
        virtual void setSelected( GObjectInterface *o );

        /**
        *	Получаем выделенные объекты
        *
        *	@param o обьект, который выделен рамкой выделения.
        */
        virtual QObject * getSelected();
        virtual GContainer * getSelectedAsGContainer();

		/**
		*		Добавляет к выделению ещё один обьект. Если не было выделенно ни одного, то данный
		*	обьект становится елинственным выделенным.
		*
		*	@param o указатель на обьект, который необходимо добавить вы выделение.
		*/
		virtual void addSelected( GObjectInterface *o );

		/**
		*	Исключает из выделения все ранее добавленные в него обьекты.
		*/
		virtual void reset();

		/**
		*		Данная рамка выделения, кроме как перемещения обьектов, их поворота и прочего,
		*	может так же добавлять дополнительные точки в выделенный обьект, если он выделен один.
		*	Данная функция проверяет, находится ли при этом рамка в режиме добавления точек в обьект.
		*
		*	@return true, если рамка находится в режиме добавления новых точек в выделенный обьект.
		*/
		virtual bool isInAddPointMode() const;

		/**
		*		Данная рамка выделения, кроме как перемещения обьектов, их поворота и прочего,
		*	может так же добавлять дополнительные точки в выделенный обьект, если он выделен один.
		*	Данная функция устанавливает или сбрасывает этот режим.
		*
		*	@param enabled в зависимости от значения либо включает (true), либо выключает
		*		режим добавления новых точек в обьект.
		*/
		virtual void enableAddPointMode( bool enabled );

		/**virtual MarksPositions getMovedMark();

		virtual void setMovedMark(MarksPositions mm);

		virtual QPoint getLastHitPoint();

		virtual void setLastHitPoint(QPoint lhp);

		virtual QPointF getСenterPoint();

		virtual void setСenterPoint(QPointF cp);*/

		virtual QPoint getShearPoint();

		virtual void setShearPoint(QPoint sp);

		virtual QPointF getShearOffsets();

		virtual void setShearOffsets(QPointF so);

        virtual qreal getRotateAngle();

        virtual void setScale(qreal dx, qreal dy);

        virtual void setRotateAngle(qreal ra);

		virtual int getSelectedObjectPoint();

		virtual void setSelectedObjectPoint(int sop);

		virtual QWidget * getParent();

		virtual void setParent(QWidget * p);

		QVector< QRect > marksRects(); // задание положения и размеров (области рисования для каждого маркера) маркеров выделения

				/**
                *	Конструктор.
				*
				*	@param parent указатель на элемент управления, в котором будет отображаться данная
				*			рамка выделения.
				*	@param viewport прямоугольная область элемента управления, в которой может
				*			находится данная рамка.
				*	@param pos начальная позиция рамки.
				*
				*	@warning элемент управления, в котором отображается данная рамка,
				*		должен оставаться "живым" всё время, пока "жива" рамка выделения.
				*/
				GSelectionRect( plugin::PluginsManager *manager, QWidget *parent, const QRect viewport,
						const QRect &pos = QRect( 10, 10, 120, 100 ) );

		/**
		*	Деструктор.
		*
		*	@warning при удалении рамки, все обьекты просто исключаются из выделения, но не удаляются.
		*/
		virtual ~GSelectionRect();

	public slots:
		void onMoved( const qreal dx, const qreal dy );
		void onScaled( const qreal sx, const qreal sy, const QPointF &scaleCenter );
		void onSheared( const qreal sx, const qreal sy, const QPointF &shearPoint );
        void onRotated( const qreal angle, const QPointF &center );

	private:

		QRect position;
		QRect maxRect;
		QPoint lastHitPoint;
		GSRInterface::MarksPositions movedMark;
		QPointF centerPoint;
		QPoint shearPoint;
		QPointF shearOffsets;
		qreal rotateAngle;
		GSRInterface::InputMode _inputMode;
		bool visible;
		int selectedObjectPoint;
		bool addPointMode;
		QWidget *_parent;
		bool createFigureMode;
		bool isNewFigure;
		GContainer *selectedObjects;
		GContainer *selectedLayer;
		bool isMove;
		bool isShear;
		bool isRotate;
		bool isScale;
		bool isPoint;
		/** радиус маркеров выделения */
		const static int marksRadius;
		/** смещение маркеров выделения */
		const static int displacement;




		GSRInterface::MarksPositions selectedMark( const QPoint &p );
		void moveMark( const  GSRInterface::MarksPositions mark, QPoint p, Qt::KeyboardModifiers modifiers );

		void recalcCenter( const QRect &oldPosition );


		void normalize();

        static void drawShiftArrow( QPainter &p, const QPoint &pos, const ShiftArrowDirection dir );
		static void drawRotateArrow( QPainter &p, const QPoint &pos );
		static qreal calcSheareAngle( qreal height, qreal width );
		static qreal calcRotateAngle( const QPointF &center, const QPoint &beginPoint, const QPoint &endPoint );
};

#endif /* __GSELECTION_RECT_H__ */
