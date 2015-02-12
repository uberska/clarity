#pragma once


#include <git2.h>
#include <string>

#include "oid.h"
#include "status.h"


class StatusEntry
{
public:
	explicit StatusEntry(const git_status_entry* pEntry);

	Status getStatus() const;

	std::string toString() const;

	OId getHeadToIndexOldFileOId();
	OId getHeadToIndexNewFileOId();
	OId getIndexToWorkDirOldFileOId();
	OId getIndexToWorkDirNewFileOId();

	std::string getHeadToIndexOldFilePath();
	std::string getHeadToIndexNewFilePath();
	std::string getIndexToWorkDirOldFilePath();
	std::string getIndexToWorkDirNewFilePath();

private:
	const git_status_entry* mpEntry;
};
