#pragma once
#include "MGTask.h"
#include <vector>

namespace MCSAS
{

namespace MatrixGenerator
{
	template<typename Type>
	class CMatrixGenerator
	{
		/*���������� ������������� ������-----------------------------------------------------------------*/
	private:
		/*���������, � ������� �������� ����� - ������ ������ �� ���, �������, ��������*/
		struct Link
		{
			size_t	RowNumber;
			size_t	ColNumber;
			Type		Value;
		};
		/*���������, � ������� �������� ������� �� �������*/
		struct Chain
		{
			size_t					StartNode;						// ������ ������� ���� � ������ ������� �����
			size_t					NodesNumber;					// ���������� ����� � ����
			std::vector<Link>		Links;							// ������ ������������ ������
			std::vector<Type>		RightVector;					// ������ ������
			std::vector<Type>		a;								// ������ ��������� �������
			std::vector<Type>		b;								// ������� ��������� �������
			std::vector<Type>		c;								// ������� ��������� �������
		};
		/*------------------------------------------------------------------------------------------------*/

	public:
		CMatrixGenerator();
		~CMatrixGenerator();

		/*������������ ������������ ����------------------------------------------------------------------*/
	private:
		MGTask<Type>				m_Task;						/*������������ ������������ ����*/
		bool						m_HasTask;					/*���� �� ������ �������*/
		std::vector<Chain>			m_Chains;					/*������ ������� - !!!!!!! ��� ������� � ����� ���������� ������������� ����*/
	public:
		void						SetTask(MGTask<Type>);		/*������ �������*/
		MGTask<Type>				GetTask();					/*���������� ������� �������*/
		/*------------------------------------------------------------------------------------------------*/

	//	/*��������� ���������� ��������-------------------------------------------------------------------*/
	//public:
	//	void						GenerateInternal();
	//private:
	//	void						CreateChains();
	//	void						CreateRandomLinks();
	//	void						CreateEDS();
	//	/*------------------------------------------------------------------------------------------------*/

	//	/*�������-�������---------------------------------------------------------------------------------*/
	//private:
	//	Type						GetRandomAdmittance();
	//	Type						GetRandomEDSAdmittance();
	//	Type						GetRandomEDS();
	//	Type						GetRandomAddition();
	//	size_t						GetNodesNumber();				// �������� ����� ����� � �������������� �������
	//	static std::vector<int>		RandomVectorInsert(int, int);
	//	void						RandomPairVectorInsert(int, vector<int>&, vector<int>&);
	//	/*------------------------------------------------------------------------------------------------*/

	//	/*��������� �������� ������ �� ���������� ��������------------------------------------------------*/
	//public:
	//	COO_matrix					GetCOOMatrix();
	//	COO_matrix					GetCOOMatrix(size_t);
	//	std::vector<double>			GetRightVector();
	//	std::vector<double>			GetRightVector(size_t);
	//	COO_matrix					GetCOOTriDiagMatrix();
	//	COO_matrix					GetCOOTriDiagMatrix(size_t);
	//	/*------------------------------------------------------------------------------------------------*/
	};
}
}
