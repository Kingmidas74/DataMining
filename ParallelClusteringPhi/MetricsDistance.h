#pragma once

/*
"���������� ������������ ������� ��������� ������������� ������
� �������������� ������������ �����������"

������������ ��� ���������� � ���� ��������� ������� ��� ���������� ����������
����� ����� ���������.
������: ���������� �.�., ������� �.�.
*/

#include <vector>
#include <functional>
#include <cmath>

using namespace std;

namespace Metrics {
	
	enum class MetricsDistanceTypes
	{
		Evklid,
		Custom
	};
	function<double(vector<double>, vector<double>)> GetMetrics(MetricsDistanceTypes type);
	double EvklidDistance(vector<double> first_data, vector<double> second_data);
	double CustomDistance(vector<double> first_data, vector<double> second_data);
}
