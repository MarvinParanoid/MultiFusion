#include <QtGui>
#include "MainWindow.h"
#include "../pluginTool/PluginDialog.h"

class QMainWindow;
class QAction;

mainWindow::mainWindow( QWidget *parent):
	manager( "./plugins.xml", this ),processSignals( true )
	{
		painter = new PaintWidget( this, &manager);
		setCentralWidget( painter );
		mainMenu = menuBar();
        QMenu *qMenu;
		qMenu = painter->getContextMenu();

		fileMenu = mainMenu->addMenu( tr( "&File" ) );
		fileToolBar = new QToolBar( tr( "Main toolbar" ),this );
		addToolBar( fileToolBar );
		fileToolBar->setIconSize(QSize(24,24));
		editMenu = mainMenu->addMenu( tr( "&Edit" ) );
		windowsMenu = mainMenu->addMenu( tr( "&Tools" ) );
		helpMenu = mainMenu->addMenu( tr( "&Help" ) );

		manager.addPlugins(this, "Main");

		QAction *pluginsAction = windowsMenu->addAction( tr( "&Plugins..." ) );
		connect( pluginsAction, SIGNAL( triggered( bool ) ), this, SLOT( onShowPlugins() ) );
		windowsMenu->addSeparator();

		QAction *propertiesAction = windowsMenu->addAction( tr( "Show viewport properties..." ));

		connect( propertiesAction, SIGNAL( triggered( bool ) ), this, SLOT( showViewportProperties() ) );

		/*QAction *helpAction = helpMenu->addAction( tr( "&Help..." ) );
		connect( helpAction, SIGNAL( triggered( bool ) ), this, SLOT( onShowHelp() ) );*/

		QAction *helpAction = helpMenu->addAction(  tr( "About &Qt..." ) );
		helpAction->setIcon( QIcon( ":/main/images/aboutQt.png" ) );
		connect( helpAction, SIGNAL( triggered( bool ) ), this, SLOT( onAboutQt() ) );

		helpAction = helpMenu->addAction(  tr( "&About..." ) );
		connect( helpAction, SIGNAL( triggered( bool ) ), this, SLOT( onAbout() ) );

		this->setWindowTitle( QString( APP_NAME ) + tr( ":Untitled" ) );

		connect( painter, SIGNAL( frameChanged( qreal ) ),
				this, SLOT( onPlayFrameChanged( qreal ) ) );

		connect( painter, SIGNAL( countFramesChanged( int ) ),
				this, SLOT( onCountFramesChanged( int ) ) );

		connect( painter, SIGNAL( objectCreated() ),
				this, SLOT( onObjectCreated() ) );

		QAction* newFileAction = fileMenu->addAction( tr( "&New" ) );
		newFileAction->setIcon( QIcon( ":/main/images/new.png" ) );
		fileToolBar->insertAction( 0 , newFileAction );
		newFileAction->setShortcut( tr( "Ctrl+N" ) );

		fileToolBar->addSeparator();
		connect( newFileAction,
			SIGNAL( triggered( bool ) ), this, SLOT( onNewFile() ) );

		fileMenu->addSeparator();
		QAction* openFileAction = fileMenu->addAction(  tr( "&Open..." ) );
		openFileAction->setIcon( QIcon( ":/main/images/open.png" ) );
		openFileAction->setShortcut( tr( "Ctrl+O" ) );

		fileToolBar->insertAction( 0 , openFileAction );
		fileToolBar->addSeparator();

		connect( openFileAction,
			SIGNAL( triggered( bool ) ), this, SLOT(onOpenFile() ) );

		fileMenu->addSeparator();

		QAction* saveFileAction = fileMenu->addAction(tr( "&Save" ) );
		saveFileAction->setIcon( QIcon( ":/main/images/save.png" ) );
		connect( saveFileAction,
			SIGNAL( triggered( bool ) ), this, SLOT( onSaveFile() ) );
		saveFileAction->setShortcut( tr( "Ctrl+S" ) );

		fileToolBar->insertAction( 0 , saveFileAction );
		fileToolBar->addSeparator();

		QAction* saveFileAsAction = fileMenu->addAction( tr( "Save &as..." ) );
		connect( saveFileAsAction,
			SIGNAL( triggered( bool ) ), this, SLOT( onSaveFileAs() ) );

		fileMenu->addSeparator();
		QAction *exitAction = fileMenu->addAction( tr( "&Exit" ) );
		exitAction->setIcon( QIcon( ":/main/images/exit.png" ) );
		connect( exitAction,
			SIGNAL( triggered( bool ) ), this, SLOT( onExitMenu() ) );
		exitAction->setShortcut( tr( "Ctrl+Q" ) );

		fileToolBar->insertAction( 0 , exitAction );

		toSplineAction = editMenu->addAction( tr( "To spline" ) );
		showBezier = editMenu->addAction( tr( "Show Bezier points" ) );
		editMenu->addSeparator();
		deleteAction = editMenu->addAction( tr( "&Delete" ) );
		deleteAction->setIcon( QIcon( ":/main/images/delete.png" ) );
		editMenu->addSeparator();

		cutAction = editMenu->addAction( tr( "C&ut" ) );
		cutAction->setIcon( QIcon( ":/main/images/cut.png" ) );
		copyAction = editMenu->addAction( tr( "&Copy" ) );
		copyAction->setIcon( QIcon( ":/main/images/copy.png" ) );
		pasteAction = editMenu->addAction( tr( "&Paste" ) );
		pasteAction->setIcon( QIcon( ":/main/images/paste.png" ) );

		editMenu->addSeparator();

		groupAction = editMenu->addAction( tr( "&Group" ) );
		ungroupAction = editMenu->addAction( tr( "&Ungroup" ) );

		qMenu->addAction(toSplineAction);
		qMenu->addAction(showBezier);
		qMenu->addSeparator();
		qMenu->addAction(deleteAction);
		qMenu->addAction(cutAction);
		qMenu->addAction(copyAction);
		qMenu->addAction(pasteAction);
		qMenu->addAction(groupAction);
		qMenu->addAction(ungroupAction);
		connect( qMenu, SIGNAL( aboutToShow(  ) ), this, SLOT( onShowEditMenu() ) );
		groupAction->setShortcut( tr( "Ctrl+G" ) );
		ungroupAction->setShortcut( tr( "Ctrl+U" ) );

		deleteAction->setShortcut( tr( "Del" ) );
		cutAction->setShortcut( tr( "Ctrl+X" ) );
		copyAction->setShortcut( tr( "Ctrl+C" ) );
		pasteAction->setShortcut( tr( "Ctrl+V" ) );
		editMenu->addSeparator();

		connect( groupAction, SIGNAL( triggered( bool ) ), this, SLOT( onGroupSelected() ) );
		connect( groupAction, SIGNAL( triggered( bool ) ), this, SLOT( onGroupSelected() ) );
		connect( ungroupAction, SIGNAL( triggered( bool ) ), this, SLOT( onUngroupSelected() ) );
		connect( deleteAction, SIGNAL( triggered( bool ) ), this, SLOT( onDelete() ) );
		connect( cutAction, SIGNAL( triggered( bool ) ), this, SLOT( onCut() ) );
		connect( copyAction, SIGNAL( triggered( bool ) ), this, SLOT( onCopy() ) );
		connect( pasteAction, SIGNAL( triggered( bool ) ), this, SLOT( onPaste() ) );
		connect( toSplineAction, SIGNAL( triggered( bool ) ), this, SLOT( onStyleFigure() ) );
		connect( showBezier, SIGNAL( triggered( bool ) ), this, SLOT( onShowBezier() ) );

        m_plblX = new QLabel("X=0",this);
        m_plblY = new QLabel("Y=0",this);
        statusBar()->addWidget(m_plblY);
        statusBar()->addWidget(m_plblX);

}


