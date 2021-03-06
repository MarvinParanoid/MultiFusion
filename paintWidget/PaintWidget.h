﻿
/**
*	@mainpage Документация по GCore
*
*		В данной справке вы найдёте документацию по GCore - библиотеке, позволяющей создавать
*	анимационнае ролики, редактировать их и, так же, просматривать.
*/

#ifndef PAINT_WIDGET_H
#define PAINT_WIDGET_H

#include "PWE.h"
#include "RPW.h"
#include "./../interfaces/PaintWidgetInterface.h"
#include "./../interfaces/GObjectInterface.h"
#include <QDialog>
#include <QDialogButtonBox>
#include <QMouseEvent>

/**
*	@file paintwidget.h Данный файл содержит описание класса, предоставляющего, собственно, элемент
*				управления, позволяющий редактировать анимацию и/или проигрывать её.
*/

/**
*		Класс, реализующий элемент управления,
*	позволяющий редактировать анимацию и/или проигрывать её.
*
*	@warning Данный виджет в настоящее время зависит от некоторых плагинов. Это:<br>
*			Background<br>
*			BrushEditor<br>
*		Причём, BrushEditor, на самом деле, необходим плагину Background. В будующих версиях,
*	вероятно, данная зависимость будет устранена.
*/
class PaintWidget:public PaintWidgetInterface
{
	Q_OBJECT
	Q_INTERFACES(PaintWidgetInterface)
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
		void layerSelected( );

		/**
		*		Вызывается в тот момент, когда произошли глобальные изменения слоёв
		*	(например, они были все удалены).
		*/
		void allLayersChanged();

		/**
		 * Вызывается, когда в позиции таймлайна нет кадра
		 */
		void isFrame(bool value);

		void figureSelected(int layer , int obj);

		/**
		*		Сигнал для внутреннего использования, вызывается в тот момент, когда изменяется
		*	настройка заднего фона для данного виджета.
		*/
		void backgroundChanged( QWidget *from );

	public:

        virtual QPoint getWidgetOriginPoint();
        virtual void scrollContentsBy(int dx, int dy);
        virtual void mouseMoveEvent( QMouseEvent * event );
        virtual void paintEvent( QPaintEvent * event);
        virtual void mySetViewportMargins(int left, int top, int right, int bottom);


		/**
		 * Задает скрывать фигуры в текущем слое или нет
		 */
		virtual void setHideFigures(int layer, bool value);

		/**
		 * Проверяет скрывать фигуры в текущем слое или нет
		 */
		virtual bool getHideFigures(int layer);

		/**
		 * Получает через какое количество линий нужно скрыть линию
		 * @return количество линий
		 */
		virtual int getFigureHideLines();

		/**
		 * Задаёт через какое количество линий нужно скрыть линию
		 * @param value количество линий
		 */
		virtual void setFigureHideLines(int value);

		/**
		 * Проверяет является ли фигура в сплайновой
		 * @return 1 если да, 0 если нет -1 если это контейнер
		 */
		virtual int FigureIsSpline();

		/**
		 * Переводит фигуру в сплайновый режим или нормальный в зависимоти от spline
		 */
		virtual void FigureSetStyle(bool spline);

		/**
		 * Задаёт отбражать для фигуры линии Бизье или нет
		 * @param value true если отображать
		 */
		virtual void FigureSetShowBezier(bool value);

		/**
		 * Задаёт отбражать для фигуры линии Бизье или нет
		 * @return 1 если да, 0 если нет -1 если это контейнер
		 */
		virtual int FigureIsShowBezier();

		/**
		 * Возвращает максимальный кадр
		 */
		virtual int maxFrame();

		/**
		 * Проверяет есть ли в текущей позиции таймлайна кадр для данного слоя
		 * @param layer индекс слоя
		 */
		virtual bool isContainsFrame(int layer);

		/**
		 * 	Возвращпет позицию кадра на таймлайне
		 * @param i позиция в списке
		 * @return позиция на таймлайне
		 */
		virtual int getPositionFrame(int i);

		/**
		*	Устанваливает текущий кадр анимации.
		*
		*	@param frame устанавливаемый кадр анимации.
		*/
		virtual void setFrame( qreal frame, bool played );

		/**
		*	Возвращает текущий кадр анимации.
		*
		*	@return текущий кадр анимации.
		*/
		virtual int frame() const;

