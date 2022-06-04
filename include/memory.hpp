#ifndef PPLUGGER_MEMORY_HPP
#define PPLUGGER_MEMORY_HPP

#include <cstdint>
#include <memory>
#include <vector>

#include "types.hpp"

namespace pplugger {
	class page {
		addr_type base  = 0;
		addr_type size  = 0;
		pplugger::os::protect_type   prot  = pplugger::os::protect_type::invalid;
		pplugger::os::pagestate_type state = pplugger::os::pagestate_type::invalid;
		pplugger::os::pagetype_type  type  = pplugger::os::pagetype_type::invalid;
	public:
		page();
		page(const addr_type addr, const pplugger::os::proc_handle& hproc);
		page(const addr_type base, const addr_type      size,
				const pplugger::os::protect_type   prot,
				const pplugger::os::pagestate_type state,
				const pplugger::os::pagetype_type  type);

		addr_type getbase() const;
		addr_type getsize() const;
		pplugger::os::protect_type getprot() const;
		pplugger::os::pagestate_type getstate() const;
		pplugger::os::pagetype_type gettype() const;

		bool contains(const addr_type addr) const;
		bool readable() const;
		bool writeable() const;
		bool isinvalid() const;
	};

	class memory {
		pid_type pid = 0;
		std::vector<page> pages{};
		pplugger::os::proc_handle readhandle{};
		pplugger::os::proc_handle writehandle{};
		void setupreadhandle();
		void setupwritehandle();
	public:
		memory(const pid_type pid);
		memory(memory&) = delete;
		memory(memory&& m);

		memory& operator=(memory&) = delete;
		memory& operator=(memory&& m);

		template<class T> inline T read(const addr_type addr) { return *reinterpret_cast<T *>(readmem(addr, sizeof(T)).get()); }
		template<class T> inline void write(const addr_type addr, const T obj) { writemem(addr, sizeof(T), &obj); }
		std::shared_ptr<uint8_t[]> readmem(const addr_type base, const addr_type size);
		void writemem(const addr_type base, const addr_type size, const void *const obj);
		page getpage(const addr_type addr);
	};
}

#endif /* PPLUGGER_MEMORY_HPP */
