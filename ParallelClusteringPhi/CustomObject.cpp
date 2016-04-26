/*
"Реализация параллельных версиий алгоритов кластеризации данных
с использованием многоядерных ускорителей"

Класс для отображения объекта сериализации.
Авторы: Сулейманов Д.Э., Цымблер М.Л.
*/

#include "CustomObject.h"
#include "Helper.h";

namespace ParallelClustering {

	CustomObject::CustomObject(vector<double> rgb_data)
	{
		Data = rgb_data;
	}

	CustomObject::CustomObject()
	{

	}

	vector<CustomObject*> CustomObject::GetRandomObjects(int count, int dimension)
	{
		vector<CustomObject*> result(count);
		for (int i = 0; i < count; i++) {
			vector<double> data(dimension);
			for (int j = 0; j < dimension; j++) {
				data[j] = GetRandomDouble();				
			}
			result[i] = new CustomObject(data);
		}
		return result;
	}

	vector<CustomObject*> CustomObject::GetTest3DBatterfly() {
		vector<CustomObject*> result(13);
		vector<double> coord0 = { 0,1,0 };
		vector<double> coord1 = { 0,5,0 };
		vector<double> coord2 = { 3,2,0 };
		vector<double> coord3 = { 3,4,0 };
		vector<double> coord4 = { 5,3,0 };
		vector<double> coord5 = { 7,2,0 };
		vector<double> coord6 = { 7,4,0 };
		vector<double> coord7 = { 10,1,0 };
		vector<double> coord8 = { 10,5,0 };
		vector<double> coord9 = { 5,2,2 };
		vector<double> coord10 = { 5,4,2 };
		vector<double> coord11 = { 5,1,5 };
		vector<double> coord12 = { 5,5,5 };
		result[0] = new CustomObject(coord0);
		result[1] = new CustomObject(coord1);
		result[2] = new CustomObject(coord2);
		result[3] = new CustomObject(coord3);
		result[4] = new CustomObject(coord4);
		result[5] = new CustomObject(coord5);
		result[6] = new CustomObject(coord6);
		result[7] = new CustomObject(coord7);
		result[8] = new CustomObject(coord8);
		result[9] = new CustomObject(coord9);
		result[10] = new CustomObject(coord10);
		result[11] = new CustomObject(coord11);
		result[12] = new CustomObject(coord12);
		return result;
	}


	CustomObject::~CustomObject()
	{
	}

	void CustomObject::DefineCluster(int num_of_cluster, double probability) {
		ProbabilityClusters[num_of_cluster] = probability;
	}
}