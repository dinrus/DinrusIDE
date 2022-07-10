#include "Browser.h"

#define CODEITEM   "37138531426314131252341829483370"
#define STRUCTITEM "37138531426314131252341829483380"
#define BEGINSTYLE "05600065144404261032431302351956" 
#define ENDSTYLE   "96390100711032703541132217272105"

static const char styles[] =
	"[ $$0,0#00000000000000000000000000000000:дефолт]"
	"[i448;a25;kKO9;2 $$1,0#" CODEITEM ":codeitem]"
	"[i448;a25;kKO9;3 $$2,0#" STRUCTITEM ":structitem]"
	"[l288;2 $$3,0#27521748481378242620020725143825:desc]"
	"[H6;0 $$4,0#" BEGINSTYLE ":begin]"
	"[l288;a25;kK~~~.1408;@3;2 $$5,0#61217621437177404841962893300719:param]"
	"[0 $$7,0#" ENDSTYLE ":end]"
;

void TopicEditor::JumpToDefinition()
{
	постОбрвыз(callback1(IdeGotoCodeRef, editor.GetFormatInfo().label));
}

void TopicEditor::Надпись(Ткст& label)
{
	сохрани();
	if(ref.элт.IsMultiSelect())
		ref.элт.очистьВыделение();
	ref.элт.MultiSelect(false);
	ref.Титул("Справочник");
	ref.уст(label);
	ref.classlist.скрой();
	if(ref.выполни() != IDOK)
		return;
	label = ref.дай();
}

Uuid CodeItemUuid()
{
	return ScanUuid(CODEITEM);
}

Uuid StructItemUuid()
{
	return ScanUuid(STRUCTITEM);
}

Uuid BeginUuid()
{
	return ScanUuid(BEGINSTYLE);
}

Uuid EndUuid()
{
	static Uuid u = ScanUuid(ENDSTYLE);
	return u;
}

void TopicEditor::FindBrokenRef()
{
	Uuid codeitem = CodeItemUuid();
	Uuid stritem = StructItemUuid();;
	for(;;) {
		if(пусто_ли(topicpath))
			return;
		const RichText& txt = editor.дай();
		int c = editor.дайКурсор();
		int i = txt.FindPart(c);
		while(++i < txt.GetPartCount()) {
			if(txt.IsPara(i)) {
				Uuid style = txt.GetParaStyle(i);
				if(style == codeitem || style == stritem) {
					RichPara para = txt.дай(i);
					if(para.формат.label == "noref")
						continue;
					if(!пусто_ли(para.формат.label) && GetCodeRefItem(para.формат.label))
						continue;
					editor.Move(txt.GetPartPos(i));
					return;
				}
			}
		}
		if(!topics_list.курсор_ли())
			break;
		c = topics_list.дайКурсор() + 1;
		if(c >= topics_list.дайСчёт()) {
			PromptOK("Больше никаких ломанных ссылок.");
			break;
		}
		topics_list.устКурсор(c);
		if(!пусто_ли(topicpath))
			editor.Move(0);
	}
}

void TopicEditor::Tools(Бар& bar)
{
	bar.добавь("Вставить элемент кода..", IdeCommonImg::InsertItem(), THISBACK(InsertItem))
	   .Ключ(K_CTRL_INSERT);
	Ткст l = editor.GetFormatInfo().label;
	bool b = l.дайСчёт() > 2 && l != "noref";
	bar.добавь(b, "Посмотреть код по ссылке", IdeCommonImg::Source(), THISBACK(JumpToDefinition))
	   .Ключ(K_ALT_J).Ключ(K_ALT_I);
	bar.добавь("Найти ломанные ссылки..", IdeCommonImg::FindBrokenRef(), THISBACK(FindBrokenRef))
	   .Ключ(K_CTRL_F3);
#ifdef REPAIR
	bar.Separator();
	bar.добавь("Починить!", CtrlImg::Toggle(), THISBACK(Repair)).Ключ(K_ALT_F5);
	bar.Separator();
#endif
}

