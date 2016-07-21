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

		virtual void StartClustering(IncomingType* centroids) {}

		void CalculateAllDistance() const
		{
			for(int i=0;i<AlgorithmParameters->CountOfObjects;i++)
			{
				for (int j = 0; j < AlgorithmParameters->CountOfObjects; j++)
				{
					DistanceMatrix[i*AlgorithmParameters->CountOfObjects + j] = DistanceCalculate(&VectorsForClustering[i*AlgorithmParameters->CountOfObjects], &VectorsForClustering[j*AlgorithmParameters->CountOfObjects], AlgorithmParameters->CountOfDimensions);
				}
			}
		}

		virtual ~Clustering()
		{
			freeAlign(DistanceMatrix);
		}
	};
}