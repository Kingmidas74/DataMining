/*
"Реализация параллельных версиий алгоритов кластеризации данных
с использованием многоядерных ускорителей"

Класс реализующий алгоритм кластеризации Fuzzy C-means.
Авторы: Сулейманов Д.Э., Цымблер М.Л.
*/

#include "FuzzyCMeans.h"


namespace ParallelClustering 
{

	namespace FuzzyCMeansCollection
	{

		FuzzyCMeans::FuzzyCMeans(vector<vector<double>> objects, double epsilon, double fuzzy, function<double(vector<double>, vector<double>)> metrics)
		{
			VectorsForClustering = objects;
			CountOfObjects = objects.size();
			CountofDimensions = objects[0].size();
			Epsilon = epsilon;
			Fuzzy = fuzzy;
			DistanceCalculate = metrics;
			//ClusterCount = cluster_count;
		}


		FuzzyCMeans::~FuzzyCMeans()
		{
		}

		void FuzzyCMeans::StartClustering(vector<vector<double>> centroids)
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
				for (int i = 0; i < sizeOfMatrix;i+=8) {
					for (int ii=i; ii<i+8;ii++) {
					for (int j = 0; j < ClusterCount; j++) {
						double distance = DistanceCalculate(VectorsForClustering[ii], centroids[j]);
						UMatrix[ii][j] = pow(1 / distance, 2 / (Fuzzy - 1));
					}
					UMatrix[ii] = normalizeUMatrixRow(UMatrix[ii]);
					}
				}
				currentDecisionValue = calculateDecisionFunction(UMatrix, centroids);
				Centroids = centroids;
			}
			ClearRuntime = (double)(clock() - start) / (CLOCKS_PER_SEC);
			VectorsOfProbabilities = getProbabilities(UMatrix);

		}

		vector<vector<double>> FuzzyCMeans::getProbabilities(vector<vector<double>> matrix)
		{
			vector<vector<double>> result(CountOfObjects,vector<double>(ClusterCount));
			for (int i = 0; i < CountOfObjects;i++) {
				for (int j = 0; j < ClusterCount; j++) {
					result[i][j] = matrix[i][j];
				}
			}
			return result;
		}

		vector<vector<double>> FuzzyCMeans::generateUMatrix() {
			vector<vector<double>> result(CountOfObjects, vector<double>(ClusterCount));
			for (int i = 0; i < CountOfObjects; i++) {
				for (int j = 0; j < ClusterCount; j++) {
					result[i][j] = GetRandomDouble();
				}
				result[i] = normalizeUMatrixRow(result[i]);
			}
			return result;
		}

		vector<double> FuzzyCMeans::normalizeUMatrixRow(vector<double> row) {
			vector<double> new_row(ClusterCount);
			double sum = 0;
			for (int i = 0; i < ClusterCount; i++) {
				sum += row[i];
			}
			for (int i = 0; i < ClusterCount; i++) {
				new_row[i] = row[i] / sum;
			}
			return new_row;
		}

		vector<vector<double>> FuzzyCMeans::calculateCentroids(vector<vector<double>> matrix)
		{
			vector<vector<double>> new_centroids(ClusterCount, vector<double>(CountofDimensions));
			vector<vector<double>> powMatrix(CountOfObjects,vector<double>(ClusterCount));
			for (int i = 0; i < CountOfObjects; i+=8) {
				for(int ii=i; ii<i+8;ii++) {
				for (int j = 0; j < ClusterCount; j++) {
					powMatrix[ii][j] = pow(matrix[ii][j], Fuzzy);
				}
				}
			}
			for (int i = 0; i < ClusterCount; i++) {
				for (int d = 0; d < CountofDimensions; d++) {
					double numenator = 0.0;
					double denomenator = 0.0;
					for (int j = 0; j < CountOfObjects; j+=8) {
						for(int jj=j;jj<j+8;jj++) {
						numenator += powMatrix[jj][i] * VectorsForClustering[jj][d];
						denomenator += powMatrix[jj][i];
					}
					}
					new_centroids[i][d] = numenator / denomenator;
				}
			}
			return new_centroids;
		}

		double FuzzyCMeans::calculateDecisionFunction(vector<vector<double>> matrix, vector<vector<double>> centroids) {
			double sum = 0;
			for (int i = 0; i < CountOfObjects;i+=8) {
				for(int ii=i; ii<i+8; ii++) {
				for (int j = 0; j < ClusterCount; j++) {
					sum += DistanceCalculate(centroids[j], matrix[ii]);
				}
				}
			}
			return sum;
		}
	}
}