void mainWindow::onShowBezier()
{
	int isBezier = painter->FigureIsShowBezier();
	if(isBezier!=-1)
	{
		if(isBezier==1)
			painter->FigureSetShowBezier(false);
		else
			painter->FigureSetShowBezier(true);
	}
}

void mainWindow::onStyleFigure()
{
	int isSpline = painter->FigureIsSpline();
	if(isSpline!=-1)
	{
		if(isSpline==1)
			painter->FigureSetStyle(false);
		else
			painter->FigureSetStyle(true);
	}
}

void mainWindow::onShowPlugins()
{
	PluginDialog::EnabledDialog(this, manager.getPluginsDom());
}

void mainWindow::onDelete()
{
	painter->deleteSelected();
}

void mainWindow::onShowEditMenu()
{
	if( !processSignals ) return;

	int isSpline = painter->FigureIsSpline();
	if(isSpline!=-1)
	{
		if(isSpline==1)
			toSplineAction->setText(tr("To normal"));
		else
			toSplineAction->setText(tr("To spline"));
		toSplineAction->setEnabled(true);
	}
	else
	{
		toSplineAction->setEnabled(false);
	}

	int isBezier = painter->FigureIsShowBezier();
	if(isBezier!=-1 && isSpline!=0)
	{
		if(isBezier==1)
			showBezier->setText(tr("Hide bezier points"));
		else
			showBezier->setText(tr("Show bezier points"));
		showBezier->setEnabled(true);
	}
	else
	{
		showBezier->setEnabled(false);
	}

	groupAction->setEnabled( painter->canGroup() );
	ungroupAction->setEnabled( painter->canUngroup() );

	deleteAction->setEnabled( painter->canDeleteSelected() );
	cutAction->setEnabled( painter->canCopyOrCutToClipboard() );
	copyAction->setEnabled( painter->canCopyOrCutToClipboard() );
	pasteAction->setEnabled( painter->canPasteFromClipboard() );
}

