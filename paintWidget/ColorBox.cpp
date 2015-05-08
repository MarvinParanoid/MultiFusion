/*
***************************************************
*		ColorBox class implementation
*/

#include "ColorBox.h"
#include "qdrawutil.h"

bool ColorBox::isEnabled() const
{
	return enabled;
}

void ColorBox::setEnabled( const bool _enabled )
{
	if( enabled == _enabled )
		return;

	enabled = _enabled;
	setAutoFillBackground( !enabled );
	update();
}

ColorBox::ColorBox( QWidget *parent, QColor &c ):
	QWidget( parent ), _color( c ), enabled( true )
{
	setAutoFillBackground( false );
	setMinimumSize( QSize( 128, 128 ) );

	QPixmap pm( 20, 20 );

	QPainter pmp( &pm );
		pmp.fillRect( 0, 0, 10, 10, Qt::lightGray );
		pmp.fillRect(10, 10, 10, 10, Qt::lightGray );
		pmp.fillRect( 0, 10, 10, 10, Qt::darkGray );
		pmp.fillRect( 10, 0, 10, 10, Qt::darkGray );
	pmp.end();

	QPalette pal = palette();
	pal.setBrush( backgroundRole(), QBrush( pm ) );
	setAutoFillBackground( false );
	setPalette( pal );
}

ColorBox::~ColorBox()
{
}

void ColorBox::mousePressEvent( QMouseEvent *event )
{
	if( !isEnabled() )
		return;

	QColor newColor = QColorDialog::getColor( _color, this );
	if( newColor.isValid() )
	{
		_color = newColor;
		emit colorChanged();
		update();
	}
};

QColor* ColorBox::getGolor()const
{
	return &_color;
}

void ColorBox::paintEvent( QPaintEvent *event )
{
	QPainter p( this );
    p.setRenderHints( QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform );
	qDrawShadePanel( &p, 0, 0, width(), height(), palette(), true, 3 );
	p.setPen( Qt::NoPen );
	if( enabled )
		p.fillRect( 3, 3, width() - 6, height() - 6, _color );
}
