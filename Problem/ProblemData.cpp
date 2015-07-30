#include "ProblemData.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

CProblemData::CProblemData( void ):
	diag(true),
	link(true),
	right(true),
	vector_d(true),
	chains(true)
{
	warp_size = 32;
	blocks_number = 0;

	cudaError_t cudaStatus = cudaSetDevice(0);
	if (cudaStatus != cudaSuccess) 
		cout<<"cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?";
}

CProblemData::~CProblemData( void )
{
	  
}

size_t CProblemData::getWarpSize() const
{
	return warp_size;
}

size_t CProblemData::getBlocksNumber() const
{
	return blocks_number;
}

DataError CProblemData::FromCPUData( const MCSAS::MatrixGenerator::CPUData input )
{
	cout<<"data initialize start\n";

	blocks_number	=	GetChainsInBlocks(input);
						GetRightsInBlocks(input);
	vectors_d_num	=	GetDVectorsInBlocks(input);
	chains.chains_number	= input.Chains.size();
	links_in_blocks_max		= *std::max_element(links_in_blocks.cbegin(), links_in_blocks.cend());
	rights_in_blocks_max	= *std::max_element(rights_in_blocks.cbegin(), rights_in_blocks.cend());
	chains_in_blocks_max	= *std::max_element(chains_in_blocks.cbegin(), chains_in_blocks.cend());
	vectors_d_in_blocks_max	= *std::max_element(chains_in_blocks.cbegin(), chains_in_blocks.cend());

	DataError Res;

	// diag
	Res = FromGPUData_MallocCpyDiag(input);
	if (Res == DataSuccess)
		cout<<"diag is initialized\n";
	else
	{
		cout<<"problem in FromGPUData_MallocCpyDiag, error code = " << Res << "\n";
		goto Error;
	}

	// link
	Res = FromGPUData_MallocCpyLink(input);
	if (Res == DataSuccess)
		cout<<"link is initialized\n";
	else
	{
		cout<<"problem in FromGPUData_MallocCpyLink, error code = " << Res << "\n";
		goto Error;
	}

	// right
	Res = FromGPUData_MallocCpyRight(input);
	if (Res == DataSuccess)
		cout<<"right is initialized\n";
	else
	{
		cout<<"problem in FromGPUData_MallocCpyRight, error code = " << Res << "\n";
		goto Error;
	}

	// vector_d
	Res = FromGPUData_MallocCpyVectorD(input);	
	if (Res == DataSuccess)
		cout<<"vector_d is initialized\n";
	else
	{
		cout<<"problem in FromGPUData_MallocCpyVectorD, error code = " << Res << "\n";
		goto Error;
	}

	// chains
	Res = FromCPUData_MallocCpyChainsInBlocks(input);
	if (Res == DataSuccess)
		cout<<"chains is initialized\n";
	else
	{
		cout<<"problem in FromCPUData_MallocCpyChainsInBlocks, error code = " << Res << "\n";
		goto Error;
	}

	cout<<"dev_data is initialized\n";
Error:
	return Res;
}

size_t CProblemData::GetChainsInBlocks( const MCSAS::MatrixGenerator::CPUData input )
{
	chains_in_blocks.clear();
	links_in_blocks.clear();

	size_t num_blocks = 0;
	size_t links_in_block = 0;
	size_t chains_in_block = 0;
	for(size_t chain_idx = 0; chain_idx < input.Chains.size(); chain_idx++)
	{
		size_t links_in_chain = input.Chains[chain_idx].LinksVal.size();

		if (links_in_block + links_in_chain <= warp_size)
		{
			links_in_block += links_in_chain;
			chains_in_block++;
		}
		else
		{
			if (links_in_chain > warp_size)
				cout<<"links_in_chain > warp_size!!!\n";
			if (links_in_block > warp_size)
				cout<<"links_in_block > warp_size!!!\n";
			chains_in_blocks.push_back(chains_in_block);
			links_in_blocks.push_back(links_in_block);

			chains_in_block = 0;
			links_in_block = 0;
			num_blocks++;
			chain_idx--;

		}
	}

	blocks_number = num_blocks;

	return num_blocks;
}


