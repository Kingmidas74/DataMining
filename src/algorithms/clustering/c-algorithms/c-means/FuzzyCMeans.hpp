#pragma once

namespace DataMining {
	namespace ClusteringAlgorithms {
		namespace CCollection {


			using namespace std;
			using namespace DataMining::Metrics;
			using namespace DataMining::Normalization;
			using namespace DataMining::IO;

			template<typename IncomingType, typename OutcomingType>
			class FuzzyCMeans: public Clustering<IncomingType, OutcomingType>
			{

			protected:

				double* _powMatrix;

				virtual void GenerateDefaultResultMatrix()
				{
					size_t length = this->AlgorithmParameters.CountOfObjects * this->AlgorithmParameters.CountOfDimensions;
					for (size_t i = 0; i < length; i++)
					{
							this->ResultMatrix[i] = GetRandom<OutcomingType>();
					}
					for (size_t i = 0; i < this->AlgorithmParameters.CountOfObjects;i++)
					{
						this->Normalization->Normalize(&(this->ResultMatrix[i*this->AlgorithmParameters.CountOfClusters]), this->AlgorithmParameters.CountOfClusters);
					}
				}

				virtual void GenerateCentroids()
				{
					size_t length = this->AlgorithmParameters.CountOfClusters * this->AlgorithmParameters.CountOfDimensions;
					for (size_t i = 0; i < length; i++)
					{
						this->Centroids[i] = GetRandom<IncomingType>();						
					}
				}

				virtual void ExecuteClustering(IncomingType* centroids)
				{
					bool decision = false;
					while (decision == false)
					{
						CalculateCentroids(centroids);
						SetClusters(centroids);
						decision = calculateDecision(centroids);
						copyArray<IncomingType>(centroids, this->Centroids, this->AlgorithmParameters.CountOfDimensions*this->AlgorithmParameters.CountOfClusters);
					}
				}

				virtual void SetClusters(IncomingType * const centroids)
				{
					auto CountOfObjects = this->AlgorithmParameters.CountOfObjects;
					auto CountOfDimensions = this->AlgorithmParameters.CountOfDimensions;
					auto CountOfClusters = this->AlgorithmParameters.CountOfClusters;
					auto Fuzzy = this->AlgorithmParameters.Fuzzy;
					
					#pragma omp parallel for schedule(static,8)
					for (size_t i = 0; i < CountOfObjects;i++) {
						for (size_t j = 0; j < CountOfClusters; j++) {
							double distance = this->Metric->CalculateDistance(
								&(this->VectorsForClustering[i*CountOfDimensions]),
								&centroids[j*CountOfDimensions],
								CountOfDimensions);
							this->ResultMatrix[i*CountOfClusters + j] = pow(1.0 / distance, 2.0 / (Fuzzy - 1.0));
						}
					}

					for (size_t i = 0; i < CountOfObjects;i++) {
						this->Normalization->Normalize(&(this->ResultMatrix[i*CountOfClusters]), CountOfClusters);
					}
				}

				virtual void CalculateCentroids(IncomingType* centroids)
				{
					auto CountOfObjects = this->AlgorithmParameters.CountOfObjects;
					auto CountOfDimensions = this->AlgorithmParameters.CountOfDimensions;
					auto CountOfClusters = this->AlgorithmParameters.CountOfClusters;
					
					powMatrix<OutcomingType>(this->ResultMatrix,_powMatrix, this->AlgorithmParameters.Fuzzy,
										this->AlgorithmParameters.CountOfObjects*this->AlgorithmParameters.CountOfClusters);

					
					#pragma omp parallel for collapse(2)
					for (size_t i = 0; i < CountOfClusters; i++) {
						for (size_t d = 0; d < CountOfDimensions; d++) {
							double numenator = 0.0;
							double denomenator = 0.0;
							#pragma omp parallel for simd schedule(static,8) reduction(+:numenator,denomenator)
							for (size_t j = 0; j < CountOfObjects; j++) {
								numenator += _powMatrix[j*CountOfClusters + i] * this->VectorsForClustering[j*CountOfDimensions + d];
								denomenator += _powMatrix[j*CountOfClusters + i];
							}
							centroids[i*CountOfClusters + d] = numenator / denomenator;
						}
					}
				}

				virtual bool calculateDecision(IncomingType * const centroids)
				{
					for (size_t j = 0; j < this->AlgorithmParameters.CountOfClusters; j++)
					{
						if (this->Metric->CalculateDistance(this->Centroids, centroids, this->AlgorithmParameters.CountOfDimensions)>this->AlgorithmParameters.Epsilon)
						{
							return false;
						}
					}
					return true;
				}

			public:

				FuzzyCMeans(Parameters* algorithm_parameters, DistanceMetric* metric, NormalizationBase<IncomingType> * normalization, FileIO fileIO):Clustering<IncomingType,OutcomingType>(algorithm_parameters, metric,normalization,fileIO)
				{
					this->ResultMatrix = allocateAlign<OutcomingType>(this->AlgorithmParameters.CountOfObjects*this->AlgorithmParameters.CountOfClusters);
					_powMatrix = allocateAlign<double>(this->AlgorithmParameters.CountOfObjects*this->AlgorithmParameters.CountOfClusters);
				}

				void StartClustering() override
				{
					GenerateDefaultResultMatrix();

					GenerateCentroids();
					IncomingType* centroids = allocateAlign<IncomingType>(this->AlgorithmParameters.CountOfClusters*this->AlgorithmParameters.CountOfDimensions);
					copyArray<IncomingType>(this->Centroids, centroids, this->AlgorithmParameters.CountOfDimensions*this->AlgorithmParameters.CountOfClusters);
					ExecuteClustering(centroids);
					freeAlign(centroids);
				}

				bool Verification() override
				{
					for (size_t i = 0; i < this->AlgorithmParameters.CountOfObjects; i++)
					{
						double sum=0;
						for (size_t j = 0; j < this->AlgorithmParameters.CountOfClusters; j++)
						{
							sum+=this->ResultMatrix[i*this->AlgorithmParameters.CountOfClusters+j];
						}
						if(abs(1-sum)>this->AlgorithmParameters.Epsilon) {
							return false;
						}
					}
					return true;
				}

				bool TrySaveData() override
				{
					return this->fileIO.template tryWriteMatrixToFile<OutcomingType>(this->AlgorithmParameters.OutputFilePath, this->AlgorithmParameters.CountOfObjects, this->AlgorithmParameters.CountOfClusters, this->ResultMatrix);
				}

				bool TryGetData() override
				{
					return this->fileIO.template tryReadMatrixFromFile<IncomingType>(this->AlgorithmParameters.InputFilePath, this->AlgorithmParameters.CountOfObjects, this->AlgorithmParameters.CountOfDimensions, this->VectorsForClustering);
				}

				virtual ~FuzzyCMeans()
				{
					freeAlign(_powMatrix);
				}
			};
		}
	}
}
