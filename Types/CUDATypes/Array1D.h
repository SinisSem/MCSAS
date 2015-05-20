#pragma once
#include "Data.h"

namespace MCSAS
{
	namespace Types
	{
		namespace CUDATypes
		{
			template<typename Type>
			class CArray1D :
				public CData<Type>
			{
			private:
				size_t			array_size;			// Размер массива (в байтах)

			public:
				void			CopyFrom(const Type* const src_ptr, const size_t size, bool data_type = false);
			public:
				size_t			GetLength() const;
				size_t			GetSize() const;

				void			MallocBySize(const size_t size);
				void			MallocByLength(const size_t length);
				void			CopyFrom(const CArray1D<Type>& inputArray);
				//CArray1D(void);
				CArray1D(const bool data_type);
				~CArray1D(void);
				void			Clear();
				Type			&operator[](const size_t);
				CArray1D(const CArray1D&);
			};
		}
	}
}