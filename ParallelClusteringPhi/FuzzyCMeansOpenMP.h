#pragma once

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
		};
	}
}