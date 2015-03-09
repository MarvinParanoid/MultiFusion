#ifndef __BACKGROUND_INTERFACE_H__
#define __BACKGROUND_INTERFACE_H__

#include <QDataStream>
#include <QPainter>
#include <QColor>
#include <QRect>

/**
*	@file backgroundinterface.h В данном файле описан интерфейс, который должен реализовывать
*		любой плагин, отвечающий за отрисовку заднего фона в GCore и за редактирование
*		его настроек.
*/

/**
*		Интерфейс, который должен реализовывать
*	любой плагин, отвечающий за отрисовку заднего фона в GCore и за редактирование
*	его настроек.
*
*		Кроме реализации данного интерфейса, любой такой плагин должен предоставлять
*	следующие сигналы:
*
*	@code
*
*	void changed(); //Должен генерироваться на любые изменения в настройках фона.
*
*	@endcode
*/
class BackgroundInterface
{
	public:

		/**
		 * Создаёт копию данного виджета
		 */
		virtual QObject* copyBackground() = 0;

		/**
		*	Сброс настроек плагина.
		*/
		virtual void reset() = 0;

		/**
		*	Сохранение настроек плагина в поток.
		*
		*	@param stream поток, в который производится запись.
		*/
		virtual void save( QDataStream &stream ) const = 0;

		/**
		*	Чтение настроек плагина из потока.
		*
		*	@param stream поток, из которого происходит чтение.
		*/
		virtual void load( QDataStream &stream ) = 0;

		/**
		*		Должен возвращать виджет, реализующий настройки плагина. Причём, плагин и виджет
		*	должны быть связанны. Т.е. любое изменение настроек в данном виджете должно сразу-же
		*	сказываться на самом плагине.
		*
		*	@return виджет, реализующий настройки плагина.
		*/
		virtual QWidget *editor() = 0;

		/**
		*		Данный плагин предназначен для заливки заднего фона анимации. Эта функциональность
		*	должна реализовываться с помощью данного метода.
		*
		*	@param p то, куда производится рисование.
		*	@param rect прямоугольная область, которую необходимо закрасить.
		*/
		virtual void fill( QPainter &p, QRect rect ) const = 0;

				/**
				*	Конструктор.
				*/
				 BackgroundInterface() {};

		/**
		*	Деструктор.
		*/
		virtual ~BackgroundInterface() {};
};

Q_DECLARE_INTERFACE( BackgroundInterface, "com.alxn1.sacs2.BackgroundInterface/1.0" )

/**
*		Приводит указатель на любой обьект к указателью на интерфейс BackgroundInterface и,
*	если приведение прошло успешно, вызывает метод fill().
*
*	@param o предположительно, указатель на обьект, реализующий интерфейс BackgroundInterface.
*	@param p то, куда будет производится отрисовка.
*	@param rect прямоугольная область, которую нужно закрасить.
*/
template < class T > inline void FILL( T *o, QPainter &p, QRect rect )
{
	BackgroundInterface *i = qobject_cast< BackgroundInterface* >( o );
	if( i == 0 ) return;
	i->fill( p, rect );
}

/**
*		Приводит указатель на любой обьект к указателью на интерфейс BackgroundInterface и,
*	если приведение прошло успешно, вызывает метод editor().
*
*	@param o предположительно, указатель на обьект, реализующий интерфейс BackgroundInterface.
*
*	@return указатель на виджет, реализующий настройки данного плагина. И 0, если где-то произошла
*		ошибка, или плагином это не поддерживается.
*/
template < class T > inline QWidget* GET_EDITOR( T *o )
{
	BackgroundInterface *i = qobject_cast< BackgroundInterface* >( o );
	if( i == 0 ) return 0;
	return i->editor();
};

/**
*	Приводит указатель на любой обьект к указателью на интерфейс BackgroundInterface.
*
*	@return Приведённый указатель или 0, если произошла ошибка.
*/
template < class T > inline BackgroundInterface* BACKGROUND( T *o )
{
	return qobject_cast< BackgroundInterface* >( o );
}

#endif /* __BACKGROUND_INTERFACE_H__ */
