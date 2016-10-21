#pragma once

/**
Allocators.hpp
Purpose: Alignment allocators for XeonPhi.

@author		Suleymanov D.
@version	0.0.0.1 9/17/2016
*/

namespace ParallelClustering
{
	using namespace std;

	/**
	Allocate memory align by custom length block.

	@param count	The count of elements in array.
	@param length	Length block (64 by default).
	@return Pointer to type T.
	*/
	template <typename Type>
	Type* allocateAlign(unsigned long long count, unsigned int length = 64) {
		return static_cast<Type*>(_mm_malloc((count) * sizeof(Type), length));
		//return new Type[count + 1];
		/*void* memptr;
		posix_memalign(&memptr, length, count * sizeof(Type));
		return static_cast<Type*>(memptr);*/
	}



	/**
	Free memory align

	@param pointer The pointer on array of type T.
	*/
	template <typename Type>
	void freeAlign(Type* pointer) {
		if (pointer) {
			_mm_free(pointer);
			//delete[] pointer;
		}
	}

	/**
	Re-Allocate memory align by custom length block.

	@param pointer	Pointer to previous memory
	@param count	The count of elements in array.
	@param length	Length block (64 by default).	
	@return Pointer to type T.
	*/
	template <typename Type>
	Type* reAllocateAlign(Type* pointer, unsigned long long count, unsigned int length = 64) {
		freeAlign<Type>(pointer);
		return allocateAlign<Type>(count, length);
	}
}
