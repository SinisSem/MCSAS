#include "Problem.h"

#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <vector>
#include <algorithm>
#include <math.h>
#include "PException.h"
using namespace std;
using namespace MCSAS;
using namespace Problem;
using namespace Types;
using namespace BaseTypes;
using namespace MatrixGenerator;

template<typename Type>
MCSAS::Problem::CProblemReal<Type>::CProblemReal():
	Data(true)
{
	cudaGetDeviceProperties(&DevProp, 0);
}

template<typename Type>
MCSAS::Problem::CProblemReal<Type>::~CProblemReal()
{
	cudaDeviceReset();
}	

template<typename Type>
void MCSAS::Problem::CProblemReal<Type>::Clear()
{
	Data.blocks_number = 0;

	Data.chains.chains_idx_in_blocks.Clear();
	Data.chains.chains_num_in_blocks.Clear();
	Data.chains.chains_number = 0;

	Data.dense_matrix.Clear();

	Data.diag.a.Clear();
	Data.diag.b.Clear();
	Data.diag.c.Clear();

	Data.link.col_idx.Clear();
	Data.link.idx.Clear();
	Data.link.row_idx.Clear();
	Data.link.val.Clear();

	Data.right.idx.Clear();
	Data.right.row_idx.Clear();
	Data.right.val.Clear();

	Data.vector_d.col_idx.Clear();
	Data.vector_d.idx.Clear();
	Data.vector_d.val.Clear();
	Data.vector_d.X.Clear();
}

template<typename Type>
void MCSAS::Problem::CProblemReal<Type>::FormulateProblem( const Types::COO_matrix<Type> Matrix, const std::vector<Type> RightVector )
{
	std::vector<MCSAS::MatrixGenerator::Chain<Type>> RetChains = MCSAS::MatrixGenerator::GetChains(Matrix, RightVector, DevProp.warpSize);
	FormulateProblem(RetChains);
}


template<typename Type>
void MCSAS::Problem::CProblemReal<Type>::FormulateProblem( std::vector<Chain<Type>> Chains )
{
	std::vector<size_t>	chains_in_blocks;				// —колько цепей приходитс€ на каждый блок
	std::vector<size_t>	links_in_blocks;				// —колько св€зей приходитс€ на каждый блок
	std::vector<size_t>	rights_in_blocks;				// —колько частей правого вектора приходитс€ на каждый блок
	std::vector<size_t>	vectors_d_in_blocks;			// —колько векторов D приходитс€ на каждый блок

							GetChainsLinksInBlocks(	Chains, chains_in_blocks, links_in_blocks);
	rights_in_blocks	=	GetRightsInBlocks(		Chains, chains_in_blocks);
	vectors_d_in_blocks	=	GetDVectorsInBlocks(	Chains, chains_in_blocks);
	
	Data.blocks_number = chains_in_blocks.size();

	MallocCpyDiag(Chains);
	MallocCpyLink(Chains, chains_in_blocks, links_in_blocks);
	MallocCpyRight(Chains, chains_in_blocks, rights_in_blocks);
	MallocCpyVectorD(Chains, chains_in_blocks, vectors_d_in_blocks);
	MallocCpyChainsInBlocks(Chains, chains_in_blocks);
	
}

//TODO - оптимизировать загрузку цеп€ми по блокам
template<typename Type>
void MCSAS::Problem::CProblemReal<Type>::GetChainsLinksInBlocks(const std::vector<Chain<Type>> Chains, std::vector<size_t>& chains_in_blocks, std::vector<size_t>& links_in_blocks)
{
	chains_in_blocks.clear();
	links_in_blocks.clear();
	size_t num_blocks = 0;
	size_t links_in_block = 0;
	size_t chains_in_block = 0;

	size_t chain_link_start = 0;

	for(size_t chain_idx = 0; chain_idx < Chains.size(); chain_idx++)
	{
		size_t links_in_chain = Chains[chain_idx].Links.size();

		if (links_in_block + links_in_chain <= DevProp.warpSize)
		{
			links_in_block += links_in_chain;
			chains_in_block++;
			if (chain_idx == Chains.size() - 1)
			{
				chains_in_blocks.push_back(chains_in_block);
				links_in_blocks.push_back(links_in_block);
			}
		}
		else
		{
//			if (links_in_chain > DevProp.warpSize)
//				cout<<"links_in_chain > warp_size!!!\n";
//			if (links_in_block > DevProp.warpSize)
//				cout<<"links_in_block > warp_size!!!\n";
			chains_in_blocks.push_back(chains_in_block);
			links_in_blocks.push_back(links_in_block);

			chains_in_block = 0;
			links_in_block = 0;
			num_blocks++;
			chain_idx--;
		}
	}
}

