/**
 * BoolOperations.h
 *
 *  Created on: 20.05.2015
 *      Author: Sergey Korolev
 */

#ifndef BOOLOPERATIONS_H
#define BOOLOPERATIONS_H

#include <QWidget>
#include <QObject>
#include <QMainWindow>
#include <QDockWidget>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include "./../../pluginTool/Plugin.h"
#include "./../../pluginTool/InterfacePlugin.h"
#include "./../../interfaces/MainWindowInterface.h"
#include "./../../interfaces/PaintWidgetInterface.h"
#include "./../../interfaces/RPWInterface.h"
#include "./../../interfaces/GSRInterface.h"
#include "./../../interfaces/BoolOperationsInterface.h"
#include "./../../paintWidget/GObject.h"
#include "./../../paintWidget/GContainer.h"
#include "./../../paintWidget/GVectorFigure.h"
#include "ui_booloperations.h"

class BoolOperations: public QWidget, public BoolOperationsInterface, public InterfacePlugin
{
	Q_OBJECT
	Q_INTERFACES(BoolOperationsInterface)
	Q_INTERFACES(InterfacePlugin)

private:
	QAction* unionAction;
	QAction* differenceAction;
	QAction* intersectionAction;
	QAction* exclusionAction;
	QAction* devisionAction;

	MainWindowInterface* mainWin;
	PaintWidgetInterface* painter;
	RPWInterface* realPainter;
	GSRInterface* selection;
	GContainer* container;
	QDockWidget* dock;
	Ui::BoolOperationsDock ui;


public slots:
	void onUnion() {
		createUnion();
	}

	void onDifference() {
		createDifference();
	}

	void onIntersection() {
		createIntersection();
	}

	void onExclusion() {
		createExclusion();
	}

	void onDivision() {
		createDivision();
	}

public:
	void createUnion();
	void createDifference();
	void createIntersection();
	void createExclusion();
	void createDivision();

	virtual void createPlugin(QObject *parent, QString idParent, plugin::PluginsManager *manager)
	{
		if (idParent != "Main") return;
		mainWin = MAINWINDOW(parent);
		if (mainWin == 0) return;

		painter = PAINTWIDGETINTERFACE(mainWin->getPaintWidget());
		if (painter == 0) return;

		realPainter = RPWINTEFACE(painter->getRealPaintWidget());
		if (realPainter == 0) return;

		selection = GSRINTEFACE(realPainter->getSelection());
		if (selection == 0) return;

		dock = new QDockWidget(mainWin);
		dock->setWindowTitle(tr("Bool Operations"));
		this->setParent(dock);
		dock->setWidget(this);
		mainWin->addDockWidget(Qt::RightDockWidgetArea, dock);

		QMenu* objectMenu = mainWin->getObjectMenu();
		objectMenu->addSeparator();

		unionAction = objectMenu->addAction(QIcon(":/BoolOperations/images/union.png"), tr("Union"));
		unionAction->setShortcut(tr("Ctrl++"));
		unionAction->setEnabled(true);
		connect(unionAction, SIGNAL(triggered(bool)), this, SLOT(onUnion()));

		differenceAction = objectMenu->addAction(QIcon(":/BoolOperations/images/subtract.png"), tr("Difference"));
		differenceAction->setShortcut(tr("Ctrl+-"));
		differenceAction->setEnabled(true);
		connect(differenceAction, SIGNAL(triggered(bool)), this, SLOT(onDifference()));

		intersectionAction = objectMenu->addAction(QIcon(":/BoolOperations/images/intersect.png"), tr("Intersection"));
		intersectionAction->setShortcut(tr("Ctrl+*"));
		intersectionAction->setEnabled(true);
		connect(intersectionAction, SIGNAL(triggered(bool)), this, SLOT(onIntersection()));

		exclusionAction = objectMenu->addAction(QIcon(":/BoolOperations/images/exclude.png"), tr("Exclusion"));
		exclusionAction->setShortcut(tr("Ctrl+^"));
		exclusionAction->setEnabled(true);
		connect(exclusionAction, SIGNAL(triggered(bool)), this, SLOT(onExclusion()));

		devisionAction = objectMenu->addAction(QIcon(":/BoolOperations/images/divide.png"), tr("Division"));
		devisionAction->setShortcut(tr("Ctrl+/"));
		devisionAction->setEnabled(true);
		connect(devisionAction, SIGNAL(triggered(bool)), this, SLOT(onDivision()));

		objectMenu->addSeparator();
		manager->addPlugins(this, "BoolOperations");
	}

	virtual QString getName() const
	{
		return "BoolOperations";
	}

	BoolOperations(plugin::PluginsManager */*manager*/): mainWin(0), painter(0), realPainter(0) {
		ui.setupUi(this);
		connect(ui.unionButton, SIGNAL(clicked()), this, SLOT(onUnion()));
		connect(ui.subtractButton, SIGNAL(clicked()), this, SLOT(onDifference()));
		connect(ui.intersectButton, SIGNAL(clicked()), this, SLOT(onIntersection()));
		connect(ui.excludeButton, SIGNAL(clicked()), this, SLOT(onExclusion()));
		connect(ui.divideButton, SIGNAL(clicked()), this, SLOT(onDivision()));
	}

	~BoolOperations() {}
};

#endif

