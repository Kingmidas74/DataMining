#pragma once

#include <vector>
#include <iostream>
#include "CustomObject.h"

namespace ParallelClustering {
	double GetRandomDouble(double min = 0, double max = 1);
	void PrintObjects(vector<CustomObject*> objects);
}
