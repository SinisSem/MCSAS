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
		/*Внутреннее представление данных-----------------------------------------------------------------*/
	private:


	public:
		CMatrixGenerator();
		~CMatrixGenerator();

		/*Конфигурация генерируемой сети------------------------------------------------------------------*/
	private:
		MGTask<Type>				m_Task;																						/*Конфигурация генерируемой сети*/
		bool						m_HasTask;																					/*Было ли задано задание*/
		std::vector<Chain<Type>>	m_Chains;																					/*Вектор цепочек - !!!!!!! тут реально и живет внутреннее представление сети*/
	public:
		MGTask<Type>				GetTask();																					/*Возвращает текущее задание*/
		std::vector<Chain<Type>>	GetChains() const;																			/*Возвращает внутреннее представление*/
		/*------------------------------------------------------------------------------------------------*/

		/*Генерация внутренних структур-------------------------------------------------------------------*/
	public:
		void						GenerateInternal(MGTask<Type>);
	private:
		void						SetTask(MGTask<Type>);		/*Задает задание*/
		void						CreateChains();
		void						CreateRandomLinks();
		void						CreateEDS();
		/*------------------------------------------------------------------------------------------------*/

		/*Функции-утилиты---------------------------------------------------------------------------------*/
	private:
		Type						GetRandomAdmittance();
		Type						GetRandomEDSAdmittance();
		Type						GetRandomEDS();
		Type						GetRandomAddition();
		size_t						GetNodesNumber();				// Получить число узлов в результирующей матрице
		static std::vector<size_t>	RandomVectorInsert(size_t, size_t);
		void						RandomPairVectorInsert(size_t, std::vector<size_t>&, std::vector<size_t>&);
		/*------------------------------------------------------------------------------------------------*/

		/*Генерация выходных данных из внутренних структур------------------------------------------------*/
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
