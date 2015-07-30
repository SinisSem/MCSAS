#pragma once
#include "../Problem/ProblemTypes.h"

namespace MCSAS
{
	namespace Solver
	{
		template<typename Type>
		class CSolverReal
		{
		private:
			Problem::ProblemData<Type> Data;
		public:
			CSolverReal(Problem::ProblemData Data_){Data = Data_;}
			~CSolverReal(){}
			void Test1();
		};
	}
}