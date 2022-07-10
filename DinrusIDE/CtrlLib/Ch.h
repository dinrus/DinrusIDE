void ChBaseSkin();

void ChHostSkin();
void ChClassicSkin();
void ChStdSkin();
void ChDarkSkin();
void ChGraySkin();
void ChFlatSkin();
void ChFlatDarkSkin();
void ChFlatGraySkin();

Вектор<Кортеж<void (*)(), Ткст>> GetAllChSkins();

enum {
	CORNER_TOP_LEFT = 0x01,
	CORNER_TOP_RIGHT = 0x02,
	CORNER_BOTTOM_LEFT = 0x04,
	CORNER_BOTTOM_RIGHT = 0x08,
};

void  RoundedRect(Рисовало& w, double x, double y, double cx, double cy, double rx, double ry, dword corners);
void  RoundedRect(Рисовало& w, ПрямПЗ r, double rx, double ry, dword corner);
Рисунок MakeElement(Размер sz, double radius, const Рисунок& face, double border_width, Цвет border_color, Событие<Рисовало&, const ПрямПЗ&> shape);
Рисунок сделайКнопку(int radius, const Рисунок& face, double border_width, Цвет border_color = Null, dword corner = 0xff);
Рисунок сделайКнопку(int radius, Цвет face, double border_width, Цвет border_color = Null, dword corner = 0xff);

Рисунок Hot3(const Рисунок& m); // Adds resizing hotspots at 1/3
Рисунок ChHot(const Рисунок& m, int n = 2); // Adds resizing hotspots DPI(n)
Цвет AvgColor(const Рисунок& m, const Прям& rr);
Цвет AvgColor(const Рисунок& m, int margin = 0);

Цвет GetInk(const Рисунок& m); // the color that is most different from AvgColor
int   GetRoundness(const Рисунок& m);

Рисунок WithRect(Рисунок m, int x, int y, int cx, int cy, Цвет c);
Рисунок WithLeftLine(const Рисунок& m, Цвет c, int w = DPI(1));
Рисунок WithRightLine(const Рисунок& m, Цвет c, int w = DPI(1));
Рисунок WithTopLine(const Рисунок& m, Цвет c, int w = DPI(1));
Рисунок WithBottomLine(const Рисунок& m, Цвет c, int w = DPI(1));

Цвет FaceColor(int adj);

// Creates synthetic style based on SColors and basic button face
void ChSynthetic(Рисунок *button100x100, Цвет *text, bool macos = false);

// for diagnostics purposes
#ifdef _ОТЛАДКА
void  SetChameleonSample(const Значение& m, bool once = true);
Значение GetChameleonSample();
#endif
