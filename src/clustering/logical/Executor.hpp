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
			auto data		= allocateAlign<double>(AlgorithmParameters->CountOfObjects*AlgorithmParameters->CountOfDimensions);
			///auto centroids	= allocateAlign<double>(AlgorithmParameters->CountOfClusters*AlgorithmParameters->CountOfDimensions);

			setDateTime();

			if (tryReadFile(data))
			{
				auto clustering = FuzzyCMeans(data, AlgorithmParameters, Metrics::MinkowskiSquare);
				clustering.CalculateAllDistance();
				clustering.StartClustering();
				auto resultData = clustering.GetResult();

				Runtime = 1;

				tryWriteFile(AlgorithmParameters->OutputFilePath,resultData,AlgorithmParameters->CountOfObjects*AlgorithmParameters->CountOfClusters,AlgorithmParameters->CountOfClusters);
				WriteLog();
				//freeAlign<double>(data);
				//freeAlign<double>(centroids);
			}
			else {
				//freeAlign<double>(data);
				//freeAlign<double>(centroids);
				exit(EXIT_FAILURE);
			}
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

		bool tryReadFile(double* data)
		{			
			fstream infile(AlgorithmParameters->InputFilePath);
			unsigned int row = 0;
			long long elementN = 0;
			while (infile && row<AlgorithmParameters->CountOfObjects)
			{
				string s;
				if (!getline(infile, s)) break;

				istringstream ss(s);
				
				unsigned int dim = 0;
				while (ss && dim<AlgorithmParameters->CountOfDimensions)
				{

					string str;
					double p;
					if (!getline(ss, str, ';')) break;
					
					istringstream iss(str);
					if (iss >> p)
					{
						data[elementN]=p;
						dim++;
						elementN++;
					}
				}
				row++;
			}
			infile.close();
			return true;
		}

		void tryWriteFile(string filePath, double * data, unsigned int length, unsigned int row_length)
		{
			ofstream outfile(filePath);
			outfile.precision(5);
			int current_size = 0;
			if (outfile.is_open())
			{
				for (unsigned int i = 0;i < length;i++)
				{
					outfile << data[i];
					current_size++;
					if (current_size == row_length)
					{
						outfile << endl;
						current_size = 0;
					}
					else
					{
						outfile << ";";
					}
				}
				outfile.close();
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
