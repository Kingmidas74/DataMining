#pragma once

#include <iostream>

namespace ParallelClustering
{
	namespace IO
	{
		using namespace std;

		inline void PrintAsMatrix(double* arr, unsigned int row_length, unsigned int length)
		{

			unsigned int current_size = 0;
			cout << "###################################################################" << endl;
			for (unsigned int i = 0;i < length;i++)
			{
				cout << arr[i] << " ";
				current_size++;
				if (current_size == row_length)
				{
					cout << endl;
					current_size = 0;
				}
			}
			cout << "###################################################################" << endl;
		}
	}

}
