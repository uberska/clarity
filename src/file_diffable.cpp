#include <sstream>
#include "src/diff_text.h"
#include "src/diff_text_edit.h"
#include "src/file_diffable.h"


FileDiffable::FileDiffable(const std::string& onePath, const std::string& twoPath) :
	mOnePath(onePath),
	mTwoPath(twoPath) {

}


std::string FileDiffable::getDisplayString() const {
	std::stringstream displayStringStream;
	displayStringStream << mOnePath << " -> " << mTwoPath;
	return displayStringStream.str();
}


QWidget* FileDiffable::getQWidget() const {
	DiffModelText model;
	diffTextFromFiles(model, mOnePath, mTwoPath);

	DiffTextUIUnified* pTextUI = new DiffTextUIUnified();
	pTextUI->updateFromModel(model);

	return pTextUI;
}