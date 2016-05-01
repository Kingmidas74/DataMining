#include "ParallelClustering.h"

using namespace std;


void WriteLog(Executor* executor)
{
	fstream log;
	log.open("log.csv", ios::out | ios::app);
	log <<
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