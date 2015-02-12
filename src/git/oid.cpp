#include "oid.h"

#include <iostream>

#include <git2.h>

#include "exception.h"


OId::OId(const OId& oId) {
	git_oid_cpy(&mOId, &oId.mOId);
}


OId::OId(git_oid* pOId) {
	if (pOId != NULL) {
		git_oid_cpy(&mOId, pOId);
	} else {
		for (int i = 0; i < GIT_OID_RAWSZ; i++) {
			mOId.id[i] = 0;
		}
	}
}


std::ostream& operator<< (std::ostream &stream, const OId& oId)
{
	char oidStr[41];
	oidStr[40] = '\0';
	git_oid_fmt(oidStr, &oId.mOId);

	return stream << "{\"type\":\"OId\",\"value\":\"" << oidStr << "\"}";
}
