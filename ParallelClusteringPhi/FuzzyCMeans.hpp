#pragma once

/*
"Реализация параллельных версиий алгоритов кластеризации данных
с использованием многоядерных ускорителей"

Класс реализующий алгоритм кластеризации Fuzzy C-means.
Авторы: Сулейманов Д.Э., Цымблер М.Л.
*/

#include <functional>

#include "Helper.hpp"
#include "Clustering.hpp"

namespace ParallelClustering {

	namespace CMeansCollection {

		using namespace std;

		template<class IncomingType, class OutcommingType>
		class FuzzyCMeans : public Clustering<IncomingType, OutcommingType>
		{

		protected:

			OutcommingType* _powMatrix;

			virtual void GenerateCentroids()
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

			virtual void GenerateDefaultResultMatrix()
			{
				for (int i = 0; i < Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfObjects;i++)
				{
					for (int j = 0; j < Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfClusters;j++)
					{
						Clustering<IncomingType, OutcommingType>::ResultMatrix[i*Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfClusters+j] = GetRandomDouble();
					}
					normalizeArray(&Clustering<IncomingType, OutcommingType>::ResultMatrix[i*Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfClusters], Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfClusters);
				}
			}

			virtual void normalizeArray(double* row, int length) {
				double sum = 0;
				for (int i = 0; i < length; i++) {
					sum += row[i];
				}
				for (int i = 0; i < length; i++) {
					row[i] /= sum;
				}
			}

			virtual bool calculateDecision(IncomingType* centroids)
			{
				double sum = 0;
				for (int i = 0; i < Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfClusters; i++)
				{
					sum += Clustering<IncomingType, OutcommingType>::DistanceCalculate(Clustering<IncomingType, OutcommingType>::Centroids, centroids, Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfDimensions);
				}
				return sum <= Clustering<IncomingType, OutcommingType>::AlgorithmParameters->Epsilon;
			}

			void ExecuteClustering(IncomingType* centroids) override
			{
				bool decision = false;
				
				while (decision==false)
				{
					SetClusters(centroids);
					CalculateCentroids(centroids);
					decision = calculateDecision(centroids);
					copy(centroids, 
						centroids+sizeof(IncomingType)*Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfClusters*Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfDimensions,
						Clustering<IncomingType, OutcommingType>::Centroids);
				}
			}

			virtual void SetClusters(IncomingType* centroids)
			{
				for (int i = 0; i < Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfObjects;i++) {
					for (int j = 0; j < Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfClusters; j++) {
						double distance = Clustering<IncomingType, OutcommingType>::DistanceCalculate(&Clustering<IncomingType, OutcommingType>::VectorsForClustering[i*Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfDimensions], &centroids[j*Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfDimensions], Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfDimensions);
						Clustering<IncomingType, OutcommingType>::ResultMatrix[i*Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfClusters + j] = pow(1 / distance, 2 / (Clustering<IncomingType, OutcommingType>::AlgorithmParameters->Fuzzy - 1));
					}
					normalizeArray(&Clustering<IncomingType, OutcommingType>::ResultMatrix[i*Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfClusters], Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfClusters);
				}
			}

			virtual void CalculateCentroids(IncomingType* centroids)
			{
				for (int i = 0; i < Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfObjects; i++) {
					for (int j = 0; j < Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfClusters; j++) {
						_powMatrix[i*Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfClusters + j] = pow(Clustering<IncomingType, OutcommingType>::ResultMatrix[i*Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfClusters + j], Clustering<IncomingType, OutcommingType>::AlgorithmParameters->Fuzzy);
					}
				}

				for (int i = 0; i < Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfClusters; i++) {
					for (int d = 0; d < Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfDimensions; d++) {
						double numenator = 0.0;
						double denomenator = 0.0;
						for (int j = 0; j < Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfObjects; j++) {

							numenator += _powMatrix[j*Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfClusters + i] * Clustering<IncomingType, OutcommingType>::VectorsForClustering[j*Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfDimensions + d];
							denomenator += _powMatrix[j*Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfClusters + i];

						}
						centroids[i*Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfClusters + d] = numenator / denomenator;
					}
				}
			}



		public:


			FuzzyCMeans(IncomingType* data, Parameters* algorithm_parameters, function<double(IncomingType*, IncomingType*, long)> distance)
				: Clustering<IncomingType, OutcommingType>(data, algorithm_parameters, distance)
			{
				Clustering<IncomingType, OutcommingType>::Centroids = allocateAlign<IncomingType>(Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfClusters * Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfDimensions);
				Clustering<IncomingType, OutcommingType>::ResultMatrix = allocateAlign<OutcommingType>(Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfObjects*Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfClusters);
				_powMatrix = allocateAlign<OutcommingType>(Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfObjects*Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfClusters);
			}

			void StartClustering() override
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

			void StartClustering(IncomingType* centroids) override
			{	
				copy(centroids,
					centroids + sizeof(IncomingType)*Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfClusters*Clustering<IncomingType, OutcommingType>::AlgorithmParameters->CountOfDimensions,
					Clustering<IncomingType, OutcommingType>::Centroids);

				GenerateDefaultResultMatrix();

				ExecuteClustering(centroids);
			}

			virtual ~FuzzyCMeans()
			{
				freeAlign(Clustering<IncomingType, OutcommingType>::ResultMatrix);
				freeAlign(_powMatrix);
				freeAlign(Clustering<IncomingType, OutcommingType>::Centroids);
			}
		};
	}
}