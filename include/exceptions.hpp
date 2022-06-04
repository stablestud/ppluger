#ifndef PPLUGGER_EXCEPTIONS_HPP
#define PPLUGGER_EXCEPTIONS_HPP

#include <stdexcept>
#include <string>

#include "macros.hpp"
#include "types.hpp"
#include "utils.hpp"

namespace pplugger::error {
	using error_type = unsigned long;

	error_type getlasterr();
	std::string geterrstr();
	std::string geterrstr(error_type error);

	class logic : public std::logic_error {
	public:
		explicit inline logic(std::string msg) : logic_error(msg) {}
	};


	class oserror : public std::runtime_error {
	public:
		explicit oserror(std::string msg) : runtime_error(msg) {}
		explicit oserror(std::string msg, error_type err) : runtime_error(msg + ": " + geterrstr(err)) {}
	};

	class open_proc : public oserror {
	public:
		explicit inline open_proc() : oserror("cannot open process") {}
		explicit inline open_proc(error_type err) : oserror("cannot open process", err) {}
	};

	class open_mod : public oserror {
	public:
		explicit inline open_mod() : oserror("cannot open module") {}
		explicit inline open_mod(error_type err) : oserror("cannot open module", err) {}
	};

	class open_snap : public oserror {
	public:
		explicit inline open_snap() : oserror("cannot create process snapshot") {}
		explicit inline open_snap(error_type err) : oserror("cannot create process snapshot", err) {}
	};

	class read_mods : public oserror {
	public:
		explicit inline read_mods() : oserror("cannot read process modules") {}
		explicit inline read_mods(error_type err) : oserror("cannot read process modules", err) {}
	};

	class read_procs : public oserror {
	public:
		explicit inline read_procs() : oserror("cannot read processes") {}
		explicit inline read_procs(error_type err) : oserror("cannot read processes", err) {}
	};

	class read_page : public oserror {
	public:
		explicit inline read_page() : oserror("cannot read page") {}
		explicit inline read_page(error_type err) : oserror("cannot read page", err) {}
	};

	class read_arch : public oserror {
	public:
		explicit inline read_arch() : oserror("cannot read process arch") {}
		explicit inline read_arch(error_type err) : oserror("cannot read process arch", err) {}
	};

	class read_procname : public oserror {
	public:
		explicit inline read_procname() : oserror("cannot read process name") {}
		explicit inline read_procname(error_type err) : oserror("cannot read process name", err) {}
	};

	class read_modname : public oserror {
	public:
		explicit inline read_modname() : oserror("cannot read module name") {}
		explicit inline read_modname(error_type err) : oserror("cannot read module name", err) {}
	};

	class read_modinfo : public oserror {
	public:
		explicit inline read_modinfo() : oserror("cannot read module info") {}
		explicit inline read_modinfo(error_type err) : oserror("cannot read module info", err) {}
	};

	class read_mem : public oserror {
	public:
		explicit inline read_mem() : oserror("cannot read memory") {}
		explicit inline read_mem(error_type err) : oserror("cannot read memory", err) {}
		explicit inline read_mem(pplugger::addr_type addr, error_type err) : oserror("cannot read memory at address: " + util::to_hexstr(addr), err) {}
		explicit inline read_mem(pplugger::addr_type base, pplugger::addr_type size, error_type err) : oserror("cannot read memory at address: " + util::to_hexstr(base) + " - " + util::to_hexstr(base + size), err) {}
	};

	class write_mem : public oserror {
	public:
		explicit inline write_mem() : oserror("cannot write memory") {}
		explicit inline write_mem(error_type err) : oserror("cannot write memory", err) {}
		explicit inline write_mem(pplugger::addr_type addr, error_type err) : oserror("cannot write memory at address: " + util::to_hexstr(addr), err) {}
		explicit inline write_mem(pplugger::addr_type base, addr_type size, error_type err) : oserror("cannot write data to address: " + util::to_hexstr(base) + " - " + util::to_hexstr(base + size), err) {}
	};

	class not_found_proc : public std::runtime_error {
	public:
		explicit inline not_found_proc(pplugger::pid_type pid) : runtime_error("pid not found: " + util::to_hexstr(pid)) {}
		explicit inline not_found_proc(std::string name) : runtime_error("process not found: " + name) {}
	};

	class empty_procs : public std::runtime_error {
	public:
		explicit inline empty_procs() : runtime_error("no processes") {}
	};

	class empty_mods : public std::runtime_error {
	public:
		explicit inline empty_mods() : runtime_error("no process modules") {}
	};

	class non_compat_arch : public std::runtime_error {
	public:
		explicit inline non_compat_arch() : runtime_error("arch is not compatible to build") {}
	};

	class bad_ptr : public std::runtime_error {
	public:
		explicit inline bad_ptr() : runtime_error("cannot access memory at ptr") {}
		explicit inline bad_ptr(pplugger::addr_type addr) : runtime_error("cannot access memory at ptr: " + util::to_hexstr(addr)) {}
	};
}


#endif /* PPLUGGER_EXCEPTIONS_HPP */
