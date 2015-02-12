#pragma once


#include <stdexcept>


class GitException : public std::exception
{
public:
	GitException();
	~GitException() throw() {};

	const char* what() const throw();
private:
	std::string mErrorMessage;
};


int gitThrow(int returnValue);
