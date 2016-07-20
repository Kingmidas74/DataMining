#pragma once

/*
"Реализация параллельных версиий алгоритов кластеризации данных
с использованием многоядерных ускорителей"

Пространство имён содержащие в себе различные метрики для вычисления расстояния
между двумя векторами.
Авторы: Сулейманов Д.Э., Цымблер М.Л.
*/

#include <cmath>

namespace ParallelClustering
{

	namespace Metrics {

		using namespace std;

		template <typename Type>
		double Minkowski(Type* first_array, Type* second_array, long size, int power) {
			double result = 0;
			for (long i = 0; i < size; i++) {
				result += pow(first_array[i] - second_array[i], power);
			}
			return pow(result, 1.0 / power);
		}

		template <typename Type>
		double EuclidianSquare(Type* first_array, Type* second_array, long size) {
			double result = 0;
			for (long i = 0; i < size; i++) {
				result += pow(first_array[i] - second_array[i], 2);
			}
			return result;
		}

	}
}
