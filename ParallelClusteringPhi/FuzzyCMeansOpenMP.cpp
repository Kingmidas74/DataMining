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
			int start = clock();
			ClusterCount = centroids.size();
			vector<vector<double>> UMatrix = generateUMatrix();
			double previousDecisionValue = 0;
			double currentDecisionValue = 1;
			int sizeOfMatrix = VectorsForClustering.size();
			int step = 0;
			while (abs(previousDecisionValue - currentDecisionValue) > Epsilon)
			{
				step++;
				previousDecisionValue = currentDecisionValue;
				centroids = calculateCentroids(UMatrix);
				#pragma omp parallel for 
				for (int i = 0; i < sizeOfMatrix;i++) {
					for (int j = 0; j < ClusterCount; j++) {
						double distance = DistanceCalculate(VectorsForClustering[i], centroids[j]);
						UMatrix[i][j] = pow(1 / distance, 2 / (Fuzzy - 1));
					}
					UMatrix[i] = normalizeUMatrixRow(UMatrix[i]);
				}
				currentDecisionValue = calculateDecisionFunction(UMatrix, centroids);
				Centroids = centroids;
			}
			ClearRuntime = (clock() - start) / double(CLOCKS_PER_SEC) * 1000;
			VectorsOfProbabilities = getProbabilities(UMatrix);
		}

	}
}