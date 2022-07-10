#include "GLDraw.h"

namespace РНЦП {
	
void GLTexture::очисть()
{
	if(data && --data->refcount == 0) {
		glDeleteTextures(1, &data->textureid);
		delete data;
	}
	data = NULL;
}

void GLTexture::уст(GLuint id, Размер sz, Точка hotspot)
{
	очисть();
	data = new Данные;
	data->sz = sz;
	data->hotspot = hotspot;
	data->textureid = id;
}

void GLTexture::уст(const Рисунок& img, dword flags)
{
	уст(CreateGLTexture(img, flags), img.дайРазм(), img.GetHotSpot());
}

extern int sTextureCounter;

void GLTexture::свяжи(int ii) const
{
	if(data) {
		glActiveTexture(GL_TEXTURE0 + ii);
		glBindTexture(GL_TEXTURE_2D, data->textureid);
		sTextureCounter++;
	}
}

GLTexture::GLTexture(const GLTexture& src)
{
	data = src.data;
	if(data)
		data->refcount++;
}

GLTexture& GLTexture::operator=(const GLTexture& src)
{
	очисть();
	data = src.data;
	if(data)
		data->refcount++;
	return *this;
}

void GLBind(int ii, const Рисунок& img, dword style)
{
	extern int sTextureCounter;
	glActiveTexture(GL_TEXTURE0 + ii);
	glBindTexture(GL_TEXTURE_2D, GetTextureForImage(style, img));
	sTextureCounter++;
}

void GLBind(const Рисунок& img, dword style)
{
	GLBind(0, img, style);
}

const GLVertexData& GLRectMesh()
{
	static GLVertexData mesh;
	ONCELOCK {
		static const float box[] = {
			0, 0, // 0
			0, 1, // 1
			1, 0, // 2
			1, 1, // 3
		};
		static const int ndx[] = {
			0, 1, 2, 1, 2, 3
		};
		mesh.добавь(box, 2, 4).Индекс(ndx, 6);
	}
	return mesh;
}

void GLDrawTexture(const GLContext2D& dd, const ПрямПЗ& rect, int textureid, Размер tsz, const Прям& src)
{
	static GLCode program(R"(
//		#version 330 core
		uniform vec2 offset;
		uniform vec2 scale;

		uniform vec2 toffset;
		uniform vec2 tscale;

	    attribute vec2 aPos;
		varying vec2 tPos;
	    void main()
	    {
			gl_Position = vec4(scale * aPos + offset, 0, 1);
			tPos = tscale * aPos + toffset;
	    }
	)", R"(
//		#version 330 core
		varying vec2 tPos;
		uniform float alpha;
		uniform sampler2D s_texture;
		void main()
		{
			vec4 v = texture2D(s_texture, tPos);
			gl_FragColor = alpha * clamp(v, vec4(0, 0, 0, 0), v.aaaa); // clamp fixes non-premultiplied textures
		}
	)");

	if(tsz.cx * tsz.cy == 0)
		return;
	
	static int ioffset = program["offset"];
	static int iscale = program["scale"];
	static int ialpha = program["alpha"];
	static int itscale = program["tscale"];
	static int itoffset = program["toffset"];

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	glBindTexture(GL_TEXTURE_2D, textureid);
	sTextureCounter++;
	GLRectMesh().Draw(
		program(ioffset, dd.vs * rect.верхЛево() + dd.off)
		       (iscale, dd.vs * rect.дайРазм())
		       (ialpha, dd.alpha)
		       (itoffset, РазмерПЗ((float)src.left / tsz.cx, (float)src.top / tsz.cy))
		       (itscale, РазмерПЗ((float)src.дайШирину() / tsz.cx, (float)src.дайВысоту() / tsz.cy))
	);
}

void GLDrawTexture(const GLContext2D& dd, const ПрямПЗ& rect, const GLTexture& img, const Прям& src)
{
	GLDrawTexture(dd, rect, img.GetID(), img.дайРазм(), src);
}

void GLDrawImage(const GLContext2D& dd, const ПрямПЗ& rect, const Рисунок& img, const Прям& src)
{
	GLDrawTexture(dd, rect, GetTextureForImage(img), img.дайРазм(), src);
}

void GLDrawTexture(const GLContext2D& dd, const ПрямПЗ& rect, const GLTexture& img)
{
	GLDrawTexture(dd, rect, img, img.дайРазм());
}

void GLDrawImage(const GLContext2D& dd, const ПрямПЗ& rect, const Рисунок& img)
{
	GLDrawTexture(dd, rect, img, img.дайРазм());
}

};
