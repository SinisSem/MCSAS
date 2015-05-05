#include "MatrixGenerator.h"
#include <random>
#include <ctime>
#include <algorithm>
#include <numeric>
#include <vector>
#include "MGException.h"
#include "../Types/BaseTypes.h"
using namespace std;
using namespace MCSAS;
using namespace MatrixGenerator;

template<typename Type>
CMatrixGenerator<Type>::CMatrixGenerator()
{
	std::srand((size_t)std::time(0));
	m_HasTask = false;
}

template<typename Type>
CMatrixGenerator<Type>::~CMatrixGenerator()
{
}

/*Возвращает то задание, которое введено на данный момент*/
template<typename Type>
MGTask<Type> CMatrixGenerator<Type>::GetTask()
{
	return m_Task;
}

template<typename Type>
void CMatrixGenerator<Type>::SetTask(const MGTask<Type> inTask)
{
	if (inTask.ChainNodesNumber.empty() == true)
		throw MGException("С пустым набором цепей не начинать");		// С пустым набором цепей не начинать
//	if (inTask.BaseAdmittance == 0)
//		throw MGException("Базовая проводимость не может быть == 0");		// Базовая проводимость не может быть == 0
//	if (inTask.BaseMainDiagonalAddition == 0)
//		throw MGException("Базовая добавка для диагонали не может быть == 0");		// Базовая добавка для диагонали не может быть == 0
//	if (inTask.CheckConductivity == true									&&
//		inTask.RandomNetAdmittancesNumber < inTask.ChainNodesNumber.size() - 1)
//		throw MGException("Число случайных связей - хотя бы == числу цепочек - 1");		// Число случайных связей - хотя бы == числу цепочек - 1
	for (size_t idx = 0; idx < inTask.ChainNodesNumber.size(); idx++)
	{
		if (inTask.ChainNodesNumber[idx] < 2)
			throw MGException("В цепочке должно быть хотябы 2 узла");	// В цепочке должно быть хотябы 2 узла
	}

	m_Task = inTask;
	m_HasTask = true;

//	return GenerateInternal();
}

