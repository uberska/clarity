#pragma once


#include <ostream>


struct git_blob;


class Blob
{
public:
	explicit Blob(git_blob* pBlob);

	const void* content() const;

	std::string contentAsString() const;

	unsigned int size() const;

	friend std::ostream& operator<< (std::ostream &stream, const Blob& blob);

private:
	git_blob* mpBlob;
};


std::ostream& operator<< (std::ostream &stream, const Blob& blob);