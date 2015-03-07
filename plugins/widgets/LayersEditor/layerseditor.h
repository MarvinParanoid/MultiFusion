#ifndef __LAYERS_EDITOR_H__
#define __LAYERS_EDITOR_H__

#include <QInputDialog>
#include <QScrollArea>
#include <QToolButton>
#include <QMessageBox>
#include <QLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QWidget>
#include <QVector>
#include <QFrame>
#include <QtGui>
#include <QtGui/QLineEdit>

#include "./../../../pluginTool/Plugin.h"
#include "./../../../pluginTool/InterfacePlugin.h"
#include "./../../../interfaces/LayersEditorInterface.h"
#include "./../../../interfaces/MainWindowInterface.h"
#include "./../../../interfaces/PaintWidgetInterface.h"

class PlaceDrag:public QFrame
{
	public:
		PlaceDrag(QWidget *parent, int l, int o, QString type):QFrame(parent)
		{
			this->setObjectName(type);
			layer = l;
			object = o;
			this->setVisible(false);
			this->setFixedHeight(5);
			this->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
		}
		virtual int getLayerIndex()
		{
			return layer;
		}

		virtual int getObjectIndex()
		{
			return object;
		}

	private:
		int layer;
		int object;
};

class LineEditor:public QLineEdit
{
	Q_OBJECT
	signals:
		void changed();
		void onClick();
	public:
		LineEditor(QWidget* parent):QLineEdit(parent)
		{
			this->setReadOnly(true);
		};

		~LineEditor(){};

		virtual void mouseDoubleClickEvent( QMouseEvent *event )
		{
			this->setReadOnly(false);
		}

		virtual void mousePressEvent( QMouseEvent *event )
		{
			emit onClick();
		}

		virtual void focusOutEvent(QFocusEvent *event)
		{
			if(!this->isReadOnly())
			{
				this->setReadOnly(true);
				this->setBackgroundRole(QPalette::Button);
				emit changed();
			}
		}

};

class ItemMenu:public QFrame
{
	Q_OBJECT

	signals:
		void onClicked(int lr, int obj);
		void visibleChanged(bool visible);
		void blockedChanged(bool blocked);
		void transformChanged(bool trns);

	public:

		ItemMenu(QWidget* parent, int l, int o):QFrame(parent),visibleFrame(this),blockedFrame(this),
		ItemName(this),layer(l), object(o),transformFrame(0)
		{
			visible = true;
			blocked = false;
			visibleFrame.setFixedSize(24,24);
			blockedFrame.setFixedSize(24,24);
			visibleFrame.setMinimumSize(QSize(24, 24));
			blockedFrame.setMinimumSize(QSize(24, 24));

			blockedFrame.setBackgroundRole(QPalette::Window);
			blockedFrame.setAutoFillBackground(true);

			QPalette palette = visibleFrame.palette();
			palette.setBrush(QPalette::Window, QBrush(QPixmap(":/layerseditor/images/openEye.png")));
			visibleFrame.setPalette(palette);
			visibleFrame.setBackgroundRole(QPalette::Window);

			palette = blockedFrame.palette();
			palette.setBrush(QPalette::Window, QBrush(QPixmap(":/layerseditor/images/unlock.png")));
			blockedFrame.setPalette(palette);
			blockedFrame.setBackgroundRole(QPalette::Window);

			visibleFrame.setAutoFillBackground(true);
			visibleFrame.setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
			blockedFrame.setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
			ItemName.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

			visibleFrame.setFrameShape(QFrame::WinPanel);
			visibleFrame.setFrameShadow(QFrame::Sunken);

			blockedFrame.setFrameShape(QFrame::WinPanel);
			blockedFrame.setFrameShadow(QFrame::Sunken);

			connect(&ItemName,SIGNAL(onClick()),this, SLOT(onClick()));
			connect(this,SIGNAL(onClicked(int, int)),this->parentWidget(), SLOT(onSelect(int, int)));
			connect(this,SIGNAL(onClicked(int, int)),this->parentWidget()->parentWidget(), SLOT(onSelect(int, int)));

			ItemName.setFrame(false);

		};

		virtual ~ItemMenu(){};

		virtual QString getNameItem()
		{
			return ItemName.text();
		}

		virtual void setNameItem(QString value)
		{
			ItemName.setText(value);
		}

		virtual bool getBlocked()
		{
			return blocked;
		}

		virtual void setBlocked(bool value)
		{
			QPalette palette = blockedFrame.palette();
			blocked = value;
			if(blocked)
				palette.setBrush(QPalette::Window, QBrush(QPixmap(":/layerseditor/images/lock.png")));
			else
				palette.setBrush(QPalette::Window, QBrush(QPixmap(":/layerseditor/images/unlock.png")));

			blockedFrame.setPalette(palette);
		}

		virtual bool getVisibility()
		{
			return visible;
		}

		virtual void setVisibility(bool value)
		{
			QPalette palette = visibleFrame.palette();
			visible = value;
			if(visible)
				palette.setBrush(QPalette::Window, QBrush(QPixmap(":/layerseditor/images/openEye.png")));
			else
				palette.setBrush(QPalette::Window, QBrush(QPixmap(":/layerseditor/images/closedEye.png")));

			visibleFrame.setPalette(palette);
		}

