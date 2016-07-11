/*
"Реализация параллельных версиий алгоритов кластеризации данных
с использованием многоядерных ускорителей"

Вспомогательные функции.
Авторы: Сулейманов Д.Э., Цымблер М.Л.
*/

#include "Helper.h"

namespace ParallelClustering {

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
		srand(time(NULL));
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
	
	void GetRandomObjectsArray(int count, int dimension,double* my_array)
	{
		srand(time(NULL));
		for (int i = 0; i < count; i++) {
			for (int j = 0; j < dimension; j++) {
				my_array[i*dimension+j] = GetRandomDouble();
			}
		}
	}

	void printArray(double* array, int width, int height, char* text) {
		cout << endl;
		cout << text << endl;
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				cout << array[i*width + j] << " ";
			}
			cout << endl;
		}
		cout << endl;
	}


}
