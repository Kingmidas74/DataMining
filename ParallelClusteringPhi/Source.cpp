#include "ParallelClustering.h"

using namespace std;

int main(int argc, char *argv[])
{	
	srand(time(NULL));
	ClusterParameters clusterParameters(argc, argv);
	Executor<double,double>* executor = new Executor<double,double>(&clusterParameters.GetParameters());
	executor->CalculateProbabilities();
	return EXIT_SUCCESS;
}