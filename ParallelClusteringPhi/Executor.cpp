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
		if (tryReadFile(data))
		{
			AlgorithmParameters.CountOfObjects = data.size();
			AlgorithmParameters.CountOfDimensions = data[0].size();
			centroids = GetRandomObjects(AlgorithmParameters.CountOfClusters, data[0].size());
			FuzzyCMeans* cmeans = new FuzzyCMeans(data, AlgorithmParameters.Epsilon, AlgorithmParameters.Fuzzy, GetMetrics(MetricsDistanceTypes::Evklid));
			cmeans->StartClustering(centroids);
			Runtime = cmeans->ClearRuntime;			
			tryWriteFile(cmeans->VectorsOfProbabilities);
			
		}
		else {
			exit(EXIT_FAILURE);
		}
		return vector<vector<double>>();
	}

	bool Executor::tryReadFile(vector<vector<double>> &data)
	{
		fstream infile(AlgorithmParameters.InputFilePath);

		while (infile)
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
	{
		time_t rawtime;
		struct tm timeinfo;
		char buffer[80];

		time(&rawtime);
		localtime_s(&timeinfo, &rawtime);

		strftime(buffer, 80, "%d-%m-%Y;%H:%M:%S", &timeinfo);
		std::string str(buffer);
		DateTimeNow = str;
	}

}