#pragma once


#include <QTextDocument>
#include <QPlainTextEdit>
#include <QTextEdit>
#include <QObject>
#include "src/diff_model_text.h"


class QObject;
class QWidget;


class DiffTextUIUnifiedDocument : public QTextDocument {
	Q_OBJECT

public:

	DiffTextUIUnifiedDocument(QObject* parent = 0);

	void updateFromModel(const DiffModelText& model);
};


class DiffTextUIUnified : public QTextEdit {
	Q_OBJECT

public:
	DiffTextUIUnified(QWidget* parent = 0);

	void updateFromModel(const DiffModelText& model);
};


class DiffTextEditDocument : public QTextDocument {
	Q_OBJECT

public:
	enum Type {
		ONE,
		TWO
	};

	DiffTextEditDocument(Type type, QObject* parent = 0);

	void updateFromModel(const DiffModelText& model);

private:
	Type mType;
};


class DiffTextEdit : public QTextEdit {
	Q_OBJECT

public:
	DiffTextEdit(DiffTextEditDocument::Type type, QWidget* parent = 0);

	void updateFromModel(const DiffModelText& model);

private:
	DiffTextEditDocument::Type mType;
};
