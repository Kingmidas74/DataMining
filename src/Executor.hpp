#pragma once

#include "./io/main.h"
#include "./functions/main.h"
#include "./algorithms/main.h"

namespace DataMining {

	using namespace std;

	using namespace Metrics;
	using namespace Normalization;
	using namespace ClusteringAlgorithms;
	using namespace ClusteringAlgorithms::CCollection;


	class Executor
	{
	public:
		
		Executor() :AlgorithmParameters(nullptr), Runtime(0),DateTimeNow(""),fileIO() {}

		explicit Executor(Parameters* algorithmParameters)
		{
			AlgorithmParameters = algorithmParameters;
			Runtime = 0;
			fileIO = FileIO();
			DateTimeNow = "";
		}

		explicit Executor(Parameters* algorithmParameters, FileIO _fileIO): DateTimeNow("")
		{
			AlgorithmParameters = algorithmParameters;
			Runtime = 0;
			fileIO = _fileIO;
			
		}

		void CalculateProbabilities()
		{
			auto metric = MetricFactory::GetMetric(MetricTypes::Minkowsi,2,true);

			auto normalization = NormalizationFactory::GetNormalization<double>(NormalizationTypes::Mean);
			
			auto clustering = FuzzyCMeans<double,double>(AlgorithmParameters, metric, normalization, fileIO);
			
			if (clustering.TryGetData()) 
			{
				setDateTime();

				omp_set_num_threads(AlgorithmParameters->CountOfThreads);
				//clustering.CalculateAllDistance();

				
				Runtime = omp_get_wtime();
				clustering.StartClustering();
				Runtime = omp_get_wtime() - Runtime;
				Runtime = RoundTo(Runtime, 3);
				auto correctData = clustering.Verification();				
				if (!correctData || !clustering.TrySaveData())
				{
					exit(EXIT_FAILURE);
				}				
				CreateLogRecord(correctData);
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

		void CreateLogRecord(bool correctData)
		{
			
			string* row = new string[8];
			row[0] = DateTimeNow;
			row[1] = std::to_string(static_cast<unsigned long long>(AlgorithmParameters->CountOfObjects));
			row[2] = std::to_string(static_cast<unsigned long long>(AlgorithmParameters->CountOfDimensions));
			row[3] = std::to_string(static_cast<unsigned long long>(AlgorithmParameters->CountOfClusters));
			row[4] = std::to_string(static_cast<unsigned long long>(AlgorithmParameters->CountOfThreads));
			ostringstream fstring;
			fstring << AlgorithmParameters->Fuzzy;
			row[5] = fstring.str();
			ostringstream estring;
			estring << AlgorithmParameters->Epsilon;
			row[6] = estring.str();
			ostringstream rstring;
			rstring << Runtime;
			row[7] = rstring.str();
			//row[8]=std::to_string(static_cast<bool>(correctData));
			fileIO.tryAppendStringRowToFile(AlgorithmParameters->LogFilePath, 1,8, row);
			delete[] row;
		}

		void setDateTime()
		{
			DateTimeNow = GetDate();			
		}
	};
}
