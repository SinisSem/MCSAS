#include "MatrixGenerator.h"
#include <random>
#include <ctime>
#include <algorithm>
#include <numeric>
#include <vector>
#include "MGException.h"
#include "../Types/BaseTypes.h"
#include "../Types/BaseTypesRandom.h"
using namespace std;
using namespace MCSAS;
using namespace MatrixGenerator;
using namespace Types;
using namespace BaseTypes;

template<typename Type>
CMatrixGenerator<Type>::CMatrixGenerator()
{
	BaseTypes::StartRandom();
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

	GenerateInternal();
}

template<typename Type>
void CMatrixGenerator<Type>::GenerateInternal()
{
	if (m_HasTask == false)
		throw MGException("There is no task");

	m_Chains.clear();

	CreateChains();

	CreateRandomLinks();

	CreateEDS();
}

template<typename Type>
size_t CMatrixGenerator<Type>::GetNodesNumber()
{
	size_t NodesNumber = 0;
	for (size_t chainIdx = 0; chainIdx < m_Task.ChainNodesNumber.size(); chainIdx++)
		NodesNumber += m_Task.ChainNodesNumber[chainIdx];
	return NodesNumber;
}

template<typename Type>
void CMatrixGenerator<Type>::CreateChains()
{
	size_t nodeIdx = 0;
	for (size_t chainIdx = 0; chainIdx < m_Task.ChainNodesNumber.size(); chainIdx++)
	{
		Chain Current;
		Current.NodesNumber = m_Task.ChainNodesNumber[chainIdx];
		Type Element;
		BaseTypes::SetValue(Element, 0, 0);
		vector<Type> RightVector(Current.NodesNumber, Element);
		Current.RightVector = RightVector;
		Current.StartNode = nodeIdx;

		// Создадим вектор a
		for (size_t a_element = 0; a_element < Current.NodesNumber - 1; a_element++)
			Current.a.push_back(GetRandomAdmittance());

		// Скопируем a в c
		Current.c = Current.a;

		// Cкопируем с в b
		Current.b = Current.c;
		Current.b.push_back(Element);

		// Сформируем окончательный вектор b
		for (size_t a_element = 0; a_element < Current.NodesNumber - 1; a_element++)
		{
			Current.b[a_element + 1] = Add(Current.b[a_element + 1], Current.a[a_element]);
			Type NewAddition = GetRandomAddition();
			Current.b[a_element + 1] = Add(Current.b[a_element + 1], NewAddition);
			Current.b[a_element] = Add(Current.b[a_element], NewAddition);
		}
		for (size_t b_element = 0; b_element < Current.NodesNumber; b_element++)
			Current.b[b_element] = Sub(Element, Current.b[b_element]);

		nodeIdx += Current.NodesNumber;
		m_Chains.push_back(Current);
	}
}

template<typename Type>
Type CMatrixGenerator<Type>::GetRandomAdmittance()
{
	return BaseTypes::RandomBaseDispersion(m_Task.BaseAdmittance, m_Task.AdmittancesDispersion);
}

template<typename Type>
Type CMatrixGenerator<Type>::GetRandomAddition()
{
	return BaseTypes::RandomBaseDispersion(m_Task.BaseMainDiagonalAddition, m_Task.AdditionDispersion);
}

template<typename Type>
vector<size_t>	CMatrixGenerator<Type>::RandomVectorInsert(size_t iVectorSize, size_t iElementsNumber)
{
	vector<size_t> oRes;
	vector<size_t> Vec(iVectorSize);
	iota(Vec.begin(), Vec.end(), 0);
	size_t ElementsLeft = iElementsNumber;
	while (ElementsLeft != 0)
	{
		size_t SetElement = BaseTypes::RandomFromTo((size_t)0, (size_t)Vec.size());
		oRes.push_back(Vec[SetElement]);
		Vec.erase(Vec.begin() + SetElement);
		ElementsLeft--;
	}
	return oRes;
}

