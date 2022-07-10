#ifndef _GLDrawDemo_Ugl_h_
#define _GLDrawDemo_Ugl_h_

namespace РНЦП {
	
#define GL_TIMING(x)

#ifdef _ОТЛАДКА
#define GLCHK(x) do { \
	x; \
	int err = glGetError(); \
	if(err) LOG("Ошибка " << err << " (" << __LINE__ << "): " << #x); \
	LOG((const char *)gluErrorString(err)); \
} while(0)
#endif

struct GLContext2D { // TODO: This should be changed to regular matrix (later)
	РазмерПЗ  vs;
	РазмерПЗ  off = РазмерПЗ(-1, 1);
	double alpha = 1;
	
	void уст(Размер sz)    { vs = РазмерПЗ(2.0 / sz.cx, -2.0 / sz.cy); }
	
	GLContext2D(Размер sz) { уст(sz); }
	GLContext2D()        {}
};

struct GLCode : GLProgram {
	int64 serialid;

	GLCode(const char *vertex_shader, const char *pixel_shader);

	int operator[](const char *id)                                             { return GetUniform(id); }
	
	GLCode& Uniform(int i, double a);
	GLCode& Uniform(int i, double a, double b);
	GLCode& Uniform(int i, double a, double b, double c);
	GLCode& Uniform(int i, double a, double b, double c, double d);

	GLCode& Uniform(const char *id, double a);
	GLCode& Uniform(const char *id, double a, double b);
	GLCode& Uniform(const char *id, double a, double b, double c);
	GLCode& Uniform(const char *id, double a, double b, double c, double d);

	GLCode& operator()(const char *id, double a)                               { return Uniform(id, a); }
	GLCode& operator()(const char *id, double a, double b)                     { return Uniform(id, a, b); }
	GLCode& operator()(const char *id, double a, double b, double c)           { return Uniform(id, a, b, c); }
	GLCode& operator()(const char *id, double a, double b, double c, double d) { return Uniform(id, a, b, c, d); }

	GLCode& operator()(int i, double a)                                        { return Uniform(i, a); }
	GLCode& operator()(int i, double a, double b)                              { return Uniform(i, a, b); }
	GLCode& operator()(int i, double a, double b, double c)                    { return Uniform(i, a, b, c); }
	GLCode& operator()(int i, double a, double b, double c, double d)          { return Uniform(i, a, b, c, d); }

	GLCode& operator()(const char *id, ТочкаПЗ p)                               { return Uniform(id, p.x, p.y); }
	GLCode& operator()(int i, ТочкаПЗ p)                                        { return Uniform(i, p.x, p.y); }
	GLCode& operator()(const char *id, РазмерПЗ sz)                               { return Uniform(id, sz.cx, sz.cy); }
	GLCode& operator()(int i, РазмерПЗ sz)                                        { return Uniform(i, sz.cx, sz.cy); }
	GLCode& operator()(const char *id, Цвет c, double alpha = 1);
	GLCode& operator()(int i, Цвет c, double alpha = 1);
	
	GLCode& Mat4(int i, float *mat4);
	GLCode& Mat4(const char *id, float *mat4);
};

class GLTexture {
	struct Данные {
		int     refcount = 1;
		GLuint  textureid = 0;
		Точка   hotspot = Точка(0, 0);
		Размер    sz = Размер(0, 0);
	};
	
	Данные    *data = NULL;
	
	void уст(GLuint texture, Размер sz, Точка hotspot = Точка(0, 0));
	
	friend class GLTextureDraw;
	
public:
	void     очисть();
	void     уст(const Рисунок& img, dword flags = TEXTURE_LINEAR|TEXTURE_MIPMAP);
	
	void     свяжи(int ii = 0) const;
	int      GetID() const      { return data ? data->textureid : 0; }
	operator GLuint() const     { return GetID(); }
	Размер     дайРазм() const    { return data ? data->sz : Размер(0, 0); }
	Точка    GetHotSpot() const { return data ? data->hotspot : Точка(0, 0); }

	GLTexture()                 {}
	GLTexture(const Рисунок& img, dword flags = TEXTURE_LINEAR|TEXTURE_MIPMAP) { уст(img, flags); }
	~GLTexture()                { очисть(); }

	GLTexture(const GLTexture& src);
	GLTexture& operator=(const GLTexture& src);
};

void GLBind(int ii, const Рисунок& img, dword style = TEXTURE_LINEAR|TEXTURE_MIPMAP);
void GLBind(const Рисунок& img, dword style = TEXTURE_LINEAR|TEXTURE_MIPMAP);

void GLDrawTexture(const GLContext2D& dd, const ПрямПЗ& rect, int textureid, Размер tsz, const Прям& src);
void GLDrawTexture(const GLContext2D& dd, const ПрямПЗ& rect, const GLTexture& img, const Прям& src);
void GLDrawImage(const GLContext2D& dd, const ПрямПЗ& rect, const Рисунок& img, const Прям& src);
void GLDrawTexture(const GLContext2D& dd, const ПрямПЗ& rect, const GLTexture& img);
void GLDrawImage(const GLContext2D& dd, const ПрямПЗ& rect, const Рисунок& img);

class GLVertexData {
	struct Данные {
		int            refcount = 1;
		GLuint         VAO = 0;
		GLuint         EBO = 0;
		int            elements = 0;
	    
