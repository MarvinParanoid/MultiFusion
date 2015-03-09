#ifndef ColorBoxInterface_H
#define ColorBoxInterface_H




class ColorBoxInterface
{
	signals:
		void colorChanged();

	public:
		virtual bool isEnabled() const = 0;
		virtual void setEnabled( const bool _enabled ) = 0;
		virtual QColor* getGolor()const  = 0;

		ColorBoxInterface(){}
		virtual ~ColorBoxInterface(){}

		virtual void mousePressEvent( QMouseEvent *event ) = 0 ;
		virtual void paintEvent( QPaintEvent *event ) = 0;


};

Q_DECLARE_INTERFACE( ColorBoxInterface, "com.Tailer.MultiFusion.ColorBoxInterface/1.0" )

template < class T > inline ColorBoxInterface * COLORBOX( T *o )
{
	return qobject_cast< ColorBoxInterface* >( o );
}

#endif
