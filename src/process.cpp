#include <cstdint>
#include <string>
#include <utility>

#include "exceptions.hpp"
#include "process.hpp"
#include "exceptions.hpp"
#include "memory.hpp"
#include "modules.hpp"
#include "types.hpp"
#include "windows.hpp"

using namespace pplugger;

pplugger::process::process(const pid_type pid) :
	pid(pid),
	handle(pid, {
			pplugger::os::procflag_type::query_limited_information,
			pplugger::os::procflag_type::vm_read
		}),
	arch(pplugger::os::getarch(handle)),
	name(pplugger::os::getprocname(handle)),
	mods(handle),
	mem(pid) {}

pplugger::process::process(process&& proc)
{
	*this = std::move(proc);
}

process& pplugger::process::operator=(process&& proc)
{
	if (this == &proc) {
		throw PPLUGGER_LOGICERR("cannot move process to itself");
	}

	pid    = proc.pid;
	name   = std::move(proc.name);
	handle = std::move(proc.handle);
	arch   = proc.arch;
	mods   = std::move(proc.mods);
	mem    = std::move(proc.mem);

	proc.pid  = 0;
	proc.arch = arch_type::invalid;
	return *this;
}

pid_type pplugger::process::getpid() const noexcept
{
	return pid;
}

std::string pplugger::process::getname() const noexcept
{
	return name;
}

pmodule& pplugger::process::getmodule(const std::string name)
{
	return mods.name(name);
}

arch_type pplugger::process::getarch() const noexcept
{
	return arch;
}

modules& pplugger::process::getmodules()
{
	return mods;
}

pplugger::processes::processes() : procs()
{
	std::vector<pid_type> pidlist(pplugger::os::getallpids());
	const pid_type thispid = pplugger::os::getthispid();
	for (const pid_type pt : pidlist) {
		try {
			if (pt == thispid) {
				continue;
			}
			procs.push_back(pt);
		} catch(pplugger::error::oserror& e) {
		} catch(pplugger::error::empty_mods& e) {}
	}
	if (procs.empty()) {
		throw pplugger::error::empty_procs();
	}
}

process& pplugger::processes::pid(const pid_type pid)
{
	std::vector<process>::iterator it;
	for (it = procs.begin(); it < procs.end(); it++) {
		if ((*it).getpid() == pid) {
			break;
		}
	}

	if (procs.end() == it) {
		throw pplugger::error::not_found_proc(pid);
	}
	return *it;
}

static process& getbestproc(std::vector<std::vector<process>::iterator>& hits, const std::string& name)
{
	std::vector<std::vector<process>::iterator>::iterator it;
	for (it = hits.begin(); it < hits.end(); it++) {
		// TODO implement function that returns the "best" process
		// i.E. which is already initialized and has a window, etc
		return **it;
	}

	throw pplugger::error::not_found_proc(name);
}

process& pplugger::processes::name(const std::string& name)
{
	std::vector<std::vector<process>::iterator> hits;
	for (std::vector<process>::iterator it = procs.begin(); it < procs.end(); it++) {
		// TODO compare case insensitive
		if (0 == (*it).getname().compare(name)) {
			hits.push_back(it);
		}
	}

	if (0 == hits.size()) {
		throw pplugger::error::not_found_proc(name);
	}
	return getbestproc(hits, name);
}

process pplugger::find(pid_type pid)
{
	try {
		return std::move(processes().pid(pid));
	} catch(pplugger::error::empty_procs&) {
		throw pplugger::error::not_found_proc(pid);
	}
}

process pplugger::find(std::string exe)
{
	try {
		return std::move(processes().name(exe));
	} catch(pplugger::error::empty_procs&) {
		throw pplugger::error::not_found_proc(exe);
	}
}
