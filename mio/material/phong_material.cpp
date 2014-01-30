#include <mio/material/phong_material.h>
#include <mio/material/material_visitor.h>

namespace mio
{
	namespace material
	{
		PhongMaterial::PhongMaterial(
			Rgba _ambient,
			Rgba _diffuse,
			Rgba _emission,
			Rgba _specular,
			float _shininess,
			const char *name) : Material(name)
		{			
			ambient = _ambient; 
			diffuse = _diffuse;
			specular = _specular;
			emission = _emission;
			shininess = _shininess;
		}


		PhongMaterial::~PhongMaterial()
		{
		}

		void PhongMaterial::accept(mio::material::MaterialVisitor *visitor)
		{
			visitor->visit_PhongMaterial(this);
		}
	}
}