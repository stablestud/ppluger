#ifndef PPLUGGER_TYPES_H
#define PPLUGGER_TYPES_H 

#include <string>
#include <utility>
#include <vector>

#include <basetsd.h>
#include <windef.h>
#include <winnt.h>

namespace pplugger {
	using addr_type = uintptr_t;
	using image_size = DWORD;
	using pid_type = DWORD;

	using arch_type = enum class _arch_type { invalid, i386_cisc, amd64_cisc };
	std::ostream& operator<<(std::ostream& out, arch_type arch);
	std::string   operator+(std::string str, arch_type arch);
}

namespace pplugger::os {
	using handle_type = HANDLE;
	using modhandle_type = HMODULE;

	using snapflag_type = enum class _snapflag_type { invalid,
		inherit,
		snapall, snapheaplist, snapmodules, snapmodules32, snapprocesses, snapthreads
	};

	using procflag_type = enum class _procflag_type { invalid,
		all_access,       
		create_process,    create_thread,
		dup_handle,        synchronize,
		query_information, query_limited_information,
		set_information,   set_quota,
		suspend_resume,    terminate,
		vm_operation,      vm_read,      vm_write,
		read_control,      obj_delete,
		write_dac,         write_owner
	};

	using pagestate_type = enum class _pagestate_type { invalid, commit, free, reserve };
	std::ostream& operator<<(std::ostream& out, pagestate_type pagestate);
	std::string   operator+(std::string str, pagestate_type pagestate);
	bool isreadablestate(const pagestate_type state);
	bool iswriteablestate(const pagestate_type state);

	using pagetype_type = enum class _pagetype_type { invalid, image, mapped, privatepage };
	std::ostream& operator<<(std::ostream& out, pagetype_type pagetype);
	std::string   operator+(std::string str, pagetype_type pagetype);
	bool isreadabletype(const pagetype_type type);
	bool iswriteabletype(const pagetype_type type);

	using protect_type = enum class _protect_type { invalid, noaccess,
		readonly, readwrite, writecopy,
		execute, execute_readonly, execute_readwrite, execute_writecopy,
		guard_readonly, guard_readwrite, guard_writecopy,
		guard_execute, guard_execute_readonly, guard_execute_readwrite, guard_execute_writecopy,
		nocache_readonly, nocache_readwrite, nocache_writecopy,
		nocache_execute, nocache_execute_readonly, nocache_execute_readwrite, nocache_execute_writecopy
	};

	std::ostream& operator<<(std::ostream& out, protect_type prot);
	std::string   operator+(std::string str, protect_type prot);
	bool isreadableprot(const protect_type prot);
	bool iswriteableprot(const protect_type prot);

	class handle {
		handle_type handl = nullptr;
	protected:
		handle();
		handle(const handle_type handl);
		virtual ~handle();
		handle_type gethandle() const;
	public:
		handle(handle&) = delete;
		handle(handle&& othrhandle);
		handle& operator=(handle&) = delete;
		handle& operator=(handle&& othr);

		virtual handle_type get() const;
	};

	class proc_handle : public handle {
		pplugger::pid_type pid = 0;
		std::vector<procflag_type> flags{};
	public:
		proc_handle();
		proc_handle(const pplugger::pid_type pid, const std::initializer_list<procflag_type> flags);
		proc_handle(proc_handle&& othr);
		proc_handle& operator=(proc_handle&& othr);

		virtual handle_type get() const;
		pplugger::pid_type getpid() const;
		bool isopen() const;
	};

	class snap_handle : public handle {
		pplugger::pid_type pid = 0;
		std::vector<snapflag_type> flags{};
	public:
		snap_handle();
		snap_handle(const pplugger::pid_type pid, const std::initializer_list<snapflag_type> flags);
		snap_handle(snap_handle&& othr);
		snap_handle& operator=(snap_handle&& othr);

		virtual handle_type get() const;
		pplugger::pid_type getpid() const;
		bool isopen() const;
	};

	class mod_handle {
		modhandle_type hmod = nullptr;
	public:
		mod_handle();
		mod_handle(const modhandle_type hmod);
		mod_handle(mod_handle&) = delete;
		mod_handle& operator=(mod_handle&) = delete;

		modhandle_type get() const;
	};
}

#endif /* PPLUGGER_TYPES_H */
