#pragma once

/**
Executor.h
Purpose: Incapsulation of workflow.

@author		Suleymanov D.
@version	0.0.0.1 9/17/2016
*/

namespace ParallelClustering {

	using namespace std;

	using namespace Metrics;
	using namespace ClusteringAlgorithms;
	using namespace ClusteringAlgorithms::CMeansCollection;


	class Executor
	{
	public:
		
		Executor() :AlgorithmParameters(nullptr), Runtime(0),DateTimeNow(""),fileIO(';') {}

		explicit Executor(Parameters* algorithmParameters)
		{
			AlgorithmParameters = algorithmParameters;
			Runtime = 0;
			fileIO = FileIO();
			DateTimeNow = "";
		}

		explicit Executor(Parameters* algorithmParameters, FileIO _fileIO)
		{
			AlgorithmParameters = algorithmParameters;
			Runtime = 0;
			fileIO = _fileIO;
			DateTimeNow = "";
		}

		void CalculateProbabilities()
		{
			auto metric = DynamicTimeWarping(); 
			auto clustering = FuzzyCMeans(AlgorithmParameters, &metric, fileIO);
			cout << "1" << endl;
			
			if (clustering.TryGetData()) 
			{
				setDateTime();

				clustering.CalculateAllDistance();
				cout << "2" << endl;
				clustering.StartClustering();
				cout << "3" << endl;
				if (!clustering.TrySaveData())
				{
					exit(EXIT_FAILURE);
				}

				Runtime = 1;
				CreateLogRecord();
			}
			else
			{
				exit(EXIT_FAILURE);
			}
		}

	private:
		Parameters* AlgorithmParameters;
		double		Runtime;
		string		DateTimeNow;
		FileIO		fileIO;

		void CreateLogRecord()
		{
			string* row = new string[8];
			row[0] = DateTimeNow;
			row[1] = std::to_string(static_cast<unsigned long long>(AlgorithmParameters->CountOfObjects));
			row[2] = std::to_string(static_cast<unsigned long long>(AlgorithmParameters->CountOfDimensions));
			row[3] = std::to_string(static_cast<unsigned long long>(AlgorithmParameters->CountOfClusters));
			row[4] = std::to_string(static_cast<long double>(AlgorithmParameters->Fuzzy));
			row[5] = std::to_string(static_cast<long double>(AlgorithmParameters->Epsilon));
			row[6] = std::to_string(static_cast<unsigned long long>(Runtime));
			row[7] = std::to_string(static_cast<unsigned long long>(AlgorithmParameters->CountOfThreads));
			fileIO.tryAppendStringRowToFile(AlgorithmParameters->LogFilePath, 1, 8, row);
			delete[] row;
		}

		void setDateTime()
		{
			DateTimeNow = GetDate();			
		}
	};
}
