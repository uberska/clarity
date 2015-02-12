#pragma once

#include <deque>
#include "range.h"

class DiffModelTextChunk;
typedef std::deque<DiffModelTextChunk> DiffModelTextChunkList;


class DiffModelTextChunk
{
public:
	enum Status
	{
		SAME,
		ADDED,
		REMOVED,
		MODIFIED
	};

	static std::string getStatusString(const Status& status) {
		if (status == ADDED) {
			return "Added";
		} else if (status == REMOVED) {
			return "Removed";
		} else if (status == MODIFIED) {
			return "Modified";
		} else if (status == SAME) {
			return "Same";
		} else {
			return "Unknown";
		}
	}

	DiffModelTextChunk(Status status, UnsignedIntRange oneRange, UnsignedIntRange twoRange) :
		mStatus(status),
		mOneRange(oneRange),
		mTwoRange(twoRange) {

	}

	Status getStatus() const { return mStatus; }

	inline friend std::ostream& operator<<(std::ostream& out, const DiffModelTextChunk& chunk) {
		out << "[DiffModelTextChunk - " << DiffModelTextChunk::getStatusString(chunk.mStatus) << " - " << chunk.mOneRange << " - " << chunk.mTwoRange << "]";
		return out;
	}

	UnsignedIntRange getOneRange() const { return mOneRange; }

	UnsignedIntRange getTwoRange() const { return mTwoRange; }

private:
	Status mStatus;
	UnsignedIntRange mOneRange;
	UnsignedIntRange mTwoRange;
};


class DiffModelText
{
public:
	const DiffModelTextChunkList& getChunks() const { return mChunks; }

	void addChunk(const DiffModelTextChunk& chunk) {
		mChunks.push_back(chunk);
	}

	void clearChunks() {
		mChunks.clear();
	}

	void setOneContents(const std::string& oneContents) {
		mOneContents = oneContents;
	}

	const std::string& getOneContents() const {
		return mOneContents;
	}

	const std::string& getTwoContents() const {
		return mTwoContents;
	}

	void setTwoContents(const std::string& twoContents) {
		mTwoContents = twoContents;
	}

	inline friend std::ostream& operator<<(std::ostream& out, const DiffModelText& model) {
		out << "[";

		DiffModelTextChunkList::const_iterator iterator;
		for ( iterator = model.mChunks.begin(); iterator != model.mChunks.end(); ++iterator ) {
			out << *iterator;

			DiffModelTextChunkList::const_iterator nextIterator = iterator;
			std::advance(nextIterator, 1);
			if (nextIterator != model.mChunks.end()) {
				out << ", ";
			}
		}
		out << "]";

		return out;
	}

private:
	DiffModelTextChunkList mChunks;

	std::string mOneContents;

	std::string mTwoContents;
};
