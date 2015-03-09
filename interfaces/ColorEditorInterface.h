#ifndef __COLOR_EDITOR_INTERFACE_H__
#define __COLOR_EDITOR_INTERFACE_H__

#include <QColor>
#include <QWidget>

//;
/**
*	@file coloreditorinterface.h В данном файле описан интерфейс, который должен реализовывать
*		любой плагин, отвечающий за редактирование цвета.
*/

/**
*		Интерфейс, который должен реализовывать
*	любой плагин, отвечающий за редактирование цвета.
*	Плагин, реализующий данный интерфейс должен наследоваться от любого потомка класса
*	QWidget или от него самого.
*
*		Кроме реализации данного интерфейса, любой такой плагин должен предоставлять
*	следующие сигналы:
*
*	@code
*
*	void colorChanged( const QColor newColor ); //Должен генерироваться на любые изменения в
*									//цвете. Параметром должен передаваться новый цвет.
*
*	@endcode
*/
class ColorEditorInterface:public QWidget
{
	public:
		/**
		*	Должен возвращать текущий цвет.
		*/
		virtual QColor color() const = 0;

		/**
		*	Установка начального цвета перед редактированием.
		*/
		virtual void setColor( const QColor newColor ) = 0;

				/**
				*	Конструктор.
				*/
				 ColorEditorInterface() {};

		/**
		*	Деструктор.
		*/
		virtual ~ColorEditorInterface() {};
};

Q_DECLARE_INTERFACE( ColorEditorInterface, "com.alxn1.sacs2.ColorEditorInterface/1.0" )

/**
*		Приводит указатель на любой обьект к указателью на интерфейс ColorEditorInterface и,
*	если приведение прошло успешно, вызывает метод color().
*
*	@param o предположительно, указатель на обьект, реализующий интерфейс ColorEditorInterface.
*
*	@return текущий цвет.
*/
template < class T > inline QColor GET_COLOR( T *o )
{
	ColorEditorInterface *i = qobject_cast< ColorEditorInterface* >( o );
	if( i == 0 ) return QColor();
	return i->color();
}

/**
*		Приводит указатель на любой обьект к указателью на интерфейс ColorEditorInterface и,
*	если приведение прошло успешно, вызывает метод setСolor().
*
*	@param o предположительно, указатель на обьект, реализующий интерфейс ColorEditorInterface.
*	@param c устанавливаемый цвет.
*/
template < class T > inline void SET_COLOR( T *o, const QColor &c )
{
	ColorEditorInterface *i = qobject_cast< ColorEditorInterface* >( o );
	if( i == 0 ) return;
	return i->setColor( c );
}

/**
*	Приводит указатель на любой обьект к указателью на интерфейс ColorEditorInterface.
*
*	@return Приведённый указатель или 0, если произошла ошибка.
*/
template < class T > inline ColorEditorInterface* COLOREDITOR( T *o )
{
	return qobject_cast< ColorEditorInterface* >( o );
}

#endif /* __COLOR_EDITOR_INTERFACE_H__ */
