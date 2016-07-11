/*
"Реализация параллельных версиий алгоритов кластеризации данных
с использованием многоядерных ускорителей"

Класс, использующий OpenMP в базовой версии алгоритма FCMeans
Авторы: Сулейманов Д.Э., Цымблер М.Л.
*/

#include "FuzzyCMeansOpenMP.h"

namespace ParallelClustering
{
	namespace FuzzyCMeansCollection
	{
		FuzzyCMeansOpenMP::FuzzyCMeansOpenMP(vector<vector<double>> objects, double epsilon, double fuzzy, function<double(vector<double>, vector<double>)> metrics, int threads)
			:FuzzyCMeans(objects, epsilon, fuzzy, metrics)
		{
			CountOfThreads = threads;
			Epsilon = epsilon;
		}

		FuzzyCMeansOpenMP::~FuzzyCMeansOpenMP()
		{
		}

		void FuzzyCMeansOpenMP::StartClustering(vector<vector<double>> centroids)
		{
			const clock_t start = clock();
			ClusterCount = centroids.size();
			omp_set_num_threads(CountOfThreads);
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
				for (int i = 0; i < sizeOfMatrix;i+=8) {
					for(int ii=0; ii<8; ii++) {
					for (int j = 0; j < ClusterCount; j++) {						
						UMatrix[i][j] = updateUMatrixElement(VectorsForClustering[i], centroids[j]);
					}
					UMatrix[i] = normalizeUMatrixRow(UMatrix[i]);
					}
				}
				currentDecisionValue = calculateDecisionFunction(UMatrix, centroids);
				Centroids = centroids;
			}
			ClearRuntime = double(clock() - start) / CLOCKS_PER_SEC;
			VectorsOfProbabilities = getProbabilities(UMatrix);
		}

		double FuzzyCMeansOpenMP::updateUMatrixElement(vector<double> &vectorForClustering, vector<double> &centroid)
		{
			return pow(1 / DistanceCalculate(vectorForClustering, centroid), 2 / (Fuzzy - 1));
		}


		vector<vector<double>> FuzzyCMeansOpenMP::calculateCentroids(vector<vector<double>> &matrix)
		{
			vector<vector<double>> new_centroids(ClusterCount, vector<double>(CountofDimensions));
						
			for (int numOfCluster = 0; numOfCluster < ClusterCount; numOfCluster++)
			{
				for (int numOfDimension = 0; numOfDimension < CountofDimensions; numOfDimension++)
				{
					double numenator = 0.0;
					double denomenator = 0.0;
					for (int numOfObject = 0; numOfObject < CountOfObjects; numOfObject++)
					{
						double powElement = pow(matrix[numOfObject][numOfCluster], Fuzzy);
						numenator += (powElement * VectorsForClustering[numOfObject][numOfDimension]);						
						denomenator += powElement;
					}
					new_centroids[numOfCluster][numOfDimension] = numenator / denomenator;
				}
			}
			return new_centroids;
		}

		vector<vector<double>> FuzzyCMeansOpenMP::transposeMatrix(vector<vector<double>> inputMatrix)
		{	
			int height = inputMatrix[0].size();
			int width = inputMatrix.size();
			vector<vector<double>> result(height);
			for (int i = 0; i < height; i++)
			{
				vector<double>row(width);
				for (int j = 0; j < width; j++)
				{
					row[j] = inputMatrix[j][i];
				}
				result[i] = row;
			}
			return result;
		}

	}
}
