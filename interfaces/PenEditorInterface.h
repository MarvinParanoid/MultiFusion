#ifndef __PEN_EDITOR_INTERFACE_H__
#define __PEN_EDITOR_INTERFACE_H__

#include <QPainter>

/**
*	@file peneditorinterface.h В данном файле описан интерфейс, который должен реализовывать
*		любой плагин, отвечающий за редактирование типа и настроек линий.
*/

/**
*		Интерфейс, который должен реализовывать
*	любой плагин, отвечающий за редактирование типа и настроек линий.
*	Плагин, реализующий данный интерфейс должен наследоваться от любого потомка класса
*	QWidget или от него самого.
*
*		Кроме реализации данного интерфейса, любой такой плагин должен предоставлять
*	следующие сигналы:
*
*	@code
*
*	void penChanged( const QPen &pen ); //Должен генерироваться на любые изменения в
*						//настройках линий. Параметром должны передаваться новые настройки.
*
*	@endcode
*/
class PenEditorInterface
{
	public:
		/**
		*	Должен возвращать текущие настройки.
		*/
		virtual QPen pen() const = 0;

		/**
		*	Установка начальных настроек перед редактированием.
		*/
		virtual void setPen( const QPen &p ) = 0;

		/**
		*	Конструктор.
		*/
		PenEditorInterface() {};

		/**
		*	Деструктор.
		*/
		virtual ~PenEditorInterface() {};
};

Q_DECLARE_INTERFACE( PenEditorInterface, "com.alxn1.sacs2.PenEditorInterface/1.0" )

/**
*		Приводит указатель на любой обьект к указателью на интерфейс PenEditorInterface и,
*	если приведение прошло успешно, вызывает метод pen().
*
*	@param o предположительно, указатель на обьект, реализующий интерфейс PenEditorInterface.
*
*	@return текущие тип и настройки линий.
*/
template < class T > inline QPen GET_PEN( T *o )
{
	PenEditorInterface *i = qobject_cast< PenEditorInterface* >( o );
	if( i == 0 ) return QPen();
	return i->pen();
}

/**
*		Приводит указатель на любой обьект к указателью на интерфейс PenEditorInterface и,
*	если приведение прошло успешно, вызывает метод setPen().
*
*	@param o предположительно, указатель на обьект, реализующий интерфейс PenEditorInterface.
*	@param p устанавливаемые тип и настройки линий.
*/
template < class T > inline void SET_PEN( T *o, const QPen &p )
{
	PenEditorInterface *i = qobject_cast< PenEditorInterface* >( o );
	if( i == 0 ) return;
	return i->setPen( p );
}

/**
*	Приводит указатель на любой обьект к указателью на интерфейс PenEditorInterface.
*
*	@return Приведённый указатель или 0, если произошла ошибка.
*/
template < class T > inline PenEditorInterface* PENEDITOR( T *o )
{
	return qobject_cast< PenEditorInterface* >( o );
}

#endif /* __PEN_EDITOR_INTERFACE_H__ */
