#pragma once

namespace DataMining
{
	namespace Metrics
	{
		class DistanceMetric
		{

		public:
			DistanceMetric() {};

			virtual double CalculateDistance(double* first_array, double* second_array, int length) { return 0.0; };

			virtual void CalculateDistanceMatrix(double* objects, size_t objects_count, size_t object_dimensions, double* distance_matrix)
			{
				for(size_t i=0; i<objects_count;i++)
				{
					for(size_t j=0;j<objects_count; j++)
					{
						distance_matrix[i*objects_count+j] = this->CalculateDistance(&objects[i*object_dimensions],&objects[j*object_dimensions],object_dimensions);
					}
				}
			}
			
			
			virtual ~DistanceMetric()
			{
			}
		};
	}
}
