#pragma once

namespace ParallelClustering {
	namespace ClusteringAlgorithms {
		namespace CCollection {


			using namespace std;
			using namespace ParallelClustering::Metrics;
			using namespace ParallelClustering::IO;

			class FuzzyCMedoids : public Clustering
			{

			private:

				double* _powMatrix;
				int* _medoids;
				int* _objects;

				void GenerateDefaultResultMatrix()
				{
					for (int i = 0; i < AlgorithmParameters->CountOfObjects;i++)
					{
						for (int j = 0; j < AlgorithmParameters->CountOfClusters;j++)
						{
							ResultMatrix[i*AlgorithmParameters->CountOfClusters + j] = GetRandomDouble();
						}
						normalizeArray(&ResultMatrix[i*AlgorithmParameters->CountOfClusters], AlgorithmParameters->CountOfClusters);
					}
				}

				void GenerateObjects()
				{
					int objectsCount = 0;
					for (int i = 0; i<AlgorithmParameters->CountOfObjects;i++)
					{
						bool contains = false;
						for (int j = 0;j<AlgorithmParameters->CountOfClusters;j++)
						{
							if (_medoids[j] == i)
							{
								contains = true;
								break;
							}
						}
						if (!contains)
						{
							_objects[objectsCount] = i;
							objectsCount++;
						}
					}
				}

				void GenerateCentroids() override
				{		
					for(int i=0;i<AlgorithmParameters->CountOfClusters;i++)
					{
						int random = GetRandomInt(0, AlgorithmParameters->CountOfObjects-1);
						bool exist = false;
						for (int j = 0;j < i;j++)
						{
							if (_medoids[j] == random)
							{
								exist = true;
								i--;
							}
						}
						if (exist == false)
						{
							_medoids[i] = random;
						}						
					} 
				}

				void ExecuteClustering(int* medoids)
				{
					bool decision = false;
					int step = 0;
					while(decision==false && step<8)
					{
						step++;
						SetClusters(medoids);
						cout << "SetClusters" << endl;
						for (int i = 0; i < AlgorithmParameters->CountOfClusters; i++) cout << medoids[i] << " ";
						cout << endl;
						for (int i = 0; i < AlgorithmParameters->CountOfClusters; i++) cout << _medoids[i] << " ";
						cout << endl;
						PrintAsMatrix(ResultMatrix, AlgorithmParameters->CountOfClusters, AlgorithmParameters->CountOfClusters*AlgorithmParameters->CountOfObjects);
						CalculateMedoids(medoids);						
						cout << "calcMedoids" << endl;
						for (int i = 0; i < AlgorithmParameters->CountOfClusters; i++) cout << medoids[i] << " ";
						cout << endl;
						for (int i = 0; i < AlgorithmParameters->CountOfClusters; i++) cout << _medoids[i] << " ";
						cout << endl;
						PrintAsMatrix(ResultMatrix, AlgorithmParameters->CountOfClusters, AlgorithmParameters->CountOfClusters*AlgorithmParameters->CountOfObjects);
						decision = calculateDecision(medoids);
						cout << "CalcDes=" <<decision<< endl;
						for (int i = 0; i < AlgorithmParameters->CountOfClusters; i++) cout << medoids[i] << " ";
						cout << endl;
						for (int i = 0; i < AlgorithmParameters->CountOfClusters; i++) cout << _medoids[i] << " ";
						cout << endl;
						PrintAsMatrix(ResultMatrix, AlgorithmParameters->CountOfClusters, AlgorithmParameters->CountOfClusters*AlgorithmParameters->CountOfObjects);

						copyArray(medoids, _medoids, AlgorithmParameters->CountOfClusters);						
						for (int i = 0; i < AlgorithmParameters->CountOfClusters; i++) cout << medoids[i] << " ";
						cout << endl;
						for (int i = 0; i < AlgorithmParameters->CountOfClusters; i++) cout << _medoids[i] << " ";
						cout << endl;
						PrintAsMatrix(ResultMatrix, AlgorithmParameters->CountOfClusters, AlgorithmParameters->CountOfClusters*AlgorithmParameters->CountOfObjects);

					}
				}

