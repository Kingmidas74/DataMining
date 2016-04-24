#include "MetricsDistance.h"



namespace Metrics {
	function<double(vector<double>, vector<double>)> GetMetrics(MetricsDistanceTypes type) {
		if (type == MetricsDistanceTypes::Evklid) {
			return EvklidDistance;
		}
		return CustomDistance;
	}

	double EvklidDistance(vector<double> first_data, vector<double> second_data) {
		double result = 0;
		while (first_data.size() > second_data.size()) {
			second_data.push_back(0);
		}
		while (first_data.size() < second_data.size()) {
			first_data.push_back(0);
		}
		int countOfData = first_data.size();
		
		for (int i = 0; i < countOfData; i++) {			
			double diff = first_data[i] - second_data[i];
			double power = pow(diff, 2);
			result += power;
		}
		return sqrt(result);

	}

	double CustomDistance(vector<double> first_data, vector<double> second_data) {
		double result = 0;
		int countOfData = first_data.size();
		for (int i = 0; i < countOfData; i++) {
			result += (first_data[i] * second_data[i]);
		}
		return result;
	}
}