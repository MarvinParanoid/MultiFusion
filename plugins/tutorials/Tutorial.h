/*
 * tutorial.h
 *
 *  Created on: 31.05.2010
 *      Author: Trinity
 */

#ifndef TUTORIAL_H_
#define TUTORIAL_H_

#include <QAction>
#include <QObject>

class Tutorial: public QAction {
	Q_OBJECT

private:
	QString id;
	QString caption;

public:
	Tutorial(QObject * parent, QString id, QString caption):QAction(parent) {
		this->id = id;
		this->caption = caption;
		connect(this, SIGNAL (triggered(bool)), this, SLOT (pushed()) );
	}

	~Tutorial() {}

public slots:
	void pushed() {
		emit shouldShow(id, caption);
	}

signals:
	void shouldShow(QString, QString);
};


#endif /* TUTORIAL_H_ */
