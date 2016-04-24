#pragma once

#include <vector>
#include <functional>
#include "CustomObject.h"
#include "MetricsDistance.h"



namespace ParallelClustering {
	class FuzzyCMeans
	{
	public:

		int						ClusterCount;
		double					Epsilon;
		double					Fuzzy;
		vector<CustomObject*>	ObjectsForClustering;

		function<double(vector<double>, vector<double>)> DistanceCalculate;

		FuzzyCMeans(vector<CustomObject*> objects, double epsilon, double fuzzy, function<double(vector<double>, vector<double>)> metrics);
		~FuzzyCMeans();
		void StartClustering(vector<CustomObject*> centroids);

	private:

		int countOfObjects;
		int countofDimensions;

		vector<vector<double>> generateUMatrix();
		vector<double> normalizeUMatrixRow(vector<double> row);
		vector<CustomObject*> calculateCentroids(vector<vector<double>> matrix, vector<CustomObject*> centroids);
		double calculateDecisionFunction(vector<CustomObject*> centers, vector<vector<double>> matrix);
		void joinMatrix(vector<vector<double>> matrix);
	};
}

