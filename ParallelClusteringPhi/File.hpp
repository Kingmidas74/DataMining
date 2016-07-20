#pragma once

#include<iostream>

namespace ParallelClustering {
	
	using namespace std;

	class File
	{
	private:
		
	public:

		string PathToFile;

		File(string path)
		{
			PathToFile = path;
		}



		~File()
		{
		}
	};

}