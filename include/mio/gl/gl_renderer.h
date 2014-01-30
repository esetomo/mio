#ifndef __MIO_GL_GL_RENDERER_H__
#define __MIO_GL_GL_RENDERER_H__

#include <mio/mio.h>
#include <mio/scenegraph/node_visitor.h>
#include <mio/material/material_visitor.h>
#include <mio/texture/texture_visitor.h>
#include <mio/reference.h>

namespace mio
{
    namespace gl
    {
		class MIO_API GLRenderer : 
			public mio::scenegraph::NodeVisitor, 
			public mio::material::MaterialVisitor, 
			public mio::material::TextureVisitor
        {
		private:			
			int current_material_index_;			

			bool draw_normals_;
			bool draw_wireframe_;
			bool draw_surface_;

			static Rgba normal_color_;
			static Rgba wireframe_color_;

        public:
            GLRenderer();
            virtual ~GLRenderer();

			virtual void visit_SimplePrimitive(mio::scenegraph::SimplePrimitive *primitive);			
			virtual void visit_Group(mio::scenegraph::Group *group);
			virtual void visit_TransformNode(mio::scenegraph::TransformNode *transformNode);			
			virtual void visit_RiggedPrimitive(mio::scenegraph::RiggedPrimitive *primitive);			

			virtual void visit_PhongMaterial(mio::material::PhongMaterial *material);
			virtual void visit_TexturedPhongMaterial(mio::material::TexturedPhongMaterial *material);
			virtual void visit_MqoMaterial(mio::material::MqoMaterial *material);
			virtual void visit_TexturedMqoMaterial(mio::material::TexturedMqoMaterial *material);

			virtual void visit_ILTexture(mio::texture::ILTexture *texture);
			virtual void visit_LazyILTexture(mio::texture::LazyILTexture *texture);

			void reset_state();
			void set_draw_normals(bool drawNormals);
			void set_draw_wireframes(bool drawWireframe);
			void set_draw_surface(bool drawSurface);
        };
    }
}

#endif
