#ifndef _ide_GuiCommon_GuiCommon_h_
#define _ide_GuiCommon_GuiCommon_h_

#include <DinrusIDE/Core/Core.h>
#include <CtrlLib/CtrlLib.h>
#include <CodeEditor/CodeEditor.h>

#define IMAGECLASS IdeCommonImg
#define IMAGEFILE  <DinrusIDE/Common/common.iml>
#include <Draw/iml_header.h>

struct Отладчик {
	virtual void DebugBar(Бар& bar) = 0;
	virtual bool SetBreakpoint(const Ткст& filename, int line, const Ткст& bp) = 0;
	virtual bool RunTo() = 0;
	virtual void пуск() = 0;
	virtual void стоп() = 0;
	virtual bool окончен() = 0;
	virtual bool Подсказка(const Ткст& exp, РедакторКода::MouseTip& mt);

	virtual ~Отладчик() {}
};

void      SourceFs(FileSel& fsel);
FileSel&  AnySourceFs();
FileSel&  AnyPackageFs();
FileSel&  BasedSourceFs();
FileSel&  OutputFs();

void      ShellOpenFolder(const Ткст& dir);

Рисунок     ImageOver(const Рисунок& back, const Рисунок& over);

Рисунок     IdeFileImage(const Ткст& filename, bool include_path, bool pch);

bool FinishSave(Ткст tmpfile, Ткст outfile);
void DeactivationSave(bool b);
bool IsDeactivationSave();

bool FinishSave(Ткст outfile);
bool SaveFileFinish(const Ткст& filename, const Ткст& data);
bool SaveChangedFileFinish(const Ткст& filename, const Ткст& data);

struct IdeDesigner  {
	virtual Ткст дайИмяф() const = 0;
	virtual void   сохрани() = 0;
	virtual void   SyncUsc()                                        {}
	virtual void   SaveEditPos()                                    {}
	virtual void   RestoreEditPos()                                 {}
	virtual void   EditMenu(Бар& menu) = 0;
	virtual int    дайНабсим() const                               { return -1; }
	virtual Ктрл&  DesignerCtrl() = 0;
	virtual void   устФокус()                                       { DesignerCtrl().устФокус(); }

	virtual ~IdeDesigner() {}
};

struct МодульИСР {
	virtual Ткст       GetID() = 0;
	virtual void         CleanUsc() {}
	virtual bool         ParseUsc(СиПарсер&)                                       { return false; }
	virtual Рисунок        FileIcon(const char *filename)                           { return Null; }
	virtual bool         AcceptsFile(const char *filename)                        { return !пусто_ли(FileIcon(filename)); }
	virtual IdeDesigner *CreateDesigner(Иср *ide, const char *path, byte charset) { return CreateDesigner(path, charset); }
	virtual IdeDesigner *CreateDesigner(const char *path, byte charset)           { return NULL; }
	virtual void         сериализуй(Поток& s) {}

	virtual ~МодульИСР() {}
};

void       регМодульИСР(МодульИСР& module);
int        дайСчётМодульИСР();
МодульИСР& дайМодульИСР(int q);

enum {
	DEBUG_NONE, DEBUG_MINIMAL, DEBUG_FULL
};

void             RegisterWorkspaceConfig(const char *имя);
void             RegisterWorkspaceConfig(const char *имя, Событие<>  WhenFlush);
Ткст&          WorkspaceConfigData(const char *имя);

template <class T>
bool LoadFromWorkspace(T& x, const char *имя)
{
	ТкстПоток ss(WorkspaceConfigData(имя));
	return ss.кф_ли() || грузи(x, ss);
}

template <class T>
void StoreToWorkspace(T& x, const char *имя)
{
	ТкстПоток ss;
	сохрани(x, ss);
	WorkspaceConfigData(имя) = ss;
}

void SerializeWorkspaceConfigs(Поток& s);

extern bool IdeExit;
extern bool IdeAgain; // Used to restart theide after checking out SVN (SetupSVNTrunk)

bool копируйПапку(const char *dst, const char *src, Progress *pi = NULL);

bool HasSvn();
bool HasGit();
#ifdef PLATFORM_WIN32
Ткст GetInternalGitPath();
#endif

int  MaxAscent(Шрифт f);

#endif
