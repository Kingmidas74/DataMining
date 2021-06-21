#pragma once

namespace DataMining {
	namespace ClusteringAlgorithms {
		namespace KCollection {


			using namespace std;
			using namespace DataMining::Metrics;
			using namespace DataMining::Normalization;
			using namespace DataMining::IO;

			template<typename IncomingType>
			class KMeans: public Clustering<IncomingType, size_t>
			{

			protected:


				virtual void GenerateDefaultResultMatrix()
				{
					size_t length = this->AlgorithmParameters.CountOfObjects;
					for (size_t i = 0; i < length; i++)
					{
							this->ResultMatrix[i] = GetRandom<size_t>(0,this->AlgorithmParameters.CountOfClusters-1);
					}
				}

				virtual void GenerateCentroids()
				{
					size_t length = this->AlgorithmParameters.CountOfClusters * this->AlgorithmParameters.CountOfDimensions;
					this->Centroids =allocateAlign<IncomingType>(length);
					for(size_t c =0; c< this->AlgorithmParameters.CountOfClusters; c++)
					{
						size_t fromIdx = GetRandom<size_t>(0,c) * this->AlgorithmParameters.CountOfDimensions;
						size_t toIdx = c*this->AlgorithmParameters.CountOfDimensions;
						for(size_t d=0; d< this->AlgorithmParameters.CountOfDimensions;d++)
						{
							this->Centroids[toIdx+d] = this->VectorsForClustering[fromIdx+d];
						}
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

					#pragma omp parallel for schedule(static,8)
					for (size_t i = 0; i < CountOfObjects;i++) {
						double minDistanse = this->Metric->CalculateDistance(
								&(this->VectorsForClustering[i*CountOfDimensions]),
								&centroids[0],
								CountOfDimensions);
						size_t predictedCluster = 0;
						for (size_t j = 1; j < CountOfClusters; j++) {
							double distance = this->Metric->CalculateDistance(
								&(this->VectorsForClustering[i*CountOfDimensions]),
								&centroids[j*CountOfDimensions],
								CountOfDimensions);
							if(distance<minDistanse)
							{
								predictedCluster = j;
								minDistanse = distance;
							}
						}
						this->ResultMatrix[i] = predictedCluster;
					}
				}

				virtual void CalculateCentroids(IncomingType* centroids)
				{
					auto CountOfObjects = this->AlgorithmParameters.CountOfObjects;
					auto CountOfDimensions = this->AlgorithmParameters.CountOfDimensions;
					auto CountOfClusters = this->AlgorithmParameters.CountOfClusters;

					#pragma omp parallel for collapse(2)
					for (size_t i = 0; i < CountOfClusters; i++) {
						for (size_t d = 0; d < CountOfDimensions; d++) {
							double numenator = 0.0;
							double denomenator = 0.0;
							#pragma omp parallel for simd schedule(static,8) reduction(+:numenator,denomenator)
							for (size_t j = 0; j < CountOfObjects; j++) {
								if(this->ResultMatrix[j]==i)
								{
									numenator += this->VectorsForClustering[j*CountOfDimensions + d];
									denomenator += 1;
								}
							}
							centroids[i*CountOfDimensions + d] = denomenator!=0 ? numenator / denomenator : 0;
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

				KMeans(Parameters* algorithm_parameters, DistanceMetric* metric, NormalizationBase<IncomingType> * normalization):Clustering<IncomingType,size_t>(algorithm_parameters, metric,normalization)
				{
					this->ResultMatrix = allocateAlign<size_t>(this->AlgorithmParameters.CountOfObjects*this->AlgorithmParameters.CountOfClusters);
				}

				bool Guard()
				{
					return this->AlgorithmParameters.CountOfClusters<=this->AlgorithmParameters.CountOfDimensions;
				}

				void StartClustering(IncomingType* vectors, double * distanceMatrix) override
				{
					this->VectorsForClustering = vectors;
					this->DistanceMatrix = distanceMatrix;

					GenerateDefaultResultMatrix();

					GenerateCentroids();
					IncomingType* centroids = allocateAlign<IncomingType>(this->AlgorithmParameters.CountOfClusters*this->AlgorithmParameters.CountOfDimensions);
					copyArray<IncomingType>(this->Centroids, centroids, this->AlgorithmParameters.CountOfDimensions*this->AlgorithmParameters.CountOfClusters);
					ExecuteClustering(centroids);
					freeAlign(centroids);
				}

				void StartClustering(IncomingType* vectors) override
				{
					this->VectorsForClustering = vectors;

					GenerateDefaultResultMatrix();
					GenerateCentroids();
					IncomingType* centroids = allocateAlign<IncomingType>(this->AlgorithmParameters.CountOfClusters*this->AlgorithmParameters.CountOfDimensions);
					copyArray<IncomingType>(this->Centroids, centroids, this->AlgorithmParameters.CountOfDimensions*this->AlgorithmParameters.CountOfClusters);
					ExecuteClustering(centroids);
					freeAlign(centroids);
				}

				bool Verification() override
				{
					return true;
				}

				virtual ~KMeans()
				{
					freeAlign<size_t>(this->ResultMatrix);
				}
			};
		}
	}
}
