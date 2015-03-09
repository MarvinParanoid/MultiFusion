#ifndef __TEXT_H__
#define __TEXT_H__

#include <QApplication>
#include <QInputDialog>
#include <QPainterPath>
#include <QFontDialog>
#include <QLineEdit>
#include <QMatrix>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QVBoxLayout>
#include "./../../../pluginTool/Plugin.h"
#include "./../../../pluginTool/InterfacePlugin.h"
#include "./../../../interfaces/ToolBoxInterface.h"
#include "./../../../interfaces/FigureToolInterface.h"

#include <QToolButton>
#include <QWidget>
#include <QObject>
#include "./../../../interfaces/ToolButtonInterface.h"

class ToolButton:public ToolButtonInterface
{
	Q_OBJECT
	Q_INTERFACES( ToolButtonInterface )

	signals:
		void toolSelected( FigureToolInterface *tool );

	public:
		QObject* getTool()
		{
			return tool;
		}

		void setTool(QObject* t)
		{
			tool = FIGURETOOL(t);
		}

		ToolButton( FigureToolInterface *_tool, QWidget *parent ):
			ToolButtonInterface( parent ), tool( _tool )
		{
			setIcon( tool->icon() );
			setToolTip( tool->description() );

			connect( this, SIGNAL( clicked( bool ) ),
					this, SLOT( onClicked( bool ) ) );
		}

		virtual ~ToolButton()
		{
			delete tool;
		}

	private slots:
		void onClicked( bool checked )
		{
			if( !checked ) return;

			tool->toolSelected();
			emit toolSelected( tool );
		}

	private:
		FigureToolInterface *tool;
};

class I_Text:public FigureToolInterface, public InterfacePlugin
{
	Q_OBJECT
	Q_INTERFACES( FigureToolInterface )
	Q_INTERFACES( InterfacePlugin )
	signals:
		void setActive(QString);

	public:

		virtual void createPlugin(QObject *parent, QString idParent,
				plugin::PluginsManager *manager)
		{
			ToolBoxInterface* tool = TOOLBOX(parent);
			if(tool!=0)
			{
				button = new ToolButton( FIGURETOOL(this), tool);

				QGridLayout* gridLayout= tool->getFigureLayout();
				button->setAutoExclusive( true );
				button->setCheckable( true );
				button->setAutoRaise(true);
				tool->connect( button, SIGNAL( toolSelected( FigureToolInterface* ) ),
						tool, SIGNAL( toolSelected( FigureToolInterface* ) ) );
				gridLayout->addWidget( button, 5, 1, Qt::AlignTop);

			}
			connect( this, SIGNAL( setActive( QString ) ), getBeep(), SIGNAL( setActive( QString ) ) );
			manager->addPlugins(this, getName());
		}

		virtual QWidget* getWidget() {
			return button;
		}

		virtual QString getName()const
		{
			return "Text";
		}

		virtual QIcon icon()
		{
			return QIcon( ":/text/images/text.png" );
		}

		virtual CreateStyle createStyle() const
		{
			return createAndResize;
		}

		virtual FiguresInfo figure() const
		{
			FigureInfo info;
			FiguresInfo figure;

			QMatrix m;
			m.scale( 1.0, 1.0 );

			int countChars = text.size();

			QRectF rect;
			QPointF pos = QPointF( 0.0, 0.0 );

			for( int i = 0; i < countChars; i++ )
			{
				QPainterPath path;
				path.addText( pos, font, text[i] );

				info.closed = true;
				info.spline = false;

				if( text[i] == QChar( ' ' ) )
				{
					info.points.clear();
					info.points << QPointF( pos.x(), pos.y() ) <<
						QPointF( pos.x() + rect.normalized().width(), pos.y() ) <<
						QPointF( pos.x() + rect.normalized().width(), pos.y() -
								rect.normalized().height() ) <<
						QPointF( pos.x(), pos.y() - rect.normalized().height() );
				}
				else
					info.points = path.toFillPolygon( m );

				info.name = figureName() + ':' + text[i];

				if( info.points.size() < 2 ) continue;

				rect = info.points.boundingRect();
				pos.setX( pos.x() + rect.normalized().width() );

				figure << info;
			}

			countChars = figure.size();
			rect = QRectF();
			for( int i = 0; i < countChars; i++ )
				rect |= figure[i].points.boundingRect();

			QPointF p = - rect.normalized().bottomRight();
			for( int i = 0; i < countChars; i++ )
				figure[i].points.translate( p );

			for( int i = 0; i < countChars; i++ )
			{
				int countPoints = figure[i].points.size();
				int step = 1;
				for(int j=0; j<countPoints; j+=step)
				{
					if(j==0 || j==countPoints-1)
					{
						figure[i].points.insert( j, figure[i].points[j]);
						countPoints++;
						step=2;
					}
					else
					{
						figure[i].points.insert( j, figure[i].points[j]);
						figure[i].points.insert( j, figure[i].points[j]);
						countPoints+=2;
						step=3;
					}

				}
			}

			return figure;
		}

		virtual QString description() const
		{
			return tr( "Vector text" );
		}

		virtual QString figureName() const
		{
			return tr( "Text" );
		}

		virtual void toolSelected()
		{
			emit setActive(getName());

			bool ok = true;
			QString value;
			QFont newFont;

			while( ( value.size() <= 0 ) && ok )
			{
				value = QInputDialog::getText( QApplication::activeWindow(),
							tr( "Input text" ), tr( "Text:" ),
							QLineEdit::Normal,
							text, &ok  );
			}

			if( !ok ) return;

			ok = false;
			newFont = QFontDialog::getFont( &ok, font,
						QApplication::activeWindow(), tr( "Choose font" ) );

			text = value.simplified();

			if( !ok ) return;

			font = newFont;
		}

				I_Text( const plugin::PluginsManager *manager ):text( "Text" )
				{
					if( !resourcesInited )
					{
						Q_INIT_RESOURCE( Text );
						resourcesInited = true;
					}

				}

		virtual ~I_Text()
		{
		}

	private:
		QString text;
		QFont font;
		ToolButton *button;
		static bool resourcesInited;
};

#endif /* __TEXT_H__ */