		virtual bool getTransformed() const
		{
			return transformed;
		}

		virtual void setTransformed(bool value)
		{
			QPalette palette = transformFrame->palette();
			transformed = value;
			if(transformed)
				palette.setBrush(QPalette::Window, QBrush(QPixmap(":/layerseditor/images/process.png")));
			else
				palette.setBrush(QPalette::Window, QBrush(QPixmap(":/layerseditor/images/unprocess.png")));

			transformFrame->setPalette(palette);
		}

		virtual int getLayerIndex()
		{
			return layer;
		}

		virtual int getObjectIndex()
		{
			return object;
		}

	protected:

		QFrame visibleFrame;
		QFrame blockedFrame;
		LineEditor ItemName;	//имя элемента
		bool visible; 		//видимость элемента
		bool blocked; 	 	//блокировка элемента
		bool transformed;	//трансформирование элемента
		QString type;
		int layer;
		int object;
		QFrame* transformFrame;

		virtual void mousePressEvent( QMouseEvent *event )
		{
			QPoint p = event->pos();
			if(visibleFrame.rect().contains(visibleFrame.mapFromParent(p)))
			{
				visible = (!visible);
				setVisibility(visible);
				emit visibleChanged(visible);
				return;
			}

			if(blockedFrame.rect().contains(blockedFrame.mapFromParent(p)))
			{
				blocked = (!blocked);
				setBlocked(blocked);
				emit blockedChanged(blocked);
				return;
			}
			if(transformFrame!=0)
			{
				if(transformFrame->rect().contains(transformFrame->mapFromParent(p)))
				{
					transformed = (!transformed);
					setTransformed(transformed);
					emit transformChanged(transformed);
					return;
				}
			}
		}

	public slots:
		void onClick()
		{
			emit onClicked(layer, object);
		}

};

class ObjectItem:public ItemMenu
{
	Q_OBJECT

	signals:
		void renamedFigure( ObjectItem* index, const QString &name );
		void visibleFigureChanged( ObjectItem* index, const bool &visible );
		void transformedFigureChanged( int layer, int object, const bool &transformed );
		void blockedFigureChanged( ObjectItem* index, const bool &blocked );

	public slots:
		void renameFigure()
		{
			emit renamedFigure(this, ItemName.text());
		}

		void visibleFigure(bool visible)
		{
			emit visibleFigureChanged( this, visible );
		}

		void blockedFigure(bool blocked)
		{
			emit blockedFigureChanged( this, blocked );
		}

		void transformFigure(bool trns)
		{
			emit transformedFigureChanged( layer, object, trns );
		}

	public:

		QFrame* getFrameTool()
		{
			return frameTool;
		}


		ObjectItem(QWidget* parent, int l, int o, bool canTransformed, bool transformed):ItemMenu(parent,l,o)
		{
			this->setObjectName("Figure");
			frameTool = new QFrame(this);
			QVBoxLayout* mainVLayout = new QVBoxLayout(this);
			QHBoxLayout* mainHLayout = new QHBoxLayout();
			QHBoxLayout* HLayout = new QHBoxLayout(frameTool);

			mainVLayout->addLayout(mainHLayout);
			mainHLayout->setSizeConstraint(QLayout::SetFixedSize);

			this->setMinimumWidth(100);
			this->setFrameShape(QFrame::StyledPanel);
			this->setFrameShadow(QFrame::Plain);

			frameTool->setFrameShape(QFrame::WinPanel);
			frameTool->setFrameShadow(QFrame::Raised);

			mainVLayout->setSpacing(5);
			mainVLayout->setContentsMargins(0,0,5,0);

			HLayout->setMargin(5);
			HLayout->addWidget(&visibleFrame);
			HLayout->addWidget(&blockedFrame);

			if(canTransformed)
			{
				transformFrame = new QFrame(this);
				transformFrame->setFixedSize(24,24);
				transformFrame->setMinimumSize(QSize(24, 24));
				transformFrame->setBackgroundRole(QPalette::Window);
				transformFrame->setAutoFillBackground(true);
				transformFrame->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
				transformFrame->setFrameShape(QFrame::WinPanel);
				transformFrame->setFrameShadow(QFrame::Sunken);
				setTransformed(transformed);
				HLayout->addWidget(transformFrame);

				connect( this , SIGNAL( transformChanged(bool) ),this,
						SLOT( transformFigure( bool) ) );
				connect( this, SIGNAL( transformedFigureChanged(int,int,bool)),
						parent->parentWidget(), SIGNAL( transformedFigure(int,int,bool ) ) );
			}

			mainHLayout->addWidget(frameTool);
			mainHLayout->addWidget(&ItemName);

			connect( &ItemName, SIGNAL( changed() ),this,
					SLOT( renameFigure() ) );
			connect( this, SIGNAL( renamedFigure(ObjectItem*,QString) ),
					parent->parentWidget(), SLOT( renameFigure(ObjectItem*,QString ) ) );
			connect( this , SIGNAL( visibleChanged(bool) ),this,
					SLOT( visibleFigure( bool) ) );
			connect( this, SIGNAL( visibleFigureChanged(ObjectItem*,bool)),
					parent->parentWidget(), SLOT( visibleFigureChanged(ObjectItem*,bool ) ) );
			connect( this , SIGNAL( blockedChanged(bool) ),this,
					SLOT( blockedFigure( bool) ) );
			connect( this, SIGNAL( blockedFigureChanged(ObjectItem*,bool)),
					parent->parentWidget(), SLOT( blockedFigureChanged(ObjectItem*,bool ) ) );

		}

