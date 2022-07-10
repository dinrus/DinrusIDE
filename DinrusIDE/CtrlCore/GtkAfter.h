class TopFrameDraw : public SystemDraw {
#if GTK_CHECK_VERSION(3, 22, 0)
	GdkDrawingContext *ctx;
#endif
public:
	TopFrameDraw(Ктрл *ctrl, const Прям& r);
	~TopFrameDraw();
};

class DHCtrl : Ктрл {};

void InitGtkApp(int argc, char **argv, const char **envptr);
void ExitGtkApp();

#define ГЛАВНАЯ_ГИП_ПРИЛ \
void главФнГип_(); \
\
int main(int argc, char **argv, const char **envptr) { \
	РНЦП::иницПрил__(argc, (const char **)argv, envptr); \
	GUI_APP_MAIN_HOOK \
	РНЦП::InitGtkApp(argc, argv, envptr); \
	РНЦП::выполниПрил__(главФнГип_); \
	РНЦП::Ктрл::закройТопКтрлы(); \
	РНЦП::ExitGtkApp(); \
	РНЦП::выходПрил__(); \
	return РНЦП::дайКодВыхода(); \
} \
 \
void главФнГип_()
