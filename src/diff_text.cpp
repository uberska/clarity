#include "diff_text.h"

#include <fstream>
#include <stdlib.h>
#include <sstream>
#include <stdio.h>

#include "src/command.h"
#include "src/strings.h"


void readFile(std::string& contents, const std::string& filename) {
	std::ifstream fileStream(filename.c_str());
	contents.assign((std::istreambuf_iterator<char>(fileStream)),
		(std::istreambuf_iterator<char>()));
}


void writeFile(const std::string& contents, const std::string& filename) {
	std::ofstream fileStream(filename.c_str());
	fileStream << contents;
	fileStream.close();
}


static bool isDiffFileLine(const std::string& line) {
	return line.length() == 0 || !isdigit(line[0]);
}


static UnsignedIntRange parseTextRange(const std::string& rangeText) {
	/* This functions converts from diffs 1-based lines to 0-based indices */
	if (rangeText.find(",") == std::string::npos) {
		return UnsignedIntRange(atoi(rangeText.c_str()) - 1);
	} else {
		unsigned int delimiterIndex = rangeText.find(",");

		return UnsignedIntRange(
			atoi(rangeText.substr(0, delimiterIndex).c_str()) - 1,
			atoi(rangeText.substr(delimiterIndex + 1).c_str()) - 1
			);
	}
}


static DiffModelTextChunk parseTextDiffLine(
	const DiffModelTextChunk::Status& status,
	const std::string& delimiter,
	const std::string& line) {

	unsigned int delimiterIndex = line.find(delimiter);

	UnsignedIntRange oneRange = parseTextRange(line.substr(0, delimiterIndex));
	UnsignedIntRange twoRange = parseTextRange(line.substr(delimiterIndex + 1));

	DiffModelTextChunk chunk(status, oneRange, twoRange);

	return chunk;
}


static void parseTextDiff(DiffModelText& model, const StringList& stringList) {
	/*
	This function assumes that model contains the contents of the files being diffed.
	*/
	model.clearChunks();

	std::string oneContents = model.getOneContents();
	StringList oneLineList = getLineList(oneContents);

	if (stringList.size() == 0) {
		UnsignedIntRange fullRange(
			0,
			oneLineList.size() - 1
			);
		DiffModelTextChunk sameChunk(DiffModelTextChunk::SAME, fullRange, fullRange);
		model.addChunk(sameChunk);
		return;
	}

	int lastOneEndIndex = -1;

	StringList::const_iterator iterator;
	for ( iterator = stringList.begin(); iterator != stringList.end(); ++iterator ) {
		if (iterator->find("c") != std::string::npos) {
			DiffModelTextChunk chunk = parseTextDiffLine(DiffModelTextChunk::MODIFIED, "c", *iterator);

			/* TODO: combine this with other ifs */
			if (chunk.getOneRange().getStart() - lastOneEndIndex > 1) {
				UnsignedIntRange oneRange(
					lastOneEndIndex + 1,
					chunk.getOneRange().getStart() - 1
					);
				UnsignedIntRange twoRange(
					chunk.getTwoRange().getStart() - oneRange.length(),
					chunk.getTwoRange().getStart() - 1
					);
				DiffModelTextChunk sameChunk(DiffModelTextChunk::SAME, oneRange, twoRange);
				model.addChunk(sameChunk);
			}

			model.addChunk(chunk);

			lastOneEndIndex = chunk.getOneRange().getEnd();
		} else if (iterator->find("a") != std::string::npos) {
			DiffModelTextChunk chunk = parseTextDiffLine(DiffModelTextChunk::ADDED, "a", *iterator);

			/* TODO: combine this with other ifs */
			if (chunk.getOneRange().getStart() - lastOneEndIndex > 1) {
				UnsignedIntRange oneRange(
					lastOneEndIndex + 1,
					chunk.getOneRange().getStart() - 1
					);
				UnsignedIntRange twoRange(
					chunk.getTwoRange().getStart() - oneRange.length(),
					chunk.getTwoRange().getStart() - 1
					);
				DiffModelTextChunk sameChunk(DiffModelTextChunk::SAME, oneRange, twoRange);
				model.addChunk(sameChunk);
			}

			model.addChunk(chunk);

			lastOneEndIndex = chunk.getOneRange().getEnd();
		} else if (iterator->find("d") != std::string::npos) {
			DiffModelTextChunk chunk = parseTextDiffLine(DiffModelTextChunk::REMOVED, "d", *iterator);

			/* TODO: combine this with other ifs */
			if (chunk.getOneRange().getStart() - lastOneEndIndex > 1) {
				UnsignedIntRange oneRange(
					lastOneEndIndex + 1,
					chunk.getOneRange().getStart() - 1
					);
				UnsignedIntRange twoRange(
					chunk.getTwoRange().getStart() - oneRange.length() + 1,
					chunk.getTwoRange().getStart() - 1 + 1
					/* TODO: note extra plus ones when combining in method */
					/* numbers in range of removed is what line the removed line is after,
					not which line number is the removed line */
					);
				DiffModelTextChunk sameChunk(DiffModelTextChunk::SAME, oneRange, twoRange);
				model.addChunk(sameChunk);
			}

			model.addChunk(chunk);

			lastOneEndIndex = chunk.getOneRange().getEnd();
		} else {
			std::cerr << "WARNING: invalid diff line: " << *iterator << std::endl;
		}
	}

	/* TODO: this could be optimized. Consider storing line list instead of string */
	const DiffModelTextChunkList& chunkList = model.getChunks();
	if ( lastOneEndIndex < ( (int) oneLineList.size() ) - 1 ) {
		const DiffModelTextChunk& lastChunk = chunkList.back();
		UnsignedIntRange oneRange(
			lastOneEndIndex + 1,
			oneLineList.size() - 1
			);
		UnsignedIntRange twoRange(
			lastChunk.getTwoRange().getEnd() + 1,
			lastChunk.getTwoRange().getEnd() + 1 + oneRange.length() - 1
			);
		/*TODO: assert that twoRange goes until end of twoLineList */
		DiffModelTextChunk sameChunk(DiffModelTextChunk::SAME, oneRange, twoRange);
		model.addChunk(sameChunk);
	}
}


static void diffText(
	DiffModelText& model,
	const std::string& one,
	const std::string& two,
	const std::string& oneFileContents,
	const std::string& twoFileContents
	) {
	model.setOneContents(oneFileContents);

	model.setTwoContents(twoFileContents);

	std::stringstream cmdStream;
	cmdStream << "diff " << one << " " << two;

	std::string result = exec(cmdStream.str());
	StringList lines = getLineList(result);
	lines.remove_if(isDiffFileLine);

	parseTextDiff(model, lines);
}


void diffTextFromFiles(DiffModelText& model, const std::string& one, const std::string& two) {
	std::string oneFileContents;
	readFile(oneFileContents, one);

	std::string twoFileContents;
	readFile(twoFileContents, two);

	diffText(model, one, two, oneFileContents, twoFileContents);
}


void diffTextFromContents(
	DiffModelText& model,
	const std::string& oneFileContents,
	const std::string& twoFileContents) {

	std::string oneTempFile(tmpnam(NULL));
	std::string twoTempFile(tmpnam(NULL));

	std::cout << "TMP 1 FILE: " << oneTempFile << "\n";
	std::cout << "TMP 2 FILE: " << twoTempFile << "\n";

	writeFile(oneFileContents, oneTempFile);
	writeFile(twoFileContents, twoTempFile);

	diffText(model, oneTempFile, twoTempFile, oneFileContents, twoFileContents);
}