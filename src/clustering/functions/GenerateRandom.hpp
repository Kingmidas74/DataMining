#pragma once



namespace ParallelClustering
{
	using namespace std;

	inline double GetRandomDouble(int from=0, int to=10)
	{
		double random = static_cast<double>(rand()) / RAND_MAX;
		return from + random * (to - from);
	}
}