#ifndef __TIMELINE_INTERFACE_H__
#define __TIMELINE_INTERFACE_H__

/**
*	@file timelineinterface.h В данном файле описан интерфейс, который должен реализовывать
*		любой плагин, отвечающий за т.н. таймлайн.
*/

/**
*		Интерфейс, который должен реализовывать любой плагин, отвечающий за т.н. таймлайн.
*	Плагин, реализующий данный интерфейс должен наследоваться от любого потомка класса
*	QWidget или от него самого.
*
*		Кроме реализации данного интерфейса, любой такой плагин должен предоставлять
*	следующие сигналы:
*
*	@code
*
*	void playPressed(); 	//Догжен генерироваться, когда пользователь нажал на кнопку "Play".
*	void stopPressed();		//Догжен генерироваться, когда пользователь нажал на кнопку "Stop".
*	void pausePressed();	//Догжен генерироваться, когда пользователь нажал на кнопку "Pause".
*	void frameChanged( int frame );//Догжен генерироваться, когда пользователь сменил текущий кадр.
*
*	@endcode
*/
class TimelineInterface
{
	public:
		/**
		*	Конструктор.
		*/
		TimelineInterface()
				{

				}
		virtual int getFps() = 0;
		/**
		*	Деструктор.
		*/
		virtual ~TimelineInterface()
		{
		}
};

Q_DECLARE_INTERFACE( TimelineInterface, "com.alxn1.MultiFusion.TimelineInterface/1.0" )

/**
*	Приводит указатель на любой обьект к указателью на интерфейс TimelineInterface.
*
*	@return Приведённый указатель или 0, если произошла ошибка.
*/
template < class T > inline TimelineInterface* TIMELINE( T *o )
{
	return qobject_cast< TimelineInterface* >( o );
}

#endif /* __TIMELINE_INTERFACE_H__ */
