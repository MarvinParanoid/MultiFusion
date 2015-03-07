#ifndef __RULER_H__
#define __RULER_H__

#include <QClipboard>
#include <QApplication>
#include <QMouseEvent>
#include <QObject>
#include <QPainter>
#include "./../PluginTool/Plugin.h"
#include "PaintWidget.h"


class GRuler:public QWidget
{
	Q_OBJECT
	public:
		GRuler( const plugin::PluginsManager *manager, QWidget *parent = 0, PaintWidget *p = 0);
		~GRuler();
	signals:
		void change();
	protected:
		virtual bool drawNumber(QString txt, int x, int y, QPainter p); //Рисует цифры на линейке
		virtual void paintEvent( QPaintEvent * event );
		virtual void mousePressEvent( QMouseEvent *event );
		virtual void mouseMoveEvent( QMouseEvent * event );
		virtual void mouseReleaseEvent( QMouseEvent * event );
	private:
		PaintWidget *painter;
};

#endif
