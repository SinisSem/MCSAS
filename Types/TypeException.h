#pragma once
#include "../MCSASException.h"

namespace MCSAS
{
namespace Types
{
class TypeException : public MCSASException
{
public:
	template <typename MessageType>
	TypeException(const MessageType& message) : MCSASException(message) {}
};
}
}