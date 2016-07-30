#pragma once

/*
"Реализация параллельных версиий алгоритов кластеризации данных
с использованием многоядерных ускорителей"

Вспомогательные функции.
Авторы: Сулейманов Д.Э., Цымблер М.Л.
*/

#include <iostream>

namespace ParallelClustering 
{
	using namespace std;

	struct Parameters
	{
		int CountOfObjects;
		int CountOfDimensions;
		int CountOfClusters;
		int CountOfThreads;
		double Fuzzy;
		double Epsilon;
		string InputFilePath;
		string OutputFilePath;
	};	

	/*Allocators*/
	template <typename Type>
	Type* allocateAlign(long long count, int length = 64) {
		return static_cast<Type*>(_mm_malloc(count*sizeof(Type), length));
	}

	template <typename Type>
	void freeAlign(Type* pointer) {
		if (pointer) {
			_mm_free(pointer);
		}
	}

	/**/

	inline double GetRandomDouble(double min=0, double max=1) {
		return (static_cast <double> (rand()) / static_cast <double> (RAND_MAX));
	}

	inline void GetRandomObjectsArray(int count, int dimension, double* my_array)
	{
		for (int i = 0; i < count; i++) {
			for (int j = 0; j < dimension; j++) {
				my_array[i*dimension + j] = GetRandomDouble();
			}
		}
	}

	inline void printArray(double* array, int width, int height, char* text) {
		cout << endl;
		cout << text << endl;
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				cout << array[i*width + j] << " ";
			}
			cout << endl;
		}
		cout << endl;
	}

}
