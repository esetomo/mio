#ifndef __MIO_MATERIAL_MATERIAL_H__
#define __MIO_MATERIAL_MATERIAL_H__

#include <mio/mio.h>
#include <mio/mio_object.h>

namespace mio
{
	namespace material
	{
		class MaterialVisitor;

		class MIO_API Material : public MioObject
		{
		public:
			Material(const char *name="");
			virtual ~Material() = 0;

			virtual void accept(mio::material::MaterialVisitor *visitor);
		};
	}
}

#endif
