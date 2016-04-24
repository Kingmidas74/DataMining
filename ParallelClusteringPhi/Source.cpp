#include <vector>
#include <functional>
#include <time.h>
#include <omp.h>
#include <mpi.h>
#include <sstream>
#include "FuzzyCMeans.h"
#include "CustomObject.h"
#include "Helper.h"


using namespace std;
using namespace ParallelClustering;
using namespace Metrics;

int Rank;
int Size;
int CountOfObject = 10;
int CountOfDimension = 2;

void MainProcess() {
	vector<CustomObject*> data = CustomObject::GetRandomObjects(CountOfObject, CountOfDimension);
	PrintObjects(data);
	vector<CustomObject*> centroids = CustomObject::GetRandomObjects(7, CountOfDimension);
	PrintObjects(centroids);
	FuzzyCMeans* cmeans = new FuzzyCMeans(data, 0.1, 1.5, GetMetrics(MetricsDistanceTypes::Evklid));
	cmeans->StartClustering(centroids);
	PrintObjects(cmeans->ObjectsForClustering);
}

void SlaveProcess() {
	
}


void main(int argc, char *argv[])
{
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &Size);
	MPI_Comm_rank(MPI_COMM_WORLD, &Rank);
	srand(time(NULL));
	vector<function<void()>> actions = { MainProcess, SlaveProcess };
	
	if (argc >= 3)
	{
		int val;
		std::istringstream iss1(argv[1]);		
		if (iss1 >> val)
		{
			CountOfObject = val;
		}

		std::istringstream iss2(argv[2]);
		if (iss2 >> val)
		{
			CountOfDimension = val;
		}
	}

	actions[Rank]();
	MPI_Finalize();
}