		Вектор<GLuint> VBO;
	};
	
	Данные    *data = NULL;
	
	void     Do();

public:
	void    очисть();

	GLVertexData& добавь(const void *data, int тип, int ntuple, int count);
	GLVertexData& добавь(const float *data, int ntuple, int count) { return добавь(data, GL_FLOAT, ntuple, count); }
	GLVertexData& добавь(const byte *data, int ntuple, int count)  { return добавь(data, GL_UNSIGNED_BYTE, ntuple, count); }
	GLVertexData& добавь(const dword *data, int ntuple, int count) { return добавь(data, GL_UNSIGNED_INT, ntuple, count); }
	GLVertexData& добавь(const Вектор<float>& data, int ntuple)    { return добавь(data, ntuple, data.дайСчёт() / ntuple); }
	GLVertexData& добавь(const Вектор<byte>& data, int ntuple)     { return добавь(data, ntuple, data.дайСчёт() / ntuple); }
	GLVertexData& добавь(const Вектор<dword>& data, int ntuple)    { return добавь(data, ntuple, data.дайСчёт() / ntuple); }
	GLVertexData& добавь(const Вектор<ТочкаПЗ>& pt);
	GLVertexData& Индекс(const int *indices, int count);
	GLVertexData& Индекс(const Вектор<int>& indices)             { return Индекс(indices, indices.дайСчёт()); }
	
	void    Draw(int mode = GL_TRIANGLES) const;

	void    Draw(GLCode& shaders, int mode = GL_TRIANGLES) const;
	
	operator bool() const                                       { return data; }
	bool    пустой() const                                     { return !data; }

	GLVertexData()                                              {}
	~GLVertexData()                                             { очисть(); }

	GLVertexData(const GLVertexData& src);
	GLVertexData& operator=(const GLVertexData& src);
};

const GLVertexData& GLRectMesh();

template <typename Src>
void GLPolygons(GLVertexData& mesh, const Src& polygon);

void GLDrawPolygons(const GLContext2D& dd, ТочкаПЗ at, const GLVertexData& mesh, РазмерПЗ scale, Цвет color);
void GLDrawConvexPolygons(const GLContext2D& dd, ТочкаПЗ at, const GLVertexData& mesh, РазмерПЗ scale, Цвет color);

template <typename Src>
void GLPolylines(GLVertexData& data, const Src& polygon, bool close_loops = false);

void DashPolyline(Вектор<Вектор<ТочкаПЗ>>& polyline, const Вектор<ТочкаПЗ>& line,
                  const Вектор<double>& pattern, double distance = 0);

void GLDrawPolylines(const GLContext2D& dd, ТочкаПЗ at, const GLVertexData& mesh, РазмерПЗ scale, double width, Цвет color);

void GLDrawStencil(Цвет color, double alpha);

void GLDrawEllipse(const GLContext2D& dd, ТочкаПЗ center, РазмерПЗ radius, Цвет fill_color,
                   double width, Цвет line_color, const Вектор<double>& dash, double distance);
void GLDrawEllipse(const GLContext2D& dd, ТочкаПЗ center, РазмерПЗ radius, Цвет fill_color,
                   double width, Цвет line_color);

GLTexture GetGlyphGLTextureCached(double angle, int chr, Шрифт font, Цвет color);

void GLDrawText(const GLContext2D& dd, ТочкаПЗ pos, double angle, const wchar *text, Шрифт font,
                Цвет ink, int n = -1, const int *dx = NULL);

void GLArc(Вектор<Вектор<ТочкаПЗ>>& line, const ПрямПЗ& rc, ТочкаПЗ start, ТочкаПЗ end);

class GLTriangles {
	Вектор<float>  pos;
	Вектор<byte>   color;
	Вектор<GLint>  elements;
	int            ii = 0;

public:
	int  Vertex(float x, float y, byte r, byte g, byte b, double alpha) {
		pos << x << y << (float)alpha;
		color << r << g << b;
		return ii++;
	}

	int  Vertex(float x, float y, Цвет c, double alpha)  { return Vertex(x, y, c.дайК(), c.дайЗ(), c.дайС(), alpha); }
	int  Vertex(ТочкаПЗ p, Цвет c, double alpha)          { return Vertex((float)p.x, (float)p.y, c, alpha); }
	int  Vertex(int x, int y, Цвет c, double alpha)      { return Vertex((float)x, (float)y, c, alpha); }

	void Triangle(int a, int b, int c)                    { elements << a << b << c; }
	
