#ifndef __PEN_EDITOR_H__
#define __PEN_EDITOR_H__

#include <QPen>
#include <QWidget>
#include <QLineEdit>
#include <QPainter>

#include "./../../../pluginTool/Plugin.h"
#include "./../../../pluginTool/InterfacePlugin.h"
#include "./../../../interfaces/PenEditorInterface.h"
#include "./../../../interfaces/ColorEditorInterface.h"
#include "qdrawutil.h"

#include "ui_peneditor.h"

class PenBox:public QWidget
{
	Q_OBJECT

	public:
		 PenBox( QWidget *parent, QPen &pen ):QWidget( parent ), _pen( pen )
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

			setMinimumSize( QSize( 64, 64 ) );
		 }

		virtual ~PenBox()
		{
		}

	protected:
		virtual void paintEvent( QPaintEvent *event )
		{
			QPainter p( this );
            //QRect r = rect();

			p.setRenderHints( QPainter::Antialiasing |
							  QPainter::TextAntialiasing |
							  QPainter::SmoothPixmapTransform );

			qDrawShadePanel( &p, 0, 0, width(), height(), palette(), true, 3 );

			p.setPen( _pen );

			QPoint points[] =
			{
				QPoint( 20, 20 ),
				QPoint( width() - 20, height() - 20 ),
				QPoint( 20, height() / 2 ),
				QPoint( width() - 20, 20 ),
				QPoint( width() - 20, height() / 2 )
			};

			p.setClipRect( 3, 3, width() - 6, height() - 6 );
			p.drawPolyline( points, 5 );
		}

	private:
		QPen &_pen;
};

class PenEditor:public QWidget, public PenEditorInterface, public InterfacePlugin
{
	Q_OBJECT
	Q_INTERFACES( PenEditorInterface )
	Q_INTERFACES( InterfacePlugin )

	signals:
		void penChanged( const QPen &pen );
		void setActive(QString);

	public:
		virtual QPen pen() const
		{
			return p;
		}

	public slots:
		virtual void setPen( const QPen &_pen )
		{
			if( _pen == p )
				return;

			p = _pen;
			penBox.repaint();
			syncWithPen();
			emit penChanged( p );

			emit setActive(getName());
		}

	public:

		virtual void createPlugin(QObject *parent, QString idParent,plugin::PluginsManager *manager)
		{
			colorEditor = COLOREDITOR( manager->copy("ColorEditor"));
			if(colorEditor != 0)
			{
				QVBoxLayout *vboxLayout = new QVBoxLayout( ui.penColorBox );
				colorEditor->setParent( ui.penColorBox );
				vboxLayout->addWidget( colorEditor );
				connect( colorEditor, SIGNAL( colorChanged( const QColor ) ),
								this, SLOT( onColorBoxColorChanged( const QColor ) ) );
			}
			connect( this, SIGNAL( setActive( QString ) ), getBeep(), SIGNAL( setActive( QString ) ) );
			manager->addPlugins(this, "PenEditor");
		}

		virtual QString getName()const
		{
			return "PenEditor";
		}

