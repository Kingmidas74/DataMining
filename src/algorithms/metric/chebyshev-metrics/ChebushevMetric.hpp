#pragma once

namespace DataMining
{
	namespace Metrics
	{
		class ChebyshevMetric :public DistanceMetric
		{
		public:

			ChebyshevMetric() :DistanceMetric() {}

			double CalculateDistance(double* first_array, double* second_array, int length) override
			{
				double result=0.0;
				for(int i=0;i<length;i++)
				{
					double tmp=abs(first_array[i] - second_array[i]);
					if (tmp > result) result = tmp;
				}
				return result;
			};

			virtual ~ChebyshevMetric()
			{
			}
		};
	}
}