//template<typename Type>
//MCSASMGResult 
//	CMatrixGenerator<Type>::GenerateInternal()
//{
//	if (m_HasTask == false)
//		return MCSASMG_UNSUFFICIENT_TASK;
//
//	m_Chains.clear();
//
//	MCSASMGResult Ret;
//
//	Ret = CreateChains();
//	if (Ret != MCSASMG_SUCCESS)
//		return Ret;
//
//	Ret = CreateRandomLinks();
//	if (Ret != MCSASMG_SUCCESS)
//		return Ret;
//
//	Ret = CreateEDS();
//	if (Ret != MCSASMG_SUCCESS)
//		return Ret;
//
//	return Ret;
//}
//
//unsigned int
//	CMatrixGenerator::GetNodesNumber()
//{
//	int NodesNumber = 0;
//	for (size_t chainIdx = 0; chainIdx < m_Task.ChainNodesNumber.size(); chainIdx++)
//		NodesNumber += m_Task.ChainNodesNumber[chainIdx];
//
//	return NodesNumber;
//}
//
//MCSASMGResult 
//	CMatrixGenerator::CreateChains()
//{
//	int nodeIdx = 0;
//	for (size_t chainIdx = 0; chainIdx < m_Task.ChainNodesNumber.size(); chainIdx++)
//	{
//		Chain Current;
//		Current.NodesNumber = m_Task.ChainNodesNumber[chainIdx];
//		vector<double> RightVector(Current.NodesNumber, 0.0);
//		Current.RightVector = RightVector;
//		Current.StartNode = nodeIdx;
//
//		// Создадим вектор a
//		for (int a_element = 0; a_element < Current.NodesNumber - 1; a_element++)
//			Current.a.push_back(GetRandomAdmittance());
//
//		// Скопируем a в c
//		Current.c = Current.a;
//
//		// Cкопируем с в b
//		Current.b = Current.c;
//		Current.b.push_back(0.0);
//
//		// Сформируем окончательный вектор b
//		for (int a_element = 0; a_element < Current.NodesNumber - 1; a_element++)
//		{
//			Current.b[a_element + 1] += Current.a[a_element];
//			double NewAddition = GetRandomAddition();
//			Current.b[a_element + 1] += NewAddition;
//			Current.b[a_element] += NewAddition;
//		}
//		for (int b_element = 0; b_element < Current.NodesNumber; b_element++)
//			Current.b[b_element] = -Current.b[b_element];
//
//		nodeIdx += Current.NodesNumber;
//		m_Chains.push_back(Current);
//	}
//	return MCSASMG_SUCCESS;
//}
//
//double
//	CMatrixGenerator::GetRandomAdmittance()
//{
//	return MCSASMGUtility::RandomDouble(m_Task.BaseAdmittance, m_Task.AdmittancesDispersion);
//}
//
//double
//	CMatrixGenerator::GetRandomAddition()
//{
//	return MCSASMGUtility::RandomDouble(m_Task.BaseMainDiagonalAddition, m_Task.AdditionDispersion);
//}
//
//vector<int>
//	CMatrixGenerator::RandomVectorInsert(int iVectorSize, int iElementsNumber)
//{
//	vector<int> oRes;
//	vector<int> Vec(iVectorSize);
//	iota(Vec.begin(), Vec.end(), 0);
//	int ElementsLeft = iElementsNumber;
//	while (ElementsLeft > 0)
//	{
//		int SetElement = MCSASMGUtility::RandomInt(0, Vec.size());
//		oRes.push_back(Vec[SetElement]);
//		Vec.erase(Vec.begin() + SetElement);
//		ElementsLeft--;
//	}
//
//	return oRes;
//}
//
//void
//	CMatrixGenerator::RandomPairVectorInsert(const int ElementsNumber, vector<int>& oVector1, vector<int>& oVector2)
//{
//	oVector1.clear();
//	oVector2.clear();
//
//	int AddedElements = 0;
//	int SetElement1;
//	int SetElement2;
//	bool duplicate;
//
//	int iVectorSize = GetNodesNumber();
//
//	do
//	{
//		duplicate = false;
//		SetElement1 = MCSASMGUtility::RandomInt(0, iVectorSize - 1);
//		SetElement2 = MCSASMGUtility::RandomInt(0, iVectorSize - 1);
//		for (size_t element_idx = 0; element_idx < oVector1.size(); element_idx++)
//		{
//			int Pair1In = oVector1[element_idx];	int Pair2In = SetElement1;
//			int Pair1To = oVector2[element_idx];	int Pair2To = SetElement2;
//
//			if ((Pair1In == Pair2To &&	Pair1To == Pair2In) ||
//				(Pair1In == Pair2In &&	Pair1To == Pair2To))
//			{
//				duplicate = true;
//				break;
//			}
//		}
//		if (duplicate == false)
//		{
//			oVector1.push_back(SetElement1);
//			oVector2.push_back(SetElement2);
//			AddedElements++;
//		}
//	}while (AddedElements < ElementsNumber);
//}
//
//MCSASMGResult 
//	CMatrixGenerator::CreateRandomLinks()
//{
//	// Добавим случайные связи
//	vector<int> NodesIn, NodesTo;
//	RandomPairVectorInsert(m_Task.RandomNetAdmittancesNumber, NodesIn, NodesTo);
//
//	for (size_t insertIdx = 0; insertIdx < NodesTo.size(); insertIdx++)
//	{
//		double AddAdmittance = GetRandomAdmittance();
//		double AddAddition = GetRandomAddition();
//
//		Link NewLinkIn;
//		NewLinkIn.RowNumber = NodesIn[insertIdx];
//		NewLinkIn.ColNumber = NodesTo[insertIdx];
//		NewLinkIn.Value = AddAdmittance;
//
//		Link NewLinkTo;
//		NewLinkTo.RowNumber = NodesTo[insertIdx];
//		NewLinkTo.ColNumber = NodesIn[insertIdx];
//		NewLinkTo.Value = AddAdmittance;
//
//		int added = 0;
//
//		int ChainIn;
//		int ChainInRow;
//		int ChainTo;
//		int ChainToRow;
//		// Вставим случайные связи в соответствующие цепи
//		for (size_t chainIdx = 0; chainIdx < m_Chains.size(); chainIdx++)
//		{
//			int StartNode = m_Chains[chainIdx].StartNode;
//			int EndNode = m_Chains[chainIdx].StartNode + m_Chains[chainIdx].NodesNumber - 1;
//			if (NewLinkIn.RowNumber >= StartNode		&&
//				NewLinkIn.RowNumber <= EndNode)
//			{
//				ChainIn = chainIdx;
//				ChainInRow = NewLinkIn.RowNumber - StartNode;
//				//				m_Chains[chainIdx].Links.push_back(NewLinkIn);
//				//				m_Chains[chainIdx].b[NewLinkIn.RowNumber - StartNode] -= NewLinkIn.Value + AddAddition;
//				added++;
//			}
//			if (NewLinkTo.RowNumber >= StartNode		&&
//				NewLinkTo.RowNumber <= EndNode)
//			{
//				ChainTo = chainIdx;
//				ChainToRow = NewLinkTo.RowNumber - StartNode;
//				//				m_Chains[chainIdx].Links.push_back(NewLinkTo);
//				//				m_Chains[chainIdx].b[NewLinkTo.RowNumber - StartNode] -= NewLinkTo.Value + AddAddition;
//				added++;
//			}
//		}
//
//		if (added != 2)
//			return MCSASMG_UNKNOWN_ERROR;
//
//		if (m_Task.MaximumLinksPerChain > 1)
//		{
//			if (m_Chains[ChainIn].Links.size() < m_Task.MaximumLinksPerChain	&&
//				m_Chains[ChainTo].Links.size() < m_Task.MaximumLinksPerChain	)
//			{
//				if (ChainIn == ChainTo)
//				{
//					if ((m_Chains[ChainIn].Links.size() + 1) < m_Task.MaximumLinksPerChain)
//					{
//						m_Chains[ChainIn].Links.push_back(NewLinkIn);
//						m_Chains[ChainIn].b[ChainInRow] -= NewLinkIn.Value + AddAddition;
//
//						m_Chains[ChainTo].Links.push_back(NewLinkIn);
//						m_Chains[ChainTo].b[ChainToRow] -= NewLinkIn.Value + AddAddition;
//					}
//				}
//				else
//				{
//					m_Chains[ChainIn].Links.push_back(NewLinkIn);
//					m_Chains[ChainIn].b[ChainInRow] -= NewLinkIn.Value + AddAddition;
//
//					m_Chains[ChainTo].Links.push_back(NewLinkIn);
//					m_Chains[ChainTo].b[ChainToRow] -= NewLinkIn.Value + AddAddition;
//				}
//			}
//		}
//
//	}
//	vector<int> Result;
//	size_t Res = 0;
//	for(size_t chain_idx = 0; chain_idx < m_Chains.size(); chain_idx++)
//	{
//		Res += m_Chains[chain_idx].Links.size();
//		Result.push_back(m_Chains[chain_idx].Links.size());
//	}
//	Res /= 2;
//	return MCSASMG_SUCCESS;
//}
//
//MCSASMGResult 
//	CMatrixGenerator::CreateEDS()
//{
//	int NodesNumber = GetNodesNumber();
//	vector<int> WhereEDS = RandomVectorInsert(NodesNumber, (size_t)(NodesNumber * m_Task.EDSNumber));
//
//	for (size_t addIdx = 0; addIdx < WhereEDS.size(); addIdx++)
//	{
//		double EDSValue = GetRandomEDS();
//		double AddAdmittance = GetRandomEDSAdmittance();
//		double AddAddition = GetRandomAddition();
//
//		for (size_t chainIdx = 0; chainIdx < m_Chains.size(); chainIdx++)
//		{
//			int StartNode = m_Chains[chainIdx].StartNode;
//			int EndNode = m_Chains[chainIdx].StartNode + m_Chains[chainIdx].NodesNumber - 1;
//			if (WhereEDS[addIdx] >= StartNode		&&
//				WhereEDS[addIdx] <= EndNode)
//			{
//				int chainAddIdx = WhereEDS[addIdx] - StartNode;
//				m_Chains[chainIdx].b[chainAddIdx] -= AddAdmittance;
//				m_Chains[chainIdx].RightVector[chainAddIdx] = -EDSValue;
//				break;
//			}
//		}
//	}
//	return MCSASMG_SUCCESS;
//}
//
//double 
//	CMatrixGenerator::GetRandomEDS()
//{
//	return MCSASMGUtility::RandomDouble(m_Task.EDSBase, m_Task.EDSDispersion);
//}
//
//double
//	CMatrixGenerator::GetRandomEDSAdmittance()
//{
//	return MCSASMGUtility::RandomDouble(m_Task.EDSAdmittanceBase, m_Task.EDSAdmittanceDispersion);
//}
//
//CPUData 
//	CMatrixGenerator::GetCPUChain()
//{
//	CPUData NewData;
//	for (size_t ChainIdx = 0; ChainIdx < m_Chains.size(); ChainIdx++)
//		NewData.Chains.push_back(GetCPUChain(ChainIdx));
//	return NewData;
//}
//
//CPUChain
//	CMatrixGenerator::GetCPUChain(size_t ChainIdx)
//{
//	Chain Chain1 = m_Chains[ChainIdx];
//	CPUChain ChainOut;
//
//	// diag
//	for (size_t elementIdx = 0; elementIdx < Chain1.a.size(); elementIdx++)
//		ChainOut.a.push_back(Chain1.a[elementIdx]);
//
//	for (size_t elementIdx = 0; elementIdx < Chain1.b.size(); elementIdx++)
//		ChainOut.b.push_back(Chain1.b[elementIdx]);
//
//	for (size_t elementIdx = 0; elementIdx < Chain1.c.size(); elementIdx++)
//		ChainOut.c.push_back(Chain1.c[elementIdx]);
//
//	// link
//	vector<int>			Idx;
//	vector<int>			Cols;
//	vector<int>			Rows;
//	vector<double>		Vals;
//	vector<int>			SortedRows;
//	vector<int>			SortedCols;
//	vector<double>		SortedVals;
//
//	for (size_t LinkIdx = 0; LinkIdx < Chain1.Links.size(); LinkIdx++)
//	{
//		Cols.push_back(Chain1.Links[LinkIdx].ColNumber);
//		Rows.push_back(Chain1.Links[LinkIdx].RowNumber);
//		Vals.push_back(Chain1.Links[LinkIdx].Value);
//	}
//	
//	// Сортируем по рядам
//	MCSASMGUtility::SortTwoVectors(Rows, Idx, SortedRows);
//	for (size_t LinkIdx = 0; LinkIdx < Chain1.Links.size(); LinkIdx++)
//	{
//		SortedCols.push_back(Cols[Idx[LinkIdx]]);
//		SortedVals.push_back(Vals[Idx[LinkIdx]]);
//	}
//
//	// Сортируем по столбцам
//	MCSASMGUtility::SortTwoVectors(SortedCols, Idx, Cols);
//	for (size_t LinkIdx = 0; LinkIdx < Chain1.Links.size(); LinkIdx++)
//	{
//		Rows[LinkIdx] = SortedRows[Idx[LinkIdx]];
//		Vals[LinkIdx] = SortedVals[Idx[LinkIdx]];
//	}
//
//	for (size_t LinkIdx = 0; LinkIdx < Chain1.Links.size(); LinkIdx++)
//	{
//		ChainOut.LinksColIdx.push_back(Cols[LinkIdx]);
//		ChainOut.LinksRowIdx.push_back(Rows[LinkIdx]);
//		ChainOut.LinksVal.push_back(Vals[LinkIdx]);
//	}
//
//	// Right
//	for (size_t rightIdx = 0; rightIdx < Chain1.RightVector.size(); rightIdx++)
//	{
//		if (Chain1.RightVector[rightIdx] != 0.0)
//		{
//			ChainOut.RightVectorRowIdx.push_back(rightIdx);
//			ChainOut.RightVectorVal.push_back(Chain1.RightVector[rightIdx]);
//		}
//	}
//
//	// StartNode
//	ChainOut.StartNode = Chain1.StartNode;
//
//	return ChainOut;
//}
//
//COO_matrix 
//	CMatrixGenerator::GetCOOTriDiagMatrix()
//{
//	COO_matrix FullMatrix;
//	for (size_t ChainIdx = 0; ChainIdx < m_Chains.size(); ChainIdx++)
//	{
//		COO_matrix Matrix = GetCOOTriDiagMatrix(ChainIdx);
//		for (size_t elementIdx = 0; elementIdx < Matrix.Cols.size(); elementIdx++)
//		{
//			FullMatrix.Cols.push_back(Matrix.Cols[elementIdx]);
//			FullMatrix.Rows.push_back(Matrix.Rows[elementIdx]);
//			FullMatrix.Vals.push_back(Matrix.Vals[elementIdx]);
//		}
//	}
//	return FullMatrix;
//}
//
//vector<double>
//	CMatrixGenerator::GetRightVector()
//{
//	vector<double> FullVector;
//	for (size_t ChainIdx = 0; ChainIdx < m_Chains.size(); ChainIdx++)
//	{
//		vector<double> Vector = GetRightVector(ChainIdx);
//		for (size_t elementIdx = 0; elementIdx < Vector.size(); elementIdx++)
//			FullVector.push_back(Vector[elementIdx]);
//	}
//	return FullVector;
//}
//
//vector<double>
//	CMatrixGenerator::GetRightVector(size_t ChainIdx)
//{
//	return m_Chains[ChainIdx].RightVector;
//}
//
//COO_matrix
//	CMatrixGenerator::GetCOOMatrix()
//{
//	COO_matrix FullMatrix;
//	for (size_t ChainIdx = 0; ChainIdx < m_Chains.size(); ChainIdx++)
//	{
//		COO_matrix Matrix = GetCOOMatrix(ChainIdx);
//		for (size_t elementIdx = 0; elementIdx < Matrix.Cols.size(); elementIdx++)
//		{
//			FullMatrix.Cols.push_back(Matrix.Cols[elementIdx]);
//			FullMatrix.Rows.push_back(Matrix.Rows[elementIdx]);
//			FullMatrix.Vals.push_back(Matrix.Vals[elementIdx]);
//		}
//	}
//	return FullMatrix;
//}
//
//COO_matrix 
//	CMatrixGenerator::GetCOOTriDiagMatrix(size_t ChainIdx)
//{
//	Chain ChainOut = m_Chains[ChainIdx];
//	COO_matrix Matrix;
//
//	int Col = ChainOut.StartNode;
//	int Row = ChainOut.StartNode;
//	for (int b_element = 0; b_element < ChainOut.b.size(); b_element++)
//	{
//		Matrix.Cols.push_back(Col);
//		Matrix.Rows.push_back(Row);
//		Matrix.Vals.push_back(ChainOut.b[b_element]);
//		Col++;
//
//		if (b_element == ChainOut.b.size() - 1)
//			break;
//
//		Matrix.Cols.push_back(Col);
//		Matrix.Rows.push_back(Row);
//		Matrix.Vals.push_back(ChainOut.c[b_element]);
//		Col -= 1; Row++;
//		Matrix.Cols.push_back(Col);
//		Matrix.Rows.push_back(Row);
//		Matrix.Vals.push_back(ChainOut.a[b_element]);
//		Col++;
//	}
//
//	return Matrix;
//}
//
//COO_matrix
//	CMatrixGenerator::GetCOOMatrix(size_t ChainIdx)
//{
//	Chain ChainOut = m_Chains[ChainIdx];
//	COO_matrix Matrix;
//
//	Matrix = GetCOOTriDiagMatrix(ChainIdx);
//
//	for (int LinkIdx = 0; LinkIdx < ChainOut.Links.size(); LinkIdx++)
//	{
//		int irow_idx = ChainOut.Links[LinkIdx].RowNumber;
//		int icol_idx = ChainOut.Links[LinkIdx].ColNumber;
//		double ival = ChainOut.Links[LinkIdx].Value;
//
//		int rowStartIdx = -1;
//		for (int elementIdx = 0; elementIdx < Matrix.Rows.size(); elementIdx++)
//		{
//			if (irow_idx == Matrix.Rows[elementIdx])
//			{
//				rowStartIdx = elementIdx;
//				break;
//			}
//		}
//		int rowEndIdx = -1;
//		for (int elementIdx = rowStartIdx; elementIdx < Matrix.Rows.size(); elementIdx++)
//		{
//			if (irow_idx != Matrix.Rows[elementIdx])
//			{
//				rowEndIdx = elementIdx;
//				break;
//			}
//		}
//		if (rowEndIdx == -1)
//			rowEndIdx = Matrix.Rows.size();
//
//		if (icol_idx < Matrix.Cols[rowStartIdx])
//		{
//			Matrix.Vals.insert(Matrix.Vals.begin() + rowStartIdx, ival);
//			Matrix.Cols.insert(Matrix.Cols.begin() + rowStartIdx, icol_idx);
//			Matrix.Rows.insert(Matrix.Rows.begin() + rowStartIdx, irow_idx);
//			continue;
//		}
//		if (icol_idx > Matrix.Cols[rowEndIdx - 1])
//		{
//			Matrix.Vals.insert(Matrix.Vals.begin() + rowEndIdx, ival);
//			Matrix.Cols.insert(Matrix.Cols.begin() + rowEndIdx, icol_idx);
//			Matrix.Rows.insert(Matrix.Rows.begin() + rowEndIdx, irow_idx);
//			continue;
//		}
//		for (int elementIdx = rowStartIdx; elementIdx < rowEndIdx - 1; elementIdx++)
//		{
//			if (icol_idx > Matrix.Cols[elementIdx] &&
//				icol_idx < Matrix.Cols[elementIdx + 1])
//			{
//				Matrix.Vals.insert(Matrix.Vals.begin() + elementIdx + 1, ival);
//				Matrix.Cols.insert(Matrix.Cols.begin() + elementIdx + 1, icol_idx);
//				Matrix.Rows.insert(Matrix.Rows.begin() + elementIdx + 1, irow_idx);
//				break;
//			}
//		}
//	}
//
//	return Matrix;
//}

template class CMatrixGenerator<float>;
template class CMatrixGenerator<double>;
template class CMatrixGenerator<MCSAS::Types::BaseTypes::ComplexDoubleCartesian>;
template class CMatrixGenerator<MCSAS::Types::BaseTypes::ComplexDoubleHybrid>;
template class CMatrixGenerator<MCSAS::Types::BaseTypes::ComplexDoublePolar>;
template class CMatrixGenerator<MCSAS::Types::BaseTypes::ComplexFloatCartesian>;
template class CMatrixGenerator<MCSAS::Types::BaseTypes::ComplexFloatHybrid>;
template class CMatrixGenerator<MCSAS::Types::BaseTypes::ComplexFloatPolar>;