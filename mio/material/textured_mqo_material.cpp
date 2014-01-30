#include <mio/material/textured_mqo_material.h>
#include <mio/material/material_visitor.h>

namespace mio
{
	namespace material
	{
		TexturedMqoMaterial::TexturedMqoMaterial(mio::Rgba _color,
			float _ambient, 
			float _diffuse, 			
			float _emission,
			float _specular, 
			float _power,
			Texture *_texture,
			const char *name) : MqoMaterial(_color, _ambient, _diffuse, _emission, _specular, _power, name)
		{
			texture_ = _texture;
		}

		TexturedMqoMaterial::~TexturedMqoMaterial()
		{
			texture_ = NULL;
		}

		void TexturedMqoMaterial::accept(mio::material::MaterialVisitor *visitor)
		{
			visitor->visit_TexturedMqoMaterial(this);
		}
	}
}