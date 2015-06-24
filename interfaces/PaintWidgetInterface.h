/**
* PaintWidgetInterface.h
*
*  Created on: 23.04.2009
*      Author: Alexey Kukushkin
*/

#ifndef PAINTWIDGETINTERFACE_H_
#define PAINTWIDGETINTERFACE_H_

#include <QApplication>
#include <QMenu>
#include <QObject>
#include <QScrollArea>

class PaintWidgetInterface:public QScrollArea
{
	signals:

		void zoomEvent(qreal scale);
		void paintEvent(QPoint origin);
        void mouseMoveEvent(QPoint origin, QPoint global, qreal scale);
	
		 /**
		 * Отправляется после выполнения события, которое нам нужно сохранить в истории
		 * @param event имя события
		 */
		void StateChanged(QString event);

		/**
		 * Генерируется при перемещении по истории событий
		 */
		void undoEvents();

		/**
		* Генериреутся при перемещении слоя или фигуры
		*/
		void movedObject();

		/**
		*	Генериреутся в момент создания нового обьета.
		*/
		void objectCreated();

		/**
		*	Генерируется каждый раз, когда сменяется текущий показываемый кадр.
		*
		*	@param frame новый текущий кадр.
		*/
		void frameChanged( qreal frame );

		/**
		*	Генерируется каждый раз, когда изменяется общее количество кадров в анимации.
		*/
		void countFramesChanged( int count );

		/**
		*	Генерируется каждый раз при измеении выделения.
		*
		*	@param layer номер выбранного слоя, или -1, если ни один слой не выбран.
		*/
		void layerSelected( int layer );

		/**
		*		Вызывается в тот момент, когда произошли глобальные изменения слоёв
		*	(например, они были все удалены).
		*/
		void allLayersChanged();

		/**
		*		Сигнал для внутреннего использования, вызывается в тот момент, когда изменяется
		*	настройка заднего фона для данного виджета.
		*/
		void backgroundChanged( QWidget *from );

		void figureSelected(int layer , int obj);

	public:
	
		virtual void scrollContentsBy(int dx, int dy) = 0;
		virtual void mouseMoveEvent( QMouseEvent * event ) = 0;
		virtual void paintEvent( QPaintEvent * event) = 0;
		virtual void mySetViewportMargins(int left, int top, int right, int bottom)=0;
	
		/**
		 * Задает скрывать фигуры в текущем слое или нет
		 */
		virtual void setHideFigures(int layer, bool value)=0;

		/**
		 * Проверяет скрывать фигуры в текущем слое или нет
		 */
		virtual bool getHideFigures(int layer)=0;

		/**
		 * Получает через какое количество линий нужно скрыть линию
		 * @return количество линий
		 */
		virtual int getFigureHideLines() = 0;

		/**
		 * Задаёт через какое количество линий нужно скрыть линию
		 * @param value количество линий
		 */
		virtual void setFigureHideLines(int value) = 0;

		/**
		 * Проверяет является ли фигура в сплайновой
		 * @return 1 если да, 0 если нет -1 если это контейнер
		 */
		virtual int FigureIsSpline() = 0;

		/**
		 * Переводит фигуру в сплайновый режим или нормальный в зависимоти от spline
		 */
		virtual void FigureSetStyle(bool spline) = 0;

		/**
		 * Задаёт отбражать для фигуры линии Бизье или нет
		 * @param value true если отображать
		 */
		virtual void FigureSetShowBezier(bool value) = 0;

		/**
		 * Задаёт отбражать для фигуры линии Бизье или нет
		 * @return 1 если да, 0 если нет -1 если это контейнер
		 */
		virtual int FigureIsShowBezier() = 0;

		/**
		 * Возвращает максимальный кадр
		 */
		virtual int maxFrame() = 0;

		/**
		 * Проверяет есть ли в текущей позиции таймлайна кадр для данного слоя
		 * @param layer индекс слоя
		 */
		virtual bool isContainsFrame(int layer) = 0;

		/**
		 * 	Возвращпет позицию кадра на таймлайне
		 * @param i позиция в списке
		 * @return позиция на таймлайне
		 */
		virtual int getPositionFrame(int i) = 0;

		/**
		*	Определяет 3 типа представления создаваемого/показываемого ролика.
		*/
		enum ViewportType
		{
			fixedViewport,		/** ролик имеет фиксированную высоту и ширину. Выход за границы не разрешается. */
			resizableViewport, 	/** ролик не имеет фиксированных размеров и зависит от высоты/ширины окна просмотра.. */
			hintViewport		/** ролик имеет фиксированную высоту и ширину. Выход за границы разрешается. */
		};