		/**
		*	Возвращает текущий слой.
		*
		*	@return текущий слой.
		*/
		virtual int layer() const;

		/**
		*	Возвращает общее количество кадров, содержащихся в ролике.
		*/
		virtual int countFrames() const;

		/**
		*	Масштабирует текущий кадр.
		*
		*	@param s коэффициент масштабирования.
		*
		*	@warning масштабируется только текуший кадр.
		*/
		virtual void scale( qreal s );

		/**
		*	Проверяет на возможность группировки выделенных обьектов.
		*
		*	@return true, если группировка возможна.
		*/
		virtual bool canGroup();

		/**
		*	Проверяет на возможность разгруппировки выделенных обьектов.
		*
		*	@return true, если разгруппировка возможна.
		*/
		virtual bool canUngroup();

		/**
		*	Группирует выделенные обьекты, если это возможно.
		*/
		virtual void group();

		/**
		*	Разгруппировывает выделенные обьекты, если это возможно.
		*/
		virtual void ungroup();

		/**
		 * Прверяет является ли объект контейнером
		 * @param layer слой в котором находится объект
		 * @param object объект
		 */
		virtual bool objectIsContainer(int layer, int object);

		/**
		*	Возвращает тип и параметры линий выделенного обьекта.
		*
		*	@return тип и параметры линий выделенного обьекта.
		*/
		virtual QPen pen();

		/**
		*	Возвращает тип и параметры заливки выделенного обьекта.
		*
		*	@return тип и параметры заливки выделенного обьекта.
		*/
		virtual QBrush brush();

		/**
		*	Устанавливает тип и параметры линий выделенного обьекта.
		*
		*	@param p тип и параметры линий выделенного обьекта.
		*/
		virtual void setPen( const QPen &p );

		/**
		*	Устанавливает тип и параметры заливки выделенного обьекта.
		*
		*	@param p тип и параметры заливки выделенного обьекта.
		*/
		virtual void setBrush( const QBrush &b );

		/**
		*	Проверяет выделенную фигуру на замкнутость.
		*
		*	@return true, если фигура замкнута.
		*/
		virtual bool isClosed() const;

		/**
		*	Устанавливает замкнутость выделенной фигуры.
		*
		*	@param closed замкнутость фигуры.
		*/
		virtual void setClosed( bool closed );

		virtual int countFigures(int layer) const;

		/**
		*	Проверяет некоторый объект на блокированность.
		*
		*	@param layer слой, который содержит объект.
		*
		*	@param object объект, проверяемый на блокированность.
		*/
		virtual bool isFigureBlocked(  int layer, int object  ) const;

		/**
		 * Меняет блокированность фигуры в слое
		 * @param layer - номер слоя
		 * @param obj - номер фигуры
		 * @param visible значение блокировки
		 */
		virtual void setFigureBlocked( int layer, int obj, bool blocked );

		virtual bool isFigureVisible(  int layer, int object  ) const;

		virtual void setFigureVisible( int layer, int obj, bool visible );

		/**
		 * Проверяет является ли фигура трансформируемой
		 * @param layer слой в котором находится фигура
		 * @param object номер фигуры
		 * @return true если трансформируема
		 */
		virtual bool isFigureTransformed(  int layer, int object  ) const;

		/**
		 * Определяет трансформируемость фигуры
		 * @param layer слой в котором находится фигура
		 * @param object номер фигуры
		 * @param trns значение трансформируемости
		 */
		virtual void setFigureTransformed( int layer, int obj, bool trns );

		virtual QString figureName( int layer, int object  ) const;

		virtual void renameFigure( int layer, int obj,  const QString &name );

		virtual void deleteFigure(int layer, int obj );

        virtual void addFigure(const QPolygonF &poly, QPen pen, QBrush brush, QString name, bool select);

		/**
		*	Устанавливает выделение на некоторый слой и некоторую фигуру
		*
		*	@param layer выделяемый слой.
		*
		*	@param obj выделяемая фигурв.
		*/
		virtual void selectFigure( int layer, int obj);

		virtual int selectedFigure();

		/**
		 * Добавляет новый слой
		 */
		virtual void addLayer(bool visible, bool blocked, const QString &name);

		/**
		*	Возвращает текущее количество слоёв.
		*
		*	@return текущее количество слоёв.
		*/
		virtual int countLayers() const;

		/**
		 * Задает текущий слой.
		 */
		virtual void setLayer(int layer);

