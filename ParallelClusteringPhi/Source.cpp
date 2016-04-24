#include <vector>
#include <functional>
#include <time.h>
#include "FuzzyCMeans.h"
#include "CustomObject.h"
#include "Helper.h"

using namespace std;
using namespace ParallelClustering;
using namespace Metrics;

void main(int argc, char *argv[])
{
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