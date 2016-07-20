#pragma once
#include <functional>
#include <iostream>
#include <algorithm>

#include "Helper.hpp"
#include "Clustering.hpp"

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
				for (int i = 0; i < Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfClusters; i++)
				{
					sum+=Clustering<IncomingType, OutcommingType>::DistanceCalculate(Clustering<IncomingType, OutcommingType>::Centroids, centroids, Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfDimensions);
				}
				return sum<=Clustering<IncomingType, OutcommingType>::AlgorithmParameters->Epsilon;
			}

			void GenerateCentroids() 
			{				
				for (int i = 0; i < Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfClusters; i++)
				{
					for (int j = 0; j < Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfDimensions; j++)
					{
						double r = GetRandomDouble();

						Clustering<IncomingType, OutcommingType>::Centroids[i*Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfDimensions + j] = r;
					}
				}
			}

			void GenerateDefaultResultMatrix()
			{
				for (int i = 0; i < Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfObjects;i++)
				{
					Clustering<IncomingType, OutcommingType>::ResultMatrix[i] = 0;
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
					copy(centroids,
						centroids + sizeof(IncomingType)*Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfClusters*Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfDimensions,
						Clustering<IncomingType, OutcommingType>::Centroids);

				}
			}

			void SetClusters(IncomingType* centroids)
			{
				for (int i = 0; i < Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfObjects; i++)
				{
					int numCluster = Clustering<IncomingType, OutcommingType>::ResultMatrix[i];
					double distanceToCluster = Clustering<IncomingType, OutcommingType>::DistanceCalculate(&Clustering<IncomingType, OutcommingType>::VectorsForClustering[i*Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfDimensions], &centroids[numCluster*Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfDimensions], Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfDimensions);
					for (int j = 0; j < Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfClusters; j++)
					{
						double distance = Clustering<IncomingType, OutcommingType>::DistanceCalculate(&Clustering<IncomingType, OutcommingType>::VectorsForClustering[i*Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfDimensions], &centroids[j*Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfDimensions], Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfDimensions);
						if (distance < distanceToCluster)
						{
							Clustering<IncomingType, OutcommingType>::ResultMatrix[i] = j;
							distanceToCluster = distance;
						}
					}
				}
			}

			void CalculateCentroids(IncomingType* centroids)
			{
				for (int i = 0; i < Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfClusters; i++)
				{
					for (int j = 0; j < Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfDimensions; j++)
					{
						centroids[i*Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfDimensions + j] = 0;
						long count = 0;
						for (int k = 0; k < Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfObjects; k++)
						{
							if (Clustering<IncomingType, OutcommingType>::ResultMatrix[k] == i) {
								centroids[i*Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfDimensions + j] += Clustering<IncomingType, OutcommingType>::VectorsForClustering[k*Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfDimensions + j];
								count++;
							}
						}
						centroids[i*Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfDimensions + j] /= count;
					}
				}
			}
			
		public:


			KMeans(IncomingType* data, Parameters* algorithm_parameters, function<double(IncomingType*, IncomingType*, long)> distance)
				: Clustering<IncomingType, OutcommingType>(data, algorithm_parameters, distance)
			{
				Clustering<IncomingType, OutcommingType>::Centroids = allocateAlign<IncomingType>(Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfClusters * Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfDimensions);
				Clustering<IncomingType, OutcommingType>::ResultMatrix = allocateAlign<OutcommingType>(Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfObjects);
				
			}

			void StartClustering()
			{
				GenerateCentroids();
				IncomingType* centroids = allocateAlign<IncomingType>(Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfClusters*Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfDimensions);
				copy(Clustering<IncomingType, OutcommingType>::Centroids,
					Clustering<IncomingType, OutcommingType>::Centroids + sizeof(IncomingType)*Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfClusters*Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfDimensions,
					centroids);

				GenerateDefaultResultMatrix();
				
				ExecuteClustering(centroids);

				freeAlign<IncomingType>(centroids);
			}

			void StartClustering(IncomingType* centroids)
			{
				copy(centroids,
					centroids + sizeof(IncomingType)*Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfClusters*Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfDimensions,
					Clustering<IncomingType, OutcommingType>::Centroids);


				GenerateDefaultResultMatrix();

				ExecuteClustering(centroids);
			}

			virtual ~KMeans()
			{
				freeAlign(Clustering<IncomingType, OutcommingType>::ResultMatrix);
				freeAlign(Clustering<IncomingType, OutcommingType>::Centroids);
			}
		};
	}
}