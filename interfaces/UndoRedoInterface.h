/**
 * UndoRedoInterface.h
 *
 *  Created on: 13.06.2009
 *      Author: Alexey Kukushkin
 */

#ifndef UNDOREDOINTERFACE_H_
#define UNDOREDOINTERFACE_H_

class UndoRedoInterface
{
	public:

		/**
		*	Конструктор.
		*/
		UndoRedoInterface(){}

		/**
		*	Деструктор.
		*/
		virtual ~UndoRedoInterface(){}
};

/**
 * Декларация интерфейса
 */
Q_DECLARE_INTERFACE( UndoRedoInterface, "com.alxn1.MultiFusion.TimelineInterface/1.0" )


/**
*	Приводит указатель на любой обьект к указателью на интерфейс TimelineInterface.
*
*	@return Приведённый указатель или 0, если произошла ошибка.
*/
template < class T > inline UndoRedoInterface* UNDOREDO( T *o )
{
	return qobject_cast< UndoRedoInterface* >( o );
}
#endif /* UNDOREDOINTERFACE_H_ */
