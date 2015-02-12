#pragma once


#include <QObject>


class DiffSelectorWidget;
class QSplitter;


class DiffController : public QObject {
	Q_OBJECT

public:
	DiffController(QSplitter* pSplitter, DiffSelectorWidget* pDiffSelectorWidget);

private slots:
	void onDiffSelected();

private:
	QSplitter* mpSplitter;
	DiffSelectorWidget* mpDiffSelectorWidget;
};
