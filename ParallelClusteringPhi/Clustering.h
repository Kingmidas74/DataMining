#pragma once

#include <functional>
#include <iostream>

#include "Helper.h"

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


	public:

		OutcomingType* ResultMatrix;

		Clustering(IncomingType* data, Parameters* algorithm_parameters, function<double(IncomingType*, IncomingType*, long)> distance)
		{
			VectorsForClustering = data;
			AlgorithmParameters	= algorithm_parameters;
			DistanceCalculate = distance;
		}

		virtual void StartClustering() {

		}

		virtual ~Clustering()
		{
		}
	};
}