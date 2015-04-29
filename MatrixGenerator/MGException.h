#pragma once
#include "../MCSASException.h"

namespace MCSAS
{
	namespace MatrixGenerator
	{
		class MGException : public MCSASException
		{
		public:
			template <typename MessageType>
			MGException(const MessageType& message) : MCSASException(message) {}
		};
	}
}