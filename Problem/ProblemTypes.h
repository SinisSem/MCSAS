#pragma once
#include "../Types/CUDATypes/Array1D.h"
#include "../Types/CUDATypes/Array2D.h"

namespace MCSAS
{
	namespace Problem
	{
		template<typename Type>
		struct ProblemDiag
		{
			MCSAS::Types::CUDATypes::CArray2D<Type>			a;
			MCSAS::Types::CUDATypes::CArray2D<Type>			b;
			MCSAS::Types::CUDATypes::CArray2D<Type>			c;
			ProblemDiag(bool data_type):
				a(data_type),
				b(data_type),
				c(data_type){}
		};

		template<typename Type>
		struct ProblemLink
		{
			MCSAS::Types::CUDATypes::CArray2D<size_t>		idx;
			MCSAS::Types::CUDATypes::CArray2D<size_t>		row_idx;
			MCSAS::Types::CUDATypes::CArray2D<size_t>		col_idx;
			MCSAS::Types::CUDATypes::CArray2D<Type>			val;
			ProblemLink(bool data_type):
				idx(data_type),
				row_idx(data_type),
				col_idx(data_type),
				val(data_type){}
		};

		template<typename Type>
		struct ProblemRight
		{
			MCSAS::Types::CUDATypes::CArray2D<size_t>		idx;
			MCSAS::Types::CUDATypes::CArray2D<size_t>		row_idx;
			MCSAS::Types::CUDATypes::CArray2D<Type>			val;
			ProblemRight(bool data_type):
				idx(data_type),
				row_idx(data_type),
				val(data_type){}
		};

		template<typename Type>
		struct ProblemVector_D
		{
			MCSAS::Types::CUDATypes::CArray2D<size_t>		idx;
			MCSAS::Types::CUDATypes::CArray1D<size_t>		col_idx;
			MCSAS::Types::CUDATypes::CArray2D<Type>			val;
			MCSAS::Types::CUDATypes::CArray2D<Type>			X;
			ProblemVector_D(bool data_type):
				idx(data_type),
				col_idx(data_type),
				val(data_type),
				X(data_type){}
		};

		struct ProblemChains
		{
			size_t											chains_number;
			MCSAS::Types::CUDATypes::CArray2D<size_t>		chains_idx_in_blocks;
			MCSAS::Types::CUDATypes::CArray1D<size_t>		chains_num_in_blocks;	
			ProblemChains(bool data_type):
				chains_number(data_type),
				chains_idx_in_blocks(data_type),
				chains_num_in_blocks(data_type){}
		};

		template<typename Type>
		struct ProblemData
		{
			size_t										blocks_number;
			ProblemDiag<Type>							diag;
			ProblemLink<Type>							link;
			ProblemRight<Type>							right;
			ProblemVector_D<Type>						vector_d;
			ProblemChains								chains;
			MCSAS::Types::CUDATypes::CArray2D<Type>		dense_matrix;
			ProblemData(bool data_type):
				blocks_number(data_type),
				diag(data_type),
				link(data_type),
				right(data_type),
				vector_d(data_type),
				chains(data_type),
				dense_matrix(data_type){}
		};
	}
}
