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

}//namespace MCSAS
}//namespace Types
}//namespace MCSAS
