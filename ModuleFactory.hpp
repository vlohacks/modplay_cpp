#ifndef _MODULE_FACTORY_HPP
#define _MODULE_FACTORY_HPP

#include "Module.hpp"
#include "ModuleMOD.hpp"
#include "ModuleS3M.hpp"

namespace vmp
{
	class ModuleFactory
	{
	public:
		static Module* byHeader(Io& io);
	};
}

#endif
