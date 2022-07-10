#include "GLDraw.h"

namespace РНЦП {

GLVertexData::GLVertexData(const GLVertexData& src)
{
	if(src.data) {
		data = src.data;
		data->refcount++;
	}
}

GLVertexData& GLVertexData::operator=(const GLVertexData& src)
{
	if(data != src.data) {
		if(data) очисть();
		data = src.data;
		data->refcount++;
	}
	return *this;
}

void GLVertexData::очисть()
{
	if(data && --data->refcount == 0) {
	    glDeleteVertexArrays(1, &data->VAO);
	    glDeleteBuffers(1, &data->EBO);
	    for(auto h : data->VBO)
	        glDeleteBuffers(1, &h);
		delete data;
	}
	data = NULL;
}

void GLVertexData::Do()
{
	if(!data) {
		data = new Данные;
	    glGenVertexArrays(1, &data->VAO);
	    glGenBuffers(1, &data->EBO);
	}
	ПРОВЕРЬ(data->refcount == 1); // Changes are only allowed before copied
}

GLVertexData& GLVertexData::добавь(const void *values, int тип, int ntuple, int count)
{
	Do();
    glBindVertexArray(data->VAO);
    int ii = data->VBO.дайСчёт();
	GLuint& vbo = data->VBO.добавь();
	glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    int sz = (int)decode(тип, GL_FLOAT, sizeof(float),
	                           GL_BYTE, sizeof(byte),
	                           GL_UNSIGNED_BYTE, sizeof(byte),
	                           GL_SHORT, sizeof(int16),
	                           GL_UNSIGNED_SHORT, sizeof(uint16),
	                           GL_INT, sizeof(int32),
	                           GL_UNSIGNED_INT, sizeof(uint32),
	                           sizeof(double));
	glBufferData(GL_ARRAY_BUFFER, sz * ntuple * count, values, GL_STATIC_DRAW);
	if(тип == GL_FLOAT)
		glVertexAttribPointer(ii, ntuple, тип, GL_FALSE, ntuple * sz, (void*)0);
	else
		glVertexAttribIPointer(ii, ntuple, тип, ntuple * sz, (void*)0);
    glEnableVertexAttribArray(ii);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return *this;
}

GLVertexData& GLVertexData::Индекс(const int *indices, int count)
{
	Do();
	glBindVertexArray(data->VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * count, indices, GL_STATIC_DRAW);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	data->elements = count;
	return *this;
}

extern int sDrawCounter;
extern int sElementCounter;

void GLVertexData::Draw(int mode) const
{
	if(data) {
		GL_TIMING("GLVertexData::Draw");
		glBindVertexArray(data->VAO);
		glDrawElements(mode, data->elements, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		sDrawCounter++;
		sElementCounter += data->elements;
	}
}

void GLVertexData::Draw(GLCode& shaders, int mode) const
{
	shaders.Use();
	Draw(mode);
}

GLVertexData& GLVertexData::добавь(const Вектор<ТочкаПЗ>& pt)
{
	Буфер<float> f(2 * pt.дайСчёт());
	float *t = f;
	for(const ТочкаПЗ& p : pt) {
		*t++ = (float)p.x;
		*t++ = (float)p.y;
	}
	return добавь(f, GL_FLOAT, 2, pt.дайСчёт());
}

};