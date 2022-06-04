#ifndef PPLUGGER_WINDOWS_HPP
#define PPLUGGER_WINDOWS_HPP

#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

#include "memory.hpp"
#include "modules.hpp"
#include "types.hpp"

namespace pplugger::os {
	pplugger::arch_type getarch(const proc_handle& proc);
	pplugger::pid_type  getthispid();

	std::string getprocname(const proc_handle& handle);
	std::string getmodname(const  proc_handle& hproc, const mod_handle& hmod);

	std::vector<pplugger::pid_type> getallpids();

	std::vector<pplugger::pmodule> getprocmodsv1(const proc_handle& hproc);
	std::vector<pplugger::pmodule> getprocmodsv2(const proc_handle& hproc);

	pplugger::addr_type  getmodbase(const proc_handle& hproc, const mod_handle& hmod);
	pplugger::image_size getmodsize(const proc_handle& hproc, const mod_handle& hmod);

	pplugger::page getpagev1(const pplugger::addr_type addr, const proc_handle& hproc);

	void closehandle(handle_type handl) noexcept;

	handle_type openproc(const pplugger::pid_type pid, std::initializer_list<procflag_type> flags);
	handle_type opensnap(const pplugger::pid_type pid, std::initializer_list<snapflag_type> flags);

	std::shared_ptr<uint8_t[]> readprocmem(const proc_handle& hproc, const pplugger::addr_type base, const pplugger::addr_type size);
	void writeprocmem(const proc_handle& hproc, const pplugger::addr_type base, const pplugger::addr_type size, const void *const obj);
}

#endif /* PPLUGGER_WINDOWS_HPP */
