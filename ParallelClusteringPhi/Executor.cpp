/*
"Реализация параллельных версиий алгоритов кластеризации данных
с использованием многоядерных ускорителей"

Класс для инкапсуляции работы с кластеризацией данных.
Авторы: Сулейманов Д.Э., Цымблер М.Л.
*/

#include "Executor.h"

namespace ParallelClustering
{
	Executor::Executor(Parameters algorithmParameters)
	{
		AlgorithmParameters = algorithmParameters;
	}

	Executor::~Executor()
	{

	}

	vector<vector<double>> Executor::CalculateProbabilities()
	{
		vector<vector<double>> data;
		vector<vector<double>> centroids;
		setDateTime();
		//for(int n=100;n<AlgorithmParameters.CountOfObjects; n+=100)
		//{
			
			if (tryReadFile(data,AlgorithmParameters.CountOfObjects))
			{				
				AlgorithmParameters.CountOfDimensions = data[0].size();
				FuzzyCMeans* cmeans;
				Runtime=0;
				double time=0;
				//for(int i=0; i<100; i++) {
					centroids = GetRandomObjects(AlgorithmParameters.CountOfClusters, data[0].size());
					cmeans = new FuzzyCMeans(data, AlgorithmParameters.Epsilon, AlgorithmParameters.Fuzzy, GetMetrics(MetricsDistanceTypes::Evklid));
					cmeans->StartClustering(centroids);
					Runtime = cmeans->ClearRuntime;
					time+=Runtime;
				//}
				Runtime=time;
				WriteLog(AlgorithmParameters.CountOfObjects);
				tryWriteFile(cmeans->VectorsOfProbabilities);
				
			}
			else {
				exit(EXIT_FAILURE);
			}
		//}
		return vector<vector<double>>();
	}
	
	void Executor::WriteLog(int &n)
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

	bool Executor::tryReadFile(vector<vector<double>> &data, int &n)
	{
		fstream infile(AlgorithmParameters.InputFilePath);
		int readCount=0;
		while (infile && readCount<n)
		{
			string s;
			if (!getline(infile, s)) break;

			istringstream ss(s);
			vector <double> record;

			while (ss)
			{
				string s;
				double p;
				if (!getline(ss, s, ';')) break;

				istringstream iss(s);
				if (iss >> p)
				{
					record.push_back(p);
				}
			}

			data.push_back(record);
			readCount++;
		}
		return true;
	}

	void Executor::tryWriteFile(vector<vector<double>> &data)
	{
		fstream outfile(AlgorithmParameters.OutputFilePath, fstream::out);
		for (int n = 0; n < data.size(); n++)
		{
			int c;
			for (c = 0; c < data[n].size() - 1; c++)
			{
				outfile << data[n][c] << ";";
			}
			outfile << data[n][c] << endl;
		}
	}

	void Executor::setDateTime()
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
