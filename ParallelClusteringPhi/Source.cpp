#include "ParallelClustering.h"

using namespace std;

int main(int argc, char *argv[])
{
	ClusterParameters clusterParameters(argc, argv);
	Executor* executor = new Executor(clusterParameters.GetParameters());
	executor->CalculateProbabilities();
	return EXIT_SUCCESS;
}