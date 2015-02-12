#include "status_options.h"


StatusOptions::StatusOptions() :
	mShow(static_cast<Show>(GIT_STATUS_SHOW_INDEX_AND_WORKDIR)),
	mFlags(static_cast<Flags>(GIT_STATUS_OPT_INCLUDE_UNTRACKED |
		GIT_STATUS_OPT_RECURSE_UNTRACKED_DIRS |
		GIT_STATUS_OPT_RENAMES_HEAD_TO_INDEX |
		GIT_STATUS_OPT_RENAMES_INDEX_TO_WORKDIR |
		GIT_STATUS_OPT_SORT_CASE_SENSITIVELY |
		GIT_STATUS_OPT_RENAMES_FROM_REWRITES))
{
}


StatusOptions::StatusOptions(const git_status_options& status_options) :
	mShow(static_cast<Show>(status_options.show)),
	mFlags(static_cast<Flags>(status_options.flags))
{
}
