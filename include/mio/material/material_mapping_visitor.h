#ifndef __MIO_MATERIAL_MATERIAL_MAPPING_VISITOR_H__
#define __MIO_MATERIAL_MATERIAL_MAPPING_VISITOR_H__

#include <mio/material/material_mapping.h>
#include <mio/material/by_polygon_material_mapping.h>

namespace mio
{
	namespace material
	{
		class MIO_API MaterialMappingVisitor
		{
		public:
			virtual ~MaterialMappingVisitor();			

			virtual void visit_ByPolygonMaterialMapping(mio::material::MaterialMapping *mapping);
		};
	}
}

#endif