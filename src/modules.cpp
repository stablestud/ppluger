#include <algorithm>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <vector>

#include "exceptions.hpp"
#include "modules.hpp"
#include "types.hpp"
#include "windows.hpp"
#include "utils.hpp"

using namespace pplugger;

pplugger::pmodule::pmodule() {}

pplugger::pmodule::pmodule(const pplugger::os::proc_handle& hproc, const pplugger::os::mod_handle& hmod) :
	name(pplugger::os::getmodname(hproc, hmod)),
	base(pplugger::os::getmodbase(hproc, hmod)),
	size(pplugger::os::getmodsize(hproc, hmod)) {}

pplugger::pmodule::pmodule(const std::string name, addr_type base, addr_type size) :
	name(name),
	base(base),
	size(size) {}

std::string pplugger::pmodule::getname() const
{
	if (isinvalid()) {
		throw PPLUGGER_LOGICERR("cannot reference uninitialized module name");
	}
	return name;
}

addr_type pplugger::pmodule::getbase() const
{
	if (isinvalid()) {
		throw PPLUGGER_LOGICERR("cannot reference uninitialized module base");
	}
	return base;
}

addr_type pplugger::pmodule::getsize() const
{
	if (isinvalid()) {
		throw PPLUGGER_LOGICERR("cannot reference uninitialized module size");
	}
	return size;
}

bool pplugger::pmodule::isinvalid() const
{
	if (0 == size || 0 == base || name.empty()) {
		return true;
	} else {
		return false;
	}
}

static bool modequal(const pmodule& mod1, const pmodule& mod2)
{
	if ((mod1.getname() == mod2.getname())
			&& (mod1.getbase() == mod2.getbase())
			&& (mod1.getsize() == mod2.getsize())) {
		return true;
	} else {
		return false;
	}
}

static bool modsort(const pmodule& mod1, const pmodule& mod2)
{
	return (mod1.getbase() < mod2.getbase());
}

static void mergemods(std::vector<pmodule>& dest, std::vector<pmodule>& src)
{
	for (pmodule& mod : src) {
		dest.push_back(mod);
	}

	std::sort(dest.begin(), dest.end(), modsort);
	std::vector<pmodule>::iterator it = std::unique(dest.begin(), dest.end(), modequal);
	dest.resize(std::distance(dest.begin(), it));
}

pplugger::modules::modules() : mods() {}

pplugger::modules::modules(const pplugger::os::proc_handle& hproc)
{
	try { // Method 1
		std::vector<pmodule> mods1{pplugger::os::getprocmodsv1(hproc)};
		mergemods(mods, mods1);
	} catch(pplugger::error::oserror& e) {}

	try { // Method 2, as backup, if method 1 didn't work or didn't catch all pages
		std::vector<pmodule> mods2{pplugger::os::getprocmodsv2(hproc)};
		mergemods(mods, mods2);
	} catch(pplugger::error::oserror& e) {}

	if (mods.empty()) {
		throw pplugger::error::empty_mods();
	}
}

pmodule& pplugger::modules::addr(const addr_type addr)
{
	if (mods.empty()) {
		throw PPLUGGER_LOGICERR("cannot return module from empty module list");
	}

	bool found = false;

	std::vector<pmodule>::iterator hit;
	for (std::vector<pmodule>::iterator it = mods.begin(); it < mods.end(); it++) {
		if ((addr >= (*it).getbase()) && (addr < ((*it).getbase() + (*it).getsize()))) {
			if (found) {
				throw PPLUGGER_LOGICERR("cannot have multiple modules in the same addr range: " + pplugger::util::to_hexstr(addr));
			}
			hit = it;
			found = true;
		}
	}
	return *hit;
}

pmodule& pplugger::modules::name(const std::string& name)
{
	if (mods.empty()) {
		throw PPLUGGER_LOGICERR("cannot return module from empty module list");
	}

	bool found = false;

	std::vector<pmodule>::iterator hit;
	for (std::vector<pmodule>::iterator it = mods.begin(); it < mods.end(); it++) {
		if (0 == (*it).getname().compare(name)) {
			if (found) {
				PPLUGGER_LOGICERR("cannot have multiple modules with the same name: " + name);
			}
			hit = it;
			found = true;
		}
	}
	return *hit;
}

std::vector<pmodule>& pplugger::modules::getmods()
{
	return mods;
}
