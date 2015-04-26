#include "PaintWidget.h"

#include <QDialog>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QDebug>

const static QString SIGNATURE = "SACS2_GCORE_1.0";
const static QString MIMETYPE = "sacs2/object";

PaintWidget::PaintWidget( QWidget *parent, plugin::PluginsManager *manager):
	PaintWidgetInterface(parent), painter(manager, this)
{
    setMouseTracking(true);
    mainWin = MAINWINDOW(parent);
    connect(&painter, SIGNAL(mouseMoveEvent(QPoint,QPoint)), mainWin, SLOT(onRPWMouseMove(QPoint,QPoint)));


	setWidget( &painter );
	setAlignment( Qt::AlignCenter );
    setViewportColor( QColor( 100, 100, 100 ) );


    isCreatedPWE = false;

    manager->addPlugins(this, "PaintWidget");

	
	connect( &painter, SIGNAL( objectCreated() ),
			this, SIGNAL( objectCreated() ) );

	connect( &painter, SIGNAL( frameChanged( qreal ) ),
			this, SIGNAL( frameChanged( qreal ) ) );

	connect( &painter, SIGNAL( undoEvents() ),
			this, SIGNAL( undoEvents() ) );

	connect( &painter, SIGNAL( isFrame( bool) ),
		this, SIGNAL( isFrame(bool) ) );

	connect( &painter, SIGNAL( figureSelected( int, int ) ),
			this, SIGNAL( figureSelected( int, int ) ) );
}

PaintWidget::~PaintWidget()
{
	delete painter.background;
}

void PaintWidget::mouseMoveEvent(QMouseEvent *event)
{
    emit mouseMoveEvent(event->pos());
}

void PaintWidget::mySetViewportMargins(int left, int top, int right, int bottom)
{
    setViewportMargins(left,top,right,bottom);
}

void PaintWidget::setHideFigures(int layer, bool value)
{
	painter.layers[layer]->setHideFigures(value);
}

bool PaintWidget::getHideFigures(int layer)
{
	return painter.layers[layer]->getHideFigures();
}


int PaintWidget::FigureIsSpline()
{
	if(painter.selection.countSelected() == 1)
	{
		GContainer* c = GCONTAINER(painter.selection.getSelected());
		if(c!=0)
		{
			GObject* ob =c->object(0);
			if(!ob->isContainer())
			{
				GVectorFigure* v = GVECTORFIGURE(ob);
				return (v->isSpline())?1:0;
			}
		}
	}
	return -1;
}

void PaintWidget::FigureSetStyle(bool spline)
{
	if(painter.selection.countSelected() == 1)
	{
		GContainer* c = GCONTAINER(painter.selection.getSelected());
		if(c!=0)
		{
			GObject* ob =c->object(0);
			if(!ob->isContainer())
			{

				GVectorFigure* v = GVECTORFIGURE(ob);
				if(spline)
					v->toSpline();
				else
					v->toNormal();

				painter.update();
			}
		}
	}
}

void PaintWidget::FigureSetShowBezier(bool value)
{
	if(painter.selection.countSelected() == 1)
	{
		GContainer* c = GCONTAINER(painter.selection.getSelected());
		if(c!=0)
		{
			GObject* ob =c->object(0);
			if(!ob->isContainer())
			{
				GVectorFigure* v =GVECTORFIGURE(ob);
				v->setShowBezier(value);
				painter.update();
			}
		}
	}
}

int PaintWidget::getFigureHideLines()
{
	if(painter.selection.countSelected() == 1)
	{
		GContainer* c = GCONTAINER(painter.selection.getSelected());
		if(c!=0)
		{
			GObject* ob =c->object(0);
			if(!ob->isContainer())
			{
				GVectorFigure* v = GVECTORFIGURE(ob);
				return v->getHideLines();
			}
		}
	}
	return -1;
}

void PaintWidget::setFigureHideLines(int value)
{
	if(painter.selection.countSelected() == 1)
	{
		GContainer* c = GCONTAINER(painter.selection.getSelected());
		if(c!=0)
		{
			GObject* ob =c->object(0);
			if(!ob->isContainer())
			{
				GVectorFigure* v =GVECTORFIGURE(ob);
				v->setHideLines(value);
				painter.update();
			}
		}
	}
}

