#include "diff_text_edit.h"

#include <limits>

#include <QtGui>
#include <QPlainTextDocumentLayout>
#include <QTextCursor>

#include "diff_match_patch/diff_match_patch.h"
#include "strings.h"


DiffTextUIUnified::DiffTextUIUnified(QWidget* parent /*= 0*/) :
	QTextEdit(parent) {

	this->setLineWrapMode(QTextEdit::NoWrap);

	this->setDocument(new DiffTextUIUnifiedDocument(this));

	this->setReadOnly(true);
}

void DiffTextUIUnified::updateFromModel(const DiffModelText& model) {
	QTextDocument* pDocument = this->document();
	DiffTextUIUnifiedDocument* pDiffDocument = dynamic_cast<DiffTextUIUnifiedDocument*>(pDocument);
	pDiffDocument->updateFromModel(model);
	this->moveCursor(QTextCursor::Start);
}


DiffTextUIUnifiedDocument::DiffTextUIUnifiedDocument(QObject* parent /*= 0*/) :
	QTextDocument(parent) {

}


static void addDiffTextUIUnifiedDocumentChunk(
	QTextBlockFormat& blockFormat,
	QTextCursor& cursor,
	UnsignedIntRange& range,
	StringDeque& lineList,
	QTextCharFormat& format,
	QColor* pColor = NULL
	)
{
	if (pColor) {
		blockFormat.setBackground(QBrush(*pColor));
	} else {
		blockFormat.clearBackground();
	}
	cursor.setBlockFormat(blockFormat);

	for (unsigned int index = range.getStart(); index <= range.getEnd(); index++) {
		std::string line = lineList[index];

		cursor.insertText(QString::fromStdString(line), format);
		// TODO: what to do about newlines in getLineList...
		cursor.insertText(QString::fromStdString("\n"), format);
	}
}


static void addDiffTextUIUnifiedDocumentChunkDiff(
	QList<Diff>& diffs,
	QColor& sameColor,
	QColor& differentColor,
	Operation skipOperation,
	QTextCursor& cursor,
	QTextBlockFormat& blockFormat,
	QTextCharFormat& format
	)
{
	blockFormat.setBackground(QBrush(sameColor));
	cursor.setBlockFormat(blockFormat);

	foreach(Diff diff, diffs) {
		if (diff.operation == INSERT || diff.operation == DELETE) {
			format.setBackground(differentColor);
		} else if (diff.operation == EQUAL) {
			format.clearBackground();
		}

		if (diff.operation != skipOperation) {
			cursor.insertText(diff.text, format);
		}
	}
}


