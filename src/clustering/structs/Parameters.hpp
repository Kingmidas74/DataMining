#pragma once

/**
Parameters.hpp
Purpose: Struct for save parameters.

@author		Suleymanov D.
@version	0.0.0.1 9/17/2016
*/

#include <iostream>

namespace ParallelClustering
{
	using namespace std;

	struct Parameters
	{
		unsigned int CountOfObjects;
		unsigned int CountOfDimensions;
		unsigned int CountOfClusters;
		unsigned int CountOfThreads;
		double Fuzzy;
		double Epsilon;
		string InputFilePath;
		string OutputFilePath;

		
	};
}