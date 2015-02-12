#pragma once


#include <string>

#include "blob.h"
#include "oid.h"
#include "status_list.h"
#include "status_options.h"


struct git_repository;


class Repository {
public:
	Repository(const Repository& repository);
	Repository(const std::string& gitFolderPath);
	~Repository();

	std::string getCurrentBranchName() const;

	const StatusList getStatusList(const StatusOptions& options) const;

	//Blob lookupBlob(const OId& oId);
	const Blob lookupBlob(const OId& oId) const;

private:
	git_repository* mpRepository;
};