#ifndef PPLUGGER_MODULES_HPP
#define PPLUGGER_MODULES_HPP

#include <string>
#include <vector>

#include "types.hpp"

namespace pplugger {
	class pmodule {
		std::string name{};
		addr_type   base = 0;
		image_size  size = 0;
	public:
		pmodule();
		pmodule(const pplugger::os::proc_handle& hproc, const pplugger::os::mod_handle& hmod);
		pmodule(const std::string name, addr_type base, addr_type size);

		bool isinvalid() const;
		addr_type getbase() const;
		addr_type getsize() const;
		std::string getname() const;
	};

	class modules {
		std::vector<pmodule> mods{};
	public:
		modules();
		modules(const pplugger::os::proc_handle& hproc);

		pmodule& addr(const addr_type addr);
		pmodule& name(const std::string& name);
		std::vector<pmodule>& getmods();
	};
}

#endif /* PPLUGGER_MODULES_HPP */
