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

	void PrintObjects(vector<vector<double>> objects) {
		int size = objects.size();
		cout<<endl<<"VectorSize is "<<size<<endl;
		for (int i = 0; i < size; i++) {
			cout << i << ": ";
			for (int j = 0; j < objects[i].size(); j++) {
				cout << objects[i][j] << " ";
			}
			cout << endl;
		}
	}

	vector<vector<double>> GetRandomObjects(int count, int dimension)
	{
		vector<vector<double>> result(count);
		for (int i = 0; i < count; i++) {
			vector<double> data(dimension);
			for (int j = 0; j < dimension; j++) {
				data[j] = GetRandomDouble();
			}
			result[i]=data;
		}
		return result;
	}

	vector<vector<double>> GetTest3DBatterfly() {
		return vector<vector<double>> 
		{
			{ 0, 1, 0 }, { 0,5,0 }, { 3,2,0 },
			{ 3,4,0 },  {5,3,0}, {7,2,0},
			{7,4,0}, {10,1,0}, {10,5,0},
			{5,2,2}, {5,4,2}, {5,1,5},
			{5,5,5}
		};
	}

}
