/*
 * SaveSVGInterface.h
 *
 *  Created on: 04.11.2009
 *      Author: Trinity
 */

#ifndef SAVESVGINTERFACE_H_
#define SAVESVGINTERFACE_H_

class SaveSVGInterface: public QWidget
{
	public:

		/**
		*	Конструктор.
		*/
		SaveSVGInterface(){}

		/**
		*	Деструктор.
		*/
		virtual ~SaveSVGInterface(){}

		virtual bool SaveFile(QString to) = 0;
};

/**
 * Декларация интерфейса
 */
Q_DECLARE_INTERFACE( SaveSVGInterface, "ifmo.lothe.SaveSVGInterface/1.0" )


/**
*	Приводит указатель на любой обьект к указателью на интерфейс SaveSVGInterface.
*
*	@return Приведённый указатель или 0, если произошла ошибка.
*/
template < class T > inline SaveSVGInterface* SAVESVG( T *o )
{
	return qobject_cast< SaveSVGInterface* >( o );
}

#endif /* SAVESVGINTERFACE_H_ */
