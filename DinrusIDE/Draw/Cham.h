enum LookOp {
	LOOK_PAINT,
	LOOK_MARGINS,
	LOOK_PAINTEDGE,
};

enum {
	CH_SCROLLBAR_IMAGE = -1000, // special Рисунок hotspot x значение for ChPaint of scrollbar
	CH_EDITFIELD_IMAGE = -1001, // special Рисунок hotspot x2 значение, y2 is then x and y for the purposes of painting
};

void  ChLookFn(Значение (*фн)(Draw& w, const Прям& r, const Значение& look, int lookop, Цвет ink));

Рисунок AdjustColors(const Рисунок& img);

void   Override(Iml& target, Iml& source, bool colored = false);
void   ColoredOverride(Iml& target, Iml& source);

void   ChReset();
void   ChFinish();

void   ChPaint(Draw& w, const Прям& r, const Значение& look, Цвет ink = Null);
void   ChPaint(Draw& w, int x, int y, int cx, int cy, const Значение& look, Цвет ink = Null);
void   ChPaintEdge(Draw& w, const Прям& r, const Значение& look, Цвет ink = Null);
void   ChPaintEdge(Draw& w, int x, int y, int cx, int cy, const Значение& look, Цвет ink = Null);
void   ChPaintBody(Draw& w, const Прям& r, const Значение& look, Цвет ink = Null);
void   ChPaintBody(Draw& w, int x, int y, int cx, int cy, const Значение& look, Цвет ink = Null);
Прям   ChMargins(const Значение& look);

void   DeflateMargins(Прям& r, const Прям& margin);
void   ChDeflateMargins(Прям& r, const Значение& look);
void   DeflateMargins(Размер& sz, const Прям& m);
void   ChDeflateMargins(Размер& sz, const Значение& look);
void   InflateMargins(Прям& r, const Прям& m);
void   ChInflateMargins(Прям& r, const Значение& look);
void   InflateMargins(Размер& sz, const Прям& m);
void   ChInflateMargins(Размер& sz, const Значение& look);

void   ChInvalidate();
bool   ChIsInvalidated();
bool   IsLabelTextColorMismatch();
bool   IsDarkColorFace();

template <class T>
struct ChStyle {
	byte status;
	byte registered;
	T   *standard;

	const T& Standard() const      { return *standard; }
	T&       пиши() const         { T& x = *(T *)this; x.status = 2; ChInvalidate(); return x; }
	void     присвой(const T& src)  { *(T *)this = src; }

	ChStyle()                      { status = 0; registered = 0; standard = NULL; }
};

#define CH_STYLE(klass, тип, style) \
struct COMBINE5(klass, __, тип, __, style) : klass::тип { \
	void иниц(); \
	static void InitIt(); \
}; \
\
void COMBINE5(klass, __, тип, __, style)::InitIt() { \
	klass::style(); \
} \
\
const klass::тип& klass::style() \
{ \
	static COMBINE5(klass, __, тип, __, style) b, standard; \
	if(b.status == 0) { \
		ChRegisterStyle__(b.status, b.registered, COMBINE5(klass, __, тип, __, style)::InitIt); \
		b.иниц(); \
		b.status = 1; \
		standard = b; \
		standard.standard = b.standard = &standard; \
	} \
	return b; \
} \
\
void COMBINE5(klass, __, тип, __, style)::иниц()


// CH_VAR0 allows inserting action into _Write (missing ending '}')
#define CH_VAR0(chtype, тип, имя, init) \
chtype& КОМБИНИРУЙ(ch_var__, имя)(); \
void КОМБИНИРУЙ(ch_init__, имя)() { \
	КОМБИНИРУЙ(ch_var__, имя)(); \
} \
\
chtype& КОМБИНИРУЙ(ch_var__, имя)() { \
	static chtype b; \
	if(b.status == 0) { \
		ChRegisterStyle__(b.status, b.registered, КОМБИНИРУЙ(ch_init__, имя)); \
		b.значение = init; \
		b.status = 1; \
	} \
	return b; \
} \
\
тип имя() { return КОМБИНИРУЙ(ch_var__, имя)().значение; } \
void КОМБИНИРУЙ(имя, _Write)(тип v) { КОМБИНИРУЙ(ch_var__, имя)().пиши().значение = v;

#define CH_VAR(chtype, тип, имя, init) CH_VAR0(chtype, тип, имя, init) }

struct ChColor : ChStyle<ChColor> { Цвет значение; };
#define CH_COLOR(имя, init) CH_VAR(ChColor, Цвет, имя, init)

struct ChInt : ChStyle<ChInt> { int значение; };
#define CH_INT(имя, init) CH_VAR(ChInt, int, имя, init)

struct ChValue : ChStyle<ChValue> { Значение значение; };
#define CH_VALUE(имя, init) CH_VAR(ChValue, Значение, имя, init)

struct ChImage : ChStyle<ChImage> { Рисунок значение; };
#define CH_IMAGE(имя, init) CH_VAR(ChImage, Рисунок, имя, init)

Значение ChLookWith(const Значение& look, const Рисунок& img, Точка offset = Точка(0, 0));
Значение ChLookWith(const Значение& look, const Рисунок& img, Цвет color, Точка offset = Точка(0, 0));
Значение ChLookWith(const Значение& look, const Рисунок& img, Цвет (*color)(int i), int i, Точка offset = Точка(0, 0));

void  ChLookWith(Значение *look, const Рисунок& image, const Цвет *color, int n = 4);

//private:
void ChRegisterStyle__(byte& state, byte& registered, void (*init)());

Значение ChBorder(const ColorF *colors, const Значение& face = SColorFace());
