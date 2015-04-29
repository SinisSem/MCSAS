#pragma once
#include "../../MCSASException.h"

namespace MCSAS
{
	namespace Types
	{
		namespace IO
		{
		class IOException : public MCSASException
		{
		public:
			template <typename MessageType>
			IOException(const MessageType& message) : MCSASException(message) {}
		};
		}
	}
}