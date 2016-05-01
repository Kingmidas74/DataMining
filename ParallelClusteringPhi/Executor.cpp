/*
"Реализация параллельных версиий алгоритов кластеризации данных
с использованием многоядерных ускорителей"

Класс для инкапсуляции работы с кластеризацией данных.
Авторы: Сулейманов Д.Э., Цымблер М.Л.
*/

#include "Executor.h"

namespace ParallelClustering
{
	Executor::Executor(Parameters algorithmParameters)
	{
		_algorithmParameters = algorithmParameters;
	}

	Executor::~Executor() 
	{

	}

	vector<vector<double>> Executor::CalculateProbabilities()
	{
		vector<vector<double>> data;
		vector<vector<double>> centroids;
		data = GetRandomObjects(_algorithmParameters.CountOfObjects, _algorithmParameters.CountOfDimensions);
		centroids = GetRandomObjects(_algorithmParameters.CountOfClusters, _algorithmParameters.CountOfDimensions);
		PrintObjects(centroids);
		PrintObjects(data);
		FuzzyCMeans* cmeans = new FuzzyCMeans(data, _algorithmParameters.Epsilon, _algorithmParameters.Fuzzy, GetMetrics(MetricsDistanceTypes::Evklid));
		cmeans->StartClustering(centroids);
		PrintObjects(cmeans->Centroids);
		PrintObjects(cmeans->VectorsOfProbabilities);
		return vector<vector<double>>();
	}

}