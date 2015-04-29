#pragma once

#include <string>
#include <stdexcept>

namespace MCSAS
{
class MCSASException : public std::exception
{
public:
	MCSASException(const MCSASException& exception_) : message(exception_.message) {}
	MCSASException(const std::string& message_) : message(message_) {}
	~MCSASException() throw() {}
	const char* what() const throw() { return message.c_str(); }

protected:
	std::string message;
};
}