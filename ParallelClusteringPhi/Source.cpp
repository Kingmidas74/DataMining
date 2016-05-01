#include "ParallelClustering.h"

using namespace std;


void WriteLog(Executor* executor)
{
	fstream file;
	file.open("log.csv", ios::out | ios::app);
	file <<
		executor->DateTimeNow << ";" <<
		executor->AlgorithmParameters.CountOfObjects << ";" <<
		executor->AlgorithmParameters.CountOfDimensions << ";" <<
		executor->AlgorithmParameters.CountOfClusters << ";" <<
		executor->AlgorithmParameters.Fuzzy << ";" <<
		executor->AlgorithmParameters.Epsilon << ";" <<		
		executor->Runtime << endl;
}

int main(int argc, char *argv[])
{	
	ClusterParameters clusterParameters(argc, argv);
	Executor* executor = new Executor(clusterParameters.GetParameters());
	executor->CalculateProbabilities();	
	WriteLog(executor);
	return EXIT_SUCCESS;
}