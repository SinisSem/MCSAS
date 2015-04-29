#pragma once
namespace MCSAS
{
namespace Types
{
namespace BaseTypes
{

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

}//namespace MCSAS
}//namespace Types
}//namespace MCSAS
