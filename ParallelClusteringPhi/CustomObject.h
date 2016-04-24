#pragma once

#include <vector>
#include <map>


namespace ParallelClustering {

	using namespace std;

	class CustomObject
	{
	public:
		CustomObject(vector<double> rgb_data);
		CustomObject();
		~CustomObject();


		void DefineCluster(int num_of_cluster, double probability);
		static vector<CustomObject*> GetRandomObjects(int count, int dimension);
		static vector<CustomObject*> GetTest3DBatterfly();

		vector<double> Data;
		map<int, double> ProbabilityClusters;
	};
}
