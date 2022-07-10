#include "GLDraw.h"

namespace РНЦП {

GLCode::GLCode(const char *vertex_shader, const char *pixel_shader)
{
	Compile(vertex_shader, pixel_shader);
	Вектор<Tuple2<int, const char *>> ins;
	СиПарсер p(vertex_shader);
	auto readID = [&](int& n) {
		Ткст id;
		n = 0;
		while(!p.кф_ли() && !p.сим_ли(';'))
			if(p.ид_ли()) {
				id = p.читайИд();
				if(p.сим('[') && p.цел_ли()) {
					n = p.читайЦел();
					p.сим(']');
				}
			}
			else {
				p.пропустиТерм();
				n = 0;
			}
		return id;
	};
	int ii = 0;
	while(!p.кф_ли() && !p.сим('{'))
		if(p.ид("attribute") || p.ид("in")) {
			int n;
			Ткст id = readID(n);
			if(id.дайСчёт())
				glBindAttribLocation(program, ii++, id);
		}
		else
			p.пропустиТерм();
	
	Link();
	Use();

	p.уст(pixel_shader);
	int ti = 0;
	while(!p.кф_ли() && !p.сим('{'))
		if(p.ид("sampler2D") || p.ид("sampler3D")) {
			int n;
			Ткст id = readID(n);
			if(id.дайСчёт()) {
				if(n) {
					for(int i = 0; i < n; i++) {
						ПРОВЕРЬ(GetUniform(id + '[' + какТкст(i) + ']') >= 0);
						glUniform1i(GetUniform(id + '[' + какТкст(i) + ']'), ti++);
					}
				}
				else
					glUniform1i(GetUniform(id), ti++);
			}
		}
		else
			p.пропустиТерм();
}

GLCode& GLCode::Uniform(int i, double a)
{
	Use();
	glUniform1f(i, (float)a);
	return *this;
}

GLCode& GLCode::Uniform(int i, double a, double b)
{
	Use();
	glUniform2f(i, (float)a, (float)b);
	return *this;
}

GLCode& GLCode::Uniform(int i, double a, double b, double c)
{
	Use();
	glUniform3f(i, (float)a, (float)b, (float)c);
	return *this;
}

GLCode& GLCode::Uniform(int i, double a, double b, double c, double d)
{
	Use();
	glUniform4f(i, (float)a, (float)b, (float)c, (float)d);
	return *this;
}

GLCode& GLCode::Uniform(const char *id, double a)
{
	Use();
	glUniform1f(GetUniform(id), (float)a);
	return *this;
}

GLCode& GLCode::Uniform(const char *id, double a, double b)
{
	Use();
	glUniform2f(GetUniform(id), (float)a, (float)b);
	return *this;
}

GLCode& GLCode::Uniform(const char *id, double a, double b, double c)
{
	Use();
	glUniform3f(GetUniform(id), (float)a, (float)b, (float)c);
	return *this;
}

GLCode& GLCode::Uniform(const char *id, double a, double b, double c, double d)
{
	Use();
	glUniform4f(GetUniform(id), (float)a, (float)b, (float)c, (float)d);
	return *this;
}

GLCode& GLCode::operator()(const char *id, Цвет c, double alpha)
{
	return Uniform(GetUniform(id), c, alpha);
}

GLCode& GLCode::operator()(int i, Цвет c, double alpha)
{
	return Uniform(i, c.дайК() / 255.0f, c.дайЗ() / 255.0f, c.дайС() / 255.0f, alpha);
}

GLCode& GLCode::Mat4(int i, float *mat4)
{
	glUniformMatrix4fv(i, 1, GL_FALSE, mat4);
	return *this;
}

GLCode& GLCode::Mat4(const char *id, float *mat4)
{
	return Mat4(GetUniform(id), mat4);
}

};
