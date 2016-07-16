#pragma once

/*
"Реализация параллельных версиий алгоритов кластеризации данных
с использованием многоядерных ускорителей"

Класс реализующий алгоритм кластеризации Fuzzy C-means.
Авторы: Сулейманов Д.Э., Цымблер М.Л.
*/

#include <functional>
#include <iostream>

#include "Helper.h"
#include "Clustering.h"

namespace ParallelClustering {

	namespace CMeansCollection {

		using namespace std;

		template<class IncomingType, class OutcommingType>
		class FuzzyCMeans : public Clustering<IncomingType, OutcommingType>
		{

		protected:

			OutcommingType* _powMatrix;

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
					for (int j = 0; j < AlgorithmParameters->CountOfClusters;j++)
					{
						ResultMatrix[i*AlgorithmParameters->CountOfClusters+j] = GetRandomDouble();
					}
					normalizeArray(&ResultMatrix[i*AlgorithmParameters->CountOfClusters], AlgorithmParameters->CountOfClusters);
				}
			}

			void normalizeArray(double* row, int length) {
				double sum = 0;
				for (int i = 0; i < length; i++) {
					sum += row[i];
				}
				for (int i = 0; i < length; i++) {
					row[i] /= sum;
				}
			}

			bool calculateDecision(IncomingType* centroids)
			{
				double sum = 0;
				for (int i = 0; i < AlgorithmParameters->CountOfClusters; i++)
				{
					sum += DistanceCalculate(Centroids, centroids, AlgorithmParameters->CountOfDimensions);
				}
				return sum <= AlgorithmParameters->Epsilon;
			}

			virtual void ExecuteClustering(IncomingType* centroids)
			{
				bool decision = false;
				
				while (decision==false)
				{
					SetClusters(centroids);
					CalculateCentroids(centroids);
					decision = calculateDecision(centroids);
					memcpy(Centroids, centroids, sizeof(IncomingType)*AlgorithmParameters->CountOfClusters*AlgorithmParameters->CountOfDimensions);
				}
			}

			void SetClusters(IncomingType* centroids)
			{
				for (int i = 0; i < AlgorithmParameters->CountOfObjects;i++) {
					for (int j = 0; j < AlgorithmParameters->CountOfClusters; j++) {
						double distance = DistanceCalculate(&VectorsForClustering[i*AlgorithmParameters->CountOfDimensions], &centroids[j*AlgorithmParameters->CountOfDimensions], AlgorithmParameters->CountOfDimensions);
						ResultMatrix[i*AlgorithmParameters->CountOfClusters + j] = pow(1 / distance, 2 / (AlgorithmParameters->Fuzzy - 1));
					}
					normalizeArray(&ResultMatrix[i*AlgorithmParameters->CountOfClusters], AlgorithmParameters->CountOfClusters);
				}
			}

			void CalculateCentroids(IncomingType* centroids)
			{
				for (int i = 0; i < AlgorithmParameters->CountOfObjects; i++) {
					for (int j = 0; j < AlgorithmParameters->CountOfClusters; j++) {
						_powMatrix[i*AlgorithmParameters->CountOfClusters + j] = pow(ResultMatrix[i*AlgorithmParameters->CountOfClusters + j], AlgorithmParameters->Fuzzy);
					}
				}

				for (int i = 0; i < AlgorithmParameters->CountOfClusters; i++) {
					for (int d = 0; d < AlgorithmParameters->CountOfDimensions; d++) {
						double numenator = 0.0;
						double denomenator = 0.0;
						for (int j = 0; j < AlgorithmParameters->CountOfObjects; j++) {

							numenator += _powMatrix[j*AlgorithmParameters->CountOfClusters + i] * VectorsForClustering[j*AlgorithmParameters->CountOfDimensions + d];
							denomenator += _powMatrix[j*AlgorithmParameters->CountOfClusters + i];

						}
						centroids[i*AlgorithmParameters->CountOfClusters + d] = numenator / denomenator;
					}
				}
			}



		public:


			FuzzyCMeans(IncomingType* data, Parameters* algorithm_parameters, function<double(IncomingType*, IncomingType*, long)> distance)
				: Clustering(data, algorithm_parameters, distance)
			{
				Centroids = allocateAlign<IncomingType>(AlgorithmParameters->CountOfClusters * AlgorithmParameters->CountOfDimensions);
				ResultMatrix = allocateAlign<OutcommingType>(AlgorithmParameters->CountOfObjects*AlgorithmParameters->CountOfClusters);
				_powMatrix = allocateAlign<OutcommingType>(AlgorithmParameters->CountOfObjects*AlgorithmParameters->CountOfClusters);
			}

			virtual void StartClustering()
			{
				GenerateCentroids();
				IncomingType* centroids = allocateAlign<IncomingType>(AlgorithmParameters->CountOfClusters*AlgorithmParameters->CountOfDimensions);
				memcpy(centroids, Centroids, sizeof(IncomingType)*AlgorithmParameters->CountOfClusters*AlgorithmParameters->CountOfDimensions);

				GenerateDefaultResultMatrix();

				ExecuteClustering(centroids);

				freeAlign<IncomingType>(centroids);
			}

			virtual void StartClustering(IncomingType* centroids)
			{
				memcpy(Centroids, centroids, sizeof(IncomingType)*AlgorithmParameters->CountOfClusters*AlgorithmParameters->CountOfDimensions);

				GenerateDefaultResultMatrix();

				ExecuteClustering(centroids);
			}

			virtual ~FuzzyCMeans()
			{
				freeAlign(ResultMatrix);
				freeAlign(_powMatrix);
				freeAlign(Centroids);
			}
		};
	}
}