#include "ide.h"

void AssistEditor::Annotate(const String& filename)
{
	CodeBaseLock __;
	int fi = GetSourceFileIndex(filename);
	CppBase& base = CodeBase();
	ClearAnnotations();
	for(int j = 0; j < base.GetCount(); j++) {
		String nest = base.GetKey(j);
		if(*nest != '@') { // Annotations of anonymous structures not suported
			const Array<CppItem>& n = base[j];
			for(int k = 0; k < n.GetCount(); k++) {
				const CppItem& m = n[k];
				if(m.file == fi) {
					String coderef = MakeCodeRef(nest, m.qitem);
					SetAnnotation(m.line - 1,
					              GetRefLinks(coderef).GetCount() ? IdeImg::tpp_doc()
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

bool AssistEditor::GetAnnotationRefs(Vector<String>& tl, String& coderef, int q)
{
	if(annotation_popup.IsOpen())
		annotation_popup.Close();
	if(q < 0)
		q = GetActiveAnnotationLine();
	if(q < 0)
		return false;
	coderef = GetAnnotation(q);
	if(IsNull(coderef))
		return false;
	tl = GetRefLinks(coderef);
	return true;
}

bool AssistEditor::GetAnnotationRef(String& t, String& coderef, int q)
{
	Vector<String> tl;
	if(!GetAnnotationRefs(tl, coderef, q))
		return false;
	if(tl.GetCount() == 0)
		return true;
	String path = theide ? theide->editfile : Null;
	int mi = 0;
	int m = 0;
	for(int i = 0; i < tl.GetCount(); i++) {
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
	String coderef;
	String tl;
	if(!GetAnnotationRef(tl, coderef))
		return;
	if(tl.GetCount()) {
		static String   last_path;
		static RichText topic_text;
		String path = GetTopicPath(tl);
		if(path != last_path)
			topic_text = ParseQTF(ReadTopic(LoadFile(path)).text);
		RichText result;
		String cr = coderef;
		for(int pass = 0; pass < 2; pass++) {
			for(int i = 0; i < topic_text.GetPartCount(); i++)
				if(topic_text.IsTable(i)) {
					const RichTable& t = topic_text.GetTable(i);
					Size sz = t.GetSize();
					for(int y = 0; y < sz.cy; y++)
						for(int x = 0; x < sz.cx; x++) {
							const RichTxt& txt = t.Get(y, x);
							for(int i = 0; i < txt.GetPartCount(); i++) {
								if(txt.IsPara(i) && txt.Get(i, topic_text.GetStyles()).format.label == cr) {
									RichTable r(t, 1);
									result.CatPick(pick(r));
									goto done;
								}
							}
						}
				}
				else
				if(IsCodeItem(topic_text, i) && topic_text.Get(i).format.label == cr) {
					while(i > 0 && IsCodeItem(topic_text, i)) i--;
					if(!IsCodeItem(topic_text, i)) i++;
					while(IsCodeItem(topic_text, i))
						result.Cat(topic_text.Get(i++));
					while(i < topic_text.GetPartCount() && !IsCodeItem(topic_text, i)
					      && !IsBeginEnd(topic_text, i)) {
						if(topic_text.IsPara(i))
							result.Cat(topic_text.Get(i++));
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
		annotation_popup.Pick(pick(result), GetRichTextStdScreenZoom());
	}
	else
		if(SyncRefsFinished)
			annotation_popup.SetQTF("[A1 [@b* " + DeQtf(coderef) + "]&?????? ???? ?????????????????????????????? - ????????????????, ?????????? ?????? ??????????????");
		else
			annotation_popup.SetQTF("[A1 [@b* " + DeQtf(coderef) + "]&???????????????????????? ?????? ???? ??????????????????");
	Rect r = GetLineScreenRect(GetActiveAnnotationLine());
	int h = annotation_popup.GetHeight(Zx(580));
	h = min(h, Zx(550));
	int y = r.top - h - DPI(16);
	if(y < GetWorkArea().top)
		y = r.bottom;
	annotation_popup.SetRect(r.left, y, Zx(600), h + DPI(16));
	annotation_popup.Ctrl::PopUp(this, false, false, true);
}

void AssistEditor::OpenTopic(String topic, String create, bool before)
{
	if(theide)
		theide->OpenTopic(topic, create, before);
}

void AssistEditor::NewTopic(String group, String coderef)
{
	if(!theide)
		return;
	String ef = theide->editfile;
	String n = GetFileTitle(ef);
	theide->EditFile(AppendFileName(PackageDirectory(theide->GetActivePackage()), group + ".tpp"));
	if(!theide->designer)
		return;
	TopicEditor *te = dynamic_cast<TopicEditor *>(&theide->designer->DesignerCtrl());
	if(!te)
		return;
	String scope, item;
	SplitCodeRef(coderef, scope, item);
	if(!te->NewTopicEx(IsNull(scope) ? n : Join(Split(scope, ':'), "_"), coderef))
		theide->EditFile(ef);
}

void AssistEditor::EditAnnotation(bool leftclick)
{
	if(!SyncRefsFinished)
		return;
	String coderef;
	Vector<String> tl;
	if(!GetAnnotationRefs(tl, coderef))
		return;
	SetCursor(GetPos64(GetActiveAnnotationLine()));
	if(leftclick) {
		auto GoToTopic = [&] (int i) {
			if(theide) {
				theide->ShowTopics();
				if(!theide->doc.GoTo(tl[i] + '#' + coderef) && LegacyRef(coderef))
					theide->doc.GoTo(tl[i] + '#' + coderef);
			}
		};
		if(tl.GetCount() > 1) {
			MenuBar bar;
			for(int i = 0; i < tl.GetCount(); i++)
				bar.Add(tl[i], [&] { GoToTopic(i); });
			bar.Execute();
			return;
		}
		if(tl.GetCount()) {
			GoToTopic(0);
			return;
		}
	}

	MenuBar bar;
	if(tl.GetCount()) {
		for(int i = 0; i < tl.GetCount(); i++)
			bar.Add("?????????????????????????? " + tl[i], THISBACK3(OpenTopic, tl[i] + '#' + coderef, String(), false));
		bar.Separator();
	}

	String scope, item;
	SplitCodeRef(coderef, scope, item);
	VectorMap<String, String> tpp;
	int backi = 0;
	for(int pass = 0; pass < 2; pass++) {
		for(int i = GetCursorLine(); pass ? i < GetLineCount() : i >= 0; pass ? i++ : i--) {
			String coderef2;
			if(GetAnnotationRefs(tl, coderef2, i) && tl.GetCount()) {
				String scope2, item2;
				SplitCodeRef(coderef2, scope2, item2);
				for(const String& t : tl)
					if(tpp.Find(t) < 0)
						tpp.Add(t, coderef2);
			}
		}
		if(pass == 0)
			backi = tpp.GetCount();
	}

	if(tpp.GetCount()) {
		for(int i = 0; i < tpp.GetCount(); i++) {
			String l = tpp.GetKey(i);
			bar.Add("???????????????? ?? " + l, THISBACK3(OpenTopic, l + '#' + tpp[i], coderef, i >= backi));
		}
		bar.Separator();
	}
	bar.Add("?????????? ???????????????? ??????????????..", THISBACK2(NewTopic, "src", coderef));
	bar.Add("?????????? ???????????????? ???? ????????????????????..", THISBACK2(NewTopic, "srcimp", coderef));
	bar.Execute();
}