		virtual ~ObjectItem(){};

	private:
		QFrame* frameTool;
};

class LayerItem:public ItemMenu
{
	Q_OBJECT

	public slots:

		void renameLayer()
		{
			emit renamedLayer(this, ItemName.text());
		}

		void visibleLayer(bool visible)
		{
			emit visibleLayerChanged( this, visible );
		}

		void blockedLayer(bool blocked)
		{
			emit blockedLayerChanged( this, blocked );
		}

		void setVisFigures()
		{
			hideFigures = !hideFigures;

			if(!hideFigures)
				downButton->setArrowType(Qt::DownArrow);
			else
				downButton->setArrowType(Qt::UpArrow);

			for(int i=0; i<objectItems.size(); i++)
			{
				objectItems[i]->setVisible(!hideFigures);
			}
			emit hideFigureChanged(layer,hideFigures);
		}
	signals:

		void layerSelected( int index );
		void movedFirst( int index );
		void movedUp( int index );
		void movedDown( int index );
		void movedLast( int index );
		void deleted( int index );
		void hideFigureChanged(int layer, bool value);
		void renamedLayer( LayerItem* index, const QString &name );
		void visibleLayerChanged( LayerItem* index, const bool &visible );
		void blockedLayerChanged( LayerItem* index, const bool &blocked );

	public:

		void setHideFigures(bool value)
		{
			hideFigures = value;

			if(!hideFigures)
				downButton->setArrowType(Qt::DownArrow);
			else
				downButton->setArrowType(Qt::UpArrow);

			for(int i=0; i<objectItems.size(); i++)
			{
				objectItems[i]->setVisible(!hideFigures);
			}

		}

		LayerItem(QWidget* parent, int lr):ItemMenu(parent, lr, -1), hideFigures(false)
		{
			this->setObjectName("Layer");
			QVBoxLayout* mainVLayout = new QVBoxLayout(this);
			QHBoxLayout* mainHLayout = new QHBoxLayout();
			mainVLayout->addLayout(mainHLayout);

			mainHLayout->setSizeConstraint(QLayout::SetFixedSize);

			this->setMinimumWidth(100);
			this->setFrameShape(QFrame::WinPanel);
			this->setFrameShadow(QFrame::Raised);

			downButton = new QToolButton(this);
			downButton->setObjectName(QString::fromUtf8("toolButton"));
			downButton->setMaximumHeight(13);
			downButton->setIconSize(QSize(12, 12));
			downButton->setCheckable(false);
			downButton->setAutoRepeat(false);
			downButton->setAutoExclusive(false);
			downButton->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
			downButton->setPopupMode(QToolButton::DelayedPopup);
			downButton->setAutoRaise(false);
			downButton->setArrowType(Qt::DownArrow);
			downButton->setVisible(false);
			downButton->setStyleSheet("border:0");
	        mainVLayout->addWidget(downButton);
			mainVLayout->setMargin(0);
			mainVLayout->setSpacing(0);

			mainHLayout->setSpacing(4);
			mainHLayout->setContentsMargins(6,4,6,4);
			mainHLayout->addWidget(&visibleFrame);
			mainHLayout->addWidget(&blockedFrame);
			mainHLayout->addWidget(&ItemName);

			connect( &ItemName, SIGNAL( changed() ),
					this, SLOT( renameLayer() ) );

			connect( downButton, SIGNAL( clicked() ),
							this, SLOT( setVisFigures() ) );

			connect( this, SIGNAL( renamedLayer(LayerItem*,QString) ),
					parent, SLOT( renameLayer(LayerItem*,QString ) ) );

			connect( this , SIGNAL( visibleChanged(bool) ),this,
					SLOT( visibleLayer( bool) ) );

			connect( this, SIGNAL( visibleLayerChanged(LayerItem*,bool)),
					parent, SLOT( visibleLayerChanged(LayerItem*,bool ) ) );

			connect( this , SIGNAL( blockedChanged(bool) ),this,
					SLOT( blockedLayer( bool) ) );

			connect( this, SIGNAL( blockedLayerChanged(LayerItem*,bool)),
					parent, SLOT( blockedLayerChanged(LayerItem*,bool ) ) );

			connect( this, SIGNAL( hideFigureChanged(int,bool)),
								parent, SIGNAL( hideFigureChanged(int,bool ) ) );


		}

		virtual ~LayerItem(){};

		virtual bool containsObject(ObjectItem* obj)
		{
			return objectItems.contains(obj);
		}

		virtual ObjectItem* getObject(int index)
		{
			if(index >= objectItems.size())
			{
				return 0;
			}
			return objectItems[index];
		}

		virtual int countObjects()
		{
			return objectItems.size();
		}

		virtual void deleteObject(int index)
		{
			delete objectItems[index];
			objectItems.remove(index);
		}

		virtual void deleteObjects(int i, int l, int o)
		{
			for(int j=0; j < objectItems.size(); j++)
			{
				if(l != i || o != j)
					delete objectItems[j];
			}
			objectItems.clear();
		}