void TopicEditor::MainTool(Бар& bar)
{
	editor.StyleTool(bar);
	bar.Gap();
	editor.FontTools(bar);
	bar.Gap();
	editor.InkTool(bar);
	editor.PaperTool(bar);
	bar.Gap();
	editor.LanguageTool(bar);
	editor.SpellCheckTool(bar);
	bar.Gap();
	editor.IndexEntryTool(bar);
	bar.Break();
	editor.HyperlinkTool(bar, Zx(300), K_CTRL_H);
	bar.Gap();
	editor.ParaTools(bar);
	bar.Gap();
	editor.EditTools(bar);
	bar.Gap();
	bar.добавь("Печать", CtrlImg::print(), THISBACK(Print))
	   .Ключ(K_CTRL_P);
	bar.GapRight();
	bar.Break();
	editor.LabelTool(bar, Zx(300), K_CTRL_M, "Справочник по коду");
	bar.Gap();
	Tools(bar);
	bar.Gap();
	editor.TableTools(bar);
}

Ткст NaturalDeQtf(const char *s) {
	ТкстБуф r;
	r.резервируй(256);
	bool cm = false;
	while(*s) {
		if(*s == ' ')
			r.конкат(cm ? ' ' : '_');
		else {
			if((byte)*s > ' ' && !цифра_ли(*s) && !IsAlpha(*s) && (byte)*s < 128)
				r.конкат('`');
			r.конкат(*s);
			if(*s == ',')
				cm = true;
			else
				cm = false;
		}
		s++;
	}
	return Ткст(r);
}

static int sSplitT(int c) {
	return c == ';' || c == '<' || c == '>' || c == ',';
}

bool IsCodeRefType(const Ткст& тип)
{
	if(тип.дайСчёт() == 0)
		return false;
	CodeBaseLock __;
	return CodeBase().найди(тип) >= 0;
}

Ткст DecoratedItem(const Ткст& имя, const CppItem& m, const char *natural, int pari)
{
	Ткст qtf = "[%00-00K ";
	Вектор<ItemTextPart> n = ParseItemNatural(имя, m, natural);
	if(pari < 0) {
		if(m.virt)
			qtf << "[@B virtual] ";
		if(m.kind == CLASSFUNCTION || m.kind == CLASSFUNCTIONTEMPLATE)
			qtf << "[@B static] ";
	}
	Вектор<Ткст> qt = разбей(m.qptype, sSplitT, false);
	Вектор<Ткст> tt = разбей(m.qtype, sSplitT, false);
	for(int i = 0; i < n.дайСчёт(); i++) {
		ItemTextPart& p = n[i];
		qtf << "[";
		if(p.pari == pari)
			qtf << "$C";
		switch(p.тип) {
		case ITEM_PNAME:
			qtf << "*";
		case ITEM_NUMBER:
			qtf << "@r";
			break;
		case ITEM_TNAME:
			qtf << "*@g";
			break;
		case ITEM_NAME:
			qtf << "*";
			break;
		case ITEM_UPP:
			qtf << "@c";
			break;
		case ITEM_CPP_TYPE:
		case ITEM_CPP:
			qtf << "@B";
			break;
		default:
			int q = p.тип - ITEM_PTYPE;
			if(q >= 0 && q < qt.дайСчёт() && IsCodeRefType(qt[q]) && pari < 0)
				qtf << "_^" << qt[q] << '^';
			q = p.тип - ITEM_TYPE;
			if(q >= 0 && q < tt.дайСчёт() && IsCodeRefType(tt[q]) && pari < 0)
				qtf << "_^" << tt[q] << '^';
			break;
		}
		qtf << ' ';
		qtf << NaturalDeQtf(Ткст(~m.natural + p.pos, p.len));
		qtf << ']';
	}
	return qtf + "]";
}

