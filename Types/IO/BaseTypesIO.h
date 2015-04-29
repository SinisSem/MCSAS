#pragma once
#include "../BaseTypes.h"
#include <string>
#include <math.h>

namespace MCSAS
{
	namespace Types
	{
		namespace BaseTypes
		{
		namespace detail
		{
			//---------------------------------------------------------------------------------------------------------------------------

			template<typename Type>
			std::string GetTypeString(Type Var)
			{
				return "not considered type";
			}

			template<typename Type>
			std::string GetValue(Type Var)
			{
				return "not considered type";
			}

			template<typename Type>
			void SetValue(Type &Var, double Re, double Im)
			{
				return;
			}

			//---------------------------------------------------------------------------------------------------------------------------

			template<>
			std::string GetTypeString(MCSAS::Types::BaseTypes::ComplexDoubleCartesian Var)
			{
				return "complex";
			}

			template<>
			std::string GetValue(MCSAS::Types::BaseTypes::ComplexDoubleCartesian Var)
			{
				std::string Ret = std::to_string((long double)Var.Re);
				Ret += " ";
				Ret += std::to_string((long double)Var.Im);
				return Ret;
			}

			template<>
			void SetValue(MCSAS::Types::BaseTypes::ComplexDoubleCartesian &Var, double Re, double Im)
			{
				Var.Re = Re;
				Var.Im = Im;
			}

			//---------------------------------------------------------------------------------------------------------------------------

			template<>
			std::string GetTypeString(MCSAS::Types::BaseTypes::ComplexDoubleHybrid Var)
			{
				return "complex";
			}

			template<>
			std::string GetValue(MCSAS::Types::BaseTypes::ComplexDoubleHybrid Var)
			{
				std::string Ret = std::to_string((long double)(Var.AngleCos * Var.Distance));
				Ret += " ";
				Ret += std::to_string((long double)(Var.AngleSin * Var.Distance));
				return Ret;
			}

			template<>
			void SetValue(MCSAS::Types::BaseTypes::ComplexDoubleHybrid &Var, double Re, double Im)
			{
				double angle = atan2(Im, Re);
				Var.AngleCos = cos(angle);
				Var.AngleSin = sin(angle);
				Var.Distance = sqrt(Re*Re + Im*Im);
			}

			//---------------------------------------------------------------------------------------------------------------------------

			template<>
			std::string GetTypeString(MCSAS::Types::BaseTypes::ComplexDoublePolar Var)
			{
				return "complex";
			}

			template<>
			std::string GetValue(MCSAS::Types::BaseTypes::ComplexDoublePolar Var)
			{
				std::string Ret = std::to_string((long double)(cos(Var.Angle) * Var.Distance));
				Ret += " ";
				Ret += std::to_string((long double)(sin(Var.Angle) * Var.Distance));
				return Ret;
			}

			template<>
			void SetValue(MCSAS::Types::BaseTypes::ComplexDoublePolar &Var, double Re, double Im)
			{
				Var.Angle = atan2(Im, Re);
				Var.Distance = sqrt(Re*Re + Im*Im);
			}

			//---------------------------------------------------------------------------------------------------------------------------

			template<>
			std::string GetTypeString(MCSAS::Types::BaseTypes::ComplexFloatCartesian Var)
			{
				return "complex";
			}

			template<>
			std::string GetValue(MCSAS::Types::BaseTypes::ComplexFloatCartesian Var)
			{
				std::string Ret = std::to_string((long double)(Var.Re));
				Ret += " ";
				Ret += std::to_string((long double)(Var.Im));
				return Ret;
			}

			template<>
			void SetValue(MCSAS::Types::BaseTypes::ComplexFloatCartesian &Var, double Re, double Im)
			{
				Var.Re = Re;
				Var.Im = Im;
			}

			//---------------------------------------------------------------------------------------------------------------------------

			template<>
			std::string GetTypeString(MCSAS::Types::BaseTypes::ComplexFloatHybrid Var)
			{
				return "complex";
			}

			template<>
			std::string GetValue(MCSAS::Types::BaseTypes::ComplexFloatHybrid Var)
			{
				std::string Ret = std::to_string((long double)(Var.AngleCos * Var.Distance));
				Ret += " ";
				Ret += std::to_string((long double)(Var.AngleSin * Var.Distance));
				return Ret;
			}

			template<>
			void SetValue(MCSAS::Types::BaseTypes::ComplexFloatHybrid &Var, double Re, double Im)
			{
				double angle = atan2(Im, Re);
				Var.AngleCos = cos(angle);
				Var.AngleSin = sin(angle);
				Var.Distance = sqrt(Re*Re + Im*Im);
			}

			//---------------------------------------------------------------------------------------------------------------------------

			template<>
			std::string GetTypeString(MCSAS::Types::BaseTypes::ComplexFloatPolar Var)
			{
				return "complex";
			}

			template<>
			std::string GetValue(MCSAS::Types::BaseTypes::ComplexFloatPolar Var)
			{
				std::string Ret = std::to_string((long double)(cos(Var.Angle) * Var.Distance));
				Ret += " ";
				Ret += std::to_string((long double)(sin(Var.Angle) * Var.Distance));
				return Ret;
			}

			template<>
			void SetValue(MCSAS::Types::BaseTypes::ComplexFloatPolar &Var, double Re, double Im)
			{
				Var.Angle = atan2(Im, Re);
				Var.Distance = sqrt(Re*Re + Im*Im);
			}

			//---------------------------------------------------------------------------------------------------------------------------

			template<>
			std::string GetTypeString(float Var)
			{
				return "real";
			}

			template<>
			std::string GetValue(float Var)
			{
				std::string Ret = std::to_string((long double)Var);
				return Ret;
			}

			template<>
			void SetValue(float& Var, double Re, double Im)
			{
				Var = Re;
			}

			//---------------------------------------------------------------------------------------------------------------------------

			template<>
			std::string GetTypeString(double Var)
			{
				return "real";
			}

			template<>
			std::string GetValue(double Var)
			{
				std::string Ret = std::to_string((long double)Var);
				return Ret;
			}

			template<>
			void SetValue(double& Var, double Re, double Im)
			{
				Var = Re;
			}

			//---------------------------------------------------------------------------------------------------------------------------

			template<>
			std::string GetTypeString(size_t Var)
			{
				return "integer";
			}

			template<>
			std::string GetValue(size_t Var)
			{
				std::string Ret = std::to_string((long double)Var);
				return Ret;
			}

			template<>
			void SetValue(size_t &Var, double Re, double Im)
			{
				Var = Re;
			}

			//---------------------------------------------------------------------------------------------------------------------------

			template<>
			std::string GetTypeString(int Var)
			{
				return "integer";
			}

			template<>
			std::string GetValue(int Var)
			{
				std::string Ret = std::to_string((long double)Var);
				return Ret;
			}

			template<>
			void SetValue(int &Var, double Re, double Im)
			{
				Var = Re;
			}
			}
		}
	}
}