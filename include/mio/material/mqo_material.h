#ifndef __MIO_MATERIAL_MQO_MATERIAL_H__
#define __MIO_MATERIAL_MQO_MATERIAL_H__

#include <mio/material/material.h>
#include <mio/rgba.h>
#include <mio/mio.h>

namespace mio
{
	namespace material
	{
		class MIO_API MqoMaterial : public Material
		{
		public:
			Rgba color;
			float ambient;
			float diffuse;
			float specular;
			float emission;
			float power;
			
			MqoMaterial(Rgba _color, 
				float _ambient, 
				float _diffuse, 
				float _emission, 
				float _specular, 
				float _power,
				const char *name="");
			virtual ~MqoMaterial();

			virtual void accept(mio::material::MaterialVisitor *visitor);
		};
	}
}

#endif