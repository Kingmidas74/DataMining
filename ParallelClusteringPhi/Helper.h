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
		double Fuzzy;
		double Epsilon;
		string InputFilePath;
		string OutputFilePath;
	};	

	double GetRandomDouble(double min = 0, double max = 1);
	void PrintObjects(vector<vector<double>> objects);
	Parameters* InitParameters(int argc, char *argv[]);
	vector<vector<double>> GetRandomObjects(int count, int dimension);
	vector<vector<double>> GetTest3DBatterfly();

}
