#pragma once


#include <git2.h>


class StatusOptions
{
public:
	enum Show {
		IndexAndWorkDir = GIT_STATUS_SHOW_INDEX_AND_WORKDIR,
		IndexOnly = GIT_STATUS_SHOW_INDEX_ONLY,
		WorkDirOnly = GIT_STATUS_SHOW_WORKDIR_ONLY
	};

	enum Flags {
		IncludeUntracked = GIT_STATUS_OPT_INCLUDE_UNTRACKED,
		IncludeIgnored = GIT_STATUS_OPT_INCLUDE_IGNORED,
		IncludeUnmodified = GIT_STATUS_OPT_INCLUDE_UNMODIFIED,
		ExcludeSubmodules = GIT_STATUS_OPT_EXCLUDE_SUBMODULES,
		RecurseUntrackedDirs = GIT_STATUS_OPT_RECURSE_UNTRACKED_DIRS,
		DisablePathspecMatch = GIT_STATUS_OPT_DISABLE_PATHSPEC_MATCH,
		RecurseIgnoredDirs = GIT_STATUS_OPT_RECURSE_IGNORED_DIRS,
		RenamesHeadToIndex = GIT_STATUS_OPT_RENAMES_HEAD_TO_INDEX,
		RenamesIndexToWorkDir = GIT_STATUS_OPT_RENAMES_INDEX_TO_WORKDIR,
		SortCaseSensitively = GIT_STATUS_OPT_SORT_CASE_SENSITIVELY,
		SortCaseInsensitively = GIT_STATUS_OPT_SORT_CASE_INSENSITIVELY,
		RenamesFromRewrites = GIT_STATUS_OPT_RENAMES_FROM_REWRITES,
		NoRefresh = GIT_STATUS_OPT_NO_REFRESH
	};

	StatusOptions();
	explicit StatusOptions(const git_status_options& statusOptions);

	Show getShow() const { return mShow; };

	Flags getFlags() const { return mFlags; };

private:
	Show mShow;
	Flags mFlags;
};
