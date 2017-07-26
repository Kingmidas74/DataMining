#pragma once

#include "./io/main.h"
#include "./functions/main.h"
#include "./algorithms/main.h"

namespace DataMining {

	using namespace std;

	using namespace Metrics;
	using namespace Normalization;
	using namespace EvaluationAlgorithms;
	using namespace ClusteringAlgorithms;
	using namespace ClusteringAlgorithms::CCollection;


	class Executor
	{
	public:
		
		Executor() :AlgorithmParameters(nullptr),DateTimeNow(""),fileIO() {}

		explicit Executor(Parameters* algorithmParameters)
		{
			AlgorithmParameters = algorithmParameters;
			fileIO = FileIO();
			DateTimeNow = "";
		}

		explicit Executor(Parameters* algorithmParameters, FileIO _fileIO): DateTimeNow("")
		{
			AlgorithmParameters = algorithmParameters;
			fileIO = _fileIO;
		}

		void CalculateProbabilities()
		{
			double * vectors=allocateAlign<double>(AlgorithmParameters->CountOfObjects*AlgorithmParameters->CountOfDimensions);
			
			if(fileIO.template tryReadMatrixFromFile<double>(AlgorithmParameters->InputFilePath, AlgorithmParameters->CountOfObjects, AlgorithmParameters->CountOfDimensions, vectors))
			{
				auto metric = MetricFactory::GetMetric(MetricTypes::Minkowsi,2,true);
				
				double * distanceMatrix = allocateAlign<double>(AlgorithmParameters->CountOfObjects*AlgorithmParameters->CountOfObjects);
				metric->CalculateDistanceMatrix(vectors,AlgorithmParameters->CountOfObjects,AlgorithmParameters->CountOfDimensions,distanceMatrix);
			
				auto normalization = NormalizationFactory::GetNormalization<double>(NormalizationTypes::Mean);
			
				auto clustering = FuzzyCMeans<double,double>(AlgorithmParameters, metric, normalization);

				

				setDateTime();

				omp_set_num_threads(AlgorithmParameters->CountOfThreads);
				double runtime = omp_get_wtime();
				clustering.StartClustering(vectors);
				runtime = omp_get_wtime() - runtime;
				runtime = RoundTo(runtime, 3);
				auto correctData = clustering.Verification();		
				
				if(correctData && (fileIO.template tryWriteMatrixToFile<double>(AlgorithmParameters->OutputFilePath, AlgorithmParameters->CountOfObjects, AlgorithmParameters->CountOfClusters, clustering.ResultMatrix)))
				{					
					auto evaluation = Partition(AlgorithmParameters,clustering.ResultMatrix);

					evaluation.Evaluate();
					
					

					CreateLogRecord(runtime,evaluation.EvaluationRate);

					freeAlign<double>(vectors);
					freeAlign<double>(distanceMatrix);
					exit(EXIT_SUCCESS);
				}
				freeAlign<double>(distanceMatrix);
			}
			freeAlign<double>(vectors);
			
			exit(EXIT_FAILURE);
		}

	private:
		Parameters* AlgorithmParameters;
		string		DateTimeNow;
		FileIO		fileIO;

		

		void CreateLogRecord(double runtime, double eR)
		{
			
			string* row = new string[9];
			row[0] = DateTimeNow;
			row[1] = stringify(AlgorithmParameters->CountOfObjects);
			row[2] = stringify(AlgorithmParameters->CountOfDimensions);
			row[3] = stringify(AlgorithmParameters->CountOfClusters);
			row[4] = stringify(AlgorithmParameters->CountOfThreads);
			row[5] = stringify(AlgorithmParameters->Fuzzy);
			row[6] = stringify(AlgorithmParameters->Epsilon);
			row[7] = stringify(runtime);
			row[8] = stringify(eR);
			fileIO.tryAppendStringRowToFile(AlgorithmParameters->LogFilePath, 1,9, row);
			delete[] row;
		}

		void setDateTime()
		{
			DateTimeNow = GetDate();			
		}
	};
}
