#pragma once
#include <fstream>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <vector>
#include "../IOException.h"
namespace MCSAS
{
	namespace Types
	{
		namespace IO
		{
			namespace detail
			{
				struct matrix_market_banner
				{
					std::string storage;    // "array" or "coordinate"
					std::string symmetry;   // "general", "symmetric", "hermitian", or "skew-symmetric" 
					std::string type;       // "complex", "real", "integer", or "pattern"
				};

				inline
					void tokenize(std::vector<std::string>& tokens,
					const std::string& str,
					const std::string& delimiters = "\n\r\t ")
				{
					// Skip delimiters at beginning.
					std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
					// Find first "non-delimiter".
					std::string::size_type pos     = str.find_first_of(delimiters, lastPos);

					while (std::string::npos != pos || std::string::npos != lastPos)
					{
						// Found a token, add it to the vector.
						tokens.push_back(str.substr(lastPos, pos - lastPos));
						// Skip delimiters.  Note the "not_of"
						lastPos = str.find_first_not_of(delimiters, pos);
						// Find next "non-delimiter"
						pos = str.find_first_of(delimiters, lastPos);
					}
				}

				template <typename Stream>
				void read_matrix_market_banner(matrix_market_banner& banner, Stream& input)
				{
					std::string line;
					std::vector<std::string> tokens;

					// read first line
					std::getline(input, line);
					detail::tokenize(tokens, line); 

					if (tokens.size() != 5 || tokens[0] != "%%MatrixMarket" || tokens[1] != "matrix")
						throw TypeException("invalid MatrixMarket banner");

					banner.storage  = tokens[2];
					banner.type     = tokens[3];
					banner.symmetry = tokens[4];

					if (banner.storage != "array" && banner.storage != "coordinate")
						throw TypeException("invalid MatrixMarket storage format [" + banner.storage + "]");

					if (banner.type != "complex" && banner.type != "real" 
						&& banner.type != "integer" && banner.type != "pattern")
						throw TypeException("invalid MatrixMarket data type [" + banner.type + "]");

					if (banner.symmetry != "general" && banner.symmetry != "symmetric" 
						&& banner.symmetry != "hermitian" && banner.symmetry != "skew-symmetric")
						throw TypeException("invalid MatrixMarket symmetry [" + banner.symmetry + "]");
				}
			}

			template<typename Type>
			void WriteMMFile( COO_matrix<Type> Matr, std::string filename )
			{
				std::ofstream output(filename.c_str());

				if (!output)
					throw IOException("cannot create/open file");

				output.scientific;
				output.precision(10);
				output << "%%MatrixMarket matrix coordinate " << BaseTypes::detail::GetTypeString(Matr.Vals[0]) << " general\n";

				// Rows - Cols - Entires
				output << "\t" << Matr.GetRows() << "\t" << Matr.GetCols() << "\t" << Matr.Vals.size() << "\n";

				for (size_t i = 0; i < Matr.Vals.size(); i++)
				{
					output << (Matr.Rows[i] + 1) << " ";
					output << (Matr.Cols[i] + 1) << " ";
					output << BaseTypes::detail::GetValue(Matr.Vals[i]);
					output << "\n";
				}
			}


