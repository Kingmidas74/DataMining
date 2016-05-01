/*
"Реализация параллельных версиий алгоритов кластеризации данных
с использованием многоядерных ускорителей"

Класс для определения входных данных.
Авторы: Сулейманов Д.Э., Цымблер М.Л.
*/

#include "ClusterParameters.h"


namespace ParallelClustering 
{
	ClusterParameters::ClusterParameters(int argc, char* argv[])
	{
		_argc = argc-1;		
		for (int i = 1; i < argc; i++) 
		{
			_argv.push_back(argv[i]);
		}
	}


	ClusterParameters::~ClusterParameters()
	{
	}

	void ClusterParameters::Parse()
	{
		for (int i = 0; i < _argc; i=i+2)
		{
			if (strcmp(_argv[i], "-i") == 0) getInputFilPath(i + 1);
			if (strcmp(_argv[i], "-o") == 0) getOutputFilPath(i + 1);
			if (strcmp(_argv[i], "-k") == 0) getCountOfClusters(i + 1);
			if (strcmp(_argv[i], "-d") == 0) getCountOfDimensions(i + 1);
			if (strcmp(_argv[i], "-n") == 0) getCountOfObjects(i + 1);
			if (strcmp(_argv[i], "-f") == 0) getFuzzy(i + 1);
			if (strcmp(_argv[i], "-e") == 0) getEpsilon(i + 1);
		}
	}

	Parameters ClusterParameters::GetParameters()
	{
		Parse();
		return allParameters;
	}

	void ClusterParameters::getInputFilPath(int numberOfparameter) 
	{
		if (_argv[numberOfparameter])
		{
			fstream file;
			file.open(_argv[numberOfparameter]);
			if (!file.fail()) {
				allParameters.InputFilePath = _argv[numberOfparameter];
			}
			file.close();
		}
	}

	void ClusterParameters::getOutputFilPath(int numberOfparameter)
	{
		if (_argv[numberOfparameter])
		{
			fstream file;
			file.open(_argv[numberOfparameter], fstream::out);
			if (!file.fail()) {
				allParameters.OutputFilePath = _argv[numberOfparameter];
			}
			file.close();
		}
	}

	void ClusterParameters::getCountOfClusters(int numberOfparameter)
	{		
		if (_argv[numberOfparameter])
		{			
			int val;
			istringstream iss(_argv[numberOfparameter]);
			if (iss >> val)
			{
				allParameters.CountOfClusters = val;
			}
		}
	}

	void ClusterParameters::getCountOfDimensions(int numberOfparameter)
	{
		if (_argv[numberOfparameter])
		{
			int val;
			istringstream iss(_argv[numberOfparameter]);
			if (iss >> val)
			{
				allParameters.CountOfDimensions = val;
			}
		}
	}

	void ClusterParameters::getCountOfObjects(int numberOfparameter)
	{
		if (_argv[numberOfparameter])
		{
			int val;
			istringstream iss(_argv[numberOfparameter]);
			if (iss >> val)
			{
				allParameters.CountOfObjects = val;
			}
		}
	}

	void ClusterParameters::getFuzzy(int numberOfparameter)
	{
		if (_argv[numberOfparameter])
		{
			double val;
			istringstream iss(_argv[numberOfparameter]);
			if (iss >> val)
			{
				allParameters.Fuzzy = val;
			}
		}
	}

	void ClusterParameters::getEpsilon(int numberOfparameter)
	{
		if (_argv[numberOfparameter])
		{
			double val;
			istringstream iss(_argv[numberOfparameter]);
			if (iss >> val)
			{
				allParameters.Epsilon = val;
			}
		}
	}
}
