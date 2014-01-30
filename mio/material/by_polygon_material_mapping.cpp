#include <mio/material/by_polygon_material_mapping.h>
#include <mio/material/material_mapping_visitor.h>

namespace mio
{
	namespace material
	{
		ByPolygonMaterialMapping::ByPolygonMaterialMapping(const char *name) : MaterialMapping(name)
		{
		}

		ByPolygonMaterialMapping::~ByPolygonMaterialMapping()
		{
		}

		void ByPolygonMaterialMapping::accept(mio::material::MaterialMappingVisitor *visitor)
		{
			visitor->visit_ByPolygonMaterialMapping(this);
		}
	}
}