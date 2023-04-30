#include "Core.h"

namespace РНЦПДинрус {

bool естьМпбУтф8(Поток& in)
{
	int64 pos = in.дайПоз();
	if(in.дай() == 0xef && in.дай() == 0xbb && in.дай() == 0xbf)
		return true;
	in.перейди(pos);
	return false;
}

static void sLoadBom(Поток& in, Ткст *t, ШТкст *wt, byte def_charset)
{
	if(in.открыт()) {
		Ткст s;
		if(in.GetLeft() > 3) {
			word header = in.Get16le();
			if(header == 0xfffe || header == 0xfeff) {
				int n = (int)in.GetLeft() / 2;
				ШТкстБуф ws(n);
				ws.устДлину(in.дай(~ws, 2 * n) / 2);
				if(header == 0xfffe)
					эндианРазворот((word *)~ws, ws.дайСчёт());
				if(wt)
					*wt = ws;
				else
					*t = изЮникода(ws);
				return;
			}
			int c = in.дай();
			if(c < 0)
				return;
			byte *h = (byte *)&header;
			if(h[0] == 0xef && h[1] == 0xbb && c == 0xbf) {
				if(wt)
					*wt = вУтф32(загрузиПоток(in));
				else
					*t = вНабсим(CHARSET_DEFAULT, загрузиПоток(in), НАБСИМ_УТФ8);
				return;
			}
			s.конкат(h, 2);
			s.конкат(c);
		}
		s.конкат(загрузиПоток(in));
		if(wt)
			*wt = вЮникод(s, def_charset);
		else
			*t = вНабсим(CHARSET_DEFAULT, s, def_charset);
		return;
	}
	return;
}

ШТкст загрузиМПБПотокаШ(Поток& in, byte def_charset)
{
	ШТкст s = ШТкст::дайПроц();
	sLoadBom(in, NULL, &s, def_charset);
	return s;
}

ШТкст загрузиМПБПотокаШ(Поток& in)
{
	return загрузиМПБПотокаШ(in, GetLNGCharset(GetSystemLNG()));
}

Ткст загрузиМПБПотока(Поток& in, byte def_charset)
{
	Ткст s = Ткст::дайПроц();
	sLoadBom(in, &s, NULL, def_charset);
	return s;
}

Ткст загрузиМПБПотока(Поток& in)
{
	return загрузиМПБПотока(in, GetLNGCharset(GetSystemLNG()));
}

ШТкст загрузиМПБФайлаШ(const char *path, byte def_charset)
{
	ФайлВвод in(path);
	return загрузиМПБПотокаШ(in, def_charset);
}

ШТкст загрузиМПБФайлаШ(const char *path)
{
	ФайлВвод in(path);
	return загрузиМПБПотокаШ(in);
}

Ткст загрузиМПБФайла(const char *path, byte def_charset)
{
	ФайлВвод in(path);
	return загрузиМПБПотока(in, def_charset);
}

Ткст загрузиМПБФайла(const char *path)
{
	ФайлВвод in(path);
	return загрузиМПБПотока(in);
}

bool сохраниМПБПотока(Поток& out, const ШТкст& данные) {
	if(!out.открыт() || out.ошибка_ли())
		return false;
	word w = 0xfeff;
	out.помести(&w, 2);
	out.помести(~данные, 2 * данные.дайДлину());
	out.закрой();
	return out.ок_ли();
}

bool сохраниМПБФайла(const char *path, const ШТкст& данные)
{
	ФайлВывод out(path);
	return сохраниМПБПотока(out, данные);
}

bool сохраниМПБПотокаУтф8(Поток& out, const Ткст& данные) {
	if(!out.открыт() || out.ошибка_ли())
		return false;
	static unsigned char bom[] = {0xEF, 0xBB, 0xBF};
	out.помести(bom, 3);
	out.помести(вНабсим(НАБСИМ_УТФ8, данные));
	out.закрой();
	return out.ок_ли();
}

bool сохраниМПБФайлаУтф8(const char *path, const Ткст& данные)
{
	ФайлВывод out(path);
	return сохраниМПБПотокаУтф8(out, данные);
}

}
