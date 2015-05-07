#ifndef BrushEditor_H
#define BrushEditor_H

#include <QObject>
#include <QBrush>
#include <QVBoxLayout>

#include "./../../../pluginTool/Plugin.h"
#include "./../../../pluginTool/InterfacePlugin.h"
#include "./../../../interfaces/BrushEditorInterface.h"
#include "./../../../interfaces/ColorEditorInterface.h"
#include "./../../../interfaces/GradientEditorInterface.h"

#include "TexturesList.h"
#include "ui_brusheditor.h"

#define PATH_TO_TEXTURES "./textures"


class BrushBox:public QWidget
{
	Q_OBJECT

	public:
				 BrushBox( QWidget *parent, QBrush &brush ):QWidget( parent ), _brush( brush )
				 {
					QPixmap pm( 20, 20 );

					QPainter pmp( &pm );
						pmp.fillRect( 0, 0, 10, 10, Qt::lightGray );
						pmp.fillRect(10, 10, 10, 10, Qt::lightGray );
						pmp.fillRect( 0, 10, 10, 10, Qt::darkGray );
						pmp.fillRect( 10, 0, 10, 10, Qt::darkGray );
					pmp.end();

					QPalette pal = palette();
					pal.setBrush( backgroundRole(), QBrush( pm ) );
					setAutoFillBackground( true );
					setPalette( pal );

					setMinimumSize( QSize( 64, 192 ) );
				 }

		virtual ~BrushBox()
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

			p.setPen( Qt::NoPen );
			p.fillRect( 3, 3, width() - 6, height() - 6, prepareBrushToPaint( _brush, rect() ) );
		}

	private:
		QBrush &_brush;
};

class BrushEditor:public BrushEditorInterface, public InterfacePlugin
{
	Q_OBJECT
	Q_INTERFACES( BrushEditorInterface )
	Q_INTERFACES( InterfacePlugin )

	signals:
		void brushChanged( const QBrush &brush );
		void setActive(QString name);

	public:
		virtual QBrush brush() const
		{
			return b;
		}

	public slots:
		virtual void setBrush( const QBrush &brush )
		{
			if( brush == b )
				return;

			b = brush;
			syncWithBrush();
			updateBrushViews();
		}

	public:
		virtual void createPlugin(QObject *parent, QString idParent,plugin::PluginsManager *manager)
		{
			colorEditor = COLOREDITOR( manager->copy("ColorEditor"));
			gradientEditor = GRADIENTEDITOR( manager->copy("GradientEditor"));


			if(colorEditor != 0)
			{

				colorLayout->addWidget( colorEditor );
				colorEditor->setParent( ui.colorBox );
				connect( colorEditor, SIGNAL( colorChanged( const QColor ) ),
								this, SLOT( onColorBoxColorChanged( const QColor ) ) );
				//connect(colorLayout, SIGNAL (clicked(bool)), this, SIGNAL(setActive(colorEditor->getName())));
			}

			if(gradientEditor != 0)
			{

				gradientEditor->setParent( ( ui.gradientBox ) );
				gradientLayout->addWidget( gradientEditor );
				connect( gradientEditor , SIGNAL( gradientChanged( const QGradient& ) ),
						this, SLOT( onGradientChanged() ) );
				//connect(gradientEditor, SIGNAL (clicked(bool)), this, SIGNAL(setActive(gradientEditor->getName())));
			}

			connect (this, SIGNAL (setActive(QString)), getBeep(), SIGNAL (setActive(QString)));

			manager->addPlugins(this, "BrushEditor");
		}

		virtual QString getName()const
		{
			return "BrushEditor";
		}

