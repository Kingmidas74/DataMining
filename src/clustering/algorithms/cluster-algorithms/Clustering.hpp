#pragma once

#include <functional>

namespace ParallelClustering {

	namespace ClusteringAlgorithms {

		using namespace std;

		class Clustering
		{

		protected:
			double* VectorsForClustering;
			double* Centroids;

			Parameters* AlgorithmParameters;

			function<double(double*, double*, unsigned int, unsigned int, unsigned int)> DistanceCalculate;

			virtual void ExecuteClustering(double* centroids) {};

			virtual void GenerateCentroids()
			{
				for (unsigned int i = 0; i < AlgorithmParameters->CountOfClusters; i++)
				{
					for (unsigned int j = 0; j < AlgorithmParameters->CountOfDimensions; j++)
					{
						Centroids[i*AlgorithmParameters->CountOfDimensions + j] = GetRandomDouble();
					}
				}
				PrintAsMatrix(Centroids, AlgorithmParameters->CountOfDimensions, AlgorithmParameters->CountOfDimensions*AlgorithmParameters->CountOfClusters);
			}


			double MinkowskiSquare(double* first_array, double* second_array, unsigned int length, unsigned int power) {
				double result = 0;
				for (unsigned int i = 0; i < length; i++) {
					result += pow(first_array[i] - second_array[i], power);
				}
				result = pow(result, 1.0 / power);
				return result;
			}


		public:

			double* ResultMatrix;
			double* DistanceMatrix;

			Clustering(double* data, Parameters* algorithm_parameters, function<double(double*, double*, unsigned int, unsigned int, unsigned int)> distance)
			{
				VectorsForClustering = data;
				AlgorithmParameters = algorithm_parameters;
				DistanceCalculate = distance;

				DistanceMatrix = allocateAlign<double>(AlgorithmParameters->CountOfObjects*AlgorithmParameters->CountOfObjects);
			}

			virtual void StartClustering() {}

			void CalculateAllDistance()
			{
				for (unsigned int i = 0; i < AlgorithmParameters->CountOfObjects; i++)
				{
					for (unsigned int j = 0; j < AlgorithmParameters->CountOfObjects; j++)
					{
						DistanceMatrix[i*AlgorithmParameters->CountOfObjects+j]= DistanceCalculate(
							&VectorsForClustering[i*AlgorithmParameters->CountOfObjects],
							&VectorsForClustering[j*AlgorithmParameters->CountOfObjects],
							AlgorithmParameters->CountOfDimensions,
							2, AlgorithmParameters->CountOfDimensions
						);
					}
				}
				PrintAsMatrix(DistanceMatrix, AlgorithmParameters->CountOfObjects, AlgorithmParameters->CountOfObjects*AlgorithmParameters->CountOfObjects);
			}

			void normalizeArray(double* row, int length) const {
				double sum = 0;
				for (auto i = 0; i < length; i++) {
					sum += row[i];
				}
				for (auto i = 0; i < length; i++) {
					row[i] /= sum;
				}
			}

			virtual void StartClustering(double* centroids) {}

			virtual ~Clustering()
			{
				//freeAlign<double>(DistanceMatrix);
			}
		};
	}
}