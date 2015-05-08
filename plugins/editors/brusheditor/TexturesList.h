#ifndef __TEXTURES_LIST__
#define __TEXTURES_LIST__

#include <QDir>
#include <QPixmap>
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QListWidgetItem>
#include "qdrawutil.h"

#include "ui_texturesform.h"

class AlphaEditor:public QWidget
{
	Q_OBJECT

	signals:
		void alphaChanged( const int a );

	public:
		virtual int alpha() const
		{
			return _alpha;
		};

		virtual void setAlpha( const int a )
		{
			if( _alpha == a )
				return;

			_alpha = a;
			emit alphaChanged( _alpha );
			update();
		};

				 AlphaEditor( QWidget *parent = 0 ):QWidget( parent ), _alpha( 255 )
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
				 };

		virtual ~AlphaEditor()
		{
		};

	protected:
		virtual void paintEvent( QPaintEvent *event )
		{
			QPainter p( this );

            //QRect r = rect();

			QGradient g = QLinearGradient( QPointF( 0, 0 ), QPointF( width(), 0 ) );
			g.setColorAt( 0, QColor( 0, 0, 0, 0 ) );
			g.setColorAt( 1, QColor( 0, 0, 0, 255 ) );
			p.fillRect( 4, 4, width() - 8, height() - 8, g );

			qreal pos = 4 + qreal( _alpha ) * ( width() - 9 ) / 255.0;
			p.setPen( QPen( QColor( 220, 220, 220, 180 ), 5 ) );
			p.drawLine( pos, 5, pos, height() - 6 );

			p.drawRect( 0, 0, width() - 1, height() - 1 );
			qDrawShadePanel( &p, 1, 1, width() - 2, height() - 2, palette(), true, 3 );
		};

		virtual void mousePressEvent( QMouseEvent *event )
		{
			if( event->button() == Qt::LeftButton )
			{
			    _alpha = qreal( event->pos().x() - 4 ) * 255.0 / qreal( width() - 9 );
			    if( _alpha < 0 ) _alpha = 0;
			    if( _alpha > 255 ) _alpha = 255;
			    emit alphaChanged( _alpha );
			    update();
			}
		};

		virtual void mouseMoveEvent( QMouseEvent *event )
		{
			_alpha = qreal( event->pos().x() - 4 ) * 255.0 / qreal( width() - 9 );
			if( _alpha < 0 ) _alpha = 0;
			if( _alpha > 255 ) _alpha = 255;
			emit alphaChanged( _alpha );
			update();
		};

	private:
		int _alpha;
};

class TexturesList:public QWidget
{
	Q_OBJECT

	signals:
		void pixmapChanged( const QPixmap &p, const QString &fileName );

	public:
		virtual QString selectedName()
		{
			QList< QListWidgetItem* > items = ui.texturesList->selectedItems();

			if( items.size() <= 0 )
				return QString();

			return items[0]->text();
		}

		virtual QPixmap selectedPixmap()
		{
			QList< QListWidgetItem* > items = ui.texturesList->selectedItems();

			if( items.size() <= 0 )
				return QString();

			return preparePixmap( items[0]->icon().pixmap( items[0]->icon().actualSize( QSize( 10000, 10000 ) ) ) );
		}

		virtual void selectItem( const QString &fileName, const int a )
		{
			QList< QListWidgetItem* > items =
				ui.texturesList->findItems( fileName, Qt::MatchFixedString );

			if( items.size() <= 0 )
				return;

			ui.texturesList->setCurrentItem( items[0] );
			handleSignals = false;
			alpha.setAlpha( a );
			handleSignals = true;
		}

		virtual void setTexturesPath( const QString &path )
		{
			ui.texturesList->clear();
			QStringList files = QDir( path ).entryList( QDir::Files );
			int countFiles = files.size();

            //int item = 0;
			for( int i = 0; i < countFiles; i++ )
			{
				QPixmap img( path + "/" + files[i] );

				if( !img.isNull() )
				{
					QListWidgetItem *item = new QListWidgetItem( img, files[i] );
					ui.texturesList->addItem( item );
					if( item == 0 )
					{
						item->setSelected( true );
						emit pixmapChanged(
							item->icon().pixmap( item->icon().actualSize( QSize( 10000, 10000 ) ) ),
							item->text() );
					}
					item++;
				}
				ui.texturesList->setCurrentRow( 0 );
			}
		}

				TexturesList( const QString &pathToImages, QWidget *parent = 0 ):
					QWidget( parent ), handleSignals( true )
				{
					ui.setupUi( this );
					ui.texturesList->setWrapping( true );

					alpha.setParent( ui.texturesList );
					ui.vboxLayout->addWidget( &alpha );

					connect( ui.texturesList, SIGNAL( currentItemChanged ( QListWidgetItem*, QListWidgetItem* ) ),
							 this, SLOT( onTextureActivated( QListWidgetItem* ) ) );

					connect( &alpha, SIGNAL( alphaChanged( const int ) ),
							 this, SLOT( onAlphaChanged( const int ) ) );

					setTexturesPath( pathToImages );
				}

		virtual ~TexturesList()
		{
		}

	private:
		Ui::Form ui;
		AlphaEditor alpha;
		bool handleSignals;

		QPixmap preparePixmap( const QPixmap &pixmap )
		{
			int a = alpha.alpha();
			QPixmap p = pixmap;
			QPixmap alpha( p.size() );
            alpha.fill( QColor( a, a, a ) );
            setAlphaChannel(p,a);
			return p;
        }

        void setAlphaChannel(QPixmap& pixmap, int const alpha)
        {
          QImage image(pixmap.toImage().convertToFormat(QImage::Format_ARGB32));

          for (int x(0); x != image.width(); ++x)
          {
            for (int y(0); y != image.height(); ++y)
            {
              QColor color(image.pixel(x,y));

              color.setAlpha(alpha);

              image.setPixel(x, y, color.rgba());
            }
          }

          pixmap = QPixmap::fromImage(image);
        }

	private slots:
		virtual void onTextureActivated( QListWidgetItem *item )
		{
			if( !handleSignals ) return;

			emit pixmapChanged( preparePixmap(
				item->icon().pixmap( item->icon().actualSize( QSize( 10000, 10000 ) ) ) ),
				item->text() );
		}

		virtual void onAlphaChanged( const int a )
		{
			if( !handleSignals ) return;

			emit pixmapChanged( selectedPixmap(), selectedName() );
		}
};

#endif /* __TEXTURES_LIST__ */
