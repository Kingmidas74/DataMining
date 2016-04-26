#pragma once

/*
"���������� ������������ ������� ��������� ������������� ������
� �������������� ������������ �����������"

����� ����������� �������� ������������� Fuzzy C-means.
������: ���������� �.�., ������� �.�.
*/

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
		vector<vector<double>>	VectorsForClustering;
		vector<vector<double>>	VectorsOfProbabilities;

		function<double(vector<double>, vector<double>)> DistanceCalculate;

		FuzzyCMeans(vector<vector<double>> objects, double epsilon, double fuzzy, function<double(vector<double>, vector<double>)> metrics);
		~FuzzyCMeans();
		void StartClustering(vector<vector<double>> centroids);

	private:

		int countOfObjects;
		int countofDimensions;

		vector<vector<double>> generateUMatrix();
		vector<double> normalizeUMatrixRow(vector<double> row);
		vector<vector<double>> calculateCentroids(vector<vector<double>> matrix);
		double calculateDecisionFunction(vector<vector<double>> matrix, vector<vector<double>> centers);
		vector<vector<double>> getProbabilities(vector<vector<double>> matrix);
	};
}

