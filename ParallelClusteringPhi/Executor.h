#pragma once

/*
"Реализация параллельных версиий алгоритов кластеризации данных
с использованием многоядерных ускорителей"

Класс для инкапсуляции работы с кластеризацией данных.
Авторы: Сулейманов Д.Э., Цымблер М.Л.
*/

#include <vector>
#include <fstream>
#include <sstream>
#include <ctime>
#include <omp.h>

#include "Helper.h"
#include "Clustering.h"
#include "KMeans.h"
#include "KMeansPP.h"
#include "FuzzyCMeans.h"
#include "MetricsDistance.h"

namespace ParallelClustering
{
	using namespace std;
	using namespace ParallelClustering;
	using namespace ParallelClustering::KMeansCollection;
	using namespace ParallelClustering::CMeansCollection;
	using namespace ParallelClustering::Metrics;

	template<class IncomingType, class OutcommingType>
	class Executor
	{
	public:
		Parameters* AlgorithmParameters;
		double Runtime;
		string DateTimeNow;

		Executor(Parameters* algorithmParameters)
		{
			AlgorithmParameters = algorithmParameters;
		}

		void CalculateProbabilities()
		{
			
			IncomingType* data = allocateAlign<IncomingType>(AlgorithmParameters->CountOfObjects * AlgorithmParameters->CountOfDimensions);
			IncomingType* centroids = allocateAlign<IncomingType>(AlgorithmParameters->CountOfClusters * AlgorithmParameters->CountOfDimensions);
			
			setDateTime();

			if (tryReadFile(data))
			{
				Clustering<IncomingType,OutcommingType>* clustering;
				
				int start = omp_get_wtime();
				GetRandomObjectsArray(AlgorithmParameters->CountOfClusters, AlgorithmParameters->CountOfDimensions, centroids);
				clustering = new KMeans<IncomingType,OutcommingType>(data, AlgorithmParameters, Metrics::EuclidianSquare<IncomingType>);
				clustering->StartClustering();
				Runtime = (omp_get_wtime() - start);
				
				WriteLog();				
				tryWriteFile(clustering->ResultMatrix);
			}
			else {
				freeAlign(data);
				freeAlign(centroids);
				exit(EXIT_FAILURE);
			}
			freeAlign(data);
			freeAlign(centroids);
		}

		void WriteLog()
		{
			fstream log;
			log.open("log.csv", ios::out | ios::app);
			log <<
				DateTimeNow << ";" <<
				AlgorithmParameters->CountOfObjects << ";" <<
				AlgorithmParameters->CountOfDimensions << ";" <<
				AlgorithmParameters->CountOfClusters << ";" <<
				AlgorithmParameters->CountOfThreads << ";" <<
				AlgorithmParameters->Fuzzy << ";" <<
				AlgorithmParameters->Epsilon << ";" <<
				Runtime << endl;
		}

		bool tryReadFile(IncomingType* data)
		{
			fstream infile(AlgorithmParameters->InputFilePath);
			int row = 0;
			long long elementN = 0;
			while (infile && row<AlgorithmParameters->CountOfObjects)
			{
				string s;
				if (!getline(infile, s)) break;

				istringstream ss(s);

				int dim = 0;
				while (ss && dim<AlgorithmParameters->CountOfDimensions)
				{

					string s;
					double p;
					if (!getline(ss, s, ';')) break;

					istringstream iss(s);
					if (iss >> p)
					{
						data[elementN] = p;
						dim++;
						elementN++;
					}
				}
				row++;
			}
			return true;
		}

		void tryWriteFile(OutcommingType* data)
		{
			fstream outfile(AlgorithmParameters->OutputFilePath, fstream::out);
			for (int n = 0; n < AlgorithmParameters->CountOfObjects; n++)
			{
				/*int c;
				for (c = 0; c < AlgorithmParameters->CountOfDimensions - 1; c++)
				{

					outfile << data[n*AlgorithmParameters->CountOfDimensions + c] << ";";
				}*/
				cout << data[n]<<endl;
				outfile << data[n] << endl;
			}

		}

		void setDateTime()
		{
			time_t rawtime;
			struct tm * timeinfo;
			char buffer[80];

			time(&rawtime);
			timeinfo = localtime(&rawtime);

			strftime(buffer, 80, "%d-%m-%Y;%H:%M:%S", timeinfo);
			DateTimeNow = buffer;
		}

		virtual ~Executor()
		{
			
		}
	};
}