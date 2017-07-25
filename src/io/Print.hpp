#pragma once

namespace DataMining
{
	namespace IO
	{
		using namespace std;

		inline void PrintAsMatrix(double* arr, int row_length, int length)
		{

			int current_size = 0;
			cout << "###################################################################" << endl;
			for (int i = 0;i < length;i++)
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
