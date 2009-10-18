#ifndef UTILITIES_H
#define UTILITIES_H
#include "stdafx.h"

enum ParseResult { ParseSucceded, ParseFailed };

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) ;


std::vector<std::string> split(const std::string &s, char delim) ;
class Serializable{
	virtual std::string ToString() = 0;
	virtual ParseResult Parse(Serializable * object) = 0;
};
#endif