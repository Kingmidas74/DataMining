#pragma once

/*
"Реализация параллельных версиий алгоритов кластеризации данных
с использованием многоядерных ускорителей"

Класс для инкапсуляции работы с кластеризацией данных.
Авторы: Сулейманов Д.Э., Цымблер М.Л.
*/

#include <vector>

namespace ParallelClustering
{
	using namespace std;
	class Executor
	{
	public:
		Executor();
		~Executor();
		vector<vector<double>> CalculateProbabilities();

	private:
		vector<vector<double>> _normalizeObjects;
	};

	
}