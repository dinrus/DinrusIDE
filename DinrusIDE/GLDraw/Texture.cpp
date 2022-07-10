#include "GLDraw.h"

#define LTIMING(x) // RTIMING(x)

#define LLOG(x) // DLOG(x)

namespace РНЦП {
	
GLuint CreateGLTexture(const Рисунок& img, dword flags)
{
	Размер sz = img.дайРазм();
	GLuint texture_id;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sz.cx, sz.cy, 0, GL_BGRA, GL_UNSIGNED_BYTE, ~img);
	if(flags & TEXTURE_MIPMAP)
		glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, flags & TEXTURE_LINEAR ? GL_LINEAR : GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, flags & TEXTURE_LINEAR ?
																flags & TEXTURE_MIPMAP ? GL_LINEAR_MIPMAP_LINEAR
																                       : GL_LINEAR
																                 : GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	return texture_id;
}

struct ImageGLData {
	Рисунок        img;
	GLuint       texture_id;
	
	void иниц(const Рисунок& img, dword flags);
	~ImageGLData();
};

void ImageGLData::иниц(const Рисунок& img, dword flags)
{
	LTIMING("CreateTexture");

	texture_id = CreateGLTexture(img, flags);

	LLOG("Texture id created: " << texture_id);
	SysImageRealized(img);
}

static LRUCache<ImageGLData, Кортеж<uint64, uint64, dword> > sTextureCache;
static bool sReset;

ImageGLData::~ImageGLData()
{
	SysImageReleased(img);
	if(!sReset)
		glDeleteTextures(1, &texture_id);
	LLOG("Texture id deleted: " << texture_id);
}

void GLDraw::очистьКэш()
{
	sReset = false;
	sTextureCache.очисть();
}

void GLDraw::ResetCache()
{
	sReset = true;
	sTextureCache.очисть();
	sReset = false;
}

struct ImageGLDataMaker : LRUCache<ImageGLData, Кортеж<uint64, uint64, dword> >::Делец {
	Рисунок         img;
	uint64        context;
	dword         flags;

	virtual Кортеж<uint64, uint64, dword>  Ключ() const                      { return сделайКортеж(img.GetSerialId(), context, flags); }
	virtual int                           сделай(ImageGLData& object) const  { object.иниц(img, flags); return img.дайДлину(); }
};

int max_texture_memory = 4 * 1024 * 768;
int max_texture_count = 1000;

GLuint GetTextureForImage(dword flags, const Рисунок& img, uint64 context)
{
	sTextureCache.сожми(max_texture_memory, max_texture_count);
	ImageGLDataMaker m;
	m.img = img;
	m.context = context;
	m.flags = flags;
	return sTextureCache.дай(m).texture_id;
}

};