#include <mio/material/material.h>

namespace mio
{
	namespace material
	{
		Material::Material(const char *name) : MioObject(name)
		{
		}

		Material::~Material()
		{
		}

		void Material::accept(mio::material::MaterialVisitor *visitor)
		{
			throw "Not implmented yet!";
		}
	}
}