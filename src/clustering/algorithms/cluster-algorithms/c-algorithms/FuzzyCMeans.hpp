#pragma once
#include <functional>

namespace ParallelClustering {
	namespace ClusteringAlgorithms {
		namespace CMeansCollection {


			using namespace std;

			class FuzzyCMeans
			{

			private:

				double* VectorsForClustering;
				double* Centroids;
				
				double* _powMatrix;

				Parameters* AlgorithmParameters;

				function<double(double*, double*, unsigned int, unsigned int)> DistanceCalculate;


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

				void GenerateDefaultResultMatrix()
				{
					for (unsigned int i = 0; i < AlgorithmParameters->CountOfObjects;i++)
					{
						for (unsigned int j = 0; j < AlgorithmParameters->CountOfClusters;j++)
						{
							ResultMatrix[i*AlgorithmParameters->CountOfClusters + j] = GetRandomDouble();
						}
						normalizeArray(&ResultMatrix[i*AlgorithmParameters->CountOfClusters], AlgorithmParameters->CountOfClusters);
					}
				}

				void ExecuteClustering(double* centroids)
				{
					bool decision = false;
					while (decision==false)
					{
						cout << "calcCent" << endl;
						CalculateCentroids(centroids);
						//PrintAsMatrix(ResultMatrix, AlgorithmParameters->CountOfClusters, AlgorithmParameters->CountOfClusters*AlgorithmParameters->CountOfObjects);
						cout << "setCl" << endl;
						SetClusters(centroids);															
						//PrintAsMatrix(ResultMatrix, AlgorithmParameters->CountOfClusters, AlgorithmParameters->CountOfClusters*AlgorithmParameters->CountOfObjects);
						cout << "calcDec" << endl;
						decision = calculateDecision(centroids);						
						//PrintAsMatrix(ResultMatrix, AlgorithmParameters->CountOfClusters, AlgorithmParameters->CountOfClusters*AlgorithmParameters->CountOfObjects);
						cout << "copyCent" << endl;
						copyToCentroids(centroids);
						//PrintAsMatrix(ResultMatrix, AlgorithmParameters->CountOfClusters, AlgorithmParameters->CountOfClusters*AlgorithmParameters->CountOfObjects);						
					}
				}

				void SetClusters(double* centroids)
				{
					for (unsigned int i = 0; i < AlgorithmParameters->CountOfObjects;i++) {
						for (unsigned int j = 0; j < AlgorithmParameters->CountOfClusters; j++) {
							double distance = DistanceCalculate(
								&VectorsForClustering[i*AlgorithmParameters->CountOfDimensions], 
								&centroids[j*AlgorithmParameters->CountOfDimensions], 
								AlgorithmParameters->CountOfDimensions,2);
							ResultMatrix[i*AlgorithmParameters->CountOfClusters + j] = pow(1.0 / distance, 2.0 / (AlgorithmParameters->Fuzzy - 1.0));
						}
						normalizeArray(&ResultMatrix[i*AlgorithmParameters->CountOfClusters], AlgorithmParameters->CountOfClusters);
					}

				}

				void CalculateCentroids(double* centroids)
				{
					
					for (unsigned int i = 0; i < AlgorithmParameters->CountOfObjects; i++) {
						for (unsigned int j = 0; j < AlgorithmParameters->CountOfClusters; j++) {
							_powMatrix[i*AlgorithmParameters->CountOfClusters + j] = pow(ResultMatrix[i*AlgorithmParameters->CountOfClusters + j], AlgorithmParameters->Fuzzy);
						}
					}

					for (unsigned int i = 0; i < AlgorithmParameters->CountOfClusters; i++) {
						for (unsigned int d = 0; d < AlgorithmParameters->CountOfDimensions; d++) {
							double numenator = 0.0;
							double denomenator = 0.0;
							for (unsigned int j = 0; j < AlgorithmParameters->CountOfObjects; j++) {

								numenator += _powMatrix[j*AlgorithmParameters->CountOfClusters + i] * VectorsForClustering[j*AlgorithmParameters->CountOfDimensions + d];
								denomenator += _powMatrix[j*AlgorithmParameters->CountOfClusters + i];

							}
							centroids[i*AlgorithmParameters->CountOfClusters + d] = numenator / denomenator;
						}
					}
				}

