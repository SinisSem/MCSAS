#pragma once
#include "MGTask.h"
#include "../Types/COO_matrix.h"
#include <vector>
#include "InternalTypes.h"

namespace MCSAS
{

namespace MatrixGenerator
{
	template<typename Type>
	class CMatrixGenerator
	{
		/*���������� ������������� ������-----------------------------------------------------------------*/
	private:


	public:
		CMatrixGenerator();
		~CMatrixGenerator();

		/*������������ ������������ ����------------------------------------------------------------------*/
	private:
		MGTask<Type>				m_Task;																						/*������������ ������������ ����*/
		bool						m_HasTask;																					/*���� �� ������ �������*/
		std::vector<Chain<Type>>	m_Chains;																					/*������ ������� - !!!!!!! ��� ������� � ����� ���������� ������������� ����*/
	public:
		MGTask<Type>				GetTask();																					/*���������� ������� �������*/
		std::vector<Chain<Type>>	GetChains() const;																			/*���������� ���������� �������������*/
		/*------------------------------------------------------------------------------------------------*/

		/*��������� ���������� ��������-------------------------------------------------------------------*/
	public:
		void						GenerateInternal(MGTask<Type>);
	private:
		void						SetTask(MGTask<Type>);		/*������ �������*/
		void						CreateChains();
		void						CreateRandomLinks();
		void						CreateEDS();
		/*------------------------------------------------------------------------------------------------*/

		/*�������-�������---------------------------------------------------------------------------------*/
	private:
		Type						GetRandomAdmittance();
		Type						GetRandomEDSAdmittance();
		Type						GetRandomEDS();
		Type						GetRandomAddition();
		size_t						GetNodesNumber();				// �������� ����� ����� � �������������� �������
		static std::vector<size_t>	RandomVectorInsert(size_t, size_t);
		void						RandomPairVectorInsert(size_t, std::vector<size_t>&, std::vector<size_t>&);
		/*------------------------------------------------------------------------------------------------*/

		/*��������� �������� ������ �� ���������� ��������------------------------------------------------*/
	public:
		Types::COO_matrix<Type>		GetCOOMatrix();
		Types::COO_matrix<Type>		GetCOOMatrix(size_t);
		std::vector<Type>			GetRightVector();
		std::vector<Type>			GetRightVector(size_t);
		Types::COO_matrix<Type>		GetCOOTriDiagMatrix();
		Types::COO_matrix<Type>		GetCOOTriDiagMatrix(size_t);
		/*------------------------------------------------------------------------------------------------*/
	};

}
}
