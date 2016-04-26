#pragma once

/*
"Реализация параллельных версиий алгоритов кластеризации данных
с использованием многоядерных ускорителей"

Вспомогательные функции.
Авторы: Сулейманов Д.Э., Цымблер М.Л.
*/

#include <vector>
#include <iostream>
#include "CustomObject.h"

namespace ParallelClustering {
	double GetRandomDouble(double min = 0, double max = 1);
	void PrintObjects(vector<CustomObject*> objects);
}
