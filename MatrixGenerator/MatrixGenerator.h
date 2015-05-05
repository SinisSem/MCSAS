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
		/*Внутреннее представление данных-----------------------------------------------------------------*/
	private:
		/*Структура, в которой хранится связь - храним только ее ряд, столбец, значение*/
		struct Link
		{
			size_t	RowNumber;
			size_t	ColNumber;
			Type		Value;
		};
		/*Структура, в которой хранятся цепочки со связями*/
		struct Chain
		{
			size_t					StartNode;						// Индекс первого узла в полной матрице цепей
			size_t					NodesNumber;					// Количество узлов в цепи
			std::vector<Link>		Links;							// Вектор нецепочечных связей
			std::vector<Type>		RightVector;					// Правый вектор
			std::vector<Type>		a;								// Нижняя диагональ цепочки
			std::vector<Type>		b;								// Главная диагональ цепочки
			std::vector<Type>		c;								// Верхняя диагональ цепочки
		};
		/*------------------------------------------------------------------------------------------------*/

	public:
		CMatrixGenerator();
		~CMatrixGenerator();

		/*Конфигурация генерируемой сети------------------------------------------------------------------*/
	private:
		MGTask<Type>				m_Task;						/*Конфигурация генерируемой сети*/
		bool						m_HasTask;					/*Было ли задано задание*/
		std::vector<Chain>			m_Chains;					/*Вектор цепочек - !!!!!!! тут реально и живет внутреннее представление сети*/
	public:
		void						SetTask(MGTask<Type>);		/*Задает задание*/
		MGTask<Type>				GetTask();					/*Возвращает текущее задание*/
		/*------------------------------------------------------------------------------------------------*/

	//	/*Генерация внутренних структур-------------------------------------------------------------------*/
	//public:
	//	void						GenerateInternal();
	//private:
	//	void						CreateChains();
	//	void						CreateRandomLinks();
	//	void						CreateEDS();
	//	/*------------------------------------------------------------------------------------------------*/

	//	/*Функции-утилиты---------------------------------------------------------------------------------*/
	//private:
	//	Type						GetRandomAdmittance();
	//	Type						GetRandomEDSAdmittance();
	//	Type						GetRandomEDS();
	//	Type						GetRandomAddition();
	//	size_t						GetNodesNumber();				// Получить число узлов в результирующей матрице
	//	static std::vector<int>		RandomVectorInsert(int, int);
	//	void						RandomPairVectorInsert(int, vector<int>&, vector<int>&);
	//	/*------------------------------------------------------------------------------------------------*/

	//	/*Генерация выходных данных из внутренних структур------------------------------------------------*/
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
