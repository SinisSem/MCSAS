#include "BaseTypes.h"
#define _USE_MATH_DEFINES
#include "BaseTypesMACRO.h"
using namespace MCSAS;
using namespace Types;
using namespace BaseTypes;

double MCSAS::Types::BaseTypes::Add(double Op1, double Op2)
{
	return Op1 + Op2;
}

double MCSAS::Types::BaseTypes::Sub(double Op1, double Op2)
{
	return Op1 - Op2;
}

double MCSAS::Types::BaseTypes::Mul(double Op1, double Op2)
{
	return Op1 * Op2;
}

double MCSAS::Types::BaseTypes::Div(double Op1, double Op2)
{
	return Op1 / Op2;
}

float MCSAS::Types::BaseTypes::Add(float Op1, float Op2)
{
	return Op1 + Op2;
}

float MCSAS::Types::BaseTypes::Sub(float Op1, float Op2)
{
	return Op1 - Op2;
}

float MCSAS::Types::BaseTypes::Mul(float Op1, float Op2)
{
	return Op1 * Op2;
}

float MCSAS::Types::BaseTypes::Div(float Op1, float Op2)
{
	return Op1 / Op2;
}

size_t MCSAS::Types::BaseTypes::Add(size_t Op1, size_t Op2)
{
	return Op1 + Op2;
}

size_t MCSAS::Types::BaseTypes::Sub(size_t Op1, size_t Op2)
{
	return Op1 - Op2;
}

size_t MCSAS::Types::BaseTypes::Mul(size_t Op1, size_t Op2)
{
	return Op1 * Op2;
}

size_t MCSAS::Types::BaseTypes::Div(size_t Op1, size_t Op2)
{
	return Op1 / Op2;
}

int MCSAS::Types::BaseTypes::Add(int Op1, int Op2)
{
	return Op1 + Op2;
}

int MCSAS::Types::BaseTypes::Sub(int Op1, int Op2)
{
	return Op1 - Op2;
}

int MCSAS::Types::BaseTypes::Mul(int Op1, int Op2)
{
	return Op1 * Op2;
}

int MCSAS::Types::BaseTypes::Div(int Op1, int Op2)
{
	return Op1 / Op2;
}

ComplexDoubleCartesian MCSAS::Types::BaseTypes::Add(ComplexDoubleCartesian Op1, ComplexDoubleCartesian Op2)
{
	ComplexDoubleCartesian Ret;
	C_plus_C(Ret.Re, Ret.Im, Op1.Re, Op1.Im, Op2.Re, Op2.Im);
	return Ret;
}

ComplexDoublePolar MCSAS::Types::BaseTypes::Add(ComplexDoublePolar Op1, ComplexDoublePolar Op2)
{
	ComplexDoublePolar Ret;
	P_plus_P(Ret.Distance, Ret.Angle, Op1.Distance, Op1.Angle, Op2.Distance, Op2.Angle);
	return Ret;
}

ComplexDoubleHybrid MCSAS::Types::BaseTypes::Add(ComplexDoubleHybrid Op1, ComplexDoubleHybrid Op2)
{
	ComplexDoubleHybrid Ret;
	H_plus_H(Ret.Distance, Ret.AngleSin, Ret.AngleCos, Op1.Distance, Op1.AngleSin, Op1.AngleCos, Op2.Distance, Op2.AngleSin, Op2.AngleCos);
	return Ret;
}

ComplexFloatCartesian MCSAS::Types::BaseTypes::Add(ComplexFloatCartesian Op1, ComplexFloatCartesian Op2)
{
	ComplexFloatCartesian Ret;
	C_plus_C(Ret.Re, Ret.Im, Op1.Re, Op1.Im, Op2.Re, Op2.Im);
	return Ret;
}

