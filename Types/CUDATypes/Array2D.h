#pragma once
#include "Data.h"

namespace MCSAS
{
	namespace Types
	{
		namespace CUDATypes
		{
			template<typename Type>
			class CArray2D :
				public CData<Type>
			{
			private:
				size_t			array_size;			// Размер массива (в байтах)
				size_t			length0;			
				size_t			length1;
				size_t			pitch;
			public:
				size_t			GetLength0() const;
				size_t			GetLength1() const;
				size_t			GetPitch() const;
				size_t			GetSize() const;

				void			MallocByLength(const size_t ilength0, const size_t ilength1);
				void			CopyFrom(const CArray2D<Type>& inputArray);
				void			CopyFrom(const Type* const src_ptr, const size_t ilength0, const size_t ilength1, const size_t ipitch, bool data_type = false);
				CArray2D(const bool data_type);
				~CArray2D(void);
				void			Clear();
				Type&			Element(size_t idx1, size_t idx0);
				Type*			Element(size_t idx1);
				CArray2D(const CArray2D&);
			};
		}
	}
}