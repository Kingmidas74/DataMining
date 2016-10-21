#pragma once
#include <ctime>


namespace ParallelClustering
{
	using namespace std;

	inline string GetDate()
	{
		/*
		time_t rawtime;
		struct tm timeinfo;
		char buffer[80];

		time(&rawtime);
		localtime_s(&timeinfo, &rawtime);

		strftime(buffer, 80, "%d-%m-%Y;%H:%M:%S", &timeinfo);
		std::string str(buffer);
		*/
		time_t rawtime;
		struct tm * timeinfo;
		char buffer[80];
		time(&rawtime);
		timeinfo = localtime(&rawtime);
		strftime(buffer, 80, "%d-%m-%Y;%H:%M:%S", timeinfo);
		std::cout<<buffer;
		return  buffer;
	}
}
