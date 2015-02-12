#pragma once


#include <iostream>
#include <string>


template<typename Type>
void outputPointer(std::ostream &stream, const std::string& name, const Type* pObj)
{
	stream << "\"" << name << "\":";

	if (pObj != NULL) {
		stream << *pObj;
	} else {
		stream << "\"NULL\"";
	}
}
