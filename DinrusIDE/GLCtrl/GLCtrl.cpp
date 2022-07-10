#include "ГЛКтрл.h"

namespace РНЦП {

int  ГЛКтрл::depthSize = 24;
int  ГЛКтрл::stencilSize = 8;
bool ГЛКтрл::doubleBuffering = true;
int  ГЛКтрл::numberOfSamples = 1;
Размер ГЛКтрл::current_viewport;

extern void (*restore_gl_viewport__)();

void ГЛКтрл::DoGLPaint()
{
	MemoryIgnoreLeaksBlock __;

	glClearDepth(1);
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
	glEnable(GL_MULTISAMPLE);
	Размер sz = дайРазм();
	current_viewport = sz;
	SetCurrentViewport();
	GLPaint();
}

void ГЛКтрл::иниц()
{
	Transparent();
#ifdef PLATFORM_WIN32
	pane.ctrl = this;
	добавь(pane.SizePos());
#endif
	restore_gl_viewport__ = SetCurrentViewport;
	MemoryIgnoreNonMainLeaks();
	MemoryIgnoreNonUppThreadsLeaks(); // Linux drivers leak memory in threads
}

Рисунок ГЛКтрл::MouseEvent(int event, Точка p, int zdelta, dword keyflags)
{
	if(mouseTarget) {
		return mouseTarget->MouseEvent(event, p + GetScreenView().верхЛево() - mouseTarget->GetScreenView().верхЛево(), zdelta, keyflags);
	}
	return Ктрл::MouseEvent(event, p, zdelta, keyflags);
}

void ГЛКтрл::SetCurrentViewport()
{
	glViewport(0, 0, (GLsizei)current_viewport.cx, (GLsizei)current_viewport.cy);
}

void ГЛКтрл::StdView()
{
	MemoryIgnoreLeaksBlock __;

	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	Размер sz = дайРазм();
	glViewport(0, 0, (GLsizei)sz.cx, (GLsizei)sz.cy);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)(sz.cx)/(GLfloat)(sz.cy), 1.0f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

#ifdef PLATFORM_WIN32

Рисунок ГЛКтрл::GLPane::MouseEvent(int event, Точка p, int zdelta, dword keyflags)
{
	p = p - GetScreenView().верхЛево() + ctrl->GetScreenView().верхЛево();
	return ctrl->MouseEvent(event, p, zdelta, keyflags);
}

void ГЛКтрл::GLPane::Pen(Точка p, const ИнфОПере& pen, dword keyflags)
{
	(ctrl->mouseTarget ? ctrl->mouseTarget : ctrl)->Pen(p, pen, keyflags);
}

#endif


}
