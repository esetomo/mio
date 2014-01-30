#include <mio/material/material_visitor.h>
#include <mio/exceptions.h>

namespace mio
{
	namespace material
	{
		MaterialVisitor::~MaterialVisitor()
		{
		}		

		void MaterialVisitor::visit_PhongMaterial(mio::material::PhongMaterial * material)
		{
			throw NotImplementedException();
		}

		void MaterialVisitor::visit_TexturedPhongMaterial(mio::material::TexturedPhongMaterial *material)
		{
			throw NotImplementedException();
		}

		void MaterialVisitor::visit_MqoMaterial(mio::material::MqoMaterial *material)
		{
			throw NotImplementedException();
		}

		void MaterialVisitor::visit_TexturedMqoMaterial(mio::material::TexturedMqoMaterial *material)
		{
			throw NotImplementedException();
		}
	}
}