#pragma once

/*
"Реализация параллельных версиий алгоритов кластеризации данных
с использованием многоядерных ускорителей"

Класс для отображения объекта сериализации.
Авторы: Сулейманов Д.Э., Цымблер М.Л.
*/

#include <vector>
#include <map>

namespace ParallelClustering {

	using namespace std;

	class CustomObject
	{
	public:		
		CustomObject();
		~CustomObject();
	};
}
