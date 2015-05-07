#ifndef __TOOLBOX_INTERFACE_H__
#define __TOOLBOX_INTERFACE_H__

#include <QPen>
#include <QBrush>
#include <QWidget>
#include <QGridLayout>

#include "Plugin.h"
#include "FigureToolInterface.h"

/**
*	@file toolboxinterface.h В данном файле описан интерфейс, который должен реализовывать
*		любой плагин, отвечающий за реализацию панели инструментов.
*/


/**
*		Интерфейс, который должен реализовывать
*	любой плагин, отвечающий за реализацию панели инструментов.
*	Плагин, реализующий данный интерфейс должен наследоваться от любого потомка класса
*	QWidget или от него самого.
*
*		Кроме реализации данного интерфейса, любой такой плагин должен предоставлять
*	следующие сигналы:
*
*	@code
*
*	void selectionToolSelected(); 		//Выбран инструмент выделения обьектов.
*	void addPointToolSelected();		//Выбран инструмент добавления точек в обьект.
*	void zoomInToolSelected();			//Нажата кнопка увеличения масштаба кадра.
*	void zoomOutToolSelected();			//Нажата кнопка уменьшения масштаба кадра.
*	void closedChanged( bool closed );	//Нажата кнопка "замкнутости" фигуры.
*							//В параметре должна передваться выбрання замкнутость
*							//(т.е. true, если фигура должна быть замкнута и false
*							//в противном случае).
*
*	void toolSelected( FigureToolInterface *tool ); //Выбран некоторый инструмен, загруженный
*									//как плагин. В параметре должен передаваться тот самый
*									//выбранный пользователем и загруженный данным обьектом плагин.
*
*	void penChanged( const QPen &p );	//Вызывается в тот момент, когда пользователь изменил настройки или тип линий.
*	void brushChanged( const QBrush &b ); //Вызывается в тот момент, когда пользователь изменил настройки или тип заливки.
*
*	@endcode
*/
class ToolBoxInterface: public QWidget
{
	signals:
		 /**
		 * Отправляется после выполнения события, которое нам нужно сохранить в истории
		 * @param event имя события
		 */
		void StateChanged(QString event);
	public:
		virtual QGridLayout* getFigureLayout() = 0;

		virtual QVBoxLayout& getMainLayout() = 0;

		/**
		*	Метод должен возвращать текущие выбранные настройки по поводу замкнутости фигуры.
		*
		*	@return true, если фигуры должна быть замкнута.
		*/
		virtual bool isClosedChecked() const = 0;

		/**
		*	Метод должен устанавливать текущие выбранные настройки по поводу замкнутости фигуры.
		*
		*	@param closed будет установлен в true, если фигуры должна быть замкнута.
		*/
		virtual void setClosedChecked( bool closed ) = 0;

		/**
		*	Конструктор.
		*/
		ToolBoxInterface(){};

		/**
		*	Деструктор.
		*/
		virtual ~ToolBoxInterface(){};
};

Q_DECLARE_INTERFACE( ToolBoxInterface, "com.Tailer.MultiFusion.ToolBoxInterface/1.0" )

/**
*	Приводит указатель на любой обьект к указателью на интерфейс ToolboxInterface.
*
*	@return Приведённый указатель или 0, если произошла ошибка.
*/
template < class T > inline ToolBoxInterface* TOOLBOX( T *o )
{
	return qobject_cast< ToolBoxInterface* >( o );
}

#endif /* __TOOLBOX_INTERFACE_H__ */
