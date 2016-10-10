#include <iostream>

#include "./clustering/ParallelClustering.hpp"


#include <ctime>

int main(int argc, char *argv[])
{	
	srand(time(nullptr));
	
	ClusterParameters clusterParameters(argc, argv);
	auto parameters = clusterParameters.GetParameters();
	auto executor = Executor(&parameters);
	executor.CalculateProbabilities();
	
	return EXIT_SUCCESS;
}
