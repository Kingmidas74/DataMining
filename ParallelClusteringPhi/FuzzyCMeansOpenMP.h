#pragma once

/*
"Реализация параллельных версиий алгоритов кластеризации данных
с использованием многоядерных ускорителей"

Класс, использующий OpenMP в базовой версии алгоритма FCMeans
Авторы: Сулейманов Д.Э., Цымблер М.Л.
*/

#include <vector>
#include <functional>
#include <omp.h>
#include "FuzzyCMeans.h"

namespace ParallelClustering
{
	namespace FuzzyCMeansCollection
	{
		using namespace std;

		class FuzzyCMeansOpenMP:public FuzzyCMeans
		{
		public:
			FuzzyCMeansOpenMP(vector<vector<double>> objects, double epsilon, double fuzzy, function<double(vector<double>, vector<double>)> metrics, int threads);
			~FuzzyCMeansOpenMP();
			void StartClustering(vector<vector<double>> centroids);
		private:
			int CountOfThreads;
			vector<vector<double>> calculateCentroids(vector<vector<double>> &matrix);
			double updateUMatrixElement(vector<double> &vectorForClustering, vector<double> &centroid);
			vector<vector<double>> transposeMatrix(vector<vector<double>> powMatrix);
		};
	}
}