void CProblemData::GetRightsInBlocks( const MCSAS::MatrixGenerator::CPUData input )
{
	rights_in_blocks.clear();

	size_t chain_idx = 0;

	for(size_t block_idx = 0; block_idx < blocks_number;	block_idx++)
	{
		size_t rights_in_block = 0;
		for(size_t right_in_block_idx = 0; right_in_block_idx < chains_in_blocks[block_idx]; right_in_block_idx++)
		{
			rights_in_block += input.Chains[chain_idx].RightVectorRowIdx.size();
			chain_idx++;
		}
		rights_in_blocks.push_back(rights_in_block);

	}
}

size_t CProblemData::GetDVectorsInBlocks( const MCSAS::MatrixGenerator::CPUData input )
{
	vectors_d_in_blocks.clear();

	size_t chain_idx = 0;

	size_t vectors_d_num = 0;
	for(size_t block_idx = 0; block_idx < blocks_number;	block_idx++)
	{
		size_t vectors_d_in_block = 0;
		for(size_t chain_in_block_idx = 0; chain_in_block_idx < chains_in_blocks[block_idx]; chain_in_block_idx++)
		{
			//vectors_d_in_blocks += input.Chains[chain_idx].LinksColIdx.size();
			if (input.Chains[chain_idx].LinksColIdx.size() > 0)
				vectors_d_in_block++;
			for(size_t link_in_chain = 1; link_in_chain < input.Chains[chain_idx].LinksColIdx.size(); link_in_chain++)
			{
				if (input.Chains[chain_idx].LinksColIdx[link_in_chain] != input.Chains[chain_idx].LinksColIdx[link_in_chain - 1])
					vectors_d_in_block++;
			}
			chain_idx++;
		}
		vectors_d_num += vectors_d_in_block;
		vectors_d_in_blocks.push_back(vectors_d_in_block);
	}
	return vectors_d_num;
}

void CProblemData::Clear()
{
	diag.a.Clear();
	diag.b.Clear();
	diag.c.Clear();

	link.col_idx.Clear();
	link.idx.Clear();
	link.row_idx.Clear();
	link.col_idx.Clear();

	vector_d.col_idx.Clear();
	vector_d.idx.Clear();
	vector_d.val.Clear();

	right.idx.Clear();
	right.row_idx.Clear();
	right.val.Clear();

	chains.chains_idx_in_blocks.Clear();
	chains.chains_num_in_blocks.Clear();
	chains.chains_number = 0;
}

