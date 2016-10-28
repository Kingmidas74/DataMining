#pragma once



namespace ParallelClustering
{
	using namespace std;

	inline double GetRandomDouble(int from=0, int to=10)
	{
		double random = static_cast<double>(rand()) / RAND_MAX;
		return from + random * (to - from);
	}

	inline int GetRandomInt(int from = 0, int to = 10)
	{
		int random = from + (rand() % (int)(to - from + 1));
		return random;
	}
}