Ткст CreateQtf(const Ткст& элт, const Ткст& имя, const CppItem& m, const Ткст& lang, bool onlyhdr = false)
{
	Ткст qtf;
	bool str = m.kind == STRUCT || m.kind == STRUCTTEMPLATE;
	if(!str)
		qtf << "[s4 &]";
	Ткст st = str ? "[s2;" : "[s1;";
	Ткст k = st + ':' + DeQtf(элт) + ": ";
	if(m.IsTemplate() && str) {
		int q = 0;
		int w = 0;
		while(q < m.natural.дайДлину()) {
			if(m.natural[q] == '<')
				w++;
			if(m.natural[q] == '>') {
				w--;
				if(w == 0) {
					q++;
					break;
				}
			}
			q++;
		}
		qtf << "[s2:noref: " << DecoratedItem(имя, m, m.natural.середина(0, q)) << "&][s2 " << k;
		if(q < m.natural.дайДлину()) {
			while((byte)m.natural[q] <= 32)
				q++;
			qtf << DecoratedItem(имя, m, m.natural.середина(q));
		}
	}
	else
		qtf << k << DecoratedItem(имя, m, m.natural);

	qtf << "&]";
	if(onlyhdr)
		return qtf;
	qtf << "[s3%" << lang << " ";
	Ткст d;
	Вектор<Ткст> t = разбей(m.tname, ';');
	for(int i = 0; i < t.дайСчёт(); i++) {
		if(i)
			d << ' ';
		d << "[%-*@g " << DeQtf(t[i]) << "]";
	}
	d.очисть();
	d << "[%" << lang << " ";
	Вектор<Ткст> p = разбей(m.pname, ';');
	if(!str)
		for(int i = 0; i < p.дайСчёт(); i++)
			d << " [%-*@r " << DeQtf(p[i]) << "]";
	if(!str && p.дайСчёт())
		qtf << d << " .";
	qtf << "&]";
	qtf << "[s7 &]";
	return qtf;
}

Ткст TopicEditor::GetLang() const
{
	int q = topicpath.найдирек('@');
	if(q >= 0) {
		int lang = LNGFromText(~topicpath + q + 1);
		if(lang)
			return LNGAsText(lang);
	}
	return "%";
}

void TopicEditor::InsertItem()
{
	if(пусто_ли(topicpath))
		return;
	сохрани();
	ref.Титул("Вставить");
	if(ref.элт.курсор_ли())
		ref.элт.устФокус();
	ref.элт.MultiSelect();
	ref.classlist.покажи();
	int c = ref.выполни();
	if(c == IDCANCEL)
		return;
	if(c == IDYES) {
		Ткст qtf = "&{{1 ";
		bool next = false;
		for(int i = 0; i < ref.scope.дайСчёт(); i++) {
			Ткст s = ref.scope.дай(i, 1);
			if(*s != '<') {
				if(next)
					qtf << ":: ";
				qtf << DeQtf(s);
				next = true;
			}
		}
		qtf << "}}&";
		editor.PasteText(ParseQTF(qtf));
		return;
	}
	Ткст qtf;
	if(ref.элт.выделение_ли()) {
		for(int i = 0; i < ref.элт.дайСчёт(); i++)
			if(ref.элт.выделен(i)) {
				CppItemInfo m = ref.GetItemInfo(i);
				qtf << CreateQtf(ref.GetCodeRef(i), m.имя, m, GetLang());
			}
	}
	else
	if(ref.элт.курсор_ли()) {
		CppItemInfo m = ref.GetItemInfo();
		qtf << CreateQtf(ref.GetCodeRef(), m.имя, m, GetLang());
	}
	else
		return;
	editor.BeginOp();
	editor.PasteText(ParseQTF(styles + qtf));
	editor.PrevPara();
	editor.PrevPara();
}

