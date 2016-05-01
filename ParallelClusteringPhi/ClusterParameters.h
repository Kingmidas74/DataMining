#pragma once

#include <functional>
#include <map>
#include <fstream>
#include <sstream>
#include "Helper.h"


namespace ParallelClustering
{

	using namespace std;

	class ClusterParameters
	{
	public:	

		ClusterParameters(int argc, char* argv[]);
		~ClusterParameters();

		void Parse();
		Parameters GetParameters();

	private:

		map<string, function<void(int)>> _handlers;
		int _argc;
		vector<char*> _argv;
		Parameters allParameters;

		void getInputFilPath(int numberOfparameter);
		void getOutputFilPath(int numberOfparameter);
		void getCountOfObjects(int numberOfparameter);
		void getCountOfClusters(int numberOfparameter);
		void getCountOfDimensions(int numberOfparameter);
		void getFuzzy(int numberOfparameter);
		void getEpsilon(int numberOfparameter);
	};
}

