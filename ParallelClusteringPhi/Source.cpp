#include <vector>
#include <functional>
#include <time.h>
#include <omp.h>
#include <sstream>
#include "FuzzyCMeans.h"
#include "CustomObject.h"
#include "Helper.h"


using namespace std;
using namespace ParallelClustering;
using namespace Metrics;

int Rank;
int Size;
int CountOfObjects = 10;
int CountOfDimensions = 2;
int CountOfClusters = 2;

void MainProcess(int batterfly) {

	vector<CustomObject*> data;
	vector<CustomObject*> centroids;
	if (batterfly == 1) {
		data = CustomObject::GetTest3DBatterfly();
		centroids = CustomObject::GetRandomObjects(3, 3);
	}
	else {
		data = CustomObject::GetRandomObjects(CountOfObjects, CountOfDimensions);
		centroids = CustomObject::GetRandomObjects(CountOfClusters, CountOfDimensions);
	}
	PrintObjects(data);	 
	PrintObjects(centroids);
	FuzzyCMeans* cmeans = new FuzzyCMeans(data, 0.1, 1.5, GetMetrics(MetricsDistanceTypes::Evklid));
	cmeans->StartClustering(centroids);
	PrintObjects(cmeans->ObjectsForClustering);
}

void SlaveProcess(int butterfly) {
	
}


int main(int argc, char *argv[])
{
	vector<function<void(int)>> actions = { MainProcess, SlaveProcess };	
	if (argc >= 4)
	{
		int val;
		std::istringstream iss1(argv[1]);		
		if (iss1 >> val)
		{
			CountOfObjects = val;
		}
		std::istringstream iss2(argv[2]);
		if (iss2 >> val)
		{
			CountOfDimensions = val;
		}

		std::istringstream iss3(argv[3]);
		if (iss3 >> val)
		{
			CountOfClusters = val;
		}

		std::istringstream iss4(argv[3]);
		if (iss4 >> val)
		{
			CountOfClusters = val;
		}
		actions[0](0);
	}
	else {
		actions[0](1);
	}
	
	return 0;
}