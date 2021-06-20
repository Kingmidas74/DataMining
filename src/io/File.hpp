#pragma once

namespace DataMining
{
	namespace IO
	{
		using namespace std;

		class FileIO
		{
		private:
			char delimeter;
		public:
			FileIO() :delimeter(','){}
			explicit FileIO(char _delimenter) :delimeter(_delimenter) {}
			~FileIO() {}

			template <typename Type>
			bool tryReadMatrixFromFile(string filePath, int rowCount, int elementsByRow, Type* target)
			{
				ifstream infile(filePath);
				if (infile.is_open())
				{
					int currentRow = 0;
					int elementN = 0;

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

			template <typename Type>
			bool tryWriteMatrixToFile(string filePath, int rowCount, int elementsByRow, Type* source)
			{
				for (int i = 0; i < rowCount; i++)
				{
					for (int j = 0; j < elementsByRow; j++)
					{
						cout << source[i*elementsByRow + j];
						if (j != elementsByRow - 1) { cout << delimeter; }
					}
					cout << endl;
				}
				return true;
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
