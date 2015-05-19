#pragma once
#include <vector>

namespace MCSAS
{
namespace Types
{
template<typename Type>
struct COO_matrix
{
	std::vector<Type>		Vals;														// ������ ��������
	std::vector<size_t>		Cols;														// ������ ��������
	std::vector<size_t>		Rows;														// ������ �����

	void					PushElement(Type Val, size_t Row, size_t Col);				// ������ ������� �������� (������ � �����)
	void					SortRowsCols();												// ���������� ��������� ������� �� �������, ����� �� ��������
	size_t					GetRows() const;											// �������� (��������� �� ���������) ����� �����
	size_t					GetCols() const;											// �������� (��������� �� ���������) ����� ��������
};

}//namespace Types
}//namespace MCSAS

#include "detail/COO_matrix.inl"

