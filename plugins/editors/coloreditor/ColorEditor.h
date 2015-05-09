#ifndef __COLOR_EDITOR_H__
#define __COLOR_EDITOR_H__

#include <QWidget>
#include <QPainter>
#include <QImage>
#include <QColorDialog>
#include <QMouseEvent>
#include <QBoxLayout>
#include <QSlider>
#include "qdrawutil.h"

#include "./../../../pluginTool/Plugin.h"
#include "./../../../pluginTool/InterfacePlugin.h"
#include "./../../../interfaces/ColorEditorInterface.h"

class ColorEditor:public ColorEditorInterface, public InterfacePlugin
{
	Q_OBJECT
	Q_INTERFACES( ColorEditorInterface )
	Q_INTERFACES( InterfacePlugin )

	signals:
		void colorChanged( const QColor newColor );
		void setActive(QString);

	public:
		virtual QColor color() const
		{
			return c;
		};

	public slots:
		virtual void setColor( const QColor newColor )
		{
			if( c == newColor )
				return;

			c = newColor;
			emit colorChanged( newColor );
			update();
		};

	public:
		virtual void createPlugin(QObject *parent, QString idParent,plugin::PluginsManager *manager)
		{
			connect( this, SIGNAL( setActive( QString ) ), getBeep(), SIGNAL( setActive( QString ) ) );
			manager->addPlugins(this, "ColorEditor");
		}

		virtual QString getName()const
		{
			return "ColorEditor";
		}
				ColorEditor( plugin::PluginsManager *manager )
				{
					QPixmap pm( 20, 20 );
					QPainter pmp( &pm );

					pmp.fillRect( 0, 0, 10, 10, Qt::lightGray );
					pmp.fillRect( 10, 10, 10, 10, Qt::lightGray );
					pmp.fillRect( 0, 10, 10, 10, Qt::darkGray );
					pmp.fillRect( 10, 0, 10, 10, Qt::darkGray );
					pmp.end();

					QPalette pal = palette();
					pal.setBrush( backgroundRole(), QBrush( pm ) );

					setAutoFillBackground( true );
					setPalette( pal );

					setMinimumSize( QSize( 24, 24 ) );

					//connect( this, SIGNAL( setActive( QString ) ), getBeep(), SIGNAL( setActive( QString ) ) );
				};

		virtual ~ColorEditor()
		{
		};

	protected:
		virtual void paintEvent( QPaintEvent *event )
		{
			QPainter p( this );

			QRect r = rect();

			QGradient g = QLinearGradient( QPointF( 0, 0 ), QPointF( width(), 0 ) );
			g.setColorAt( 0, QColor( c.red(), c.green(), c.blue(), 0 ) );
			g.setColorAt( 1, QColor( c.red(), c.green(), c.blue(), 255 ) );
			p.fillRect( 4, 4, width() - 8, height() - 8, g );

			qreal pos = 4 + qreal( c.alpha() ) * ( width() - 9 ) / 255.0;
			p.setPen( QPen( QColor( 220, 220, 220, 180 ), 5 ) );
			p.drawLine( (int)(pos), 5, (int)(pos), height() - 6 );

			p.drawRect( 0, 0, width() - 1, height() - 1 );
			qDrawShadePanel( &p, 1, 1, width() - 2, height() - 2, palette(), true, 3 );
		};

		virtual void mousePressEvent( QMouseEvent *event )
		{
			emit setActive(getName());
			if( event->button() == Qt::LeftButton )
			{
				qreal alpha = qreal( event->pos().x() - 4 ) * 255.0 / qreal( width() - 9 );
				if( alpha < 0.0 ) alpha = 0.0;
				if( alpha > 255.0 ) alpha = 255.0;
				c.setAlpha( (int)(alpha));
				emit colorChanged( c );
				update();
			}
			else if( event->button() == Qt::RightButton )
			{
				QColor newColor = QColorDialog::getColor( color(), this );
				if( newColor.isValid() )
				{
					newColor.setAlpha( color().alpha() );
					setColor( newColor );
				}
			}
		};

		virtual void mouseMoveEvent( QMouseEvent *event )
		{
			emit setActive(getName());
			qreal alpha = qreal( event->pos().x() - 4 ) * 255.0 / qreal( width() - 9 );
			if( alpha < 0.0 ) alpha = 0.0;
			if( alpha > 255.0 ) alpha = 255.0;
			c.setAlpha((int)(alpha));
			emit colorChanged( c );
			update();
		};

	private:
		QColor c;
};

#endif /* __COLOR_EDITOR_H__ */