int PaintWidget::FigureIsShowBezier()
{
	if(painter.selection.countSelected() == 1)
	{
		GContainer* c = GCONTAINER(painter.selection.getSelected());
		if(c!=0)
		{
			GObject* ob =c->object(0);
			if(!ob->isContainer())
			{

				GVectorFigure* v = GVECTORFIGURE(ob);
				return (v->isShowBezier())?1:0;
			}
		}
	}
	return -1;
}

int PaintWidget::maxFrame()
{
	int maxFrame=0;
	int count = painter.layers.size();
	for(int i=0; i < count; i++)
	{
		if(maxFrame < painter.layers[i]->maxFrame())
			maxFrame = painter.layers[i]->maxFrame();
	}

	return maxFrame;
}

bool PaintWidget::isContainsFrame(int layer)
{
	return painter.layers[layer]->isContainsFrame(painter.currentFrame);
}

bool PaintWidget::objectIsContainer(int layer, int object)
{
	return painter.layers[layer]->objectIsContainer(object);
}

void PaintWidget::setFrame( qreal frame, bool played )
{
	painter.setFrame( frame, played );
}

int PaintWidget::frame() const
{
	return painter.currentFrame;
}

int PaintWidget::layer() const
{
	return painter.currentLayer;
}

int PaintWidget::countFrames() const
{
	return painter.layers[painter.currentLayer]->countFramesForLayer();
}

// масштабирование
void PaintWidget::scale( qreal s )
{
	QPointF center;
	if( !painter.fixedSize )
		center = QPointF( painter.width() / 2, painter.height() / 2 );
	else
		center = QPointF( painter.size.width() / 2, painter.size.height() / 2 );

	for(int i = 0; i<painter.layers.size(); i++ )
	{
		painter.layers[i]->scale( s, s, center );
	}
    //painter.resize(painter.width()*s,painter.height()*s);
	painter.selection.reset();

	painter.update();
	emit StateChanged("Scale viewport");
}

bool PaintWidget::canGroup()
{
	return ( painter.selection.countSelected() > 1 );
}

bool PaintWidget::canUngroup()
{
	if( painter.selection.countSelected() != 1 )
		return false;

	return painter.selection.selected( 0 )->isContainer();
}

void PaintWidget::group()
{
	if( !canGroup() ) return;

	GContainer *c = new GContainer;
	c->QObject::setObjectName( "New group" );

	int count = painter.layers[painter.currentLayer]->countObjects();
	for( int i = count - 1; i >= 0; i-- )
	{
		if( painter.selection.isInside( painter.layers[painter.currentLayer]->object( i ) ) )
			c->add( painter.layers[painter.currentLayer]->object( i ), true );
	}

	count = painter.selection.countSelected();
	for( int i = 0; i < count; i++ )
	{
		painter.layers[painter.currentLayer]->remove(
				painter.layers[painter.currentLayer]->objectIndex(
					GOBJECT(painter.selection.selected( i )) ) );
	}

	painter.selection.setSelected( c );
	painter.layers[painter.currentLayer]->add( c, true );
	emit allLayersChanged();
	painter.update();
	emit StateChanged("Group");
}

void PaintWidget::ungroup()
{
	if( !canUngroup() ) return;

	GContainer *c = GCONTAINER( painter.selection.selected( 0 ) );

	painter.selection.reset();
	painter.layers[painter.currentLayer]->remove( painter.layers[painter.currentLayer]->objectIndex( c ) );

	QVector< GObject* > ungrouped = c->removeAll();
	int countUngrouped = ungrouped.size();

	for( int i = countUngrouped - 1; i >= 0; i-- )
	{
		painter.layers[painter.currentLayer]->add( GOBJECT(ungrouped[ i ]), true );
		painter.selection.addSelected( ungrouped[ i ] );
	}

	delete c;
	emit allLayersChanged();
	painter.update();
	emit StateChanged("Ungroup");
}

QPen PaintWidget::pen()
{
	return painter.selection.pen();
}

QBrush PaintWidget::brush()
{
	return painter.selection.brush();
}

void PaintWidget::setPen( const QPen &p )
{
	if(painter.selection.countSelected() > 0)
	{
		painter.selection.setPen( p );
		painter.update();
	}
}

