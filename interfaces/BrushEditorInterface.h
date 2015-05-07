#ifndef __BRUSH_INTERFACE_H__
#define __BRUSH_INTERFACE_H__

#include <QRect>
#include <QBrush>
#include <QMatrix>
#include <QPainter>
#include "GradientEditorInterface.h"

/**
*	@file brusheditorinterface.h В данном файле описан интерфейс, который должен реализовывать
*		любой плагин, отвечающий за редактирование типа и настроек заливки.
*/

/**
*		Интерфейс, который должен реализовывать
*	любой плагин, отвечающий за редактирование типа и настроек заливки.
*	Плагин, реализующий данный интерфейс должен наследоваться от любого потомка класса
*	QWidget или от него самого.
*
*		Все возможные настройки, если они являются настройками типа qreal и могут изменяться
*	в зависимости от положения закрашиваемой области или её размера, должны принадлежать
*	интервалу [0..1].
*
*		Кроме реализации данного интерфейса, любой такой плагин должен предоставлять
*	следующие сигналы:
*
*	@code
*
*	void brushChanged( const QBrush &brush ); //Должен генерироваться на любые изменения в
*					//настройках заливки. Параметром должны передаваться новые настройки.
*
*	@endcode
*/
class BrushEditorInterface:public QWidget
{
	public:
		/**
		*	Должен возвращать текущие настройки.
		*/
		virtual QBrush brush() const = 0;

		/**
		*	Установка начальных настроек перед редактированием.
		*/
		virtual void setBrush( const QBrush &b ) = 0;

				/**
				*	Конструктор.
				*/
				 BrushEditorInterface() {};

		/**
		*	Деструктор.
		*/
		virtual ~BrushEditorInterface() {};
};

Q_DECLARE_INTERFACE( BrushEditorInterface, "com.alxn1.sacs2.BrushEditorInterface/1.0" )

/**
*		Приводит указатель на любой обьект к указателью на интерфейс BrushEditorInterface и,
*	если приведение прошло успешно, вызывает метод brush().
*
*	@param o предположительно, указатель на обьект, реализующий интерфейс BrushEditorInterface.
*
*	@return текущие тип и настройки заливки.
*/
template < class T > inline QBrush GET_BRUSH( T *o )
{
	BrushEditorInterface *i = qobject_cast< BrushEditorInterface* >( o );
	if( i == 0 ) return QBrush();
	return i->brush();
}

/**
*		Приводит указатель на любой обьект к указателью на интерфейс BrushEditorInterface и,
*	если приведение прошло успешно, вызывает метод setBrush().
*
*	@param o предположительно, указатель на обьект, реализующий интерфейс BrushEditorInterface.
*	@param b устанавливаемые тип и настройки заливки.
*/
template < class T > inline void SET_BRUSH( T *o, const QBrush &b )
{
	BrushEditorInterface *i = qobject_cast< BrushEditorInterface* >( o );
	if( i == 0 ) return;
	return i->setBrush( b );
}

/**
*	Приводит указатель на любой обьект к указателью на интерфейс BrushEditorInterface.
*
*	@return Приведённый указатель или 0, если произошла ошибка.
*/
template < class T > inline BrushEditorInterface* BRUSHEDITOR( T *o )
{
	return qobject_cast< BrushEditorInterface* >( o );
}

/**
*		Подготавливает настройки перед их использованием при рисовании.
*	Все возможные настройки, если они являются настройками типа qreal и могут изменяться
*	в зависимости от положения закрашиваемой области или её размера, будут приведены от
*	интервалу [0..1] к интервалу, необходимому для заданной прямоугольной области.
*
*	@param b настройки, которые необходимо подготовить.
*	@param rect область, которую необходимо залить, используюя эти настройки.
*/
inline QBrush prepareBrushToPaint( const QBrush &b, const QRect &rect )
{
	switch( b.style() )
	{
		case Qt::LinearGradientPattern:
		case Qt::ConicalGradientPattern:
		case Qt::RadialGradientPattern:
			return prepareGradientToPaint( *b.gradient(), rect );
			break;

		case Qt::TexturePattern:
		{
			QMatrix m;
			QPoint pos = rect.topLeft();
			m.translate( pos.x(), pos.y() );
			QBrush brush( b );
			brush.setMatrix( m );
			return brush;
			break;
		}

		default:
			break;
	}

	return b;
}

/**
*		Подготавливает настройки перед их использованием при редактировании.
*	Все возможные настройки, если они являются настройками типа qreal и могут изменяться
*	в зависимости от положения закрашиваемой области или её размера, будут приведены к
*	интервалу [0..1].
*
*	@param b настройки, которые необходимо подготовить.
*	@param rect область, которую до этого заливали с помощью данных настроек.
*/
inline QBrush prepareBrushToEdit( const QBrush &b, const QRect &rect )
{
	switch( b.style() )
	{
		case Qt::LinearGradientPattern:
		case Qt::ConicalGradientPattern:
		case Qt::RadialGradientPattern:
			return prepareGradientToEdit( *b.gradient(), rect );
			break;

		case Qt::TexturePattern:
		{
			QMatrix m;
			QPoint pos = rect.topLeft();
			m.translate( -pos.x(), -pos.y() );
			QBrush brush( b );
			brush.setMatrix( m );
			return brush;
			break;
		}

		default:
			break;
	}

	return b;
}

#endif /* __BRUSH_INTERFACE_H__ */