ComplexFloatPolar MCSAS::Types::BaseTypes::Add(ComplexFloatPolar Op1, ComplexFloatPolar Op2)
{
	ComplexFloatPolar Ret;
	P_plus_P(Ret.Distance, Ret.Angle, Op1.Distance, Op1.Angle, Op2.Distance, Op2.Angle);
	return Ret;
}

ComplexFloatHybrid MCSAS::Types::BaseTypes::Add(ComplexFloatHybrid Op1, ComplexFloatHybrid Op2)
{
	ComplexFloatHybrid Ret;
	H_plus_H(Ret.Distance, Ret.AngleSin, Ret.AngleCos, Op1.Distance, Op1.AngleSin, Op1.AngleCos, Op2.Distance, Op2.AngleSin, Op2.AngleCos);
	return Ret;
}

ComplexDoubleCartesian MCSAS::Types::BaseTypes::Sub(ComplexDoubleCartesian Op1, ComplexDoubleCartesian Op2)
{
	ComplexDoubleCartesian Ret;
	C_plus_C(Ret.Re, Ret.Im, Op1.Re, Op1.Im, -Op2.Re, -Op2.Im);
	return Ret;
}

ComplexDoublePolar MCSAS::Types::BaseTypes::Sub(ComplexDoublePolar Op1, ComplexDoublePolar Op2)
{
	ComplexDoublePolar Ret;
	P_plus_P(Ret.Distance, Ret.Angle, Op1.Distance, Op1.Angle, Op2.Distance, Op2.Angle + M_PI);
	return Ret;
}

ComplexDoubleHybrid MCSAS::Types::BaseTypes::Sub(ComplexDoubleHybrid Op1, ComplexDoubleHybrid Op2)
{
	ComplexDoubleHybrid Ret;
	H_plus_H(Ret.Distance, Ret.AngleSin, Ret.AngleCos, Op1.Distance, -Op1.AngleSin, Op1.AngleCos, Op2.Distance, -Op2.AngleSin, -Op2.AngleCos);
	return Ret;
}

ComplexFloatCartesian MCSAS::Types::BaseTypes::Sub(ComplexFloatCartesian Op1, ComplexFloatCartesian Op2)
{
	ComplexFloatCartesian Ret;
	C_plus_C(Ret.Re, Ret.Im, Op1.Re, Op1.Im, -Op2.Re, -Op2.Im);
	return Ret;
}

ComplexFloatPolar MCSAS::Types::BaseTypes::Sub(ComplexFloatPolar Op1, ComplexFloatPolar Op2)
{
	ComplexFloatPolar Ret;
	P_plus_P(Ret.Distance, Ret.Angle, Op1.Distance, Op1.Angle, Op2.Distance, Op2.Angle + (float)M_PI);
	return Ret;
}

ComplexFloatHybrid MCSAS::Types::BaseTypes::Sub(ComplexFloatHybrid Op1, ComplexFloatHybrid Op2)
{
	ComplexFloatHybrid Ret;
	H_plus_H(Ret.Distance, Ret.AngleSin, Ret.AngleCos, Op1.Distance, Op1.AngleSin, Op1.AngleCos, Op2.Distance, -Op2.AngleSin, -Op2.AngleCos);
	return Ret;
}

ComplexDoubleCartesian MCSAS::Types::BaseTypes::Mul(ComplexDoubleCartesian Op1, ComplexDoubleCartesian Op2)
{
	ComplexDoubleCartesian Ret;
	C_prod_C(Ret.Re, Ret.Im, Op1.Re, Op1.Im, Op2.Re, Op2.Im);
	return Ret;
}

ComplexDoublePolar MCSAS::Types::BaseTypes::Mul(ComplexDoublePolar Op1, ComplexDoublePolar Op2)
{
	ComplexDoublePolar Ret;
	P_prod_P(Ret.Distance, Ret.Angle, Op1.Distance, Op1.Angle, Op2.Distance, Op2.Angle);
	return Ret;
}

