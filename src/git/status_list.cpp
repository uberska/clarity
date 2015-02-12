#include "status_list.h"


StatusList::StatusList(git_status_list* pStatusList) :
	mpStatusList(pStatusList)
{
}


StatusList::~StatusList()
{
	git_status_list_free(mpStatusList);
}


size_t StatusList::getEntryCount()
{
	return git_status_list_entrycount(mpStatusList);
}


StatusEntry StatusList::getEntryByIndex(unsigned int index)
{
	return StatusEntry(git_status_byindex(mpStatusList, index));
}
