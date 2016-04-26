#pragma once

/*
"���������� ������������ ������� ��������� ������������� ������
� �������������� ������������ �����������"

��������������� �������.
������: ���������� �.�., ������� �.�.
*/

#include <vector>
#include <iostream>
#include "CustomObject.h"

namespace ParallelClustering {
	double GetRandomDouble(double min = 0, double max = 1);
	void PrintObjects(vector<vector<double>> objects);
	vector<vector<double>> GetRandomObjects(int count, int dimension);
	vector<vector<double>> GetTest3DBatterfly();
}
