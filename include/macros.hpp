#ifndef PPLUGGER_MACROS_HPP
#define PPLUGGER_MACROS_HPP

#define PPLUGGER_STR(x) #x 
#define PPLUGGER_STRING(x) PPLUGGER_STR(x)

#include "exceptions.hpp"
#define PPLUGGER_LOGICERR(x) (pplugger::error::logic(std::string(__FILE__ ":" PPLUGGER_STRING(__LINE__) ": ") + x))


#endif /* PPLUGGER_MACROS_HPP */