void TopicEditor::InsertNew(const Ткст& coderef)
{
	const CppItem *m = GetCodeRefItem(coderef);
	if(!m)
		return;
	editor.BeginOp();
	editor.PasteText(ParseQTF(styles + CreateQtf(coderef, m->имя, *m, GetLang())));
	editor.PrevPara();
	editor.PrevPara();
}

void TopicEditor::GoTo(const Ткст& _topic, const Ткст& link, const Ткст& create, bool before)
{
	if(topics_list.FindSetCursor(_topic) && !пусто_ли(link)) {
		editor.выдели(editor.дайДлину(), 0);
		if(!editor.GotoLabel(link)) {
			Ткст l = link;
			LegacyRef(l);
			editor.GotoLabel(l);
		}
		if(!пусто_ли(create)) {
			if(!before)
				for(int pass = 0; pass < 2; pass++)
					for(;;) {
						int c = editor.дайКурсор();
						RichText::FormatInfo f = editor.GetFormatInfo();
						if(f.styleid == BeginUuid() || (пусто_ли(f.label) || f.label == "noref") && pass)
							break;
						editor.NextPara();
						if(editor.дайКурсор() == c)
							break;
					}
			InsertNew(create);
		}
	}
}

void   TopicEditor::FixTopic()
{
	Ткст nest;
	if(!редактируйТекст(nest, "Фиксировать тематику", "Гнездо"))
		return;
	CodeBaseLock __;
	CppBase& base = CodeBase();
	int q = base.найди(nest);
	if(q < 0) {
		Exclamation("Гнездо не найдено");
		return;
	}
	Массив<CppItem>& n = base[q];
	Индекс<Ткст> natural;
	Вектор<Ткст> link;
	for(int i = 0; i < n.дайСчёт(); i++) {
		const CppItem& m = n[i];
		Ткст nat;
		if(m.virt)
			nat << "virtual ";
		if(m.kind == CLASSFUNCTION || m.kind == CLASSFUNCTIONTEMPLATE)
			nat << "static ";
		nat << m.natural;
		natural.добавь(nat);
		link.добавь(MakeCodeRef(nest, n[i].qitem));
	}
	RichText result;
	const RichText& txt = editor.дай();
	bool started = false;

	int l, h;
	if(editor.дайВыделение(l, h)) {
		l = txt.FindPart(l);
		h = txt.FindPart(h);
	}
	else {
		l = 0;
		h = txt.GetPartCount();
	}

	for(int i = 0; i < txt.GetPartCount(); i++)
		if(txt.IsPara(i)) {
			RichPara p = txt.дай(i);
			if(i >= l && i < h) {
				ШТкст h = p.дайТекст();
				Ткст nat;
				const wchar *s = h;
				while(*s)
					if(*s == 160 || *s == ' ') {
						nat.конкат(' ');
						while(*s == 160 || *s == ' ') s++;
					}
					else
						nat.конкат(*s++);
				int q = nat.дайСчёт() ? natural.найди(nat) : -1;
				if(q >= 0) {
					started = true;
					const CppItem& m = n[q];
					RichText h = ParseQTF(styles + ("[s7; &]" + CreateQtf(link[q], n[q].имя, m, GetLang(), true)));
					if(h.GetPartCount())
						h.RemovePart(h.GetPartCount() - 1);
					result.CatPick(pick(h));
				}
				else
				if(!started || p.дайДлину())
					result.конкат(p);
			}
			else
				result.конкат(p);
		}
		else {
			RichTable b;
			b <<= txt.GetTable(i);
			result.CatPick(pick(b));
		}
	RichPara empty;
	result.конкат(empty);
	editor.BeginOp();
	editor.устВыделение(0, txt.дайДлину());
	editor.PasteText(result);
}

Ткст TopicEditor::дайИмяф() const
{
	return grouppath;
}

void TopicEditor::сохрани()
{
	SaveTopic();
	SaveInc();
}

void TopicEditor::устФокус()
{
	if(editor.включен_ли())
		editor.устФокус();
	else
		Ктрл::устФокус();
}
