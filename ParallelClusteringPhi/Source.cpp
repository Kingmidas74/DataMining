#include "ParallelClustering.h"

using namespace std;

int main(int argc, char *argv[])
{
	
	ClusterParameters clusterParameters(argc, argv);
	Executor* executor = new Executor(clusterParameters.GetParameters());
	int start_time = clock();
	executor->CalculateProbabilities();
	int stop_time = clock();
	cout << (stop_time - start_time) / double(CLOCKS_PER_SEC) * 1000 << endl;
	cout << executor->Runtime;
	return EXIT_SUCCESS;
}