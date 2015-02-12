#pragma once


#include <git2.h>
#include <iostream>


std::ostream& operator<< (std::ostream &stream, const git_diff_file& file);