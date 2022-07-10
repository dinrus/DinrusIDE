#include "LayDes.h"

struct EnumProperty : public EditorProperty<СписокБроса> {
	virtual void     устДанные(const Значение& v);
	virtual Ткст   сохрани() const              { return ~editor; }
	virtual void     читай(СиПарсер& p)          { устДанные(ReadPropertyParam(p)); }

	EnumProperty(ВекторМап<Ткст, Ткст>& e) {
		добавь(editor.HSizePosZ(100, 2).TopPos(2));
		for(int i = 0; i < e.дайСчёт(); i++)
			editor.добавь(e.дайКлюч(i), e[i]);
		устДанные(defval = e.дайКлюч(0));
	}
};

void  EnumProperty::устДанные(const Значение& v)
{
	if(!editor.HasKey(v))
		editor.добавь(v, v);
	editor <<= v;
}

bool ParseTemplate(Ткст& тип, Ткст& temp)
{
	int q = тип.найди('<');
	if(q < 0)
		return false;
	temp = тип.середина(q + 1);
	тип.обрежь(q);
	q = temp.найди('>');
	if(q >= 0)
		temp.обрежь(q);
	разверни(temp, тип);
	return true;
}

void LayoutItem::создай(const Ткст& _type)
{
	инивалидируй();
	property.очисть();
	тип = _type;
	pos.x = Ктрл::позЛев(0, 10);
	pos.y = Ктрл::позВерх(0, 10);
	Ткст tp = тип;
	Ткст tm;
	if(ParseTemplate(tp, tm))
		CreateProperties(tm, 0);
	CreateProperties(tp, 1);
}

void LayoutItem::CreateProperties(const Ткст& classname, int level)
{
	int q = LayoutTypes().найди(classname);
	if(q < 0)
		q = LayoutTypes().найди("Unknown");
	if(q < 0)
		return;
	const LayoutType& c = LayoutTypes()[q];
	for(int i = 0; i < c.property.дайСчёт(); i++) {
		const TypeProperty& r = c.property[i];
		if(пусто_ли(r.тип))
			CreateProperties(r.имя, level + 1);
		else {
			ItemProperty *n = ItemProperty::создай(r.тип);
			if(!n) {
				q = LayoutEnums().найди(r.тип);
				if(q >= 0)
					n = new EnumProperty(LayoutEnums()[q]);
			}
			if(!n)
				n = new RawProperty;
			n->устНабсим(charset);
			int q = -1;
			for(int i = 0; i < property.дайСчёт(); i++)
				if(r.имя == property[i].имя)
					q = i;
			int l = пусто_ли(r.level) ? q >= 0 ? property[q].level : level : level + r.level;
			ItemProperty& ip = q >= 0 ? property.уст(q, n) : property.добавь(n);
			ip.level = l;
			ip.имя = r.имя;
			if(!пусто_ли(r.defval))
				try {
					СиПарсер p(r.defval);
					ip.читай(p);
					ip.defval = ~ip;
				}
				catch(СиПарсер::Ошибка e) {
					вКонсоль(e + "\n");
				}
			else
				ip.defval = ~ip;
			ip.help = r.help;
		}
	}
}

void LayoutItem::устНабсим(byte cs)
{
	charset = cs;
	for(int i = 0; i < property.дайСчёт(); i++)
		property[i].устНабсим(charset);
}

void LayoutItem::ReadProperties(СиПарсер& p, bool addunknown)
{
	do {
		if(p.ид("LeftPosZ") || p.ид("LeftPos")) {
			Точка pt = ReadPoint(p);
			pos.x = Ктрл::позЛев(pt.x, pt.y);
		}
		else
		if(p.ид("RightPosZ") || p.ид("RightPos")) {
			Точка pt = ReadPoint(p);
			pos.x = Ктрл::позПрав(pt.x, pt.y);
		}
		else
		if(p.ид("HSizePosZ") || p.ид("HSizePos")) {
			Точка pt = ReadPoint(p);
			pos.x = Ктрл::позРазмер(pt.x, pt.y);
		}
		else
		if(p.ид("HCenterPosZ") || p.ид("HCenterPos")) {
			Точка pt = ReadPoint(p);
			pos.x = Ктрл::позЦентр(pt.x, pt.y);
		}
		else
		if(p.ид("TopPosZ") || p.ид("TopPos")) {
			Точка pt = ReadPoint(p);
			pos.y = Ктрл::позЛев(pt.x, pt.y);
		}
		else
		if(p.ид("BottomPosZ") || p.ид("BottomPos")) {
			Точка pt = ReadPoint(p);
			pos.y = Ктрл::позПрав(pt.x, pt.y);
		}
		else
		if(p.ид("VSizePosZ") || p.ид("VSizePos")) {
			Точка pt = ReadPoint(p);
			pos.y = Ктрл::позРазмер(pt.x, pt.y);
		}
		else
		if(p.ид("VCenterPosZ") || p.ид("VCenterPos")) {
			Точка pt = ReadPoint(p);
			pos.y = Ктрл::позЦентр(pt.x, pt.y);
		}
		else {
			Ткст имя = p.читайИд();
			int q = FindProperty(имя);
			if(q < 0) {
				if(addunknown) {
					q = property.дайСчёт();
					ItemProperty& new_prop = property.добавь(new RawProperty);
					new_prop.устНабсим(charset);
					new_prop.имя = имя;
				}
				else {
					p.передайСим('(');
					ReadPropertyParam(p);
					p.передайСим(')');
				}
			}
			if(q >= 0) {
				ItemProperty& ip = property[q];
				ip.устНабсим(charset);
				p.передайСим('(');
				ip.читай(p);
				p.передайСим(')');
			}
		}
	}
	while(p.сим('.'));
}

