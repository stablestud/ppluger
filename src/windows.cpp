#include <cstdint>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <windows.h>

#include <psapi.h>
#include <tlhelp32.h>

#include "exceptions.hpp"
#include "process.hpp"
#include "types.hpp"
#include "windows.hpp"

using namespace pplugger::os;

pplugger::pid_type pplugger::os::getthispid()
{
	return GetCurrentProcessId();
}

pplugger::arch_type pplugger::os::getarch(const proc_handle& proc)
{
	int value = 0;
	if (!IsWow64Process(proc.get(), &value)) {
		throw pplugger::error::read_arch(pplugger::error::getlasterr()); 
	}
	return (value ? pplugger::arch_type::i386_cisc : pplugger::arch_type::amd64_cisc);
}

void pplugger::os::closehandle(handle_type handl) noexcept
{
	CloseHandle(handl);
}

static unsigned long procfl2int(const procflag_type flag)
{
	switch(flag) {
	case procflag_type::all_access:
		return PROCESS_ALL_ACCESS;
		break;
	case procflag_type::create_process:
		return PROCESS_CREATE_PROCESS;
		break;
	case procflag_type::create_thread:
		return PROCESS_CREATE_THREAD;
		break;
	case procflag_type::dup_handle:
		return PROCESS_DUP_HANDLE;
		break;
	case procflag_type::query_information:
		return PROCESS_QUERY_INFORMATION;
		break;
	case procflag_type::query_limited_information:
		return PROCESS_QUERY_LIMITED_INFORMATION;
		break;
	case procflag_type::set_information:
		return PROCESS_SET_INFORMATION;
		break;
	case procflag_type::set_quota:
		return PROCESS_SET_QUOTA;
		break;
	case procflag_type::suspend_resume:
		return PROCESS_SUSPEND_RESUME;
		break;
	case procflag_type::terminate:
		return PROCESS_TERMINATE;
		break;
	case procflag_type::vm_operation:
		return PROCESS_VM_OPERATION;
		break;
	case procflag_type::vm_read:
		return PROCESS_VM_READ;
		break;
	case procflag_type::vm_write:
		return PROCESS_VM_WRITE;
		break;
	case procflag_type::synchronize:
		return SYNCHRONIZE;
		break;
	case procflag_type::obj_delete:
		return DELETE;
		break;
	case procflag_type::read_control:
		return READ_CONTROL;
		break;
	case procflag_type::write_dac:
		return WRITE_DAC;
		break;
	case procflag_type::write_owner:
		return WRITE_OWNER;
		break;
	case procflag_type::invalid:
	default:
		throw PPLUGGER_LOGICERR("cannot convert procflag_type to integral");
		break;
	}
}

static unsigned long procflags2integral(std::initializer_list<procflag_type> flags)
{
	unsigned long intflag = 0UL;
	for (procflag_type i : flags) {
		intflag |= procfl2int(i);
	}
	return intflag;
}

static unsigned long snapfl2int(const snapflag_type flag)
{
	switch(flag) {
	case snapflag_type::inherit:
		return TH32CS_INHERIT;
		break;
	case snapflag_type::snapall:
		return TH32CS_SNAPALL;
		break;
	case snapflag_type::snapheaplist:
		return TH32CS_SNAPHEAPLIST;
		break;
	case snapflag_type::snapmodules:
		return TH32CS_SNAPMODULE;
		break;
	case snapflag_type::snapmodules32:
		return TH32CS_SNAPMODULE32;
		break;
	case snapflag_type::snapprocesses:
		return TH32CS_SNAPPROCESS;
		break;
	case snapflag_type::snapthreads:
		return TH32CS_SNAPTHREAD;
		break;
	case snapflag_type::invalid:
	default:
		throw PPLUGGER_LOGICERR("cannot convert snapflag_type to integral");
		break;
	}
}

static unsigned long snapflags2integral(std::initializer_list<snapflag_type> flags)
{
	unsigned long intflag = 0UL;
	for (snapflag_type i : flags) {
		intflag |= snapfl2int(i);
	}
	return intflag;
}

handle_type pplugger::os::openproc(const pplugger::pid_type pid, std::initializer_list<procflag_type> flags)
{
	handle_type hproc = OpenProcess(procflags2integral(flags), false, pid);
	if (nullptr == hproc) {
		throw pplugger::error::open_proc(pplugger::error::getlasterr());
	}
	return hproc;
}

handle_type pplugger::os::opensnap(const pplugger::pid_type pid, std::initializer_list<snapflag_type> flags)
{
	handle_type hsnap;
	if ((hsnap = CreateToolhelp32Snapshot(snapflags2integral(flags), pid)) == INVALID_HANDLE_VALUE) {
		throw pplugger::error::open_snap(pplugger::error::getlasterr());
	}
	return hsnap;
}

