#pragma once

/*
"���������� ������������ ������� ��������� ������������� ������
� �������������� ������������ �����������"

����� ��� ������������ ������ � �������������� ������.
������: ���������� �.�., ������� �.�.
*/

#include <vector>

namespace ParallelClustering
{
	using namespace std;
	class Executor
	{
	public:
		Executor();
		~Executor();
		vector<vector<double>> CalculateProbabilities();

	private:
		vector<vector<double>> _normalizeObjects;
	};

	
}