				PenEditor( plugin::PluginsManager *manager ):
						colorEditor( 0 ), penBox( this, p )
				{
					ui.setupUi( this );
					ui.lineWidthCombo->lineEdit()->setText( "1" );
					QVBoxLayout *vboxLayout2 = new QVBoxLayout( ui.previewBox );
					penBox.setParent( ui.previewBox );
					vboxLayout2->addWidget( &penBox );

					connect( ui.noneLineStyleBtn, SIGNAL( clicked( bool ) ),
								this, SLOT( onNoneLineStyleChecked( bool ) ) );
					connect( ui.solidLineStyleBtn, SIGNAL( clicked( bool ) ),
								this, SLOT( onSolidLineStyleChecked( bool ) ) );
					connect( ui.dashLineStyleBtn, SIGNAL( clicked( bool ) ),
								this, SLOT( onDashLineStyleChecked( bool ) ) );

					connect( ui.dotLineStyleBtn, SIGNAL( clicked( bool ) ),
								this, SLOT( onDotLineStyleChecked( bool ) ) );
					connect( ui.dashDotLineStyleBtn, SIGNAL( clicked( bool ) ),
								this, SLOT( onDashDotLineStyleChecked( bool ) ) );
					connect( ui.dashDotDotLineStyleBtn, SIGNAL( clicked( bool ) ),
								this, SLOT( onDashDotDotLineStyleChecked( bool ) ) );

					connect( ui.squareCapStyleBtn, SIGNAL( clicked( bool ) ),
								this, SLOT( onSquareCapLineStyleChecked( bool ) ) );
					connect( ui.flatCapStyleBtn, SIGNAL( clicked( bool ) ),
								this, SLOT( onFlatCapLineStyleChecked( bool ) ) );
					connect( ui.roundCapStyleBtn, SIGNAL( clicked( bool ) ),
								this, SLOT( onRoundCapLineStyleChecked( bool ) ) );

					connect( ui.bevelJoinStyleBtn, SIGNAL( clicked( bool ) ),
								this, SLOT( onBevelJoinLineStyleChecked( bool ) ) );
					connect( ui.miterJoinStyleBtn, SIGNAL( clicked( bool ) ),
								this, SLOT( onMiterJoinLineStyleChecked( bool ) ) );
					connect( ui.roundJoinStyleBtn, SIGNAL( clicked( bool ) ),
								this, SLOT( onRoundJoinLineStyleChecked( bool ) ) );

					connect( ui.lineWidthCombo, SIGNAL( editTextChanged( const QString& ) ),
								this, SLOT( onLineWidthEdited( const QString& ) ) );

					connect( ui.lineWidthCombo, SIGNAL( activated( const QString& ) ),
								this, SLOT( onLineWidthChanged( const QString& ) ) );



					syncWithPen();
				}

		virtual ~PenEditor()
		{
		}

	protected slots:

		/*virtual void onPenChanged( PenEditorInt *exclude )
		{
  			 if( this != exclude )
				syncWithPen();
		}

		virtual void destroy()
		{
			delete this;
		}*/

		virtual void onColorBoxColorChanged( const QColor newColor )
		{
			p.setColor( newColor );
			penBox.repaint();
			emit penChanged( p );
			emit setActive(getName());
		}

		virtual void onLineWidthChanged( const QString &text )
		{
			emit setActive(getName());
			if( text.isNull() )
			{
				ui.lineWidthCombo->lineEdit()->setText( "1" );
				return;
			}
			bool isInt = false;
			text.toInt( &isInt );
			if( !isInt )
				ui.lineWidthCombo->lineEdit()->setText( "1" );
		}

		virtual void onLineWidthEdited( const QString &text )
		{
			if( text.isNull() ) return;

			bool isInt = false;
			int val = text.toInt( &isInt );
			if( !isInt )
				ui.lineWidthCombo->lineEdit()->setText( "1" );

			p.setWidth( val );
			penBox.repaint();
			emit penChanged( p );
			emit setActive(getName());
		}

		virtual void onNoneLineStyleChecked( bool checked )
		{
			if( !checked ) return;

			p.setStyle( Qt::NoPen );
			penBox.repaint();
			emit penChanged( p );
			emit setActive(getName());
		}

		virtual void onSolidLineStyleChecked( bool checked )
		{
			if( !checked ) return;

			p.setStyle( Qt::SolidLine );
			penBox.repaint();
			emit penChanged( p );
			emit setActive(getName());
		}

		virtual void onDashLineStyleChecked( bool checked )
		{
			if( !checked ) return;

			p.setStyle( Qt::DashLine );
			penBox.repaint();
			emit penChanged( p );
			emit setActive(getName());
		}

		virtual void onDotLineStyleChecked( bool checked )
		{
			if( !checked ) return;

			p.setStyle( Qt::DotLine );
			penBox.repaint();
			emit penChanged( p );
			emit setActive(getName());
		}

