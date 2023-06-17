#include <DinrusPro/DinrusCore.h>

бул естьМпбУтф8(Поток& in)
{
	дол pos = in.дайПоз();
	if(in.дай() == 0xef && in.дай() == 0xbb && in.дай() == 0xbf)
		return true;
	in.перейди(pos);
	return false;
}

static проц sLoadBom(Поток& in, Ткст *t, ШТкст *wt, ббайт def_charset)
{
	if(in.открыт()) {
		Ткст s;
		if(in.GetLeft() > 3) {
			бкрат header = in.Get16le();
			if(header == 0xfffe || header == 0xfeff) {
				цел n = (цел)in.GetLeft() / 2;
				ШТкстБуф ws(n);
				ws.устДлину(in.дай(~ws, 2 * n) / 2);
				if(header == 0xfffe)
					эндианРазворот((бкрат *)~ws, ws.дайСчёт());
				if(wt)
					*wt = ws;
				else
					*t = изЮникода(ws);
				return;
			}
			цел c = in.дай();
			if(c < 0)
				return;
			ббайт *h = (ббайт *)&header;
			if(h[0] == 0xef && h[1] == 0xbb && c == 0xbf) {
				if(wt)
					*wt = вУтф32(загрузиПоток(in));
				else
					*t = вНабсим(ДЕФНАБСИМ, загрузиПоток(in), НАБСИМ_УТФ8);
				return;
			}
			s.кат(h, 2);
			s.кат(c);
		}
		s.кат(загрузиПоток(in));
		if(wt)
			*wt = вЮникод(s, def_charset);
		else
			*t = вНабсим(ДЕФНАБСИМ, s, def_charset);
		return;
	}
	return;
}

ШТкст загрузиМПБПотокаШ(Поток& in, ббайт def_charset)
{
	ШТкст s = ШТкст::дайПроц();
	sLoadBom(in, NULL, &s, def_charset);
	return s;
}

ШТкст загрузиМПБПотокаШ(Поток& in)
{
	return загрузиМПБПотокаШ(in, GetLNGCharset(дайСисЯЗ()));
}

Ткст загрузиМПБПотока(Поток& in, ббайт def_charset)
{
	Ткст s = Ткст::дайПроц();
	sLoadBom(in, &s, NULL, def_charset);
	return s;
}

Ткст загрузиМПБПотока(Поток& in)
{
	return загрузиМПБПотока(in, GetLNGCharset(дайСисЯЗ()));
}

ШТкст загрузиМПБФайлаШ(кткст0 path, ббайт def_charset)
{
	ФайлВвод in(path);
	return загрузиМПБПотокаШ(in, def_charset);
}

ШТкст загрузиМПБФайлаШ(кткст0 path)
{
	ФайлВвод in(path);
	return загрузиМПБПотокаШ(in);
}

Ткст загрузиМПБФайла(кткст0 path, ббайт def_charset)
{
	ФайлВвод in(path);
	return загрузиМПБПотока(in, def_charset);
}

Ткст загрузиМПБФайла(кткст0 path)
{
	ФайлВвод in(path);
	return загрузиМПБПотока(in);
}

бул сохраниМПБПотока(Поток& out, const ШТкст& данные) {
	if(!out.открыт() || out.ошибка_ли())
		return false;
	бкрат w = 0xfeff;
	out.помести(&w, 2);
	out.помести(~данные, 2 * данные.дайДлину());
	out.закрой();
	return out.ок_ли();
}

бул сохраниМПБФайла(кткст0 path, const ШТкст& данные)
{
	ФайлВывод out(path);
	return сохраниМПБПотока(out, данные);
}

бул сохраниМПБПотокаУтф8(Поток& out, const Ткст& данные) {
	if(!out.открыт() || out.ошибка_ли())
		return false;
	static ббайт bom[] = {0xEF, 0xBB, 0xBF};
	out.помести(bom, 3);
	out.помести(вНабсим(НАБСИМ_УТФ8, данные));
	out.закрой();
	return out.ок_ли();
}

бул сохраниМПБФайлаУтф8(кткст0 path, const Ткст& данные)
{
	ФайлВывод out(path);
	return сохраниМПБПотокаУтф8(out, данные);
}
