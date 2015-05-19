#pragma once
#include <vector>

namespace MCSAS
{
	/*���������, � ������� �������� ����� - ������ ������ �� ���, �������, ��������*/
	template<typename Type>
	struct Link
	{
		size_t		RowNumber;
		size_t		ColNumber;
		Type		Value;
	};
	/*���������, � ������� �������� ������� �� �������*/
	template<typename Type>
	struct Chain
	{
		size_t					StartNode;						// ������ ������� ���� � ������ ������� �����
		size_t					NodesNumber;					// ���������� ����� � ����
		std::vector<Link<Type>>	Links;							// ������ ������������ ������
		std::vector<Type>		RightVector;					// ������ ������
		std::vector<Type>		a;								// ������ ��������� �������
		std::vector<Type>		b;								// ������� ��������� �������
		std::vector<Type>		c;								// ������� ��������� �������
	};
	/*------------------------------------------------------------------------------------------------*/
}