		/**
		*	Устанваливает текущий кадр анимации.
		*
		*	@param frame устанавливаемый кадр анимации.
		*/
		virtual void setFrame( qreal frame, bool played ) = 0;

		/**
		*	Возвращает текущий кадр анимации.
		*
		*	@return текущий кадр анимации.
		*/
		virtual int frame() const = 0;

		/**
		*	Возвращает текущий слой.
		*
		*	@return текущий слой.
		*/
		virtual int layer() const = 0;

		/**
		*	Возвращает общее количество кадров, содержащихся в ролике.
		*/
		virtual int countFrames() const = 0;

		/**
		*	Масштабирует текущий кадр.
		*
		*	@param s коэффициент масштабирования.
		*
		*	@warning масштабируется только текуший кадр.
		*/
		virtual void scale( qreal s ) = 0;

		/**
		*	Проверяет на возможность группировки выделенных обьектов.
		*
		*	@return true, если группировка возможна.
		*/
		virtual bool canGroup() = 0;

		/**
		*	Проверяет на возможность разгруппировки выделенных обьектов.
		*
		*	@return true, если разгруппировка возможна.
		*/
		virtual bool canUngroup() = 0;

		/**
		*	Группирует выделенные обьекты, если это возможно.
		*/
		virtual void group() = 0;

		/**
		*	Разгруппировывает выделенные обьекты, если это возможно.
		*/
		virtual void ungroup() = 0;

		/**
		*	Возвращает тип и параметры линий выделенного обьекта.
		*
		*	@return тип и параметры линий выделенного обьекта.
		*/
		virtual QPen pen() = 0;

		/**
		*	Возвращает тип и параметры заливки выделенного обьекта.
		*
		*	@return тип и параметры заливки выделенного обьекта.
		*/
		virtual QBrush brush() = 0;

		/**
		*	Устанавливает тип и параметры линий выделенного обьекта.
		*
		*	@param p тип и параметры линий выделенного обьекта.
		*/
		virtual void setPen( const QPen &p ) = 0;

		/**
		*	Устанавливает тип и параметры заливки выделенного обьекта.
		*
		*	@param p тип и параметры заливки выделенного обьекта.
		*/
		virtual void setBrush( const QBrush &b ) = 0;

		/**
		*	Проверяет выделенную фигуру на замкнутость.
		*
		*	@return true, если фигура замкнута.
		*/
		virtual bool isClosed() const = 0;

		/**
		*	Устанавливает замкнутость выделенной фигуры.
		*
		*	@param closed замкнутость фигуры.
		*/
		virtual void setClosed( bool closed ) = 0;

		/**
		 * Прверяет является ли объект контейнером
		 * @param layer слой в котором находится объект
		 * @param object объект
		 */
		virtual bool objectIsContainer(int layer, int object) = 0;

		/**
		*	Возвращает текущее количество объектов в слое layer.
		*/
		virtual int countFigures(int layer) const = 0;

		/**
		*	Возвращает имя объекта.
		*
		*	@param layer слой, который содержит объект.
		*
		*	@param object объект.
		*/
		virtual QString figureName( int layer, int object  ) const = 0;

		/**
		 * Меняет имя фигуры в слое
		 * @param layer - номер слоя
		 * @param obj - номер фигуры
		 * @param name имя
		 */
		virtual void renameFigure( int layer, int obj,  const QString &name ) = 0;

		/**
		*	Проверяет некоторый объект на блокированность.
		*
		*	@param layer слой, который содержит объект.
		*
		*	@param object объект, проверяемый на блокированность.
		*/
		virtual bool isFigureBlocked(  int layer, int object  ) const = 0;

		/**
		 * Меняет блокированность фигуры в слое
		 * @param layer - номер слоя
		 * @param obj - номер фигуры
		 * @param visible значение блокировки
		 */
		virtual void setFigureBlocked( int layer, int obj, bool blocked ) = 0;

		/**
		*	Проверяет некоторый объект на видимость на экране.
		*
		*	@param layer слой, который содержит объект.
		*
		*	@param object объект, проверяемый на видимость.
		*/
		virtual bool isFigureVisible(  int layer, int object  ) const = 0;

		/**
		 * Меняет видимость фигуры в слое
		 * @param layer - номер слоя
		 * @param obj - номер фигуры
		 * @param visible значение видимости
		 */
		virtual void setFigureVisible( int layer, int obj, bool visible ) = 0;