int  LayoutItem::FindProperty(const Ткст& s) const
{
	for(int i = 0; i < property.дайСчёт(); i++)
		if(property[i].имя == s)
			return i;
	return -1;
}

Ткст LayoutItem::SaveProperties(int y) const
{
	Ткст out;
	Вектор<int> o = GetStableSortOrder(property, [](const ItemProperty& a, const ItemProperty& b) { return a.level < b.level; });
	for(int i = 0; i < o.дайСчёт(); i++) {
		const ItemProperty& ip = property[o[i]];
		if(ip.дайДанные() != ip.defval)
			out << '.' << ip.имя << '(' << ip.сохрани() << ')';
	}
	switch(pos.x.дайЛин()) {
	case Ктрл::LEFT:   out << фмт(".LeftPosZ(%d, %d)", pos.x.GetA(), pos.x.дайС()); break;
	case Ктрл::RIGHT:  out << фмт(".RightPosZ(%d, %d)", pos.x.GetA(), pos.x.дайС()); break;
	case Ктрл::SIZE:   out << фмт(".HSizePosZ(%d, %d)", pos.x.GetA(), pos.x.дайС()); break;
	case Ктрл::CENTER: out << фмт(".HCenterPosZ(%d, %d)", pos.x.дайС(), pos.x.GetA()); break;
	}
	switch(pos.y.дайЛин()) {
	case Ктрл::TOP:    out << фмт(".TopPosZ(%d, %d)", pos.y.GetA() - y, pos.y.дайС()); break;
	case Ктрл::BOTTOM: out << фмт(".BottomPosZ(%d, %d)", pos.y.GetA(), pos.y.дайС()); break;
	case Ктрл::SIZE:   out << фмт(".VSizePosZ(%d, %d)", pos.y.GetA(), pos.y.дайС()); break;
	case Ктрл::CENTER: out << фмт(".VCenterPosZ(%d, %d)", pos.y.дайС(), pos.y.GetA()); break;
	}
	out.удали(0);
	return out;
}

Ткст LayoutItem::сохрани(int i, int y, const Ткст& eol) const
{
	Ткст out;
	if(тип.пустой())
		out << "\tUNTYPED(";
	else {
		Ткст s = тип;
		int q = LayoutTypes().найди(тип);
		if(q >= 0) {
			Ткст n = LayoutTypes()[q].name_space;
			if(n.дайСчёт())
				s = n + "::" + s;
		}
		out << "\tITEM(" << s << ", ";
	}
	Ткст var = variable.пустой() ? фмт("dv___%d", i) : variable;
	out << var << ", " << SaveProperties(y) << ")" << eol;
	return out;
}

void LayoutItem::UnknownPaint(Draw& w)
{
	DrawFatFrame(w, 0, 0, csize.cx, csize.cy, SGray(), 2);
	int q = FindProperty("устНадпись");
	if(q >= 0 && ткст_ли(~property[q]))
		DrawSmartText(w, 0, 0, csize.cx, вУтф8((ШТкст)~property[q]));
	Шрифт fnt = LayFont();
	Ткст s = тип;
	Цвет ink = белый();
	if(пусто_ли(s)) {
		s = "Untyped";
		fnt.Italic();
		ink = светлоЦыан();
	}
	Размер tsz = дайРазмТекста(s, fnt);
	w.DrawRect(csize.cx - tsz.cx - tsz.cy, csize.cy - tsz.cy, tsz.cx + tsz.cy, tsz.cy, SGray());
	w.DrawText(csize.cx - tsz.cx - tsz.cy / 2, csize.cy - tsz.cy, s, fnt, ink);
}

void LayoutItem::CreateMethods(EscValue& ctrl, const Ткст& тип, bool copy) const
{
	int q = LayoutTypes().найди(тип);
	if(q < 0)
		q = LayoutTypes().найди("Unknown");
	if(q < 0)
		return;
	const LayoutType& m = LayoutTypes()[q];
	for(q = 0; q < m.property.дайСчёт(); q++)
		if(пусто_ли(m.property[q].тип))
			CreateMethods(ctrl, m.property[q].имя, copy);
	for(q = 0; q < m.methods.дайСчёт(); q++) {
		ctrl.MapSet(m.methods.дайКлюч(q), m.methods[q]);
		if(copy)
			ctrl.MapSet("Ктрл" + m.methods.дайКлюч(q), m.methods[q]);
	}
}

