#pragma once
#include "../Types/COO_matrix.h"
#include "../MatrixGenerator/InternalTypes.h"
#include "../Types/CUDATypes/Array1D.h"
#include "../Types/CUDATypes/Array2D.h"
#include "ProblemTypes.h"

#include "cuda_runtime.h"
#include "device_launch_parameters.h"

namespace MCSAS
{
	namespace Problem
	{
		template<typename Type>
		class CProblemReal
		{
		private:
			ProblemData<Type>	Data;

			// Данные для распределения памяти
		private:

			cudaDeviceProp DevProp;


		public:
			CProblemReal();
			~CProblemReal();

		public:
			ProblemData<Type>	GetProblemData();
			void				FormulateProblem(Types::COO_matrix<Type> Matrix, std::vector<Type> RightVector);
			void				FormulateProblem(std::vector<MatrixGenerator::Chain<Type>> Chains);
			void				Clear();

		private:
			void				GetChainsLinksInBlocks(const std::vector<MatrixGenerator::Chain<Type>>, std::vector<size_t>&, std::vector<size_t>&);
			std::vector<size_t>	GetRightsInBlocks(const std::vector<MatrixGenerator::Chain<Type>>, const std::vector<size_t>);
			std::vector<size_t>	GetDVectorsInBlocks(const std::vector<MatrixGenerator::Chain<Type>>, const std::vector<size_t>);

			void				MallocCpyDiag(const std::vector<MatrixGenerator::Chain<Type>>);
			void				MallocCpyLink(const std::vector<MatrixGenerator::Chain<Type>>, const std::vector<size_t>, const std::vector<size_t>);
			void				MallocCpyRight(const std::vector<MatrixGenerator::Chain<Type>>, const std::vector<size_t>, const std::vector<size_t>);
			void				MallocCpyVectorD(const std::vector<MatrixGenerator::Chain<Type>>, const std::vector<size_t>, const std::vector<size_t>);
			void				MallocCpyChainsInBlocks(const std::vector<MatrixGenerator::Chain<Type>>, const std::vector<size_t>);
			void				MallocDenseMatrix(const std::vector<MatrixGenerator::Chain<Type>>);
		};
	}
}
