#include <string>
#include <iostream>
#include <utility>

#include "exceptions.hpp"
#include "types.hpp"
#include "windows.hpp"

using namespace pplugger;
using namespace pplugger::os;

std::ostream& pplugger::operator<<(std::ostream& out, arch_type arch)
{
	switch(arch) {
	case arch_type::amd64_cisc:
		out << "amd64";
		break;
	case arch_type::i386_cisc:
		out << "i386";
		break;
	case arch_type::invalid:
	default:
		out << "invalid";
		break;
	}
	return out;
}

std::string pplugger::operator+(std::string str, arch_type arch)
{
	switch(arch) {
	case arch_type::amd64_cisc:
		str + "amd64";
		break;
	case arch_type::i386_cisc:
		str + "i386";
		break;
	case arch_type::invalid:
	default:
		str + "invalid";
		break;
	}
	return str;
}

std::ostream& pplugger::os::operator<<(std::ostream& out, pagestate_type pagestate)
{
	switch(pagestate) {
	case pagestate_type::commit:
		out << "commit";
		break;
	case pagestate_type::free:
		out << "free";
		break;
	case pagestate_type::reserve:
		out << "reserve";
		break;
	case pagestate_type::invalid:
	default:
		out << "invalid";
		break;
	}
	return out;
}

std::string pplugger::os::operator+(std::string str, pagestate_type pagestate)
{
	switch(pagestate) {
	case pagestate_type::commit:
		str + "commit";
		break;
	case pagestate_type::free:
		str + "free";
		break;
	case pagestate_type::reserve:
		str + "reserve";
		break;
	case pagestate_type::invalid:
	default:
		str + "invalid";
		break;
	}
	return str;
}

std::ostream& pplugger::os::operator<<(std::ostream& out, pagetype_type pagetype)
{
	switch(pagetype) {
	case pagetype_type::image:
		out << "image";
		break;
	case pagetype_type::mapped:
		out << "mapped";
		break;
	case pagetype_type::privatepage:
		out << "private";
		break;
	case pagetype_type::invalid:
	default:
		out << "invalid";
		break;
	}
	return out;
}

std::string pplugger::os::operator+(std::string str, pagetype_type pagetype)
{
	switch(pagetype) {
	case pagetype_type::image:
		str + "image";
		break;
	case pagetype_type::mapped:
		str + "mapped";
		break;
	case pagetype_type::privatepage:
		str + "private";
		break;
	case pagetype_type::invalid:
	default:
		str + "invalid";
		break;
	}
	return str;
}

std::ostream& pplugger::os::operator<<(std::ostream& out, protect_type prot)
{
	switch(prot) {
	case protect_type::noaccess:
		out << "noaccess";
		break;
	case protect_type::readonly:
		out << "readonly";
		break;
	case protect_type::readwrite:
		out << "readwrite";
		break;
	case protect_type::writecopy:
		out << "writecopy";
		break;
	case protect_type::execute:
		out << "execute";
		break;
	case protect_type::execute_readonly:
		out << "execute_readonly";
		break;
	case protect_type::execute_readwrite:
		out << "execute_readwrite";
		break;
	case protect_type::execute_writecopy:
		out << "execute_readonly";
		break;
	case protect_type::guard_readonly:
		out << "guard_readonly";
		break;
	case protect_type::guard_readwrite:
		out << "guard_readwrite";
		break;
	case protect_type::guard_writecopy:
		out << "guard_writecopy";
		break;
	case protect_type::guard_execute:
		out << "guard_execute";
		break;
	case protect_type::guard_execute_readonly:
		out << "guard_execute_readonly";
		break;
	case protect_type::guard_execute_readwrite:
		out << "guard_execute_readwrite";
		break;
	case protect_type::guard_execute_writecopy:
		out << "guard_execute_writecopy";
		break;
	case protect_type::nocache_readonly:
		out << "nocache_readonly";
		break;
	case protect_type::nocache_readwrite:
		out << "nocache_readwrite";
		break;
	case protect_type::nocache_writecopy:
		out << "nocache_writecopy";
		break;
	case protect_type::nocache_execute:
		out << "nocache_execute";
		break;
	case protect_type::nocache_execute_readonly:
		out << "nocache_execute_readonly";
		break;
	case protect_type::nocache_execute_readwrite:
		out << "nocache_execute_readwrite";
		break;
	case protect_type::nocache_execute_writecopy:
		out << "nocache_execute_writecopy";
		break;
	case protect_type::invalid:
	default:
		out << "invalid";
		break;
	}
	return out;
}

