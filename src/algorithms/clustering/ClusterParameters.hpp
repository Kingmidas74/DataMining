#pragma once

/**
DICParameters.hpp
Purpose: Parser of cmd parameters.

@author		Suleymanov D.
@version	0.0.0.1 9/17/2016
*/

namespace DataMining {
	using namespace std;

	class ClusterParameters
	{
	public:

		ClusterParameters(int argc, char* argv[])
		{
			allParameters.CountOfClusters = 3;
			allParameters.Epsilon = 0.1;
			allParameters.Fuzzy = 1.5;
			allParameters.OutputFilePath = "output.csv";
			allParameters.LogFilePath = "log.csv";
			allParameters.CountOfThreads = 1;
			_argc = argc;
			_argv = argv;
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
		char ** _argv;
		Parameters allParameters;


		void parse()
		{
			for (int i = 1; i < _argc; i = i + 2)
			{
				if ((string)_argv[i] == ("-i")) getInputFilPath(i + 1);
				if ((string)_argv[i] == ("-o")) getOutputFilPath(i + 1);
				if ((string)_argv[i] == ("-l")) getLogFilePath(i + 1);
				if ((string)_argv[i] == ("-k")) getCountOfClusters(i + 1);
				if ((string)_argv[i] == ("-d")) getCountOfDimensions(i + 1);
				if ((string)_argv[i] == ("-n")) getCountOfObjects(i + 1);
				if ((string)_argv[i] == ("-f")) getFuzzy(i + 1);
				if ((string)_argv[i] == ("-e")) getEpsilon(i + 1);
				if ((string)_argv[i] == ("-t")) getCountOfThreads(i + 1);
				//if ((string)_argv[i] == ("-m")) getMetricType(i + 1);
			}
		}

		void getInputFilPath(int numberOfparameter)
		{
			if (&_argv[numberOfparameter])
			{
				if(tryGetDataParameters(_argv[numberOfparameter]))
				{
					allParameters.InputFilePath = _argv[numberOfparameter];
				}
			}
		}

		void getMetricType(int numberOfparameter)
		{

		}

		void getLogFilePath(int numberOfparameter)
		{
			if (&_argv[numberOfparameter])
			{
				fstream file;
				file.open(_argv[numberOfparameter]);
				if (!file.fail()) {
					allParameters.LogFilePath = _argv[numberOfparameter];
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

		bool tryGetDataParameters(string filePath)
		{
			bool result = false;
			ifstream file;
			file.open(filePath);
			if (!file.fail())
			{
				string sLine;
				getline(file, sLine);
				std::stringstream ss(sLine);
				allParameters.CountOfDimensions = count(std::istreambuf_iterator<char>(ss), std::istreambuf_iterator<char>(), ',')+1;
				allParameters.CountOfObjects = count(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), '\n')+1;
				result = true;
			}
			file.close();
			return result;
		}
	};
}
