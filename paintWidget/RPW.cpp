#include "RPW.h"
#include "GPropertiesObj.h"
#include "UndoStructure.h"

RealPaintWidget::RealPaintWidget( plugin::PluginsManager *manager, QWidget *parent ):
    RPWInterface( parent ), background( 0 ),
    fixedSize( false ), size( 640, 480 ),
    selection( manager, this, QRect( QPoint( 0, 0 ), size ) ),
    inKeyPressedHandler( false ),
    currentTool( 0 ), inSelectionMode( false ), _manager(manager)
{
    GLayer* layer = new GLayer();
    layer->setVisible(true);
    layer->setObjectName(tr("Layer") + " 0");
    layers.append(layer);
    currentLayer = 0;
    currentFrame = 0;
    layers[currentLayer]->addFrameForLayer(currentFrame,false);
    emit figureSelected(currentLayer, -1);
    paintConMenu = new QMenu(this);
    propertiesAct = paintConMenu->addAction(tr("Properties..."));
    setAutoFillBackground( false );
    resize( size );

    connect( propertiesAct,SIGNAL( triggered( bool ) ), this, SLOT( onPropertiesObj() ) );
    connect( &selection, SIGNAL( changed() ), this, SLOT( update() ) );

    manager->addPlugins(this, "RealPaint");

    isMousePress = false;
    setMouseTracking(true);

}

void RealPaintWidget::paintEvent( QPaintEvent * event )
{
    QPainter p( this );
    QRect viewportRect( rect() );

    QWidget::paintEvent(event);

    if( fixedSize )
    {
        QPoint origin;
        QSize sz = viewportRect.size() - size;
        origin = QPoint( sz.width() / 2, sz.height() / 2 );
        p.translate( origin );
        viewportRect.setSize( size );
        emit paintEvent(origin);
    }
   // else
    {

    }

    p.setRenderHints( QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform );

    viewportRect.setWidth( viewportRect.width() - 1 );
    viewportRect.setHeight( viewportRect.height() - 1 );

    FILL( background, p, viewportRect );

    p.setPen( QColor( 0, 0, 0 ) );
    p.drawRect( viewportRect );

    for(int i = 0; i<layers.size(); i++)
    {
        layers[i]->paintLayer( p );
    }
    selection.paint( p );

    if( inSelectionMode )
    {
        QPen selectionRectPen( Qt::DashLine );
        selectionRectPen.setColor( QColor( 50, 50, 50, 200 ) );

        p.setBrush( QBrush() );
        p.setPen( selectionRectPen );

        p.drawRect( selectionRect.x(), selectionRect.y(),
            selectionRect.width() - 1, selectionRect.height() - 1 );
    }
}

void RealPaintWidget::mouseMoveEvent( QMouseEvent * event )
{

    QPoint pos = event->pos();
    if( fixedSize )
    {
        QSize sz = rect().size() - size;
        pos -= QPoint( sz.width() / 2, sz.height() / 2 );
    }

    //QPoint point(event->x(),event->y());
    //emit mouseMoveEvent(QWidget::mapToParent(point),pos);
    QWidget::mouseMoveEvent(event);

    if( inSelectionMode )
    {
        selectionRect.setBottomRight( pos );
        update();
        return;
    }

    if( ( currentTool != 0 ) && ( currentTool->createStyle() == FigureToolInterface::paint ) )
    {
        GObjectInterface *o = layers[currentLayer]->object( 0 );
        if( o == 0 ) return;
        QPolygonF points = o->points(currentFrame);
        if( points.size() > 0 )
        {
            QPointF p = pos - points[ points.size() - 1 ];
            if( ( fabs( p.x() ) < 15 ) && ( fabs( p.y() ) < 15 ) )
                return;
        }
        if (isMousePress)
            o->addPointToEnd( pos );
        update();
        return;
    }

    selection.mouseMove( event->button(), pos, event->modifiers() );
}

