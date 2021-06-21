#pragma once

namespace DataMining
{
	namespace IO
	{
		using namespace std;

		template<typename Type>
		inline void PrintAsMatrix(Type* arr, int row_length, int length)
		{
			cout << "###################################################################" << endl;
			for (int i = 0;i < length;i++)
			{
				for(int j=0;j<row_length;j++)
				{
					cout << arr[i*row_length+j] << " ";
				}
				cout<<endl;
			}
			cout << "###################################################################" << endl;
		}
	}

}
