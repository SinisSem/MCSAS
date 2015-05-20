#include "Array2D.h"

template<typename Type>
void MCSAS::Types::CUDATypes::CArray2D<Type>::Clear()
{
	CData::Clear();
	length0 = 0;
	length1 = 0;
	pitch = 0;
	array_size = 0;
}

template<typename Type>
size_t MCSAS::Types::CUDATypes::CArray2D<Type>::GetLength0() const
{
	return length0;
}

template<typename Type>
size_t MCSAS::Types::CUDATypes::CArray2D<Type>::GetLength1() const
{
	return length1;
}

template<typename Type>
size_t MCSAS::Types::CUDATypes::CArray2D<Type>::GetPitch() const
{
	return pitch;
}

template<typename Type>
size_t MCSAS::Types::CUDATypes::CArray2D<Type>::GetSize() const
{
	return array_size;
}

template<typename Type>
void MCSAS::Types::CUDATypes::CArray2D<Type>::MallocByLength( const size_t ilength0, const size_t ilength1 )
{
	if (ilength0 < 1	||
		ilength1 < 1)
	{
		throw TypeException("Illegal arguments");
	}
	if (raw_pointer != nullptr)
	{
		Clear();
	}

	if (GetDataType() == false)
	{
		raw_pointer = (Type*)malloc(ilength0 * ilength1 * sizeof(Type));
		pitch = ilength0 * sizeof(Type);
	}
	else
	{
		size_t width = (ilength0) * sizeof(Type);
		cudaError_t cudaStatus = cudaMallocPitch((void**)(&raw_pointer), &(pitch), (ilength0) * sizeof(Type), (ilength1));
		if (cudaStatus != cudaSuccess)
			raw_pointer = nullptr;
	}
	if (raw_pointer == nullptr)
	{
		pitch = 0;
		throw TypeException("Cannot allocate an array");
	}
	else
	{
		array_size = pitch * ilength1;
		length0 = ilength0;
		length1 = ilength1;
	}
}

template<typename Type>
void MCSAS::Types::CUDATypes::CArray2D<Type>::CopyFrom( const Type* const src_ptr, const size_t ilength0, const size_t ilength1, const size_t ipitch, bool data_type = false )
{
	if (ilength0 < 1	||
		ilength1 < 1	||
		ipitch < 1		||
		src_ptr == nullptr)
	{
		throw TypeException("Illegal arguments");
	}
	if (raw_pointer == nullptr	||
		length0 != ilength0		||
		length1 != ilength1		)
	{
		throw TypeException("Array is too small");
	}

	if (data_type == true	&&
		GetDataType() == true)
	{
		cudaError_t  cudaStatus = cudaMemcpy2D((raw_pointer), (pitch), (src_ptr), ipitch, (length0) * sizeof(Type), (length1), cudaMemcpyDeviceToDevice);
		if (cudaStatus != cudaSuccess)
		{
			throw TypeException("Cannot allocate an array");
		}
	}
	else	
	if (data_type == true	&&
		GetDataType() == false)
	{
		cudaError_t  cudaStatus = cudaMemcpy2D((raw_pointer), (pitch), (src_ptr), ipitch, (length0) * sizeof(Type), (length1), cudaMemcpyDeviceToHost);
		if (cudaStatus != cudaSuccess)
		{
			throw TypeException("Cannot allocate an array");
		}
	}
	else	
	if (data_type == false	&&
		GetDataType() == true)
	{
		cudaError_t  cudaStatus = cudaMemcpy2D((raw_pointer), (pitch), (src_ptr), ipitch, (length0) * sizeof(Type), (length1), cudaMemcpyHostToDevice);
		if (cudaStatus != cudaSuccess)
		{
			throw TypeException("Cannot allocate an array");
		}
	}
	else	
	if (data_type == false	&&
		GetDataType() == false)
	{
		Type* check = (Type*)memcpy(raw_pointer, src_ptr, ipitch * length1 * sizeof(Type));
		if (check == nullptr)
		{
			throw TypeException("Cannot allocate an array");
		}
	}
}

template<typename Type>
void MCSAS::Types::CUDATypes::CArray2D<Type>::CopyFrom( const CArray2D<Type>& inputArray )
{
	return CopyFrom(inputArray.GetRawPointer(), inputArray.GetLength0(), inputArray.GetLength1(), inputArray.GetPitch(), inputArray.GetDataType());
}

template<typename Type>
MCSAS::Types::CUDATypes::CArray2D<Type>::CArray2D( const bool data_type = false ):
	CData(data_type)
{
	length0 = 0;
	length1 = 0;
	pitch = 0;
	array_size = 0;
}

template<typename Type>
MCSAS::Types::CUDATypes::CArray2D<Type>::~CArray2D( void )
{

}

template<typename Type>
Type& MCSAS::Types::CUDATypes::CArray2D<Type>::Element( size_t idx1, size_t idx0 )
{
	return *((Type*)((char*)raw_pointer + pitch * idx1) + idx0);
}

template<typename Type>
MCSAS::Types::CUDATypes::CArray2D<Type>::CArray2D( const CArray2D& copy_array )
{
	if (&copy_array == this)
		return;
	else
	{
		MallocByLength(copy_array.GetLength0(), copy_array.GetLength1());
		CopyFrom(copy_array);
	}
}

template<typename Type>
Type* MCSAS::Types::CUDATypes::CArray2D<Type>::Element( size_t idx1 )
{
	return (Type*)((char*)raw_pointer + pitch * idx1);
}



template class MCSAS::Types::CUDATypes::CArray2D<char>;
template class MCSAS::Types::CUDATypes::CArray2D<char2>;
template class MCSAS::Types::CUDATypes::CArray2D<char3>;
template class MCSAS::Types::CUDATypes::CArray2D<char4>;
template class MCSAS::Types::CUDATypes::CArray2D<size_t>;
template class MCSAS::Types::CUDATypes::CArray2D<uint2>;
template class MCSAS::Types::CUDATypes::CArray2D<uint3>;
template class MCSAS::Types::CUDATypes::CArray2D<uint4>;
template class MCSAS::Types::CUDATypes::CArray2D<int>;
template class MCSAS::Types::CUDATypes::CArray2D<int2>;
template class MCSAS::Types::CUDATypes::CArray2D<int3>;
template class MCSAS::Types::CUDATypes::CArray2D<int4>;
template class MCSAS::Types::CUDATypes::CArray2D<float>;
template class MCSAS::Types::CUDATypes::CArray2D<float2>;
template class MCSAS::Types::CUDATypes::CArray2D<float3>;
template class MCSAS::Types::CUDATypes::CArray2D<float4>;
template class MCSAS::Types::CUDATypes::CArray2D<double>;
template class MCSAS::Types::CUDATypes::CArray2D<double2>;
template class MCSAS::Types::CUDATypes::CArray2D<double3>;
template class MCSAS::Types::CUDATypes::CArray2D<double4>;