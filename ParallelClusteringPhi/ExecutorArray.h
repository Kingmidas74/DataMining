#pragma once

/*
"���������� ������������ ������� ��������� ������������� ������
� �������������� ������������ �����������"

����� ��� ������������ ������ � �������������� ������.
������: ���������� �.�., ������� �.�.
*/

#include <fstream>
#include <sstream>
#include <ctime>
#include <omp.h>
#include "Helper.h"
#include "FuzzyCMeansArrayOpenMP.h"


namespace ParallelClustering
{
	using namespace std;
	using namespace ParallelClustering;
	using namespace ParallelClustering::FuzzyCMeansCollection;
	using namespace ParallelClustering::Metrics;


	class ExecutorArray
	{
	public:
		ExecutorArray(Parameters algorithmParameters);
		~ExecutorArray();
		double* CalculateProbabilities();
		double Runtime;
		string DateTimeNow;
		Parameters AlgorithmParameters;

	private:
		double data;		

		void WriteLog(int n);
		void setDateTime();
		bool tryReadFile(double* data, int n);
		void tryWriteFile(double* data);
	};

	
}
