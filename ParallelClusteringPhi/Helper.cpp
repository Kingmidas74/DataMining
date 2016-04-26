/*
"Реализация параллельных версиий алгоритов кластеризации данных
с использованием многоядерных ускорителей"

Вспомогательные функции.
Авторы: Сулейманов Д.Э., Цымблер М.Л.
*/

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
			cout << i << ": ";
			for (int j = 0; j < objects[i]->Data.size(); j++) {
				cout << objects[i]->Data[j] << " ";
			}
			double sum = 0;
			for (int j = 0; j < objects[i]->ProbabilityClusters.size(); j++) {
				cout << objects[i]->ProbabilityClusters[j] << " ";
				sum += objects[i]->ProbabilityClusters[j];
			}
			cout <<sum<< endl;
		}
	}

}