DataError CProblemData::FromGPUData_MallocCpyDiag( const MCSAS::MatrixGenerator::CPUData input )
{
	DataError Res = DataSuccess;

	ProblemDiag host_daig(false);

	host_daig.a.MallocByLength(warp_size, chains.chains_number);
	host_daig.b.MallocByLength(warp_size, chains.chains_number);
	host_daig.c.MallocByLength(warp_size, chains.chains_number);

	for(uint chain_idx = 0; chain_idx < chains.chains_number; chain_idx++)
	{
		memcpy((void*)host_daig.a.Element(chain_idx), input.Chains[chain_idx].a.data(), input.Chains[chain_idx].a.size() * sizeof(RealType));
		host_daig.a.Element(chain_idx, warp_size - 1) = 0;

		memcpy((void*)host_daig.b.Element(chain_idx), input.Chains[chain_idx].b.data(), input.Chains[chain_idx].b.size() * sizeof(RealType));
		host_daig.b.Element(chain_idx, warp_size - 1) = 0;

		memcpy((void*)host_daig.c.Element(chain_idx), input.Chains[chain_idx].c.data(), input.Chains[chain_idx].c.size() * sizeof(RealType));
		host_daig.c.Element(chain_idx, warp_size - 1) = 0;
	}

	Res = diag.a.MallocByLength(host_daig.a.GetLength0(), host_daig.a.GetLength1());
	if (Res != DataSuccess)
	{
		Clear();
		return Res;
	}
	Res = diag.a.CopyFrom(host_daig.a);
	if (Res != DataSuccess)
	{
		Clear();
		return Res;
	}
	Res = diag.b.MallocByLength(host_daig.b.GetLength0(), host_daig.b.GetLength1());
	if (Res != DataSuccess)
	{
		Clear();
		return Res;
	}
	Res = diag.b.CopyFrom(host_daig.b);
	if (Res != DataSuccess)
	{
		Clear();
		return Res;
	}
	Res = diag.c.MallocByLength(host_daig.c.GetLength0(), host_daig.c.GetLength1());
	if (Res != DataSuccess)
	{
		Clear();
		return Res;
	}
	Res = diag.c.CopyFrom(host_daig.c);
	if (Res != DataSuccess)
	{
		Clear();
		return Res;
	}

	return Res;
}

DataError CProblemData::FromGPUData_MallocCpyLink( const MCSAS::MatrixGenerator::CPUData input )
{
	DataError Res = DataSuccess;
	//	links_in_blocks_max = 32;
	ProblemLink host_link(false);

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
			for(link_in_chain = 0; link_in_chain < input.Chains[chain_idx].LinksColIdx.size(); link_in_chain++)
			{
				host_link.col_idx.Element(block_idx, link_in_block)	= input.Chains[chain_idx].LinksColIdx[link_in_chain];
				host_link.row_idx.Element(block_idx, link_in_block)	= input.Chains[chain_idx].LinksRowIdx[link_in_chain];
				host_link.val.Element(block_idx, link_in_block)		= input.Chains[chain_idx].LinksVal[link_in_chain];

				link_in_block++;
			}
			host_link.idx.Element(block_idx, chain_in_block_idx + 1) = link_in_block;
			chain_idx++;
		}
	}

	Res = link.idx.MallocByLength(host_link.idx.GetLength0(), host_link.idx.GetLength1());
	Res = link.idx.CopyFrom(host_link.idx);
	if (Res != DataSuccess)
	{
		Clear();
		return Res;
	}
	Res = link.col_idx.MallocByLength(host_link.col_idx.GetLength0(), host_link.col_idx.GetLength1());
	Res = link.col_idx.CopyFrom(host_link.col_idx);
	if (Res != DataSuccess)
	{
		Clear();
		return Res;
	}
	Res = link.row_idx.MallocByLength(host_link.row_idx.GetLength0(), host_link.row_idx.GetLength1());
	Res = link.row_idx.CopyFrom(host_link.row_idx);
	if (Res != DataSuccess)
	{
		Clear();
		return Res;
	}
	Res = link.val.MallocByLength(host_link.val.GetLength0(), host_link.val.GetLength1());
	Res = link.val.CopyFrom(host_link.val);
	if (Res != DataSuccess)
	{
		Clear();
		return Res;
	}

	return Res;
}