static unsigned long getsyspcount(const unsigned long len = 256UL)
{
	unsigned long count = 0UL;
	std::vector<pplugger::pid_type> array(len);

	if (false == EnumProcesses(array.data(), sizeof(array[0]) * len, &count)) {
		throw pplugger::error::read_procs(pplugger::error::getlasterr());
	} else if ((count /= sizeof(array[0])) >= len) {
		count = getsyspcount(len * 2UL);
	}

	return count;
}

std::vector<pplugger::pid_type> pplugger::os::getallpids()
{
	unsigned long count = 0UL;
	std::vector<pplugger::pid_type> pidlist(getsyspcount());

	if (false == EnumProcesses(pidlist.data(), sizeof(pidlist[0]) * pidlist.size(), &count)) {
		throw pplugger::error::read_procs(pplugger::error::getlasterr());
	} else if (count / (sizeof(pidlist[0])) != pidlist.size()) {
		throw PPLUGGER_LOGICERR("incorrect process list size");
	}

	if (pidlist.empty()) {
		throw PPLUGGER_LOGICERR("process list empty");
	}

	return pidlist;
}

static pagetype_type getpagetype(const unsigned long type)
{
	switch(type) {
	case MEM_IMAGE:
		return pagetype_type::image;
		break;
	case MEM_MAPPED:
		return pagetype_type::mapped;
		break;
	case MEM_PRIVATE:
		return pagetype_type::privatepage;
		break;
	default:
		return pagetype_type::invalid;
		break;
	}
}

static pagestate_type getpagestate(const unsigned long state)
{
	switch(state) {
	case MEM_COMMIT:
		return pagestate_type::commit;
		break;
	case MEM_FREE:
		return pagestate_type::free;
		break;
	case MEM_RESERVE:
		return pagestate_type::reserve;
		break;
	default:
		throw PPLUGGER_LOGICERR("unknown page state " + std::to_string(state));
		break;
	}
}

static protect_type getprottype(const unsigned long prot)
{
	switch(prot) {
	case PAGE_NOACCESS:
		return protect_type::noaccess;
		break;
	case PAGE_READONLY:
		return protect_type::readonly;
		break;
	case PAGE_READWRITE:
		return protect_type::readwrite;
		break;
	case PAGE_WRITECOPY:
		return protect_type::writecopy;
		break;
	case PAGE_EXECUTE:
		return protect_type::execute;
		break;
	case PAGE_EXECUTE_READ:
		return protect_type::execute_readonly;
		break;
	case PAGE_EXECUTE_READWRITE:
		return protect_type::execute_readwrite;
		break;
	case PAGE_EXECUTE_WRITECOPY:
		return protect_type::execute_writecopy;
		break;
	case PAGE_GUARD | PAGE_READONLY:
		return protect_type::guard_readonly;
		break;
	case PAGE_GUARD | PAGE_READWRITE:
		return protect_type::guard_readwrite;
		break;
	case PAGE_GUARD | PAGE_WRITECOPY:
		return protect_type::guard_writecopy;
		break;
	case PAGE_GUARD | PAGE_EXECUTE:
		return protect_type::guard_execute;
		break;
	case PAGE_GUARD | PAGE_EXECUTE_READ:
		return protect_type::guard_execute_readonly;
		break;
	case PAGE_GUARD | PAGE_EXECUTE_READWRITE:
		return protect_type::guard_execute_readwrite;
		break;
	case PAGE_GUARD | PAGE_EXECUTE_WRITECOPY:
		return protect_type::guard_execute_writecopy;
		break;
	case PAGE_NOCACHE | PAGE_READONLY:
		return protect_type::nocache_readonly;
		break;
	case PAGE_NOCACHE | PAGE_READWRITE:
		return protect_type::nocache_readwrite;
		break;
	case PAGE_NOCACHE | PAGE_WRITECOPY:
		return protect_type::nocache_writecopy;
		break;
	case PAGE_NOCACHE | PAGE_EXECUTE:
		return protect_type::nocache_execute;
		break;
	case PAGE_NOCACHE | PAGE_EXECUTE_READ:
		return protect_type::nocache_execute_readonly;
		break;
	case PAGE_NOCACHE | PAGE_EXECUTE_READWRITE:
		return protect_type::nocache_execute_readwrite;
		break;
	case PAGE_NOCACHE | PAGE_EXECUTE_WRITECOPY:
		return protect_type::nocache_execute_writecopy;
		break;
	default: 
		throw PPLUGGER_LOGICERR("unknown page protection " + std::to_string(prot));
		break;
	}
}

