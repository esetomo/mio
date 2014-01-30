#ifndef __MIO_GL_GL_TEXTURE_H__
#define __MIO_GL_GL_TEXTURE_H__

#include <mio/mio.h>
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <mio/mio_object.h>
#include <mio/gl/gl_exception.h>

namespace mio
{
	namespace gl
	{		
		class MIO_API GLTexture : public MioObject
		{
		private:
			GLuint texID_;

			GLint components_;
			GLenum format_;
			GLenum type_;
			GLint width_;
			GLint height_;

			GLint mag_filter_;
			GLint min_filter_;
			GLint wrap_s_;
			GLint wrap_t_;

			bool mipmapBuilt_;

		public:			
			GLTexture();
			virtual ~GLTexture();

			void bind();
			void download(GLint _components, 
				int width, int height, 
				GLenum _format, GLenum _type, 
				const GLvoid *data);
			void build_mipmap(GLint _components, 
				int width, int height, 
				GLenum _format, GLenum _type, 
				const GLvoid *data);
		};
	}
}

#endif