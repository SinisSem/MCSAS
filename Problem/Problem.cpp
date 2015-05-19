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

template<typename Type>
MCSAS::Problem::CProblem<Type>::CProblem()
{
	cudaGetDeviceProperties(&DevProp, 0);	
}

template<typename Type>
MCSAS::Problem::CProblem<Type>::~CProblem()
{

}	

template<typename Type>
void MCSAS::Problem::CProblem<Type>::FormulateProblem( const Types::COO_matrix<Type> Matrix, const std::vector<Type> RightVector )
{
	size_t matrixElementsNumber = Matrix.Vals.size();
	size_t warpSize = DevProp.warpSize;
	COO_matrix<Type> ChainMatrix;

	size_t ChainsNumber = (size_t)ceil((double)(Matrix.GetRows())/(double)warpSize);

	for(size_t chainIdx = 0; chainIdx < ChainsNumber; chainIdx++)
	{
		Chain<Type> CurrentChain;
		CurrentChain.StartNode = chainIdx * warpSize;
		CurrentChain.NodesNumber = warpSize;
		size_t ChainLastRow = CurrentChain.StartNode + CurrentChain.NodesNumber;

		// 1. Распарсим правый вектор на цепочки
		CurrentChain.RightVector.assign(RightVector.data() + CurrentChain.StartNode, RightVector.data() + ChainLastRow);
		while(CurrentChain.RightVector.size() < warpSize)
		{
			Type Val;
			SetValue(Val, 0, 0);
			CurrentChain.RightVector.push_back(Val);
		}

		// 2. Распарсим матрицу на цепочки
		// 2.1 Раскидаем всю матрицу по матрицам цепочек
		vector<size_t>::const_iterator firstIdx_in_chain = find_if(Matrix.Rows.begin(), Matrix.Rows.end(), 
			[&](const size_t& a)->bool
			{return (a >= CurrentChain.StartNode);});
		vector<size_t>::const_iterator lastIdx_in_chain = find_if(Matrix.Rows.begin(), Matrix.Rows.end(), 
			[&](const size_t& a)->bool
			{return (a >= ChainLastRow);});

		size_t FirstChainMtxElIdx = distance(Matrix.Rows.begin(), firstIdx_in_chain);
		size_t AmountChainMtxElIdx = distance(firstIdx_in_chain, lastIdx_in_chain);

		ChainMatrix.Rows.assign(Matrix.Rows.data() + FirstChainMtxElIdx, Matrix.Rows.data() + FirstChainMtxElIdx + AmountChainMtxElIdx);
		ChainMatrix.Cols.assign(Matrix.Cols.data() + FirstChainMtxElIdx, Matrix.Cols.data() + FirstChainMtxElIdx + AmountChainMtxElIdx);
		ChainMatrix.Vals.assign(Matrix.Vals.data() + FirstChainMtxElIdx, Matrix.Vals.data() + FirstChainMtxElIdx + AmountChainMtxElIdx);

		while(ChainMatrix.Rows.size() < warpSize)
		{
			Type Val;
			SetValue(Val, 1, 0);
			size_t ridx = ChainMatrix.Rows[ChainMatrix.Rows.size() - 1];
			ChainMatrix.PushElement(Val, ridx + 1, ridx + 1);
		}

		// 2.2 Раскидаем матрицы цепочек на связь
		Type Val;
		SetValue(Val, 0, 0);
		CurrentChain.b.assign(warpSize, Val);
		CurrentChain.a.assign(warpSize - 1, Val);
		CurrentChain.c.assign(warpSize - 1, Val);

		for(size_t RowIdx = 0; RowIdx < warpSize; RowIdx++)
		{
			vector<size_t>::iterator firstIdx_in_row = find_if(ChainMatrix.Rows.begin(), ChainMatrix.Rows.end(), 
				[&](const size_t& a)->bool
			{return (a == RowIdx);});
			vector<size_t>::iterator lastIdx_in_row = find_if(ChainMatrix.Rows.begin(), ChainMatrix.Rows.end(), 
				[&](const size_t& a)->bool
			{return (a > RowIdx);});
			if (firstIdx_in_row == ChainMatrix.Rows.end())
				throw PException("Обнаружен пустой ряд!");

			size_t FirstRowMtxElIdx = distance(ChainMatrix.Rows.begin(), firstIdx_in_row);
			size_t LastRowMtxElIdx = distance(ChainMatrix.Rows.begin(), lastIdx_in_row);

			std::vector<size_t> index(LastRowMtxElIdx - FirstRowMtxElIdx);
			iota(index.begin(), index.end(), 0);			// Не самая лучшая реализация, но заполнять в стиле 1, 2, 3... в цикле не охота

			// Распарсим элемент b
			vector<size_t>::const_iterator diagIdx = find_if(index.begin(), index.end(), 
				[=](const size_t a)->bool
				{return (ChainMatrix.Rows[a + FirstRowMtxElIdx] == ChainMatrix.Cols[a + FirstRowMtxElIdx]);});
			if (diagIdx == index.end())
			{
				Type Val;
				SetValue(Val, 0, 0);
				CurrentChain.b[RowIdx] = Val;
			}
			else
				CurrentChain.b[RowIdx] = ChainMatrix.Vals[*diagIdx + FirstRowMtxElIdx];
			
			// Распарсим элемент a
			vector<size_t>::const_iterator aDiagIdx = index.end();
			if (RowIdx != 0)
			{
				aDiagIdx = find_if(index.begin(), index.end(), 
					[=](const size_t a)->bool
					{return (ChainMatrix.Rows[a + FirstRowMtxElIdx] == ChainMatrix.Cols[a + FirstRowMtxElIdx] + 1);});
				if (aDiagIdx == index.end())
				{
					Type Val;
					SetValue(Val, 0, 0);
					CurrentChain.a[RowIdx-1] = Val;
				}
				else
					CurrentChain.a[RowIdx-1] = ChainMatrix.Vals[*aDiagIdx + FirstRowMtxElIdx];
			}

			// Распарсим элемент c
			vector<size_t>::const_iterator cDiagIdx = index.end();
			if (RowIdx != warpSize - 1)
			{
				cDiagIdx = find_if(index.begin(), index.end(), 
					[=](const size_t a)->bool
					{return (ChainMatrix.Rows[a + FirstRowMtxElIdx] + 1 == ChainMatrix.Cols[a + FirstRowMtxElIdx]);});
				if (cDiagIdx == index.end())
				{
					Type Val;
					SetValue(Val, 0, 0);
					CurrentChain.c[RowIdx] = Val;
				}
				else
					CurrentChain.c[RowIdx] = ChainMatrix.Vals[*cDiagIdx + FirstRowMtxElIdx];
			}

			// Все остальное - закинуть в линки
			for(vector<size_t>::iterator idxIterator = index.begin(); idxIterator < index.end(); idxIterator++)
			{
				if (idxIterator == diagIdx ||
					idxIterator == aDiagIdx||
					idxIterator == cDiagIdx)
					continue;

				Link<Type> Lnk;
				Lnk.Value = ChainMatrix.Vals[*idxIterator + FirstRowMtxElIdx];
				Lnk.RowNumber = ChainMatrix.Rows[*idxIterator + FirstRowMtxElIdx];
				Lnk.ColNumber = ChainMatrix.Cols[*idxIterator + FirstRowMtxElIdx];
				CurrentChain.Links.push_back(Lnk);
			}
		}

		m_Chains.push_back(CurrentChain);
	}

}



template class CProblem<float>;
template class CProblem<double>;
template class CProblem<MCSAS::Types::BaseTypes::ComplexDoubleCartesian>;
template class CProblem<MCSAS::Types::BaseTypes::ComplexDoubleHybrid>;
template class CProblem<MCSAS::Types::BaseTypes::ComplexDoublePolar>;
template class CProblem<MCSAS::Types::BaseTypes::ComplexFloatCartesian>;
template class CProblem<MCSAS::Types::BaseTypes::ComplexFloatHybrid>;
template class CProblem<MCSAS::Types::BaseTypes::ComplexFloatPolar>;