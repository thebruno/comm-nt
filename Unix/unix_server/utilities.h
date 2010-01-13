#ifndef UTILITIES_H
#define UTILITIES_H
#include "stdafx.h"

enum Result { RESULT_NOTSET, OK, FAILED, DISCONNECTED, EMPTY, NOTEMPTY , TIMEOUT};

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) ;
std::vector<std::string> split(const std::string &s, char delim) ;

#endif
