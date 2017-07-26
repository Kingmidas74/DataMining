#pragma once

namespace DataMining {
	namespace EvaluationAlgorithms {


		using namespace std;
		
		
		class Silhouette
		{
            private:
            Parameters parameters;
            int* clusteringResults;
            int* nearestClusters;
            double* distanceMatrix;

            #pragma omp declare simd
            double avgInCluster(int indexofPoint, int clusterNumber)
            {
                double result = 0.0;
                size_t count = 0;
                size_t N=parameters.CountOfObjects;
                
                for (size_t i = 0; i<N;i++)
                {
                    if(clusteringResults[i] == clusterNumber) 
                    {
                        result += distanceMatrix[indexofPoint*N + i];
                        count++;
                    }
                }
                if(clusterNumber==clusteringResults[indexofPoint])
                {
                    count--;
                }
                return (result/count);
            }

            void calculateNearestClusters()
            {
                size_t N = parameters.CountOfObjects;
                for(size_t i=0;i<N;i++)
                {
                    nearestClusters[i] = clusteringResults[i];
                    double distance = std::numeric_limits<double>::max();
                    
                    for (size_t j = 0;j < N;j++)
                    {
                        if (clusteringResults[j] != clusteringResults[i] && distanceMatrix[i*parameters.CountOfObjects + j] < distance)
                        {
                                distance = distanceMatrix[i*parameters.CountOfObjects + j];
                                nearestClusters[i] = clusteringResults[j];
                        }
                    }
                }
            }

            public:

                double EvaluationRate;

                Silhouette(Parameters * algorithm_parameters, double * distance_matrix, int * clustering_results) {
                    parameters=algorithm_parameters[0];
                    distanceMatrix = distance_matrix;
                    clusteringResults = clustering_results;

                    EvaluationRate=0;

                    nearestClusters=allocateAlign<int>(parameters.CountOfClusters);                    
                }

                ~Silhouette()
                {
                    freeAlign<int>(nearestClusters);
                }

                void Evaluate()
                {
                    calculateNearestClusters();     
                    auto result = 0.0;
                    for (size_t i = 0; i<parameters.CountOfObjects;i++)
                    {
                        double a = avgInCluster(i, clusteringResults[i]);
                        double b = avgInCluster(i, nearestClusters[i]);
                        double s = (b - a) / max(a, b);
                        result += s;
                    }
                    result/= parameters.CountOfObjects; 
                    EvaluationRate=result;             
                }
        };
    }
}