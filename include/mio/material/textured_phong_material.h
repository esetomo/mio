#ifndef __MIO_MATERIAL_TEXTURED_MQO_MATERIAL_H__
#define __MIO_MATERIAL_TEXTURED_MQO_MATERIAL_H__

#include <mio/material/phong_material.h>
#include <mio/reference.h>
#include <mio/texture/texture.h>
#include <mio/mio.h>

namespace mio
{
	namespace material
	{
		using namespace texture;

		class MIO_API TexturedPhongMaterial : public PhongMaterial
		{
		private:
			Reference<Texture> texture_;

		public:
			TexturedPhongMaterial(
				Rgba _ambient, 
				Rgba _diffuse, 				
				Rgba _emission,
				Rgba _specular, 
				float _shininess,
				Texture *_texture,
				const char *name="");
			virtual ~TexturedPhongMaterial();

			inline Texture *get_texture()
			{
				return texture_.get_pointer();
			}

			void set_texture(Texture *texture)
			{
				texture_ = texture;
			}
			

			virtual void accept(mio::material::MaterialVisitor *visitor);
		};
	}
}

#endif