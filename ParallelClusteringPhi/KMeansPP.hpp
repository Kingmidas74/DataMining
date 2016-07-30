#pragma once
#include "KMeans.hpp"
#include "Clustering.hpp"

namespace ParallelClustering {

	namespace KMeansCollection {

		using namespace std;

		template<class IncomingType, class OutcommingType>
		class KMeansPP : public KMeans<IncomingType, OutcommingType>
		{

		protected:

			void ExecuteClustering(IncomingType* centroids) override
			{
				
			}


		public:

			KMeansPP(IncomingType* data, Parameters* algorithm_parameters, function<double(IncomingType*, IncomingType*, long)> distance)
				: KMeans<IncomingType, OutcommingType>(data, algorithm_parameters, distance)
			{
				
			}

			void StartClustering() override
			{}

			void StartClustering(IncomingType* centroids) override
			{}

			virtual ~KMeansPP()
			{
			}
		};

	}
}