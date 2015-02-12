#include <sstream>

#include "diff_delta.h"
#include "output.h"
#include "status_entry.h"


StatusEntry::StatusEntry(const git_status_entry* pEntry) :
	mpEntry(pEntry)
{
}


Status StatusEntry::getStatus() const
{
	return Status(mpEntry->status);
}


std::string StatusEntry::toString() const
{
	std::stringstream thisAsString;

	thisAsString << "{\"type\":\"StatusEntry\", \"status\":\"" << this->getStatus() << "\",";
	outputPointer<git_diff_delta>(thisAsString, "head_to_index", mpEntry->head_to_index);
	thisAsString << ",";
	outputPointer<git_diff_delta>(thisAsString, "index_to_workdir", mpEntry->index_to_workdir);
	thisAsString << "}";

	return thisAsString.str();
}


OId StatusEntry::getHeadToIndexOldFileOId()
{
	// TODO: FIX OIds to copy
	if (mpEntry && mpEntry->head_to_index) {
		return OId(&(mpEntry->head_to_index->old_file.oid));
	} else {
		return OId(NULL);
	}
}


OId StatusEntry::getHeadToIndexNewFileOId()
{
	if (mpEntry && mpEntry->head_to_index) {
		return OId(&(mpEntry->head_to_index->new_file.oid));
	} else {
		return OId(NULL);
	}
}


OId StatusEntry::getIndexToWorkDirOldFileOId()
{
	if (mpEntry && mpEntry->index_to_workdir) {
		return OId(&(mpEntry->index_to_workdir->old_file.oid));
	} else {
		return OId(NULL);
	}
}


OId StatusEntry::getIndexToWorkDirNewFileOId()
{
	if (mpEntry && mpEntry->index_to_workdir) {
		return OId(&(mpEntry->index_to_workdir->new_file.oid));
	} else {
		return OId(NULL);
	}
}


std::string StatusEntry::getHeadToIndexOldFilePath()
{
	if (mpEntry && mpEntry->head_to_index) {
		return mpEntry->head_to_index->old_file.path;
	} else {
		return "";
	}
}


std::string StatusEntry::getHeadToIndexNewFilePath()
{
	if (mpEntry && mpEntry->head_to_index) {
		return mpEntry->head_to_index->new_file.path;
	} else {
		return "";
	}
}


std::string StatusEntry::getIndexToWorkDirOldFilePath()
{
	if (mpEntry && mpEntry->index_to_workdir) {
		return mpEntry->index_to_workdir->old_file.path;
	} else {
		return "";
	}
}


std::string StatusEntry::getIndexToWorkDirNewFilePath()
{
	if (mpEntry && mpEntry->index_to_workdir) {
		return mpEntry->index_to_workdir->new_file.path;
	} else {
		return "";
	}
}

