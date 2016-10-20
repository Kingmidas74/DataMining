#pragma once

namespace ParallelClustering
{
	namespace Metrics
	{
		class DistanceMetric
		{

		public:
			DistanceMetric() {};

			virtual double CalculateDistance(double* first_array, double* second_array, unsigned int length) { cout << "DM" << endl; return 0.0; };

			virtual ~DistanceMetric()
			{
			}
		};
	}
}
