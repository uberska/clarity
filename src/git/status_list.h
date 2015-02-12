#pragma once


#include <git2.h>


#include "status_entry.h"


class StatusList
{
public:
	explicit StatusList(git_status_list* pStatusList);
	~StatusList();

	size_t getEntryCount();

	StatusEntry getEntryByIndex(unsigned int index);

private:
	git_status_list* mpStatusList;
};
