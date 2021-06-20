#pragma once

namespace DataMining {
	namespace ClusteringAlgorithms {


		using namespace std;
		using namespace DataMining::Metrics;
		using namespace DataMining::Normalization;
		using namespace DataMining::IO;

		template<typename IncomingType, typename OutcomingType>
		class Clustering
		{

		protected:


			Parameters AlgorithmParameters;
			DistanceMetric* Metric;
			NormalizationBase<IncomingType> * Normalization;


			virtual void GenerateCentroids() {}

			inline virtual void NormalizeData()
			{
				for(size_t i=0; i<AlgorithmParameters.CountOfObjects;i++)
				{
					Normalization->Normalize(&VectorsForClustering[i*AlgorithmParameters.CountOfDimensions], AlgorithmParameters.CountOfDimensions);
				}
			}

		public:

			IncomingType* Centroids;
			OutcomingType* ResultMatrix;
			IncomingType* VectorsForClustering;
			double* DistanceMatrix;


			Clustering(Parameters* algorithm_parameters, DistanceMetric* metric, NormalizationBase<IncomingType>* normalization): AlgorithmParameters(algorithm_parameters[0]), Metric(metric), Normalization(normalization), ResultMatrix(nullptr)
			{
				DistanceMatrix = allocateAlign<double>(AlgorithmParameters.CountOfObjects*AlgorithmParameters.CountOfObjects);
				Centroids = allocateAlign<IncomingType>(AlgorithmParameters.CountOfClusters * AlgorithmParameters.CountOfDimensions);
			}

			virtual void StartClustering(IncomingType * vectors, double * distanceMatrix) {};
			virtual void StartClustering(IncomingType * vectors) {};

			virtual bool Verification() {return false;};

			virtual ~Clustering()
			{
				freeAlign(DistanceMatrix);
				freeAlign(Centroids);
			}
		};
	}
}