				bool calculateDecision(double* centroids)
				{
					for (unsigned int j = 0; j < AlgorithmParameters->CountOfClusters; j++)
					{
						if(DistanceCalculate(Centroids, centroids, AlgorithmParameters->CountOfDimensions, 2)>AlgorithmParameters->Epsilon)
						{
							return false;
						}
					}
					return true;
				}

				void copyToCentroids(double* centroids)
				{
					unsigned int length = AlgorithmParameters->CountOfClusters*AlgorithmParameters->CountOfDimensions;
					for(unsigned int i=0; i<length;i++)
					{
						Centroids[i] = centroids[i];
					}
				}

			public:

				double* ResultMatrix;
				double* DistanceMatrix;


				FuzzyCMeans(double* data, Parameters* algorithm_parameters, function<double(double*, double*, unsigned int, unsigned int)> distance)	
				{
					VectorsForClustering = data;
					AlgorithmParameters = algorithm_parameters;
					DistanceCalculate = distance;

					DistanceMatrix = allocateAlign<double>(AlgorithmParameters->CountOfObjects*AlgorithmParameters->CountOfObjects);
					Centroids = allocateAlign<double>(AlgorithmParameters->CountOfClusters * AlgorithmParameters->CountOfDimensions);
					ResultMatrix = allocateAlign<double>(AlgorithmParameters->CountOfObjects*AlgorithmParameters->CountOfClusters);
					_powMatrix = allocateAlign<double>(AlgorithmParameters->CountOfObjects*AlgorithmParameters->CountOfClusters);
				}

				void StartClustering()
				{
					GenerateCentroids();
					double* centroids = allocateAlign<double>(AlgorithmParameters->CountOfClusters*AlgorithmParameters->CountOfDimensions);

					copy(Centroids,
						Centroids + sizeof(double)*AlgorithmParameters->CountOfClusters*AlgorithmParameters->CountOfDimensions,
						centroids);
					
					GenerateDefaultResultMatrix();
					PrintAsMatrix(ResultMatrix, AlgorithmParameters->CountOfClusters, AlgorithmParameters->CountOfClusters*AlgorithmParameters->CountOfObjects);
					ExecuteClustering(centroids);
					PrintAsMatrix(ResultMatrix, AlgorithmParameters->CountOfClusters, AlgorithmParameters->CountOfClusters*AlgorithmParameters->CountOfObjects);
					//freeAlign<double>(centroids);
					
					
				}

				void StartClustering(double* centroids)
				{
					copy(centroids,
						centroids + sizeof(double)*AlgorithmParameters->CountOfClusters*AlgorithmParameters->CountOfDimensions,
						Centroids);

					GenerateDefaultResultMatrix();

					ExecuteClustering(centroids);
				}

				void CalculateAllDistance()
				{
					for (unsigned int i = 0; i < AlgorithmParameters->CountOfObjects; i++)
					{
						for (unsigned int j = 0; j < AlgorithmParameters->CountOfObjects; j++)
						{
							DistanceMatrix[i*AlgorithmParameters->CountOfObjects + j] = DistanceCalculate(
								&VectorsForClustering[i*AlgorithmParameters->CountOfDimensions],
								&VectorsForClustering[j*AlgorithmParameters->CountOfDimensions],
								AlgorithmParameters->CountOfDimensions,
								2
							);
						}
					}
				}

				double* GetResult() const
				{
					return ResultMatrix;
				}


				virtual ~FuzzyCMeans()
				{
					/*freeAlign(ResultMatrix);
					freeAlign(_powMatrix);
					freeAlign(Centroids);*/
				}
			};
		}
	}
}
