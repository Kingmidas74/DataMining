#include <vector>
#include <functional>
#include <time.h>
#include <omp.h>
#include <mpi.h>
#include "FuzzyCMeans.h"
#include "CustomObject.h"
#include "Helper.h"


using namespace std;
using namespace ParallelClustering;
using namespace Metrics;

int Rank;
int Size;

void MainProcess() {
	vector<CustomObject*> data = CustomObject::GetTest3DBatterfly();
	PrintObjects(data);
	vector<CustomObject*> centroids(3);
	vector<double> red = { 2,3,0 };
	vector<double> green = { 8,3,0 };
	vector<double> blue = { 5,3,3 };
	centroids[0] = new CustomObject(red);
	centroids[1] = new CustomObject(green);
	centroids[2] = new CustomObject(blue);
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
	actions[Rank]();
	MPI_Finalize();
}