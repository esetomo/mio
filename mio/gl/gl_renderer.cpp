#include <windows.h>
#include <GL/gl.h>
#include <mio/gl/gl_renderer.h>
#include <mio/gl/gl_exception.h>
#include <mio/geometry/mesh.h>

namespace mio
{
    namespace gl
    {
		Rgba GLRenderer::normal_color_ = Rgba(1.0, 0.0, 0.0);
		Rgba GLRenderer::wireframe_color_ = Rgba(1.0, 0.0, 1.0);

        GLRenderer::GLRenderer()
        {
			current_material_index_ = -1;
			draw_surface_ = true;
			draw_normals_ = false;
			draw_wireframe_ = false;
        }

        GLRenderer::~GLRenderer()
        {
        }

		void GLRenderer::reset_state()
		{
			current_material_index_ = -1;		
		}

		void GLRenderer::visit_SimplePrimitive(mio::scenegraph::SimplePrimitive *primitive)
		{
			mio::geometry::Mesh *mesh = primitive->get_mesh();
			mio::texture::TextureMapping *texture_mapping = primitive->get_texture_mapping();
			mio::material::MaterialMapping *material_mapping = primitive->get_material_mapping();			
			mio::material::MaterialList *material_list = primitive->get_material_list();

			current_material_index_ = -1;

			int polygon_count = mesh->get_polygon_count();

			if (draw_surface_)
			{
				glEnable(GL_LIGHTING);
				FOR(polygon_index, polygon_count)
				{				
					int material = material_mapping->_get_material(polygon_index);

					if (material != current_material_index_)
					{						
						material_list->get_material(material)->accept(this);
						current_material_index_ = material;
					}

					glBegin(GL_POLYGON);
					FOR(vertex_offset, mesh->get_polygon_vertex_count(polygon_index))
					{
						Uv tex_coord = texture_mapping->_get_polygon_tex_coord(polygon_index, vertex_offset);
						Vector3 normal = mesh->_get_polygon_normal(polygon_index, vertex_offset);
						Point3 vertex = mesh->_get_polygon_vertex(polygon_index, vertex_offset);

						glTexCoord2f(tex_coord.u, tex_coord.v);
						glNormal3f(normal.x, normal.y, normal.z);
						glVertex3f(vertex.x, vertex.y, vertex.z);
					}
					glEnd();
				}				
			}

			if (draw_normals_)
			{
				glDisable(GL_LIGHTING);
				glBegin(GL_LINES);
				glColor3f(normal_color_.r, normal_color_.g, normal_color_.b);
				
				FOR(polygon_index, polygon_count)
				{
					FOR(vertex_offset, mesh->get_polygon_vertex_count(polygon_index))
					{
						Vector3 normal = mesh->_get_polygon_normal(polygon_index, vertex_offset);
						Point3 normal_end_0 = mesh->_get_polygon_vertex(polygon_index, vertex_offset);
						Point3 normal_end_1 = normal_end_0 + 0.1f * normal;

						glVertex3f(normal_end_0.x, normal_end_0.y, normal_end_0.z);
						glVertex3f(normal_end_1.x, normal_end_1.y, normal_end_1.z);
					}
				}
								
				glEnd();
			}

			if (draw_wireframe_)
			{
				glDisable(GL_LIGHTING);
				glColor3f(wireframe_color_.r, wireframe_color_.g, wireframe_color_.b);

				FOR(polygon_index, polygon_count)
				{
					glBegin(GL_LINE_LOOP);
					FOR(vertex_offset, mesh->get_polygon_vertex_count(polygon_index))
					{					
						Point3 vertex = mesh->_get_polygon_vertex(polygon_index, vertex_offset);
						glVertex3f(vertex.x, vertex.y, vertex.z);
					}
					glEnd();
				}
			}
		}		

		void GLRenderer::visit_RiggedPrimitive(mio::scenegraph::RiggedPrimitive *primitive)		
		{
			visit_SimplePrimitive(primitive->get_morphed_primitive());
		}

		void GLRenderer::visit_Group(mio::scenegraph::Group *group)
		{
			int childrenCount = group->get_children_count();
			FOR(childIndex, childrenCount)
			{
				mio::scenegraph::Node *child = group->_get_child(childIndex);
				child->accept(this);				
			}
		}

