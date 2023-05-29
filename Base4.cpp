#pragma once
#include <string>
#include "Base4.h"

//uint_fast64_t ConvertFromBase10(const int& _num, int _toBase)
//{
//	std::string numAsString = std::to_string(_num);
//	std::string res;
//
//	int quotient{ _num };
//	int remainder{ 0 };
//
//
//	do
//	{
//		remainder = quotient % _toBase;
//		res = std::to_string(remainder) + res;
//		quotient /= _toBase;
//	} 
//	while (quotient > 0);
//
//	return std::stoull(res);
//}