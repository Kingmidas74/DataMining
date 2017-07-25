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
	inline Type RoundTo(Type source, size_t count)
	{
		size_t zeros = 1;
		for(size_t i=0;i<count; i++)
		{
			zeros*=10;
		}
		source *= zeros;
		return floor(source*zeros)/zeros;				
	}

	template<typename Type>
	inline void powMatrix(Type* source, Type* result, double_t exp, size_t countOfElements)
	{
		#pragma omp parallel for schedule(static,8)
		for (size_t i = 0; i < countOfElements; i++)
		{
				result[i] = pow(source[i], exp);
		}
	}

	template <typename Type>
	inline Type GetRandom(int from=0, int to=10)
	{
		Type random = static_cast<Type>(rand()) / RAND_MAX;
		return from + random * (to - from);
	}
}
