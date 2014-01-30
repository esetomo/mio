#ifndef __MIO_MATERIAL_BY_TRIANGLE_MATERIAL_MAPPING_H__
#define __MIO_MATERIAL_BY_TRIANGLE_MATERIAL_MAPPING_H__

#include <mio/mio.h>
#include <mio/material/material_mapping.h>
#include <mio/array.h>

#pragma warning(push)
#pragma warning(disable : 4251)

namespace mio
{
	namespace material
	{
		class MIO_API ByPolygonMaterialMapping : public MaterialMapping
		{
		private:
			Array<int> mappings_;

		public:					
			ByPolygonMaterialMapping(const char *name="");
			virtual ~ByPolygonMaterialMapping();

			
			inline virtual int _get_material(int triangleIndex) const
			{
				return mappings_[triangleIndex];
			}

			inline virtual void append_mapping(int mapping)
			{
				mappings_.push_back(mapping);
			}

			inline virtual int get_polygon_count() const
			{
				return mappings_.size();
			}

			virtual void accept(mio::material::MaterialMappingVisitor *visitor);
		};
	}
}

#pragma warning(pop)

#endif