		virtual void deleteObjects()
		{
			for(int i=0; i < objectItems.size(); i++)
			{
				delete objectItems[i];
			}
			objectItems.clear();
		}

		virtual ObjectItem* addObject( const QString &name, bool visible, bool blocked, bool transformed,
				QVBoxLayout* layout, int l, bool canTransformed)
		{
			ObjectItem* item = new ObjectItem(this, l, objectItems.size(),canTransformed, transformed);
			item->setVisibility(visible);

			item->setBlocked(blocked);
			item->setNameItem(name);
			objectItems.append(item);
			layout->addWidget(item);
			return item;
		}

	protected:
		QToolButton* downButton;
		QVector <ObjectItem*> objectItems;
		bool hideFigures;
	friend class LayersView;

};

class LayersView:public QWidget
{
	Q_OBJECT

	signals:
		void hideFigureChanged(int layer, bool value);
		void layerSelected( int index );
        void figureSelected( int , int );
		void movedLayer( int layerOld, int layerNew );
		void movedFigure( int layerOld, int objOld, int layerNew, int objNew );
		void deletedFigure( int layer, int obj );
		void renamedLayer( int layer, const QString &name );
		void renamedFigure( int layer, int obj, const QString &name );

		void transformedFigure( int layer, int obj, bool visible );
		void visibleFigure( int layer, int obj, bool visible );
		void visibleLayer( int layer, bool visible );
		void blockedFigure( int layer, int obj, bool blocked );
		void blockedLayer( int layer, bool blocked );
		void layerAdded(bool visible, bool blocked, const QString &name );
	public slots:

		virtual void clear()
		{
			if( layerItems.size() == 0 )
				return;

			for(int i=0; i < placesDrag.size(); i++)
				delete placesDrag[i];

			placesDrag.clear();

			for(int i=0; i < layerItems.size(); i++)
			{
				layerItems[i]->deleteObjects();
				delete layerItems[i];
			}

			layerItems.clear();
		}

		virtual void onSelect(int layer, int obj)
		{
			selLayers = layer;
			selFigure = obj;
			emit figureSelected( selLayers, selFigure);
			update();
		}

		virtual void showPlacesDrag(bool value)
		{

			for(int i = 0; i < placesDrag.size(); i++)
			{
				if(dragItem != 0)
				{
					if(dragItem->objectName() == placesDrag[i]->objectName())
						placesDrag[i]->setVisible(value);
				}
				else
					placesDrag[i]->setVisible(value);
			}
		}

		virtual QFrame* addPlaceDrag(const QString &type, int layer, int obj)//место для вставки виджета
		{
			PlaceDrag* place = new PlaceDrag(this, layer, obj, type);
			mainVLayout.addWidget(place);
			placesDrag.append(place);
			return place;
		}

		virtual LayerItem* addLayer( const QString &name, bool visible, bool blocked)
		{
			LayerItem* item = new LayerItem(this, layerItems.size());
			item->setVisibility(visible);
			item->setBlocked(blocked);
			item->setNameItem(name);
			mainVLayout.addWidget(item);
			layerItems.append(item);

			//update();
			return item;
		}

		virtual void addLayer()
		{
			QString name = tr("Layer ")+QString::number(layerItems.size());
			selLayers = layerItems.size();
			selFigure = -1;
			emit layerAdded(true, false, name );
		}

		virtual void renameFigure( ObjectItem* item, const QString &newName )
		{
			emit renamedFigure(item->getLayerIndex(), item->getObjectIndex() , newName );
		}

		virtual void renameLayer( LayerItem* item, const QString &newName )
		{
			emit renamedLayer( item->getLayerIndex() , newName );
		}

		virtual void visibleFigureChanged( ObjectItem* item, const bool visible )
		{
			emit visibleFigure(item->getLayerIndex(), item->getObjectIndex() , visible );
		}

		virtual void visibleLayerChanged(LayerItem* item, const bool visible )
		{
			for(int i=0; i<item->countObjects(); i++)
			{
				item->getObject(i)->setVisibility(visible);
			}

			emit visibleLayer( item->getLayerIndex() , visible );
			update();
		}

		virtual void blockedFigureChanged( ObjectItem* item, const bool blocked )
		{
			emit blockedFigure(item->getLayerIndex(), item->getObjectIndex() , blocked );
		}

		virtual void blockedLayerChanged(LayerItem* item, const bool blocked )
		{
			for(int i=0; i<item->countObjects(); i++)
			{
				item->getObject(i)->setBlocked(blocked);
			}

			emit blockedLayer( item->getLayerIndex() , blocked );
			update();
		}

		virtual void deleteFigure()
		{
			if( selFigure == -1 )
			{
				if( layerItems.size() == 1 )
				{
					QMessageBox::warning(this,tr("Exist only one layer!"), tr("You can't delete last layer."));
				}
				else
				{
					//layerItems[selLayers]->deleteObjects();
					emit deletedFigure( selLayers, selFigure);
					selFigure = -1;
					selLayers = 0;
					update();
				}
			}
			else
			{
				layerItems[selLayers]->deleteObject(selFigure);
				emit deletedFigure( selLayers, selFigure);
				selFigure = -1;
				update();
			}
		}

