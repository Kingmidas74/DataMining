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
	using namespace ClusteringAlgorithms::KCollection;


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
				auto kmetric = MetricFactory::GetMetric(AlgorithmParameters->Metric,2,true);
				auto cmetric = MetricFactory::GetMetric(AlgorithmParameters->Metric,2,true);

				auto normalization = NormalizationFactory::GetNormalization<double>(NormalizationTypes::Mean);

				auto kclustering = KMeans<double>(AlgorithmParameters, kmetric, normalization);
				auto cclustering = FuzzyCMeans<double,double>(AlgorithmParameters, cmetric, normalization);

				if(!kclustering.Guard() || !cclustering.Guard()) exit(EXIT_FAILURE);

				auto date = GetDate();

				omp_set_num_threads(AlgorithmParameters->CountOfThreads);

				double * kdistanceMatrix = allocateAlign<double>(AlgorithmParameters->CountOfObjects*AlgorithmParameters->CountOfObjects);
				double * cdistanceMatrix = allocateAlign<double>(AlgorithmParameters->CountOfObjects*AlgorithmParameters->CountOfObjects);
				kmetric->CalculateDistanceMatrix(vectors,AlgorithmParameters->CountOfObjects,AlgorithmParameters->CountOfDimensions,kdistanceMatrix);
				cmetric->CalculateDistanceMatrix(vectors,AlgorithmParameters->CountOfObjects,AlgorithmParameters->CountOfDimensions,cdistanceMatrix);

				double runtime = omp_get_wtime();
				kclustering.StartClustering(vectors);
				cclustering.StartClustering(vectors);
				runtime = RoundTo(omp_get_wtime() - runtime, 3);
				auto correctData = kclustering.Verification() && cclustering.Verification();

				if(correctData
					&& (fileIO.template tryOutMatrix<double>(AlgorithmParameters->CountOfObjects, AlgorithmParameters->CountOfClusters, cclustering.ResultMatrix))
					&& (fileIO.template tryOutMatrix<size_t>(AlgorithmParameters->CountOfObjects, 1, kclustering.ResultMatrix))
				)
				{
					auto evaluation = MonotonicPartition(AlgorithmParameters,cclustering.ResultMatrix);

					evaluation.Evaluate();

					CreateLogRecord(date,runtime,evaluation.EvaluationRate);

					freeAlign<double>(vectors);
					freeAlign<double>(kdistanceMatrix);
					freeAlign<double>(cdistanceMatrix);
					exit(EXIT_SUCCESS);
				}
				freeAlign<double>(kdistanceMatrix);
				freeAlign<double>(cdistanceMatrix);
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