void PaintWidget::setBrush( const QBrush &b )
{
	if(painter.selection.countSelected() > 0)
	{
		painter.selection.setBrush( b );
		painter.update();

	}
}

bool PaintWidget::isClosed() const
{
	return painter.selection.isClosed();
}

void PaintWidget::setClosed( bool closed )
{
	if(painter.selection.countSelected() > 0)
	{
		painter.selection.setClosed( closed );
		painter.update();
	}
}

int PaintWidget::getPositionFrame(int i)
{
	return painter.layers[painter.currentLayer]->getPositionFrame(i);
}

/**
*  Перемещает фигуру между слоями и внутри слоя по уровням
*/
void PaintWidget::moveFigure( int layerOld, int objOld, int layerNew, int objNew )
{

	painter.layers[layerOld]->move(objOld, objNew);
	selectFigure(layerNew, objNew);
	emit movedObject();
	painter.update();
	emit StateChanged("Move figure");
}

/**
*  Перемещает слой по уровням
*/
void PaintWidget::moveLayer(int layerOld, int layerNew)
{

	GLayer* l = painter.layers[layerOld];
	painter.layers.remove(layerOld);
	painter.layers.insert(layerNew, l);

	selectFigure(layerNew, -1);
	emit movedObject();
	painter.update();
	emit StateChanged("Move layer");
}

int PaintWidget::countFigures(int layer) const
{
	return painter.layers[layer]->countObjects();
}

QString PaintWidget::figureName( int layer, int object  ) const
{
	return painter.layers[layer]->objectName(object);
}

void PaintWidget::renameFigure( int layer, int obj,  const QString &name )
{
	if(layer!=-1 && obj!=-1)
	{
		painter.layers[layer]->setObjectName( obj, name );
		emit StateChanged("Rename figure");
	}
}

bool PaintWidget::isFigureTransformed(  int layer, int object  ) const
{
	return painter.layers[layer]->objectIsTransformed(object, painter.currentFrame);
}

void PaintWidget::setFigureTransformed( int layer, int obj, bool trns )
{
	painter.layers[layer]->setObjectTransformed( obj,painter.currentFrame, trns );
}

bool PaintWidget::isFigureVisible(  int layer, int object  ) const
{
	return painter.layers[layer]->isObjectVisible(object);
}

void PaintWidget::setFigureVisible( int layer, int obj, bool visible )
{
	painter.layers[layer]->setObjectVisible( obj, visible );

	if(visible)
		selectFigure( layer, obj );
	else
		selectFigure( layer, -1 );

	painter.update();
}

bool PaintWidget::isFigureBlocked(  int layer, int object  ) const
{
	return painter.layers[layer]->isObjectBlocked(object);
}

void PaintWidget::setFigureBlocked( int layer, int obj, bool blocked )
{
	painter.layers[layer]->setObjectBlocked( obj, blocked );

	if(!blocked)
		selectFigure( layer, obj );
	else
		selectFigure( layer, -1 );

	painter.update();
}

void PaintWidget::deleteFigure(int layer, int obj )
{
	if(obj == -1)
	{
		if(painter.layers.size()!=1)
		{
			int countObj = painter.layers[layer]->countObjects();
			for(int i=0; i<countObj; i++)
			{
				delete painter.layers[layer]->remove( i );
			}
			delete painter.layers[layer];
			painter.layers.remove(layer);
			selectFigure( 0, -1 );
		}
	}
	else{
		delete painter.layers[layer]->remove( obj );
		selectFigure( layer, -1 );
		emit StateChanged("Delete");
	}
	emit allLayersChanged();
}

void PaintWidget::selectFigure( int layer, int obj)
{
	if( painter.inKeyPressedHandler ) return;

	GObjectInterface *o = painter.layers[layer]->object( obj );

	if(layer != painter.currentLayer)
	{
		painter.currentLayer = layer;
		painter.setFrame( painter.currentFrame, false );
		emit layerSelected();
	}

	if( o == 0 )
	{
		painter.selection.reset();
		emit figureSelected( layer, -1 );
		return;
	}


	if( !o->isVisible() || o->isBlocked())
	{
		painter.selection.reset();
		painter.update();
		return;
	}

	painter.selection.setSelected( o );
	painter.update();
	emit figureSelected( layer, obj );
}

int PaintWidget::countLayers() const
{
	return painter.layers.size();
}

