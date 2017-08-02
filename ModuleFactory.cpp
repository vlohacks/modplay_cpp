#include "ModuleFactory.hpp"
#include "Exception.hpp"

namespace vmp
{
	/* Loads a mod by specific header information in io and returns
	 * a object if one loader matches 
	 */
	Module* ModuleFactory::byHeader(Io& io) 
	{	
		Module* mod = 0;

		if (ModuleS3M::loadCheck(io)) { mod = new ModuleS3M(); }
		if (ModuleMOD::loadCheck(io)) { mod = new ModuleMOD(); }
		
		if (mod == 0) {
			throw Exception("ModuleFactory: no loader found for module");
		} else {
			mod->load(io);
		}
		
		return mod;
	}
}
