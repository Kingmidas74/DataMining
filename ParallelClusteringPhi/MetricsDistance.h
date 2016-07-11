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

namespace ParallelClustering
{

	namespace Metrics {

		using namespace std;

		enum class MetricsDistanceTypes
		{
			Evklid,
			Custom
		};
		function<double(vector<double>, vector<double>)> GetMetrics(MetricsDistanceTypes type);
		double EvklidDistance(vector<double> first_data, vector<double> second_data);
		double CustomDistance(vector<double> first_data, vector<double> second_data);
		double EvklidArray(double* first_data, long long first_data_size, double* second_data, long long second_data_size);
	}
}
