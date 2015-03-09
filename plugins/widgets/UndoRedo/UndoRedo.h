/**
 * UndoRedo.h
 *
 *  Created on: 13.06.2009
 *      Author: Alexey Kukushkin
 */

#ifndef UNDOREDO_H_
#define UNDOREDO_H_

#include <QObject>
#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include "./../../../pluginTool/Plugin.h"
#include "./../../../pluginTool/InterfacePlugin.h"
#include "./../../../interfaces/MainWindowInterface.h"
#include "./../../../interfaces/UndoRedoInterface.h"
#include "./../../../interfaces/GSRInterface.h"
#include "./../../../interfaces/RPWInterface.h"
#include "./../../../interfaces/PaintWidgetInterface.h"

class UndoRedo:public QWidget, public InterfacePlugin,  public UndoRedoInterface
{
	Q_OBJECT
	Q_INTERFACES( UndoRedoInterface )
	Q_INTERFACES( InterfacePlugin )

	private:

		MainWindowInterface* mainWin;
		PaintWidgetInterface* painter;
		GSRInterface* selection;
		RPWInterface* realPainter;
		QAction* Undo;
		QAction* Redo;
		int position;
		int maxLength;
		QVector <QObject* > history;

	public slots:

		void addToHistory(QString event)
		{
			//QMessageBox::warning(this,"dsa",event);
			int count = history.size();
			if( position != count-1 )
			{

				for(int i = position+1; i<count; i++ )
					delete history[i];

				for(int i = count-1; i>position; i-- )
					history.remove(i);

				history.append(realPainter->getUndo());
				position++;
			}
			else if( maxLength!= count)
			{
				history.append(realPainter->getUndo());
				position++;
			}
			else if(maxLength == count)
			{
				delete history[0];
				history.remove(0);
				history.append(realPainter->getUndo());
			}

			if(position == count-1)
				Redo->setEnabled(false);

			if(position > 0)
				Undo->setEnabled(true);
		}

		void onUndo()
		{
			if(position > 0)
			{
				position--;
				//QMessageBox::warning(this,"Trace1",QString::number(position));
				realPainter->setFromUndo(history[position]);
			}

			if(position == 0)
				Undo->setEnabled(false);
			int count = history.size();
			if(position != count-1 && count != 1)
				Redo->setEnabled(true);
		}

		void onRedo()
		{
			int count = history.size();
			if(position != count-1 && count != 1)
			{
				position++;
				realPainter->setFromUndo(history[position]);
			}

			if(position == count-1 || count == 1)
				Redo->setEnabled(false);

			if(position > 0)
				Undo->setEnabled(true);
		}

	public:

		virtual void createPlugin(QObject *parent, QString idParent,plugin::PluginsManager *manager)
		{
			if(idParent == "Main")
			{
				mainWin = MAINWINDOW(parent);
				if(mainWin!=0)
				{
					painter = PAINTWIDGETINTERFACE(mainWin->getPaintWidget());

					QMenu* editMenu = mainWin->getEditMenu();
					Undo = editMenu->addAction( tr( "Undo" ) );
					Undo->setShortcut( tr( "Ctrl+Z" ) );
					Undo->setEnabled(false);

					Redo = editMenu->addAction( tr( "Redo" ) );
					Redo->setShortcut( tr( "Ctrl+Y" ) );
					Redo->setEnabled(false);

					connect(Undo,SIGNAL( triggered( bool ) ), this, SLOT( onUndo() ) );
					connect(mainWin,SIGNAL(StateChanged(QString)),this,SLOT(addToHistory(QString)));
					connect(painter,SIGNAL(StateChanged(QString)),this,SLOT(addToHistory(QString)));
					connect(Redo,SIGNAL( triggered( bool ) ), this, SLOT( onRedo() ) );
					manager->addPlugins(this, "UndoRedo");
				}
			}
			else if(idParent == "RealPaint")
			{
				realPainter = RPWINTEFACE(parent);
				if(realPainter != 0)
				{
					selection = GSRINTEFACE(realPainter->getSelection());
					history.append(realPainter->getUndo());
					connect(realPainter,SIGNAL(StateChanged(QString)),this,SLOT(addToHistory(QString)));
					connect(selection,SIGNAL(StateChanged(QString)),this,SLOT(addToHistory(QString)));
					position = 0;
				}
			}

		}

		virtual QString getName()const
		{
			return "UndoRedo";
		}

		UndoRedo( plugin::PluginsManager *manager ):maxLength(10){}

		~UndoRedo(){}


};

#endif /* UNDOREDO_H_ */
