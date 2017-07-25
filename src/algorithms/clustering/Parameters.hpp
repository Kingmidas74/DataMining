#pragma once

/**
Parameters.hpp
Purpose: Struct for save parameters.

@author		Suleymanov D.
@version	0.0.0.1 9/17/2016
*/

namespace DataMining
{
	using namespace std;
	using namespace Metrics;

	struct Parameters
	{
		size_t CountOfObjects;
		size_t CountOfDimensions;
		size_t CountOfClusters;
		size_t CountOfThreads;
		double Fuzzy;
		double Epsilon;
		string InputFilePath;
		string OutputFilePath;
		string LogFilePath;
		MetricTypes MetricType;
		
	};
}