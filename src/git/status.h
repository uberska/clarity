#pragma once


#include <git2.h>
#include <string>


class Status
{
public:
	explicit Status(const git_status_t& flags) : mFlags(flags) {}

	bool isCurrent() const { return mFlags == GIT_STATUS_CURRENT; }

	bool isNewInIndex() const { return mFlags & GIT_STATUS_INDEX_NEW; }
	bool isModifiedInIndex() const { return mFlags & GIT_STATUS_INDEX_MODIFIED; }
	bool isDeletedInIndex() const { return mFlags & GIT_STATUS_INDEX_DELETED; }
	bool isRenamedInIndex() const { return mFlags & GIT_STATUS_INDEX_RENAMED; }
	bool hasTypeChangedInIndex() const { return mFlags & GIT_STATUS_INDEX_TYPECHANGE; }

	bool isNewInWorkDir() const { return mFlags & GIT_STATUS_WT_NEW; }
	bool isModifiedInWorkDir() const { return mFlags & GIT_STATUS_WT_MODIFIED; }
	bool isDeletedInWorkDir() const { return mFlags & GIT_STATUS_WT_DELETED; }
	bool hasTypeChangedInWorkDir() const { return mFlags & GIT_STATUS_WT_TYPECHANGE; }
	bool isRenamedInWorkDir() const { return mFlags & GIT_STATUS_WT_RENAMED; }

	bool isIgnored() const { return mFlags & GIT_STATUS_IGNORED; }

	std::string toString() const;

	friend std::ostream& operator<< (std::ostream& stream, const Status& status) {
		stream << status.toString();
		return stream;
	}

private:
	git_status_t mFlags;
};
