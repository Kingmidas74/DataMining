#pragma once

#include <iostream>
#include <fstream>
#include <sstream>

namespace ParallelClustering
{
	namespace IO
	{
		using namespace std;

		class FileIO
		{
		private:
			char delimeter;
		public:
			FileIO() :delimeter(';'){}
			FileIO(char _delimenter) :delimeter(_delimenter) {}
			~FileIO() {}

			bool tryReadMatrixFromFile(string filePath, unsigned int rowCount, unsigned int elementsByRow, double* target)
			{
				ifstream infile(filePath);
				unsigned int currentRow = 0;
				unsigned int elementN = 0;
				if (infile.is_open())
				{
					while (infile && currentRow < rowCount)
					{
						string rowString;
						if (!getline(infile, rowString)) break;

						istringstream rowStream(rowString);

						unsigned int elementInRow = 0;
						while (rowStream && elementInRow < elementsByRow)
						{

							string elementString;
							double elementValue;
							if (!getline(rowStream, elementString, delimeter)) break;

							istringstream elementStream(elementString);
							if (elementStream >> elementValue)
							{
								target[elementN] = elementValue;
								elementInRow++;
								elementN++;
							}
						}
						currentRow++;
					}
					infile.close();
					return true;
				}
				return false;
			}

			bool tryWriteMatrixToFile(string filePath, unsigned int rowCount, unsigned int elementsByRow, double* source)
			{
				ofstream offile(filePath);
				if (offile.is_open())
				{
					for (unsigned int i = 0; i < rowCount; i++)
					{
						for (unsigned j = 0; j < elementsByRow; j++)
						{
							offile << source[i*elementsByRow + j] << delimeter;
						}
						offile << endl;
					}
					offile.close();
					return true;
				}
				return false;
			}
		};
	}
}
