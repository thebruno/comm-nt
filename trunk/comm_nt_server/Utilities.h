#ifndef UTILITIES_H
#define UTILITIES_H
#include "stdafx.h"

enum Result { OK, FAILED, EMPTY, NOTEMPTY, DISCONNECTED};

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) ;
std::vector<std::string> split(const std::string &s, char delim) ;

class Serializable{
	virtual std::string ToString() = 0;
	virtual Result Parse(std::string &s) = 0;
};

#endif