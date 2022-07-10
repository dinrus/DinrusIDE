#ifndef _GlDraw_GlDraw_h_
#define _GlDraw_GlDraw_h_

#include <Draw/Draw.h>
#include <Painter/Painter.h>

#define GLEW_STATIC

#include <plugin/glew/glew.h>

#include <plugin/tess2/tess2.h>

#ifdef PLATFORM_WIN32
#include <plugin/glew/wglew.h>
#endif

#define GL_USE_SHADERS

#define GL_COMB_OPT

#include <GL/gl.h>

#ifdef Complex
#undef Complex
#endif

namespace РНЦП {

enum {
	TEXTURE_LINEAR     = 0x01,
	TEXTURE_MIPMAP     = 0x02,
	TEXTURE_COMPRESSED = 0x04,
};

GLuint CreateGLTexture(const Рисунок& img, dword flags);

GLuint GetTextureForImage(dword flags, const Рисунок& img, uint64 context = 0); // cached
inline GLuint GetTextureForImage(const Рисунок& img, uint64 context = 0) { return GetTextureForImage(TEXTURE_LINEAR|TEXTURE_MIPMAP, img, context); }

#ifdef GL_USE_SHADERS

enum {
	ATTRIB_VERTEX = 1,
	ATTRIB_COLOR,
	ATTRIB_TEXPOS,
	ATTRIB_ALPHA,
};

class GLProgram {
protected:
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint program;
	int64  serialid;

	void Compile(const char *vertex_shader_, const char *fragment_shader_);
	void Link();

public:
	void создай(const char *vertex_shader, const char *fragment_shader,
	            Tuple2<int, const char *> *bind_attr = NULL, int bind_count = 0);
	void создай(const char *vertex_shader, const char *fragment_shader,
	            int attr1, const char *attr1name);
	void создай(const char *vertex_shader, const char *fragment_shader,
	            int attr1, const char *attr1name,
	            int attr2, const char *attr2name);
	void создай(const char *vertex_shader, const char *fragment_shader,
	            int attr1, const char *attr1name,
	            int attr2, const char *attr2name,
	            int attr3, const char *attr3name);
	void очисть();
	
	int  GetAttrib(const char *имя)           { return glGetAttribLocation(program, имя); }
	int  GetUniform(const char *имя)          { return glGetUniformLocation(program, имя); }
	
	void Use();

	GLProgram();
	~GLProgram();
};

extern GLProgram gl_image, gl_image_colored, gl_rect;

#endif

class GLDraw : public SDraw {
	void устЦвет(Цвет c);

	uint64   context;
	
#ifdef GL_COMB_OPT
	struct RectColor : Движимое<RectColor> {
		Прям  rect;
		Цвет color;
	};
	Вектор<RectColor> put_rect;
#endif

	void FlushPutRect();

public:
	void    слей()                   { FlushPutRect(); }

	virtual void  PutImage(Точка p, const Рисунок& img, const Прям& src);
#ifdef GL_USE_SHADERS
	virtual void  PutImage(Точка p, const Рисунок& img, const Прям& src, Цвет color);
#endif
	virtual void  PutRect(const Прям& r, Цвет color);
	
	void иниц(Размер sz, uint64 context = 0);
	void финиш();
	
	static void очистьКэш();
	static void ResetCache();
	
	GLDraw()        { context = 0; }
	GLDraw(Размер sz) { иниц(sz); }

	~GLDraw();
};

void GLOrtho(float left, float right, float bottom, float top, float near_, float far_, GLuint u_projection);

};

#include "GLPainter.h"

#endif