void PaintWidget::setLayer(int layer)
{
	painter.currentLayer = layer;
}

QString PaintWidget::layerName( int layer  )
{
	return painter.layers[layer]->objectName();
}

void PaintWidget::renameLayer( int layer, const QString &name )
{
	painter.layers[layer]->setObjectName(name);
}

void PaintWidget::addLayer(bool visible, bool blocked, const QString &name)
{
	painter.addLayer(visible, blocked, name);
	painter.setFrame( painter.currentFrame, false );
	emit allLayersChanged();
}

int PaintWidget::selectedFigure()
{
	if( painter.selection.countSelected() != 1 )
		return -1;

	return painter.layers[painter.currentLayer]->objectIndex( GOBJECT(painter.selection.selected( 0 )));
}

int PaintWidget::selectedLayer()
{
	return painter.currentLayer;
}

bool PaintWidget::isLayerVisible( int layer ) const
{
	return painter.layers[layer]->isVisible();
}

bool PaintWidget::isLayerBlocked( int layer ) const
{
	return painter.layers[layer]->isBlocked();
}

void PaintWidget::setLayerBlocked( int layer, bool blocked )
{
	painter.layers[layer]->setBlocked(blocked);
	selectFigure( layer, -1 );
	painter.update();
}

void PaintWidget::setLayerVisible( int layer, bool visible )
{
	painter.layers[layer]->setVisible(visible);
	selectFigure( layer, -1 );
	painter.update();
}

void PaintWidget::addFrame(int position)
{
	painter.layers[painter.currentLayer]->addFrameForLayer( position, false );
	painter.setFrame( position, false );
	emit frameChanged(qreal(position));
	emit StateChanged("Add frame");
}

void PaintWidget::deleteFrame(int position)
{
	if(position == 0)
	{
		return;
	}

	painter.selection.reset();
	painter.layers[painter.currentLayer]->deleteFrameForLayer(position);
	painter.setFrame( position, false );
	painter.update();
	emit countFramesChanged( countFrames() );
	emit frameChanged( frame() );
	emit StateChanged("Delete frame");
}

void PaintWidget::setSelectionTool()
{
	painter.currentTool = 0;
	painter.selection.enableAddPointMode( false );
}

void PaintWidget::setAddPointTool()
{
	painter.currentTool = 0;
	painter.selection.enableAddPointMode( true );
}

void PaintWidget::setTool( QObject *tool )
{
	painter.currentTool = FIGURETOOL(tool);
}

void PaintWidget::paintFrameTo( QPainter &to, const QRect &r, qreal frame )
{
	painter.paintFrameTo( to, r, frame );
}

/*void PaintWidget::paintWholeFrameTo( QPainter &to, const QRect &r, qreal frame )
{
	painter.paintWholeFrameTo( to, r, frame );
}*/

bool PaintWidget::reset()
{
    setFrame(0.0, false);
	if( !painter.isEnabled() ) return false;

	setViewportColor( QColor( 100, 100, 100 ) );

	BACKGROUND( painter.background )->reset();

    //setViewportType( fixedViewport );
    setViewportType( hintViewport );
	setViewportFixedSize( QSize( 640, 480 ) );

	for(int i=0; i<painter.layers.size(); i++)
	{
		while( painter.layers[i]->countObjects() > 0 )
				delete painter.layers[i]->remove( 0 );
		delete painter.layers[i];
	}
	painter.layers.clear();

	painter.addLayer(true, false, tr("Layer ") + "0");
	painter.selection.reset();
	painter.inKeyPressedHandler = false;
	painter.inSelectionMode = false;
	update();

	emit figureSelected( 0, -1);
	emit allLayersChanged();
	emit countFramesChanged( countFrames() );

	return true;
}

void PaintWidget::save( QDataStream &stream )
{
	if( !painter.isEnabled() ) return;

	stream << SIGNATURE;

	stream << transparent;
	stream << viewportColor;

	BACKGROUND( painter.background )->save( stream );
	stream << ( int ) viewportType();
	stream << painter.size;
	stream << painter.layers.size();
	//QMessageBox::warning(this ,"dsad",QString::number(painter.layers.size()));
	for(int i=0; i<painter.layers.size(); i++)
	{
		stream <<  painter.layers[i]->objectName();
		painter.layers[i]->saveLayer( stream );
	}
}