DataError CProblemData::FromGPUData_MallocCpyRight( const MCSAS::MatrixGenerator::CPUData input )
{
	DataError Res;

	ProblemRight host_right(false);
	host_right.idx.MallocByLength((chains_in_blocks_max+1), blocks_number);
	host_right.row_idx.MallocByLength(rights_in_blocks_max, blocks_number);
	host_right.val.MallocByLength(rights_in_blocks_max, blocks_number);

	uint chain_idx = 0;
	for(uint block_idx = 0; block_idx < blocks_number; block_idx++)
	{
		uint right_in_block = 0;
		host_right.idx.Element(block_idx, 0) = 0;
		for(uint chain_in_block_idx = 0; chain_in_block_idx < chains_in_blocks[block_idx]; chain_in_block_idx++)
		{
			for(uint right_in_chain = 0; right_in_chain < input.Chains[chain_idx].RightVectorRowIdx.size(); right_in_chain++)
			{
				host_right.row_idx.Element(block_idx, right_in_block) = input.Chains[chain_idx].RightVectorRowIdx[right_in_chain];
				host_right.val.Element(block_idx, right_in_block) = input.Chains[chain_idx].RightVectorVal[right_in_chain];
				right_in_block++;
			}
			host_right.idx.Element(block_idx, chain_in_block_idx + 1) = right_in_block;
			chain_idx++;
		}
	}

	Res = right.idx.MallocByLength(host_right.idx.GetLength0(), host_right.idx.GetLength1());
	Res = right.idx.CopyFrom(host_right.idx);
	if (Res != DataSuccess)
	{
		Clear();
		return Res;
	}
	Res = right.row_idx.MallocByLength(host_right.row_idx.GetLength0(), host_right.row_idx.GetLength1());
	Res = right.row_idx.CopyFrom(host_right.row_idx);
	if (Res != DataSuccess)
	{
		Clear();
		return Res;
	}
	Res = right.val.MallocByLength(host_right.val.GetLength0(), host_right.val.GetLength1());
	Res = right.val.CopyFrom(host_right.val);
	if (Res != DataSuccess)
	{
		Clear();
		return Res;
	}

	return Res;
}

DataError CProblemData::FromGPUData_MallocCpyVectorD( const MCSAS::MatrixGenerator::CPUData input )
{
	DataError Res;

	ProblemVector_D host_vector_d(false);

	host_vector_d.idx.MallocByLength(chains_in_blocks_max+1, blocks_number);

	uint chain_idx = 0;
	for(uint block_idx = 0; block_idx < blocks_number; block_idx++)
	{
		uint vector_d_in_block = 0;
		host_vector_d.idx.Element(block_idx, 0) = 0;
		for(uint chain_in_block_idx = 0; chain_in_block_idx < chains_in_blocks[block_idx]; chain_in_block_idx++)
		{
			vector_d_in_block+=input.Chains[chain_idx].RightVectorRowIdx.size();
			host_vector_d.idx.Element(block_idx, chain_in_block_idx + 1) = vector_d_in_block;
			chain_idx++;
		}
	}
	Res = vector_d.idx.MallocByLength(host_vector_d.idx.GetLength0(), host_vector_d.idx.GetLength1());
	if (Res != DataSuccess)
	{
		Clear();
		return Res;
	}
	Res = vector_d.idx.CopyFrom(host_vector_d.idx);
	if (Res != DataSuccess)
	{
		Clear();
		return Res;
	}
	Res = vector_d.col_idx.MallocByLength(vectors_d_num);
	if (Res != DataSuccess)
	{
		Clear();
		return Res;
	}
	Res = vector_d.val.MallocByLength(warp_size, vectors_d_num);
	if (Res != DataSuccess)
	{
		Clear();
		return Res;
	}

	return Res;
}

DataError CProblemData::FromCPUData_MallocCpyChainsInBlocks( const MCSAS::MatrixGenerator::CPUData input )
{
	DataError Res;

	ProblemChains host_chains(false);
	host_chains.chains_number = input.Chains.size();
	host_chains.chains_num_in_blocks.MallocByLength(blocks_number);
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
	
	Res = chains.chains_idx_in_blocks.MallocByLength(host_chains.chains_idx_in_blocks.GetLength0(), host_chains.chains_idx_in_blocks.GetLength1());
	if (Res != DataSuccess)
	{
		Clear();
		return Res;
	}
	Res = vector_d.idx.CopyFrom(host_chains.chains_idx_in_blocks);
	if (Res != DataSuccess)
	{
		Clear();
		return Res;
	}

	return Res;

}

