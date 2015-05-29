#pragma once
#include <vector>
#include "../Types/COO_matrix.h"

namespace MCSAS
{
	namespace MatrixGenerator
	{
		/*Структура, в которой хранится связь - храним только ее ряд, столбец, значение*/
		template<typename Type>
		struct Link
		{
			size_t		RowNumber;
			size_t		ColNumber;
			Type		Value;
		};
		/*Структура, в которой хранятся цепочки со связями*/
		template<typename Type>
		struct Chain
		{
			size_t					StartNode;						// Индекс первого узла в полной матрице цепей
			size_t					NodesNumber;					// Количество узлов в цепи
			std::vector<Link<Type>>	Links;							// Вектор нецепочечных связей
			std::vector<Type>		RightVector;					// Правый вектор
			std::vector<Type>		a;								// Нижняя диагональ цепочки
			std::vector<Type>		b;								// Главная диагональ цепочки
			std::vector<Type>		c;								// Верхняя диагональ цепочки
		};
		/*------------------------------------------------------------------------------------------------*/
		template<typename Type>
		std::vector<Chain<Type>>	GetChains(const MCSAS::Types::COO_matrix<Type>, const std::vector<Type>, const size_t);		/*Возвращает внутреннее представление из правого вектора и матрицы*/
	}
}