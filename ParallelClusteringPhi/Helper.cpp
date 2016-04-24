#include "Helper.h"

namespace ParallelClustering {
	using namespace std;

	double GetRandomDouble(double min, double max) {
		return (static_cast <double> (rand()) / static_cast <double> (RAND_MAX));
	}

	void PrintObjects(vector<CustomObject*> objects) {
		int size = objects.size();
		cout<<endl<<"VectorSize is "<<size<<endl;
		for (int i = 0; i < size; i++) {
			cout << i<<": "
				<<objects[i]->Data[0]<<" "<<objects[i]->Data[1] << " " <<objects[i]->Data[2] << " "
				<<objects[i]->ProbabilityClusters[0] << " " << objects[i]->ProbabilityClusters[1] << " " << objects[i]->ProbabilityClusters[2] 
				<< endl;
		}
	}

}
