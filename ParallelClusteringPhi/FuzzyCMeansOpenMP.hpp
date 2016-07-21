#pragma once

/*
"Реализация параллельных версиий алгоритов кластеризации данных
с использованием многоядерных ускорителей"

Класс реализующий алгоритм кластеризации Fuzzy C-means.
Авторы: Сулейманов Д.Э., Цымблер М.Л.
*/

#include <functional>
#include <iostream>
#include <algorithm>
#include <omp.h>

#include "Helper.hpp"
#include "Clustering.hpp"
#include "FuzzyCMeans.hpp"

namespace ParallelClustering {

	namespace CMeansCollection {

		using namespace std;

		template<class IncomingType, class OutcommingType>
		class FuzzyCMeansOpenMP : public FuzzyCMeans<IncomingType, OutcommingType>
		{

		protected:


			inline void GenerateCentroids()
			{
				for (int i = 0; i < Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfClusters; i++)
				{
					for (int j = 0; j < Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfDimensions; j++)
					{
						Clustering<IncomingType, OutcommingType>::Centroids[i*Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfDimensions + j] = GetRandomDouble();
					}
				}
			}

			inline void GenerateDefaultResultMatrix()
			{
				#pragma omp parallel for
				for (int i = 0; i < Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfObjects;i++)
				{
					for (int j = 0; j < Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfClusters;j++)
					{
						Clustering<IncomingType, OutcommingType>::ResultMatrix[i*Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfClusters + j] = GetRandomDouble();
					}
					normalizeArray(&Clustering<IncomingType, OutcommingType>::ResultMatrix[i*Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfClusters], Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfClusters);
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
				for (int i = 0; i < Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfClusters; i++)
				{
					sum += Clustering<IncomingType, OutcommingType>::DistanceCalculate(Clustering<IncomingType, OutcommingType>::Centroids, centroids, Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfDimensions);
				}
				return sum <= Clustering<IncomingType, OutcommingType>::AlgorithmParameters->Epsilon;
			}

			inline virtual void ExecuteClustering(IncomingType* centroids)
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

			inline void SetClusters(IncomingType* centroids)
			{
				#pragma omp parallel for
				for (int i = 0; i < Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfObjects;i++) {
					for (int j = 0; j < Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfClusters; j++) {
						double distance = Clustering<IncomingType, OutcommingType>::DistanceCalculate(&Clustering<IncomingType, OutcommingType>::VectorsForClustering[i*Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfDimensions], &centroids[j*Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfDimensions], Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfDimensions);
						Clustering<IncomingType, OutcommingType>::ResultMatrix[i*Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfClusters + j] = pow(1 / distance, 2 / (Clustering<IncomingType, OutcommingType>::AlgorithmParameters->Fuzzy - 1));
					}
					normalizeArray(&Clustering<IncomingType, OutcommingType>::ResultMatrix[i*Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfClusters], Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfClusters);
				}
			}

			inline void CalculateCentroids(IncomingType* centroids)
			{
				
				#pragma omp parallel for simd
				for (int i = 0; i < Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfObjects; i++) {
					for (int j = 0; j < Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfClusters; j++) {
						FuzzyCMeans<IncomingType, OutcommingType>::_powMatrix[i*Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfClusters + j] = pow(Clustering<IncomingType, OutcommingType>::ResultMatrix[i*Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfClusters + j], Clustering<IncomingType, OutcommingType>::AlgorithmParameters->Fuzzy);
					}
				}

				#pragma omp parallel for collapse(2)
				for (int i = 0; i < Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfClusters; i++) {
					for (int d = 0; d < Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfDimensions; d++) {
						double numenator = 0.0;
						double denomenator = 0.0;
						#pragma omp simd reduction(+:numenator, denomenator)
						for (int j = 0; j < Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfObjects; j++) {
							numenator += FuzzyCMeans<IncomingType, OutcommingType>::_powMatrix[j*Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfClusters + i] * Clustering<IncomingType, OutcommingType>::VectorsForClustering[j*Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfDimensions + d];
							denomenator += FuzzyCMeans<IncomingType, OutcommingType>::_powMatrix[j*Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfClusters + i];

						}
						centroids[i*Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfClusters + d] = numenator / denomenator;
					}
				}
			}



		public:


			FuzzyCMeansOpenMP(IncomingType* data, Parameters* algorithm_parameters, function<double(IncomingType*, IncomingType*, long)> distance)
				: FuzzyCMeans<IncomingType, OutcommingType>(data, algorithm_parameters, distance)	{}

			virtual void StartClustering()
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

			virtual void StartClustering(IncomingType* centroids)
			{
				copy(centroids,
					centroids + sizeof(IncomingType)*Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfClusters*Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfDimensions,
					Clustering<IncomingType, OutcommingType>::Centroids);

				GenerateDefaultResultMatrix();

				ExecuteClustering(centroids);
			}

			virtual ~FuzzyCMeansOpenMP() {}
		};
	}
}