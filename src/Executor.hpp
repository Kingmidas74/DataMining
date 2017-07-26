#pragma once

#include "./io/main.h"
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
		
		Executor() :AlgorithmParameters(nullptr),fileIO() {}

		explicit Executor(Parameters* algorithmParameters):AlgorithmParameters(algorithmParameters), fileIO(FileIO(','))
		{}

		Executor(Parameters* algorithmParameters, FileIO _fileIO):AlgorithmParameters(algorithmParameters), fileIO(_fileIO)
		{}

		void CalculateProbabilities()
		{
			double * vectors=allocateAlign<double>(AlgorithmParameters->CountOfObjects*AlgorithmParameters->CountOfDimensions);
			
			if(fileIO.template tryReadMatrixFromFile<double>(AlgorithmParameters->InputFilePath, AlgorithmParameters->CountOfObjects, AlgorithmParameters->CountOfDimensions, vectors))
			{
				auto metric = MetricFactory::GetMetric(MetricTypes::Minkowsi,2,true);
				
				auto normalization = NormalizationFactory::GetNormalization<double>(NormalizationTypes::Mean);
			
				auto clustering = FuzzyCMeans<double,double>(AlgorithmParameters, metric, normalization);

				auto date = GetDate();

				omp_set_num_threads(AlgorithmParameters->CountOfThreads);

				double * distanceMatrix = allocateAlign<double>(AlgorithmParameters->CountOfObjects*AlgorithmParameters->CountOfObjects);
				metric->CalculateDistanceMatrix(vectors,AlgorithmParameters->CountOfObjects,AlgorithmParameters->CountOfDimensions,distanceMatrix);
			
				double runtime = omp_get_wtime();
				clustering.StartClustering(vectors);
				runtime = RoundTo(omp_get_wtime() - runtime, 3);
				auto correctData = clustering.Verification();		
				
				if(correctData && (fileIO.template tryWriteMatrixToFile<double>(AlgorithmParameters->OutputFilePath, AlgorithmParameters->CountOfObjects, AlgorithmParameters->CountOfClusters, clustering.ResultMatrix)))
				{					
					auto evaluation = Partition(AlgorithmParameters,clustering.ResultMatrix);

					evaluation.Evaluate();

					CreateLogRecord(date,runtime,evaluation.EvaluationRate);

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
		FileIO		fileIO;
		
		void CreateLogRecord(string date, double runtime, double eR)
		{
			
			string* row = new string[9];
			row[0] = date;
			row[1] = stringify(AlgorithmParameters->CountOfObjects);
			row[2] = stringify(AlgorithmParameters->CountOfDimensions);
			row[3] = stringify(AlgorithmParameters->CountOfClusters);
			row[4] = stringify(AlgorithmParameters->CountOfThreads);
			row[5] = stringify(AlgorithmParameters->Fuzzy);
			row[6] = stringify(AlgorithmParameters->Epsilon);
			row[7] = stringify(eR);
			row[8] = stringify(runtime);
			fileIO.tryAppendStringRowToFile(AlgorithmParameters->LogFilePath, 1,9, row);
			delete[] row;
		}
	};
}
