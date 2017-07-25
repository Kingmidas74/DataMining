
#include "./Executor.hpp"

using namespace DataMining;

int main(int argc, char *argv[])
{	
	srand(time(nullptr));
	
	ClusterParameters clusterParameters(argc, argv);
	auto parameters = clusterParameters.GetParameters();
	auto fileIO = FileIO(',');
	auto executor = Executor(&parameters, fileIO);
	executor.CalculateProbabilities();
	return EXIT_SUCCESS;
}