		void GLRenderer::visit_TransformNode(mio::scenegraph::TransformNode *transformNode)
		{
			glPushMatrix();
			float matrix[16];
			transformNode->get_transform().m.get_column_major_array(matrix);
			glMultMatrixf(matrix);
			transformNode->get_child()->accept(this);
			glPopMatrix();
		}

		void GLRenderer::visit_PhongMaterial(mio::material::PhongMaterial *material)
		{			
			glMaterialfv(GL_FRONT, GL_AMBIENT, material->ambient.get_pointer());
			glMaterialfv(GL_FRONT, GL_DIFFUSE, material->diffuse.get_pointer());
			glMaterialfv(GL_FRONT, GL_SPECULAR, material->specular.get_pointer());
			glMaterialfv(GL_FRONT, GL_EMISSION, material->emission.get_pointer());
			glMaterialf(GL_FRONT, GL_SHININESS, material->shininess);
			glDisable(GL_TEXTURE_2D);
		}

		void GLRenderer::visit_TexturedPhongMaterial(mio::material::TexturedPhongMaterial *material)
		{
			glMaterialfv(GL_FRONT, GL_AMBIENT, material->ambient.get_pointer());
			glMaterialfv(GL_FRONT, GL_DIFFUSE, material->diffuse.get_pointer());
			glMaterialfv(GL_FRONT, GL_SPECULAR, material->specular.get_pointer());
			glMaterialfv(GL_FRONT, GL_EMISSION, material->emission.get_pointer());
			glMaterialf(GL_FRONT, GL_SHININESS, material->shininess);
			material->get_texture()->accept(this);			
		}

		void GLRenderer::visit_MqoMaterial(mio::material::MqoMaterial *material)
		{			
			Rgba ambient = material->ambient * material->color;
			ambient.a = 1.0f;
			glMaterialfv(GL_FRONT, GL_AMBIENT, ambient.get_pointer());
			Rgba diffuse = material->diffuse * material->color;
			diffuse.a = 1.0f;
			glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse.get_pointer());
			Rgba specular = material->specular * material->color;
			specular.a = 1.0f;
			glMaterialfv(GL_FRONT, GL_SPECULAR, specular.get_pointer());
			Rgba emission = material->emission * material->color;
			emission.a = 1.0f;
			glMaterialfv(GL_FRONT, GL_EMISSION, emission.get_pointer());
			glMaterialf(GL_FRONT, GL_SHININESS, material->power);
			glDisable(GL_TEXTURE_2D);
		}

		void GLRenderer::visit_TexturedMqoMaterial(mio::material::TexturedMqoMaterial *material)
		{			
			Rgba ambient = material->ambient * material->color; ambient.a = 1.0f;			
			glMaterialfv(GL_FRONT, GL_AMBIENT, ambient.get_pointer());
			Rgba diffuse = material->diffuse * material->color; diffuse.a = 1.0f;			
			glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse.get_pointer());
			Rgba specular = material->specular * material->color; specular.a = 1.0f;			
			glMaterialfv(GL_FRONT, GL_SPECULAR, specular.get_pointer());
			Rgba emission = material->emission * material->color; emission.a = 1.0f;			
			glMaterialfv(GL_FRONT, GL_EMISSION, emission.get_pointer());
			glMaterialf(GL_FRONT, GL_SHININESS, material->power);
			material->get_texture()->accept(this);			
		}

		void GLRenderer::visit_ILTexture(mio::texture::ILTexture *texture)
		{
			GLTexture *glTexture = texture->get_image()->get_texture();
			glTexture->bind();
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		}

		void GLRenderer::visit_LazyILTexture(mio::texture::LazyILTexture *texture)
		{
			mio::texture::ILTexture *il_texture = texture->get_il_texture();
			visit_ILTexture(il_texture);
		}

		void GLRenderer::set_draw_normals(bool drawNormals)
		{
			draw_normals_ = drawNormals;
		}

		void GLRenderer::set_draw_wireframes(bool drawWireframe)
		{
			draw_wireframe_ = drawWireframe;
		}

		void GLRenderer::set_draw_surface(bool drawSurface)
		{
			draw_surface_ = drawSurface;
		}
    }
}
