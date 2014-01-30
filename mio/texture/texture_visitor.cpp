#include <mio/texture/texture_visitor.h>

namespace mio
{
	namespace texture
	{
		TextureVisitor::TextureVisitor()
		{
		}

		TextureVisitor::~TextureVisitor()
		{
		}		

		void TextureVisitor::visit_ILTexture(mio::texture::ILTexture *texture)
		{
			throw "Not implmented yet!";
		}

		void TextureVisitor::visit_LazyILTexture(mio::texture::LazyILTexture *texture)
		{
			throw "Not implmented yet!";
		}
	}
}