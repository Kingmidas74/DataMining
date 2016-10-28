#pragma once

namespace ParallelClustering {
	namespace ClusteringAlgorithms {
		namespace CCollection {


			using namespace std;
			using namespace ParallelClustering::Metrics;
			using namespace ParallelClustering::IO;

			class FuzzyCMeansOpenMP : public FuzzyCMeans
			{

			private:

				void GenerateDefaultResultMatrix() override
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

				void GenerateCentroids() override
				{
					for (int i = 0; i < AlgorithmParameters->CountOfClusters; i++)
					{
						for (int j = 0; j < AlgorithmParameters->CountOfDimensions; j++)
						{
							Centroids[i*AlgorithmParameters->CountOfDimensions + j] = GetRandomDouble();
						}
					}
				}

				void ExecuteClustering(double* centroids) override
				{
					bool decision = false;
					while (decision == false)
					{
						//cout << "calcCentOpenMP" << endl;
						CalculateCentroids(centroids);
						//PrintAsMatrix(ResultMatrix, AlgorithmParameters->CountOfClusters, AlgorithmParameters->CountOfClusters*AlgorithmParameters->CountOfObjects);
						//cout << "setClOpenMP" << endl;
						SetClusters(centroids);
						//PrintAsMatrix(ResultMatrix, AlgorithmParameters->CountOfClusters, AlgorithmParameters->CountOfClusters*AlgorithmParameters->CountOfObjects);
						//cout << "calcDecOpenMP" << endl;
						decision = calculateDecision(centroids);
						//PrintAsMatrix(ResultMatrix, AlgorithmParameters->CountOfClusters, AlgorithmParameters->CountOfClusters*AlgorithmParameters->CountOfObjects);
						//cout << "copyCentOpenMP" << endl;
						copyArray(centroids, Centroids, AlgorithmParameters->CountOfDimensions*AlgorithmParameters->CountOfClusters);
						//PrintAsMatrix(ResultMatrix, AlgorithmParameters->CountOfClusters, AlgorithmParameters->CountOfClusters*AlgorithmParameters->CountOfObjects);
					}
				}

				void SetClusters(double* centroids) override
				{
					#pragma omp parallel for
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

				void CalculateCentroids(double* centroids) override
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

				bool calculateDecision(double* centroids) override
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

				inline void normalizeArray(double* row, int length) override
				{
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

			public:

				FuzzyCMeansOpenMP(Parameters* algorithm_parameters, DistanceMetric* metric, FileIO fileIO) :
					FuzzyCMeans(algorithm_parameters, metric, fileIO)
				{}

				virtual ~FuzzyCMeansOpenMP()
				{
					/*freeAlign(ResultMatrix);
					freeAlign(_powMatrix);
					freeAlign(Centroids);*/
				}
			};
		}
	}
}
