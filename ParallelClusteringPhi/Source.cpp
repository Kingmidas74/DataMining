#include <vector>
#include <functional>
#include <time.h>
#include <sstream>
#include "FuzzyCMeans.h"
#include "Helper.h"
#include "ClusterParameters.h"


using namespace std;
using namespace ParallelClustering;
using namespace ParallelClustering::FuzzyCMeansCollection;
using namespace ParallelClustering::Metrics;

int Rank;
int Size;
Parameters ParametersForClustering;

void MainProcess() {

	vector<vector<double>> data;
	vector<vector<double>> centroids;	
	data = GetRandomObjects(ParametersForClustering.CountOfObjects, ParametersForClustering.CountOfDimensions);
	centroids = GetRandomObjects(ParametersForClustering.CountOfClusters, ParametersForClustering.CountOfDimensions);	
	PrintObjects(centroids);
	PrintObjects(data);
	FuzzyCMeans* cmeans = new FuzzyCMeans(data, ParametersForClustering.Epsilon, ParametersForClustering.Fuzzy, GetMetrics(MetricsDistanceTypes::Evklid));
	cmeans->StartClustering(centroids);
	PrintObjects(cmeans->Centroids);
	PrintObjects(cmeans->VectorsOfProbabilities);
}

void SlaveProcess() {
	
}


int main(int argc, char *argv[])
{
	srand(time(NULL));
	vector<function<void()>> actions = { MainProcess, SlaveProcess };	
	ClusterParameters clusterParameters(argc, argv);
	clusterParameters.Parse();
	ParametersForClustering = clusterParameters.GetParameters();
	actions[0]();
	return EXIT_SUCCESS;
}