void RealPaintWidget::setContextMenu(QMenu *qMenu)//задание контектстного меню
{
	paintConMenu = qMenu;
}

QObject* RealPaintWidget::getUndo()
{
	UndoStructure* undo = new UndoStructure(this);
	return undo;
}

void RealPaintWidget::setFromUndo(QObject* undo)
{
	UndoStructure* properties = UNDO(undo);

	fixedSize = properties->fixedSize;
	size = properties->size;


	for(int i=0; i<layers.size(); i++)
		delete layers[i];

	layers.clear();

	//QMessageBox::warning(this,"Trace2",QString::number(properties->layers.size()));

	for(int i=0; i<properties->layers.size(); i++)
		layers.append(properties->layers[i]->copyLayer());

	//delete background;
	//background = properties->background;
	currentLayer = properties->currentLayer;
	currentFrame = properties->currentFrame;
	inKeyPressedHandler = properties->inKeyPressedHandler;
	inSelectionMode = properties->inSelectionMode;
	selectionRect = properties->selectionRect;
	selection.reset();
	emit figureSelected(0,-1);
	update();
	emit undoEvents();
}

QMenu* RealPaintWidget::getContextMenu()//получение контектстного меню
{
	return paintConMenu;
}

void RealPaintWidget::setFrame( qreal f , bool played)
{
	for(int i=0; i<layers.size(); i++)
		layers[i]->setCurrentFrameForLayer((int)(f),played);

	if(currentFrame != int(f) && !played)
	{
		currentFrame = int(f);
		emit frameChanged( qreal(currentFrame) );
		selection.reset();
	}

	update();

	if(played)
	{
		this->setEnabled(false);
		emit isPlayed();
		return;
	}

	if( !layers[currentLayer]->isContainsFrame( currentFrame ))
	{
		this->setEnabled(false);
		emit isFrame(false);
	}
	else
	{
		this->setEnabled(true);
		emit isFrame(true);
	}
}

qreal RealPaintWidget::frame() const
{
	return layers[currentLayer]->frame();
}

int RealPaintWidget::countFrames() const
{
	return layers[currentLayer]->countFrames();
}

void RealPaintWidget::paintFrameTo( QPainter &to, const QRect &r, qreal frame )
{
	qreal currentFrame = layers[currentLayer]->frame();
	layers[currentLayer]->setFrame( frame );
	QRect viewportRect( rect() );

	if( fixedSize )
	{
		QSize sz = viewportRect.size() - size;
		viewportRect.setSize( size );
	}

	to.setRenderHints( QPainter::Antialiasing |
						QPainter::TextAntialiasing |
						QPainter::SmoothPixmapTransform );

	to.save();
	to.scale( qreal( r.width() ) / qreal( viewportRect.width() ),
				qreal( r.height() ) / qreal( viewportRect.height() ) );

	to.translate( r.x(), r.y() );

	viewportRect.setWidth( viewportRect.width() - 1 );
	viewportRect.setHeight( viewportRect.height() - 1 );

	//FILL( background, to, viewportRect );

	to.setPen( QColor( 0, 0, 0 ) );
	to.drawRect( viewportRect );
	layers[currentLayer]->paint( to );

	layers[currentLayer]->setFrame( currentFrame);
	to.restore();
}

void RealPaintWidget::addLayer(bool visible, bool blocked, const QString &name)
{
	GLayer* layer = new GLayer();
	layer->setObjectName(name);
	layer->setVisible(visible);
	layer->setBlocked(blocked);
	layer->addFrameForLayer(0, false);
	layers.append(layer);
	currentLayer = layers.size()-1;

	selection.reset();
	emit figureSelected(currentLayer, -1);
	update();
	emit StateChanged(tr("Add Layer"));
}

RealPaintWidget::~RealPaintWidget()
{
}