		BrushEditor(plugin::PluginsManager *manager ):
				colorEditor( 0 ),brushBox( this, b ),
				texturesList( PATH_TO_TEXTURES )
		{
			updateViews = true;
			ui.setupUi( this );
			colorLayout = new QVBoxLayout( ui.colorBox );
			gradientLayout = new QVBoxLayout( ui.gradientBox );

			QVBoxLayout *vboxLayout2 = new QVBoxLayout( ui.PreviewBox );
			brushBox.setParent( ui.PreviewBox );
			vboxLayout2->addWidget( &brushBox );

			QVBoxLayout *vboxLayout4 = new QVBoxLayout( ui.textureBrushTab );
			texturesList.setParent( ( ui.textureBrushTab ) );
			vboxLayout4->addWidget( &texturesList );

			connect( ui.dense4PatternButton, SIGNAL( clicked( bool ) ),
						this, SLOT( onDense4PatternButton( bool ) ) );
			connect( ui.dense1PatternButton, SIGNAL( clicked( bool ) ),
						this, SLOT( onDense1PatternButton( bool ) ) );
			connect( ui.noBrushButton, SIGNAL( clicked( bool ) ),
						this, SLOT( onNoBrushButton( bool ) ) );
			connect( ui.dense5PatternButton, SIGNAL( clicked( bool ) ),
						this, SLOT( onDense5PatternButton( bool ) ) );
			connect( ui.FDiagPatternButton, SIGNAL( clicked( bool ) ),
						this, SLOT( onFDiagPatternButton( bool ) ) );
			connect( ui.solidPatternButton, SIGNAL( clicked( bool ) ),
						this, SLOT( onSolidPatternButton( bool ) ) );
			connect( ui.dense2PatternButton, SIGNAL( clicked( bool ) ),
						this, SLOT( onDense2PatternButton( bool ) ) );
			connect( ui.dense6PatternButton, SIGNAL( clicked( bool ) ),
						this, SLOT( onDense6PatternButton( bool ) ) );
			connect( ui.dense7PatternButton, SIGNAL( clicked( bool ) ),
						this, SLOT( onDense7PatternButton( bool ) ) );
			connect( ui.VerPatternButton, SIGNAL( clicked( bool ) ),
						this, SLOT( onVerPatternButton( bool ) ) );
			connect( ui.CrossPatternButton, SIGNAL( clicked( bool ) ),
						this, SLOT( onCrossPatternButton( bool ) ) );
			connect( ui.BDiagPatternButton, SIGNAL( clicked( bool ) ),
						this, SLOT( onBDiagPatternButton( bool ) ) );
			connect( ui.dense3PatternButton, SIGNAL( clicked( bool ) ),
						this, SLOT( onDense3PatternButton( bool ) ) );
			connect( ui.HorPatternButton, SIGNAL( clicked( bool ) ),
						this, SLOT( onHorPatternButton( bool ) ) );
			connect( ui.DiagCrossPatternButton, SIGNAL( clicked( bool ) ),
						this, SLOT( onDiagCrossPatternButton( bool ) ) );
			connect( &texturesList, SIGNAL( pixmapChanged( const QPixmap &, const QString & ) ),
						this, SLOT( onPixmapChanged( const QPixmap &, const QString & ) ) );

			connect( ui.BrushPropsTab, SIGNAL( currentChanged( int ) ),
						this, SLOT( onTabChanged( int ) ) );

			syncWithBrush();
		}

		virtual ~BrushEditor()
		{
		}

	protected slots:

		virtual void onColorBoxColorChanged( const QColor newColor )
		{
			b.setColor( newColor );
			updateBrushViews();
		}

		virtual void onDense4PatternButton( bool checked )
		{
			if( !checked ) return;
			emit setActive(getName());

			b.setStyle( Qt::Dense4Pattern );
			updateBrushViews();
		}

		virtual void onDense1PatternButton( bool checked )
		{
			if( !checked ) return;
			emit setActive(getName());

			b.setStyle( Qt::Dense1Pattern );
			updateBrushViews();
		}

		virtual void onNoBrushButton( bool checked )
		{
			if( !checked ) return;
			emit setActive(getName());

			b.setStyle( Qt::NoBrush );
			updateBrushViews();
		}

		virtual void onDense5PatternButton( bool checked )
		{
			if( !checked ) return;
			emit setActive(getName());

			b.setStyle( Qt::Dense5Pattern );
			updateBrushViews();
		}

