#ifndef PPLUGGER_PROCESS_HPP
#define PPLUGGER_PROCESS_HPP

#include <cstdint>
#include <string>
#include <vector>

#include "exceptions.hpp"
#include "memory.hpp"
#include "modules.hpp"
#include "types.hpp"

namespace pplugger {
	class process {
		pid_type    pid = 0;
		pplugger::os::proc_handle handle{};
		arch_type   arch = arch_type::invalid;
		std::string name{};
		modules	    mods{};
		memory      mem{0};
	public:
		process(const pid_type pid);
		process(const process&) = delete;
		process(process&& proc);

		process& operator=(process&) = delete;
		process& operator=(process&& proc);

		arch_type   getarch() const noexcept;
		std::string getname() const noexcept;
		pmodule&    getmodule(const std::string name);
		modules&    getmodules();
		pid_type    getpid()  const noexcept;
		template<class T> inline T    read(const addr_type addr) { return mem.read<T>(addr); }
		template<class T> inline void write(const addr_type addr, T obj) { mem.write<T>(addr, obj); }
	};

	class processes {
		std::vector<process> procs{};
	public:
		processes();
		processes(const processes&) = delete;

		processes& operator=(processes&) = delete;

		process& pid(const pid_type pid);
		process& name(const std::string& name);
	};

	process find(pid_type pid);
	process find(std::string name);
}

#endif /* PPLUGGER_PROCESS_HPP */
