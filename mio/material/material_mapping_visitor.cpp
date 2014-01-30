#include <mio/material/material_mapping_visitor.h>

namespace mio
{
	namespace material
	{
		MaterialMappingVisitor::~MaterialMappingVisitor()
		{
		}

		void MaterialMappingVisitor::visit_ByPolygonMaterialMapping(mio::material::MaterialMapping *mapping)
		{
			throw "Not implmented yet!";
		}
	}
}