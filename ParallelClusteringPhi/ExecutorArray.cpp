/*
"Реализация параллельных версиий алгоритов кластеризации данных
с использованием многоядерных ускорителей"

Класс для инкапсуляции работы с кластеризацией данных.
Авторы: Сулейманов Д.Э., Цымблер М.Л.
*/

#include "ExecutorArray.h"

namespace ParallelClustering
{
	ExecutorArray::ExecutorArray(Parameters algorithmParameters)
	{
		AlgorithmParameters = algorithmParameters;
	}

	ExecutorArray::~ExecutorArray()
	{

	}

	double* ExecutorArray::CalculateProbabilities()
	{
		double* data = (double*)_mm_malloc(AlgorithmParameters.CountOfObjects*AlgorithmParameters.CountOfDimensions*sizeof(double),64);
		double* centroids = (double*)_mm_malloc(AlgorithmParameters.CountOfClusters*AlgorithmParameters.CountOfDimensions*sizeof(double), 64);
		setDateTime();
		//cout<<"I am here CalculateProbabilities";
		//for(int n=100;n<AlgorithmParameters.CountOfObjects; n+=100)
		//{
			
			if (tryReadFile(data,AlgorithmParameters.CountOfObjects))
			{
			//	printArray(data, AlgorithmParameters.CountOfDimensions, AlgorithmParameters.CountOfObjects, "data");
				FuzzyCMeansArrayOpenMP* cmeans;
				Runtime=0;
				double time=0;
				//for(int i=0; i<100; i++) {
					GetRandomObjectsArray(AlgorithmParameters.CountOfClusters, AlgorithmParameters.CountOfDimensions, centroids);
					cmeans = new FuzzyCMeansArrayOpenMP(data,AlgorithmParameters, Metrics::EvklidArray);
					//cout<<"I am here CalculateProbabilities 2";
					int start = omp_get_wtime();
					cmeans->StartClustering(centroids);
					Runtime = (omp_get_wtime() - start);
					time+=Runtime;
				//}
				Runtime=time;
				WriteLog(AlgorithmParameters.CountOfObjects);
				tryWriteFile(cmeans->UMatrix);
			}
			else {
				exit(EXIT_FAILURE);
			}
		//}
		return NULL;
	}
	
	void ExecutorArray::WriteLog(int n)
	{
		fstream log;
		log.open("log.csv", ios::out | ios::app);
		log <<
			DateTimeNow << ";" <<
			n << ";" <<
			AlgorithmParameters.CountOfDimensions << ";" <<
			AlgorithmParameters.CountOfClusters << ";" <<
			AlgorithmParameters.CountOfThreads << ";" <<
			AlgorithmParameters.Fuzzy << ";" <<
			AlgorithmParameters.Epsilon << ";" <<		
			Runtime << endl;
	}

	bool ExecutorArray::tryReadFile(double* data, int n)
	{
		
		fstream infile(AlgorithmParameters.InputFilePath);
		int row = 0;
		long long elementN = 0;
		while (infile && row<n)
		{
			string s;
			if (!getline(infile, s)) break;

			istringstream ss(s);


			while (ss)
			{

				string s;
				double p;
				if (!getline(ss, s, ';')) break;
				
				istringstream iss(s);
				if (iss >> p)
				{
					data[elementN] = p;
					elementN++;
				}
			}
			row++;
		}
		//cout << endl << endl;
		//cout << "ELEMENTN=" << elementN;
		return true;
	}

	void ExecutorArray::tryWriteFile(double* data)
	{
		fstream outfile(AlgorithmParameters.OutputFilePath, fstream::out);
		for (int n = 0; n < AlgorithmParameters.CountOfObjects; n++)
		{
			int c;
			for (c = 0; c < AlgorithmParameters.CountOfDimensions - 1; c++)
			{
				
				outfile << data[n*AlgorithmParameters.CountOfDimensions+c] << ";";
			}
			outfile << data[n*AlgorithmParameters.CountOfDimensions+c] << endl;
			
		}
		
	}

	void ExecutorArray::setDateTime()
	{/*
		time_t rawtime;
		struct tm timeinfo;
		char buffer[80];

		time(&rawtime);
		localtime_s(&timeinfo, &rawtime);

		strftime(buffer, 80, "%d-%m-%Y;%H:%M:%S", &timeinfo);
		std::string str(buffer);*/
		
		time_t rawtime;
		struct tm * timeinfo;
		char buffer[80];

		time(&rawtime);
		timeinfo = localtime(&rawtime);

		strftime(buffer, 80, "%d-%m-%Y;%H:%M:%S", timeinfo);		
		DateTimeNow = buffer;
	}

}
