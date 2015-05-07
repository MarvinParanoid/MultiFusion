#ifndef MAINWINDOWINTERFACE_H_
#define MAINWINDOWINTERFACE_H_

#include <QMainWindow>
#include "./../pluginTool/Plugin.h"


class MainWindowInterface :public QMainWindow
{
	signals:
		 /**
		 * Отправляется после выполнения события, которое нам нужно сохранить в истории
		 * @param event имя события
		 */
		void StateChanged(QString event);
	public:
        virtual QMenu * getFileMenu()const = 0;
        virtual QMenu * getEditMenu()const = 0;
        virtual QMenu * getObjectMenu()const = 0;
		virtual QMenu * getWindowsMenu()const = 0;
		virtual QMenu * getHelpMenu()const = 0;
		virtual QMenuBar * getMainMenu()const = 0;
		virtual QToolBar * getToolBar()const = 0;
		virtual QObject * getPaintWidget()const = 0;
		virtual bool getProcessSignals()const = 0;
		virtual void setProcessSignals(bool ps) = 0;
		MainWindowInterface(){}
		virtual ~MainWindowInterface(){}

	public slots:
		void onStyleFigure();
		void onShowBezier();
		void onShowEditMenu();
		void onDelete();
		void onCut();
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
		void onFrameChanged( int frame, bool played );
		void onRenameLayer( int layer, const QString &newName );
		void onRenameFigure( int layer, int obj, const QString &newName );
		void onAddLayer(bool visible, bool blocked, const QString &name);
		void onFigureBlockedChange( int layer, int obj, bool blocked );
		void onLayerBlockedChange( int layer, bool blocked );
		void onFigureVisibleChange( int layer, int obj, bool visible );
		void onLayerVisibleChange( int layer, bool visible );
		void onFigureDeletedFromLayersEditor( int layer, int obj );
		void onExitMenu();
		void onToggleMainToolbarWindow( bool visible );
		void onPenChanged( const QPen &p );
		void onBrushChanged( const QBrush &b );
		void onGroupSelected();
		void onUngroupSelected();
		void onFigureSelected( int layer, int  obj);
		void onShowHelp();
		void onAbout();
		void onAboutQt();
        void onRPWMouseMove(QPoint,QPoint,qreal);

	protected:
		virtual void closeEvent( QCloseEvent *event )= 0;

	private:

		QMenuBar *mainMenu;

		/** Menus **/
		QMenu *editMenu;
		QMenu * fileMenu;

		bool processSignals;
		QString lastSavedFile;

		QToolBar *fileToolBar;
};


Q_DECLARE_INTERFACE( MainWindowInterface, "com.alxn1.sacs2.MainWindowInterface/1.0" )

template < class T > inline MainWindowInterface * MAINWINDOW( T *o )
{
	return qobject_cast< MainWindowInterface* >( o );
}

#endif
