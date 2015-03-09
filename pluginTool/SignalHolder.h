/*
 * SignalHolder.h
 *
 *  Created on: 10.05.2010
 *      Author: Irina Latysheva
 */

#ifndef SIGNALHOLDER_H_
#define SIGNALHOLDER_H_

#include <QObject>

//class QObject;

class SignalHolder: public QObject
{
	Q_OBJECT
	signals:
		void setActive( QString name );

	public:
		SignalHolder() {}
		~SignalHolder() {}
		void doNothing();

	//friend class InterfacePlugin;
};

#endif /* SIGNALHOLDER_H_ */