template<typename Type>
std::vector<size_t> MCSAS::Problem::CProblemReal<Type>::GetDVectorsInBlocks(const std::vector<Chain<Type>> Chains, const std::vector<size_t> chains_in_blocks)
{
	std::vector<size_t>	vectors_d_in_blocks;			// —колько векторов D приходитс€ на каждый блок

	size_t chain_idx = 0;

//	size_t vectors_d_num = 0;
	for(size_t block_idx = 0; block_idx < chains_in_blocks.size();	block_idx++)
	{
		size_t vectors_d_in_block = 0;
		for(size_t chain_in_block_idx = 0; chain_in_block_idx < chains_in_blocks[block_idx]; chain_in_block_idx++)
		{
			//vectors_d_in_blocks += input.Chains[chain_idx].LinksColIdx.size();
			if (Chains[chain_idx].Links.size() > 0)
				vectors_d_in_block++;
			for(size_t link_in_chain = 1; link_in_chain < Chains[chain_idx].Links.size(); link_in_chain++)
			{
				if (Chains[chain_idx].Links[link_in_chain].ColNumber != Chains[chain_idx].Links[link_in_chain - 1].ColNumber)
					vectors_d_in_block++;
			}
			chain_idx++;
		}
////		vectors_d_num += vectors_d_in_block;
		vectors_d_in_blocks.push_back(vectors_d_in_block);
	}
	return vectors_d_in_blocks;
}

template<typename Type>
std::vector<size_t> MCSAS::Problem::CProblemReal<Type>::GetRightsInBlocks(const std::vector<Chain<Type>> Chains, const std::vector<size_t> chains_in_blocks)
{
	std::vector<size_t>	rights_in_blocks;				// —колько частей правого вектора приходитс€ на каждый блок

	size_t chain_idx = 0;

	for(size_t block_idx = 0; block_idx < chains_in_blocks.size();	block_idx++)
	{
		size_t rights_in_block = 0;
		for(size_t right_in_block_idx = 0; right_in_block_idx < chains_in_blocks[block_idx]; right_in_block_idx++)
		{
			for(size_t rv_idx = 0; rv_idx < Chains[chain_idx].RightVector.size(); rv_idx++)
				if(Chains[chain_idx].RightVector[rv_idx] != 0.0)
					rights_in_block++;
			chain_idx++;
		}
		rights_in_blocks.push_back(rights_in_block);
	}
	return rights_in_blocks;
}

template<typename Type>
void MCSAS::Problem::CProblemReal<Type>::MallocDenseMatrix( const std::vector<MatrixGenerator::Chain<Type>> Chains )
{
	//for(size_t )
}

template<typename Type>
void MCSAS::Problem::CProblemReal<Type>::MallocCpyChainsInBlocks( const std::vector<MatrixGenerator::Chain<Type>> Chains, 
	const std::vector<size_t> chains_in_blocks )
{
	Data.chains.chains_number = std::accumulate(chains_in_blocks.cbegin(), chains_in_blocks.cend(), 0);

	ProblemChains host_chains(false);
	host_chains.chains_number = Chains.size();

	size_t blocks_number =  chains_in_blocks.size();
	size_t chains_in_blocks_max	= *std::max_element(chains_in_blocks.cbegin(), chains_in_blocks.cend());


	host_chains.chains_num_in_blocks.MallocByLength(chains_in_blocks.size());
	host_chains.chains_num_in_blocks.CopyFrom(chains_in_blocks.data(), chains_in_blocks.size() * sizeof(size_t), false);

	host_chains.chains_idx_in_blocks.MallocByLength(chains_in_blocks_max + 1, blocks_number);
	size_t chain_idx = 1;
	for(size_t block_idx = 0; block_idx < blocks_number; block_idx++)
	{
		host_chains.chains_idx_in_blocks.Element(block_idx, 0) = 0;
		for(size_t chain_in_block_idx = 0; chain_in_block_idx < chains_in_blocks[block_idx]; chain_in_block_idx++)
		{
			host_chains.chains_idx_in_blocks.Element(block_idx, chain_in_block_idx) = chain_idx;
			chain_idx++;
		}
	}

	Data.chains.chains_idx_in_blocks.MallocByLength(host_chains.chains_idx_in_blocks.GetLength0(), host_chains.chains_idx_in_blocks.GetLength1());
	Data.vector_d.idx.CopyFrom(host_chains.chains_idx_in_blocks);
}