static pplugger::page getpagev1b32(const pplugger::addr_type addr, const proc_handle& hproc)
{
	unsigned long len = 0UL;
	MEMORY_BASIC_INFORMATION32 pageinfo;

	if (0UL == (len = VirtualQueryEx(hproc.get(), reinterpret_cast<void *>(addr), reinterpret_cast<MEMORY_BASIC_INFORMATION *>(&pageinfo), sizeof(pageinfo)))) {
		throw pplugger::error::read_page(pplugger::error::getlasterr());
	} else if (sizeof(pageinfo) != len) {
		throw PPLUGGER_LOGICERR("invalid return size for pageinfo");
	}
	return pplugger::page(pageinfo.BaseAddress, pageinfo.RegionSize, getprottype(pageinfo.Protect), getpagestate(pageinfo.State), getpagetype(pageinfo.Type));
}

#ifdef __WIN64
static pplugger::page getpagev1b64(const pplugger::addr_type addr, const proc_handle& hproc)
{
	unsigned long len = 0UL;
	MEMORY_BASIC_INFORMATION64 pageinfo;

	if (0UL == (len = VirtualQueryEx(hproc.get(), reinterpret_cast<void *>(addr), reinterpret_cast<MEMORY_BASIC_INFORMATION *>(&pageinfo), sizeof(pageinfo)))) {
		throw pplugger::error::read_page(pplugger::error::getlasterr());
	} else if (sizeof(pageinfo) != len) {
		throw PPLUGGER_LOGICERR("invalid return size for pageinfo");
	}
	return pplugger::page(pageinfo.BaseAddress, pageinfo.RegionSize, getprottype(pageinfo.Protect), getpagestate(pageinfo.State), getpagetype(pageinfo.Type));
}
#endif /* __WIN64 */

pplugger::page pplugger::os::getpagev1(const pplugger::addr_type addr, const proc_handle& hproc)
{
	pplugger::arch_type arch = getarch(hproc);
	switch(arch) {
	case pplugger::arch_type::amd64_cisc:
#ifdef __WIN64
		return getpagev1b64(addr, hproc);
#elif defined(__WIN32)
		throw pplugger::error::non_compat_arch();
#endif /* __WIN64 */
		break;
	case pplugger::arch_type::i386_cisc:
		return getpagev1b32(addr, hproc);
		break;
	default:
		throw PPLUGGER_LOGICERR("unknown arch for page read");
		break;
	}
}

std::shared_ptr<uint8_t[]> pplugger::os::readprocmem(const proc_handle& hproc, const pplugger::addr_type base, const pplugger::addr_type size)
{
	unsigned long long len = 0ULL;
	std::shared_ptr<uint8_t[]> mem = std::make_shared<uint8_t[]>(size);
	if (!ReadProcessMemory(hproc.get(), reinterpret_cast<void *>(base), mem.get(), size, reinterpret_cast<SIZE_T*>(&len))) {
		throw pplugger::error::read_mem(base, size, pplugger::error::getlasterr());
	} else if (size != len) {
		throw pplugger::error::read_mem();
	}
	return mem;
}

void pplugger::os::writeprocmem(const proc_handle& hproc, const pplugger::addr_type base, const pplugger::addr_type size, const void *const obj)
{
	unsigned long long len = 0ULL;
	if (!WriteProcessMemory(hproc.get(), reinterpret_cast<void *>(base), obj, size, reinterpret_cast<SIZE_T*>(&len))) {
		throw pplugger::error::write_mem(base, size, pplugger::error::getlasterr());
	} else if (size != len) {
		throw pplugger::error::write_mem();
	}
}

static unsigned long getenumprocmodsn(const proc_handle& proc, const unsigned long len = 256UL)
{
	unsigned long count = 0UL;
	std::vector<modhandle_type> array(len);

	if (false == EnumProcessModulesEx(proc.get(), array.data(), sizeof(array[0]) * len, &count, LIST_MODULES_ALL)) {
		throw pplugger::error::read_mods(pplugger::error::getlasterr());
	} else if ((count /= sizeof(array[0])) >= len) {
		count = getenumprocmodsn(proc, len * 2UL);
	}

	return count;
}

