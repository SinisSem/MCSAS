#pragma once

namespace MCSAS
{
	namespace Types
	{
		namespace BaseTypes
		{
			void StartRandom();

			template<typename Type>
			Type RandomFromTo(Type From, Type To);

			template<typename Type>
			Type RandomBaseDispersion(Type Base, Type Dispersion);
		}
	}
}