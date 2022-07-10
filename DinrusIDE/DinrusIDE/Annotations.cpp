#include "DinrusIDE.h"

void AssistEditor::Annotate(const Ткст& filename)
{
	CodeBaseLock __;
	int fi = GetSourceFileIndex(filename);
	CppBase& base = CodeBase();
	ClearAnnotations();
	for(int j = 0; j < base.дайСчёт(); j++) {
		Ткст nest = base.дайКлюч(j);
		if(*nest != '@') { // Annotations of anonymous structures not suported
			const Массив<CppItem>& n = base[j];
			for(int k = 0; k < n.дайСчёт(); k++) {
				const CppItem& m = n[k];
				if(m.file == fi) {
					Ткст coderef = MakeCodeRef(nest, m.qitem);
					SetAnnotation(m.line - 1,
					              GetRefLinks(coderef).дайСчёт() ? IdeImg::tpp_doc()
					                                              : IdeImg::tpp_pen(),
					              coderef);
				}
			}
		}
	}
}

bool IsCodeItem(const RichTxt& txt, int i)
{
	static Uuid codeitem = CodeItemUuid();
	static Uuid stritem = StructItemUuid();
	if(i < txt.GetPartCount() && txt.IsPara(i)) {
		Uuid style = txt.GetParaStyle(i);
		return style == codeitem || style == stritem;
	}
	return false;
}

bool IsBeginEnd(RichText& txt, int i)
{
	static Uuid begin = BeginUuid();
	static Uuid end = EndUuid();
	if(i < txt.GetPartCount() && txt.IsPara(i)) {
		Uuid style = txt.GetParaStyle(i);
		return style == begin || style == end;
	}
	return false;
}

bool AssistEditor::GetAnnotationRefs(Вектор<Ткст>& tl, Ткст& coderef, int q)
{
	if(annotation_popup.открыт())
		annotation_popup.закрой();
	if(q < 0)
		q = GetActiveAnnotationLine();
	if(q < 0)
		return false;
	coderef = GetAnnotation(q);
	if(пусто_ли(coderef))
		return false;
	tl = GetRefLinks(coderef);
	return true;
}

bool AssistEditor::GetAnnotationRef(Ткст& t, Ткст& coderef, int q)
{
	Вектор<Ткст> tl;
	if(!GetAnnotationRefs(tl, coderef, q))
		return false;
	if(tl.дайСчёт() == 0)
		return true;
	Ткст path = theide ? theide->editfile : Null;
	int mi = 0;
	int m = 0;
	for(int i = 0; i < tl.дайСчёт(); i++) {
		int mm = GetMatchLen(tl[i], path);
		if(mm > m) {
			mi = i;
			m = mm;
		}
	}
	t = tl[mi];
	return true;
}

void AssistEditor::SyncAnnotationPopup()
{
	Ткст coderef;
	Ткст tl;
	if(!GetAnnotationRef(tl, coderef))
		return;
	if(tl.дайСчёт()) {
		static Ткст   last_path;
		static RichText topic_text;
		Ткст path = GetTopicPath(tl);
		if(path != last_path)
			topic_text = ParseQTF(ReadTopic(загрузиФайл(path)).text);
		RichText result;
		Ткст cr = coderef;
		for(int pass = 0; pass < 2; pass++) {
			for(int i = 0; i < topic_text.GetPartCount(); i++)
				if(topic_text.IsTable(i)) {
					const RichTable& t = topic_text.GetTable(i);
					Размер sz = t.дайРазм();
					for(int y = 0; y < sz.cy; y++)
						for(int x = 0; x < sz.cx; x++) {
							const RichTxt& txt = t.дай(y, x);
							for(int i = 0; i < txt.GetPartCount(); i++) {
								if(txt.IsPara(i) && txt.дай(i, topic_text.GetStyles()).формат.label == cr) {
									RichTable r(t, 1);
									result.CatPick(pick(r));
									goto done;
								}
							}
						}
				}
				else
				if(IsCodeItem(topic_text, i) && topic_text.дай(i).формат.label == cr) {
					while(i > 0 && IsCodeItem(topic_text, i)) i--;
					if(!IsCodeItem(topic_text, i)) i++;
					while(IsCodeItem(topic_text, i))
						result.конкат(topic_text.дай(i++));
					while(i < topic_text.GetPartCount() && !IsCodeItem(topic_text, i)
					      && !IsBeginEnd(topic_text, i)) {
						if(topic_text.IsPara(i))
							result.конкат(topic_text.дай(i++));
						else {
							RichTable table(topic_text.GetTable(i++), 1);
							result.CatPick(pick(table));
						}
					}
					pass = 2;
					break;
				}
			if(pass == 0 && !LegacyRef(cr))
				break;
		}
	done:
		result.SetStyles(topic_text.GetStyles());
		annotation_popup.подбери(pick(result), GetRichTextStdScreenZoom());
	}
	else
		if(SyncRefsFinished)
			annotation_popup.SetQTF("[A1 [@b* " + DeQtf(coderef) + "]&Ещё не документировано - кликните, чтобы это сделать");
		else
			annotation_popup.SetQTF("[A1 [@b* " + DeQtf(coderef) + "]&Документация ещё не загружена");
	Прям r = GetLineScreenRect(GetActiveAnnotationLine());
	int h = annotation_popup.дайВысоту(Zx(580));
	h = min(h, Zx(550));
	int y = r.top - h - DPI(16);
	if(y < GetWorkArea().top)
		y = r.bottom;
	annotation_popup.устПрям(r.left, y, Zx(600), h + DPI(16));
	annotation_popup.Ктрл::PopUp(this, false, false, true);
}

