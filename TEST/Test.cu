
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include "../MatrixGenerator/MatrixGenerator.h"
#include "../Types/COO_matrix.h"
#include <vector>
#include "../Types/IO/MatrixMarket.h"

using namespace MCSAS;

int main()
{
	MCSAS::MatrixGenerator::MGTask<double> Task = MCSAS::MatrixGenerator::GetStandardTask<double>(1);
	MCSAS::MatrixGenerator::CMatrixGenerator<double> MG;
	MG.SetTask(Task);
	MCSAS::Types::COO_matrix<double> A = MG.GetCOOMatrix();
	std::vector<double> B = MG.GetRightVector();

	MCSAS::Types::IO::WriteMMFile(A, "mat1.txt");
	MCSAS::Types::IO::WriteMMFile(B, "vec1.txt");

	MCSAS::Types::IO::ReadMMFile(A, "mat0.txt");
	MCSAS::Types::IO::ReadMMFile(B, "vec0.txt");

    return 0;
}