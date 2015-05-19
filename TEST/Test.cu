
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include "../MatrixGenerator/MatrixGenerator.h"
#include "../Types/COO_matrix.h"
#include <vector>
#include "../Types/IO/MatrixMarket.h"
#include "../Problem/Problem.h"
#include <iostream>

using namespace MCSAS;

int main()
{
//	MCSAS::MatrixGenerator::MGTask<double> Task = MCSAS::MatrixGenerator::GetStandardTask<double>(1);
//	MCSAS::MatrixGenerator::CMatrixGenerator<double> MG;
//	MG.GenerateInternal(Task);
	try{
		MCSAS::Types::COO_matrix<double> A;// = MG.GetCOOMatrix();
		std::vector<double> B;// = MG.GetRightVector();

		MCSAS::Types::IO::ReadMMFile(A, "mat0.txt");
		MCSAS::Types::IO::ReadMMFile(B, "vec0.txt");

		MCSAS::Problem::CProblem<double> Problem;
	
		Problem.FormulateProblem(A, B);
	}
	catch(MCSAS::MCSASException Ex)
	{
		std::cout<<Ex.what(); 
	}

    return 0;
}