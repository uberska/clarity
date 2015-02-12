#include <iostream>
#include <sstream>
#include "strings.h"


/* TODO: attempt to make this static once it works */
template <typename ContainerType>
static ContainerType getLineContainer(const std::string& linesStr) {
	ContainerType lineContainer;

	std::istringstream linesStream(linesStr);

	std::string line;

	while (getline(linesStream, line)) {
		lineContainer.push_back(line);
	}

	return lineContainer;
}


StringDeque getLineDeque(const std::string& linesStr) {
	return getLineContainer<StringDeque>(linesStr);
}


StringList getLineList(const std::string& linesStr) {
	return getLineContainer<StringList>(linesStr);
}


void printStringList(const StringList& stringList) {
	StringList::const_iterator iterator;
	for ( iterator = stringList.begin(); iterator != stringList.end(); ++iterator ) {
		std::cout << *iterator << std::endl;
	}
}


std::string getRangeString(const StringDeque& lines, const UnsignedIntRange& range) {
	std::stringstream stream;

	for (unsigned int index = range.getStart(); index <= range.getEnd(); index++) {
		stream << lines[index] << "\n";
	}

	return stream.str();
}