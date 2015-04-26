#ifndef __RULER_INTERFACE_H__
#define __RULER_INTERFACE_H__

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
*
*	@endcode
*/
class RulerInterface
{
	public:
		/**
		*	Конструктор.
		*/
		RulerInterface() {}
		
		//virtual int getFps() = 0;
		/**
		*	Деструктор.
		*/
		virtual ~RulerInterface(){}
};

Q_DECLARE_INTERFACE( RulerInterface, "com.alxn1.MultiFusion.RulerInterface/1.0" )

/**
*	Приводит указатель на любой обьект к указателью на интерфейс TimelineInterface.
*
*	@return Приведённый указатель или 0, если произошла ошибка.
*/
template < class T > inline RulerInterface* RULER( T *o )
{
	return qobject_cast< RulerInterface* >( o );
}

#endif /* __RULER_INTERFACE_H__ */