void AssistEditor::OpenTopic(Ткст topic, Ткст create, bool before)
{
	if(theide)
		theide->OpenTopic(topic, create, before);
}

void AssistEditor::NewTopic(Ткст группа, Ткст coderef)
{
	if(!theide)
		return;
	Ткст ef = theide->editfile;
	Ткст n = дайТитулф(ef);
	theide->EditFile(приставьИмяф(PackageDirectory(theide->GetActivePackage()), группа + ".tpp"));
	if(!theide->designer)
		return;
	TopicEditor *te = dynamic_cast<TopicEditor *>(&theide->designer->DesignerCtrl());
	if(!te)
		return;
	Ткст scope, элт;
	SplitCodeRef(coderef, scope, элт);
	if(!te->NewTopicEx(пусто_ли(scope) ? n : Join(разбей(scope, ':'), "_"), coderef))
		theide->EditFile(ef);
}

void AssistEditor::EditAnnotation(bool leftclick)
{
	if(!SyncRefsFinished)
		return;
	Ткст coderef;
	Вектор<Ткст> tl;
	if(!GetAnnotationRefs(tl, coderef))
		return;
	устКурсор(дайПоз64(GetActiveAnnotationLine()));
	if(leftclick) {
		auto GoToTopic = [&] (int i) {
			if(theide) {
				theide->ShowTopics();
				if(!theide->doc.GoTo(tl[i] + '#' + coderef) && LegacyRef(coderef))
					theide->doc.GoTo(tl[i] + '#' + coderef);
			}
		};
		if(tl.дайСчёт() > 1) {
			БарМеню bar;
			for(int i = 0; i < tl.дайСчёт(); i++)
				bar.добавь(tl[i], [&] { GoToTopic(i); });
			bar.выполни();
			return;
		}
		if(tl.дайСчёт()) {
			GoToTopic(0);
			return;
		}
	}

	БарМеню bar;
	if(tl.дайСчёт()) {
		for(int i = 0; i < tl.дайСчёт(); i++)
			bar.добавь("Редактировать " + tl[i], THISBACK3(OpenTopic, tl[i] + '#' + coderef, Ткст(), false));
		bar.Separator();
	}

	Ткст scope, элт;
	SplitCodeRef(coderef, scope, элт);
	ВекторМап<Ткст, Ткст> tpp;
	int backi = 0;
	for(int pass = 0; pass < 2; pass++) {
		for(int i = GetCursorLine(); pass ? i < дайСчётСтрок() : i >= 0; pass ? i++ : i--) {
			Ткст coderef2;
			if(GetAnnotationRefs(tl, coderef2, i) && tl.дайСчёт()) {
				Ткст scope2, item2;
				SplitCodeRef(coderef2, scope2, item2);
				for(const Ткст& t : tl)
					if(tpp.найди(t) < 0)
						tpp.добавь(t, coderef2);
			}
		}
		if(pass == 0)
			backi = tpp.дайСчёт();
	}

	if(tpp.дайСчёт()) {
		for(int i = 0; i < tpp.дайСчёт(); i++) {
			Ткст l = tpp.дайКлюч(i);
			bar.добавь("Вставить в " + l, THISBACK3(OpenTopic, l + '#' + tpp[i], coderef, i >= backi));
		}
		bar.Separator();
	}
	bar.добавь("Новая тематика справки..", THISBACK2(NewTopic, "src", coderef));
	bar.добавь("Новая тематика по реализации..", THISBACK2(NewTopic, "srcimp", coderef));
	bar.выполни();
}
