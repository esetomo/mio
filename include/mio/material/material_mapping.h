#ifndef __MIO_MATERIAL_MATERIAL_MAPPING_H__
#define __MIO_MATERIAL_MATERIAL_MAPPING_H__

#include <mio/mio.h>
#include <mio/exceptions.h>
#include <mio/mio_object.h>

namespace mio
{
	namespace material
	{
		class MaterialMappingVisitor;

		class MIO_API MaterialMapping : public MioObject
		{
		public:
			MaterialMapping(const char *name="");
			virtual ~MaterialMapping();
			virtual int _get_material(int polygon_index) const = 0;
			
			virtual int get_material(int polygon_index) const
			{
				if (polygon_index < 0 || polygon_index >= get_polygon_count())
					throw mio::IndexException("MaterialMapping::get_material() : polygon index out of range");
				else
					return _get_material(polygon_index);
			}

			virtual int get_polygon_count() const = 0;

			virtual void accept(mio::material::MaterialMappingVisitor *visitor);
		};
	}
}

#endif