				void SetClusters(int* medoids)
				{
					for (int i = 0; i < AlgorithmParameters->CountOfObjects;i++) {
						for (int j = 0; j < AlgorithmParameters->CountOfClusters; j++) {
							double distance = DistanceMatrix[i*AlgorithmParameters->CountOfObjects + _medoids[j]];							
							double prob = pow(1.0 / distance, 2.0 / (AlgorithmParameters->Fuzzy - 1.0));
							//cout << "DIST FROM " << i << " TO " << _medoids[j] << " IS " << distance << " AND PROB IS "<<prob<< endl;
							ResultMatrix[i*AlgorithmParameters->CountOfClusters + j] = prob;
						}
						normalizeArray(&ResultMatrix[i*AlgorithmParameters->CountOfClusters], AlgorithmParameters->CountOfClusters);
						for(int j=0;j<AlgorithmParameters->CountOfClusters;j++)
						{
							ResultMatrix[_medoids[j] * AlgorithmParameters->CountOfClusters + j] = 1;
						}
					}

				}

				void CalculateMedoids(int* medoids)
				{
					int totalCount = 0;
					for (int m = 0;m < AlgorithmParameters->CountOfClusters;m++)
					{
						double countInCluster=0;
						for (int i = 0;i < AlgorithmParameters->CountOfObjects;i++)
						{
							if((1-ResultMatrix[i*AlgorithmParameters->CountOfClusters+m])<AlgorithmParameters->Epsilon)
							{
								int index = totalCount + countInCluster;
								_objects[index] = i;
								countInCluster++;
							}
						}
						double maxSumDistance = 0.0;
						double objectIndex = medoids[m];
						for(int i=totalCount;i<totalCount+countInCluster;i++)
						{
							double sum = 0.0;
							for(int j=totalCount; j<totalCount+countInCluster;j++)
							{
								sum += DistanceMatrix[_objects[i] * AlgorithmParameters->CountOfObjects + _objects[j]];
							}
							if(sum>maxSumDistance)
							{
								maxSumDistance = sum;
								objectIndex = _objects[i];
							}
						}
						
						medoids[m] = objectIndex;
						totalCount += countInCluster;
					}
				}

				bool calculateDecision(int* medoids)
				{
					for (int j = 0; j < AlgorithmParameters->CountOfClusters; j++)
					{
						cout << "M1=" << _medoids[j] << " M2=" << medoids[j] << endl;
						if (_medoids[j]!=medoids[j])
						{
							return false;
						}
					}
					return true;
				}

			public:

				FuzzyCMedoids(Parameters* algorithm_parameters, DistanceMetric* metric, FileIO fileIO) :Clustering(algorithm_parameters, metric, fileIO)
				{
					ResultMatrix = allocateAlign<double>(AlgorithmParameters->CountOfObjects*AlgorithmParameters->CountOfClusters);
					_powMatrix = allocateAlign<double>(AlgorithmParameters->CountOfObjects*AlgorithmParameters->CountOfClusters);
					_medoids = allocateAlign<int>(AlgorithmParameters->CountOfClusters);
					_objects = allocateAlign<int>(AlgorithmParameters->CountOfObjects);//allocateAlign<int>(AlgorithmParameters->CountOfObjects-AlgorithmParameters->CountOfClusters);
				}

				void StartClustering() override
				{
					GenerateCentroids();
					//GenerateObjects();
					
					int* medoids = allocateAlign<int>(AlgorithmParameters->CountOfClusters);

					copyArray(_medoids, medoids, AlgorithmParameters->CountOfClusters);
					
					GenerateDefaultResultMatrix();
					
					ExecuteClustering(medoids);
				}

				bool TrySaveData() override
				{
					return true;fileIO.tryWriteMatrixToFile(AlgorithmParameters->OutputFilePath, AlgorithmParameters->CountOfObjects, AlgorithmParameters->CountOfClusters, ResultMatrix);
				}

				bool TryGetData() override
				{
					return fileIO.tryReadMatrixFromFile(AlgorithmParameters->InputFilePath, AlgorithmParameters->CountOfObjects, AlgorithmParameters->CountOfDimensions, VectorsForClustering);
				}

				virtual ~FuzzyCMedoids()
				{
					/*freeAlign(ResultMatrix);
					freeAlign(_powMatrix);
					freeAlign(Centroids);*/
				}
			};
		}
	}
}
