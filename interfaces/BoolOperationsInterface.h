/*
 * BoolOperationsInterface.h
 *
 *  Created on: 20.05.2015
 *      Author: Sergey Korolev
 */

#ifndef BOOLOPERINTERFACE_H_
#define BOOLOPERINTERFACE_H_

class BoolOperationsInterface
{
	public:

		/**
		*	Конструктор.
		*/
                BoolOperationsInterface() {}

		/**
		*	Деструктор.
		*/
                virtual ~BoolOperationsInterface() {}

                virtual void createUnion() = 0;
                virtual void createDifference() = 0;
                virtual void createIntersection() = 0;
                virtual void createExclusion() = 0;
                virtual void createDivision() = 0;
};

/**
 * Декларация интерфейса
 */
Q_DECLARE_INTERFACE(BoolOperationsInterface, "com.whiteproggy.BoolOperationsInterface/1.0")


/**
*	Приводит указатель на любой обьект к указателью на интерфейс BoolOperationsInterface.
*
*	@return Приведённый указатель или 0, если произошла ошибка.
*/
template <class T> inline BoolOperationsInterface* BOOLOPERATIONS(T *o)
{
        return qobject_cast<BoolOperationsInterface*>(o);
}

#endif /* BOOLOPERINTERFACE_H_ */
