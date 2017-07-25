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

			IncomingType* VectorsForClustering;
			Parameters AlgorithmParameters;
			DistanceMetric* Metric;
			NormalizationBase<IncomingType> * Normalization;
			FileIO fileIO;


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
			double* DistanceMatrix;


			Clustering(Parameters* algorithm_parameters, DistanceMetric* metric, NormalizationBase<IncomingType>* normalization, FileIO _fileIO): AlgorithmParameters(algorithm_parameters[0]), Metric(metric), Normalization(normalization), fileIO(_fileIO), ResultMatrix(nullptr)
			{
				VectorsForClustering = allocateAlign<IncomingType>(AlgorithmParameters.CountOfObjects*AlgorithmParameters.CountOfDimensions);
				DistanceMatrix = allocateAlign<double>(AlgorithmParameters.CountOfObjects*AlgorithmParameters.CountOfObjects);
				Centroids = allocateAlign<IncomingType>(AlgorithmParameters.CountOfClusters * AlgorithmParameters.CountOfDimensions);				
			}

			virtual void StartClustering() {};
			virtual bool Verification() {return false;};

			void CalculateAllDistance()
			{
				for (size_t i = 0; i < AlgorithmParameters.CountOfObjects; i++)
				{
					for (size_t j = 0; j < AlgorithmParameters.CountOfObjects; j++)
					{
						DistanceMatrix[i*AlgorithmParameters.CountOfObjects + j] = Metric->CalculateDistance(
							&VectorsForClustering[i*AlgorithmParameters.CountOfDimensions],
							&VectorsForClustering[j*AlgorithmParameters.CountOfDimensions],
							AlgorithmParameters.CountOfDimensions
						);
					}
				}
			}

			virtual bool TrySaveData() {return false;}

			virtual bool TryGetData() { return false; }

			virtual ~Clustering()
			{
				freeAlign(ResultMatrix);
				freeAlign(DistanceMatrix);
				freeAlign(VectorsForClustering);
				freeAlign(Centroids);
			}
		};
	}
}