		/**
		 * Проверяет является ли фигура трансформируемой
		 * @param layer слой в котором находится фигура
		 * @param object номер фигуры
		 * @return true если трансформируема
		 */
		virtual bool isFigureTransformed(  int layer, int object  ) const = 0;

		/**
		 * Определяет трансформируемость фигуры
		 * @param layer слой в котором находится фигура
		 * @param object номер фигуры
		 * @param trns значение трансформируемости
		 */
		virtual void setFigureTransformed( int layer, int obj, bool trns ) = 0;

		/**
		*	Возвращает номер выбранного слоя. Или -1, если их несколько или ни одного.
		*
		*	@return номер выбранного слоя. Или -1, если их несколько или ни одного.
		*/
		virtual int selectedFigure() = 0;

		/**
		*	Устанавливает выделение на некоторый слой и некоторую фигуру
		*
		*	@param layer выделяемый слой.
		*
		*	@param obj выделяемая фигурв.
		*/
		virtual void selectFigure( int layer, int obj) = 0;

		/**
		 * Удаляет фигуру из слоя
		 * @param layer - номер слоя
		 * @param obj - номер фигуры
		 */
		virtual void deleteFigure(int layer, int obj ) = 0;

		                /**
                 * Добавляет фигуру в текущий слой, создавая ее из переданных параметров
                 * @param poly - точки: из которых строится фигура
                 * @param pen - перо
                 * @param brush - кисть
                 * @param name - название фигуры
                 * @param select - выбирать ли фигуру после добавления
                 */
                virtual void addFigure(const QPolygonF &poly, QPen pen, QBrush brush, QString name, bool select) = 0;
		
		/**
		 * Добавляет новый слой
		 */
		virtual void addLayer(bool visible, bool blocked, const QString &name) = 0;

		/**
		*	Возвращает текущее количество слоёв.
		*
		*	@return текущее количество слоёв.
		*/
		virtual int countLayers() const = 0;

		/**
		 * Задает текущий слой.
		 */
		virtual void setLayer(int layer) = 0;

		/**
		*	Возвращает имя некоторого слоя.
		*
		*	@param layer слой, имя которого запрашивается.
		*
		*	@return имя запрашиваемого слоя.
		*/
		virtual QString layerName( int layer ) = 0;

		/**
		*	Переименовывает некоторый слой.
		*
		*	@param layer слой.
		*	@param name новое имя слоя.
		*/
		virtual void renameLayer( int layer, const QString &name ) = 0;

		/**
		*	Проверяет некоторый слой на блокированность на экране.
		*
		*	@param layer слой, проверяемый на блокированность.
		*
		*	@return true, если слой блокирован.
		*/
		virtual bool isLayerBlocked( int layer ) const = 0;

		/**
		*	Устанавливает блокированность некоторого слоя в данном кадре.
		*
		*	@param layer слой.
		*	@param blocked блокированность слоя в данном кадре.
		*/
		virtual void setLayerBlocked( int layer, bool blocked ) = 0;

		/**
		*	Проверяет некоторый слой на видимость на экране.
		*
		*	@param layer слой, проверяемый на видимость.
		*
		*	@return true, если слой видим.
		*/
		virtual bool isLayerVisible( int layer ) const = 0;

		/**
		*	Устанавливает видимость некоторого слоя в данном кадре.
		*
		*	@param layer слой.
		*	@param visible видимость слоя в данном кадре.
		*/
		virtual void setLayerVisible( int layer, bool visible ) = 0;

		/**
		*	Возвращает номер выбранного слоя. Или -1, если ни одного.
		*
		*	@return номер выбранного слоя. Или -1, если ни одного.
		*/
		virtual int selectedLayer() = 0;

		/**
		*	Удаляет кадр по позиции.
		*/
		virtual void deleteFrame(int position) = 0;

		/**
		 * Добавляет кадр в определённую позицию
		 * @param position
		 */
		virtual void addFrame(int position) = 0;

		/**
		*		Устанавливает инструмент, отвечающий за выбор обьектов и за их модификацию
		*	(перемещение точек), как текущий.
		*/
		virtual void setSelectionTool() = 0;

		/**
		*		Устанавливает инструмент, отвечающий за добавление точек в
		*	выбранную фигуру, как текущий.
		*/
		virtual void setAddPointTool() = 0;

		/**
		*	Устанавливает новый инструмент, как текущий.
		*
		*	@param tool обьект, реализующий интерфейс инструмента.
		*/
		virtual void setTool( QObject *tool ) = 0;