		virtual void deleteLayer( int index )
		{
			if( ( index < 0 ) || ( index >= countLayers() ) )
				return;

			layerItems.remove( index );
			//emit deleted( index );
			selLayers = -1;
			emit layerSelected( selLayers );
			update();
		}


		virtual void setSelectedFigure( int layer, int obj )
		{
			if( layer < 0 ) layer = 0;

			if( layer >= countLayers() ) layer = 0;

			if( countFigures(layer) <= 0 )
				return;

			if( obj < 0 ) obj = -1;
			if( obj >= countFigures(layer) ) obj = -1;

			if( selFigure == obj )
				return;

			selLayers = layer;
			selFigure = obj;
			update();
		}

	public:

		virtual void setPallete(QWidget* widget, QColor color)
		{
			QPalette pal;
			pal = widget->palette();
			widget->setBackgroundRole(QPalette::Button);
			pal.setBrush( QPalette::Button,color);
			widget->setAutoFillBackground( true );
			widget->setPalette( pal );
		}

		virtual void setSelectedLayer(int value)
		{
			selLayers = value;
		}

		virtual void setSelectedFigure(int value)
		{
			selFigure = value;
		}

		virtual int selectedLayer() const
		{
			return selLayers;
		}

		virtual int selectedFigure() const
		{
			return selFigure;
		}

		virtual int countLayers() const
		{
			return layerItems.size();
		}

		virtual int countFigures(int layer) const
		{
			return layerItems[layer]->countObjects();
		}

		virtual QString layerName( int layer ) const
		{
			if( ( layer < 0 ) || ( layer >= layerItems.size() ) )
				return QString();

			return layerItems[ layer ]->getNameItem();
		}

		virtual bool layerIsVisible( int layer ) const
		{
			if( ( layer < 0 ) || ( layer >= layerItems.size() ) )
				return false;

			return layerItems[ layer ]->getVisibility();
		}

		virtual QVBoxLayout* mainLayout()
		{
			return &mainVLayout;
		}

		LayersView( QWidget *parent = 0 ):
			QWidget( parent ),mainVLayout(this), selLayers( -1 )
		{
            defaultColor = this->palette().background().color();
			defaultLayer.setRgb(213,213,165);
			mainVLayout.setAlignment(Qt::AlignHCenter|Qt::AlignTop);
			mainVLayout.setMargin(0);
			mainVLayout.setSpacing(0);
			dragItem = 0;
			selFigure = -1;
			selLayers = 0;
			SelectedFigure.setRgb(104, 107, 152);
			SelectedLayer.setRgb(185, 185, 109);

			setFocusPolicy( Qt::StrongFocus );
			setAcceptDrops(true);
			update();
		}

		virtual ~LayersView()
		{
		}

	protected:

		virtual void paintEvent( QPaintEvent *event )
		{
			int countLayers = layerItems.size();
			for( int i = 0; i < countLayers; i++ )
			{
				setPallete(layerItems[i],defaultLayer);
				int countFigures = layerItems[i]->countObjects();

				if(countFigures!=0)
					layerItems[i]->downButton->setVisible(true);
				else
					layerItems[i]->downButton->setVisible(false);

				for(int j = 0; j < layerItems[i]->countObjects(); j++ )
				{
					ObjectItem* obj = layerItems[i]->getObject(j);
					obj->setVisibility(obj->getVisibility());
					setPallete(obj->getFrameTool() ,defaultColor);
				}
 			}

			if(selLayers<0)
			{
				selLayers = 0;
			}

			if(selFigure >= 0)
			{
				ObjectItem* obj = layerItems[selLayers]->getObject(selFigure);
				if(obj!=0)
				{
					setPallete(obj->getFrameTool(),SelectedFigure);
				}
			}
			setPallete(layerItems[selLayers],SelectedLayer);
		}

		virtual void mousePressEvent( QMouseEvent *event )
		{
			selected( event->pos() );
		}

		virtual void mouseMoveEvent( QMouseEvent *event )
		{
			selected( event->pos() );
			if((event->buttons() & Qt::LeftButton) && dragItem != 0)
			{
				int distance = (event->pos() - startPos).manhattanLength();
				if( distance = QApplication::startDragDistance())
					performDrag();
			}
		}

		virtual void performDrag()
		{
			QMimeData *mimeData = new QMimeData;
			mimeData->setText("Hello World");
			QPixmap img;
			QDrag *drag = new QDrag(this);
			drag->setMimeData(mimeData);
			if(dragItem!=0)
				drag->setPixmap(img.grabWidget(dragItem,dragItem->rect()));
			if(drag->exec(Qt::MoveAction) == Qt::MoveAction)
			{
				showPlacesDrag(false);
				update();
			}
			showPlacesDrag(false);


		}

		virtual void dragEnterEvent( QDragEnterEvent *event )
		{
			event->setDropAction(Qt::MoveAction);
			event->accept();
			showPlacesDrag(true);
		}

		virtual void dragMoveEvent( QDragMoveEvent *event )
		{
			event->setDropAction(Qt::MoveAction);
			event->accept();
			QPoint p = event->pos();
			for(int i=0; i<placesDrag.size(); i++)
			{
				setPallete(placesDrag[i],defaultColor);
				if(placesDrag[i]->rect().contains(placesDrag[i]->mapFromParent(p)))
				{
					setPallete(placesDrag[i],QColor(57,57,86));
				}
			}

		}

