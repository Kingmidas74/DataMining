#pragma once

/*
"���������� ������������ ������� ��������� ������������� ������
� �������������� ������������ �����������"

����� ��� ������������ ������ � �������������� ������.
������: ���������� �.�., ������� �.�.
*/

#include <vector>
#include "Helper.h"
#include "FuzzyCMeans.h"


namespace ParallelClustering
{
	using namespace std;
	using namespace ParallelClustering;
	using namespace ParallelClustering::FuzzyCMeansCollection;
	using namespace ParallelClustering::Metrics;


	class Executor
	{
	public:
		Executor(Parameters algorithmParameters);
		~Executor();
		vector<vector<double>> CalculateProbabilities();

	private:
		vector<vector<double>> _normalizeObjects;
		Parameters _algorithmParameters;
	};

	
}