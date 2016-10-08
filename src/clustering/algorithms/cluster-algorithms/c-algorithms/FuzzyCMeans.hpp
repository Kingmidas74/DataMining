#pragma once

namespace ParallelClustering {
	namespace ClusteringAlgorithms {
		namespace CMeansCollection {


			using namespace std;

			class FuzzyCMeans : public Clustering
			{

			protected:

				double* _powMatrix;

				virtual void GenerateDefaultResultMatrix()
				{
					for (unsigned int i = 0; i < AlgorithmParameters->CountOfObjects;i++)
					{
						for (unsigned int j = 0; j < AlgorithmParameters->CountOfClusters;j++)
						{
							ResultMatrix[i*AlgorithmParameters->CountOfClusters + j] = GetRandomDouble();
						}
						normalizeArray(&ResultMatrix[i*AlgorithmParameters->CountOfClusters], AlgorithmParameters->CountOfClusters);
					}
					cout << "DRF" << endl;
					PrintAsMatrix(ResultMatrix, AlgorithmParameters->CountOfClusters, AlgorithmParameters->CountOfClusters*AlgorithmParameters->CountOfObjects);
				}

				virtual bool calculateDecision(double* centroids)
				{
					double sum = 0;
					for (unsigned int i = 0; i < AlgorithmParameters->CountOfClusters; i++)
					{
						sum += DistanceCalculate(Centroids, centroids, AlgorithmParameters->CountOfDimensions,2, AlgorithmParameters->CountOfDimensions);
					}
					return sum <= AlgorithmParameters->Epsilon;
				}

				void ExecuteClustering(double* centroids) override
				{
					bool decision = false;
					
					while (decision == false)
					{
						SetClusters(centroids);
						CalculateCentroids(centroids);
						decision = calculateDecision(centroids);
						cout << "CENTROIDS OLD:" << endl;
						PrintAsMatrix(Centroids, AlgorithmParameters->CountOfDimensions, AlgorithmParameters->CountOfDimensions*AlgorithmParameters->CountOfClusters);
						cout << "CENTROIDS NEW:" << endl;
						PrintAsMatrix(centroids, AlgorithmParameters->CountOfDimensions, AlgorithmParameters->CountOfDimensions*AlgorithmParameters->CountOfClusters);
						copy(centroids,
							centroids + sizeof(double)*AlgorithmParameters->CountOfClusters*AlgorithmParameters->CountOfDimensions,
							Centroids);
					}
				}

				virtual void SetClusters(double* centroids)
				{
					for (unsigned int i = 0; i < AlgorithmParameters->CountOfObjects;i++) {
						for (unsigned int j = 0; j < AlgorithmParameters->CountOfClusters; j++) {
							double distance = DistanceCalculate(
								&VectorsForClustering[i*AlgorithmParameters->CountOfDimensions], 
								&centroids[j*AlgorithmParameters->CountOfDimensions], 
								AlgorithmParameters->CountOfDimensions,2, 
								AlgorithmParameters->CountOfDimensions);
							ResultMatrix[i*AlgorithmParameters->CountOfClusters + j] = pow(1.0 / distance, 2.0 / (AlgorithmParameters->Fuzzy - 1.0));
						}
						normalizeArray(&ResultMatrix[i*AlgorithmParameters->CountOfClusters], AlgorithmParameters->CountOfClusters);
					}

				}

				virtual void CalculateCentroids(double* centroids)
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



			public:


				FuzzyCMeans(double* data, Parameters* algorithm_parameters, function<double(double*, double*, unsigned int, unsigned int, unsigned int)> distance)
					: Clustering(data, algorithm_parameters, distance)
				{
					Centroids = allocateAlign<double>(AlgorithmParameters->CountOfClusters * AlgorithmParameters->CountOfDimensions);
					ResultMatrix = allocateAlign<double>(AlgorithmParameters->CountOfObjects*AlgorithmParameters->CountOfClusters);
					_powMatrix = allocateAlign<double>(AlgorithmParameters->CountOfObjects*AlgorithmParameters->CountOfClusters);
				}

				void StartClustering() override
				{
					GenerateCentroids();
					double* centroids = allocateAlign<double>(AlgorithmParameters->CountOfClusters*AlgorithmParameters->CountOfDimensions);

					copy(Centroids,
						Centroids + sizeof(double)*AlgorithmParameters->CountOfClusters*AlgorithmParameters->CountOfDimensions,
						centroids);
					GenerateDefaultResultMatrix();

					ExecuteClustering(centroids);

					//freeAlign<double>(centroids);

					PrintAsMatrix(ResultMatrix, AlgorithmParameters->CountOfClusters, AlgorithmParameters->CountOfClusters*AlgorithmParameters->CountOfObjects);
				}

				void StartClustering(double* centroids) override
				{
					copy(centroids,
						centroids + sizeof(double)*AlgorithmParameters->CountOfClusters*AlgorithmParameters->CountOfDimensions,
						Centroids);

					GenerateDefaultResultMatrix();

					ExecuteClustering(centroids);
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
