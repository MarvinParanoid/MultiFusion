#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <QMainWindow>
#include "../interfaces/MainWindowInterface.h"
#include "../interfaces/FigureToolInterface.h"
#include "../pluginTool/Plugin.h"
#include "../paintWidget/PaintWidget.h"


#define APP_NAME "MultiFusion"

class mainWindow: public MainWindowInterface
{
	Q_OBJECT
	Q_INTERFACES(MainWindowInterface)
	signals:
		 /**
		 * Отправляется после выполнения события, которое нам нужно сохранить в истории
		 * @param event имя события
		 */
		void StateChanged(QString event);

	public:
		virtual QMenu * getFileMenu()const;
		virtual QMenu * getEditMenu()const;
		virtual QMenu * getWindowsMenu()const;
		virtual QMenu * getHelpMenu()const;
		virtual QMenuBar * getMainMenu()const;
		virtual QToolBar * getToolBar()const;
		virtual QObject * getPaintWidget()const;
		virtual bool getProcessSignals()const;
		virtual void setProcessSignals(bool ps);
		mainWindow( QWidget *parent = 0 );
		virtual ~mainWindow(){}

	public slots:
		void onStyleFigure();
		void onShowBezier();
		void showViewportProperties();
		void onShowPlugins();
		void onDelete();
		void onCut();
		void onShowEditMenu();
		void onCopy();
		void onPaste();
		void onNewFile();
		void onOpenFile();
		void onSaveFile();
		void onSaveFileAs();
		void deleteFrame(int position);
		void onZoomInPressed();
		void onZoomOutPressed();
		void onSelectionToolPressed();
		void onAddPointToolPressed();
		void onToolSelected( FigureToolInterface *tool );
		void onFrameChanged( int frame, bool played );
		void onRenameFigure( int layer, int obj, const QString &newName );
		void onRenameLayer( int layer, const QString &newName );
		void onFigureVisibleChange( int layer, int obj, bool visible );
		void onLayerVisibleChange( int layer, bool visible );
		void onFigureBlockedChange( int layer, int obj, bool blocked );
		void onLayerBlockedChange( int layer, bool blocked );
		void onFigureSelected( int layer, int  obj);
		void onAddLayer(bool visible, bool blocked, const QString &name);
		void onFigureDeletedFromLayersEditor( int layer, int obj );
		void onExitMenu();
		void onToggleMainToolbarWindow( bool visible );
		void onPenChanged( const QPen &p );
		void onBrushChanged( const QBrush &b );
		void onGroupSelected();
		void onUngroupSelected();
		void onShowHelp();
		void onAbout();
		void onAboutQt();

        void onRPWMouseMove(int x, int y);


	protected:

		virtual void closeEvent( QCloseEvent *event )
		{
			if( QMessageBox::question( this,tr("Exit from MultiFusion."),tr("Are you shure to exit from MultiFusion?"),
				QMessageBox::Ok | QMessageBox::Cancel,
				QMessageBox::Cancel ) == QMessageBox::Cancel )
			{
				event->ignore();
				return;
			}

			event->accept();
		}

	private:

		PaintWidget *painter;

		plugin::PluginsManager manager;

		QMenuBar *mainMenu;
		/** Menus **/
		QMenu *helpMenu;
		QMenu *windowsMenu;
		QMenu *editMenu;
		QMenu * fileMenu;

		QAction *deleteAction;
		QAction *copyAction;
		QAction *cutAction;
		QAction *pasteAction;
		QAction *groupAction;
		QAction *ungroupAction;
		QAction *toSplineAction;
		QAction *showBezier;

		bool processSignals;
		QString lastSavedFile;

		QToolBar *fileToolBar;

        QLabel* m_plblX;
        QLabel* m_plblY;
};

#endif /* __MAINWINDOW_H__ */
