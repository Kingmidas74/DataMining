#pragma once

#include <cmath>

namespace ParallelClustering
{
	namespace Metrics
	{
		class DynamicTimeWarping :public DistanceMetric
		{
		protected:
			double* distanceMatrix;
			int		length;


			void allocateIfNot(int _length)
			{
				if(length!=_length)
				{
					if (length == 0)
					{
						distanceMatrix = allocateAlign<double>(_length*_length);
					}
					else
					{
						distanceMatrix=reAllocateAlign<double>(distanceMatrix, _length*_length);				
					}
					length = _length;
				}
			}

			void calculateDistanceMatrix(double* first_array, double* second_array)
			{
				for(int i=0; i<length;i++)
				{
					for(int j=0;j<length;j++)
					{
						distanceMatrix[i*length + j] = abs(first_array[j]-second_array[i]);						
					}
				}
			}

			double min(double a, double b)
			{
				if (a < b) return a;
				return b;
			}

			double min(double a, double b, double c)
			{
				return min(a, min(b, c));
			}

		public:

			DynamicTimeWarping() :DistanceMetric(), distanceMatrix(nullptr), length(0) {}

			double CalculateDistance(double* first_array, double* second_array, int _length) override
			{
				allocateIfNot(_length);
				
				calculateDistanceMatrix(first_array,second_array);

				//resultMatrix[0] = distanceMatrix[0];
				for(int i=1;i<length;i++)
				{
					distanceMatrix[i] += distanceMatrix[i-1];
					distanceMatrix[i*length] += distanceMatrix[(i - 1)*length];
				}

				for (int i = 1;i < length;i++)
				{
					for (int j = 1;j < length;j++)
					{
						distanceMatrix[i*length + j] += min(
							distanceMatrix[(i - 1)*length + j],
							distanceMatrix[i*length + (j - 1)],
							distanceMatrix[(i - 1)*length + (j - 1)]
						);
					}
				}
				return distanceMatrix[length*length-1];
			};

			virtual ~DynamicTimeWarping()
			{
			}
		};
	}
}
