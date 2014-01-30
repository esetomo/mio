#include <mio/gl/gl_texture.h>

namespace mio
{
	namespace gl
	{
		GLTexture::GLTexture() : MioObject()
		{			
			glGetError();
			glGenTextures(1, &texID_);			
			GLint error = glGetError();
			if (error != GL_NO_ERROR)
				throw GLException("GLException: cannot generate new texture");

			components_ = 4;
			format_ = GL_RGBA;
			type_ = GL_UNSIGNED_BYTE;
			width_ = 0;
			height_ = 0;

			mag_filter_ = GL_LINEAR;
			min_filter_ = GL_LINEAR;
			wrap_s_ = GL_REPEAT;
			wrap_t_ = GL_REPEAT;

			mipmapBuilt_ = false;
		}

		GLTexture::~GLTexture()
		{
			glDeleteTextures(1, &texID_);			
		}

		void GLTexture::bind()
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texID_);
			
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter_);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter_);			
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s_);			
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t_);						
		}

		void GLTexture::download(GLint _components, 
			int _width, int _height, 
			GLenum _format, GLenum _type, 
			const GLvoid *data)
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texID_);
			glTexImage2D(GL_TEXTURE_2D, 0, _components, _width, _height, 0, _format, _type, data);

			components_ = _components;
			width_ = _width;
			height_ = _height;
			format_ = _format;
			type_ = _type;

			mipmapBuilt_ = false;
			if (min_filter_ != GL_NEAREST || min_filter_ != GL_LINEAR)
				min_filter_ = GL_LINEAR;
		}

		void GLTexture::build_mipmap(GLint _components, 
			int _width, int _height, 
			GLenum _format, GLenum _type, 
			const GLvoid *data)
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texID_);
			gluBuild2DMipmaps(GL_TEXTURE_2D, _components, _width, _height, _format, _type, data);			

			components_ = _components;
			width_ = _width;
			height_ = _height;
			format_ = _format;
			type_ = _type;

			mipmapBuilt_ = true;
			min_filter_ = GL_LINEAR_MIPMAP_LINEAR;
		}
	}
}