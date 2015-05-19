#pragma once
#include <random>
#include <algorithm>
#include <numeric>
#include <vector>
#include "BaseTypes.h"
#include "../TypeException.h"

namespace MCSAS
{
	namespace Types
	{
		template<typename Type>
		size_t	COO_matrix<Type>::GetRows() const
		{
			if (Rows.size() == 0)
				return 0;
			size_t first = *std::min_element(Rows.cbegin(), Rows.cend());
			size_t last = *std::max_element(Rows.cbegin(), Rows.cend());

			return (last - first + 1);
		}

		template<typename Type>
		size_t	COO_matrix<Type>::GetCols() const
		{
			if (Cols.size() == 0)
				return 0;
			size_t first = *std::min_element(Cols.cbegin(), Cols.cend());
			size_t last = *std::max_element(Cols.cbegin(), Cols.cend());
			return (last - first + 1);
		}

		template<typename Type>
		void COO_matrix<Type>::PushElement(Type Val, size_t Row, size_t Col)
		{
			Vals.push_back(Val);
			Rows.push_back(Row);
			Cols.push_back(Col);
		}

		template<typename Type>
		void COO_matrix<Type>::SortRowsCols()
		{
			if (Rows.size() == 0 || Cols.size() == 0 || Vals.size() == 0)
				throw TypeException("Rows, Cols, or Vals vectors' sizes are zero.");
			if (Rows.size() != Cols.size() || Cols.size() != Vals.size())
				throw TypeException("Rows, Cols, or Vals vectors' sizes aren't equal.");
			std::vector<size_t> index(Cols.size());

			// Кешируемые значения
			std::vector<size_t> Cols_;
			std::vector<Type> Vals_;
			std::vector<size_t> Rows_;

			// Сортируем сначала по столбцам
			iota(index.begin(), index.end(), 0);			// Не самая лучшая реализация, но заполнять в стиле 1, 2, 3... в цикле не охота
			std::sort(index.begin(), index.end(),
				[&](const size_t& a, const size_t& b)->bool
				{return (Cols[a] < Cols[b]);});
			Cols_ = Cols;
			Vals_ = Vals;
			Rows_ = Rows;
			std::for_each(index.begin(), index.end(), 
				[&](const size_t i_) mutable
			{
				Cols[i_] = Cols_[index[i_]];
				Rows[i_] = Rows_[index[i_]];
				Vals[i_] = Vals_[index[i_]];
			}
			);

			// Потом по строкам
			iota(index.begin(), index.end(), 0);			// Не самая лучшая реализация, но заполнять в стиле 1, 2, 3... в цикле не охота
			std::sort(index.begin(), index.end(),
				[&](const size_t& a, const size_t& b)->bool
			{return (Rows[a] < Rows[b]);});
			Cols_ = Cols;
			Vals_ = Vals;
			Rows_ = Rows;
			std::for_each(index.begin(), index.end(), 
				[&](const size_t i_) mutable
			{
				Cols[i_] = Cols_[index[i_]];
				Rows[i_] = Rows_[index[i_]];
				Vals[i_] = Vals_[index[i_]];
			}
			);
			
		}

	}//namespace Types
}//namespace MCSAS