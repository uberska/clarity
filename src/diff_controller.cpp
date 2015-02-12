#include "diff_controller.h"

#include <iostream>

#include <QSplitter>

#include "src/diff_selector_widget.h"
#include "src/idiffable.h"


DiffController::DiffController(QSplitter* pSplitter, DiffSelectorWidget* pDiffSelectorWidget) :
	mpSplitter(pSplitter),
	mpDiffSelectorWidget(pDiffSelectorWidget) {

	connect(mpDiffSelectorWidget, SIGNAL(itemSelectionChanged()), this, SLOT(onDiffSelected()));
}


void DiffController::onDiffSelected() {
	QByteArray splitterState(mpSplitter->saveState());

	DiffSelectorWidgetItem* pItem =
		static_cast<DiffSelectorWidgetItem*>(mpDiffSelectorWidget->currentItem());
	IDiffable* pDiffable = pItem->getDiffable();
	std::cout << "onDiffSelected\n";
	std::cout << pDiffable->getDisplayString() << "\n";

	QWidget* pWidget = pDiffable->getQWidget();

	// TODO: do we want to keep these widgets for future use???
	QWidget* pOldWidget = mpSplitter->widget(1);
	pOldWidget->setParent(NULL);
	pOldWidget->deleteLater();

	mpSplitter->addWidget(pWidget);

	mpSplitter->restoreState(splitterState);
}