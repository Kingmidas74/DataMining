#include "ParallelClustering.hpp"

using namespace std;

int main(int argc, char *argv[])
{	
	srand(time(nullptr));
	ClusterParameters clusterParameters(argc, argv);
	auto parameters = clusterParameters.GetParameters();
	auto executor = new Executor<double,double>(&parameters);
	executor->CalculateProbabilities();
	delete executor;
	return EXIT_SUCCESS;
}