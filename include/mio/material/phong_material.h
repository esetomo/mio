#ifndef __MIO_MATERIAL_PHONG_MATERIAL_H__
#define __MIO_MATERIAL_PHONG_MATERIAL_H__

#include <mio/material/material.h>
#include <mio/rgba.h>
#include <mio/mio.h>

namespace mio
{
	namespace material
	{
		class MIO_API PhongMaterial : public Material
		{
		public:
			Rgba ambient;
			Rgba diffuse;
			Rgba specular;
			Rgba emission;
			float shininess;			
			
			PhongMaterial(
				Rgba _ambient, 
				Rgba _diffuse, 
				Rgba _emission, 
				Rgba _specular, 
				float _shininess,
				const char *name="");
			virtual ~PhongMaterial();

			virtual void accept(mio::material::MaterialVisitor *visitor);
		};
	}
}

#endif