#pragma once

/*
"Реализация параллельных версиий алгоритов кластеризации данных
с использованием многоядерных ускорителей"

Класс для инкапсуляции работы с кластеризацией данных.
Авторы: Сулейманов Д.Э., Цымблер М.Л.
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