		virtual void dropEvent( QDropEvent *event )
		{
			QPoint p = event->pos();
			event->setDropAction(Qt::MoveAction);
			event->accept();
			QString name = dragItem->objectName();
			int indexL = dragItem->getLayerIndex();
			int indexO = dragItem->getObjectIndex();
			dragItem = NULL;
			for(int i=0; i<placesDrag.size(); i++)
			{
				if(placesDrag[i]->rect().contains(placesDrag[i]->mapFromParent(p)))
				{
					if(name == "Layer" && indexL != placesDrag[i]->getLayerIndex())
						emit movedLayer( indexL, placesDrag[i]->getLayerIndex() );
					else if(name == "Figure" && (indexL != placesDrag[i]->getLayerIndex() ||  indexO != placesDrag[i]->getObjectIndex()))
						emit movedFigure( indexL, indexO, placesDrag[i]->getLayerIndex(), placesDrag[i]->getObjectIndex() );
					break;
				}
			}


		}

		virtual void mouseDoubleClickEvent( QMouseEvent *event )
		{

		}

		/*virtual void keyPressEvent( QKeyEvent * event )
		{
			switch( event->key() )
			{
				case Qt::Key_Up:
					selLayers--;
					if( selLayers < 0 )
						selLayers = layerItems.size() - 1;
					emit layerSelected( selLayers );
					update();
					break;

				case Qt::Key_Down:
					selLayers++;
					if( selLayers >= layerItems.size() )
						selLayers = 0;
					if( layerItems.size() == 0 )
						selLayers = -1;
					emit layerSelected( selLayers );
					update();
					break;

				case Qt::Key_Enter:
				case Qt::Key_Return:
				case Qt::Key_Space:
					if( selLayers != -1 )
						setLayerVisible( !layerItems[ selLayers ]->getVisibility() );
					break;
			}
		}*/

	private:
		QVector <LayerItem*> layerItems;
		QVector <PlaceDrag*> placesDrag;
		QVBoxLayout mainVLayout;

		int selLayers; //Текущий слой и текущая фигура
		int selFigure;

		QColor SelectedFigure;
		QColor SelectedLayer;
		QColor defaultLayer;
		QColor defaultColor;

		QPoint startPos;
		ItemMenu* dragItem;

		void selected( const QPoint &p )
		{
			dragItem = 0;
			int countLayers = layerItems.size();
			//QMessageBox::warning(this,"CurLasdayer111",QString::number(selLayers));

			for( int i = 0; i < countLayers; i++ )
			{
				QRect rect = layerItems[i]->rect();
				setPallete(layerItems[i],defaultLayer);

				if( rect.contains( layerItems[i]->mapFromParent(p) ) )
				{
					startPos = p;
					dragItem = layerItems[i];
					selLayers = i;
					selFigure = -1;
					break;
				}

				for(int j = 0; j < layerItems[i]->countObjects(); j++ )
				{

					ObjectItem* obj = layerItems[i]->getObject(j);
					QPoint pos = obj->mapFromParent(p);
					QRect rectObj = obj->rect();

					setPallete(obj->getFrameTool() ,defaultColor);
					if( rect.contains( pos ) )
					{
						startPos = p;
						dragItem = obj;
						selFigure = j;
						selLayers = i;
					}
				}
 			}

			if(selFigure >= 0)
			{
				ObjectItem* obj = layerItems[selLayers]->getObject(selFigure);
				if(obj!=0)
				{
					setPallete(obj->getFrameTool(),SelectedFigure);
				}
			}
			setPallete(layerItems[selLayers],SelectedLayer);
			emit figureSelected( selLayers, selFigure);
			update();
		}

		friend class LayerItem;
};

class LayersEditor:public LayersEditorInterface, public InterfacePlugin
{
	Q_OBJECT
	Q_INTERFACES( LayersEditorInterface )
	Q_INTERFACES( InterfacePlugin )
private:

	LayersView view;
	MainWindowInterface* mainWin;
	PaintWidgetInterface* painter;
	QDockWidget *layersWindow;
	QToolButton deleteBtn;
	QToolButton addLayer;
	QAction *deleteAct;
	QAction *addLayerAct;
	static bool resourcesInited;

	signals:
        void figureSelected( int , int );
		void movedFirst( int index );
		void movedUp( int index );
		void movedDown( int index );
		void movedLast( int index );
		void deleted( int index );
		void renamed( int index, const QString &name );
		void visibleChanged( int index, bool visible );
		void setActive(QString);

	public:
		virtual void createActions()
		{
			deleteAct = new QAction(tr("Delete"),this);
			deleteAct->setIcon(QIcon( ":/layerseditor/images/delete.png" ));
			deleteAct->setToolTip( tr( "Delete" ) );
			connect(deleteAct, SIGNAL(triggered()), &view, SLOT(deleteFigure()));

			addLayerAct = new QAction(tr("Add layer"),this);
			addLayerAct->setIcon(QIcon( ":/layerseditor/images/addLayer.png" ));
			addLayerAct->setToolTip( tr( "Add layer" ) );
			connect(addLayerAct, SIGNAL(triggered()), &view, SLOT(addLayer()));
		}

		virtual void selectFigure( int index, int obj )
		{
			view.setSelectedFigure( index, obj );
		}

