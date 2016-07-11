#include "ParallelClustering.h"

using namespace std;

int main(int argc, char *argv[])
{	
	ClusterParameters clusterParameters(argc, argv);
	ExecutorArray* executor = new ExecutorArray(clusterParameters.GetParameters());
	//cout<<"I am here Main";
	executor->CalculateProbabilities();	
	return EXIT_SUCCESS;
}