void mainWindow::onCut()
{
	if( QMessageBox::question( this,
		tr( "Cut selection to clipboard." ),
		tr( "Are you shure to cut this layers to clipboard?" ),
		QMessageBox::Ok | QMessageBox::Cancel,
		QMessageBox::Cancel ) == QMessageBox::Ok )
	{
		painter->cutToClipboard();
	}
}

void mainWindow::onCopy()
{
	painter->copyToClipboard();
}

void mainWindow::onPaste()
{
	painter->pasteFromClipboard();
}

void mainWindow::onNewFile()
{
	if( QMessageBox::question( this,
		tr( "Create new animation." ),
		tr( "Are you shure to create new animation?" ) +
		tr( "It delete loaded film, if it unsaved." ),
		QMessageBox::Ok | QMessageBox::Cancel,
		QMessageBox::Cancel ) == QMessageBox::Cancel )
	{
		return;
	}

	painter->reset();
	setWindowTitle( QString( APP_NAME ) + tr( ":Untitled" ) );
}

void mainWindow::onOpenFile()
{
	if( QMessageBox::question( this,
	tr( "Loading animation." ),
	tr( "Are you shure to load animation?" ) +
	tr( "It delete loaded film, if it unsaved." ),
	QMessageBox::Ok | QMessageBox::Cancel,
	QMessageBox::Cancel ) == QMessageBox::Cancel )
	{
		return;
	}

	QString fileName =
	QFileDialog::getOpenFileName( this, tr( "Open SACS2 animation" ),
	"/", tr( "SACS2 animation (*.sacs2);;All files (*)" ) );

	if( fileName.isNull() ) return;

	QFile file( fileName );
	if( !file.open( QIODevice::ReadOnly ) )
	{
		QMessageBox::critical( this,
						tr( "Can't open file." ),
						tr( "Can't open selected file to reading." ) +
						tr( "May be you don't have permissions?" ),
						QMessageBox::Ok );
		return;
	}

	QDataStream s( &file );
	//s.setVersion( QDataStream::Qt_4_5 ); // Для совместимости с будующими версими Qt
	if( !painter->load( s ) )
	{
		QMessageBox::critical( this,
				tr( "Error reading animation." ),
				tr( "Data is corrupt or saved in unsupported format." ),
				QMessageBox::Ok );
		return;
	}

	lastSavedFile = fileName;
	setWindowTitle( QString( APP_NAME ) + tr( ":" ) + lastSavedFile );
}

void mainWindow::onSaveFile()
{
	if( lastSavedFile.isNull() )
	{
		onSaveFileAs();
		return;
	}

	QFile file( lastSavedFile );
	if( !file.open( QIODevice::WriteOnly ) )
	{
		QMessageBox::critical( this,
				tr( "Can't open file." ),
				tr( "Can't open selected file to writing." ) +
					tr( "May be you don't have permissions?" ),
				QMessageBox::Ok );
		return;
	}

	QDataStream s( &file );
	//s.setVersion( QDataStream::Qt_4_5 ); // Для совместимости с будующими версими Qt
	painter->save( s );
}

void mainWindow::onSaveFileAs()
{
	QString fileName =
		QFileDialog::getSaveFileName( this, tr( "Save SACS2 animation" ),
					"/", tr( "SACS2 animation (*.sacs2)" ) );

	if( fileName.isNull() ) return;

	int pointIndex = fileName.lastIndexOf( '.' );
	if( pointIndex == -1 )
		fileName += ".sacs2";

	QFile file( fileName );
	if( !file.open( QIODevice::WriteOnly ) )
	{
		QMessageBox::critical( this,
				tr( "Can't open file." ),
				tr( "Can't open selected file to writing." ) +
					tr( "May be you don't have permissions?" ),
				QMessageBox::Ok );
		return;
	}

	QDataStream s( &file );
	//s.setVersion( QDataStream::Qt_4_5 ); // Для совместимости с будующими версими Qt
	painter->save( s );
	lastSavedFile = fileName;
	setWindowTitle( QString( APP_NAME ) + tr( ":" ) + lastSavedFile );
}


void mainWindow::onFigureSelected( int layer, int obj )
{
	if( !processSignals ) return;
	if( obj == painter->selectedFigure() && layer == painter->selectedLayer() ) return;
	painter->selectFigure( layer , obj);
	if( layer == -1 ) return;

}

void mainWindow::deleteFrame(int position)
{
	if( !processSignals ) return;

	if( QMessageBox::question( this,
		tr( "Deleting current frame." ),
		tr( "Are you sure you want to delete this frame?" ),
		QMessageBox::Ok | QMessageBox::Cancel,
		QMessageBox::Cancel ) == QMessageBox::Ok )
	{
		painter->deleteFrame(position);
	}
}

