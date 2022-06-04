#include <algorithm>
#include <memory>
#include <string>

#include <windows.h>

#include "exceptions.hpp"
#include "windows.hpp"

using namespace pplugger::error;

std::string pplugger::error::geterrstr()
{
	return geterrstr(getlasterr());
}

std::string pplugger::error::geterrstr(error_type error)
{
	std::string str;
	std::unique_ptr<char *> buf = std::make_unique<char *>();
	unsigned long len = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr, error,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			reinterpret_cast<char *>(buf.get()),
			0, nullptr);
	if (len > 0) {
		str = std::string(*buf);
		str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
	} else {
		str = "cannot retrieve error text";
	}
	return str;
}

error_type pplugger::error::getlasterr()
{
	return GetLastError();
}
