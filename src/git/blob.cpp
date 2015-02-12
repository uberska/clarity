#include "blob.h"

#include <iostream>

#include "oid.h"
#include "repository.h"


Blob::Blob(git_blob* pBlob)
	: mpBlob(pBlob)
{
}


const void* Blob::content() const
{
	return git_blob_rawcontent(mpBlob);
}


std::string Blob::contentAsString() const {
	std::cout << "mpBlob: " << mpBlob << "\n";
	if (mpBlob) {
		std::cout << "Returning content\n" << this->size() << "\n";
		return static_cast<const char*>(this->content());
	} else {
		std::cout << "Returning empty string.\n" << this->size() << "\n";
		return "";
	}
}


size_t Blob::size() const
{
	if (mpBlob) {
		return git_blob_rawsize(mpBlob);
	} else {
		return 0;
	}
}


std::ostream& operator<< (std::ostream &stream, const Blob& blob)
{
	//return stream << "{\"type\":\"Blob\"}";
	if (blob.mpBlob) {
		return stream << static_cast<const char*>(blob.content());
	} else {
		return stream << "<NULL Blob>";
	}
}
