#include "Data.h"
//#include "Data_Utils.h"

// Конструктор
template<typename Type>
MCSAS::Types::CUDATypes::CData<Type>::CData( bool data_type = false )
{
	raw_pointer = nullptr;
	device_data = data_type;
}

// Деструктор (виртуальный - на всякий случай)
template<typename Type>
MCSAS::Types::CUDATypes::CData<Type>::~CData( void )
{
	Clear();
}

// Очистка родных данных - не паримся в наследниках
template<typename Type>
void MCSAS::Types::CUDATypes::CData<Type>::Clear()
{
	if (device_data == true)
		cudaFree(raw_pointer);
	else
		free(raw_pointer);
	raw_pointer = nullptr;
}

template<typename Type>
Type & MCSAS::Types::CUDATypes::CData<Type>::operator[]( const size_t data_idx )
{
	return raw_pointer[data_idx];
}
template<typename Type>
Type* MCSAS::Types::CUDATypes::CData<Type>::GetRawPointer() const
{
	return raw_pointer;
}

template<typename Type>
bool MCSAS::Types::CUDATypes::CData<Type>::GetDataType() const
{
	return device_data;
}

template class MCSAS::Types::CUDATypes::CData<char>;
template class MCSAS::Types::CUDATypes::CData<char2>;
template class MCSAS::Types::CUDATypes::CData<char3>;
template class MCSAS::Types::CUDATypes::CData<char4>;
template class MCSAS::Types::CUDATypes::CData<size_t>;
template class MCSAS::Types::CUDATypes::CData<uint2>;
template class MCSAS::Types::CUDATypes::CData<uint3>;
template class MCSAS::Types::CUDATypes::CData<uint4>;
template class MCSAS::Types::CUDATypes::CData<int>;
template class MCSAS::Types::CUDATypes::CData<int2>;
template class MCSAS::Types::CUDATypes::CData<int3>;
template class MCSAS::Types::CUDATypes::CData<int4>;
template class MCSAS::Types::CUDATypes::CData<float>;
template class MCSAS::Types::CUDATypes::CData<float2>;
template class MCSAS::Types::CUDATypes::CData<float3>;
template class MCSAS::Types::CUDATypes::CData<float4>;
template class MCSAS::Types::CUDATypes::CData<double>;
template class MCSAS::Types::CUDATypes::CData<double2>;
template class MCSAS::Types::CUDATypes::CData<double3>;
template class MCSAS::Types::CUDATypes::CData<double4>;