static std::vector<modhandle_type> getenumprocmods(const proc_handle& hproc)
{
	unsigned long count = 0UL;
	std::vector<modhandle_type> hmods(getenumprocmodsn(hproc));
	if (!EnumProcessModulesEx(hproc.get(), hmods.data(), sizeof(hmods[0]) * hmods.size(), &count, LIST_MODULES_ALL)) {
		throw pplugger::error::read_mods(pplugger::error::getlasterr());
	} else if (count / (sizeof(hmods[0])) != hmods.size()) {
		throw PPLUGGER_LOGICERR("incorrect module list size");
	}

	if (hmods.empty()) {
		throw PPLUGGER_LOGICERR("module list empty");
	}

	return hmods;
}

std::vector<pplugger::pmodule> pplugger::os::getprocmodsv1(const proc_handle& hproc)
{
	std::vector<modhandle_type> hmods = getenumprocmods(hproc);
	std::vector<pplugger::pmodule> mods = {};
	for (const modhandle_type ht : hmods) {
		try {
			mods.push_back(pplugger::pmodule(hproc, ht));
		} catch(pplugger::error::oserror&) {}
	}
	return mods;
}

static MODULEENTRY32 getwinmodfirst(const snap_handle& hsnap, MODULEENTRY32 *const entry)
{
	if (nullptr == entry) {
		throw PPLUGGER_LOGICERR("received nullptr");
	}
	if (false == Module32First(hsnap.get(), entry)) {
		if (pplugger::error::getlasterr() == ERROR_NO_MORE_FILES) {
			throw std::range_error("no more modules to read");
		} else {
			throw pplugger::error::read_mods(pplugger::error::getlasterr());
		}
	}
	return *entry;
}

static MODULEENTRY32 getwinmodnext(const snap_handle& hsnap, MODULEENTRY32 *const entry)
{
	if (nullptr == entry) {
		throw PPLUGGER_LOGICERR("received nullptr");
	}
	if (false == Module32Next(hsnap.get(), entry)) {
		if (pplugger::error::getlasterr() == ERROR_NO_MORE_FILES) {
			throw std::range_error("no more modules to read");
		} else {
			throw pplugger::error::read_modinfo(pplugger::error::getlasterr());
		}
	}
	return *entry;
}

static pplugger::pmodule mkmod(const MODULEENTRY32 entry)
{
	std::string name(entry.szModule);
	if (name.empty()) {
		name = entry.szExePath;
	}
	return pplugger::pmodule(name,
			reinterpret_cast<pplugger::addr_type>(entry.modBaseAddr),
			entry.modBaseSize);
}

std::vector<pplugger::pmodule> pplugger::os::getprocmodsv2(const proc_handle& proc)
{
	std::vector<pplugger::pmodule> mods = {};
	snap_handle hsnap(proc.getpid(), { snapflag_type::snapmodules });

	MODULEENTRY32 entry = {};
	entry.dwSize = sizeof(entry);

	try {
		mods.push_back(mkmod(getwinmodfirst(hsnap, &entry)));
		do { 
			mods.push_back(mkmod(getwinmodnext(hsnap, &entry)));
		} while(true);
	} catch(std::range_error&) {}

	return mods;
}

std::string pplugger::os::getprocname(const proc_handle& handle)
{
	char buf[256];
	if (0 == GetModuleBaseNameA(handle.get(), nullptr, buf, sizeof(buf) / sizeof(buf[0]))) {
		throw pplugger::error::read_procname(pplugger::error::getlasterr());
	}

	std::string name(buf);
	if (name.empty()) {
		throw PPLUGGER_LOGICERR("process name empty");
	}
	return name;
}

std::string pplugger::os::getmodname(const proc_handle& hproc, const mod_handle& hmod)
{
	char buf[256];
	if (0 == GetModuleBaseNameA(hproc.get(), hmod.get(), buf, sizeof(buf) * sizeof(buf[0]))) {
		throw pplugger::error::read_modname(pplugger::error::getlasterr());
	}

	std::string name(buf);
	if (name.empty()) {
		throw PPLUGGER_LOGICERR("module name empty");
	}
	return name;
}

static MODULEINFO getmodinfo(const proc_handle& hproc, const mod_handle& hmod)
{
	MODULEINFO modinfo = {};
	if (false == GetModuleInformation(hproc.get(), hmod.get(), &modinfo, sizeof(modinfo))) {
		throw pplugger::error::read_modinfo(pplugger::error::getlasterr());
	}
	return modinfo;
}

pplugger::addr_type pplugger::os::getmodbase(const proc_handle& hproc, const mod_handle& hmod)
{
	return reinterpret_cast<pplugger::addr_type>(getmodinfo(hproc, hmod).lpBaseOfDll);
}

pplugger::image_size pplugger::os::getmodsize(const proc_handle& hproc, const mod_handle& hmod)
{
	return reinterpret_cast<pplugger::image_size>(getmodinfo(hproc, hmod).SizeOfImage);
}
