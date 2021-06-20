#pragma once

/**
Allocators.hpp
Purpose: Alignment allocators for XeonPhi.

@author		Suleymanov D.
@version	0.0.0.1 9/17/2016
*/

namespace DataMining
{
	using namespace std;

	template<class T>
	typename std::enable_if<std::is_fundamental<T>::value, std::string>::type stringify(const T& t)
	{
		return std::to_string(t);
	}

	template<class T>
	typename std::enable_if<!std::is_fundamental<T>::value, std::string>::type  stringify(const T& t)
	{
		return std::string(t);
	}
}
