#ifndef __PROBLEM_DATA__SINIS__
#define __PROBLEM_DATA__SINIS__
#include "Problem_Utils.h"

/*��������� ���� ������, ������������ ���������*/
enum ProblemDataError
{
	ProblemDataSuccess = 0,				/*������������, ���� ������� ��������� �������*/
	ProblemDataCudaMallocFail,			/*������������, ���� ������ ����� cudaMalloc*/
	ProblemDataCudaMemCpyFail,			/*������������, ���� ������ ����� cudaMemCpy*/
	ProblemDataUnknownError = 9999		/*����������������, ��� ����������� ������*/
};

class CProblemData
{
private:
	struct ProblemDiag
	{
		CArray2D<RealType>		a;
		CArray2D<RealType>		b;
		CArray2D<RealType>		c;
		ProblemDiag(bool data_type):
			a(data_type),
			b(data_type),
			c(data_type){}
	};

	struct ProblemLink
	{
		CArray2D<size_t>		idx;
		CArray2D<size_t>		row_idx;
		CArray2D<size_t>		col_idx;
		CArray2D<RealType>		val;
		ProblemLink(bool data_type):
			idx(data_type),
			row_idx(data_type),
			col_idx(data_type),
			val(data_type){}
	};

	struct ProblemRight
	{
		CArray2D<size_t>		idx;
		CArray2D<size_t>		row_idx;
		CArray2D<RealType>		val;
		ProblemRight(bool data_type):
			idx(data_type),
			row_idx(data_type),
			val(data_type){}
	};

	struct ProblemVector_D
	{
		CArray2D<size_t>		idx;
		CArray1D<size_t>		col_idx;
		CArray2D<size_t>		val;
		CArray2D<RealType>		X;
		ProblemVector_D(bool data_type):
			idx(data_type),
			col_idx(data_type),
			val(data_type),
			X(data_type){}
	};

	struct ProblemChains
	{
		size_t					chains_number;
		CArray2D<size_t>		chains_idx_in_blocks;
		CArray1D<size_t>		chains_num_in_blocks;	
		ProblemChains(bool data_type):
			chains_number(data_type),
			chains_idx_in_blocks(data_type),
			chains_num_in_blocks(data_type){}
	};
	
	size_t						blocks_number;
	ProblemDiag					diag;
	ProblemLink					link;
	ProblemRight				right;
	ProblemVector_D				vector_d;
	ProblemChains				chains;

	size_t						warp_size;

	// ������ ��� ������������� ������
private:
	size_t				vectors_d_num;					// ����� �������� D
	std::vector<size_t>	chains_in_blocks;				// ������� ����� ���������� �� ������ ����
	std::vector<size_t>	chains_idx_in_block;			// ������� ����� �� ����
	size_t				chains_in_blocks_max;			// ������������ ���������� ����� �� ����
	std::vector<size_t>	links_in_blocks;				// ������� ������ ���������� �� ������ ����
	size_t				links_in_blocks_max;			// ������������ ���������� ������ �� ����
	std::vector<size_t>	rights_in_blocks;				// ������� ������ ������� ������� ���������� �� ������ ����
	size_t				rights_in_blocks_max;			// ������������ ���������� ������ ������� ������� �� ����
	std::vector<size_t>	vectors_d_in_blocks;			// ������� �������� D ���������� �� ������ ����
	size_t				vectors_d_in_blocks_max;		// ������������ ���������� �������� D �� ����

public:
						CProblemData(void);
						~CProblemData(void);
	size_t				getWarpSize() const;																					// �������� ����������� �����
	size_t				getBlocksNumber() const;

	DataError			FromGPUData_MallocCpyDiag(const MCSAS::MatrixGenerator::CPUData);										// ���������� diag'�
	DataError			FromGPUData_MallocCpyLink(const MCSAS::MatrixGenerator::CPUData);
	DataError			FromGPUData_MallocCpyRight(const MCSAS::MatrixGenerator::CPUData);
	DataError			FromGPUData_MallocCpyVectorD(const MCSAS::MatrixGenerator::CPUData);
	DataError			FromCPUData_MallocCpyChainsInBlocks(const MCSAS::MatrixGenerator::CPUData);
	//DataError			FromCPUData_CalculateShared(MCSAS::MatrixGenerator::CPUData);
	DataError			FromCPUData(const MCSAS::MatrixGenerator::CPUData);		// ������� ������� �� CPU ����� � STD ���������
	size_t				GetChainsInBlocks(const MCSAS::MatrixGenerator::CPUData);
	void				GetRightsInBlocks(MCSAS::MatrixGenerator::CPUData);
	size_t				GetDVectorsInBlocks(const MCSAS::MatrixGenerator::CPUData);

private:
	void				Clear();
};

#endif // !__PROBLEM_DATA__SINIS__