std::string pplugger::os::operator+(std::string str, protect_type prot)
{
	switch(prot) {
	case protect_type::noaccess:
		str + "noaccess";
		break;
	case protect_type::readonly:
		str + "readonly";
		break;
	case protect_type::readwrite:
		str + "readwrite";
		break;
	case protect_type::writecopy:
		str + "writecopy";
		break;
	case protect_type::execute:
		str + "execute";
		break;
	case protect_type::execute_readonly:
		str + "execute_readonly";
		break;
	case protect_type::execute_readwrite:
		str + "execute_readwrite";
		break;
	case protect_type::execute_writecopy:
		str + "execute_readonly";
		break;
	case protect_type::guard_readonly:
		str + "guard_readonly";
		break;
	case protect_type::guard_readwrite:
		str + "guard_readwrite";
		break;
	case protect_type::guard_writecopy:
		str + "guard_writecopy";
		break;
	case protect_type::guard_execute:
		str + "guard_execute";
		break;
	case protect_type::guard_execute_readonly:
		str + "guard_execute_readonly";
		break;
	case protect_type::guard_execute_readwrite:
		str + "guard_execute_readwrite";
		break;
	case protect_type::guard_execute_writecopy:
		str + "guard_execute_writecopy";
		break;
	case protect_type::nocache_readonly:
		str + "nocache_readonly";
		break;
	case protect_type::nocache_readwrite:
		str + "nocache_readwrite";
		break;
	case protect_type::nocache_writecopy:
		str + "nocache_writecopy";
		break;
	case protect_type::nocache_execute:
		str + "nocache_execute";
		break;
	case protect_type::nocache_execute_readonly:
		str + "nocache_execute_readonly";
		break;
	case protect_type::nocache_execute_readwrite:
		str + "nocache_execute_readwrite";
		break;
	case protect_type::nocache_execute_writecopy:
		str + "nocache_execute_writecopy";
		break;
	case protect_type::invalid:
	default:
		str + "invalid";
		break;
	}
	return str;
}

bool pplugger::os::isreadablestate(const pagestate_type state)
{
	if (pagestate_type::commit == state) {
		return true;
	} else {
		return false;
	}
}

bool pplugger::os::iswriteablestate(const pagestate_type state)
{
	if (pagestate_type::commit == state) {
		return true;
	} else {
		return false;
	}
}

bool pplugger::os::isreadabletype(const pagetype_type type)
{
	switch(type) {
	case pagetype_type::image:
	case pagetype_type::mapped:
	case pagetype_type::privatepage:
		return true;
	case pagetype_type::invalid:
	default:
		return false;
	}
}

bool pplugger::os::iswriteabletype(const pagetype_type type)
{
	switch(type) {
	case pagetype_type::image:
	case pagetype_type::mapped:
	case pagetype_type::privatepage:
		return true;
	case pagetype_type::invalid:
	default:
		return false;
	}
}

bool pplugger::os::isreadableprot(const protect_type prot)
{
	switch(prot) {
	case protect_type::readonly:
	case protect_type::readwrite:
	case protect_type::writecopy:
	case protect_type::execute_readonly:
	case protect_type::execute_readwrite:
	case protect_type::execute_writecopy:
	case protect_type::guard_readonly:
	case protect_type::guard_readwrite:
	case protect_type::guard_writecopy:
	case protect_type::guard_execute_readonly:
	case protect_type::guard_execute_readwrite:
	case protect_type::guard_execute_writecopy:
	case protect_type::nocache_readonly:
	case protect_type::nocache_readwrite:
	case protect_type::nocache_writecopy:
	case protect_type::nocache_execute_readonly:
	case protect_type::nocache_execute_readwrite:
	case protect_type::nocache_execute_writecopy:
		return true;
		break;
	case protect_type::invalid:
	case protect_type::noaccess:
	case protect_type::execute:
	case protect_type::guard_execute:
	case protect_type::nocache_execute:
	default:
		return false;
		break;
	}
}

bool pplugger::os::iswriteableprot(const protect_type prot)
{
	switch(prot) {
	case protect_type::readwrite:
	case protect_type::writecopy:
	case protect_type::execute_readwrite:
	case protect_type::execute_writecopy:
	case protect_type::guard_readwrite:
	case protect_type::guard_writecopy:
	case protect_type::guard_execute_readwrite:
	case protect_type::guard_execute_writecopy:
	case protect_type::nocache_readwrite:
	case protect_type::nocache_writecopy:
	case protect_type::nocache_execute_readwrite:
	case protect_type::nocache_execute_writecopy:
		return true;
		break;
	case protect_type::readonly:
	case protect_type::execute_readonly:
	case protect_type::guard_readonly:
	case protect_type::guard_execute_readonly:
	case protect_type::nocache_readonly:
	case protect_type::nocache_execute_readonly:
	case protect_type::invalid:
	case protect_type::noaccess:
	case protect_type::execute:
	case protect_type::guard_execute:
	case protect_type::nocache_execute:
	default:
		return false;
		break;
	}
}

