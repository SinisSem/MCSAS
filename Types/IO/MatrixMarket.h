#pragma once

#include "../BaseTypes.h"
#include "BaseTypesIO.h"
#include "../COO_matrix.h"
#include <string>
#include <vector>

namespace MCSAS
{
	namespace Types
	{
		namespace IO
		{
			template<typename Type>
			void WriteMMFile(COO_matrix<Type> Matr, std::string filename);		// ������ ������� � ����

			template<typename Type>
			void WriteMMFile(std::vector<Type> Vector, std::string filename);	// ������ ������� � ����

			template<typename Type>
			void ReadMMFile(COO_matrix<Type> Matr, std::string filename);		// ���������� ������� �� �����

			template<typename Type>
			void ReadMMFile(std::vector<Type> Vector, std::string filename);	// ���������� ������� �� �����
		}
}
}

#include "detail/MatrixMarket.inl"