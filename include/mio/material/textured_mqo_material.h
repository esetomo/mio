#ifndef __MIO_TEXTURED_MQO_MATERIAL_H__
#define __MIO_TEXTURED_MQO_MATERIAL_H__

#include <mio/texture/texture.h>
#include <mio/material/mqo_material.h>
#include <mio/reference.h>
#include <mio/mio.h>

namespace mio
{
	namespace material
	{
		using namespace texture;

		class MIO_API TexturedMqoMaterial : public MqoMaterial
		{
		private:
			Reference<Texture> texture_;

		public:		
			TexturedMqoMaterial(Rgba _color,
				float _ambient, 
				float _diffuse, 				
				float _emission,
				float _specular, 
				float _power,
				Texture *_texture,
				const char *name="");
			virtual ~TexturedMqoMaterial();

			inline Texture *get_texture()
			{
				return texture_.get_pointer();
			}

			inline void set_texture(Texture *texture)
			{
				texture_ = texture;
			}

			void accept(mio::material::MaterialVisitor *visitor);
		};
	}
}

#endif