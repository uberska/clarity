#pragma once


#include <string>

#include <QWidget>


class IDiffable {
public:
	virtual ~IDiffable() {};

	virtual std::string getDisplayString() const = 0;

	virtual QWidget* getQWidget() const = 0;
};