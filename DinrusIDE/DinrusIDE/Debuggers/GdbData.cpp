#include "Debuggers.h"

#define METHOD_NAME UPP_METHOD_NAME("Gdb")

void SkipGdbInfo(СиПарсер& p)
{
	int level = 1;
	while(level > 0 && !p.кф_ли())
		if(p.сим('<'))
			level++;
		else
		if(p.сим('>'))
			level--;
		else
			p.дайСим();
}

Ткст DataClean(СиПарсер& p)
{
	Ткст r;
	if(p.сим('{')) {
		while(!p.кф_ли() && !p.сим('}')) {
			p.ид("static");
			if(p.сим('<')) {
				SkipGdbInfo(p);
				p.сим('=');
				Ткст q = DataClean(p);
				if(!q.пустой()) {
					if(!r.пустой())
						r << ", ";
					r << q;
				}
			}
			else
			if(p.ид_ли()) {
				Ткст id = p.читайИд();
				p.сим('=');
				Ткст q = DataClean(p);
				if(!q.пустой()) {
					if(!r.пустой())
						r << ", ";
					r << id << " = " << q;
				}
			}
			else
				p.дайСим();
		}
		if(!r.пустой() && (*r != '{' || *r.последний() != '}'))
			return "{ " + r + " }";
		return r;
	}
	int lvl = 0;
	for(;;) {
		bool sp = p.пробелы();
		if(lvl == 0 && (p.сим_ли('}') || p.сим_ли(',')) || p.кф_ли())
			break;
		if(sp)
			r << ' ';
		if(p.ткст_ли())
			r << какТкстСи(p.читайТкст());
		else
		if(p.сим('<') && lvl == 0)
			SkipGdbInfo(p);
		else {
			if(p.сим_ли('('))
				lvl++;
			if(p.сим_ли(')'))
				lvl--;
			r.конкат(p.дайСим());
		}
	}
	return r;
}

Ткст DataClean(const char *s)
{
	try {
		СиПарсер p(s);
		return DataClean(p);
	}
	catch(СиПарсер::Ошибка) {}
	return Null;
}

Ткст GdbData(const Ткст& exp)
{
	int q = exp.найди('{');
	return q < 0 ? exp : обрежьОба(exp.середина(q));
}

void Gdb::Locals()
{
	ВекторМап<Ткст, Ткст> prev = DataMap(locals);
	locals.очисть();
	Ткст s = FastCmd("info locals");
	ТкстПоток ss(s);
	while(!ss.кф_ли()) {
		Ткст ln = ss.дайСтроку();
		const char *s = ln;
		const char *q = strchr(s, '=');
		if(!q) break;
		const char *e = q;
		while(e > s && e[-1] == ' ')
			e--;
		q++;
		while(*q == ' ')
			q++;
		Ткст h = GdbData(q);
		locals.добавь(Ткст(s, e), DataClean(h), h);
	}
	MarkChanged(prev, locals);
}

Ткст Gdb::Print0(const Ткст& exp)
{
	Ткст q = FastCmd("print " + exp);
	ТкстПоток ss(q);
	Ткст ln = ss.дайСтроку();
	const char *s = strchr(ln, '=');
	if(s) {
		s++;
		while(*s == ' ')
			s++;
		return GdbData(s);
	}
	else
		return GdbData(ln);
}

Ткст Gdb::Print(const Ткст& exp)
{
	int q = expression_cache.найди(exp);
	if(q >= 0)
		return expression_cache[q];
	Ткст r = Print0(exp);
	expression_cache.добавь(exp, r);
	return r;
}

void Gdb::Watches()
{
	ВекторМап<Ткст, Ткст> prev = DataMap(watches);
	for(int i = 0; i < watches.дайСчёт(); i++) {
		Ткст h = Print(watches.дай(i, 0));
		watches.уст(i, 1, DataClean(h));
		watches.уст(i, 2, h);
	}
	MarkChanged(prev, watches);
}

