#pragma once
#include <cmath>
#include "Prints.hpp"
#include <cassert>


namespace ParallelClustering
{

	namespace Metrics {

		using namespace std;

		inline double MinkowskiSquare(double* first_array, double* second_array, unsigned int length, unsigned int power, unsigned int length2) {
			//cout << "AA=" << first_array[0] << "_" << first_array[1] <<  endl;
			//cout << "BB=" << second_array[0] << "_" << second_array[1]  << endl;
			assert(length == length2);
			double result = 0;
			for (unsigned int i = 0; i < length; i++) {
				result += pow(first_array[i] - second_array[i], power);
			}
			//result = pow(result, 1.0 / power);
			return result;
		}
	}
}
