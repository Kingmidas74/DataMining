#pragma once

/*
"���������� ������������ ������� ��������� ������������� ������
� �������������� ������������ �����������"

����� ��� ������������ ������ � �������������� ������.
������: ���������� �.�., ������� �.�.
*/

#include <vector>
#include <fstream>
#include <sstream>
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
		double Runtime;

	private:
		vector<vector<double>> _normalizeObjects;
		Parameters _algorithmParameters;

		bool tryReadFile(vector<vector<double>> &data);
		void tryWriteFile(vector<vector<double>> &data);
	};

	
}