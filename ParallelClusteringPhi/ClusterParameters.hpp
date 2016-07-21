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
#include <vector>

#include "Helper.hpp"



namespace ParallelClustering
{

	using namespace std;

	class ClusterParameters
	{
	public:	

		ClusterParameters(int argc, char* argv[])
		{
			allParameters.CountOfDimensions = 3;
			allParameters.CountOfClusters = allParameters.CountOfDimensions;
			allParameters.CountOfObjects = 13;
			allParameters.Epsilon = 0.1;
			allParameters.Fuzzy = 1.5;
			allParameters.InputFilePath = "input.csv";
			allParameters.OutputFilePath = "output.csv";
			allParameters.CountOfThreads = 1;
			_argc = argc - 1;
			for (int i = 1; i < argc; i++)
			{
				_argv.push_back(string(argv[i]));
			}
		}

		~ClusterParameters()
		{

		}

		Parameters GetParameters()
		{
			parse();
			return allParameters;
		}

	private:

		int _argc;
		vector<string> _argv;
		Parameters allParameters;


		void parse()
		{
			for (int i = 0; i < _argc; i = i + 2)
			{
				if (_argv[i].compare("-i") == 0) getInputFilPath(i + 1);
				if (_argv[i].compare("-o") == 0) getOutputFilPath(i + 1);
				if (_argv[i].compare("-k") == 0) getCountOfClusters(i + 1);
				if (_argv[i].compare("-d") == 0) getCountOfDimensions(i + 1);
				if (_argv[i].compare("-n") == 0) getCountOfObjects(i + 1);
				if (_argv[i].compare("-f") == 0) getFuzzy(i + 1);
				if (_argv[i].compare("-e") == 0) getEpsilon(i + 1);
				if (_argv[i].compare("-t") == 0) getCountOfThreads(i + 1);
			}
		}

		void getInputFilPath(int numberOfparameter)
		{
			if (&_argv[numberOfparameter])
			{
				fstream file;
				file.open(_argv[numberOfparameter]);
				if (!file.fail()) {
					allParameters.InputFilePath = _argv[numberOfparameter];
				}
				file.close();
			}
		}

		void getOutputFilPath(int numberOfparameter)
		{
			if (&_argv[numberOfparameter])
			{
				fstream file;
				file.open(_argv[numberOfparameter], fstream::out);
				if (!file.fail()) {
					allParameters.OutputFilePath = _argv[numberOfparameter];
				}
				file.close();
			}
		}

		void getCountOfClusters(int numberOfparameter)
		{
			if (&_argv[numberOfparameter])
			{
				int val;
				istringstream iss(_argv[numberOfparameter]);
				if (iss >> val)
				{
					allParameters.CountOfClusters = val;
				}
			}
		}

		void getCountOfThreads(int numberOfparameter)
		{
			if (&_argv[numberOfparameter])
			{
				int val;
				istringstream iss(_argv[numberOfparameter]);
				if (iss >> val)
				{
					allParameters.CountOfThreads = val;
				}
			}
		}

		void getCountOfDimensions(int numberOfparameter)
		{
			if (&_argv[numberOfparameter])
			{
				int val;
				istringstream iss(_argv[numberOfparameter]);
				if (iss >> val)
				{
					allParameters.CountOfDimensions = val;
				}
			}
		}

		void getCountOfObjects(int numberOfparameter)
		{
			if (&_argv[numberOfparameter])
			{
				int val;
				istringstream iss(_argv[numberOfparameter]);
				if (iss >> val)
				{
					allParameters.CountOfObjects = val;
				}
			}
		}

		void getFuzzy(int numberOfparameter)
		{
			if (&_argv[numberOfparameter])
			{
				double val;
				istringstream iss(_argv[numberOfparameter]);
				if (iss >> val)
				{
					allParameters.Fuzzy = val;
				}
			}
		}

		void getEpsilon(int numberOfparameter)
		{
			if (&_argv[numberOfparameter])
			{
				double val;
				istringstream iss(_argv[numberOfparameter]);
				if (iss >> val)
				{
					allParameters.Epsilon = val;
				}
			}
		}
	};
}

