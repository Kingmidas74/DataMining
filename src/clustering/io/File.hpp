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

			bool tryReadMatrixFromFile(string filePath, int rowCount, int elementsByRow, double* target)
			{
				ifstream infile(filePath);
				int currentRow = 0;
				int elementN = 0;
				if (infile.is_open())
				{
					while (infile && currentRow < rowCount)
					{
						string rowString;
						if (!getline(infile, rowString)) break;

						istringstream rowStream(rowString);

						int elementInRow = 0;
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

			bool tryWriteMatrixToFile(string filePath, int rowCount, int elementsByRow, double* source)
			{
				ofstream offile(filePath);
				if (offile.is_open())
				{
					for (int i = 0; i < rowCount; i++)
					{
						for (int j = 0; j < elementsByRow; j++)
						{
							offile << source[i*elementsByRow + j];
							if (j != elementsByRow - 1) { offile << delimeter; }
						}
						offile << endl;
					}
					offile.close();
					return true;
				}
				return false;
			}

			bool tryAppendStringRowToFile(string filePath, int rowCount, int elementsByRow, string* source)
			{
				ofstream offile(filePath, ios::out | ios::app);
				if(offile.is_open())
				{
					for (int i = 0; i < rowCount; i++)
					{
						for (int j = 0; j < elementsByRow; j++)
						{
							offile << source[i*elementsByRow + j];
							if (j != elementsByRow - 1) { offile << delimeter; }
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
