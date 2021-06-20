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

	template <typename Type>
	inline void copyArray(const Type *  const from, Type * to,  const size_t length)
	{
		copy(from, from+length, to);
	}
}