	void очисть()                                          { elements.очисть(); pos.очисть(); color.очисть(); ii = 0; }

	void Draw(const GLContext2D& dd);
};

void Ellipse(GLTriangles& tr, ТочкаПЗ center, РазмерПЗ radius, Цвет color, double width, Цвет line_color, double alpha);
void Polyline(GLTriangles& tr, const Вектор<ТочкаПЗ>& p, double width, Цвет color, double alpha, bool close);

#include "GLPainter.hpp"

class DrawGL : public NilPainter, GLTriangles {
public:
	virtual dword GetInfo() const;

	virtual void BeginOp();
	virtual bool ClipOp(const Прям& r);
	virtual bool ClipoffOp(const Прям& r);
	virtual bool IntersectClipOp(const Прям& r);
	virtual void OffsetOp(Точка p);
	virtual bool ExcludeClipOp(const Прям& r);
	virtual void EndOp();
	virtual bool IsPaintingOp(const Прям& r) const;

	virtual void DrawRectOp(int x, int y, int cx, int cy, Цвет color);
	virtual void DrawImageOp(int x, int y, int cx, int cy, const Рисунок& image, const Прям& src, Цвет color);
	virtual void DrawTextOp(int x, int y, int angle, const wchar *text, Шрифт font, Цвет ink, int n, const int *dx);

	virtual void DrawArcOp(const Прям& rc, Точка start, Точка end, int width, Цвет color);
	virtual void DrawEllipseOp(const Прям& r, Цвет color, int pen, Цвет pencolor);
	virtual void DrawLineOp(int x1, int y1, int x2, int y2, int width, Цвет color);
	virtual void DrawPolyPolyPolygonOp(const Точка *vertices, int vertex_count, const int *subpolygon_counts, int scc, const int *disjunct_polygon_counts, int dpcc, Цвет color, int width, Цвет outline, uint64 pattern, Цвет doxor);
	virtual void DrawPolyPolylineOp(const Точка *vertices, int vertex_count, const int *counts, int count_count, int width, Цвет color, Цвет doxor);
	
	virtual void двигОп(const ТочкаПЗ& p, bool rel);
	virtual void линияОп(const ТочкаПЗ& p, bool rel);
	virtual void OpacityOp(double o);
	virtual void закройОп();
	virtual void StrokeOp(double width, const КЗСА& rgba);
	virtual void FillOp(const КЗСА& color);
	virtual void DashOp(const Вектор<double>& dash, double start);

private:
	struct Cloff : Движимое<Cloff> {
		Прям   clip;
		ТочкаПЗ offset;
	};
	
	struct State {
		double alpha;
		double dash_start;
		Вектор<double> dash;
	};
	
	Вектор<Cloff> cloff;
	Массив<State>  state;
	GLContext2D   dd;
	Размер          view_size;
	Прям          scissor;

	ТочкаПЗ prev;
	Вектор<Вектор<ТочкаПЗ>> path;
	bool   path_done;
	bool   close_path;
	double dash_start;
	Вектор<double> dash, path_dash;

	void   сунь();
	ТочкаПЗ Off(int x, int y);
	ТочкаПЗ Off(Точка p)                      { return Off(p.x, p.y); }
	ПрямПЗ  Off(int x, int y, int cx, int cy);
	ПрямПЗ  Off(int x, int y, Размер sz);
	void   SyncScissor();
	void   делайПуть(Вектор<Вектор<ТочкаПЗ>>& poly, const Точка *pp, const Точка *end);
	static const Вектор<double>& GetDash(int& width);
	void   ApplyDash(Вектор<Вектор<ТочкаПЗ>>& polyline, int& width);
	void   DoDrawPolylines(Вектор<Вектор<ТочкаПЗ>>& poly, int width, Цвет color, bool close = false);
	void   DoDrawPolygons(const Вектор<Вектор<ТочкаПЗ>>& path, Цвет color);
	
	friend class GLTextureDraw;

public:
	using Draw::Clip;

	void иниц(Размер sz, double alpha = 1);
	
	void слей();
	
	operator const GLContext2D&()           { слей(); return dd; }

	DrawGL() {}
	DrawGL(Размер sz, double alpha = 1)       { иниц(sz, alpha); }
	~DrawGL();
};

class GLTextureDraw : public DrawGL {
	GLuint framebuffer = 0;
    GLuint texture = 0;
	GLuint rbo = 0;
	Размер   sz;
	int    msaa = 0;

public:
	void      очисть();
	void      создай(Размер sz, int msaa = 0);
	GLTexture дайРез();
	operator  GLTexture()                { return дайРез(); }

	GLTextureDraw()                      {}
	GLTextureDraw(Размер sz, int msaa = 0) { создай(sz, msaa); }
	~GLTextureDraw()                     { очисть(); }
};

void GLClearCounters();
int  GLElementCounter();
int  GLTextureCounter();
int  GLProgramCounter();
int  GLDrawCounter();
int  GLTesselateCounter();

};

#endif
