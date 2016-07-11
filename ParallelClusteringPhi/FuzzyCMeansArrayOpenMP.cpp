/*
"Реализация параллельных версиий алгоритов кластеризации данных
с использованием многоядерных ускорителей"

Класс реализующий алгоритм кластеризации Fuzzy C-means.
Авторы: Сулейманов Д.Э., Цымблер М.Л.
*/

#include "FuzzyCMeansArrayOpenMP.h"


namespace ParallelClustering 
{

	namespace FuzzyCMeansCollection
	{

		FuzzyCMeansArrayOpenMP::FuzzyCMeansArrayOpenMP(double* objects, Parameters algorithm_parameters, function<double(double*, long long,  double*, long long)> metrics)
		{
			//printArray(objects, algorithm_parameters.CountOfDimensions, algorithm_parameters.CountOfObjects, "vectors_");
			VectorsForClustering = objects;
			AlgorithmParameters = algorithm_parameters;
			DistanceCalculate = metrics;			
		}


		FuzzyCMeansArrayOpenMP::~FuzzyCMeansArrayOpenMP()
		{

		}

		void FuzzyCMeansArrayOpenMP::StartClustering(double* centroids)
		{

			UMatrix		=	(double*)_mm_malloc(AlgorithmParameters.CountOfObjects * AlgorithmParameters.CountOfClusters*sizeof(double), 64);
			_powUMatrix =	(double*)_mm_malloc(AlgorithmParameters.CountOfObjects * AlgorithmParameters.CountOfClusters*sizeof(double), 64);
			
			#pragma omp parallel for
			for (int i = 0; i < AlgorithmParameters.CountOfObjects; i++) {
				double sum = 0;
				for (int j = 0; j < AlgorithmParameters.CountOfClusters; j++) {
					double value = (static_cast <double> (rand()) / static_cast <double> (RAND_MAX));
					UMatrix[i*AlgorithmParameters.CountOfClusters+j] = value;
					sum+=value;
				}
				#pragma omp simd
				for (int j = 0; j < AlgorithmParameters.CountOfClusters; j++) {
						UMatrix[i*AlgorithmParameters.CountOfClusters+j] /= sum;
				}
			}
			
			double previousDecisionValue = 0;
			double currentDecisionValue = 1;

			omp_set_num_threads(AlgorithmParameters.CountOfThreads);
			
			while (abs(previousDecisionValue - currentDecisionValue) > AlgorithmParameters.Epsilon)
			{
				previousDecisionValue = currentDecisionValue;
				calculateCentroids(centroids);
				#pragma omp parallel for
				for (int i = 0; i < AlgorithmParameters.CountOfObjects;i++) {					
					double sum = 0;
					for (int j = 0; j < AlgorithmParameters.CountOfClusters; j++) {
						double distance = DistanceCalculate(&VectorsForClustering[i*AlgorithmParameters.CountOfDimensions], AlgorithmParameters.CountOfDimensions, &centroids[j*AlgorithmParameters.CountOfDimensions], AlgorithmParameters.CountOfDimensions);
						double value = pow(1 / distance, 2 / (AlgorithmParameters.Fuzzy - 1));
						UMatrix[i*AlgorithmParameters.CountOfClusters+j] = value;
						sum+=value;
					}
					#pragma omp simd
					for (int j = 0; j < AlgorithmParameters.CountOfClusters; j++) {
						UMatrix[i*AlgorithmParameters.CountOfClusters+j] /= sum;
					}
				}
				currentDecisionValue = 0;
				#pragma omp parallel for simd reduction(+:currentDecisionValue)
				for (int i = 0; i < AlgorithmParameters.CountOfObjects;i++) {				
					for (int j = 0; j < AlgorithmParameters.CountOfClusters; j++) {
						currentDecisionValue += DistanceCalculate(&centroids[j*AlgorithmParameters.CountOfDimensions], AlgorithmParameters.CountOfDimensions, &UMatrix[i*AlgorithmParameters.CountOfClusters], AlgorithmParameters.CountOfClusters);
					}				
				}
			}
			
		}


		void FuzzyCMeansArrayOpenMP::generateUMatrix() {
			#pragma omp parallel for
			for (int i = 0; i < AlgorithmParameters.CountOfObjects; i++) {
				for (int j = 0; j < AlgorithmParameters.CountOfClusters; j++) {
					UMatrix[i*AlgorithmParameters.CountOfClusters+j] = GetRandomDouble();
				}
				normalizeUMatrixRow(&UMatrix[i*AlgorithmParameters.CountOfClusters],AlgorithmParameters.CountOfClusters);
			}
		}

		void FuzzyCMeansArrayOpenMP::normalizeUMatrixRow(double* row, int length) {
			double sum = 0;
			for (int i = 0; i < length; i++) {
				sum += row[i];
			}
			for (int i = 0; i < length; i++) {
				row[i] /= sum;
			}
		}

		void FuzzyCMeansArrayOpenMP::calculateCentroids(double* centroids)
		{
			#pragma omp parallel for simd
			for (int i = 0; i < AlgorithmParameters.CountOfObjects; i++) {				
				for (int j = 0; j < AlgorithmParameters.CountOfClusters; j++) {
					_powUMatrix[i*AlgorithmParameters.CountOfClusters+j]=pow(UMatrix[i*AlgorithmParameters.CountOfClusters+j],AlgorithmParameters.Fuzzy);
				}
			}
			
			#pragma omp parallel for collapse(2)
			for (int d = 0; d < AlgorithmParameters.CountOfDimensions; d++) {
				for (int i = 0; i < AlgorithmParameters.CountOfClusters; i++) {
					double numenator = 0.0;
					double denomenator = 0.0; 
					#pragma omp simd reduction(+:numenator, denomenator)
					for (int j = 0; j < AlgorithmParameters.CountOfObjects; j++) {
						
							numenator += _powUMatrix[j*AlgorithmParameters.CountOfClusters+i] * VectorsForClustering[j*AlgorithmParameters.CountOfDimensions + d];
							denomenator += _powUMatrix[j*AlgorithmParameters.CountOfClusters + i];
						
					}
					centroids[i*AlgorithmParameters.CountOfClusters + d] = numenator / denomenator;
				}
			}
		}
		
		double FuzzyCMeansArrayOpenMP::calculateDecisionFunction(double* centroids) {
			double sum = 0;
			#pragma omp parallel for simd reduction(+:sum)
			for (int i = 0; i < AlgorithmParameters.CountOfObjects;i++) {				
				for (int j = 0; j < AlgorithmParameters.CountOfClusters; j++) {
					sum += DistanceCalculate(&centroids[j*AlgorithmParameters.CountOfDimensions], AlgorithmParameters.CountOfDimensions, &UMatrix[i*AlgorithmParameters.CountOfClusters], AlgorithmParameters.CountOfClusters);
				}				
			}
			return sum;
		}
	}
}
