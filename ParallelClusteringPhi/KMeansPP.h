#pragma once
#include "KMeans.h"
#include "Clustering.h"

namespace ParallelClustering {

	namespace KMeansCollection {

		using namespace std;

		template<class IncomingType, class OutcommingType>
		class KMeansPP : public KMeans<IncomingType, OutcommingType>
		{

		protected:

			virtual void ExecuteClustering(IncomingType* centroids) {
				
			}


		public:

			KMeansPP(IncomingType* data, Parameters* algorithm_parameters, function<double(IncomingType*, IncomingType*, long)> distance)
				: KMeans(data, algorithm_parameters, distance)
			{
				
			}

			virtual  void StartClustering() {}

			virtual  void StartClustering(IncomingType* centroids) {}

			virtual ~KMeansPP()
			{
			}
		};

	}
}