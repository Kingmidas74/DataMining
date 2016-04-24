#include "FuzzyCMeans.h"
#include "Helper.h";
#include <iostream>

namespace ParallelClustering {

	using namespace std;

	FuzzyCMeans::FuzzyCMeans(vector<CustomObject*> objects, double epsilon, double fuzzy, function<double(vector<double>, vector<double>)> metrics)
	{
		ObjectsForClustering = objects;
		countOfObjects = objects.size();
		countofDimensions = objects[0]->Data.size();
		Epsilon = epsilon;
		Fuzzy = fuzzy;
		DistanceCalculate = metrics;
	}


	FuzzyCMeans::~FuzzyCMeans()
	{
	}

	void FuzzyCMeans::StartClustering(vector<CustomObject*> centroids)
	{
		ClusterCount = centroids.size();
		vector<vector<double>> UMatrix = generateUMatrix();
		double previousDecisionValue = 0;
		double currentDecisionValue = 1;
		int sizeOfMatrix = ObjectsForClustering.size();
		int step = 0;
		while (abs(previousDecisionValue - currentDecisionValue) > Epsilon)
		{
			step++;
			previousDecisionValue = currentDecisionValue;
			centroids = calculateCentroids(UMatrix, centroids);
			for (int i = 0; i < sizeOfMatrix;i++) {
				for (int j = 0; j < ClusterCount; j++) {
					double distance = DistanceCalculate(ObjectsForClustering[i]->Data, centroids[j]->Data);
					UMatrix[i][j] = pow(1 / distance, 2 / (Fuzzy - 1));
				}
				UMatrix[i] = normalizeUMatrixRow(UMatrix[i]);
			}
			currentDecisionValue = calculateDecisionFunction(centroids, UMatrix);
		}
		joinMatrix(UMatrix);

	}

	void FuzzyCMeans::joinMatrix(vector<vector<double>> matrix)
	{
		for (int i = 0; i < countOfObjects;i++) {
			for (int j = 0; j < ClusterCount; j++) {
				ObjectsForClustering[i]->ProbabilityClusters[j] = matrix[i][j];
			}
		}
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

	vector<CustomObject*> FuzzyCMeans::calculateCentroids(vector<vector<double>> matrix, vector<CustomObject*> centroids)
	{
		vector<CustomObject*> new_centroids(ClusterCount);
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
					numenator += powMatrix[j][i] * ObjectsForClustering[j]->Data[d];
					denomenator += powMatrix[j][i];
				}
				centroid_data[d] = numenator / denomenator;
			}
			new_centroids[i] = new CustomObject(centroid_data);
		}
		return new_centroids;
	}
	
	double FuzzyCMeans::calculateDecisionFunction(vector<CustomObject*> centroids, vector<vector<double>> matrix) {
		double sum = 0;
		int sizeOfMatrix = matrix.size();
		for (int i = 0; i < sizeOfMatrix;i++) {
			for (int j = 0; j < ClusterCount; j++) {
				sum += DistanceCalculate(centroids[j]->Data, matrix[i]);
			}
		}
		return sum;
	}
}