		virtual void onFDiagPatternButton( bool checked )
		{
			if( !checked ) return;
			emit setActive(getName());

			b.setStyle( Qt::FDiagPattern );
			updateBrushViews();
		}

		virtual void onSolidPatternButton( bool checked )
		{
			if( !checked ) return;
			emit setActive(getName());

			b.setStyle( Qt::SolidPattern );
			updateBrushViews();
		}

		virtual void onDense2PatternButton( bool checked )
		{
			if( !checked ) return;
			emit setActive(getName());

			b.setStyle( Qt::Dense2Pattern );
			updateBrushViews();
		}

		virtual void onDense6PatternButton( bool checked )
		{
			if( !checked ) return;
			emit setActive(getName());

			b.setStyle( Qt::Dense6Pattern );
			updateBrushViews();
		}

		virtual void onDense7PatternButton( bool checked )
		{
			if( !checked ) return;
			emit setActive(getName());

			b.setStyle( Qt::Dense7Pattern );
			updateBrushViews();
		}

		virtual void onVerPatternButton( bool checked )
		{
			if( !checked ) return;
			emit setActive(getName());

			b.setStyle( Qt::VerPattern );
			updateBrushViews();
		}

		virtual void onCrossPatternButton( bool checked )
		{
			if( !checked ) return;
			emit setActive(getName());

			b.setStyle( Qt::CrossPattern );
			updateBrushViews();
		}

		virtual void onBDiagPatternButton( bool checked )
		{
			if( !checked ) return;
			emit setActive(getName());

			b.setStyle( Qt::BDiagPattern );
			updateBrushViews();
		}

		virtual void onDense3PatternButton( bool checked )
		{
			if( !checked ) return;
			emit setActive(getName());

			b.setStyle( Qt::Dense3Pattern );
			updateBrushViews();
		}

		virtual void onHorPatternButton( bool checked )
		{
			if( !checked ) return;
			emit setActive(getName());

			b.setStyle( Qt::HorPattern );
			updateBrushViews();
		}

		virtual void onDiagCrossPatternButton( bool checked )
		{
			if( !checked ) return;
			emit setActive(getName());

			b.setStyle( Qt::DiagCrossPattern );
			updateBrushViews();
		}

		virtual void onGradientChanged()
		{
			b = gradientEditor->gradient();
			updateBrushViews();
		}

		virtual void onPixmapChanged( const QPixmap &p, const QString &fileName )
		{
			b = p;
			pixmapName = fileName;
			updateBrushViews();
		}

		virtual void onTabChanged( int index )
		{
			if( !updateViews ) return;
			Qt::BrushStyle style;
			switch( index )
			{

				case 0:
					if( ui.dense4PatternButton->isChecked() )
						style = Qt::Dense4Pattern;
					else if( ui.dense1PatternButton->isChecked() )
						style = Qt::Dense1Pattern;
					else if( ui.noBrushButton->isChecked() )
						style = Qt::NoBrush;
					else if( ui.dense5PatternButton->isChecked() )
						style = Qt::Dense5Pattern;
					else if( ui.FDiagPatternButton->isChecked() )
						style = Qt::FDiagPattern;
					else if( ui.solidPatternButton->isChecked() )
						style = Qt::SolidPattern;
					else if( ui.dense2PatternButton->isChecked() )
						style = Qt::Dense2Pattern;
					else if( ui.dense6PatternButton->isChecked() )
						style = Qt::Dense6Pattern;
					else if( ui.dense7PatternButton->isChecked() )
						style = Qt::Dense7Pattern;
					else if( ui.VerPatternButton->isChecked() )
						style = Qt::VerPattern;
					else if( ui.CrossPatternButton->isChecked() )
						style = Qt::CrossPattern;
					else if( ui.BDiagPatternButton->isChecked() )
						style = Qt::BDiagPattern;
					else if( ui.dense3PatternButton->isChecked() )
						style = Qt::Dense3Pattern;
					else if( ui.HorPatternButton->isChecked() )
						style = Qt::HorPattern;
					else if( ui.DiagCrossPatternButton->isChecked() )
						style = Qt::DiagCrossPattern;

					if(colorEditor!=0)
					{
						b = QBrush( colorEditor->color(), style );
					}

					emit setActive(getName());

					break;

				case 1:
					if(gradientEditor!=0)
					{
						b = gradientEditor->gradient();
					}
					break;

				case 2:
					//texturesList.selectItem( "" );
					pixmapName = texturesList.selectedName();
					b = texturesList.selectedPixmap();
					break;
			}

			updateBrushViews();
		}

