#include <cstdint>
#include <memory>
#include <utility>

#include "exceptions.hpp"
#include "memory.hpp"
#include "types.hpp"
#include "windows.hpp"

using namespace pplugger;

pplugger::page::page() {}

pplugger::page::page(const addr_type addr, const pplugger::os::proc_handle& hproc)
{
	*this = pplugger::os::getpagev1(addr, hproc);
}

pplugger::page::page(const addr_type base, const addr_type size, const pplugger::os::protect_type prot, const pplugger::os::pagestate_type state, const pplugger::os::pagetype_type type) : base(base), size(size), prot(prot), state(state), type(type) {}

addr_type pplugger::page::getbase() const
{
	if (isinvalid()) {
		throw PPLUGGER_LOGICERR("cannot reference uninitialized page base");
	}
	return base;
}

addr_type pplugger::page::getsize() const
{
	if (isinvalid()) {
		throw PPLUGGER_LOGICERR("cannot reference uninitialized page size");
	}
	return size;
}

pplugger::os::protect_type pplugger::page::getprot() const
{
	if (isinvalid()) {
		throw PPLUGGER_LOGICERR("cannot reference uninitialized page protection");
	}
	return prot;
}

pplugger::os::pagestate_type pplugger::page::getstate() const
{
	if (isinvalid()) {
		throw PPLUGGER_LOGICERR("cannot reference uninitialized page state");
	}
	return state;
}

pplugger::os::pagetype_type pplugger::page::gettype() const
{
	if (isinvalid()) {
		throw PPLUGGER_LOGICERR("cannot reference uninitialized page type");
	}
	return type;
}

bool pplugger::page::contains(const addr_type addr) const
{
	if (isinvalid()) {
		throw PPLUGGER_LOGICERR("cannot reference uninitialized page for contains");
	}
	if (base <= addr && base + size > addr) {
		return true;
	} else {
		return false;
	}
}

bool pplugger::page::isinvalid() const
{
	if (base && size
			&& (pplugger::os::protect_type::invalid != prot)
			&& (pplugger::os::pagetype_type::invalid != type)) {
		return false;
	} else {
		return true;
	}
}

bool pplugger::page::readable() const
{
	if (!isinvalid()
			&& isreadablestate(state)
			&& isreadabletype(type)
			&& isreadableprot(prot)) {
		return true;
	} else {
		return false;
	}
}

bool pplugger::page::writeable() const
{
	if (!isinvalid()
			&& iswriteablestate(state)
			&& iswriteabletype(type)
			&& iswriteableprot(prot)) {
		return true;
	} else {
		return false;
	}
}

pplugger::memory::memory(const pid_type pid) : pid(pid) {}

pplugger::memory::memory(memory&& m)
{
	*this = std::move(m);
}

memory& pplugger::memory::operator=(memory&& m)
{
	if (this == &m) {
		throw PPLUGGER_LOGICERR("cannot move memory obj to itself");
	}
	pages = std::move(m.pages);
	readhandle = std::move(m.readhandle);
	writehandle = std::move(m.writehandle);
	pid = m.pid;
	m.pid = 0;

	return *this;
} 

page pplugger::memory::getpage(const addr_type addr)
{
	bool found = false;
	page p;
	for (const page i : pages) {
		if (i.contains(addr)) {
			found = true;
			p = i;
			break;
		}
	}
	if (!found) {
		p = page(addr, readhandle);
		pages.push_back(p);
	} 
	return p;
}

std::shared_ptr<uint8_t[]> pplugger::memory::readmem(const addr_type base, const addr_type size)
{
	setupreadhandle();
	try {
		page page = getpage(base);
		if (!page.readable()) {
			throw pplugger::error::bad_ptr(base);
		}
	} catch(pplugger::error::read_page&) {}

	return pplugger::os::readprocmem(readhandle, base, size);
}

void pplugger::memory::writemem(const addr_type base, const addr_type size, const void *const obj)
{
	setupreadhandle();
	setupwritehandle();
	try {
		page page = getpage(base);
		if (!page.writeable()) {
			throw pplugger::error::bad_ptr(base);
		}
	} catch(pplugger::error::read_page&) {}

	return pplugger::os::writeprocmem(writehandle, base, size, obj);
}

void pplugger::memory::setupreadhandle()
{
	if (!readhandle.isopen()) {
		readhandle = pplugger::os::proc_handle(pid, { pplugger::os::procflag_type::vm_read, pplugger::os::procflag_type::query_limited_information });
	}
}

void pplugger::memory::setupwritehandle()
{
	if (!writehandle.isopen()) {
		writehandle = pplugger::os::proc_handle(pid, { pplugger::os::procflag_type::vm_write, pplugger::os::procflag_type::vm_operation });
	}
}
