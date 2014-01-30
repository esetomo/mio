#ifndef __MIO_MATERIAL_MATERIAL_LIST_H__
#define __MIO_MATERIAL_MATERIAL_LIST_H__

#include <mio/mio.h>
#include <mio/exceptions.h>
#include <vector>
#include <mio/mio_object.h>
#include <mio/reference.h>
#include <mio/material/material.h>

using namespace std;

#pragma warning(push)
#pragma warning(disable : 4251)

namespace mio
{
	namespace material
	{
		class MIO_API MaterialList : public MioObject
		{
		protected:
			vector< Reference<Material> > materials_;

		public:
			MaterialList(const char *name="");
			virtual ~MaterialList();

			inline void append_material(Material *material)
			{
				materials_.push_back(Reference<Material>(material));
			}

			inline int get_material_count()
			{
				return (int)materials_.size();
			}

			inline Material *_get_material(int materialIndex)
			{
				return materials_[materialIndex].get_pointer();
			}

			inline Material *get_material(int index)
			{
				if (index < 0 || index > get_material_count())
					throw IndexException("material index not in range");
				else
					return _get_material(index);
			}
		};
	}
}

#pragma warning(pop)

#endif
