#include <git2.h>

#include "exception.h"


GitException::GitException()
{
	const git_error* pErr = giterr_last();

	if (pErr != NULL) {
		mErrorMessage = pErr->message;
		giterr_clear();
	}
}


const char* GitException::what() const throw()
{
	return mErrorMessage.c_str();
}


int gitThrow(int returnValue)
{
	if (returnValue < 0) {
		throw GitException();
	}
	return returnValue;
}