void Gdb::TryAuto(Индекс<Ткст>& tested, const Ткст& exp)
{
	if(tested.найди(exp) < 0 && findarg(exp, "true", "false") < 0) {
		tested.добавь(exp);
		Ткст val = Print(exp);
		if(!пусто_ли(val) && !val.заканчиваетсяНа(")}") && !val.заканчиваетсяНа(")>") &&
		   (!IsAlpha(*val) || findarg(val, "true", "false") >= 0))
			autos.добавь(exp, DataClean(val), val);
	}
}

void Gdb::Autos()
{
	ВекторМап<Ткст, Ткст> prev = DataMap(autos);
	autos.очисть();
	Индекс<Ткст> tested;
	try {
		СиПарсер p(autoline);
		while(!p.кф_ли())
			if(p.ид_ли()) {
				Ткст exp = p.читайИд();
				TryAuto(tested, exp);
				for(;;) {
					if(p.сим('.') && p.ид_ли())
						exp << '.';
					else
					if(p.сим2('-', '>') && p.ид_ли())
						exp << "->";
					else
						break;
					exp << p.читайИд();
					TryAuto(tested, exp);
				}
			}
			else
				p.пропустиТерм();
	}
	catch(СиПарсер::Ошибка) {}
	autos.сортируй();
	MarkChanged(prev, autos);
}

void Gdb::ReadGdbValues(СиПарсер& p, ВекторМап<Ткст, Ткст>& val)
{
	if(p.сим('{')) {
		while(!p.сим('}') && !p.кф_ли()) {
			if(p.сим_ли('{')) {
				ReadGdbValues(p, val);
				continue;
			}
			Ткст id;
			Ткст значение;
			const char *b = p.дайУк();
			while(!p.кф_ли())
				if(p.сим_ли('=')) {
					id = обрежьОба(Ткст(b, p.дайУк()));
					p.сим('=');
					break;
				}
				else
					p.пропустиТерм();
			b = p.дайУк();
			int lvl = 0;
			while(!p.кф_ли())
				if((p.сим_ли(',') || p.сим_ли('}')) && lvl == 0) {
					Ткст v = обрежьОба(Ткст(b, p.дайУк()));
					if(*id == '<') // Основа class
						ReadGdbValues(v, val);
					else
						val.добавь(id, v);
					p.сим(',');
					break;
				}
				else
				if(p.сим('{'))
					lvl++;
				else
				if(p.сим('}'))
					lvl--;
				else
					p.пропустиТерм();
		}
	}
}

void Gdb::ReadGdbValues(const Ткст& h, ВекторМап<Ткст, Ткст>& val)
{
	try {
		СиПарсер p(h);
		ReadGdbValues(p, val);
	}
	catch(СиПарсер::Ошибка) {}
}

void Gdb::Self()
{
	ВекторМап<Ткст, Ткст> prev = DataMap(locals);
	self.очисть();
	ВекторМап<Ткст, Ткст> val;
	ReadGdbValues(Print("*this"), val);
	for(int i = 0; i < val.дайСчёт(); i++)
		self.добавь(val.дайКлюч(i), DataClean(val[i]), val[i]);
	MarkChanged(prev, locals);
}

void Gdb::Cpu()
{
#ifdef PLATFORM_WIN32 // mingw dbg is crashing if listing all registers
	Ткст s = FastCmd("info registers rax rbx rcx rdx rsi rdi rbp rsp r8  r9  r10 r11 r12 r13 r14 r15");
#else
	Ткст s = FastCmd("info registers");
#endif
	ТкстПоток ss(s);
	cpu.очисть();
	bool even = false;
	while(!ss.кф_ли()) {
		Ткст ln = ss.дайСтроку();
		cpu.добавь(AttrText(ln).устШрифт(Courier(Ктрл::HorzLayoutZoom(12)))
		        .Paper(even ? смешай(SColorMark, SColorPaper, 220) : SColorPaper)
		        .Ink(SColorText));
		even = !even;
	}
}

void Gdb::ObtainData()
{
	switch(tab.дай()) {
	case 0: Autos(); break;
	case 1: Locals(); break;
	case 2: Watches(); break;
	case 3: Self(); break;
	case 4: Cpu(); break;
	case 5: Memory(); break;
	}
}

