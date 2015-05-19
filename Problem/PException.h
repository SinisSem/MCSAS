#pragma once
#include "../MCSASException.h"

namespace MCSAS
{
	namespace Problem
	{
		class PException : public MCSASException
		{
		public:
			template <typename MessageType>
			PException(const MessageType& message) : MCSASException(message) {}
		};
	}
}