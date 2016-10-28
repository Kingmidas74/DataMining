#pragma once

#include <cmath>

namespace ParallelClustering
{
	namespace Metrics
	{
		class DynamicTimeWarpingOpenMP :public DynamicTimeWarping
		{
		protected:

			void calculateDistanceMatrix(double* first_array, double* second_array) override
			{
				#pragma omp parallel for
				for (int i = 0; i<length;i++)
				{
					#pragma omp simd
					for (int j = 0;j<length;j++)
					{
						distanceMatrix[i*length + j] = abs(first_array[j] - second_array[i]);
					}
				}
			}

		public:

			DynamicTimeWarpingOpenMP() :DynamicTimeWarping() {}

			double CalculateDistance(double* first_array, double* second_array, int _length) override
			{
				allocateIfNot(_length);

				calculateDistanceMatrix(first_array, second_array);

				//resultMatrix[0] = distanceMatrix[0];
				#pragma omp simd
				for (int i = 1;i<length;i++)
				{
					distanceMatrix[i] += distanceMatrix[i - 1];
				}

				#pragma omp simd
				for (int i = 1;i<length;i++)
				{
					distanceMatrix[i*length] += distanceMatrix[(i - 1)*length];
				}

				for (int i = 1;i < length;i++)
				{
					#pragma omp simd 
					for (int j = 1;j < length;j++)
					{
						distanceMatrix[i*length + j] += min(
							distanceMatrix[(i - 1)*length + j],
							distanceMatrix[i*length + (j - 1)],
							distanceMatrix[(i - 1)*length + (j - 1)]
						);
					}
				}
				return distanceMatrix[length*length - 1];
			};

			virtual ~DynamicTimeWarpingOpenMP()
			{
			}
		};
	}
}