		virtual void clear()
		{
			view.clear();
		}

		virtual void createPlugin(QObject *parent, QString idParent,plugin::PluginsManager *manager)
		{
			mainWin = MAINWINDOW(parent);
			if(mainWin!=0)
			{
				connect( this, SIGNAL( setActive( QString ) ), getBeep(), SIGNAL( setActive( QString ) ) );

				painter = PAINTWIDGETINTERFACE(mainWin->getPaintWidget());
				layersWindow = new QDockWidget(mainWin);
				layersWindow->setMinimumWidth(150);
				layersWindow->setWindowTitle( tr( "Layers" ) );
				this->setParent( layersWindow );
				layersWindow->setWidget( this );
				mainWin->addDockWidget( Qt::RightDockWidgetArea, layersWindow );

				connect( painter, SIGNAL( allLayersChanged() ),
						this, SLOT( onStateChanged() ) );

				connect( painter, SIGNAL( movedObject() ),
						this, SLOT( onStateChanged() ) );

				connect( painter, SIGNAL( objectCreated() ),
						this, SLOT( onStateChanged() ) );

				connect( painter, SIGNAL( undoEvents() ),
						this, SLOT( onStateChanged() ) );

				connect( painter, SIGNAL( frameChanged(qreal) ),
						this, SLOT( onStateChanged() ) );

				connect( this, SIGNAL( movedFirst( int ) ),
						mainWin, SLOT( onMoveLayerToFirst( int ) ) );

				connect( this, SIGNAL( movedUp( int ) ),
						mainWin, SLOT( onMoveLayerUp( int ) ) );

				connect( this, SIGNAL( movedDown( int ) ),
						mainWin, SLOT( onMoveLayerDown( int ) ) );

				connect( this, SIGNAL( movedLast( int ) ),
						mainWin, SLOT( onMoveLayerToLast( int ) ) );

				connect( this, SIGNAL( movedFirst( int ) ),
						this, SLOT( onTouched( ) ) );

				connect( this, SIGNAL( movedUp( int ) ),
						this, SLOT( onTouched( ) ) );

				connect( this, SIGNAL( movedDown( int ) ),
						this, SLOT( onTouched( ) ) );

				connect( this, SIGNAL( movedLast( int ) ),
						this, SLOT( onTouched( ) ) );


				connect( &view, SIGNAL( renamedFigure( int, int, const QString& ) ),
						mainWin, SLOT( onRenameFigure( int, int, const QString& ) ) );

				connect( &view, SIGNAL( renamedFigure( int, int, const QString& ) ), this, SLOT( onTouched() ) );

				connect( &view, SIGNAL( layerAdded( bool, bool, const QString& ) ),
										mainWin, SLOT( onAddLayer( bool, bool, const QString& ) ) );

				connect( &view, SIGNAL( layerAdded( bool, bool, const QString& ) ), this, SLOT( onTouched() ) );

				connect( &view, SIGNAL( movedFigure( int, int, int, int ) ),
										painter, SLOT( moveFigure( int, int, int, int )) );

				connect( &view, SIGNAL( movedFigure( int, int, int, int ) ), this, SLOT( onTouched() ) );

				connect( &view, SIGNAL( movedLayer( int, int ) ),
										painter, SLOT( moveLayer( int, int )) );

				connect( &view, SIGNAL( movedLayer( int, int ) ), this, SLOT( onTouched() ) );

				connect( &view, SIGNAL( renamedLayer( int, const QString& ) ),
										mainWin, SLOT( onRenameLayer( int, const QString& ) ) );

				connect( &view, SIGNAL( renamedLayer( int, const QString& ) ), this, SLOT( onTouched() ) );

				connect( painter, SIGNAL( figureSelected( int, int ) ),
						this, SLOT( onLayerSelectedFromViewport( int, int ) ) );

				connect( &view, SIGNAL( figureSelected( int, int ) ),
						mainWin, SLOT( onFigureSelected( int, int ) ) );

				connect( &view, SIGNAL( visibleFigure( int, int, bool ) ),
						mainWin, SLOT( onFigureVisibleChange( int, int, bool ) ) );

				connect( &view, SIGNAL( visibleFigure( int, int, bool ) ), this, SLOT( onTouched() ) );

				connect( &view, SIGNAL( visibleLayer( int, bool ) ),
						mainWin, SLOT( onLayerVisibleChange( int, bool ) ) );

				connect( &view, SIGNAL( visibleLayer( int, bool ) ), this, SLOT( onTouched() ) );

				connect( &view, SIGNAL( blockedFigure( int, int, bool ) ),
						mainWin, SLOT( onFigureBlockedChange( int, int, bool ) ) );

				connect( &view, SIGNAL( blockedFigure( int, int, bool ) ), this, SLOT( onTouched() ) );

				connect( &view, SIGNAL( blockedLayer( int, bool ) ),
						mainWin, SLOT( onLayerBlockedChange( int, bool ) ) );

				connect( &view, SIGNAL( blockedLayer( int, bool ) ), this, SLOT( onTouched() ) );

				connect( &view, SIGNAL( deletedFigure( int, int ) ),
						mainWin, SLOT( onFigureDeletedFromLayersEditor( int, int ) ) );

				connect( &view, SIGNAL( deletedFigure( int, int ) ), this, SLOT( onTouched() ) );

				connect( &view, SIGNAL( transformedFigure( int, int, bool ) ),
						this, SLOT( onTransformedFigure( int, int, bool ) ) );

				connect( &view, SIGNAL( transformedFigure( int, int, bool ) ), this, SLOT( onTouched() ) );
				initLayersEditor();
			}
		}

