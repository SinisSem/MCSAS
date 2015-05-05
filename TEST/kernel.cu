
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include "../Types/BaseTypes.h"

using namespace MCSAS;
using namespace Types;
using namespace BaseTypes;

int main()
{
	double A = Add(1,2);
	ComplexDoubleCartesian o1;
	o1.Re = 1;
	o1.Im = 2;
	ComplexDoubleCartesian o2;
	o2.Re = 3;
	o2.Im = 5;
	ComplexDoubleCartesian a = Add(o1,o2);
	a = Mul(o1,o2);


    return 0;
}