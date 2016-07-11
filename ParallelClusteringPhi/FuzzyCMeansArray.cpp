/*
"Реализация параллельных версиий алгоритов кластеризации данных
с использованием многоядерных ускорителей"

Класс реализующий алгоритм кластеризации Fuzzy C-means.
Авторы: Сулейманов Д.Э., Цымблер М.Л.
*/

#include "FuzzyCMeansArray.h"


namespace ParallelClustering 
{

	namespace FuzzyCMeansCollection
	{

		FuzzyCMeansArray::FuzzyCMeansArray(double* objects, Parameters algorithm_parameters, function<double(double*, long long,  double*, long long)> metrics)
		{
			//printArray(objects, algorithm_parameters.CountOfDimensions, algorithm_parameters.CountOfObjects, "vectors_");
			VectorsForClustering = objects;
			AlgorithmParameters = algorithm_parameters;
			DistanceCalculate = metrics;			
		}


		FuzzyCMeansArray::~FuzzyCMeansArray()
		{

		}

		void FuzzyCMeansArray::StartClustering(double* centroids)
		{

			UMatrix		=	(double*)_mm_malloc(AlgorithmParameters.CountOfObjects * AlgorithmParameters.CountOfClusters*sizeof(double), 64);
			_powUMatrix =	(double*)_mm_malloc(AlgorithmParameters.CountOfObjects * AlgorithmParameters.CountOfClusters*sizeof(double), 64);
			
			generateUMatrix();
			double previousDecisionValue = 0;
			double currentDecisionValue = 1;


			int start = clock();
			

			while (abs(previousDecisionValue - currentDecisionValue) > AlgorithmParameters.Epsilon)
			{
				previousDecisionValue = currentDecisionValue;
				calculateCentroids(centroids);
				for (int i = 0; i < AlgorithmParameters.CountOfObjects;i++) {
					for (int j = 0; j < AlgorithmParameters.CountOfClusters; j++) {
						double distance = DistanceCalculate(&VectorsForClustering[i*AlgorithmParameters.CountOfDimensions], AlgorithmParameters.CountOfDimensions, &centroids[j*AlgorithmParameters.CountOfDimensions], AlgorithmParameters.CountOfDimensions);
						UMatrix[i*AlgorithmParameters.CountOfClusters+j] = pow(1 / distance, 2 / (AlgorithmParameters.Fuzzy - 1));
					}
					normalizeUMatrixRow(&UMatrix[i*AlgorithmParameters.CountOfClusters], AlgorithmParameters.CountOfClusters);					
				}
				currentDecisionValue = calculateDecisionFunction(centroids);
				//cout<<"DESIDION: "<<abs(previousDecisionValue - currentDecisionValue)<<"____"<< (abs(previousDecisionValue - currentDecisionValue)>AlgorithmParameters.Epsilon)<<endl;
				//printArray(UMatrix, AlgorithmParameters.CountOfClusters, AlgorithmParameters.CountOfObjects, "UMATRIX");
			}
			ClearRuntime = (double)(clock() - start) / (CLOCKS_PER_SEC);
		}


		void FuzzyCMeansArray::generateUMatrix() {
			for (int i = 0; i < AlgorithmParameters.CountOfObjects; i++) {
				for (int j = 0; j < AlgorithmParameters.CountOfClusters; j++) {
					UMatrix[i*AlgorithmParameters.CountOfClusters+j] = GetRandomDouble();
				}
				normalizeUMatrixRow(&UMatrix[i*AlgorithmParameters.CountOfClusters],AlgorithmParameters.CountOfClusters);
			}
		}

		void FuzzyCMeansArray::normalizeUMatrixRow(double* row, int length) {
			double sum = 0;
			for (int i = 0; i < length; i++) {
				sum += row[i];
			}
			for (int i = 0; i < length; i++) {
				row[i] /= sum;
			}
		}

		void FuzzyCMeansArray::calculateCentroids(double* centroids)
		{
			for (int i = 0; i < AlgorithmParameters.CountOfObjects; i++) {
				
				for (int j = 0; j < AlgorithmParameters.CountOfClusters; j++) {
					_powUMatrix[i*AlgorithmParameters.CountOfClusters+j]=pow(UMatrix[i*AlgorithmParameters.CountOfClusters+j],AlgorithmParameters.Fuzzy);
				}
			}
			//printArray(UMatrix, AlgorithmParameters.CountOfClusters, AlgorithmParameters.CountOfObjects, "UMatrix");
			//printArray(_powUMatrix, AlgorithmParameters.CountOfClusters, AlgorithmParameters.CountOfObjects, "powMatrix");
			//printArray(centroids, AlgorithmParameters.CountOfDimensions, AlgorithmParameters.CountOfClusters, "centroids before");
			for (int i = 0; i < AlgorithmParameters.CountOfClusters; i++) {
				for (int d = 0; d < AlgorithmParameters.CountOfDimensions; d++) {
					double numenator = 0.0;
					double denomenator = 0.0;
					for (int j = 0; j < AlgorithmParameters.CountOfObjects; j ++) {
						
							numenator += _powUMatrix[j*AlgorithmParameters.CountOfClusters+i] * VectorsForClustering[j*AlgorithmParameters.CountOfDimensions + d];
							denomenator += _powUMatrix[j*AlgorithmParameters.CountOfClusters + i];
						
					}
				//	cout << "numenator " << i << "_" << d << ":" << numenator<<endl;
				//	cout << "denomenator " << i << "_" << d << ":" << denomenator << endl;
					centroids[i*AlgorithmParameters.CountOfClusters + d] = numenator / denomenator;
				}
			}
			//printArray(centroids, AlgorithmParameters.CountOfDimensions, AlgorithmParameters.CountOfClusters, "centroids after");
		
		}

		
		double FuzzyCMeansArray::calculateDecisionFunction(double* centroids) {
			double sum = 0;
			for (int i = 0; i < AlgorithmParameters.CountOfObjects;i++) {				
				for (int j = 0; j < AlgorithmParameters.CountOfClusters; j++) {
					sum += DistanceCalculate(&centroids[j*AlgorithmParameters.CountOfDimensions], AlgorithmParameters.CountOfDimensions, &UMatrix[i*AlgorithmParameters.CountOfClusters], AlgorithmParameters.CountOfClusters);
				}				
			}
			return sum;
		}
	}
}
