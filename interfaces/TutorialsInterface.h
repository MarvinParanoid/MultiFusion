/*
 * TutorialsInterface.h
 *
 *  Created on: 18.04.2009
 *      Author: Trinity
 */

#ifndef TUTORIALSINTERFACE_H_
#define TUTORIALSINTERFACE_H_

class TutorialsInterface: public QWidget
{
	public:
		/**
		 * Метод, отображающий указанное пособие с указанным заголовком.
		 */
		virtual void show(QString id, QString caption) = 0;

		/**
		*	Конструктор.
		*/
		TutorialsInterface(){}

		/**
		*	Деструктор.
		*/
		virtual ~TutorialsInterface(){}
};

/**
 * Декларация интерфейса
 */
Q_DECLARE_INTERFACE( TutorialsInterface, "ifmo.lothe.TutorialsInterface/1.0" )


/**
*	Приводит указатель на любой обьект к указателью на интерфейс SaveSVGInterface.
*
*	@return Приведённый указатель или 0, если произошла ошибка.
*/
template < class T > inline TutorialsInterface* TUTORIALS( T *o )
{
	return qobject_cast< TutorialsInterface* >( o );
}

#endif /* TUTORIALSINTERFACE_H_ */
