#pragma once

namespace MCSAS
{
namespace Types
{
namespace BaseTypes
{
	//enum BaseType{
	//	BT_UINT = 0,
	//	BT_INT = 1,
	//	BT_FLOAT = 2,
	//	BT_DOUBLE = 3,
	//	BT_CFC = 4,
	//	BT_CFP = 5,
	//	BT_CFH = 6,
	//	BT_CDC = 7,
	//	BT_CDP = 8,
	//	BT_CDH = 9
	//};

struct ComplexFloatCartesian
{
	float Re;
	float Im;
};

struct ComplexFloatPolar
{
	float Distance;
	float Angle;
};

struct ComplexDoubleCartesian
{
	double Re;
	double Im;
};

struct ComplexDoublePolar
{
	double Distance;
	double Angle;
};

struct ComplexFloatHybrid
{
	float Distance;
	float AngleCos;
	float AngleSin;
};

struct ComplexDoubleHybrid
{
	double Distance;
	double AngleCos;
	double AngleSin;
};

// Commons

template<typename Type>
void SetValue(Type &Var, double Re, double Im);

// Operations

int Add(int Op1, int Op2);
size_t Add(size_t Op1, size_t Op2);
float Add(float Op1, float Op2);
double Add(double Op1, double Op2);
ComplexDoubleCartesian Add(ComplexDoubleCartesian Op1, ComplexDoubleCartesian Op2);
ComplexFloatCartesian Add(ComplexFloatCartesian Op1, ComplexFloatCartesian Op2);
ComplexFloatPolar Add(ComplexFloatPolar Op1, ComplexFloatPolar Op2);
ComplexFloatHybrid Add(ComplexFloatHybrid Op1, ComplexFloatHybrid Op2);
ComplexDoubleHybrid Add(ComplexDoubleHybrid Op1, ComplexDoubleHybrid Op2);
ComplexDoublePolar Add(ComplexDoublePolar Op1, ComplexDoublePolar Op2);

int Sub(int Op1, int Op2);
size_t Sub(size_t Op1, size_t Op2);
float Sub(float Op1, float Op2);
double Sub(double Op1, double Op2);
ComplexDoubleCartesian Sub(ComplexDoubleCartesian Op1, ComplexDoubleCartesian Op2);
ComplexFloatCartesian Sub(ComplexFloatCartesian Op1, ComplexFloatCartesian Op2);
ComplexFloatPolar Sub(ComplexFloatPolar Op1, ComplexFloatPolar Op2);
ComplexFloatHybrid Sub(ComplexFloatHybrid Op1, ComplexFloatHybrid Op2);
ComplexDoubleHybrid Sub(ComplexDoubleHybrid Op1, ComplexDoubleHybrid Op2);
ComplexDoublePolar Sub(ComplexDoublePolar Op1, ComplexDoublePolar Op2);

int Mul(int Op1, int Op2);
size_t Mul(size_t Op1, size_t Op2);
float Mul(float Op1, float Op2);
double Mul(double Op1, double Op2);
ComplexDoubleCartesian Mul(ComplexDoubleCartesian Op1, ComplexDoubleCartesian Op2);
ComplexFloatCartesian Mul(ComplexFloatCartesian Op1, ComplexFloatCartesian Op2);
ComplexFloatPolar Mul(ComplexFloatPolar Op1, ComplexFloatPolar Op2);
ComplexFloatHybrid Mul(ComplexFloatHybrid Op1, ComplexFloatHybrid Op2);
ComplexDoubleHybrid Mul(ComplexDoubleHybrid Op1, ComplexDoubleHybrid Op2);
ComplexDoublePolar Mul(ComplexDoublePolar Op1, ComplexDoublePolar Op2);

int Div(int Op1, int Op2);
size_t Div(size_t Op1, size_t Op2);
float Div(float Op1, float Op2);
double Div(double Op1, double Op2);
ComplexDoubleCartesian Div(ComplexDoubleCartesian Op1, ComplexDoubleCartesian Op2);
ComplexFloatCartesian Div(ComplexFloatCartesian Op1, ComplexFloatCartesian Op2);
ComplexFloatPolar Div(ComplexFloatPolar Op1, ComplexFloatPolar Op2);
ComplexFloatHybrid Div(ComplexFloatHybrid Op1, ComplexFloatHybrid Op2);
ComplexDoubleHybrid Div(ComplexDoubleHybrid Op1, ComplexDoubleHybrid Op2);
ComplexDoublePolar Div(ComplexDoublePolar Op1, ComplexDoublePolar Op2);

// Conversions
/*
ComplexDoubleCartesian ToCDC(ComplexDoubleCartesian Op1);
ComplexDoubleCartesian ToCDC(ComplexFloatCartesian Op1);
ComplexDoubleCartesian ToCDC(ComplexFloatPolar Op1);
ComplexDoubleCartesian ToCDC(ComplexFloatHybrid Op1);
ComplexDoubleCartesian ToCDC(ComplexDoubleHybrid Op1);
ComplexDoubleCartesian ToCDC(ComplexDoublePolar Op1);

ComplexFloatCartesian ToCFC(ComplexDoubleCartesian Op1);
ComplexFloatCartesian ToCFC(ComplexFloatCartesian Op1);
ComplexFloatCartesian ToCFC(ComplexFloatPolar Op1);
ComplexFloatCartesian ToCFC(ComplexFloatHybrid Op1);
ComplexFloatCartesian ToCFC(ComplexDoubleHybrid Op1);
ComplexFloatCartesian ToCFC(ComplexDoublePolar Op1);

ComplexFloatPolar ToCFP(ComplexDoubleCartesian Op1);
ComplexFloatPolar ToCFP(ComplexFloatCartesian Op1);
ComplexFloatPolar ToCFP(ComplexFloatPolar Op1);
ComplexFloatPolar ToCFP(ComplexFloatHybrid Op1);
ComplexFloatPolar ToCFP(ComplexDoubleHybrid Op1);
ComplexFloatPolar ToCFP(ComplexDoublePolar Op1);

ComplexFloatHybrid ToCFH(ComplexDoubleCartesian Op1);
ComplexFloatHybrid ToCFH(ComplexFloatCartesian Op1);
ComplexFloatHybrid ToCFH(ComplexFloatPolar Op1);
ComplexFloatHybrid ToCFH(ComplexFloatHybrid Op1);
ComplexFloatHybrid ToCFH(ComplexDoubleHybrid Op1);
ComplexFloatHybrid ToCFH(ComplexDoublePolar Op1);

ComplexDoubleHybrid ToCDH(ComplexDoubleCartesian Op1);
ComplexDoubleHybrid ToCDH(ComplexFloatCartesian Op1);
ComplexDoubleHybrid ToCDH(ComplexFloatPolar Op1);
ComplexDoubleHybrid ToCDH(ComplexFloatHybrid Op1);
ComplexDoubleHybrid ToCDH(ComplexDoubleHybrid Op1);
ComplexDoubleHybrid ToCDH(ComplexDoublePolar Op1);

ComplexDoublePolar ToCDP(ComplexDoubleCartesian Op1);
ComplexDoublePolar ToCDP(ComplexFloatCartesian Op1);
ComplexDoublePolar ToCDP(ComplexFloatPolar Op1);
ComplexDoublePolar ToCDP(ComplexFloatHybrid Op1);
ComplexDoublePolar ToCDP(ComplexDoubleHybrid Op1);
ComplexDoublePolar ToCDP(ComplexDoublePolar Op1);
*/
}//namespace MCSAS
}//namespace Types
}//namespace MCSAS
