#pragma once


#include <ostream>

#include <git2.h>


class Repository;


class OId
{
public:
	OId(const OId& oId);
	explicit OId(git_oid* pOId);

	friend Repository;

	friend std::ostream& operator<< (std::ostream &stream, const OId& oId);

private:
	git_oid mOId;
};


std::ostream& operator<< (std::ostream &stream, const OId& oId);
