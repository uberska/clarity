#pragma once


#include <string>
#include "src/idiffable.h"
#include "src/git/oid.h"
#include "src/git/repository.h"


class GitDiffable : public IDiffable {
public:
	GitDiffable(Repository& repo, OId& oneOId, OId& twoOId, const std::string& displayString);

	std::string getDisplayString() const;

	QWidget* getQWidget() const;

private:
	Repository mRepo;
	OId mOneOId;
	OId mTwoOId;
	std::string mDisplayString;
};