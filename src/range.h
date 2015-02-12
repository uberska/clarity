#pragma once


#include <climits>
#include <iostream>

/* TODO: now that this isn't a template, split into cpp and h */

class UnsignedIntRange {
public:
	UnsignedIntRange(unsigned int start = 0, unsigned int end = UINT_MAX) :
		mStart(start),
		mEnd(end)
	{
		if (mEnd == UINT_MAX) {
			mEnd = mStart;
		}
	}

	unsigned int getStart() const { return mStart; }

	unsigned int getEnd() const { return mEnd; }

	unsigned int length() const { return mEnd - mStart + 1; }

	inline friend std::ostream& operator<<(std::ostream& out, const UnsignedIntRange& range) {
		out << range.mStart << " -> " << range.mEnd;
		return out;
	}

private:
	unsigned int mStart;
	unsigned int mEnd;
};