void RealPaintWidget::mousePressEvent( QMouseEvent *event )
{
	QPoint pos = event->pos();
	int button = event->button();

	if(button == Qt::LeftButton || selection.isInAddPointMode())
	{
		if( fixedSize )
		{
			QSize sz = rect().size() - size;
			pos -= QPoint( sz.width() / 2, sz.height() / 2 );
		}

		if( currentTool != 0 )
		{
            isMousePress = true;
			GObjectInterface * o = GObject::create( currentTool, pos,
								currentTool->figureName(), currentFrame);

			layers[currentLayer]->add( GOBJECT(o), true );

			selection.setSelected( o );
			selection.setIsNewFigure(true);

			emit objectCreated();
			emit figureSelected( currentLayer, layers[currentLayer]->objectIndex( GOBJECT(o) ) );

			if( currentTool->createStyle() != FigureToolInterface::paint )
			{
				selection.mousePress( event->button(), pos, event->modifiers() );
				selection.setCreateFigureMode( true );
			}
			else
				selection.reset();

			QVector <int> frames = layers[currentLayer]->getFrames();
			for( int i = 0; i < frames.size(); i++ )
			{
				if( frames[i] != currentFrame )
					o->addFrame(frames[i], false);
			}
			return;
		}

		if( selection.mousePress( event->button(), pos, event->modifiers() ) )
			return;

		if(button == Qt::RightButton && !selection.isInAddPointMode())
		{
			paintConMenu->exec(QWidget::mapToGlobal(pos),0);
			return;
		}

		GContainer * cont = GCONTAINER(selection.getSelected());
		if(button != Qt::RightButton && cont != 0 && selection.isInAddPointMode() && cont->countObjects()==1)
		{
			cont->addPointToEnd(pos);
			emit StateChanged("Add point");
			return;
		}

		GObjectInterface* o = 0;
		for(int i=0;i<layers.size();i++)
		{
			o = layers[i]->contains( pos );
			if(o != 0)
			{
				currentLayer = i;
				break;
			}
		}

		if( o == 0 )
		{
			selection.reset();
			emit figureSelected( currentLayer, -1 );

			inSelectionMode = true;
			selectionRect = QRect( pos, QSize( 0, 0 ) );
			update();
			return;
		}



		if( ( ( event->modifiers() & Qt::ControlModifier ) == 0 ) ||
			( selection.countSelected() <= 0 ) )
		{
			selection.setSelected( o );
			emit figureSelected(currentLayer, layers[currentLayer]->objectIndex( GOBJECT(o) ) );
		}
		else
		{
			selection.addSelected( o );
			inKeyPressedHandler = true;
			emit figureSelected( currentLayer, -1 );
			inKeyPressedHandler = false;

		}


		selection.mousePress( event->button(), pos, event->modifiers() );
	}

	if(button == Qt::RightButton && !(selection.isInAddPointMode()))
	{
		if(selection.countSelected() != 1)
			propertiesAct->setEnabled(false);
		else
			propertiesAct->setEnabled(true);
		paintConMenu->exec(QWidget::mapToGlobal(pos),0);
	}


}

