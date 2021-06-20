#pragma once

namespace DataMining
{
	namespace Metrics
	{
		class MinkowskiMetric:public DistanceMetric
		{
		protected:
			int power;
			bool sqrt;

			#pragma omp declare simd uniform(first_array, second_array, length)
			virtual double calculateWithSqrt(double* first_array, double* second_array, int length)
			{
				double result = 0.0;
				for (int i = 0; i < length; i++) {
					result += pow(first_array[i] - second_array[i], power);
				}
				result = pow(result, (1.0 / power));
				return result;
			}


			#pragma omp declare simd uniform(first_array, second_array, length)
			virtual double calculateWithoutSqrt(double* first_array, double* second_array, int length)
			{
				double result = 0.0;
				for (int i = 0; i < length; i++) {
					result += pow(first_array[i] - second_array[i], power);
				}
				return result;
			}

		public:

			MinkowskiMetric():DistanceMetric(),power(1),sqrt(false) {}

			MinkowskiMetric(int _power, bool _sqrt) :DistanceMetric(), power(_power), sqrt(_sqrt) {}

			#pragma omp declare simd uniform(first_array, second_array, length)
			double CalculateDistance(double* first_array, double* second_array, int length) override
			{
				double result;
				if(sqrt)
				{
					result = calculateWithSqrt(first_array, second_array, length);
				}
				else
				{
					result = calculateWithoutSqrt(first_array, second_array, length);
				}
				return result;
			};

			virtual ~MinkowskiMetric()
			{
			}
		};
	}
}
