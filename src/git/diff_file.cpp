#include "diff_file.h"


std::ostream& operator<< (std::ostream &stream, const git_diff_file& file)
{
	char oidStr[41];
	oidStr[40] = '\0';
	git_oid_fmt(oidStr, &file.oid);

	stream << "{\"type\":\"git_diff_file\", \"oid\":\"" << oidStr <<
		"\", \"path\":\"" << file.path <<
		"\", \"size\":\"" << file.size <<
		"\", \"flags\":\"" << file.flags <<
		"\", \"mode\":\"" << file.mode <<
		"\"}";

	return stream;
}
