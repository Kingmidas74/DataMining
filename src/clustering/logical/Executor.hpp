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
				//clustering.ggg();

				tryWriteFile(clustering.ResultMatrix);
				//freeAlign<double>(data);
			}
			else {
				//freeAlign<double>(data);
				//freeAlign<double>(centroids);
				exit(EXIT_FAILURE);
			}
			//freeAlign<double>(data);
			//freeAlign<double>(centroids);
		}

	private:
		Parameters* AlgorithmParameters;
		double		Runtime;
		string		DateTimeNow;

		void WriteLog()
		{
			
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

		void tryWriteFile(double * data)
		{



			ofstream outfile(AlgorithmParameters->OutputFilePath, fstream::out );
			cout.precision(5);
			outfile.precision(5);

			if (outfile.is_open())
			{
				for (unsigned int n = 0; n < AlgorithmParameters->CountOfObjects; n++)
				{
					double s = 0.0;
					unsigned int c;
					for (c = 0; c < AlgorithmParameters->CountOfClusters - 1; c++)
					{
						cout << data[n*2 + c] << ";";
						outfile <<fixed<< data[n*AlgorithmParameters->CountOfDimensions + c] << ";";
						s += data[n*AlgorithmParameters->CountOfClusters + c];
						s += data[n * 2 + c];

					}
					cout << data[n * 2 + c] << ";" ;
					outfile << fixed << data[n*AlgorithmParameters->CountOfDimensions + c] << ";";
					s += data[n * 2 + c];
					cout << s << endl;
					outfile << fixed << s << endl;
				}
				outfile.close();
			}
		}

		void setDateTime()
		{
			/*time_t rawtime;
			struct tm * timeinfo;
			char buffer[80];

			time(&rawtime);
			timeinfo = localtime(&rawtime);

			strftime(buffer, 80, "%d-%m-%Y;%H:%M:%S", timeinfo);
			DateTimeNow = buffer;*/
		}
	};
}
