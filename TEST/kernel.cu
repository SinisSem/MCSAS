
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include "../Types/BaseTypesRandom.h"

int main()
{
	MCSAS::Types::BaseTypes::StartRandom();
	for(int i = 0; i < 100; i++)
		float a = MCSAS::Types::BaseTypes::RandomBaseDispersion((float)10, (float)5);
    return 0;
}