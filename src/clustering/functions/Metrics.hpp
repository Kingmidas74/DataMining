#pragma once
#include <cmath>
#include "Prints.hpp"
#include <cassert>


namespace ParallelClustering
{

	namespace Metrics {

		using namespace std;

		double* tempMatrix;

		inline double min(double a, double b)
		{
			if(a<=b)
			{
				return a;
			}
			return b;
		}

		inline double MinkowskiSquare(double* first_array, double* second_array, unsigned int length, unsigned int power) {
			double result = 0.0;
			for (unsigned int i = 0; i < length; i++) {
				result += pow(first_array[i] - second_array[i], 2);
			}
			result = sqrt(result);
			//result = pow(result, 1.0 / power);
			return result;
		}

		inline double DTW(double* first_array, double* second_array, unsigned int first_length, unsigned int second_length) {
			if(tempMatrix==nullptr)
			{
				tempMatrix = allocateAlign<double>(2*first_length*first_length);
			}
			for(unsigned int i=0;i<first_length;i++)
			{
				for(unsigned int j=0;j<first_length;j++)
				{
					tempMatrix[i*first_length + j] = abs(first_array[i] - second_array[j]);
				}
			}
			//for (unsigned int i = 1;i<first_length;i++)
			//{
			//	for (unsigned int j = 1;j<first_length;j++)
			//	{
			//		//tempMatrix[i*first_length + j + (first_length*first_length)] = tempMatrix[i*first_length + j]+min(min(tempMatrix[(i-1)*length + j + (length*length)], tempMatrix[(i-1)*first_length + (j-1) + (first_length*first_length)]), tempMatrix[i*first_length + (j-1) + (first_length*first_length)]);
			//	}
			//}


			return tempMatrix[first_length*first_length *2];
		}
	}
}
