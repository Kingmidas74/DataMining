#pragma once
#include <functional>

namespace ParallelClustering {
	namespace ClusteringAlgorithms {


		using namespace std;
		using namespace ParallelClustering::Metrics;
		using namespace ParallelClustering::IO;
		
		class Clustering
		{

		protected:

			double* VectorsForClustering;
			Parameters* AlgorithmParameters;
			DistanceMetric* Metric;
			FileIO fileIO;


			void GenerateCentroids()
			{
				for (unsigned int i = 0; i < AlgorithmParameters->CountOfClusters; i++)
				{
					for (unsigned int j = 0; j < AlgorithmParameters->CountOfDimensions; j++)
					{
						Centroids[i*AlgorithmParameters->CountOfDimensions + j] = GetRandomDouble();
					}
				}
			}

			inline void normalizeArray(double* row, int length) const {
				double sum = 0;
				for (auto i = 0; i < length; i++) {
					sum += row[i];
				}
				for (auto i = 0; i < length; i++) {
					row[i] /= sum;
				}
			}

			void copyArray(double* from, double* to, unsigned int length)
			{	
				for (unsigned int i = 0; i < length;i++)
				{
					to[i] = from[i];
				}
			}

		public:

			double* Centroids;
			double* ResultMatrix;
			double* DistanceMatrix;


			Clustering(Parameters* algorithm_parameters, DistanceMetric* metric, FileIO _fileIO)
			{
				fileIO = fileIO;
				AlgorithmParameters = algorithm_parameters;
				Metric = metric;

				VectorsForClustering = allocateAlign<double>(AlgorithmParameters->CountOfObjects*AlgorithmParameters->CountOfDimensions);
				DistanceMatrix = allocateAlign<double>(AlgorithmParameters->CountOfObjects*AlgorithmParameters->CountOfObjects);
				Centroids = allocateAlign<double>(AlgorithmParameters->CountOfClusters * AlgorithmParameters->CountOfDimensions);				
			}

			virtual void StartClustering() {};

			virtual void StartClustering(double* centroids) {};

			void CalculateAllDistance()
			{
				for (unsigned int i = 0; i < AlgorithmParameters->CountOfObjects; i++)
				{
					for (unsigned int j = 0; j < AlgorithmParameters->CountOfObjects; j++)
					{
						DistanceMatrix[i*AlgorithmParameters->CountOfObjects + j] = Metric->CalculateDistance(
							&VectorsForClustering[i*AlgorithmParameters->CountOfDimensions],
							&VectorsForClustering[j*AlgorithmParameters->CountOfDimensions],
							AlgorithmParameters->CountOfDimensions
						);
					}
				}
			}

			virtual bool TrySaveData() {return false;}

			virtual ~Clustering()
			{
				/*freeAlign(ResultMatrix);
				freeAlign(_powMatrix);
				freeAlign(Centroids);*/
			}
		};
	}
}
