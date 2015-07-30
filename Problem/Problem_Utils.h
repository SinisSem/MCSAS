template<typename Type>
struct Diag
{
	Type*	a;
	Type*	b;
	Type*	c;
};

template<typename Type>
struct Link
{
	size_t*	idx;
	size_t*	row_idx;
	size_t*	col_idx;
	Type*	val;
};

template<typename Type>
struct Right
{
	size_t*	idx;
	size_t*	row_idx;
	Type*	val;
};

template<typename Type>
struct Vector_D
{
	size_t*	idx;
	size_t*	col_idx;
	Type*	val;
	Type*	X;
};

template<typename Type>
struct Chains
{
	size_t		chains_number;
	size_t*		chains_idx_in_blocks;
	size_t*		chains_num_in_blocks;	
};

/* Структура, из которой будут выведены данные для расчета на GPU*/
template<typename Type>
struct KernelGPUData					
{
	size_t			blocks_number;
	Diag<Type>		diag;
	Link<Type>		link;
	Right<Type>		right;
	Vector_D<Type>	vector_d;
	Chains<Type>	chains;
};

#include "Arrays/Array1D.h"
#include "Arrays/Array2D.h"
#include <vector>
#include "MatrixGenerator.h"

#define RealType float

#define CREATENUMERICTEMPLATESOF(CCLASS)\
	template class CCLASS<float>;\
	template class CCLASS<double>;
//	template class CCLASS<float2>;
//	template class CCLASS<double2>;