		virtual void onDashDotLineStyleChecked( bool checked )
		{
			if( !checked ) return;

			p.setStyle( Qt::DashDotLine );
			penBox.repaint();
			emit penChanged( p );
			emit setActive(getName());
		}

		virtual void onDashDotDotLineStyleChecked( bool checked )
		{
			if( !checked ) return;

			p.setStyle( Qt::DashDotDotLine );
			penBox.repaint();
			emit penChanged( p );
			emit setActive(getName());
		}

		virtual void onSquareCapLineStyleChecked( bool checked )
		{
			if( !checked ) return;

			p.setCapStyle( Qt::SquareCap );
			penBox.repaint();
			emit penChanged( p );
			emit setActive(getName());
		}

		virtual void onFlatCapLineStyleChecked( bool checked )
		{
			if( !checked ) return;

			p.setCapStyle( Qt::FlatCap );
			penBox.repaint();
			emit penChanged( p );
			emit setActive(getName());
		}

		virtual void onRoundCapLineStyleChecked( bool checked )
		{
			if( !checked ) return;

			p.setCapStyle( Qt::RoundCap );
			penBox.repaint();
			emit penChanged( p );
			emit setActive(getName());
		}

		virtual void onBevelJoinLineStyleChecked( bool checked )
		{
			if( !checked ) return;

			p.setJoinStyle( Qt::BevelJoin );
			penBox.repaint();
			emit penChanged( p );
			emit setActive(getName());
		}

		virtual void onMiterJoinLineStyleChecked( bool checked )
		{
			if( !checked ) return;

			p.setJoinStyle( Qt::MiterJoin );
			penBox.repaint();
			emit penChanged( p );
			emit setActive(getName());
		}

		virtual void onRoundJoinLineStyleChecked( bool checked )
		{
			if( !checked ) return;

			p.setJoinStyle( Qt::RoundJoin );
			penBox.repaint();
			emit penChanged( p );
			emit setActive(getName());
		}

	private:
		QPen p;
		ColorEditorInterface *colorEditor;
		PenBox penBox;
		Ui::PenEditorForm ui;

		void syncWithPen()
		{
			switch( p.style() )
			{
				case Qt::NoPen:
					ui.noneLineStyleBtn->setChecked( true );
					break;

				case Qt::SolidLine:
					ui.solidLineStyleBtn->setChecked( true );
					break;

				case Qt::DashLine:
					ui.dashLineStyleBtn->setChecked( true );
					break;

				case Qt::DotLine:
					ui.dotLineStyleBtn->setChecked( true );
					break;

				case Qt::DashDotLine:
					ui.dashDotLineStyleBtn->setChecked( true );
					break;

				case Qt::DashDotDotLine:
					ui.dashDotDotLineStyleBtn->setChecked( true );
					break;
				default:break;
			}

			switch( p.capStyle() )
			{
				case Qt::SquareCap:
					ui.squareCapStyleBtn->setChecked( true );
					break;

				case Qt::FlatCap:
					ui.flatCapStyleBtn->setChecked( true );
					break;

				case Qt::RoundCap:
					ui.roundCapStyleBtn->setChecked( true );
					break;
				default:break;
			}

			switch( p.joinStyle() )
			{
				case Qt::BevelJoin:
					ui.bevelJoinStyleBtn->setChecked( true );
					break;

				case Qt::MiterJoin:
					ui.miterJoinStyleBtn->setChecked( true );
					break;

				case Qt::RoundJoin:
					ui.roundJoinStyleBtn->setChecked( true );
					break;
				default:break;
			}

			int index = ui.lineWidthCombo->findText( QString( "%1" ).arg( p.width() ) );

			if( index < 0 )
			{
				ui.lineWidthCombo->addItem( QString( "%1" ).arg( p.width() ) );
				ui.lineWidthCombo->setCurrentIndex( ui.lineWidthCombo->count() - 1 );
			}
			else
				ui.lineWidthCombo->setCurrentIndex( index );

			//colorEditor->setColor( p.color() );
			penBox.repaint();
		}
};

#endif /* __PEN_EDITOR_H__ */