void mainWindow::onZoomInPressed()
{
	if( !processSignals ) return;

	painter->scale( 1.1 );
}

void mainWindow::onZoomOutPressed()
{
	if( !processSignals ) return;

	painter->scale( 0.9 );
}

void mainWindow::onSelectionToolPressed()
{
	if( !processSignals ) return;

	painter->setSelectionTool();
}

void mainWindow::onAddPointToolPressed()
{
	if( !processSignals ) return;

	painter->setAddPointTool();
}

void mainWindow::onToolSelected( FigureToolInterface *tool )
{
	if( !processSignals ) return;

	painter->setTool( tool );
}


void mainWindow::onFrameChanged( int frame, bool played )
{
	if( !processSignals ) return;

	painter->setFrame( frame, played );
}

void mainWindow::onRenameFigure( int layer, int obj, const QString &newName )
{
	if( !processSignals ) return;
	painter->renameFigure( layer,obj, newName );
}

void mainWindow::onRenameLayer( int layer, const QString &newName )
{
	if( !processSignals ) return;
	painter->renameLayer( layer, newName );
}

void  mainWindow::onAddLayer(bool visible, bool blocked, const QString &name)
{
	if( !processSignals ) return;
	painter->addLayer(visible,blocked,name);

}

void mainWindow::onFigureBlockedChange( int layer, int obj, bool blocked )
{
	if( !processSignals ) return;

	painter->setFigureBlocked( layer, obj, blocked );
}

void mainWindow::onLayerBlockedChange( int layer, bool blocked )
{
	if( !processSignals ) return;

	painter->setLayerBlocked( layer, blocked );
}

void mainWindow::onFigureVisibleChange( int layer, int obj, bool visible )
{
	if( !processSignals ) return;

	painter->setFigureVisible( layer, obj, visible );
}

void mainWindow::onLayerVisibleChange( int layer, bool visible )
{
	if( !processSignals ) return;

	painter->setLayerVisible( layer, visible );
}


void mainWindow::onFigureDeletedFromLayersEditor( int layer, int obj )
{
	if( !processSignals ) return;

	painter->deleteFigure( layer, obj );
}

void mainWindow::onExitMenu()
{
	if( !processSignals ) return;

	close();
}

void mainWindow::onToggleMainToolbarWindow( bool visible )
{
	if( !processSignals ) return;

	fileToolBar->setVisible( visible );
}

void mainWindow::onPenChanged( const QPen &p )
{
	if( !processSignals ) return;

	painter->setPen( p );
	emit StateChanged("Set pen");
}

void mainWindow::onBrushChanged( const QBrush &b )
{
	if( !processSignals ) return;

	painter->setBrush( b );
	emit StateChanged("Set brush");
}

void mainWindow::onGroupSelected()
{
	if( !processSignals ) return;

	painter->group();
}

void mainWindow::onUngroupSelected()
{
	if( !processSignals ) return;

	painter->ungroup();
}

void mainWindow::showViewportProperties()
{
	if( !processSignals ) return;

	painter->showConfig();
}

QMenu * mainWindow::getFileMenu()const
{
	return fileMenu;
}

QMenu * mainWindow::getEditMenu()const
{
	return editMenu;
}

QMenu * mainWindow::getWindowsMenu()const
{
	return windowsMenu;
}

QMenu * mainWindow::getHelpMenu()const
{
	return helpMenu;
}

QMenuBar * mainWindow::getMainMenu()const
{
	return mainMenu;
}

QToolBar * mainWindow::getToolBar()const
{
	return fileToolBar;
}

QObject * mainWindow::getPaintWidget()const
{
	return painter;
}

bool mainWindow::getProcessSignals()const
{
	return processSignals;
}


void mainWindow::onShowHelp()
{
	//QMessageBox::information( this, tr( "Help" ), tr( "Currently not implemented :)" ) );
}

void mainWindow::onAbout()
{
	if( !processSignals ) return;

	QMessageBox::about( this, tr( "About MultiFusion" ),
		tr( "<CENTER><B>MultiFusion</B> super-puper application.\
			<BR><B>Serkov Alexander</B>,<BR>\
            <B>IFMO</B>, <B>KIKG</B>, 2007</CENTER>\
            <BR>Rofl 2014" ) );
}



void mainWindow::onAboutQt()
{
	if( !processSignals ) return;

    QMessageBox::aboutQt( this, tr( "MultiFusion application" ) );
}

void mainWindow::onRPWMouseMove(int x, int y)
{
    m_plblX->setText("X=" + QString().setNum(x));
    m_plblY->setText("Y=" + QString().setNum(y));
}


void mainWindow::setProcessSignals(bool ps)
{
	processSignals = ps;
}
