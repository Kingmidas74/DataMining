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
#include <ctime>
#include "Helper.h"
#include "FuzzyCMeans.h"
#include "FuzzyCMeansOpenMP.h"


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
		string DateTimeNow;
		Parameters AlgorithmParameters;

	private:
		vector<vector<double>> data;		

		void setDateTime();
		bool tryReadFile(vector<vector<double>> &data);
		void tryWriteFile(vector<vector<double>> &data);
	};

	
}