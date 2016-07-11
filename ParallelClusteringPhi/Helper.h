#pragma once

/*
"Реализация параллельных версиий алгоритов кластеризации данных
с использованием многоядерных ускорителей"

Вспомогательные функции.
Авторы: Сулейманов Д.Э., Цымблер М.Л.
*/

#include <vector>
#include <iostream>
#include <time.h>

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

	double GetRandomDouble(double min = 0, double max = 1);
	void PrintObjects(vector<vector<double>> objects);
	vector<vector<double>> GetRandomObjects(int count, int dimension);
	void GetRandomObjectsArray(int count, int dimension, double* my_array);
	void printArray(double* array, int width, int height, char* text);

}
