#pragma once


#include <QListWidget>
#include <QListWidgetItem>
#include <QObject>


class IDiffable;
class QWidget;


class DiffSelectorWidgetItem : public QObject, public QListWidgetItem {
	Q_OBJECT

public:
	DiffSelectorWidgetItem(IDiffable* pDiffable, QListWidget* parent = 0);

	IDiffable* getDiffable() { return mpDiffable; }

private:
	IDiffable* mpDiffable;
};


class DiffSelectorWidget : public QListWidget {
	Q_OBJECT

public:
	DiffSelectorWidget(QWidget* parent = 0);

	void addDiffable(IDiffable* pDiffable);
};