/*
"Реализация параллельных версиий алгоритов кластеризации данных
с использованием многоядерных ускорителей"

Класс реализующий алгоритм кластеризации Fuzzy C-means.
Авторы: Сулейманов Д.Э., Цымблер М.Л.
*/

#include "FuzzyCMeans.h"


namespace ParallelClustering {

	FuzzyCMeans::FuzzyCMeans(vector<vector<double>> objects, double epsilon, double fuzzy, function<double(vector<double>, vector<double>)> metrics)
	{
		VectorsForClustering = objects;
		countOfObjects = objects.size();
		countofDimensions = objects[0].size();
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
			for (int i = 0; i < sizeOfMatrix;i++) {
				for (int j = 0; j < ClusterCount; j++) {
					double distance = DistanceCalculate(VectorsForClustering[i], centroids[j]);
					UMatrix[i][j] = pow(1 / distance, 2 / (Fuzzy - 1));
				}
				UMatrix[i] = normalizeUMatrixRow(UMatrix[i]);
			}
			currentDecisionValue = calculateDecisionFunction(UMatrix, centroids);			
		}
		
		VectorsOfProbabilities=getProbabilities(UMatrix);

	}

	vector<vector<double>> FuzzyCMeans::getProbabilities(vector<vector<double>> matrix)
	{
		vector<vector<double>> result(countOfObjects);
		for (int i = 0; i < countOfObjects;i++) {
			result[i] = vector<double>(ClusterCount);
			for (int j = 0; j < ClusterCount; j++) {
				result[i][j] = matrix[i][j];
			}
		}
		return result;
	}

	vector<vector<double>> FuzzyCMeans::generateUMatrix() {
		vector<vector<double>> result(countOfObjects);
		for (int i = 0; i < countOfObjects; i++) {
			result[i] = vector<double>(ClusterCount);
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
		vector<vector<double>> new_centroids(ClusterCount);
		vector<vector<double>> powMatrix(countOfObjects);
		for (int i = 0; i < countOfObjects; i++) {
			vector<double> rowOfpow(ClusterCount);
			for (int j = 0; j < ClusterCount; j++) {
				rowOfpow[j] = pow(matrix[i][j], Fuzzy);
			}
			powMatrix[i] = rowOfpow;
		}
		for (int i = 0; i < ClusterCount; i++) {
			vector<double> centroid_data(countofDimensions);
			for (int d = 0; d < countofDimensions; d++) {
				double numenator = 0.0;
				double denomenator = 0.0;
				for (int j = 0; j < countOfObjects; j++) {
					numenator += powMatrix[j][i] * VectorsForClustering[j][d];
					denomenator += powMatrix[j][i];
				}
				centroid_data[d] = numenator / denomenator;
			}
			new_centroids[i] = centroid_data;
		}
		return new_centroids;
	}
	
	double FuzzyCMeans::calculateDecisionFunction(vector<vector<double>> matrix, vector<vector<double>> centroids) {
		double sum = 0;		
		for (int i = 0; i < countOfObjects;i++) {			
			for (int j = 0; j < ClusterCount; j++) {								
				sum += DistanceCalculate(centroids[j], matrix[i]);
				
			}
		}
		return sum;
	}
}