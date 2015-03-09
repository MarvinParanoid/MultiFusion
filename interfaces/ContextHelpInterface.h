/*
 * ContextHelpInterface.h
 *
 *  Created on: 18.04.2009
 *      Author: Trinity
 */

#ifndef CONTEXTHELPINTERFACE_H_
#define CONTEXTHELPINTERFACE_H_

class ContextHelpInterface: public QWidget
{
	public:

		/**
		 * Метод, показывающий оглавление справки.
		 */
		virtual void show() = 0;

		/**
		 * Метод, показывающий контекстную справку
		 * для текущего активного элемента.
		 */
		virtual void showContext() = 0;

		/**
		*	Конструктор.
		*/
		ContextHelpInterface(){}

		/**
		*	Деструктор.
		*/
		virtual ~ContextHelpInterface(){}
};

/**
 * Декларация интерфейса
 */
Q_DECLARE_INTERFACE( ContextHelpInterface, "ifmo.lothe.ContextHelpInterface/1.0" )


/**
*	Приводит указатель на любой обьект к указателью на интерфейс SaveSVGInterface.
*
*	@return Приведённый указатель или 0, если произошла ошибка.
*/
template < class T > inline ContextHelpInterface* CONTEXTHELP( T *o )
{
	return qobject_cast< ContextHelpInterface* >( o );
}

#endif /* CONTEXTHELPINTERFACE_H_ */
