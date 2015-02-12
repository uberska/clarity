#pragma once


#include <deque>
#include <list>
#include <string>
#include "range.h"


typedef std::deque<std::string> StringDeque;
typedef std::list<std::string> StringList;


StringDeque getLineDeque(const std::string& linesStr);
StringList getLineList(const std::string& linesStr);

std::string getRangeString(const StringDeque& lines, const UnsignedIntRange& range);

void printStringList(const StringList& stringList);
