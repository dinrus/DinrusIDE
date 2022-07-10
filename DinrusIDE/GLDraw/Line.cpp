#include "GLDraw.h"

namespace РНЦП {

void GLDrawPolylines(const GLContext2D& dd, ТочкаПЗ at, const GLVertexData& mesh, РазмерПЗ scale,
                    double width, Цвет color)
{
	if(пусто_ли(color) || width <= 0)
		return;

	static GLCode program(R"(
	    attribute vec4 pos;
		uniform vec2 offset;
		uniform vec2 scale;
		uniform vec2 scale2;
		uniform vec2 width;
	    void main()
	    {
			vec2 p = scale * scale2 * pos.xy + offset; // avoiding precision problems
			vec2 v = scale2 * pos.zw;
			gl_Position = vec4(p + scale * width * normalize(vec2(-v.y, v.x)), 0, 1);
	    }
	)", R"(
		uniform vec4 color;
		void main()
		{
			gl_FragColor = color;
		}
	)");

	static int ioffset = program["offset"];
	static int iscale = program["scale"];
	static int iscale2 = program["scale2"];
	static int iwidth = program["width"];
	static int icolor = program["color"];

	program(ioffset, ТочкаПЗ(dd.vs) * at + dd.off)
	       (iscale, dd.vs)
	       (iscale2, scale)
	       (iwidth, РазмерПЗ(width / 2, width / 2))
	       (icolor, color, dd.alpha)
	;
	
	if(dd.alpha == 1) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		mesh.Draw(program);
	}
	else {
		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);
	
		glEnable(GL_STENCIL_TEST);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glStencilFunc(GL_NEVER, 1, 1);
		glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
		
		mesh.Draw(program);
		
		GLDrawStencil(color, dd.alpha);
	}
}

void DashPolyline(Вектор<Вектор<ТочкаПЗ>>& polyline, const Вектор<ТочкаПЗ>& line,
                  const Вектор<double>& pattern, double distance)
{
	struct LineStore : LinearPathConsumer {
		Вектор<Вектор<ТочкаПЗ>>& polyline;
	
		void Move(const ТочкаПЗ& p) override { polyline.добавь().добавь(p); }
		void Строка(const ТочкаПЗ& p) override { polyline.верх().добавь(p); }
		
		LineStore(Вектор<Вектор<ТочкаПЗ>>& polyline) : polyline(polyline) {}
	};
	
	LineStore st(polyline);
	Dasher dasher;
	dasher.target = &st;
	dasher.иниц(pattern, distance);
	for(int i = 0; i < line.дайСчёт(); i++)
		if(i)
			dasher.Строка(line[i]);
		else
			dasher.Move(line[i]);
	dasher.стоп();
}

};