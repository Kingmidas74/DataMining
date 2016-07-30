#pragma once

#include <functional>

#include "Helper.hpp"

namespace ParallelClustering {
	
	using namespace std;

	template<class IncomingType, class OutcomingType> 
	class Clustering
	{

	protected:
		IncomingType* VectorsForClustering;
		IncomingType* Centroids;

		Parameters* AlgorithmParameters;
		
		function<double(IncomingType*, IncomingType*, long)> DistanceCalculate;

		virtual void ExecuteClustering(IncomingType* centroids) {};

		virtual void GenerateCentroids()
		{
			for (auto i = 0; i < AlgorithmParameters->CountOfClusters; i++)
			{
				for (auto j = 0; j < AlgorithmParameters->CountOfDimensions; j++)
				{
					Centroids[i*AlgorithmParameters->CountOfDimensions + j] = GetRandomDouble();
				}
			}
		}


	public:

		OutcomingType* ResultMatrix;
		double* DistanceMatrix;

		Clustering(IncomingType* data, Parameters* algorithm_parameters, function<double(IncomingType*, IncomingType*, long)> distance)
		{
			VectorsForClustering = data;
			AlgorithmParameters	= algorithm_parameters;
			DistanceCalculate = distance;

			DistanceMatrix = allocateAlign<double>(AlgorithmParameters->CountOfObjects*AlgorithmParameters->CountOfObjects);
		}

		virtual void StartClustering() {}

		void CalculateAllDistance() const
		{
			for(auto i=0; i<AlgorithmParameters->CountOfObjects; i++)
			{
				for(auto j=0; j<AlgorithmParameters->CountOfObjects; j++)
				{
					DistanceMatrix[i*AlgorithmParameters->CountOfObjects + j] = DistanceCalculate(&VectorsForClustering[i*AlgorithmParameters->CountOfObjects], &VectorsForClustering[j*AlgorithmParameters->CountOfObjects], AlgorithmParameters->CountOfDimensions);
				}
			}
			printArray(DistanceMatrix, AlgorithmParameters->CountOfObjects, AlgorithmParameters->CountOfObjects, "DistanceMatrix");
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

		virtual void StartClustering(IncomingType* centroids) {}

		virtual ~Clustering()
		{
			freeAlign(DistanceMatrix);
		}
	};
}