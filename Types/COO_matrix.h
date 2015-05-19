#pragma once
#include <vector>

namespace MCSAS
{
namespace Types
{
template<typename Type>
struct COO_matrix
{
	std::vector<Type>		Vals;														// Вектор значений
	std::vector<size_t>		Cols;														// Вектор столбцов
	std::vector<size_t>		Rows;														// Вектор строк

	void					PushElement(Type Val, size_t Row, size_t Col);				// Грубая вставка элемента (просто в конец)
	void					SortRowsCols();												// Сортировка элементов сначала по строкам, потом по столбцам
	size_t					GetRows() const;											// Реальное (проверено из элементов) число рядов
	size_t					GetCols() const;											// Реальное (проверено из элементов) число столбцов
};

}//namespace Types
}//namespace MCSAS

#include "detail/COO_matrix.inl"

