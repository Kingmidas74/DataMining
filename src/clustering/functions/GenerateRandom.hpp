#pragma once



namespace ParallelClustering
{
	using namespace std;

	inline double GetRandomDouble(unsigned int from=0, unsigned int to=1)
	{
		double random = static_cast<double>(rand()) / RAND_MAX;
		return from + random * (to - from);
	}
}