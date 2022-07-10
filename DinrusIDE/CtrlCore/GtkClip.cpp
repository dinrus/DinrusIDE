#include <CtrlCore/CtrlCore.h>
#include <plugin/bmp/bmp.h>

#ifdef GUI_GTK

namespace РНЦП {

#define LLOG(x)   // DLOG(x)

void Ктрл::GtkSelectionDataSet(GtkSelectionData *selection_data, const Ткст& fmt, const Ткст& data)
{
	if(fmt == "files")  {
		Вектор<Ткст> h = разбей(data, '\n');
		Буфер<gchar *> uris(h.дайСчёт() + 1, NULL);
		for(int i = 0; i < h.дайСчёт(); i++) {
			int l = h[i].дайСчёт() + 1;
			uris[i] = new gchar[l];
			memcpy(uris[i], ~h[i], l);
		}
		gtk_selection_data_set_uris(selection_data, ~uris);
		for(int i = 0; i < h.дайСчёт(); i++)
			delete uris[i];
	}
	else
	if(fmt == "text")
		gtk_selection_data_set_text(selection_data, (const gchar*)~data, data.дайСчёт());
	else
	if(fmt == "image") {
		Рисунок img;
		if(ткст_ли(data))
			грузиИзТкст(img, data);
		if(!пусто_ли(img)) {
			ImageGdk m(img);
			gtk_selection_data_set_pixbuf(selection_data, m);
		}
	}
	else
		gtk_selection_data_set(selection_data, GAtom(fmt), 8, (const guchar*)~data, data.дайСчёт());
}

void Ктрл::GtkGetClipData(GtkClipboard *clipboard, GtkSelectionData *selection_data,
                          guint info, gpointer user_data)
{
	ВекторМап<Ткст, ClipData>& target = ((Gclipboard *)user_data)->target;
	LLOG("GtkGetClipData for " << target.дайКлюч(info));
	GtkSelectionDataSet(selection_data, target.дайКлюч(info), target[info].Render());
}

void ClearClipData(GtkClipboard *clipboard, gpointer) {}

Ктрл::Gclipboard::Gclipboard(GdkAtom тип)
{
	clipboard = gtk_clipboard_get(тип);
}

void Ктрл::AddFmt(GtkTargetList *list, const Ткст& fmt, int info)
{
	if(fmt == "files")
		gtk_target_list_add_uri_targets(list, info);
	else
	if(fmt == "text")
		gtk_target_list_add_text_targets(list, info);
	else
	if(fmt == "image")
		gtk_target_list_add_image_targets(list, info, TRUE);
	else
		gtk_target_list_add(list, GAtom(fmt), 0, info);
}

GtkTargetList *Ктрл::CreateTargetList(const ВекторМап<Ткст, ClipData>& target)
{
	GtkTargetList *list = gtk_target_list_new(NULL, 0);
	for(int i = 0; i < target.дайСчёт(); i++)
		AddFmt(list, target.дайКлюч(i), i);
	return list;
}

void Ктрл::Gclipboard::помести(const Ткст& fmt, const ClipData& data)
{
	ЗамкниГип __;
	LLOG("Gclipboard::помести " << fmt);

	target.дайДобавь(fmt) = data;

	GtkTargetList *list = CreateTargetList(target);
	
	gint n;
	GtkTargetEntry *targets = gtk_target_table_new_from_list(list, &n);
	
	gtk_clipboard_set_with_data(clipboard, targets, n, GtkGetClipData, ClearClipData, this);
	gtk_clipboard_set_can_store(clipboard, NULL, 0);

	gtk_target_table_free(targets, n);
	gtk_target_list_unref(list);
}

Ткст Ктрл::GtkDataGet(GtkSelectionData *s)
{
	if(!s)
		return Null;
	const guchar *b = gtk_selection_data_get_data(s);
	int n = gtk_selection_data_get_length(s);
	return n >= 0 && b ? Ткст(b, n) : Ткст();
}

Ткст Ктрл::Gclipboard::дай(const Ткст& fmt)
{
	LLOG("Ктрл::Gclipboard::дай " << fmt);
	if(fmt == "text") {
		gchar *s = gtk_clipboard_wait_for_text(clipboard);
		if(s) {
			Ткст h = s;
			g_free(s);
			return h;
		}
		return Null;
	}
	else
	if(fmt == "image")
		return ImageClipFromPixbufUnref(gtk_clipboard_wait_for_image(clipboard));
	else
	if(fmt == "files")
		return FilesClipFromUrisFree(gtk_clipboard_wait_for_uris(clipboard));
	else
		return GtkDataGet(gtk_clipboard_wait_for_contents(clipboard, GAtom(fmt)));
}

bool Ктрл::Gclipboard::IsAvailable(const Ткст& fmt)
{
	if(fmt == "files")
		return gtk_clipboard_wait_is_uris_available(clipboard);
	if(fmt == "text")
		return gtk_clipboard_wait_is_text_available(clipboard);
	if(fmt == "image")
		return gtk_clipboard_wait_is_image_available(clipboard);
	return gtk_clipboard_wait_is_target_available(clipboard, GAtom(fmt));
}

bool PasteClip::IsAvailable(const char *fmt) const
{
	LLOG("PasteClip::IsAvailable " << fmt << ", тип: " << тип);
	if(тип == 1)
		return Ктрл::IsDragAvailable(fmt);
	return (тип == 0 ? Ктрл::gclipboard() : Ктрл::gselection()).IsAvailable(fmt);
}

Ткст PasteClip::дай(const char *fmt) const
{
	LLOG("PasteClip::дай " << fmt << ", тип: " << тип);
	if(тип == 1)
		return Ктрл::DragGet(fmt);
	return (тип == 0 ? Ктрл::gclipboard() : Ктрл::gselection()).дай(fmt);
}

void PasteClip::GuiPlatformConstruct()
{
	тип = 0;
}

void Ктрл::Gclipboard::очисть()
{
	gtk_clipboard_clear(clipboard);
	target.очисть();	
}

void ClearClipboard()
{
	Ктрл::gclipboard().очисть();
}

Ктрл::Gclipboard& Ктрл::gclipboard()
{
	ЗамкниГип __; 
	static Gclipboard c(GDK_SELECTION_CLIPBOARD);
	return c;
}

Ктрл::Gclipboard& Ктрл::gselection()
{
	ЗамкниГип __; 
	static Gclipboard c(GDK_SELECTION_PRIMARY);
	return c;
}

void AppendClipboard(const char *формат, const Значение& data, Ткст (*render)(const Значение& data))
{
	ЗамкниГип __; 
	LLOG("AppendClipboard " << формат);
	Вектор<Ткст> s = разбей(формат, ';');
	for(int i = 0; i < s.дайСчёт(); i++)
		Ктрл::gclipboard().помести(s[i], ClipData(data, render));
}

void AppendClipboard(const char *формат, const byte *data, int length)
{
	ЗамкниГип __;
	AppendClipboard(формат, Ткст(data, length));
}

void AppendClipboard(const char *формат, const Ткст& data)
{
	ЗамкниГип __;
	LLOG("AppendClipboard " << формат);
	Вектор<Ткст> s = разбей(формат, ';');
	for(int i = 0; i < s.дайСчёт(); i++)
		Ктрл::gclipboard().помести(s[i], ClipData(data));
}

Ткст ReadClipboard(const char *формат)
{
	ЗамкниГип __;
	return Null;
}

const char *ClipFmtsText()
{
	ЗамкниГип __;
	return "text";
}

void AppendClipboardUnicodeText(const ШТкст& s)
{
	AppendClipboard(ClipFmtsText(), Значение(вУтф8(s)), NULL);
}

void AppendClipboardText(const Ткст& s)
{
	AppendClipboard(ClipFmtsText(), Значение(вНабсим(CHARSET_UTF8, s)), NULL);
}

Ткст дайТкст(PasteClip& clip)
{
	ЗамкниГип __;
	if(clip.IsAvailable("text"))
		return вНабсим(CHARSET_DEFAULT, clip.дай("text"), CHARSET_UTF8);
	return Null;
}

ШТкст дайШТкст(PasteClip& clip)
{
	ЗамкниГип __;
	if(clip.IsAvailable("text"))
		return вУтф32(clip.дай("text"));
	return Null;
}


bool AcceptText(PasteClip& clip)
{
	return clip.прими(ClipFmtsText());
}

static Ткст sText(const Значение& data)
{
	return data;
}

void приставь(ВекторМап<Ткст, ClipData>& data, const Ткст& text)
{
	data.дайДобавь("text", ClipData(вНабсим(CHARSET_UTF8, text), sText));
}

void приставь(ВекторМап<Ткст, ClipData>& data, const ШТкст& text)
{
	data.дайДобавь("text", ClipData(вУтф8(text), sText));
}

Ткст GetTextClip(const ШТкст& text, const Ткст& fmt)
{
	if(fmt == "text")
		return вУтф8(text);
	return Null;
}

Ткст GetTextClip(const Ткст& text, const Ткст& fmt)
{
	if(fmt == "text")
		return вНабсим(CHARSET_UTF8, text);
	return Null;
}

Ткст ReadClipboardText()
{
	return вНабсим(CHARSET_DEFAULT, Ктрл::gclipboard().дай("text"), CHARSET_UTF8);
}

ШТкст ReadClipboardUnicodeText()
{
	return вУтф32(Ктрл::gclipboard().дай("text"));
}

bool IsClipboardAvailable(const char *id)
{
	return Ктрл::gclipboard().IsAvailable(id);
}

bool IsClipboardAvailableText()
{
	return Ктрл::gclipboard().IsAvailable("text");
}

const char *ClipFmtsImage()
{
	return "image";
}

bool AcceptImage(PasteClip& clip)
{
	ЗамкниГип __;
	return clip.прими("image");
}

Рисунок GetImage(PasteClip& clip)
{
	ЗамкниГип __;
	Рисунок m;
	грузиИзТкст(m, clip.дай("image"));
	return m;
}

Рисунок ReadClipboardImage()
{
	ЗамкниГип __;
	Рисунок m;
	грузиИзТкст(m, Ктрл::gclipboard().дай("image"));
	return m;
}

Ткст GetImageClip(const Рисунок& m, const Ткст& fmt)
{
	Рисунок h = m;
	if(fmt == "image")
		return сохраниКакТкст(h);
	return Null;
}

void AppendClipboardImage(const Рисунок& img)
{
	ЗамкниГип __;
	if(img.пустой()) return;
	Рисунок h = img;
	AppendClipboard("image", сохраниКакТкст(h));
}

void приставь(ВекторМап<Ткст, ClipData>& data, const Рисунок& img)
{
	Рисунок h = img;
	data.добавь("image", сохраниКакТкст(h));
}

bool AcceptFiles(PasteClip& clip)
{
	if(clip.прими("files")) {
		clip.SetAction(DND_COPY);
		return true;
	}
	return false;
}

bool IsAvailableFiles(PasteClip& clip)
{
	return clip.IsAvailable("files");
}

Вектор<Ткст> GetClipFiles(const Ткст& data)
{
	Вектор<Ткст> r;
	Вектор<Ткст> f = разбей(data, '\n');
	for(int i = 0; i < f.дайСчёт(); i++)
		if(f[i].начинаетсяС("file://"))
			r.добавь(UrlDecode(f[i].середина(7)));
	return r;
}

Вектор<Ткст> GetFiles(PasteClip& clip)
{
	ЗамкниГип __;
	return GetClipFiles(clip.дай("files"));
}

void AppendFiles(ВекторМап<Ткст, ClipData>& data, const Вектор<Ткст>& files)
{
	ЗамкниГип __;
	if(files.дайСчёт() == 0)
		return;
	Ткст h;
	for(int i = 0; i < files.дайСчёт(); i++)
		h << "file://" << UrlEncode(files[i]) << '\n';
	data.дайДобавь("files") = h;
}

Ук<Ктрл> Ктрл::sel_ctrl;

void Ктрл::GuiPlatformSelection(PasteClip& d)
{
	d.fmt.очисть();
	d.тип = 2;
}

Ткст Ктрл::RenderPrimarySelection(const Значение& fmt)
{
	return sel_ctrl ? sel_ctrl->GetSelectionData(fmt) : Ткст();
}

void Ктрл::SetSelectionSource(const char *fmts)
{
	ЗамкниГип __;
	LLOG("SetSelectionSource " << РНЦП::Имя(this) << ": " << fmts);
	Вектор<Ткст> s = разбей(fmts, ';');
	sel_ctrl = this;
	for(int i = 0; i < s.дайСчёт(); i++)
		gselection().помести(s[i], ClipData(s[i], RenderPrimarySelection));
}

}

#endif