template<typename Type>
void MCSAS::Problem::CProblemReal<Type>::MallocCpyVectorD( const std::vector<MatrixGenerator::Chain<Type>> Chains,
	const std::vector<size_t> chains_in_blocks, const std::vector<size_t> vectors_d_in_blocks )
{
	ProblemVector_D<Type> host_vector_d(false);

	size_t blocks_number =  chains_in_blocks.size();
	size_t chains_in_blocks_max	= *std::max_element(chains_in_blocks.cbegin(), chains_in_blocks.cend());
	size_t vectors_d_num		= std::accumulate(vectors_d_in_blocks.cbegin(), vectors_d_in_blocks.cend(), 0);

	host_vector_d.idx.MallocByLength(chains_in_blocks_max+1, blocks_number);

	size_t chain_idx = 0;
	for(size_t block_idx = 0; block_idx < blocks_number; block_idx++)
	{
		size_t vector_d_in_block = 0;
		host_vector_d.idx.Element(block_idx, 0) = 0;
		for(size_t chain_in_block_idx = 0; chain_in_block_idx < chains_in_blocks[block_idx]; chain_in_block_idx++)
		{
			vector_d_in_block += Chains[chain_idx].Links.size();
			host_vector_d.idx.Element(block_idx, chain_in_block_idx + 1) = vector_d_in_block;
			chain_idx++;
		}
	}

	Data.vector_d.idx.MallocByLength(host_vector_d.idx.GetLength0(), host_vector_d.idx.GetLength1());
	Data.vector_d.idx.CopyFrom(host_vector_d.idx);

	Data.vector_d.col_idx.MallocByLength(vectors_d_num);

	Data.vector_d.val.MallocByLength(DevProp.warpSize, vectors_d_num);

	Data.vector_d.X.MallocByLength(DevProp.warpSize, Chains.size());
}

template<typename Type>
void MCSAS::Problem::CProblemReal<Type>::MallocCpyRight(const std::vector<MatrixGenerator::Chain<Type>> Chains,
	const std::vector<size_t> chains_in_blocks, const std::vector<size_t> rights_in_blocks)
{
	ProblemRight<Type> host_right(false);

	size_t rights_in_blocks_max		= *std::max_element(rights_in_blocks.cbegin(), rights_in_blocks.cend());
	size_t chains_in_blocks_max		= *std::max_element(chains_in_blocks.cbegin(), chains_in_blocks.cend());
	size_t blocks_number =  rights_in_blocks.size();

	host_right.idx.MallocByLength((chains_in_blocks_max+1), blocks_number);
	host_right.row_idx.MallocByLength(rights_in_blocks_max, blocks_number);
	host_right.val.MallocByLength(rights_in_blocks_max, blocks_number);

	size_t chain_idx = 0;
	for(size_t block_idx = 0; block_idx < blocks_number; block_idx++)
	{
		size_t right_in_block = 0;
		host_right.idx.Element(block_idx, 0) = 0;
		for(size_t chain_in_block_idx = 0; chain_in_block_idx < chains_in_blocks[block_idx]; chain_in_block_idx++)
		{
			for(size_t right_in_chain = 0; right_in_chain < Chains[chain_idx].RightVector.size(); right_in_chain++)
			{
				if (Chains[chain_idx].RightVector[right_in_chain] != 0)
				{
					host_right.row_idx.Element(block_idx, right_in_block) = right_in_chain + Chains[chain_idx].StartNode;
					host_right.val.Element(block_idx, right_in_block) = Chains[chain_idx].RightVector[right_in_chain];
					right_in_block++;
				}
			}
			host_right.idx.Element(block_idx, chain_in_block_idx + 1) = right_in_block;
			chain_idx++;
		}
	}
	
	Data.right.idx.MallocByLength(host_right.idx.GetLength0(), host_right.idx.GetLength1());
	Data.right.idx.CopyFrom(host_right.idx);

	Data.right.row_idx.MallocByLength(host_right.row_idx.GetLength0(), host_right.row_idx.GetLength1());
	Data.right.row_idx.CopyFrom(host_right.row_idx);

	Data.right.val.MallocByLength(host_right.val.GetLength0(), host_right.val.GetLength1());
	Data.right.val.CopyFrom(host_right.val);

}

