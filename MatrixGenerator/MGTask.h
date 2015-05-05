#pragma once
#include <vector>

namespace MCSAS
{
namespace MatrixGenerator
{
	/* ������� ��� ��������� ������� ������������� */
	template<typename Type>
	struct MGTask
	{
		std::vector<size_t>			ChainNodesNumber;				// ������� ����� ����� � ������ �������
		size_t						RandomNetAdmittancesNumber;		// ���������� ��������� ������
		Type						BaseAdmittance;					// ������� ������������
		Type						AdmittancesDispersion;			// ��������� ������������� ������������ ������� (������������ �������)
		Type						BaseMainDiagonalAddition;		// ������� ������� � ������� ��������� (������������ �����)
		Type						AdditionDispersion;				// ��������� ������� � ������� ��������� (������������ �������)
		Type						AdditionsNumber;				// ���������� ����� � �������� � ������� ��������� (������������ ����� �����)
		Type						EDSBase;						// ������� ���
		Type						EDSDispersion;					// ��������� ���
		double						EDSNumber;						// ����� ����� � ��� (������������ ����� �����)
		Type						EDSAdmittanceBase;				// ������� ������������ ���
		Type						EDSAdmittanceDispersion;		// ��������� ������������� ���
//		bool						CheckConductivity;				// ��������� �� �������� ������ ��������� ��������� �����
		size_t						MaximumLinksPerChain;			// ���������� ����� ������ �� ����
	};
	template<typename Type>
	MGTask<Type>	GetStandardTask(size_t Number);
}
}