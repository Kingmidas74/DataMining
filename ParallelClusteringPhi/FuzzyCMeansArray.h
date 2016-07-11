#pragma once

/*
"Реализация параллельных версиий алгоритов кластеризации данных
с использованием многоядерных ускорителей"

Класс реализующий алгоритм кластеризации Fuzzy C-means.
Авторы: Сулейманов Д.Э., Цымблер М.Л.
*/

#include <functional>
#include "MetricsDistance.h"
#include "Helper.h"



namespace ParallelClustering 
{

	namespace FuzzyCMeansCollection 
	{

		using namespace std;
		
		class FuzzyCMeansArray
		{
		public:

			Parameters		AlgorithmParameters;
			double			ClearRuntime;

			double*			VectorsForClustering;
			double*			UMatrix;

			function<double(double*,int, double*,int)> DistanceCalculate;

			FuzzyCMeansArray(double* objects, Parameters algorithm_parameters, function<double(double*,long long, double*, long long)> metrics);
			~FuzzyCMeansArray();
			void StartClustering(double* centroids);

		protected:
			void	generateUMatrix();
			void	normalizeUMatrixRow(double* row, int length);
			void	calculateCentroids(double* centroids);
			double	calculateDecisionFunction(double* centers);

		private:
			double*		_powUMatrix;
		};

	}

}

