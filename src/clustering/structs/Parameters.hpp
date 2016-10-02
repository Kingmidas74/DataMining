#pragma once

/**
Parameters.hpp
Purpose: Struct for save parameters.

@author		Suleymanov D.
@version	0.0.0.1 9/17/2016
*/


namespace ParallelClustering 
{
	using namespace std;

	struct Parameters
	{
		int CountOfObjects;
		int CountOfDimensions;
		int CountOfClusters;
		int CountOfThreads;
		double Fuzzy;
		double Epsilon;
		string InputFilePath;
		string OutputFilePath;
	};
}