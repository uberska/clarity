#include "src/git_diffable.h"

#include "src/diff_text.h"
#include "src/diff_text_edit.h"
#include "src/git/blob.h"
#include "src/git/repository.h"


GitDiffable::GitDiffable(Repository& repo, OId& oneOId, OId& twoOId, const std::string& displayString) :
	mRepo(repo),
	mOneOId(oneOId),
	mTwoOId(twoOId),
	mDisplayString(displayString) {

}


std::string GitDiffable::getDisplayString() const {
	return mDisplayString;
}


QWidget* GitDiffable::getQWidget() const {
	std::cout << "git diffable getqwidget start\n";
	std::cout << mRepo.getCurrentBranchName() << "\n";
	std::cout << "done checking repo\n";
	const Blob oneBlob = mRepo.lookupBlob(mOneOId);
	std::cout << "blob 1\n";
	std::cout << mOneOId << "\n";
	std::cout << oneBlob.contentAsString() << "\n";
	const Blob twoBlob = mRepo.lookupBlob(mTwoOId);
	std::cout << "blob 2\n";
	std::cout << mTwoOId << "\n";
	std::cout << twoBlob.contentAsString() << "\n";

	DiffModelText model;
	diffTextFromContents(model, oneBlob.contentAsString(), twoBlob.contentAsString());

	std::cout << "diff done\n";

	DiffTextUIUnified* pTextUI = new DiffTextUIUnified();
	pTextUI->updateFromModel(model);
	std::cout << "git diffable getqwidget end\n";
	return pTextUI;
}