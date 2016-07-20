#include "ParallelClustering.hpp"

using namespace std;

int main(int argc, char *argv[])
{	
	srand(time(NULL));
	ClusterParameters clusterParameters(argc, argv);
	Parameters parameters = clusterParameters.GetParameters();
	Executor<double,double>* executor = new Executor<double,double>(&parameters);
	executor->CalculateProbabilities();
	return EXIT_SUCCESS;
}