		/**
		*	Возвращает имя некоторого слоя.
		*
		*	@param layer слой, имя которого запрашивается.
		*
		*	@return имя запрашиваемого слоя.
		*/
		virtual QString layerName( int layer );

		/**
		*	Переименовывает некоторый слой.
		*
		*	@param layer слой.
		*	@param name новое имя слоя.
		*/
		virtual void renameLayer( int layer, const QString &name );

		/**
		*	Возвращает номер выбранного слоя. Или -1, если ни одного.
		*
		*	@return номер выбранного слоя. Или -1, если их ни одного.
		*/
		virtual int selectedLayer();

		/**
		*	Проверяет некоторый слой на блокированность на экране.
		*
		*	@param layer слой, проверяемый на блокированность.
		*
		*	@return true, если слой блокирован.
		*/
		virtual bool isLayerBlocked( int layer ) const;

		/**
		*	Устанавливает блокированность некоторого слоя в данном кадре.
		*
		*	@param layer слой.
		*	@param blocked блокированность слоя в данном кадре.
		*/
		virtual void setLayerBlocked( int layer, bool blocked );

		/**
		*	Проверяет некоторый слой на видимость на экране.
		*
		*	@param layer слой, проверяемый на видимость.
		*
		*	@return true, если слой видим.
		*/
		virtual bool isLayerVisible( int layer ) const;

		/**
		*	Устанавливает видимость некоторого слоя в данном кадре.
		*
		*	@param layer слой.
		*	@param visible видимость слоя в данном кадре.
		*/
		virtual void setLayerVisible( int layer, bool visible );

		/**
		*	Удаляет кадр по позиции.
		*/
		virtual void deleteFrame(int position);

		/**
		 * Добавляет кадр в определённую позицию
		 * @param position
		 */
		virtual void addFrame(int position);

		/**
		*		Устанавливает инструмент, отвечающий за выбор обьектов и за их модификацию
		*	(перемещение точек), как текущий.
		*/
		virtual void setSelectionTool();

		/**
		*		Устанавливает инструмент, отвечающий за добавление точек в
		*	выбранную фигуру, как текущий.
		*/
		virtual void setAddPointTool();

		/**
		*	Устанавливает новый инструмент, как текущий.
		*
		*	@param tool обьект, реализующий интерфейс инструмента.
		*/
		virtual void setTool( QObject *tool );


		/**
		*	Прорисовывает некоторый кадр текущего слоя.
		*
		*	@param to это то, куда будет отрисован запрошенный кадр.
		*	@param r прямоугольник, в который будет смасштабирован кадр.
		*	@param frame кадр, который будет отрисован.
		*/
		virtual void paintFrameTo( QPainter &to, const QRect &r, qreal frame );

		/**
		*	Прорисовывает некоторый кадр.
		*
		*	@param to это то, куда будет отрисован запрошенный кадр.
		*	@param r прямоугольник, в который будет смасштабирован кадр.
		*	@param frame кадр, который будет отрисован.
		*/
		//virtual void paintWholeFrameTo( QPainter &to, const QRect &r, qreal frame );

		/**
		*		Производит сброс нарисованного мультика.
		*	Удаляются все обьекты, из которых состоит анимация.
		*/
		virtual bool reset();

		/**
		*	Сохраняет состояние виджета в поток.
		*
		*	@param stream поток, в который производится запись.
		*/
		virtual void save( QDataStream &stream ) ;

		/**
		*	Загружает состояние виджета из потока.
		*
		*	@param stream поток, из которого читается состояие.
		*/
		virtual bool load( QDataStream &stream );

		/**
		*	Проверяет возможность удаления выбранных обьектов.
		*
		*	@return true, если это возможно.
		*/
		virtual bool canDeleteSelected() const;

		/**
		*	Удаляет выбранные обьекты, если это возможно.
		*/
		virtual void deleteSelected();

		/**
		*	Проверяет возможность скопировать или вырезать в буффер обмена выбранные обьекты.
		*
		*	@return true, если это возможно.
		*/
		virtual bool canCopyOrCutToClipboard() const;

		/**
		*	Проверяет возможность вставить обьект из буффера обмена.
		*
		*	@return true, если это возможно.
		*/
		virtual bool canPasteFromClipboard() const;

		/**
		*	Вырезает в буффер обмена выделенные обьекты, если это возможно.
		*/
		virtual void cutToClipboard();

