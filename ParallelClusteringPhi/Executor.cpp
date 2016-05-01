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
		_algorithmParameters = algorithmParameters;
	}

	Executor::~Executor() 
	{

	}

	vector<vector<double>> Executor::CalculateProbabilities()
	{
		vector<vector<double>> data;
		vector<vector<double>> centroids;
		if (tryReadFile(data))
		{
			centroids = GetRandomObjects(_algorithmParameters.CountOfClusters, data[0].size());
			FuzzyCMeans* cmeans = new FuzzyCMeans(data, _algorithmParameters.Epsilon, _algorithmParameters.Fuzzy, GetMetrics(MetricsDistanceTypes::Evklid));
			cmeans->StartClustering(centroids);
			tryWriteFile(cmeans->VectorsOfProbabilities);
		}
		else {
			exit(EXIT_FAILURE);
		}
		return vector<vector<double>>();
	}

	bool Executor::tryReadFile(vector<vector<double>> &data)
	{
		PrintObjects(data);
		data.clear();
		fstream infile(_algorithmParameters.InputFilePath);

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
		PrintObjects(data);
		return true;
	}

	void Executor::tryWriteFile(vector<vector<double>> &data)
	{
		fstream outfile(_algorithmParameters.OutputFilePath);
		for (int n = 0; n<data.size(); n++)
		{
			int c;
			for (c = 0; c < data[n].size()-1; c++)
			{
				outfile << data[n][c] << ";";
			}
			outfile << data[n][c] << endl;
		}
	}

}