		virtual QString getName()const
		{
			return "LayersEditor";
		}

		LayersEditor( plugin::PluginsManager *manager ):
					view( this ),
					deleteBtn( this )
				{
					if( !resourcesInited )
					{
						Q_INIT_RESOURCE( LayersEditor );
						resourcesInited = true;
					}

					QVBoxLayout *l = new QVBoxLayout( this );
					l->setMargin(0);
					l->setSpacing(0);
					QScrollArea *area = new QScrollArea( this );
					area->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
					area->setWidgetResizable(true);
					area->setContentsMargins(0,0,0,0);
					area->setFrameShape(QFrame::WinPanel);
					area->setFrameShadow(QFrame::Sunken);
					area->setMinimumWidth(170);
					l->addWidget( area );
					area->setWidget( &view );

					QHBoxLayout *layout2 = new QHBoxLayout();
					l->addLayout( layout2 );

					createActions();
					view.addAction(deleteAct);
					view.setContextMenuPolicy(Qt::ActionsContextMenu);

					layout2->addStretch();

					layout2->addWidget( &addLayer );
					addLayer.setIcon( QIcon( ":/layerseditor/images/addLayer.png" ) );
					addLayer.setToolTip( tr( "Add layer" ) );
					addLayer.setAutoRaise(true);
					connect( &addLayer, SIGNAL( clicked() ), &view, SLOT( addLayer() ) );

					layout2->addWidget( &deleteBtn );
					deleteBtn.setIcon( QIcon( ":/layerseditor/images/delete.png" ) );
					deleteBtn.setToolTip( tr( "Delete" ) );
					deleteBtn.setAutoRaise(true);
					connect( &deleteBtn, SIGNAL( clicked() ), &view, SLOT( deleteFigure() ) );


					connect( &view, SIGNAL( layerSelected( int ) ),
									this, SLOT( onLayerSelected( int ) ) );
					connect( &view, SIGNAL( layerSelected( int ) ),
									this, SIGNAL( layerSelected( int ) ) );
					connect( &view, SIGNAL( movedFirst( int ) ),
									this, SIGNAL( movedFirst( int ) ) );
					connect( &view, SIGNAL( movedUp( int ) ),
									this, SIGNAL( movedUp( int ) ) );
					connect( &view, SIGNAL( movedDown( int ) ),
									this, SIGNAL( movedDown( int ) ) );
					connect( &view, SIGNAL( movedLast( int ) ),
									this, SIGNAL( movedLast( int ) ) );
					connect( &view, SIGNAL( deleted( int ) ),
									this, SIGNAL( deleted( int ) ) );
					connect( &view, SIGNAL( visibleChanged( int, bool ) ),
									this, SIGNAL( visibleChanged( int, bool ) ) );
					connect( &view, SIGNAL( hideFigureChanged( int, bool ) ),
														this, SLOT( onHideFigure( int, bool ) ) );
				}

		virtual ~LayersEditor()
		{
			view.clear();
		};

	private:

		void initLayersEditor()
		{
			view.setSelectedLayer(painter->layer());
			clear();

			int countLayers = painter->countLayers();

			int countFigures;
			LayerItem* layer;

			for( int i = 0; i < countLayers; i++ )
			{

				view.addPlaceDrag("Layer", i, -1);
				layer = view.addLayer( painter->layerName( i ),
						painter->isLayerVisible( i ), painter->isLayerBlocked( i ) );

				if(painter->isContainsFrame(i))
				{

					countFigures = painter->countFigures(i);
					for(int j = 0; j < countFigures; j++)
					{
						view.addPlaceDrag("Figure",i,j);
						layer->addObject( painter->figureName(i,j),painter->isFigureVisible(i,j),painter->isFigureBlocked(i,j),
								painter->isFigureTransformed(i,j),view.mainLayout(), i,!(painter->objectIsContainer(i,j)));
					}
				}
				layer->setHideFigures(painter->getHideFigures(i));
			}

		}

		virtual void deleteLayer(int)
		{

		}
	private slots:

		void onLayerSelectedFromViewport( int layer, int obj )
		{
			if( !mainWin->getProcessSignals() ) return;
			this->selectFigure( layer, obj );
			if( layer == -1 ) return;
		}

		void onTransformedFigure(int layer, int obj, bool trns)
		{
			if( !mainWin->getProcessSignals() ) return;
			painter->setFigureTransformed(layer, obj, trns);
		}

		void onHideFigure(int layer, bool value)
		{
			if( !mainWin->getProcessSignals() ) return;
			//QMessageBox::warning(this,"dasd",QString::number(1));
			painter->setHideFigures(layer,value);
		}

		void onStateChanged()
		{
			if( !mainWin->getProcessSignals() ) return;
			//QMessageBox::warning(this,"dasd",QString::number(1));
			initLayersEditor();
		}

		void onTouched() {
			emit setActive(getName());
		}

};

#endif /* __LAYERS_EDITOR_H__ */