		/**
		*	Копирует в буффер обмена выделенные обьекты, если это возможно.
		*/
		virtual void copyToClipboard();

		/**
		*	Вставляет из буффера обмена обьекты, если это возможно.
		*/

		virtual void pasteFromClipboard();

		/**
		*	Задаёт контектстное меню для области рисования.
		*	@*qMenu - указательна контектстное меню
		*/
        virtual void setContextMenu(QMenu *qMenu);

		/**
		*	Получает контектстное меню для области рисования.
		*	@return - указатель на контектстное меню
		*/
        virtual QMenu* getContextMenu();

		/**
		*	Вызывает диалог настроек данного виджета. Модально.
		*/
		virtual void showConfig();

		/**
		*	Возвращает тип представления создаваемого/показываемого ролика.
		*
		*	@return тип представления создаваемого/показываемого ролика.
		*/
		virtual ViewportType viewportType()const;

		virtual void setViewportType( const ViewportType t );

		virtual QSize viewportFixedSize() const;

        virtual void setViewportFixedSizeScale( const QSize &s );
		virtual void setViewportFixedSize( const QSize &s );

		///virtual QColor& getViewportColor();

		virtual void setViewportColor( const QColor &c );

        virtual QObject* getRealPaintWidget();

		virtual void updateAllViews( QWidget *from );

		virtual void doViewportTransparent();

        virtual void rotate90CW();
        virtual void rotate90CWW();
        virtual void flipHorisontal();
        virtual void flipVertical();

		/**
		*	Конструктор.
		*
		*	@param parent родительский для данного виджета виджет.
		*		Может принимать значение 0.
		*
		*	@param manager менеджер плагинов. Необходим для создания
		*		Background и BrushEditor для этого самого Background.
		*/
		PaintWidget( QWidget *parent, plugin::PluginsManager *manager);

		/**
		*	Деструктор.
		*/
		virtual ~PaintWidget();

	public slots:

        void setSizeTo100();
        void RPW_paintEvent(QPoint origin);

		/**
		*  Перемещает фигуру между слоями и внутри слоя по уровням
		*/
		void moveFigure( int layerOld, int objOld, int layerNew, int objNew );

		/**
		*  Перемещает слой по уровням
		*/
		void moveLayer(int layerOld, int layerNew);

	private:

        QDialogButtonBox *buttons;
        bool isCreatedPWE;
        PaintWidgetEditor *e;
        QDialog *dialog;

        qreal scaleVal;
        QPoint scroll;

        RealPaintWidget painter;
		bool transparent;
		QColor viewportColor;
		friend class PaintWidgetEditor;
};

/**
*	@example AnimPlayer Пример, показывающий как с помощью PaintWidget-а проиграть любую анимацию,
*		созданнюя с помощью библиотеки GCore.
*
*	@code
*
*	// Подключаем необходимые заголовочные файлы:
*	#include <QApplication>
*	#include "paintwidget.h"	//Это сам PaintWidget
*	#include "plugin.h"			//А это, собственно, менеджер плагинов, который
*								//нужен PaintWidget-у для загрузки нужных ему плагинов.
*
*	int main( int argC, char *argV[] )
*	{
*		QApplication app( argC, argV );			//Создаём обьект QApplication - без него никак ничего не получится :)
*		plugin::PluginsManager manager( "./" );	//Создаём PluginsManager и указываем ему, что плагины нужно искать в текущей папке.
*		PaintWidget player( 0, manager );		//Создаём сам виджет, который будет отображать ролик.
*
*		QFile file( "./test.sacs2" );	//Открываем файл с анимационным роликом.
*		QDataStream s( &file );			//Создаём из файла поток.
*		s.setVersion( QDataStream::Qt_4_2 ); //Это нужно для совместимости с будующими версими Qt.
*
*		player.load( s );	//Загружаем анимацию.
*		player.setEnabled( false );	//На всякий случай отключаем любую возможность редактирования ролика.
*		player.show();	//Показываем проигрыватель.
*		player.play();	//Запускаем проигрывание.
*
*		return app.exec();	//Запускаем цикл обработки сообщений.
*	}
*
*	@endcode
*
*	Всё, ваш самый простой первый проигрыватель готов :)
*
*	@warning После компиляции не забудте положить в папку с получившимся исполнимым файлом и плагины,
*		необходимые для работы PaintWidget-а (это Background и BrushEditor).
*
*/

#endif
