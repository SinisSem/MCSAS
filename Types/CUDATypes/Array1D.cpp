#include "Array1D.h"

template<typename Type>
MCSAS::Types::CUDATypes::CArray1D<Type>::CArray1D( const bool  data_type ):
	CData(data_type)
{
	array_size = 0;
}

template<typename Type>
size_t MCSAS::Types::CUDATypes::CArray1D<Type>::GetLength() const
{
	return array_size/(sizeof(Type));
}

template<typename Type>
size_t MCSAS::Types::CUDATypes::CArray1D<Type>::GetSize() const
{
	return array_size;
}

template<typename Type>
void 
MCSAS::Types::CUDATypes::CArray1D<Type>::MallocBySize( const size_t size )
{
	if (size < 1)
	{
		throw TypeException("Array has zero size");
	}
	if (raw_pointer != nullptr)
	{
		Clear();
	}
	if (GetDataType() == false)
		raw_pointer = (Type*)malloc(size);
	else
	{
		cudaError_t cudaStatus = cudaMalloc(&raw_pointer, size);
		if (cudaStatus != cudaSuccess)
			raw_pointer = nullptr;
	}

	if (raw_pointer == nullptr)
	{
		throw TypeException("Cannot allocate an array");
	}
	else
	{
		array_size = size;
	}
}

template<typename Type>
void MCSAS::Types::CUDATypes::CArray1D<Type>::MallocByLength( const size_t length )
{
	return MallocBySize(length * sizeof(Type));
}

template<typename Type>
void MCSAS::Types::CUDATypes::CArray1D<Type>::CopyFrom( const Type* const src_ptr, const size_t size, bool data_type = false )
{
	if (size < 1	||
		src_ptr == nullptr)
	{
		throw TypeException("Illegal arguments");
	}
	if (raw_pointer == nullptr		||
		size != array_size)
	{
		MallocBySize(size);
	}

	if (data_type == true	&&
		GetDataType() == true)
	{
		cudaError_t cudaStatus = cudaMemcpy((void*)&raw_pointer, src_ptr, size, cudaMemcpyDeviceToDevice);
		if (cudaStatus != cudaSuccess)
		{
			throw TypeException("Cannot allocate an array");
		}
	}
	else	
	if (data_type == true	&&
		GetDataType() == false)
	{
		cudaError_t cudaStatus = cudaMemcpy((void*)&raw_pointer, src_ptr, size, cudaMemcpyHostToDevice);
		if (cudaStatus != cudaSuccess)
		{
			throw TypeException("Cannot allocate an array");
		}
	}
	else	
	if (data_type == false	&&
		GetDataType() == true)
	{
		cudaError_t cudaStatus = cudaMemcpy((void*)&raw_pointer, src_ptr, size, cudaMemcpyDeviceToHost);
		if (cudaStatus != cudaSuccess)
		{
			throw TypeException("Cannot allocate an array");
		}
	}
	else	
	if (data_type == false	&&
		GetDataType() == false)
	{
		Type* check = (Type*)memcpy(raw_pointer, src_ptr, size);
		if (check == nullptr)
		{
			throw TypeException("Cannot allocate an array");
		}
	}
}

template<typename Type>
void MCSAS::Types::CUDATypes::CArray1D<Type>::CopyFrom(const CArray1D<Type>& inputArray)
{
	return CopyFrom(inputArray.GetRawPointer(), inputArray.GetSize(), inputArray.GetDataType());
}

template<typename Type>
void MCSAS::Types::CUDATypes::CArray1D<Type>::Clear()
{
	CData::Clear();
	array_size = 0;
}

template<typename Type>
MCSAS::Types::CUDATypes::CArray1D<Type>::~CArray1D( void )
{

}

//template<typename Type>
//CArray1D<Type>::CArray1D( void ):
//	CData(false)
//{
//	array_size = 0;
//}

template<typename Type>
Type			& MCSAS::Types::CUDATypes::CArray1D<Type>::operator[]( const size_t idx )
{
	if (GetDataType() == true	&&
		idx != 0)
	{
		return *GetRawPointer();
	}
	if (idx >= GetLength())
	{
		return *GetRawPointer();
	}
	return CData::operator [](idx);
}

template<typename Type>
MCSAS::Types::CUDATypes::CArray1D<Type>::CArray1D( const CArray1D& copy_array )
{
	if (&copy_array == this)
		return;
	else
	{
		MallocBySize(copy_array.GetSize());
		CopyFrom(copy_array);
	}
}


template class MCSAS::Types::CUDATypes::CArray1D<char>;
template class MCSAS::Types::CUDATypes::CArray1D<char2>;
template class MCSAS::Types::CUDATypes::CArray1D<char3>;
template class MCSAS::Types::CUDATypes::CArray1D<char4>;
template class MCSAS::Types::CUDATypes::CArray1D<size_t>;
template class MCSAS::Types::CUDATypes::CArray1D<uint2>;
template class MCSAS::Types::CUDATypes::CArray1D<uint3>;
template class MCSAS::Types::CUDATypes::CArray1D<uint4>;
template class MCSAS::Types::CUDATypes::CArray1D<int>;
template class MCSAS::Types::CUDATypes::CArray1D<int2>;
template class MCSAS::Types::CUDATypes::CArray1D<int3>;
template class MCSAS::Types::CUDATypes::CArray1D<int4>;
template class MCSAS::Types::CUDATypes::CArray1D<float>;
template class MCSAS::Types::CUDATypes::CArray1D<float2>;
template class MCSAS::Types::CUDATypes::CArray1D<float3>;
template class MCSAS::Types::CUDATypes::CArray1D<float4>;
template class MCSAS::Types::CUDATypes::CArray1D<double>;
template class MCSAS::Types::CUDATypes::CArray1D<double2>;
template class MCSAS::Types::CUDATypes::CArray1D<double3>;
template class MCSAS::Types::CUDATypes::CArray1D<double4>;