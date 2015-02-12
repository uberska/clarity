#pragma once


#include <string>
#include "src/idiffable.h"


class FileDiffable : public IDiffable {
public:
	FileDiffable(const std::string& onePath, const std::string& twoPath);

	std::string getDisplayString() const;

	QWidget* getQWidget() const;

private:
	std::string mOnePath;
	std::string mTwoPath;
};