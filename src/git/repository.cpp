#include <iostream>
#include <stdexcept>

#include <git2.h>

#include "blob.h"
#include "exception.h"
#include "repository.h"


Repository::Repository(const Repository& repository) :
	mpRepository(repository.mpRepository) {

}


Repository::Repository(const std::string& gitFolderPath) {
	gitThrow(git_repository_open(&mpRepository, gitFolderPath.c_str()));
}


Repository::~Repository() {
	// TODO: need to fix this... shared_ptr the repository???
	//git_repository_free(mpRepository);
}


std::string Repository::getCurrentBranchName() const {
	const char* pBranchName = NULL;

	git_reference* pHead = NULL;
	gitThrow(git_repository_head(&pHead, mpRepository));

	git_ref_t type = git_reference_type(pHead);

	if (type == GIT_REF_OID) {
		gitThrow(git_branch_name(&pBranchName, pHead));
	} else if (type == GIT_REF_SYMBOLIC) {
		throw std::runtime_error("Not Yet Implemented - getCurrentBranchName of symbolic references.");
	}

	return pBranchName;
}


const StatusList Repository::getStatusList(const StatusOptions& options) const {
	std::cout << "\nbegin\n";
	git_status_options gitStatusOptions = GIT_STATUS_OPTIONS_INIT;
	gitStatusOptions.show = static_cast<git_status_show_t>(options.getShow());
	gitStatusOptions.flags = options.getFlags();

		std::cout << "\ntwo\n";

	git_status_list* pStatusList;
	gitThrow(git_status_list_new(&pStatusList, mpRepository, &gitStatusOptions));

	std::cout << "\nend\n";

	return StatusList(pStatusList);
}


/*
Blob Repository::lookupBlob(const OId& oId) {
	std::cout << "NON-CONST LOOK: " << oId << "\n";
	const Repository* pConstThis = static_cast<const Repository*>(this);
	return const_cast<Blob&>(pConstThis->lookupBlob(oId));
}
*/

const Blob Repository::lookupBlob(const OId& oId) const {
	std::cout << "CONST LOOK\n";
	git_blob* pBlob = NULL;

	std::cout << "DEBUG OID: " << oId << "\n";

	// TODO: this shouldn't happen, but it does...
	if (!git_oid_iszero(&oId.mOId)) {
		std::cout << "IN-IF\n";
		gitThrow(git_blob_lookup(&pBlob, mpRepository, &oId.mOId));
		std::cout << "DONE-IF\n";
	}

	std::cout << "DONE MAKE BLOB\n";

	return Blob(pBlob);
}