template<typename Type>
void MCSAS::Problem::CProblemReal<Type>::MallocCpyLink( const std::vector<MatrixGenerator::Chain<Type>> Chains,
	const std::vector<size_t> chains_in_blocks, const std::vector<size_t> links_in_blocks)
{
	ProblemLink<Type> host_link(false);
	size_t chains_in_blocks_max = *std::max_element(chains_in_blocks.cbegin(), chains_in_blocks.cend());
	size_t links_in_blocks_max = *std::max_element(links_in_blocks.cbegin(), links_in_blocks.cend());
	size_t blocks_number =  chains_in_blocks.size();

	host_link.idx.MallocByLength((chains_in_blocks_max+1), blocks_number);
	host_link.col_idx.MallocByLength(links_in_blocks_max, blocks_number);
	host_link.row_idx.MallocByLength(links_in_blocks_max, blocks_number);
	host_link.val.MallocByLength(links_in_blocks_max, blocks_number);

	size_t chain_idx = 0;
	chain_idx = 0;
	for(size_t block_idx = 0; block_idx < blocks_number; block_idx++)
	{
		size_t link_in_block = 0;
		host_link.idx.Element(block_idx, 0) = 0;
		for(size_t chain_in_block_idx = 0; chain_in_block_idx < chains_in_blocks[block_idx]; chain_in_block_idx++)
		{
			size_t link_in_chain;
			for(link_in_chain = 0; link_in_chain < Chains[chain_idx].Links.size(); link_in_chain++)
			{
				host_link.col_idx.Element(block_idx, link_in_block)	= Chains[chain_idx].Links[link_in_chain].ColNumber;
				host_link.row_idx.Element(block_idx, link_in_block)	= Chains[chain_idx].Links[link_in_chain].RowNumber;
				host_link.val.Element(block_idx, link_in_block)		= Chains[chain_idx].Links[link_in_chain].Value;

				link_in_block++;
			}
			host_link.idx.Element(block_idx, chain_in_block_idx + 1) = link_in_block;
			chain_idx++;
		}
	}

	Data.link.idx.MallocByLength(host_link.idx.GetLength0(), host_link.idx.GetLength1());
	Data.link.idx.CopyFrom(host_link.idx);

	Data.link.col_idx.MallocByLength(host_link.col_idx.GetLength0(), host_link.col_idx.GetLength1());
	Data.link.col_idx.CopyFrom(host_link.col_idx);

	Data.link.row_idx.MallocByLength(host_link.row_idx.GetLength0(), host_link.row_idx.GetLength1());
	Data.link.row_idx.CopyFrom(host_link.row_idx);

	Data.link.val.MallocByLength(host_link.val.GetLength0(), host_link.val.GetLength1());
	Data.link.val.CopyFrom(host_link.val);
}




template<typename Type>
void MCSAS::Problem::CProblemReal<Type>::MallocCpyDiag( const std::vector<MatrixGenerator::Chain<Type>> Chains )
{
	ProblemDiag<Type> host_daig(false);

	host_daig.a.MallocByLength(DevProp.warpSize, Data.chains.chains_number);
	host_daig.b.MallocByLength(DevProp.warpSize, Data.chains.chains_number);
	host_daig.c.MallocByLength(DevProp.warpSize, Data.chains.chains_number);

	for(size_t chain_idx = 0; chain_idx < Data.chains.chains_number; chain_idx++)
	{
		memcpy((void*)host_daig.a.Element(chain_idx), Chains[chain_idx].a.data(), Chains[chain_idx].a.size() * sizeof(Type));
		host_daig.a.Element(chain_idx, DevProp.warpSize - 1) = 0;

		memcpy((void*)host_daig.b.Element(chain_idx), Chains[chain_idx].b.data(), Chains[chain_idx].b.size() * sizeof(Type));
		host_daig.b.Element(chain_idx, DevProp.warpSize - 1) = 0;

		memcpy((void*)host_daig.c.Element(chain_idx), Chains[chain_idx].c.data(), Chains[chain_idx].c.size() * sizeof(Type));
		host_daig.c.Element(chain_idx, DevProp.warpSize - 1) = 0;
	}

	Data.diag.a.MallocByLength(host_daig.a.GetLength0(), host_daig.a.GetLength1());
	Data.diag.a.CopyFrom(host_daig.a);

	Data.diag.b.MallocByLength(host_daig.b.GetLength0(), host_daig.b.GetLength1());
	Data.diag.b.CopyFrom(host_daig.b);

	Data.diag.c.MallocByLength(host_daig.c.GetLength0(), host_daig.c.GetLength1());
	Data.diag.c.CopyFrom(host_daig.c);
}


template<typename Type>
ProblemData<Type> MCSAS::Problem::CProblemReal<Type>::GetProblemData()
{
	return Data;
}









template class CProblemReal<float>;
template class CProblemReal<double>;
//template class CProblem<MCSAS::Types::BaseTypes::ComplexDoubleCartesian>;
//template class CProblem<MCSAS::Types::BaseTypes::ComplexDoubleHybrid>;
//template class CProblem<MCSAS::Types::BaseTypes::ComplexDoublePolar>;
//template class CProblem<MCSAS::Types::BaseTypes::ComplexFloatCartesian>;
//template class CProblem<MCSAS::Types::BaseTypes::ComplexFloatHybrid>;
//template class CProblem<MCSAS::Types::BaseTypes::ComplexFloatPolar>;