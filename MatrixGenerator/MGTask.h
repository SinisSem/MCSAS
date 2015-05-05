#pragma once
#include <vector>

namespace MCSAS
{
namespace MatrixGenerator
{
	/* Задание для генерации матрицы проводимостей */
	template<typename Type>
	struct MGTask
	{
		std::vector<size_t>			ChainNodesNumber;				// Сколько узлов будет в каждой цепочке
		size_t						RandomNetAdmittancesNumber;		// Количество случайных связей
		Type						BaseAdmittance;					// Базовая проводимость
		Type						AdmittancesDispersion;			// Дисперсия проводимостей относительно базовой (относительно базовой)
		Type						BaseMainDiagonalAddition;		// Базовая добавка к главной диагонали (проводимости шунта)
		Type						AdditionDispersion;				// Дисперсия добавок к главной диагонали (относительно базовой)
		Type						AdditionsNumber;				// Количество узлов с добавкой к главной диагонали (относительно числа узлов)
		Type						EDSBase;						// Базовая ЭДС
		Type						EDSDispersion;					// Дисперсия ЭДС
		double						EDSNumber;						// Число узлов с ЭДС (относительно числа узлов)
		Type						EDSAdmittanceBase;				// Базовая проводимость ЭДС
		Type						EDSAdmittanceDispersion;		// Дисперсия проводимостей ЭДС
//		bool						CheckConductivity;				// Выполнять ли проверку полной связности итогового графа
		size_t						MaximumLinksPerChain;			// Наибольшее число связей на цепь
	};
	template<typename Type>
	MGTask<Type>	GetStandardTask(size_t Number);
}
}