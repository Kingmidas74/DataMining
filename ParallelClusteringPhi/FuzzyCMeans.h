#pragma once

/*
"Реализация параллельных версиий алгоритов кластеризации данных
с использованием многоядерных ускорителей"

Класс реализующий алгоритм кластеризации Fuzzy C-means.
Авторы: Сулейманов Д.Э., Цымблер М.Л.
*/

#include <functional>
#include <iostream>

#include "Helper.h"
#include "Clustering.h"

namespace ParallelClustering {

	namespace CMeansCollection {

		using namespace std;

		template<class IncomingType, class OutcommingType>
		class FuzzyCMeans : public Clustering<IncomingType, OutcommingType>
		{

		protected:

		public:


			FuzzyCMeans(IncomingType* data, Parameters* algorithm_parameters, function<double(IncomingType*, IncomingType*, long)> distance)
				: Clustering(data, algorithm_parameters, distance)
			{
				ResultMatrix = (OutcommingType*)_mm_malloc(AlgorithmParameters->CountOfObjects * AlgorithmParameters->CountOfClusters * sizeof(OutcommingType), 64);
			}

			virtual void StartClustering()
			{
				cout << "START!" << endl;
				for (int i = 0; i < AlgorithmParameters->CountOfObjects; ++i) {
					ResultMatrix[i] = 0.1*i;
				}
			}

			virtual ~FuzzyCMeans()
			{
				_mm_free(ResultMatrix);
			}
		};
	}
}