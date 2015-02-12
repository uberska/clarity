#include "diff_delta.h"
#include "diff_file.h"


std::ostream& operator<< (std::ostream &stream, const git_diff_delta& delta)
{
	return stream << "{\"type\":\"git_diff_delta\", \"old_file\":" << delta.old_file <<
		", \"new_file\":" << delta.new_file << "}";
}
