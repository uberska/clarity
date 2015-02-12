#pragma once


#include <string>

#include "src/diff_model_text.h"


void readFile(std::string& contents, const std::string& filename);


void writeFile(std::string& contents, const std::string& filename);


void diffTextFromFiles(DiffModelText& model, const std::string& one, const std::string& two);


void diffTextFromContents(
	DiffModelText& model,
	const std::string& oneFileContents,
	const std::string& twoFileContents);