void Gdb::SetTab(int q)
{
	if(исрОтладБлокировка_ли())
		return;
	tab.уст(q);
	ObtainData();
}

void Gdb::ClearWatches()
{
	if(исрОтладБлокировка_ли())
		return;
	watches.очисть();
	SetTab(2);
}

void Gdb::QuickWatch()
{
	if(исрОтладБлокировка_ли())
		return;
	quickwatch.expression << quickwatch.Breaker(999);
	FastCmd("set print pretty on");
	for(;;) {
		int q = quickwatch.пуск();
		if(findarg(q, IDCANCEL, IDOK) >= 0) {
			FastCmd("set print pretty off");
			if(q == IDOK) {
				watches.добавь(~quickwatch.expression);
				SetTab(2);
			}
			quickwatch.закрой();
			return;
		}
		Ткст s = FastCmd("p " + (Ткст)~quickwatch.expression);
		const char *a = strchr(s, '=');
		if(a) {
			a++;
			while(*a == ' ')
				a++;
			quickwatch.значение <<= a;
			quickwatch.expression.AddHistory();
		}
		else
			quickwatch.значение <<= s;
	}
}

bool Gdb::Подсказка(const Ткст& exp, РедакторКода::MouseTip& mt)
{
	if(пусто_ли(exp))
		return false;
	Ткст val = DataClean(Print(exp));
	if(!пусто_ли(val) && !val.заканчиваетсяНа(")}") && !IsAlpha(*val)) {
		if(val.дайСчёт() > 100) {
			val.обрежь(100);
			val << "...";
		}
		mt.дисплей = &StdDisplay();
		mt.значение = val;
		mt.sz = mt.дисплей->дайСтдРазм(Ткст(mt.значение) + "X");
		return true;
	}
	return false;
}

void Gdb::SetTree(КтрлМассив *a)
{
	tree.очисть();
	if(a->курсор_ли()) {
		Ткст h = a->дай(2);
		tree.SetRoot(Null, h, Ткст().конкат() << a->дай(0) << " = " << DataClean(h));
		tree.открой(0);
	}
}

Значение Gdb::ObtainValueFromTreeCursor(int cursor) const
{
	auto cursor_str = tree.дай(cursor).вТкст();
	if (cursor_str.пустой())
		return {};
	
	Значение val;
	try {
		СиПарсер parser(cursor_str);
		while (!parser.кф_ли()) {
			if (parser.ткст_ли()) {
				val = parser.читайТкст();
				break;
			}
			
			parser.пропустиТерм();
		}
	} catch(СиПарсер::Ошибка e) {
		Loge() << METHOD_NAME << e;
	}
	
	return val;
}

void Gdb::OnTreeBar(Бар& bar)
{
	auto cursor = tree.дайКурсор();
	if(cursor < 0)
		return;
	
	if(tree.GetChildCount(cursor) > 0)
		return;
	
	Значение val = ObtainValueFromTreeCursor(cursor);
	
	bar.добавь(t_("Копировать  значение в буфер обмена"), [this, val]() { OnValueCopyToClipboard(val); }).вкл(!val.пусто_ли());
	
	Ткст var;
	while(cursor >= 0) {
		Ткст s = tree.дайЗначение(cursor);
		try {
			СиПарсер p(s);
			if(p.ид_ли())
				var = Merge(".", p.читайИд(), var);
			cursor = tree.дайРодителя(cursor);
		}
		catch(СиПарсер::Ошибка) {
			break;
		}
	}

	if(var.дайСчёт())
		MemoryMenu(bar, var);
}

void Gdb::OnValueCopyToClipboard(const Значение& val)
{
	if (ткст_ли(val))
		AppendClipboardText(val.вТкст());
}

void Gdb::OnTreeExpand(int node)
{
	if(tree.GetChildCount(node))
		return;
	ВекторМап<Ткст, Ткст> val;
	ReadGdbValues(tree.дай(node), val);
	for(int i = 0; i < val.дайСчёт(); i++)
		tree.добавь(node, Null, val[i], val.дайКлюч(i) + " = " + DataClean(val[i]), *val[i] == '{');
}