		/**
		*	Прорисовывает некоторый кадр текущего слоя.
		*
		*	@param to это то, куда будет отрисован запрошенный кадр.
		*	@param r прямоугольник, в который будет смасштабирован кадр.
		*	@param frame кадр, который будет отрисован.
		*/
		virtual void paintFrameTo( QPainter &to, const QRect &r, qreal frame ) = 0;

		/**
		*	Прорисовывает некоторый кадр.
		*
		*	@param to это то, куда будет отрисован запрошенный кадр.
		*	@param r прямоугольник, в который будет смасштабирован кадр.
		*	@param frame кадр, который будет отрисован.
		*/
		//virtual void paintWholeFrameTo( QPainter &to, const QRect &r, qreal frame ) = 0;

		/**
		*		Производит сброс нарисованного мультика.
		*	Удаляются все обьекты, из которых состоит анимация.
		*/
		virtual bool reset() = 0;

		/**
		*	Сохраняет состояние виджета в поток.
		*
		*	@param stream поток, в который производится запись.
		*/
		virtual void save( QDataStream &stream ) = 0;

		/**
		*	Загружает состояние виджета из потока.
		*
		*	@param stream поток, из которого читается состояие.
		*/
		virtual bool load( QDataStream &stream ) = 0;

		/**
		*	Проверяет возможность удаления выбранных обьектов.
		*
		*	@return true, если это возможно.
		*/
		virtual bool canDeleteSelected() const = 0;

		/**
		*	Удаляет выбранные обьекты, если это возможно.
		*/
		virtual void deleteSelected() = 0;

		/**
		*	Проверяет возможность скопировать или вырезать в буффер обмена выбранные обьекты.
		*
		*	@return true, если это возможно.
		*/
		virtual bool canCopyOrCutToClipboard() const = 0;

		/**
		*	Проверяет возможность вставить обьект из буффера обмена.
		*
		*	@return true, если это возможно.
		*/
		virtual bool canPasteFromClipboard() const = 0;

		/**
		*	Вырезает в буффер обмена выделенные обьекты, если это возможно.
		*/
		virtual void cutToClipboard() = 0;

		/**
		*	Копирует в буффер обмена выделенные обьекты, если это возможно.
		*/
		virtual void copyToClipboard() = 0;

		/**
		*	Вставляет из буффера обмена обьекты, если это возможно.
		*/
		virtual void pasteFromClipboard() = 0;

		/**
		*	Задаёт контектстное меню для области рисования.
		*	@*qMenu - указательна контектстное меню
		*/
        virtual void setContextMenu(QMenu *qMenu) = 0;

		/**
		*	Получает контектстное меню для области рисования.
		*	@return - указатель на контектстное меню
		*/
        virtual QMenu* getContextMenu() = 0;

		/**
		*	Вызывает диалог настроек данного виджета. Модально.
		*/
		virtual void showConfig() = 0;

		/**
		*	Возвращает тип представления создаваемого/показываемого ролика.
		*
		*	@return тип представления создаваемого/показываемого ролика.
		*/
		virtual ViewportType viewportType()const = 0;

		virtual void setViewportType( const ViewportType t )= 0;

		virtual QObject* getRealPaintWidget() = 0;

		///virtual void setRealPaintWidget(QObject* rpw) = 0;

		/**virtual void setViewportColor( const QColor &c ) = 0;

		virtual QColor& getViewportColor() = 0;

		virtual QSize viewportFixedSize() const = 0;

		virtual void setViewportFixedSize( const QSize &s ) = 0;*/

		virtual void doViewportTransparent() = 0;

		virtual void updateAllViews( QWidget *from ) = 0;

		/**
		*	Конструктор.
		*
		*	@param parent родительский для данного виджета виджет.
		*		Может принимать значение 0.
		*
		*	@param manager менеджер плагинов. Необходим для создания
		*		Background и BrushEditor для этого самого Background.
		*/
		PaintWidgetInterface(QWidget *parent):QScrollArea(parent)
		{

		}

		/**
		*	Деструктор.
		*/
		virtual ~PaintWidgetInterface(){};

	public slots:
		/**
		 *  Перемещает фигуру между слоями и внутри слоя по уровням
		 */
		void moveFigure( int layerOld, int objOld, int layerNew, int objNew );
		/**
		 *  Перемещает слой по уровням
		 */
		void moveLayer(int layerOld, int layerNew);
};


Q_DECLARE_INTERFACE( PaintWidgetInterface, "com.alxn1.sacs2.PaintWidgetInterface/1.0" )

template < class T > inline PaintWidgetInterface * PAINTWIDGETINTERFACE( T *o )
{
	return qobject_cast< PaintWidgetInterface* >( o );
}

#endif