ComplexDoubleHybrid MCSAS::Types::BaseTypes::Mul(ComplexDoubleHybrid Op1, ComplexDoubleHybrid Op2)
{
	ComplexDoubleHybrid Ret;
	H_prod_H(Ret.Distance, Ret.AngleSin, Ret.AngleCos, Op1.Distance, Op1.AngleSin, Op1.AngleCos, Op2.Distance, Op2.AngleSin, Op2.AngleCos);
	return Ret;
}

ComplexFloatCartesian MCSAS::Types::BaseTypes::Mul(ComplexFloatCartesian Op1, ComplexFloatCartesian Op2)
{
	ComplexFloatCartesian Ret;
	C_prod_C(Ret.Re, Ret.Im, Op1.Re, Op1.Im, Op2.Re, Op2.Im);
	return Ret;
}

ComplexFloatPolar MCSAS::Types::BaseTypes::Mul(ComplexFloatPolar Op1, ComplexFloatPolar Op2)
{
	ComplexFloatPolar Ret;
	P_prod_P(Ret.Distance, Ret.Angle, Op1.Distance, Op1.Angle, Op2.Distance, Op2.Angle);
	return Ret;
}

ComplexFloatHybrid MCSAS::Types::BaseTypes::Mul(ComplexFloatHybrid Op1, ComplexFloatHybrid Op2)
{
	ComplexFloatHybrid Ret;
	H_prod_H(Ret.Distance, Ret.AngleSin, Ret.AngleCos, Op1.Distance, Op1.AngleSin, Op1.AngleCos, Op2.Distance, Op2.AngleSin, Op2.AngleCos);
	return Ret;
}

ComplexDoubleCartesian MCSAS::Types::BaseTypes::Div(ComplexDoubleCartesian Op1, ComplexDoubleCartesian Op2)
{
	ComplexDoubleCartesian Ret;
	C_div_C(Ret.Re, Ret.Im, Op1.Re, Op1.Im, Op2.Re, Op2.Im);
	return Ret;
}

ComplexDoublePolar MCSAS::Types::BaseTypes::Div(ComplexDoublePolar Op1, ComplexDoublePolar Op2)
{
	ComplexDoublePolar Ret;
	P_div_P(Ret.Distance, Ret.Angle, Op1.Distance, Op1.Angle, Op2.Distance, Op2.Angle);
	return Ret;
}

ComplexDoubleHybrid MCSAS::Types::BaseTypes::Div(ComplexDoubleHybrid Op1, ComplexDoubleHybrid Op2)
{
	ComplexDoubleHybrid Ret;
	H_div_H(Ret.Distance, Ret.AngleSin, Ret.AngleCos, Op1.Distance, Op1.AngleSin, Op1.AngleCos, Op2.Distance, Op2.AngleSin, Op2.AngleCos);
	return Ret;
}

ComplexFloatCartesian MCSAS::Types::BaseTypes::Div(ComplexFloatCartesian Op1, ComplexFloatCartesian Op2)
{
	ComplexFloatCartesian Ret;
	C_div_C(Ret.Re, Ret.Im, Op1.Re, Op1.Im, Op2.Re, Op2.Im);
	return Ret;
}

ComplexFloatPolar MCSAS::Types::BaseTypes::Div(ComplexFloatPolar Op1, ComplexFloatPolar Op2)
{
	ComplexFloatPolar Ret;
	P_div_P(Ret.Distance, Ret.Angle, Op1.Distance, Op1.Angle, Op2.Distance, Op2.Angle);
	return Ret;
}

ComplexFloatHybrid MCSAS::Types::BaseTypes::Div(ComplexFloatHybrid Op1, ComplexFloatHybrid Op2)
{
	ComplexFloatHybrid Ret;
	H_div_H(Ret.Distance, Ret.AngleSin, Ret.AngleCos, Op1.Distance, Op1.AngleSin, Op1.AngleCos, Op2.Distance, Op2.AngleSin, Op2.AngleCos);
	return Ret;
}