void RealPaintWidget::mouseReleaseEvent( QMouseEvent * event )
{
	QPoint pos = event->pos();
	int button = event->button();

	if(button == Qt::RightButton)
	{
		return;
	}

	if( fixedSize )
	{
		QSize sz = rect().size() - size;
		pos -= QPoint( sz.width() / 2, sz.height() / 2 );
	}

	if( inSelectionMode )
	{
		inSelectionMode = false;
			int countObjects = layers[currentLayer]->countObjects();
			selection.reset();
			for( int i = 0; i < countObjects; i++ )
			{
				if( !layers[currentLayer]->object( i )->isVisible() || layers[currentLayer]->object( i )->isBlocked() )
					continue;

				if( layers[currentLayer]->object( i )->boundingRect().intersects( selectionRect ) )
					selection.addSelected( layers[currentLayer]->object( i ) );
			}
			if( selection.countSelected() != 1 )
			{
				//emit figureSelected( currentLayer, -1 );
				update();
				return;
			}
			else
				emit figureSelected(currentLayer, layers[currentLayer]->objectIndex( GOBJECT(selection.selected( 0 )) ) );
			update();
			return;
	}

	if( currentTool != 0 )
	{
		GObjectInterface *s = layers[currentLayer]->object( 0 );
		if( s != 0 )
			s->cloneFrameToAll(currentFrame);

		int countFrames = layers[currentLayer]->countFrames();
		for( int i = 0; i < countFrames; i++ )
		{
			if( layers[currentLayer]->getPositionFrame(i) != currentFrame )
			{
				s->setFrame( layers[currentLayer]->getPositionFrame(i) );
				s->setVisible( false );
				s->setBlocked( false );
			}
		}
		s->setFrame( currentFrame );

		//emit StateChanged("Create object");

        isMousePress = false;
		if( currentTool->createStyle() == FigureToolInterface::paint )
		{
			selection.setSelected( s );
			update();
			return;
		}
	}

	selection.mouseRelease( event->button(), pos, event->modifiers() );
}

void RealPaintWidget::mouseDoubleClickEvent( QMouseEvent * event )
{
	QPoint pos = event->pos();
	int button = event->button();
	if(button != Qt::RightButton)
	{
		if( fixedSize )
		{
			QSize sz = rect().size() - size;
			pos -= QPoint( sz.width() / 2, sz.height() / 2 );
		}

		selection.mouseDoubleClick( event->button(), pos, event->modifiers() );
	}
}

void RealPaintWidget::resizeEvent( QResizeEvent * event )
{
	QRect viewport = rect();

	if( fixedSize )
	{
		QSize sz = viewport.size() - size;
		viewport.translate( QPoint( -sz.width() / 2, -sz.height() / 2 ) );
	}
	selection.setViewport( viewport );
}


void RealPaintWidget::alphaChanged(int _alpha)
{
	GContainer* selCont = GCONTAINER(selection.getSelected());
	selCont->setAlpha(_alpha);
	update();
}

void RealPaintWidget::onPropertiesObj()
{
	GContainer* selCont = GCONTAINER(selection.getSelected());
	int _alpha = PropertiesObjDialog::changePropertiesObj( selCont,
					tr( "Properties of figure" ), this );
	selCont->setAlpha(_alpha);
	update();
}

QObject* RealPaintWidget::getBackground()
{
	return this->background;
}

void RealPaintWidget::setBackground(QObject* back)
{
	this->background = back;
}

bool RealPaintWidget::getFixedSize()
{
	return this->fixedSize;
}

void RealPaintWidget::setFixedSize(bool back)
{

}

QSize RealPaintWidget::getSize()
{
	return this->size;
}

void RealPaintWidget::setSize(QSize sz)
{

}

QObject* RealPaintWidget::getSelection()
{
	return &selection;
}

void RealPaintWidget::setSelection(QObject* s)
{

}


QObject* RealPaintWidget::getCurrentTool()
{
	return currentTool;
}

void RealPaintWidget::setCurrentTool(QObject* ct)
{

}

QAction* RealPaintWidget::getPropertiesAct()
{
	return this->propertiesAct;
}

void RealPaintWidget::setPropertiesAct(QAction* pa)
{

}

bool RealPaintWidget::getInKeyPressedHandler()
{
	return this->inKeyPressedHandler;
}

void RealPaintWidget::setInKeyPressedHandler(bool kph)
{
	this->inKeyPressedHandler = kph;
}

bool RealPaintWidget::getInSelectionMode()
{
	return this->inSelectionMode;
}

void RealPaintWidget::setInSelectionMode(bool sm)
{
	this->inSelectionMode = sm;
}

QRect RealPaintWidget::getSelectionRect()
{
	return this->selectionRect;
}

void RealPaintWidget::setSelectionRect(QRect sr)
{

}
