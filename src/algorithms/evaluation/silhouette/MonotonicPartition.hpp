#pragma once

namespace DataMining {
	namespace EvaluationAlgorithms {


		using namespace std;
		
		
		class MonotonicPartition
		{
            private:
            Parameters parameters;
            double * clusteringResults;
            
            
            public:

                double EvaluationRate;

                MonotonicPartition(Parameters * algorithm_parameters,  double * clustering_results) {
                    parameters=algorithm_parameters[0];
                    
                    clusteringResults = clustering_results;

                    EvaluationRate=0;
                }

                ~MonotonicPartition()
                {
                }

                void Evaluate()
                {                    
                    auto result = 0.0;
                    powMatrix<double>(clusteringResults,clusteringResults,2,parameters.CountOfObjects*parameters.CountOfClusters);
                    for(size_t i=0;i<parameters.CountOfObjects*parameters.CountOfClusters;i++)
                    {
                        result+=clusteringResults[i];
                    }
                    EvaluationRate=1-(result/parameters.CountOfObjects); 
                    EvaluationRate*=(parameters.CountOfClusters/(parameters.CountOfClusters-1));
                    EvaluationRate=1-EvaluationRate;

                }
        };
    }
}