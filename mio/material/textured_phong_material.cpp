#include <mio/material/textured_phong_material.h>
#include <mio/material/material_visitor.h>

namespace mio
{
	namespace material
	{
		TexturedPhongMaterial::TexturedPhongMaterial(
			Rgba _ambient, 
			Rgba _diffuse, 			
			Rgba _emission,
			Rgba _specular, 
			float _shininess,
			Texture *_texture,
			const char *name) : PhongMaterial(_ambient, _diffuse, _emission, _specular, _shininess, name)
		{
			texture_ = _texture;
		}

		TexturedPhongMaterial::~TexturedPhongMaterial()
		{
			texture_ = NULL;
		}

		void TexturedPhongMaterial::accept(mio::material::MaterialVisitor *visitor)
		{
			visitor->visit_TexturedPhongMaterial(this);
		}
	}
}