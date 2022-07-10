class TopFrameDraw : public SystemDraw {
public:
	TopFrameDraw(Ктрл *ctrl, const Прям& r);
	~TopFrameDraw();
};

Вектор<ШТкст>& coreCmdLine__();
Вектор<ШТкст> SplitCmdLine__(const char *cmd);

void CocoInit(int argc, const char **argv, const char **envptr);
void CocoExit();

#ifdef PLATFORM_POSIX
#define ГЛАВНАЯ_ГИП_ПРИЛ \
void главФнГип_(); \
\
int main(int argc, const char **argv, const char **envptr) { \
	РНЦП::иницПрил__(argc, (const char **)argv, envptr); \
	GUI_APP_MAIN_HOOK \
	РНЦП::CocoInit(argc, argv, envptr); \
	главФнГип_(); \
	РНЦП::Ктрл::закройТопКтрлы(); \
	РНЦП::CocoExit(); \
	return РНЦП::дайКодВыхода(); \
} \
 \
void главФнГип_()

#endif

class DHCtrl : Ктрл {};

enum {
	SF_NSDocumentDirectory,
	SF_NSUserDirectory,
	SF_NSDesktopDirectory,
	SF_NSDownloadsDirectory,
	SF_NSMoviesDirectory,
	SF_NSMusicDirectory,
	SF_NSPicturesDirectory,
};

Ткст GetSpecialDirectory(int i);

void   CocoBeep();