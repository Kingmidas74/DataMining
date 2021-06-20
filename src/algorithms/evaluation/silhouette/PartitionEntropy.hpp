#pragma once

namespace DataMining {
	namespace EvaluationAlgorithms {


		using namespace std;


		class PartitionEntropy
		{
            private:
            Parameters parameters;
            double * clusteringResults;


            public:

                double EvaluationRate;

                PartitionEntropy(Parameters * algorithm_parameters,  double * clustering_results) {
                    parameters=algorithm_parameters[0];

                    clusteringResults = clustering_results;

                    EvaluationRate=0;
                }

                ~PartitionEntropy()
                {
                }

                void Evaluate()
                {
                    auto result = 0.0;

                    for(size_t i=0;i<parameters.CountOfObjects*parameters.CountOfClusters;i++)
                    {
                        result+=clusteringResults[i]*log2(clusteringResults[i]);
                    }
                    EvaluationRate=(-1)*result/parameters.CountOfObjects;
                }
        };
    }
}