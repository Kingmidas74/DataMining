#pragma once
#include <ctime>


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

		Executor():Runtime(0)
		{
			AlgorithmParameters = nullptr;
		}

		explicit Executor(Parameters* algorithmParameters)
		{
			AlgorithmParameters = algorithmParameters;
			Runtime = 0;
		}

		void CalculateProbabilities()
		{
			setDateTime();

			auto metric = MinkowskiMetric(2, true);
			auto fileIO = FileIO(';');
			auto clustering = FuzzyCMeans(AlgorithmParameters, &metric, fileIO);
			cout << "1" << endl;				
			clustering.CalculateAllDistance();
			cout << "2" << endl;
			clustering.StartClustering();
			cout << "3" << endl;
			if(!clustering.TrySaveData())
			{
				exit(EXIT_FAILURE);
			};
			Runtime = 1;
			WriteLog();
		
		}

	private:
		Parameters* AlgorithmParameters;
		double		Runtime;
		string		DateTimeNow;

		void WriteLog()
		{
			ofstream log;
			log.open("log.csv", ios::out | ios::app);
			if (log.is_open()) {
				log <<
					DateTimeNow << ";" <<
					AlgorithmParameters->CountOfObjects << ";" <<
					AlgorithmParameters->CountOfDimensions << ";" <<
					AlgorithmParameters->CountOfClusters << ";" <<
					AlgorithmParameters->Fuzzy << ";" <<
					AlgorithmParameters->Epsilon << ";" <<
					AlgorithmParameters->CountOfThreads << ";" <<
					Runtime << endl;
				log.close();
			}
		}

		void setDateTime()
		{
			return;
			time_t rawtime;
			struct tm * timeinfo;
			char buffer[80];

			time(&rawtime);
			timeinfo = localtime(&rawtime);

			strftime(buffer, 80, "%d-%m-%Y;%H:%M:%S", timeinfo);
			DateTimeNow = buffer;
		}
	};
}