	private:
		QBrush b;
		QVBoxLayout *colorLayout;
		QVBoxLayout *gradientLayout;
		GradientEditorInterface *gradientEditor;
		ColorEditorInterface *colorEditor;
		BrushBox brushBox;
		TexturesList texturesList;
		Ui::BrushEditorForm ui;
		QString pixmapName;

		bool updateViews;

		void syncWithBrush()
		{
			switch( b.style() )
			{
				case Qt::NoBrush:
				case Qt::SolidPattern:
				case Qt::Dense1Pattern:
				case Qt::Dense2Pattern:
				case Qt::Dense3Pattern:
				case Qt::Dense4Pattern:
				case Qt::Dense5Pattern:
				case Qt::Dense6Pattern:
				case Qt::Dense7Pattern:
				case Qt::HorPattern:
				case Qt::VerPattern:
				case Qt::CrossPattern:
				case Qt::BDiagPattern:
				case Qt::FDiagPattern:
				case Qt::DiagCrossPattern:
					updateViews = false;
					ui.BrushPropsTab->setCurrentIndex( 0 );
					if(colorEditor!=0)
					{
						colorEditor->setColor( b.color() );
					}
					switch( b.style() )
					{
						case Qt::NoBrush:
							ui.noBrushButton->setChecked( true );
							break;

						case Qt::SolidPattern:
							ui.solidPatternButton->setChecked( true );
							break;

						case Qt::Dense1Pattern:
							ui.dense1PatternButton->setChecked( true );
							break;

						case Qt::Dense2Pattern:
							ui.dense2PatternButton->setChecked( true );
							break;

						case Qt::Dense3Pattern:
							ui.dense3PatternButton->setChecked( true );
							break;

						case Qt::Dense4Pattern:
							ui.dense4PatternButton->setChecked( true );
							break;

						case Qt::Dense5Pattern:
							ui.dense5PatternButton->setChecked( true );
							break;

						case Qt::Dense6Pattern:
							ui.dense6PatternButton->setChecked( true );
							break;

						case Qt::Dense7Pattern:
							ui.dense7PatternButton->setChecked( true );
							break;

						case Qt::HorPattern:
							ui.HorPatternButton->setChecked( true );
							break;

						case Qt::VerPattern:
							ui.VerPatternButton->setChecked( true );
							break;

						case Qt::CrossPattern:
							ui.CrossPatternButton->setChecked( true );
							break;

						case Qt::BDiagPattern:
							ui.BDiagPatternButton->setChecked( true );
							break;

						case Qt::FDiagPattern:
							ui.FDiagPatternButton->setChecked( true );
							break;
					}
					updateViews = true;
					break;

				case Qt::LinearGradientPattern:
				case Qt::ConicalGradientPattern:
				case Qt::RadialGradientPattern:
					{
						updateViews = false;
						ui.BrushPropsTab->setCurrentIndex( 1 );
						const QGradient *g = b.gradient();
						if( g != 0 && gradientEditor!=0)
							gradientEditor->setGradient( QGradient( *g ) );
						updateViews = true;
					}
					break;

				case Qt::TexturePattern:
					updateViews = false;
					ui.BrushPropsTab->setCurrentIndex( 2 );
					texturesList.selectItem( pixmapName,
						qAlpha( b.texture().toImage().pixel( 0, 0 ) ) );
					updateViews = true;
					break;
			}
		}

		void updateBrushViews()
		{
			if( updateViews )
			{
				brushBox.update();
				emit brushChanged( b );
			}
		}
};

#endif
