#include <sstream>
#include <string>

#include "utils.hpp"

using namespace pplugger::util;

std::string pplugger::util::to_hexstr(const unsigned long num)
{
	if (0UL == num) {
		return "0x0";
	}

	std::stringstream s;
	s << std::hex << "0x" << num;

	return s.str();
}