template<typename Type>
void CMatrixGenerator<Type>::RandomPairVectorInsert(const size_t ElementsNumber, vector<size_t>& oVector1, vector<size_t>& oVector2)
{
	oVector1.clear();
	oVector2.clear();

	size_t AddedElements = 0;
	size_t SetElement1;
	size_t SetElement2;
	bool duplicate;

	size_t iVectorSize = GetNodesNumber();

	do
	{
		duplicate = false;
		SetElement1 = BaseTypes::RandomFromTo((size_t)0, (size_t)(iVectorSize - 1));
		SetElement2 = BaseTypes::RandomFromTo((size_t)0, (size_t)(iVectorSize - 1));
		for (size_t element_idx = 0; element_idx < oVector1.size(); element_idx++)
		{
			size_t Pair1In = oVector1[element_idx];	size_t Pair2In = SetElement1;
			size_t Pair1To = oVector2[element_idx];	size_t Pair2To = SetElement2;

			if ((Pair1In == Pair2To &&	Pair1To == Pair2In) ||
				(Pair1In == Pair2In &&	Pair1To == Pair2To))
			{
				duplicate = true;
				break;
			}
		}
		if (duplicate == false)
		{
			oVector1.push_back(SetElement1);
			oVector2.push_back(SetElement2);
			AddedElements++;
		}
	}while (AddedElements < ElementsNumber);
}