bool PaintWidget::load( QDataStream &stream )
{
	if( !painter.isEnabled() ) return false;


	for(int i=0; i<painter.layers.size(); i++)
	{
		while( painter.layers[i]->countObjects() > 0 )
				delete painter.layers[i]->remove( 0 );
		delete painter.layers[i];
	}

	painter.layers.clear();
	QString signature;
	stream >> signature;

	if( signature != SIGNATURE )
		return false;

	stream >> transparent;
	stream >> viewportColor;

	if( !transparent )
		setViewportColor( viewportColor );
	else
		doViewportTransparent();

	BACKGROUND( painter.background )->load( stream );

	int type;
	stream >> type;
	setViewportType( ( ViewportType ) type );

	stream >> painter.size;
	setViewportFixedSize( painter.size );
	int countLayer;
	stream >> countLayer;

	for(int i=0; i<countLayer; i++)
	{
			QString name;
			stream >> name;
			painter.addLayer(true, false, name);
			painter.layers[i]->loadLayer( stream );
	}

	painter.selection.reset();
	painter.inKeyPressedHandler = false;
	painter.inSelectionMode = false;
	update();

	emit figureSelected( 0 , -1);
	emit allLayersChanged();
	emit countFramesChanged( countFrames() );

    setFrame(0.0, false);

	return true;
}

bool PaintWidget::canDeleteSelected() const
{
	return ( painter.selection.countSelected() > 0 );
}

void PaintWidget::deleteSelected()
{
	if( !canDeleteSelected() )
		return;

	int count = painter.selection.countSelected();

	for( int i = 0; i < count; i++ )
	{
		delete painter.layers[painter.currentLayer]->remove(
				painter.layers[painter.currentLayer]->objectIndex(
						GOBJECT(painter.selection.selected( i ))) );
	}

	painter.selection.reset();
	update();

	emit figureSelected( painter.currentLayer , -1);
	emit allLayersChanged();
	emit countFramesChanged( countFrames() );
	emit StateChanged("Delete");
}

bool PaintWidget::canCopyOrCutToClipboard() const
{
	return ( painter.selection.countSelected() > 0 );
}

bool PaintWidget::canPasteFromClipboard() const
{
	if( QApplication::clipboard()->mimeData() == 0 )
		return false;

	return QApplication::clipboard()->mimeData()->hasFormat( MIMETYPE );
}

void PaintWidget::cutToClipboard()
{
	if( !canCopyOrCutToClipboard() )
		return;

	copyToClipboard();
	deleteSelected();
	emit StateChanged("Cut");
}

void PaintWidget::copyToClipboard()
{
	if( !canCopyOrCutToClipboard() )
		return;

	GContainer c;
	c.setObjectName( "clipboard object" );

	int count = painter.layers[painter.currentLayer]->countObjects();
	for( int i = count - 1; i >= 0; i-- )
	{
		if( painter.selection.isInside( painter.layers[painter.currentLayer]->object( i ) ) )
			c.add( painter.layers[painter.currentLayer]->object( i ) );
	}

	QByteArray data;
	QDataStream stream( &data, QIODevice::WriteOnly );
	GObject::saveObject( stream, &c );

	QMimeData *mime = new QMimeData();
	mime->setData( MIMETYPE, data );

	QApplication::clipboard()->setMimeData( mime );
}

