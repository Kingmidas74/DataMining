#include "MetricsDistance.h"

/*
"Реализация параллельных версиий алгоритов кластеризации данных
с использованием многоядерных ускорителей"

Пространство имён содержащие в себе различные метрики для вычисления расстояния
между двумя векторами.
Авторы: Сулейманов Д.Э., Цымблер М.Л.
*/
namespace ParallelClustering
{
	namespace Metrics 
	{
		
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

		double EvklidArray(double* first_data, long long first_data_size, double* second_data, long long second_data_size)
		{
			double result = 0;
			long long fds=first_data_size;
			long long sds=second_data_size;
			while (first_data_size > sds) {
				second_data[sds]=0;
				sds++;
			}
			while (fds < second_data_size) {
				first_data[fds]=0;
				fds++;
			}
			long long countOfData = first_data_size;
			for (int i = 0; i < countOfData; i++) {
				result += pow(first_data[i] - second_data[i], 2);
			}
			return result;
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
}
