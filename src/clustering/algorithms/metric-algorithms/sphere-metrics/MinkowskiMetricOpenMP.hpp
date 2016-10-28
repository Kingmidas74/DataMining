#pragma once

#include <cmath>

namespace ParallelClustering
{
	namespace Metrics
	{
		class MinkowskiMetricOpenMP :public MinkowskiMetric
		{
		protected:
			
			double calculateWithSqrt(double* first_array, double* second_array, int length) override
			{
				double result = 0.0;
				#pragma omp simd reduction(+:result)
				for (int i = 0; i < length; i++) {
					result += pow(first_array[i] - second_array[i], power);
				}
				result = pow(result, (1.0 / power));
				return result;
			}

			double calculateWithoutSqrt(double* first_array, double* second_array, int length) override
			{
				double result = 0.0;
				#pragma omp simd reduction(+:result)
				for (int i = 0; i < length; i++) {
					result += pow(first_array[i] - second_array[i], power);
				}
				return result;
			}

		public:

			MinkowskiMetricOpenMP() :MinkowskiMetric(){}

			MinkowskiMetricOpenMP(int _power, bool _sqrt) :MinkowskiMetric(_power,_sqrt) {}

			virtual ~MinkowskiMetricOpenMP()
			{
			}
		};
	}
}
