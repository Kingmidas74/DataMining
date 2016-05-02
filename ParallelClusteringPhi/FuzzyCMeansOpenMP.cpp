#include "FuzzyCMeansOpenMP.h"

namespace ParallelClustering
{
	namespace FuzzyCMeansCollection
	{
		FuzzyCMeansOpenMP::FuzzyCMeansOpenMP(vector<vector<double>> objects, double epsilon, double fuzzy, function<double(vector<double>, vector<double>)> metrics, int threads)
			:FuzzyCMeans(objects, epsilon, fuzzy, metrics)
		{
			CountOfThreads = threads;
			Epsilon = epsilon * 2;
		}

		FuzzyCMeansOpenMP::~FuzzyCMeansOpenMP()
		{
		}

		void FuzzyCMeansOpenMP::StartClustering(vector<vector<double>> centroids)
		{
			cout << "I am override: " << CountOfThreads <<" and "<<Epsilon << ". And base is: "<<FuzzyCMeans::Epsilon;
		}

	}
}