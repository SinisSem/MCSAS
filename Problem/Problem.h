#pragma once
#include "../Types/COO_matrix.h"
#include "../MatrixGenerator/InternalTypes.h"

#include "cuda_runtime.h"
#include "device_launch_parameters.h"

namespace MCSAS
{
	namespace Problem
	{
		template<typename Type>
		class CProblem
		{
		private:

			cudaDeviceProp DevProp;

			std::vector<Chain<Type>> m_Chains;

		public:
			CProblem();
			~CProblem();

		public:
			void FormulateProblem(Types::COO_matrix<Type> Matrix, std::vector<Type> RightVector);
			void FormulateProblem(Chain<Type> Chains);
		};
	}
}