void DiffTextUIUnifiedDocument::updateFromModel(const DiffModelText& model) {
	std::string oneContents = model.getOneContents();
	std::string twoContents = model.getTwoContents();

	StringDeque oneLineList = getLineDeque(oneContents);
	StringDeque twoLineList = getLineDeque(twoContents);

	const DiffModelTextChunkList& chunks = model.getChunks();

	QTextCursor cursor(this);
	QTextBlockFormat blockFormat = cursor.blockFormat();
	QTextCharFormat format;

	QColor addedColor(0, 150, 110);
	QColor removedColor(220, 0, 0);
	QColor modifiedOneColor(245, 180, 180);
	QColor modifiedTwoColor(175, 225, 210);

	DiffModelTextChunkList::const_iterator iterator;
	for ( iterator = chunks.begin(); iterator != chunks.end(); ++iterator ) {
		const DiffModelTextChunk chunk = *iterator;

		UnsignedIntRange oneRange = chunk.getOneRange();
		UnsignedIntRange twoRange = chunk.getTwoRange();

		if (chunk.getStatus() == DiffModelTextChunk::ADDED) {
			addDiffTextUIUnifiedDocumentChunk(blockFormat, cursor,
				twoRange, twoLineList, format, &addedColor);
			/*
			blockFormat.setBackground(QBrush(QColor(0, 150, 110)));
			cursor.setBlockFormat(blockFormat);

			for (unsigned int index = twoRange.getStart(); index <= twoRange.getEnd(); index++) {
				std::string line = twoLineList[index];

				cursor.insertText(QString::fromStdString(line), format);
				// TODO: what to do about newlines in getLineList...
				cursor.insertText(QString::fromStdString("\n"), format);
			}
			*/
		} else if (chunk.getStatus() == DiffModelTextChunk::REMOVED) {
			addDiffTextUIUnifiedDocumentChunk(blockFormat, cursor,
				oneRange, oneLineList, format, &removedColor);
		} else if (chunk.getStatus() == DiffModelTextChunk::MODIFIED) {
			std::string oneRangeString = getRangeString(oneLineList, oneRange);
			std::string twoRangeString = getRangeString(twoLineList, twoRange);

			QString oneRangeQString = QString::fromStdString(oneRangeString);
			QString twoRangeQString = QString::fromStdString(twoRangeString);

			diff_match_patch dmp;

			/* TODO: can we abstractize this so we can get rid of passing in deadline */
			clock_t deadline = std::numeric_limits<clock_t>::max();
			QList<Diff> diffs = dmp.diff_lineMode(oneRangeQString, twoRangeQString, deadline);

			addDiffTextUIUnifiedDocumentChunkDiff(diffs, modifiedOneColor, removedColor,
				INSERT, cursor, blockFormat, format);
			addDiffTextUIUnifiedDocumentChunkDiff(diffs, modifiedTwoColor, addedColor,
				DELETE, cursor, blockFormat, format);

			/*
			QString html = dmp.diff_prettyHtml(diffs);
			blockFormat.clearBackground();
			cursor.setBlockFormat(blockFormat);
			cursor.insertText(html, format);
			// TODO: what to do about newlines in getLineList...
			cursor.insertText(QString::fromStdString("\n"), format);
			*/
			/* GOOD CODE
			addDiffTextUIUnifiedDocumentChunk(blockFormat, cursor,
				oneRange, oneLineList, format, &modifiedOneColor);
			addDiffTextUIUnifiedDocumentChunk(blockFormat, cursor,
				twoRange, twoLineList, format, &modifiedTwoColor);*/
		} else if (chunk.getStatus() == DiffModelTextChunk::SAME) {
			addDiffTextUIUnifiedDocumentChunk(blockFormat, cursor,
				oneRange, oneLineList, format, NULL /*pColor*/);
			//SAME AS:
			//addDiffTextUIUnifiedDocumentChunk(blockFormat, cursor,
			//	twoRange, twoLineList, format, NULL /*pColor*/);
		}

		/*
		DiffModelTextChunkList::const_iterator nextIterator = iterator;
		std::advance(nextIterator, 1);
		if (nextIterator != chunks.end()) {
			UnsignedIntRange nextOneRange = nextIterator->getOneRange();
			unsigned int thisEnd = oneRange.getEnd();
			unsigned int nextStart = nextOneRange.getStart();
			if (nextStart - thisEnd > 1) {
				UnsignedIntRange sameRange(thisEnd + 1, nextStart - 1);
				addDiffTextUIUnifiedDocumentChunk(blockFormat, cursor,
					sameRange, oneLineList, format, NULL /-*pColor*-/);
			}
		}
		*/
	}
}


DiffTextEdit::DiffTextEdit(DiffTextEditDocument::Type type, QWidget* parent /*= 0*/) :
	QTextEdit(parent),
	mType(type) {

	this->setLineWrapMode(QTextEdit::NoWrap);

	this->setDocument(new DiffTextEditDocument(type, this));
}

void DiffTextEdit::updateFromModel(const DiffModelText& model) {
	QTextDocument* pDocument = this->document();
	DiffTextEditDocument* pDiffDocument = dynamic_cast<DiffTextEditDocument*>(pDocument);
	pDiffDocument->updateFromModel(model);
}


DiffTextEditDocument::DiffTextEditDocument(Type type, QObject* parent /*= 0*/) :
	QTextDocument(parent),
	mType(type) {

}

void DiffTextEditDocument::updateFromModel(const DiffModelText& model) {
	std::string contents;

	if (mType == ONE) {
		contents = model.getOneContents();
	} else {
		contents = model.getTwoContents();
	}

	QString qContents = QString::fromStdString(contents);

	StringList lineList = getLineList(contents);

	QTextCursor cursor(this);

	StringList::const_iterator iterator;
	for ( iterator = lineList.begin(); iterator != lineList.end(); ++iterator ) {
		QTextBlockFormat blockFormat = cursor.blockFormat();

		if (true) {
			blockFormat.setBackground(QBrush(QColor(192,192,192)));
		} else {
			blockFormat.clearBackground();
		}

		cursor.setBlockFormat(blockFormat);
		QTextCharFormat format;
		cursor.insertText(QString::fromStdString(*iterator), format);
		// TODO: what to do about newlines in getLineList...
		cursor.insertText(QString::fromStdString("\n"), format);
	}
}