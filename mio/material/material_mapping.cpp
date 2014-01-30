#include <mio/material/material_mapping.h>
#include <mio/exceptions.h>

namespace mio
{
	namespace material
	{	
		MaterialMapping::MaterialMapping(const char *name) : MioObject(name)
		{
		}

		MaterialMapping::~MaterialMapping()
		{
		}

		void MaterialMapping::accept(mio::material::MaterialMappingVisitor *visitor)
		{
			throw NotImplementedException();
		}
	}
}