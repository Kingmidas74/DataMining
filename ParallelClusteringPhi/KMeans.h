#pragma once
#include <functional>
#include <iostream>

#include "Helper.h"
#include "Clustering.h"

namespace ParallelClustering {

	namespace KMeansCollection {

		using namespace std;

		template<class IncomingType, class OutcommingType>
		class KMeans : public Clustering<IncomingType, OutcommingType>
		{

		protected:
			bool calculateDecision(IncomingType* centroids)
			{
				double sum = 0;
				for (int i = 0; i < AlgorithmParameters->CountOfClusters; i++)
				{
					sum+=DistanceCalculate(Centroids, centroids, AlgorithmParameters->CountOfDimensions);
				}
				return sum<=AlgorithmParameters->Epsilon;
			}

			void GenerateCentroids() 
			{				
				for (int i = 0; i < AlgorithmParameters->CountOfClusters; i++)
				{
					for (int j = 0; j < AlgorithmParameters->CountOfDimensions; j++)
					{
						double r = GetRandomDouble();

						Centroids[i*AlgorithmParameters->CountOfDimensions + j] = r;
					}
				}
			}

			void GenerateDefaultResultMatrix()
			{
				for (int i = 0; i < AlgorithmParameters->CountOfObjects;i++)
				{
					ResultMatrix[i] = 0;
				}
			}

			void ExecuteClustering(IncomingType* centroids)
			{
				bool decision = false;
				while (decision == false)
				{	
					SetClusters(centroids);
					CalculateCentroids(centroids);
					decision = calculateDecision(centroids);
					memcpy(Centroids, centroids, sizeof(IncomingType)*AlgorithmParameters->CountOfClusters*AlgorithmParameters->CountOfDimensions);
				}
			}

			void SetClusters(IncomingType* centroids)
			{
				for (int i = 0; i < AlgorithmParameters->CountOfObjects; i++)
				{
					int numCluster = ResultMatrix[i];
					double distanceToCluster = DistanceCalculate(&VectorsForClustering[i*AlgorithmParameters->CountOfDimensions], &centroids[numCluster*AlgorithmParameters->CountOfDimensions], AlgorithmParameters->CountOfDimensions);
					for (int j = 0; j < AlgorithmParameters->CountOfClusters; j++)
					{
						double distance = DistanceCalculate(&VectorsForClustering[i*AlgorithmParameters->CountOfDimensions], &centroids[j*AlgorithmParameters->CountOfDimensions], AlgorithmParameters->CountOfDimensions);
						if (distance < distanceToCluster)
						{
							ResultMatrix[i] = j;
							distanceToCluster = distance;
						}
					}
				}
			}

			void CalculateCentroids(IncomingType* centroids)
			{
				for (int i = 0; i < AlgorithmParameters->CountOfClusters; i++)
				{
					for (int j = 0; j < AlgorithmParameters->CountOfDimensions; j++)
					{
						centroids[i*AlgorithmParameters->CountOfDimensions + j] = 0;
						long count = 0;
						for (int k = 0; k < AlgorithmParameters->CountOfObjects; k++)
						{
							if (ResultMatrix[k] == i) {
								centroids[i*AlgorithmParameters->CountOfDimensions + j] += VectorsForClustering[k*AlgorithmParameters->CountOfDimensions + j];
								count++;
							}
						}
						centroids[i*AlgorithmParameters->CountOfDimensions + j] /= count;
					}
				}
			}
			
		public:


			KMeans(IncomingType* data, Parameters* algorithm_parameters, function<double(IncomingType*, IncomingType*, long)> distance)
				: Clustering(data, algorithm_parameters, distance)
			{
				Centroids = allocateAlign<IncomingType>(AlgorithmParameters->CountOfClusters * AlgorithmParameters->CountOfDimensions);
				ResultMatrix = allocateAlign<OutcommingType>(AlgorithmParameters->CountOfObjects);
				
			}

			void StartClustering()
			{
				GenerateCentroids();
				IncomingType* centroids = allocateAlign<IncomingType>(AlgorithmParameters->CountOfClusters*AlgorithmParameters->CountOfDimensions);
				memcpy(centroids, Centroids, sizeof(IncomingType)*AlgorithmParameters->CountOfClusters*AlgorithmParameters->CountOfDimensions);
				
				GenerateDefaultResultMatrix();
				
				ExecuteClustering(centroids);

				freeAlign<IncomingType>(centroids);
			}

			void StartClustering(IncomingType* centroids)
			{
				memcpy(Centroids, centroids, sizeof(IncomingType)*AlgorithmParameters->CountOfClusters*AlgorithmParameters->CountOfDimensions);

				GenerateDefaultResultMatrix();

				ExecuteClustering(centroids);
			}

			virtual ~KMeans()
			{
				freeAlign(ResultMatrix);
				freeAlign(Centroids);
			}
		};
	}
}