EscValue LayoutItem::CreateEsc() const
{
	EscValue ctrl;
	Ткст tp = тип;
	Ткст tm;
	if(ParseTemplate(tp, tm)) {
		CreateMethods(ctrl, tp, true);
		if(ctrl.IsMap())
			ctrl.MapSet("CtrlPaint", ctrl.MapGet("рисуй"));
		CreateMethods(ctrl, tm, false);
	}
	else
		CreateMethods(ctrl, tp, false);
	for(int q = 0; q < property.дайСчёт(); q++) {
		EscValue w;
		const Значение& v = ~property[q];
		if(v.является<Шрифт>()) {
			Шрифт fnt = v;
			if(fnt.дайВысоту())
				fnt.устВысоту(Ктрл::VertLayoutZoom(fnt.дайВысоту()));
			w = EscFont(fnt);
		}
		if(ткст_ли(v))
			w = (ШТкст)v;
		if(число_ли(v))
			w = (double)v;
		if(v.является<Цвет>())
			w = EscColor(v);
		ctrl.MapSet(property[q].имя, w);
	}
	ctrl.MapSet("тип", (ШТкст)тип);
	ctrl.MapSet("дайРазм", ReadLambda(фмт("() { return Размер(%d, %d); }", csize.cx, csize.cy)));
	ctrl.MapSet("дайПрям", ReadLambda(фмт("() { return Прям(0, 0, %d, %d); }", csize.cx, csize.cy)));
	return ctrl;
}

EscValue LayoutItem::ExecuteMethod(const char *method, Вектор<EscValue>& arg) const
{
	try {
		EscValue self = CreateEsc();
		return ::выполни(UscGlobal(), &self, method, arg, 50000);
	}
	catch(СиПарсер::Ошибка& e) {
		вКонсоль(e + "\n");
	}
	return EscValue();
}

EscValue LayoutItem::ExecuteMethod(const char *method) const
{
	Вектор<EscValue> arg;
	EscValue h = ExecuteMethod(method, arg);
	return h;
}

Размер LayoutItem::дайМинРазм()
{
	return SizeEsc(ExecuteMethod("дайМинРазм"));
}

Размер LayoutItem::дайСтдРазм()
{
	return SizeEsc(ExecuteMethod("дайСтдРазм"));
}

void LayoutItem::рисуй(Draw& w, Размер sz, bool sample)
{
	if(csize != sz) {
		csize = sz;
		DrawingDraw dw(csize);
		int q = LayoutTypes().найди(тип);
		if(q < 0)
			q = LayoutTypes().найди("Unknown");
		if(q < 0)
			UnknownPaint(dw);
		else {
			try {
				EscValue ctrl = CreateEsc();
				if(ctrl.MapGet("рисуй").IsLambda()) {
					if(sample) {
						ctrl.MapSet("устНадпись", (ШТкст)тип);
						Вектор<EscValue> arg;
						::выполни(UscGlobal(), &ctrl, "Sample", arg, 50000);
					}
					Вектор<EscValue> arg;
					EscValue draw;
					new EscDraw(draw, dw);
					arg.добавь(draw);
					::выполни(UscGlobal(), &ctrl, "рисуй", arg, 50000);
				}
				else
					UnknownPaint(dw);
				cache = dw;
			}
			catch(СиПарсер::Ошибка e) {
				вКонсоль(e);
				DrawingDraw edw(csize);
				edw.DrawRect(0, 0, csize.cx, csize.cy, красный());
				DrawSmartText(edw, 2, 2, csize.cx - 4, "\1[g@Y \1" + e, LayFont(), жёлтый());
				cache = edw;
			}
		}
	}
	w.DrawDrawing(sz, cache);
}

Рисунок GetTypeIcon(const Ткст& тип, int cx, int cy, int i, Цвет bg)
{
	ПРОВЕРЬ(i >= 0 && i < 2);
	int q = LayoutTypes().найди(тип);
	if(q < 0)
		return Null;
	LayoutType& p = LayoutTypes()[q];
	Рисунок& icon = p.icon[i];
	if(p.iconsize[i] != Размер(cx, cy)) {
		p.iconsize[i] = Размер(cx, cy);
		LayoutItem m;
		m.создай(тип);
		Размер stdsize = Zsz(m.дайСтдРазм());
		if(stdsize.cx == 0 || stdsize.cy == 0)
			return Null;

		ImageDraw w(stdsize);
		w.DrawRect(stdsize, bg);
		m.рисуй(w, stdsize, true);
		if(stdsize.cx * cy > stdsize.cy * cx)
			cy = stdsize.cy * cx / stdsize.cx;
		else
			cx = stdsize.cx * cy / stdsize.cy;
		icon = Rescale(w, cx, cy);
	}
	return icon;
}
