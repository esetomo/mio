#include <mio/material/mqo_material.h>
#include <mio/material/material_visitor.h>

namespace mio
{
	namespace material
	{
		MqoMaterial::MqoMaterial(Rgba _color,
			float _ambient,
			float _diffuse,
			float _emission,
			float _specular,
			float _power,
			const char *name) : Material(name)
		{
			color = _color;
			ambient = _ambient; 
			diffuse = _diffuse;
			specular = _specular;
			emission = _emission;
			power = _power;
		}


		MqoMaterial::~MqoMaterial()
		{
		}

		void MqoMaterial::accept(mio::material::MaterialVisitor *visitor)
		{
			visitor->visit_MqoMaterial(this);
		}
	}
}