template<typename Type>
void CMatrixGenerator<Type>::CreateRandomLinks()
{
	// Добавим случайные связи
	vector<size_t> NodesIn, NodesTo;
	RandomPairVectorInsert(m_Task.RandomNetAdmittancesNumber, NodesIn, NodesTo);

	for (size_t insertIdx = 0; insertIdx < NodesTo.size(); insertIdx++)
	{
		Type AddAdmittance = GetRandomAdmittance();
		Type AddAddition = GetRandomAddition();

		Link NewLinkIn;
		NewLinkIn.RowNumber = NodesIn[insertIdx];
		NewLinkIn.ColNumber = NodesTo[insertIdx];
		NewLinkIn.Value = AddAdmittance;

		Link NewLinkTo;
		NewLinkTo.RowNumber = NodesTo[insertIdx];
		NewLinkTo.ColNumber = NodesIn[insertIdx];
		NewLinkTo.Value = AddAdmittance;

		size_t added = 0;

		size_t ChainIn;
		size_t ChainInRow;
		size_t ChainTo;
		size_t ChainToRow;

		// Вставим случайные связи в соответствующие цепи
		for (size_t chainIdx = 0; chainIdx < m_Chains.size(); chainIdx++)
		{
			size_t StartNode = m_Chains[chainIdx].StartNode;
			size_t EndNode = m_Chains[chainIdx].StartNode + m_Chains[chainIdx].NodesNumber - 1;
			
			if (NewLinkIn.RowNumber >= StartNode		&&
				NewLinkIn.RowNumber <= EndNode)
			{
				ChainIn = chainIdx;
				added++;
			}
			if (NewLinkTo.RowNumber >= StartNode		&&
				NewLinkTo.RowNumber <= EndNode)
			{
				ChainTo = chainIdx;
				added++;
			}
		}

		if (added != 2)
			throw MGException("Не было добавлено узлов");

		if (m_Chains[ChainIn].Links.size() <= m_Task.MaximumLinksPerChain	&&
			m_Chains[ChainTo].Links.size() <= m_Task.MaximumLinksPerChain)
		{
			ChainInRow = NewLinkIn.RowNumber - m_Chains[ChainIn].StartNode;
			m_Chains[ChainIn].Links.push_back(NewLinkIn);
			Type Val = Add(NewLinkIn.Value, AddAddition);
			m_Chains[ChainIn].b[NewLinkIn.RowNumber - m_Chains[ChainIn].StartNode] = Sub(m_Chains[ChainIn].b[NewLinkIn.RowNumber - m_Chains[ChainIn].StartNode], Val);

			ChainToRow = NewLinkTo.RowNumber - m_Chains[ChainTo].StartNode;
			m_Chains[ChainTo].Links.push_back(NewLinkTo);
			Val = Add(NewLinkTo.Value, AddAddition);
			m_Chains[ChainTo].b[NewLinkTo.RowNumber - m_Chains[ChainTo].StartNode] = Sub(m_Chains[ChainTo].b[NewLinkTo.RowNumber - m_Chains[ChainTo].StartNode], Val);
			//m_Chains[chainIdx].b[NewLinkTo.RowNumber - StartNode] -= NewLinkTo.Value + AddAddition;
		}
		//else
		//{
		//	added-=2;
		//}


		//m_Chains[chainIdx].b[NewLinkIn.RowNumber - StartNode] -= NewLinkIn.Value + AddAddition;

		//if (m_Task.MaximumLinksPerChain > 1)
		//{
		//	if (m_Chains[ChainIn].Links.size() < m_Task.MaximumLinksPerChain	&&
		//		m_Chains[ChainTo].Links.size() < m_Task.MaximumLinksPerChain	)
		//	{
		//		if (ChainIn == ChainTo)
		//		{
		//			if ((m_Chains[ChainIn].Links.size() + 1) < m_Task.MaximumLinksPerChain)
		//			{
		//				m_Chains[ChainIn].Links.push_back(NewLinkIn);
		//				Type Val = Add(NewLinkIn.Value, AddAddition);
		//				m_Chains[ChainIn].b[ChainInRow] = Sub(m_Chains[ChainIn].b[ChainInRow], Val);
		//				//m_Chains[ChainIn].b[ChainInRow] -= NewLinkIn.Value + AddAddition;

		//				m_Chains[ChainTo].Links.push_back(NewLinkIn);
		//				Val = Add(NewLinkIn.Value, AddAddition);
		//				m_Chains[ChainTo].b[ChainToRow] = Sub(m_Chains[ChainTo].b[ChainToRow], Val);
		//				//m_Chains[ChainTo].b[ChainToRow] -= NewLinkIn.Value + AddAddition;
		//			}
		//		}
		//		else
		//		{
		//			m_Chains[ChainIn].Links.push_back(NewLinkIn);
		//			Type Val = Add(NewLinkIn.Value, AddAddition);
		//			m_Chains[ChainIn].b[ChainInRow] = Sub(m_Chains[ChainIn].b[ChainInRow], Val);

		//			m_Chains[ChainTo].Links.push_back(NewLinkIn);
		//			//m_Chains[ChainTo].b[ChainToRow] -= NewLinkIn.Value + AddAddition;
		//			Val = Add(NewLinkIn.Value, AddAddition);
		//			m_Chains[ChainTo].b[ChainToRow] = Sub(m_Chains[ChainTo].b[ChainToRow], Val);
		//		}
		//	}
		//}

	}
	//vector<int> Result;
	//size_t Res = 0;
	//for(size_t chain_idx = 0; chain_idx < m_Chains.size(); chain_idx++)
	//{
	//	Res += m_Chains[chain_idx].Links.size();
	//	Result.push_back(m_Chains[chain_idx].Links.size());
	//}
	//Res /= 2;

	//size_t sum = 2;
	//for(size_t i = 0; i < m_Chains.size(); i++)
	//	sum+=m_Chains[i].Links.size();
	//sum--;
}

