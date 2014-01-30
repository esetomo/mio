#ifndef __MIO_MATERIAL_MATERIAL_VISITOR_H__
#define __MIO_MATERIAL_MATERIAL_VISITOR_H__

#include <mio/mio.h>
#include <mio/material/material.h>
#include <mio/material/phong_material.h>
#include <mio/material/textured_phong_material.h>
#include <mio/material/mqo_material.h>
#include <mio/material/textured_mqo_material.h>

namespace mio
{
	namespace material
	{
		class MIO_API MaterialVisitor
		{
		public:
			virtual ~MaterialVisitor();			

			virtual void visit_PhongMaterial(mio::material::PhongMaterial *material);
			virtual void visit_TexturedPhongMaterial(mio::material::TexturedPhongMaterial *material);
			virtual void visit_MqoMaterial(mio::material::MqoMaterial *material);
			virtual void visit_TexturedMqoMaterial(mio::material::TexturedMqoMaterial *material);
		};
	}
}

#endif