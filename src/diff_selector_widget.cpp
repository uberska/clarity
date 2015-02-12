#include <iostream>

#include <QString>

#include "diff_selector_widget.h"
#include "idiffable.h"


DiffSelectorWidgetItem::DiffSelectorWidgetItem(IDiffable* pDiffable, QListWidget* parent /*= 0*/) :
	QListWidgetItem(parent),
	mpDiffable(pDiffable) {

	this->setText(QString::fromStdString(pDiffable->getDisplayString()));
}


DiffSelectorWidget::DiffSelectorWidget(QWidget* parent /*= 0*/) :
	QListWidget(parent) {

}


void DiffSelectorWidget::addDiffable(IDiffable* pDiffable) {
	DiffSelectorWidgetItem* pItem = new DiffSelectorWidgetItem(pDiffable);
	this->addItem(pItem);
}
