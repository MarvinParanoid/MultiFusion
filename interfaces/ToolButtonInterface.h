#ifndef ToolButtonInterface_H
#define ToolButtonInterface_H

#include <QToolButton>
#include <QWidget>
#include <QObject>

class ToolButtonInterface:public QToolButton
{
	public:
		virtual QObject* getTool() = 0;
		virtual void setTool(QObject* t) = 0;

		ToolButtonInterface( QWidget *parent ):QToolButton(parent){};
		virtual ~ToolButtonInterface(){};

	private slots:
		void onClicked( bool checked );
};

Q_DECLARE_INTERFACE( ToolButtonInterface, "com.Tailer.MultiFusion.ToolButtonInterface/1.0" )

template < class T > inline ToolButtonInterface* TOOLBUTTON( T *o )
{
	return qobject_cast< ToolButtonInterface* >( o );
}
#endif
