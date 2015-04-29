// область для рисования

#ifndef RPW_H
#define RPW_H

#include <QApplication>
#include <QClipboard>
#include <QLineEdit>
#include <QScrollArea>
#include <QMouseEvent>
#include <QColorDialog>
#include <QPainterPath>
#include <QByteArray>
#include <QTimer>
#include <QMenu>
#include <math.h>

#include "Plugin.h"
#include "PWE.h"
#include "../interfaces/RPWInterface.h"
#include "../interfaces/FigureToolInterface.h"
#include "../interfaces/BackgroundInterface.h"
#include "GSelectionRect.h"
#include "GVectorFigure.h"
#include "GContainer.h"
#include "GLayer.h"

#include "ui_viewportproperties.h"

class RealPaintWidget:public RPWInterface
{
	Q_OBJECT
	Q_INTERFACES(RPWInterface)

	signals:
		void figureSelected(int layer , int obj);
		void layerSelected( int layer );
		void frameChanged( qreal frame );
		void objectCreated();
		void isPlayed();
		void undoEvents();

        void paintEvent(QPoint);
        void mouseMoveEvent(QPoint global, QPoint rpw);

		/**
		 * Отправляется после выполнения события, которое нам нужно сохранить в истории
		 * @param event имя события
		 */
		void StateChanged(QString event);

		/**
		 * Вызывается, когда в позиции таймлайна нет кадра
		 */
		void isFrame(bool value);
	public:

		virtual QObject* getUndo();

		virtual void setFromUndo(QObject* undo);

		virtual void setFrame( qreal f, bool played );

		virtual qreal frame() const;

		virtual int countFrames() const;

		virtual void paintFrameTo( QPainter &to, const QRect &r, qreal frame );
		//virtual void paintWholeFrameTo( QPainter &to, const QRect &r, qreal frame );

		/**
		* Задание контектстного меню
		* @param qMenu
		*/
        virtual void setContextMenu(QMenu *qMenu);

		/**
		* Получение контектстного меню
		* @return
		*/
        virtual QMenu* getContextMenu();

		virtual QObject* getBackground();

		virtual void setBackground(QObject* back);

		virtual bool getFixedSize();

		virtual void setFixedSize(bool back);

		virtual QSize getSize();

		virtual void setSize(QSize sz);

		/** Получение объекта для работы с выделением - класс GSelectionRect
		*
		* @return
		*/
		virtual QObject* getSelection();

		virtual void setSelection(QObject* s);

		virtual QObject* getCurrentTool();

		virtual void setCurrentTool(QObject* ct);

		virtual void addLayer(bool visible, bool blocked, const QString &name);

		virtual QAction* getPropertiesAct();

		virtual void setPropertiesAct(QAction* pa);

		virtual bool getInKeyPressedHandler();

		virtual void setInKeyPressedHandler(bool kph);

		virtual bool getInSelectionMode();

		virtual void setInSelectionMode(bool sm);

		virtual QRect getSelectionRect();

		virtual void setSelectionRect(QRect sr);

		RealPaintWidget( plugin::PluginsManager *manager, QWidget *parent = 0 );
		virtual ~RealPaintWidget();

		virtual void paintEvent( QPaintEvent * event );
		virtual void mousePressEvent( QMouseEvent *event );
		virtual void mouseMoveEvent( QMouseEvent * event );
		virtual void mouseReleaseEvent( QMouseEvent * event );
		virtual void mouseDoubleClickEvent( QMouseEvent * event );
		virtual void resizeEvent( QResizeEvent * event );

	public slots:
		/**
		* Появление окна с характеристиками объекта
		*/
		virtual void onPropertiesObj();
		/**
		* Изменение прозрачности объекта
		* @param _alpha
		*/
		virtual void alphaChanged(int _alpha);


	private:
        QMenu* paintConMenu; // контекстное меню для области рисования.
		QAction *propertiesAct;
		QObject *background;
		bool fixedSize;
		QSize size;
		GSelectionRect selection;
		QVector <GLayer*> layers;
		int currentLayer;
		int currentFrame;
		bool inKeyPressedHandler;
		FigureToolInterface* currentTool;
		bool inSelectionMode;
		plugin::PluginsManager *_manager;
		QRect selectionRect;
        bool isMousePress;

		friend class PaintWidget;
		friend class UndoStructure;
		friend class PaintWidgetEditor;

};

template < class T > inline RealPaintWidget * REALPAINTWIDGET( T *o )
{
	return qobject_cast< RealPaintWidget* >( o );
}

#endif
