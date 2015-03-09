/*
 * ColorBox.h
 *
 *  Created on: 23.04.2009
 *      Author: Admin
 */

#ifndef COLORBOX_H_
#define COLORBOX_H_

#include <QApplication>
#include <QClipboard>
#include <QLineEdit>
#include <QScrollArea>
#include <QMouseEvent>
#include <QColorDialog>
#include <QPainter>
#include <QByteArray>
#include <QTimer>
#include "../interfaces/ColorBoxInterface.h"

class ColorBox:public QWidget, public ColorBoxInterface
{
	Q_OBJECT
	Q_INTERFACES(ColorBoxInterface)

	signals:

		void colorChanged();

	public:
		virtual bool isEnabled() const;
		virtual void setEnabled( const bool _enabled );
		virtual QColor* getGolor()const;

		ColorBox( QWidget *parent, QColor &c );
		virtual ~ColorBox();

		virtual void mousePressEvent( QMouseEvent *event );
		virtual void paintEvent( QPaintEvent *event );

	private:
		QColor &_color;
		bool enabled;
};

#endif /* COLORBOX_H_ */
