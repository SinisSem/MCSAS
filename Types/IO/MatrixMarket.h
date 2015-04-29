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
			void WriteMMFile(COO_matrix<Type> Matr, std::string filename);		// Запись матрицы в файл

			template<typename Type>
			void WriteMMFile(std::vector<Type> Vector, std::string filename);	// Запись вектора в файл

			template<typename Type>
			void ReadMMFile(COO_matrix<Type> Matr, std::string filename);		// Считывание матрицы из файла

			template<typename Type>
			void ReadMMFile(std::vector<Type> Vector, std::string filename);	// Считывание вектора из файла
		}
}
}

#include "detail/MatrixMarket.inl"