template<typename Type>
void CMatrixGenerator<Type>::CreateEDS()
{
	size_t NodesNumber = GetNodesNumber();
	vector<size_t> WhereEDS = RandomVectorInsert(NodesNumber, (size_t)(NodesNumber * m_Task.EDSNumber));

	for (size_t addIdx = 0; addIdx < WhereEDS.size(); addIdx++)
	{
		Type EDSValue = GetRandomEDS();
		Type AddAdmittance = GetRandomEDSAdmittance();
		Type AddAddition = GetRandomAddition();
		Type Zero;
		SetValue(Zero, 0, 0);

		for (size_t chainIdx = 0; chainIdx < m_Chains.size(); chainIdx++)
		{
			size_t StartNode = m_Chains[chainIdx].StartNode;
			size_t EndNode = m_Chains[chainIdx].StartNode + m_Chains[chainIdx].NodesNumber - 1;
			if (WhereEDS[addIdx] >= StartNode		&&
				WhereEDS[addIdx] <= EndNode)
			{
				size_t chainAddIdx = WhereEDS[addIdx] - StartNode;
				m_Chains[chainIdx].b[chainAddIdx] = Sub(m_Chains[chainIdx].b[chainAddIdx], AddAdmittance);
//				m_Chains[chainIdx].b[chainAddIdx] -= AddAdmittance;
				m_Chains[chainIdx].RightVector[chainAddIdx] = Sub(Zero, EDSValue);
//				m_Chains[chainIdx].RightVector[chainAddIdx] = -EDSValue;
				break;
			}
		}
	}
}

template<typename Type>
Type CMatrixGenerator<Type>::GetRandomEDS()
{
	return BaseTypes::RandomBaseDispersion(m_Task.EDSBase, m_Task.EDSDispersion);
}

template<typename Type>
Type CMatrixGenerator<Type>::GetRandomEDSAdmittance()
{
	return BaseTypes::RandomBaseDispersion(m_Task.EDSAdmittanceBase, m_Task.EDSAdmittanceDispersion);
}

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

template<typename Type>
COO_matrix<Type> CMatrixGenerator<Type>::GetCOOTriDiagMatrix()
{
	COO_matrix<Type> FullMatrix;
	for (size_t ChainIdx = 0; ChainIdx < m_Chains.size(); ChainIdx++)
	{
		COO_matrix<Type> Matrix = GetCOOTriDiagMatrix(ChainIdx);
		for (size_t elementIdx = 0; elementIdx < Matrix.Cols.size(); elementIdx++)
		{
			FullMatrix.Cols.push_back(Matrix.Cols[elementIdx]);
			FullMatrix.Rows.push_back(Matrix.Rows[elementIdx]);
			FullMatrix.Vals.push_back(Matrix.Vals[elementIdx]);
		}
	}
	return FullMatrix;
}

template<typename Type>
vector<Type> CMatrixGenerator<Type>::GetRightVector()
{
	vector<Type> FullVector;
	for (size_t ChainIdx = 0; ChainIdx < m_Chains.size(); ChainIdx++)
	{
		vector<Type> Vector = GetRightVector(ChainIdx);
		for (size_t elementIdx = 0; elementIdx < Vector.size(); elementIdx++)
			FullVector.push_back(Vector[elementIdx]);
	}
	return FullVector;
}

template<typename Type>
vector<Type> CMatrixGenerator<Type>::GetRightVector(size_t ChainIdx)
{
	return m_Chains[ChainIdx].RightVector;
}

template<typename Type>
COO_matrix<Type> CMatrixGenerator<Type>::GetCOOMatrix()
{
	COO_matrix<Type> FullMatrix;
	for (size_t ChainIdx = 0; ChainIdx < m_Chains.size(); ChainIdx++)
	{
		COO_matrix<Type> Matrix = GetCOOMatrix(ChainIdx);
		for (size_t elementIdx = 0; elementIdx < Matrix.Cols.size(); elementIdx++)
		{
			FullMatrix.Cols.push_back(Matrix.Cols[elementIdx]);
			FullMatrix.Rows.push_back(Matrix.Rows[elementIdx]);
			FullMatrix.Vals.push_back(Matrix.Vals[elementIdx]);
		}
	}
	return FullMatrix;
}

template<typename Type>
COO_matrix<Type> CMatrixGenerator<Type>::GetCOOTriDiagMatrix(size_t ChainIdx)
{
	Chain ChainOut = m_Chains[ChainIdx];
	COO_matrix<Type> Matrix;

	size_t Col = ChainOut.StartNode;
	size_t Row = ChainOut.StartNode;
	for (size_t b_element = 0; b_element < ChainOut.b.size(); b_element++)
	{
		Matrix.Cols.push_back(Col);
		Matrix.Rows.push_back(Row);
		Matrix.Vals.push_back(ChainOut.b[b_element]);
		Col++;

		if (b_element == ChainOut.b.size() - 1)
			break;

		Matrix.Cols.push_back(Col);
		Matrix.Rows.push_back(Row);
		Matrix.Vals.push_back(ChainOut.c[b_element]);
		Col -= 1; Row++;
		Matrix.Cols.push_back(Col);
		Matrix.Rows.push_back(Row);
		Matrix.Vals.push_back(ChainOut.a[b_element]);
		Col++;
	}

	return Matrix;
}

