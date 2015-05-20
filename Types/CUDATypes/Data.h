#pragma once
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "../TypeException.h"

namespace MCSAS
{
	namespace Types
	{
		namespace CUDATypes
		{
			template<typename Type>
			class CData
			{
			private:
				bool			device_data;	// 0 = host, 1 = device

			protected:
				Type*			raw_pointer;	// ”казатель на данные
			public:
				CData(const bool data_type = false);
				Type*	GetRawPointer() const;
				bool	GetDataType() const;

				virtual Type&	operator[](const size_t data_idx);
				virtual			~CData(void);
				virtual void	Clear(void);
			};
		}
	}
}