pplugger::os::handle::handle() {}

pplugger::os::handle::handle(const handle_type handl) : handl(handl) {}

pplugger::os::handle::handle(handle&& othr)
{
	*this = std::move(othr);
}

pplugger::os::handle::~handle() noexcept
{
	if (nullptr != handl) {
		closehandle(handl);
		handl = nullptr;
	}
}

handle& pplugger::os::handle::operator=(handle&& othr)
{
	if (this == &othr) {
		throw PPLUGGER_LOGICERR("cannot move handle to itself");
	}

	handl = othr.handl;
	othr.handl = nullptr;

	return *this;
}

handle_type pplugger::os::handle::get() const
{
	if (nullptr == handl) {
		throw PPLUGGER_LOGICERR("cannot reference uninitialized handle");
	}
	return handl;
}

handle_type pplugger::os::handle::gethandle() const
{
	return handl;
}

pplugger::os::proc_handle::proc_handle() {}

pplugger::os::proc_handle::proc_handle(const pid_type pid, const std::initializer_list<procflag_type> flags) : handle(pplugger::os::openproc(pid, flags)), pid(pid), flags(flags) {}

pplugger::os::proc_handle::proc_handle(proc_handle&& othr) : handle(std::move(othr))
{
	*this = std::move(othr);
}

proc_handle& pplugger::os::proc_handle::operator=(proc_handle&& othr)
{
	if (this == &othr) {
		throw PPLUGGER_LOGICERR("cannot move snap_handle to itself");
	}

	handle::operator=(std::move(othr));

	flags      = othr.flags;
	pid        = othr.pid;
	othr.flags = { procflag_type::invalid };
	othr.pid   = 0;

	return *this;
}

handle_type pplugger::os::proc_handle::get() const
{
	if (0 == pid) {
		throw PPLUGGER_LOGICERR("cannot reference uninitialized proc_handle");
	}
	return handle::get();
}

pid_type pplugger::os::proc_handle::getpid() const
{
	if (0 == pid) {
		throw PPLUGGER_LOGICERR("cannot reference uninitialized proc_handle pid");
	}
	return pid;
}

bool pplugger::os::proc_handle::isopen() const
{
	if (0 == pid || handle::gethandle() == nullptr) {
		return false;
	} else {
		return true;
	}
}

pplugger::os::snap_handle::snap_handle() {}

pplugger::os::snap_handle::snap_handle(const pid_type pid, const std::initializer_list<snapflag_type> flags) : handle(pplugger::os::opensnap(pid, flags)), pid(pid), flags(flags) {}

pplugger::os::snap_handle::snap_handle(snap_handle&& othr) : handle(std::move(othr))
{
	*this = std::move(othr);
}

snap_handle& pplugger::os::snap_handle::operator=(snap_handle&& othr)
{
	if (this == &othr) {
		throw PPLUGGER_LOGICERR("cannot move snap_handle to itself");
	}

	handle::operator=(std::move(othr));

	flags      = othr.flags;
	pid        = othr.pid;
	othr.flags = { snapflag_type::invalid };
	othr.pid   = 0;

	return *this;
}

handle_type pplugger::os::snap_handle::get() const
{
	return handle::get();
}

pid_type pplugger::os::snap_handle::getpid() const
{
	if (0 == pid) {
		throw PPLUGGER_LOGICERR("cannot reference uninitialized proc_handle pid");
	}
	return pid;
}

bool pplugger::os::snap_handle::isopen() const
{
	if (0 == pid || handle::gethandle() == nullptr) {
		return false;
	} else {
		return true;
	}
}

pplugger::os::mod_handle::mod_handle() {}

pplugger::os::mod_handle::mod_handle(const modhandle_type hmod) : hmod(hmod)
{
	if (nullptr == hmod) {
		throw pplugger::error::open_mod(pplugger::error::getlasterr());
	}
}

modhandle_type pplugger::os::mod_handle::get() const
{
	if (nullptr == hmod) {
		throw PPLUGGER_LOGICERR("cannot reference uninitialized mod_handle");
	}
	return hmod;
}