			template<typename Type>
			void ReadMMFile( COO_matrix<Type> Matr, std::string filename )
			{
				std::ifstream input(filename.c_str());

				if (!input)
					throw IOException(std::string("unable to open file \"") + filename + std::string("\" for reading"));

				// read banner 
				detail::matrix_market_banner banner;
				detail::read_matrix_market_banner(banner, input);

				if (banner.storage != "coordinate") // banner.storage == "array"
				{
					throw IOException("it is not matrix");
				}

				//cusp::coo_matrix<IndexType,ValueType,cusp::host_memory> coo(mtx);

				// read file contents line by line
				std::string line;

				// skip over banner and comments
				do
				{
					std::getline(input, line);
				} while (line[0] == '%');

				// line contains [num_rows num_columns num_entries]
				std::vector<std::string> tokens;
				detail::tokenize(tokens, line); 

				if (tokens.size() != 3)
					throw IOException("invalid MatrixMarket coordinate format");

				size_t num_rows, num_cols, num_entries;

				std::istringstream(tokens[0]) >> num_rows;
				std::istringstream(tokens[1]) >> num_cols;
				std::istringstream(tokens[2]) >> num_entries;

				Matr.Cols.clear();
				Matr.Rows.clear();
				Matr.Vals.clear();
				//coo.resize(num_rows, num_cols, num_entries);

				size_t num_entries_read = 0;

				// read file contents
				if (banner.type == "pattern")
				{
					IOException("pattern type is not considered");
				} 
				else if (banner.type == "real" || banner.type == "integer")
				{
					while(num_entries_read < num_entries && !input.eof())
					{
						double real;
						size_t row;
						size_t col;
						Type val;

						input >> row;
						input >> col;
						input >> real;
						BaseTypes::detail::SetValue(val, real, 0);
						Matr.Rows.push_back(row);
						Matr.Cols.push_back(col);
						Matr.Vals.push_back(val);
						num_entries_read++;
					}
				} 
				else if (banner.type == "complex")
				{
					while(num_entries_read < num_entries && !input.eof())
					{
						double real, imag;
						size_t row;
						size_t col;
						Type val;

						input >> row;
						input >> col;
						input >> real;
						input >> imag;
						BaseTypes::detail::SetValue(val, real, imag);
						Matr.Rows.push_back(row);
						Matr.Cols.push_back(col);
						Matr.Vals.push_back(val);
						num_entries_read++;
					}
				}
				else
				{
					throw IOException("invalid MatrixMarket data type");
				}

				if(num_entries_read != num_entries)
					throw IOException("unexpected EOF while reading MatrixMarket entries");

				// check validity of row and column index data
				if (num_entries > 0)
				{
					size_t min_row_index = *std::min_element(Matr.Rows.begin(), Matr.Rows.end());
					size_t max_row_index = *std::max_element(Matr.Rows.begin(), Matr.Rows.end());
					size_t min_col_index = *std::min_element(Matr.Cols.begin(), Matr.Cols.end());
					size_t max_col_index = *std::max_element(Matr.Cols.begin(), Matr.Cols.end());

					if (min_row_index < 1)          throw IOException("found invalid row index (index < 1)");
					if (min_col_index < 1)          throw IOException("found invalid column index (index < 1)");
					if (max_row_index > num_rows)	throw IOException("found invalid row index (index > num_rows)");
					if (max_col_index > num_cols)	throw IOException("found invalid column index (index > num_columns)");
				}

				// convert base-1 indices to base-0
				for(size_t n = 0; n < Matr.Cols.size(); n++)
				{
					Matr.Cols[n]		-= 1;
					Matr.Rows[n]		-= 1;
				}

				Matr.SortRowsCols();
			}

			template<typename Type>
			void WriteMMFile( std::vector<Type> Vector, std::string filename )
			{
				std::ofstream output(filename.c_str());

				if (!output)
					throw IOException("cannot open file");

				output.scientific;
				output.precision(10);
				output << "%%MatrixMarket matrix array " << BaseTypes::detail::GetTypeString(Vector[0]) << " general\n";

				// Rows - Cols - Entires
				output << "\t" << Vector.size() << "\t" << 1 << "\n";

				for (size_t i = 0; i < Vector.size(); i++)
				{
					output << BaseTypes::detail::GetValue(Vector[i]);
					output << "\n";
				}
			}


			template<typename Type>
			void ReadMMFile( std::vector<Type> Vector, std::string filename )
			{
				std::ifstream input(filename.c_str());

				if (!input)
					throw IOException(std::string("unable to open file \"") + filename + std::string("\" for reading"));

				// read banner 
				detail::matrix_market_banner banner;
				detail::read_matrix_market_banner(banner, input);

				if (banner.storage == "coordinate")
				{
					throw IOException("It is matrix file, not vector");
				}
				// read file contents line by line
				std::string line;

				// skip over banner and comments
				do
				{
					std::getline(input, line);
				} while (line[0] == '%');

				std::vector<std::string> tokens;
				detail::tokenize(tokens, line); 

				if (tokens.size() != 2)
					throw IOException("invalid MatrixMarket array format");

				size_t num_rows, num_cols;

				std::istringstream(tokens[0]) >> num_rows;
				std::istringstream(tokens[1]) >> num_cols;

				Vector.clear();
				//cusp::array2d<ValueType,cusp::host_memory,cusp::column_major> dense(num_rows, num_cols);

				size_t num_entries = num_rows * num_cols;

				size_t num_entries_read = 0;

				// read file contents
				if (banner.type == "pattern")
				{
					throw IOException("pattern array MatrixMarket format is not supported");
				} 
				else if (banner.type == "real" || banner.type == "integer")
				{
					while(num_entries_read < num_entries && !input.eof())
					{
						double real;

						input >> real;

						Type Val;
						BaseTypes::detail::SetValue(Val, real, 0);
						Vector.push_back(Val);

						num_entries_read++;
					}
				} 
				else if (banner.type == "complex")
				{
					while(num_entries_read < num_entries && !input.eof())
					{
						double real, imag;

						input >> real;
						input >> imag;

						Type Val;
						BaseTypes::detail::SetValue(Val, real, imag);
						Vector.push_back(Val);

						num_entries_read++;
					}
				}
				else
				{
					throw IOException("invalid MatrixMarket data type");
				}

				if(num_entries_read != num_entries)
					throw IOException("unexpected EOF while reading MatrixMarket entries");

				if (banner.symmetry != "general")
					throw IOException("only general array symmetric MatrixMarket format is supported");
			}

		}// namespace IO
	}// namespace Types
}// Namespace MCSAS