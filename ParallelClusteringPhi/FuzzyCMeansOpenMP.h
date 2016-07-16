#pragma once

/*
"Реализация параллельных версиий алгоритов кластеризации данных
с использованием многоядерных ускорителей"

Класс реализующий алгоритм кластеризации Fuzzy C-means.
Авторы: Сулейманов Д.Э., Цымблер М.Л.
*/

#include <functional>
#include <iostream>
#include <omp.h>

#include "Helper.h"
#include "Clustering.h"
#include "FuzzyCMeans.h"

namespace ParallelClustering {

	namespace CMeansCollection {

		using namespace std;

		template<class IncomingType, class OutcommingType>
		class FuzzyCMeansOpenMP : public FuzzyCMeans<IncomingType, OutcommingType>
		{

		protected:


			inline void GenerateCentroids()
			{
				for (int i = 0; i < AlgorithmParameters->CountOfClusters; i++)
				{
					for (int j = 0; j < AlgorithmParameters->CountOfDimensions; j++)
					{
						Centroids[i*AlgorithmParameters->CountOfDimensions + j] = GetRandomDouble();
					}
				}
			}

			inline void GenerateDefaultResultMatrix()
			{
				#pragma omp parallel for
				for (int i = 0; i < AlgorithmParameters->CountOfObjects;i++)
				{
					for (int j = 0; j < AlgorithmParameters->CountOfClusters;j++)
					{
						ResultMatrix[i*AlgorithmParameters->CountOfClusters + j] = GetRandomDouble();
					}
					normalizeArray(&ResultMatrix[i*AlgorithmParameters->CountOfClusters], AlgorithmParameters->CountOfClusters);
				}
			}

			inline void normalizeArray(double* row, int length) {
				double sum = 0;
				#pragma omp simd reduction(+:sum)
				for (int i = 0; i < length; i++) {
					sum += row[i];
				}
				#pragma omp simd
				for (int i = 0; i < length; i++) {
					row[i] /= sum;
				}
			}

			inline bool calculateDecision(IncomingType* centroids)
			{
				double sum = 0;
				#pragma omp simd reduction(+:sum)
				for (int i = 0; i < AlgorithmParameters->CountOfClusters; i++)
				{
					sum += DistanceCalculate(Centroids, centroids, AlgorithmParameters->CountOfDimensions);
				}
				return sum <= AlgorithmParameters->Epsilon;
			}

			inline virtual void ExecuteClustering(IncomingType* centroids)
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

			inline void SetClusters(IncomingType* centroids)
			{
				#pragma omp parallel for
				for (int i = 0; i < AlgorithmParameters->CountOfObjects;i++) {
					for (int j = 0; j < AlgorithmParameters->CountOfClusters; j++) {
						double distance = DistanceCalculate(&VectorsForClustering[i*AlgorithmParameters->CountOfDimensions], &centroids[j*AlgorithmParameters->CountOfDimensions], AlgorithmParameters->CountOfDimensions);
						ResultMatrix[i*AlgorithmParameters->CountOfClusters + j] = pow(1 / distance, 2 / (AlgorithmParameters->Fuzzy - 1));
					}
					normalizeArray(&ResultMatrix[i*AlgorithmParameters->CountOfClusters], AlgorithmParameters->CountOfClusters);
				}
			}

			inline void CalculateCentroids(IncomingType* centroids)
			{
				
				#pragma omp parallel for simd
				for (int i = 0; i < AlgorithmParameters->CountOfObjects; i++) {
					for (int j = 0; j < AlgorithmParameters->CountOfClusters; j++) {
						_powMatrix[i*AlgorithmParameters->CountOfClusters + j] = pow(ResultMatrix[i*AlgorithmParameters->CountOfClusters + j], AlgorithmParameters->Fuzzy);
					}
				}

				#pragma omp parallel for collapse(2)
				for (int i = 0; i < AlgorithmParameters->CountOfClusters; i++) {
					for (int d = 0; d < AlgorithmParameters->CountOfDimensions; d++) {
						double numenator = 0.0;
						double denomenator = 0.0;
						#pragma omp simd reduction(+:numenator, denomenator)
						for (int j = 0; j < AlgorithmParameters->CountOfObjects; j++) {
							numenator += _powMatrix[j*AlgorithmParameters->CountOfClusters + i] * VectorsForClustering[j*AlgorithmParameters->CountOfDimensions + d];
							denomenator += _powMatrix[j*AlgorithmParameters->CountOfClusters + i];

						}
						centroids[i*AlgorithmParameters->CountOfClusters + d] = numenator / denomenator;
					}
				}
			}



		public:


			FuzzyCMeansOpenMP(IncomingType* data, Parameters* algorithm_parameters, function<double(IncomingType*, IncomingType*, long)> distance)
				: FuzzyCMeans(data, algorithm_parameters, distance)	{}

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

			virtual ~FuzzyCMeansOpenMP() {}
		};
	}
}