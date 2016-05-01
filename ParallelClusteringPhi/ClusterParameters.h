#pragma once

/*
"Реализация параллельных версиий алгоритов кластеризации данных
с использованием многоядерных ускорителей"

Класс для определения входных данных.
Авторы: Сулейманов Д.Э., Цымблер М.Л.
*/

#include <functional>
#include <fstream>
#include <sstream>
#include "Helper.h"


namespace ParallelClustering
{

	using namespace std;

	class ClusterParameters
	{
	public:	

		ClusterParameters(int argc, char* argv[]);
		~ClusterParameters();
		Parameters GetParameters();

	private:

		int _argc;
		vector<string> _argv;
		Parameters allParameters;


		void parse();
		void getInputFilPath(int numberOfparameter);
		void getOutputFilPath(int numberOfparameter);
		void getCountOfObjects(int numberOfparameter);
		void getCountOfClusters(int numberOfparameter);
		void getCountOfDimensions(int numberOfparameter);
		void getFuzzy(int numberOfparameter);
		void getEpsilon(int numberOfparameter);
	};
}

