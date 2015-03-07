#ifndef __ALIGNANDDISTRIBUTE_INTERFACE_H__
#define __ALIGNANDDISTRIBUTE_INTERFACE_H__

/**
*	@file AlignAndDistribute.h В данном файле описан интерфейс, который должен реализовывать
*		любой плагин, отвечающий за Выровнять и расставить.
*/

/**
*		Интерфейс, который должен реализовывать любой плагин, отвечающий за Выровнять и расставить.
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

class AlignAndDistributeInterface
{
	public:
		/**
		*	Конструктор.
		*/
		AlignAndDistributeInterface() {}
		
		//virtual int getFps() = 0;
		/**
		*	Деструктор.
		*/
		virtual ~AlignAndDistributeInterface(){}
};

Q_DECLARE_INTERFACE( AlignAndDistributeInterface, "com.alxn1.MultiFusion.AlignAndDistributeInterface/1.0" )

/**
*	Приводит указатель на любой обьект к указателью на интерфейс TimelineInterface.
*
*	@return Приведённый указатель или 0, если произошла ошибка.
*/
template < class T > inline AlignAndDistributeInterface* TIMELINE( T *o )
{
	return qobject_cast< AlignAndDistributeInterface* >( o );
}

#endif /* __ALIGNANDDISTRIBUTE_INTERFACE_H__ */