void PaintWidget::pasteFromClipboard()
{
	if( !canPasteFromClipboard() )
		return;

	const QMimeData *mime = QApplication::clipboard()->mimeData();
	if( mime == 0 )
		return;

	QByteArray data = mime->data( MIMETYPE );
	if( data.size() <= 0 ) return;

	QDataStream stream( data );
	GObjectInterface *object = GObject::loadObject( stream );
	if( object == 0 ) return;

	GContainer *o = qobject_cast< GContainer* >( object );
	if( ( o == 0 ) || ( o->countObjects() == 0 ) )
	{
		delete object;
		return;
	}

	int frameObj = (int)(o->frame());

	o->deleteFrames(frameObj);
	o->addFrame(painter.currentFrame, true);
	o->deleteFrames(painter.currentFrame);

	QVector <int> frames = painter.layers[painter.currentLayer]->getFrames();
	int countFrames = frames.size();

	for( int i = 0; i < countFrames; i++ )
	{
		if( frames[i] != painter.currentFrame )
			o->addFrame(frames[i], false);
	}

	o->cloneFrameToAll(painter.currentFrame);

	for( int i = 0; i < countFrames; i++ )
	{
		if( painter.layers[painter.currentLayer]->getPositionFrame(i) != painter.currentFrame )
		{
			o->setFrame( painter.layers[painter.currentLayer]->getPositionFrame(i) );
			o->setVisible( false );
			o->setBlocked( false );
		}
		else
		{
			o->setFrame( painter.layers[painter.currentLayer]->getPositionFrame(i) );
			o->setVisible( true );
			o->setBlocked( false );
		}
	}
	o->setFrame( painter.currentFrame );

	int count = o->countObjects();
	for( int i = count - 1; i >= 0; i-- )
		painter.layers[painter.currentLayer]->add( o->object( i ), true );

	painter.selection.reset();
	update();

	emit figureSelected( painter.currentLayer , -1);
	emit allLayersChanged();
	emit StateChanged("Paste");
}

PaintWidget::ViewportType PaintWidget::viewportType() const
{
	if( !widgetResizable() )
		return fixedViewport;

	if( painter.fixedSize )
		return hintViewport;

	return resizableViewport;
}

void PaintWidget::setViewportType( const PaintWidget::ViewportType t )
{
	switch( t )
	{
		case fixedViewport:
            painter.fixedSize = false;
            setWidgetResizable( false );
			painter.resize( painter.size );
			break;

		case resizableViewport:
            painter.fixedSize = false;
			setWidgetResizable( true );
			break;

		case hintViewport:
			painter.fixedSize = true;
			setWidgetResizable( true );
			break;
	}

	setAlignment( Qt::AlignCenter );
	painter.update();
	update();
}

QSize PaintWidget::viewportFixedSize() const
{
	return painter.size;
}

void PaintWidget::setViewportFixedSize( const QSize &s )
{
	painter.size = s;

	if( viewportType() == fixedViewport )
		painter.resize( painter.size );

	setAlignment( Qt::AlignCenter );
	painter.update();
	update();
}

void PaintWidget::doViewportTransparent()
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

	transparent = true;
	update();
}

void PaintWidget::setViewportColor( const QColor &c )
{
	QPalette pal = palette();
	pal.setBrush( backgroundRole(), c );
	setAutoFillBackground( true );
	setPalette( pal );

	transparent = false;
	viewportColor = c;
	update();
}

void PaintWidget::setContextMenu(QMenu *qMenu)//задание контектстного меню
{
	painter.setContextMenu(qMenu);
}

QMenu* PaintWidget::getContextMenu()//получение контектстного меню
{
	return painter.getContextMenu();
}

void PaintWidget::updateAllViews( QWidget *from )
{
	emit backgroundChanged( from );
}

void PaintWidget::showConfig()
{

    if (!isCreatedPWE)
    {
        dialog = new QDialog( this );
        e = new PaintWidgetEditor( *this );
        e->setParent( dialog );
        isCreatedPWE = true;

        connect( this, SIGNAL( destroyed() ), e, SLOT( destroy() ) );
        connect( this, SIGNAL( backgroundChanged( QWidget* ) ), e, SLOT( onBackgroundChanged( QWidget* ) ) );

        buttons = new QDialogButtonBox( QDialogButtonBox::Close, Qt::Horizontal, dialog );

        QVBoxLayout *l = new QVBoxLayout( dialog );
        l->addWidget( e );
        l->addWidget( buttons );

        connect( buttons, SIGNAL( rejected() ), dialog, SLOT( reject() ) );
        dialog->setWindowTitle( tr( "Viewport properties" ) );

        dialog->exec();
    }
    else{
        dialog->exec();
    }
}

QObject* PaintWidget::getRealPaintWidget()
{
	return &painter;
}


void PaintWidget::rotate90CW()
{
    painter.selection.setRotateAngle(90);
}

void PaintWidget::rotate90CWW()
{
    painter.selection.setRotateAngle(-90);
}

void PaintWidget::flipHorisontal()
{
    painter.selection.setScale(-1,1);
}

void PaintWidget::flipVertical()
{
    painter.selection.setScale(1,-1);
}

/*virtual QColor& getViewportColor();*/
