#include "status.h"


std::string Status::toString() const {
	if (isCurrent()) { return "current"; }
	if (isNewInIndex()) { return "index-new"; }
	if (isModifiedInIndex()) { return "index-modified"; }
	if (isDeletedInIndex()) { return "index-deleted"; }
	if (isRenamedInIndex()) { return "index-renamed"; }
	if (hasTypeChangedInIndex()) { return "index-typechanged"; }
	if (isNewInWorkDir()) { return "workdir-new"; }
	if (isModifiedInWorkDir()) { return "workdir-modified"; }
	if (isDeletedInWorkDir()) { return "workdir-deleted"; }
	if (hasTypeChangedInWorkDir()) { return "workdir-typechanged"; }
	if (isRenamedInWorkDir()) { return "workdir-renamed"; }
	if (isIgnored()) { return "ignored"; }

	return "invalid";
}