template<typename Type>
COO_matrix<Type> CMatrixGenerator<Type>::GetCOOMatrix(size_t ChainIdx)
{
	Chain ChainOut = m_Chains[ChainIdx];
	COO_matrix<Type> Matrix;

	Matrix = GetCOOTriDiagMatrix(ChainIdx);

	for (size_t LinkIdx = 0; LinkIdx < ChainOut.Links.size(); LinkIdx++)
	{
		size_t irow_idx = ChainOut.Links[LinkIdx].RowNumber;
		size_t icol_idx = ChainOut.Links[LinkIdx].ColNumber;
		Type ival = ChainOut.Links[LinkIdx].Value;

		int rowStartIdx = -1;
		for (size_t elementIdx = 0; elementIdx < Matrix.Rows.size(); elementIdx++)
		{
			if (irow_idx == Matrix.Rows[elementIdx])
			{
				rowStartIdx = elementIdx;
				break;
			}
		}
		int rowEndIdx = -1;
		for (size_t elementIdx = rowStartIdx; elementIdx < Matrix.Rows.size(); elementIdx++)
		{
			if (irow_idx != Matrix.Rows[elementIdx])
			{
				rowEndIdx = elementIdx;
				break;
			}
		}
		if (rowEndIdx == -1)
			rowEndIdx = Matrix.Rows.size();

		if (icol_idx < Matrix.Cols[rowStartIdx])
		{
			Matrix.Vals.insert(Matrix.Vals.begin() + rowStartIdx, ival);
			Matrix.Cols.insert(Matrix.Cols.begin() + rowStartIdx, icol_idx);
			Matrix.Rows.insert(Matrix.Rows.begin() + rowStartIdx, irow_idx);
			continue;
		}
		if (icol_idx > Matrix.Cols[rowEndIdx - 1])
		{
			Matrix.Vals.insert(Matrix.Vals.begin() + rowEndIdx, ival);
			Matrix.Cols.insert(Matrix.Cols.begin() + rowEndIdx, icol_idx);
			Matrix.Rows.insert(Matrix.Rows.begin() + rowEndIdx, irow_idx);
			continue;
		}
		for (size_t elementIdx = rowStartIdx; elementIdx < rowEndIdx - 1; elementIdx++)
		{
			if (icol_idx > Matrix.Cols[elementIdx] &&
				icol_idx < Matrix.Cols[elementIdx + 1])
			{
				Matrix.Vals.insert(Matrix.Vals.begin() + elementIdx + 1, ival);
				Matrix.Cols.insert(Matrix.Cols.begin() + elementIdx + 1, icol_idx);
				Matrix.Rows.insert(Matrix.Rows.begin() + elementIdx + 1, irow_idx);
				break;
			}
		}
	}

	return Matrix;
}

template class CMatrixGenerator<float>;
template class CMatrixGenerator<double>;
template class CMatrixGenerator<MCSAS::Types::BaseTypes::ComplexDoubleCartesian>;
template class CMatrixGenerator<MCSAS::Types::BaseTypes::ComplexDoubleHybrid>;
template class CMatrixGenerator<MCSAS::Types::BaseTypes::ComplexDoublePolar>;
template class CMatrixGenerator<MCSAS::Types::BaseTypes::ComplexFloatCartesian>;
template class CMatrixGenerator<MCSAS::Types::BaseTypes::ComplexFloatHybrid>;
template class CMatrixGenerator<MCSAS::Types::BaseTypes::ComplexFloatPolar>;