#pragma once

namespace ParallelClustering {
	namespace ClusteringAlgorithms {
		namespace CCollection {


			using namespace std;
			using namespace ParallelClustering::Metrics;
			using namespace ParallelClustering::IO;

			class FuzzyCMeans: public Clustering
			{

			private:

				double* _powMatrix;

				void GenerateDefaultResultMatrix()
				{
					for (int i = 0; i < AlgorithmParameters->CountOfObjects;i++)
					{
						for (int j = 0; j < AlgorithmParameters->CountOfClusters;j++)
						{
							ResultMatrix[i*AlgorithmParameters->CountOfClusters + j] = GetRandomDouble();
						}
						normalizeArray(&ResultMatrix[i*AlgorithmParameters->CountOfClusters], AlgorithmParameters->CountOfClusters);
					}
				}

				void ExecuteClustering(double* centroids)
				{
					bool decision = false;
					while (decision == false)
					{
						cout << "calcCent" << endl;
						CalculateCentroids(centroids);
						PrintAsMatrix(ResultMatrix, AlgorithmParameters->CountOfClusters, AlgorithmParameters->CountOfClusters*AlgorithmParameters->CountOfObjects);
						cout << "setCl" << endl;
						SetClusters(centroids);
						PrintAsMatrix(ResultMatrix, AlgorithmParameters->CountOfClusters, AlgorithmParameters->CountOfClusters*AlgorithmParameters->CountOfObjects);
						cout << "calcDec" << endl;
						decision = calculateDecision(centroids);
						PrintAsMatrix(ResultMatrix, AlgorithmParameters->CountOfClusters, AlgorithmParameters->CountOfClusters*AlgorithmParameters->CountOfObjects);
						cout << "copyCent" << endl;
						copyArray(centroids, Centroids, AlgorithmParameters->CountOfDimensions*AlgorithmParameters->CountOfClusters);
						PrintAsMatrix(ResultMatrix, AlgorithmParameters->CountOfClusters, AlgorithmParameters->CountOfClusters*AlgorithmParameters->CountOfObjects);						
					}
				}

				void SetClusters(double* centroids)
				{
					for (int i = 0; i < AlgorithmParameters->CountOfObjects;i++) {
						for (int j = 0; j < AlgorithmParameters->CountOfClusters; j++) {
							double distance = Metric->CalculateDistance(
								&VectorsForClustering[i*AlgorithmParameters->CountOfDimensions],
								&centroids[j*AlgorithmParameters->CountOfDimensions],
								AlgorithmParameters->CountOfDimensions);
							ResultMatrix[i*AlgorithmParameters->CountOfClusters + j] = pow(1.0 / distance, 2.0 / (AlgorithmParameters->Fuzzy - 1.0));
						}
						normalizeArray(&ResultMatrix[i*AlgorithmParameters->CountOfClusters], AlgorithmParameters->CountOfClusters);
					}

				}

				void CalculateCentroids(double* centroids)
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

				bool calculateDecision(double* centroids)
				{
					for (int j = 0; j < AlgorithmParameters->CountOfClusters; j++)
					{
						if (Metric->CalculateDistance(Centroids, centroids, AlgorithmParameters->CountOfDimensions)>AlgorithmParameters->Epsilon)
						{
							return false;
						}
					}
					return true;
				}

			public:

				FuzzyCMeans(Parameters* algorithm_parameters, DistanceMetric* metric, FileIO fileIO):Clustering(algorithm_parameters, metric,fileIO)
				{
					ResultMatrix = allocateAlign<double>(AlgorithmParameters->CountOfObjects*AlgorithmParameters->CountOfClusters);
					_powMatrix = allocateAlign<double>(AlgorithmParameters->CountOfObjects*AlgorithmParameters->CountOfClusters);
				}

				void StartClustering() override
				{
					GenerateCentroids();
					double* centroids = allocateAlign<double>(AlgorithmParameters->CountOfClusters*AlgorithmParameters->CountOfDimensions);

					copyArray(Centroids, centroids, AlgorithmParameters->CountOfDimensions*AlgorithmParameters->CountOfClusters);
					PrintAsMatrix(Centroids, AlgorithmParameters->CountOfDimensions, AlgorithmParameters->CountOfDimensions*AlgorithmParameters->CountOfClusters);
					PrintAsMatrix(centroids, AlgorithmParameters->CountOfDimensions, AlgorithmParameters->CountOfDimensions*AlgorithmParameters->CountOfClusters);
					GenerateDefaultResultMatrix();
					PrintAsMatrix(ResultMatrix, AlgorithmParameters->CountOfClusters, AlgorithmParameters->CountOfClusters*AlgorithmParameters->CountOfObjects);
					ExecuteClustering(centroids);
					PrintAsMatrix(ResultMatrix, AlgorithmParameters->CountOfClusters, AlgorithmParameters->CountOfClusters*AlgorithmParameters->CountOfObjects);
				}

				void StartClustering(double* centroids) override
				{
					copyArray(centroids, Centroids, AlgorithmParameters->CountOfDimensions*AlgorithmParameters->CountOfClusters);

					GenerateDefaultResultMatrix();

					ExecuteClustering(centroids);
				}

				bool TrySaveData() override
				{
					return fileIO.tryWriteMatrixToFile(AlgorithmParameters->OutputFilePath, AlgorithmParameters->CountOfObjects, AlgorithmParameters->CountOfClusters, ResultMatrix);
				}

				bool TryGetData() override
				{
					return fileIO.tryReadMatrixFromFile(AlgorithmParameters->InputFilePath, AlgorithmParameters->CountOfObjects, AlgorithmParameters->CountOfDimensions, VectorsForClustering);
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
