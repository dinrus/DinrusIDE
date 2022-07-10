#include "RichText.h"

namespace РНЦП {

Ткст HtmlFontStyle(Шрифт f, Шрифт base)
{
	Ткст style;
	if(f.GetFace() != base.GetFace())
		switch(f.GetFace()) {
		case Шрифт::ARIAL: style = "font-family:sans-serif;"; break;
		case Шрифт::ROMAN: style = "font-family:serif;"; break;
		case Шрифт::COURIER: style = "font-family:monospace;"; break;
		}
	if(f.дайВысоту() != base.дайВысоту())
		style << спринтф("font-size:%dpt;", f.дайВысоту() * 72 / 600);
	if(f.IsBold() != base.IsBold())
		style << (f.IsBold() ? "font-weight:bold;" : "font-weight:normal;");
	if(f.IsItalic() != base.IsItalic())
		style << (f.IsItalic() ? "font-style:italic;" : "font-style:normal;");
	if(f.IsUnderline() != base.IsUnderline())
		style << (f.IsUnderline() ? "text-decoration:underline;" : "text-decoration:none;");
	return style;
}

Ткст HtmlFontStyle(Шрифт f)
{
	Ткст style;
	switch(f.GetFace()) {
		case Шрифт::ARIAL: style = "font-family:sans-serif;"; break;
		case Шрифт::ROMAN: style = "font-family:serif;"; break;
		case Шрифт::COURIER: style = "font-family:monospace;"; break;
	}
	style << спринтф("font-size:%dpt;", f.дайВысоту() * 72 / 600);
	style << (f.IsBold() ? "font-weight:bold;" : "font-weight:normal;");
	style << (f.IsItalic() ? "font-style:italic;" : "font-style:normal;");
	style << (f.IsUnderline() ? "text-decoration:underline;" : "text-decoration:none;");
	return style;
}

Ткст HtmlDot(int q, Zoom z)
{
	return Ткст().конкат() << ' ' << z * q << "px";
}

Ткст HtmlDotl(int q, Zoom z)
{
	return Ткст().конкат() << ' ' << max((int)!!q, z * q) << "px";
}

Ткст HtmlStyleColor(Цвет c, const char *cl = "color")
{
	return фмт(Ткст(cl) + ":#%02x%02x%02x;", c.дайК(), c.дайЗ(), c.дайС());
}

Ткст HtmlCharStyle(const RichPara::CharFormat& cf, const RichPara::CharFormat& sf)
{
	Ткст style;
	if(cf.ink != sf.ink)
		style = HtmlStyleColor(cf.ink);
	return style + HtmlFontStyle(cf, sf);
}

Ткст HtmlParaStyle(const RichPara::фмт& f, Zoom z)
{
	Ткст style;
	int lm = z * f.lm;
	if(f.bullet && f.bullet != RichPara::BULLET_TEXT) {
		style << "дисплей:list-элт;list-style-тип:";
		switch(f.bullet) {
		case RichPara::BULLET_ROUND: style << "disc"; break;
		case RichPara::BULLET_ROUNDWHITE: style << "circle"; break;
		case RichPara::BULLET_BOX:
		case RichPara::BULLET_BOXWHITE: style << "square"; break;
		}
		style << ';';
//		style << ";list-style-position:inside;";
		lm += 20;
	}
	style << фмт("margin:%d`px %d`px %d`px %d`px;text-indent:%d`px;",
	                z * f.before,
	                z * f.rm,
	                z * f.after,
	                lm,
	                z * (f.bullet ? 0 : f.indent)
	         );
	style << "text-align:";
	switch(f.align) {
	case ALIGN_LEFT: style << "left;"; break;
	case ALIGN_RIGHT: style << "right;"; break;
	case ALIGN_CENTER: style << "center;"; break;
	case ALIGN_JUSTIFY: style << "justify;"; break;
	}
	style << HtmlStyleColor(f.ink) + HtmlFontStyle(f);
	if(!пусто_ли(f.paper))
		style << HtmlStyleColor(f.paper, "background-color");
	style << decode(f.linespacing, RichPara::LSP15, "line-height:150%",
	                               RichPara::LSP20, "line-height:200%",
	                               RichPara::LSP115, "line-height:115%",
	                               "");
	return style;
}

Ткст FormatClass(Индекс<Ткст>& css, const Ткст& fmt)
{
	return " class=\"" + фмтЦелАльфа(css.найдиДобавь(fmt) + 1) + "\"";
}

void TabBorder(Ткст& style, const char *txt, int border, Цвет bordercolor, const RichTable::фмт& tf, Zoom z)
{
	style << "border-" << txt << ':' << HtmlDotl(border + tf.grid, z) << " solid "
	      << цветВГЯР(border ? bordercolor : tf.gridcolor) << ';';
}

Ткст AsHtml(const RichTxt& text, const RichStyles& styles, Индекс<Ткст>& css,
              const ВекторМап<Ткст, Ткст>& links,
              const ВекторМап<Ткст, Ткст>& labels,
              Zoom z, const ВекторМап<Ткст, Ткст>& escape,
              HtmlObjectSaver& object_saver)
{
	Ткст html;
	for(int i = 0; i < text.GetPartCount(); i++) {
		if(text.IsTable(i)) {
			const RichTable& t = text.GetTable(i);
			const RichTable::фмт& tf = t.дайФормат();
			int nx = tf.column.дайСчёт();
			int ny = t.GetRows();
			html << "<table width=\"100%\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\">";
			if(tf.before > 0)
				html << "<tr><td height=" << HtmlDot(tf.before, z) << " colspan=\"3\"></td></tr>";
			html << "<tr>";
			if (tf.lm > 0)
				html << "<td><table border=\"0\" width=" << HtmlDot(tf.lm, z) << "><tr><td></td></tr></table></td>\r\n";
			html <<	"<td width=\"100%\">";

			Ткст style;
			style << "border-collapse:collapse;table-layout:auto;"
			      << "border:" << HtmlDotl(tf.frame, z) << " solid " << цветВГЯР(tf.framecolor) << ';';

			html << "<table width=\"100%\"" << FormatClass(css, style) << ">";
			int sum = 0;
			for(int i = 0; i < nx; i++)
				sum += tf.column[i];
			html << "<colgroup>";
			for(int i = 0; i < nx; i++)
				html << "<col width=\"" << 100 * tf.column[i] / sum << "%\">";
			html << "</colgroup>";
			html << "\r\n";
			for(int i = 0; i < ny; i++) {
				const Массив<RichCell>& r = t[i];
				html << "<tr>";
				for(int j = 0; j < r.дайСчёт(); j++) {
					if(t(i, j)) {
						const RichCell& c = r[j];
						const RichCell::фмт& cf = c.формат;
						Ткст style;
						style << "padding:" << HtmlDot(cf.margin.top, z) << HtmlDot(cf.margin.right, z)
						                    << HtmlDot(cf.margin.bottom, z) << HtmlDot(cf.margin.left, z) << ';';
						TabBorder(style, "left", cf.border.left, cf.bordercolor, tf, z);
						TabBorder(style, "top", cf.border.top, cf.bordercolor, tf, z);
						TabBorder(style, "right", cf.border.right, cf.bordercolor, tf, z);
						TabBorder(style, "bottom", cf.border.bottom, cf.bordercolor, tf, z);
						style << "background-color:" << цветВГЯР(cf.color) << ';';
						style << "vertical-align:";
						switch(cf.align) {
						case ALIGN_TOP:    style << "top"; break;
						case ALIGN_CENTER: style << "middle"; break;
						case ALIGN_BOTTOM: style << "bottom"; break;
						}
						style << ';';
						html << "<td" << FormatClass(css, style);
						if(c.hspan)
							html << " colspan=" << c.hspan + 1;
						if(c.vspan)
							html << " rowspan=" << c.vspan + 1;
						html << '>';
						html << AsHtml(c.text, styles, css, links, labels, z, escape, object_saver);
						html << "</td>\r\n";
					}
				}
				html << "</tr>\r\n";
			}
			html << "</table></td>\r\n";
			if (tf.rm > 0)
				html << "<td><table border=\"0\" width=" << HtmlDot(tf.rm, z) << "><tr><td></td></tr></table></td>";
			if(tf.after > 0)
				html << "<tr><td height=" << HtmlDot(tf.after, z) << " colspan=\"3\"></td></tr>";
			html << "</tr></table>\r\n";
		}
		else
		if(text.IsPara(i)) {
			RichPara p = text.дай(i, styles);
			if(p.формат.ruler)
				html << "<hr>";
			bool bultext = false;
			if(p.формат.bullet == RichPara::BULLET_TEXT)
				for(int i = 0; i < p.part.дайСчёт(); i++) {
					const RichPara::Part& part = p.part[i];
					if(part.text.найди(9) >= 0) {
						bultext = true;
						break;
					}
				}
			if(bultext) {
				html << "<table width=\"100%\" border=\"0\" "
				        "cellpadding=\"2\" cellspacing=\"2\">"
				        "<tr>";
				int q = z * p.формат.lm - 8;
				if(q > 0)
					html << фмт("<td width=\"%d\"></td>", q);
				html << фмт("<td valign=\"top\" width=\"%d\" bgcolor=\"#F0F0F0\">\r\n",
				               max(z * p.формат.indent, 0));
				p.формат.ruler = p.формат.after = p.формат.before = p.формат.indent = p.формат.lm = 0;
			}
			Ткст par = "<p";
			Ткст lbl = labels.дай(p.формат.label, p.формат.label);
			if(lbl.дайСчёт())
				par << " id=\"" << lbl << "\"";
			par << FormatClass(css, HtmlParaStyle(p.формат, z)) << ">";
			html << par;
			for(int i = 0; i < p.part.дайСчёт(); i++) {
				const RichPara::Part& part = p.part[i];
				int q;
				Ткст lnk = part.формат.link;
				if(lnk.дайСчёт()) {
					int q = links.найди(lnk);
					if(q < 0) {
						int q = lnk.найдирек('#');
						if(q >= 0) {
							Ткст l = lnk.лево(q);
							lnk = links.дай(l, l) + '#' + lnk.середина(q + 1);
						}
					}
					else
						lnk = links[q];
				}
				if(part.object)
					html << object_saver.GetHtml(part.object, lnk);
				else
				if(part.формат.indexentry.дайСчёт() &&
				   (q = escape.найди(part.формат.indexentry.вТкст())) >= 0)
					html << escape[q];
				else {
					Ткст endtag;
					if(!lnk.пустой() && lnk[0] != ':') {
						html << "<a href=\"" << lnk << "\">";
						endtag = "</a>";
					}
					Ткст cs;
					cs = HtmlCharStyle(part.формат, p.формат);
					if(!cs.пустой()) {
						html << "<span" << FormatClass(css, cs) << ">";
						endtag = "</span>" + endtag;
					}
					if(part.формат.sscript == 1) {
						html << "<sup>";
						endtag = "</sup>" + endtag;
					}
					if(part.формат.sscript == 2) {
						html << "<sub>";
						endtag = "</sub>" + endtag;
					}
					if(part.формат.IsStrikeout()) {
						html << "<strike>";
						endtag = "</strike>" + endtag;
					}
					if(part.формат.capitals) {
						html << "<span style=\"font-variant: small-caps;\">";
						endtag << "</span>";
					}
					bool spc = false;
					const wchar *end = part.text.стоп();
					for(const wchar *s = part.text.старт(); s != end; s++) {
						if(*s == ' ') {
							html.конкат(spc ? "&nbsp;" : " ");
							spc = true;
						}
						else {
							spc = false;
							if(*s == 160)  html.конкат("&nbsp;");
							else
							if(*s == '<')  html.конкат("&lt;");
							else
							if(*s == '>')  html.конкат("&gt;");
							else
							if(*s == '&')  html.конкат("&amp;");
							else
							if(*s == '\"') html.конкат("&quot;");
							else
							if(*s == 9) {
								if(bultext) {
									if(!cs.пустой() && part.text[0] != 9)
										html << "</span>";
									html << "</p>";
									html << "</td>\r\n<td valign=\"top\" bgcolor=\"#F0F0F0\">\r\n";
									html << par;
									if(s[1]) {
										cs = HtmlCharStyle(part.формат, p.формат);
										if(!cs.пустой())
											html << "<span" << FormatClass(css, cs) << ">";
									}
								}
								else
									html.конкат("&nbsp;&nbsp;&nbsp;&nbsp;");
							}
							else
								html.конкат(вУтф8(*s));
						}
					}
					html << endtag;
				}
			}
			if(p.part.дайСчёт() == 0)
				html << "&nbsp;";
			html << "</p>";
			if(bultext)
				html << "</td></tr></table>";
			html << "\r\n";
		}
	}
	return html;
}

class DefaultHtmlObjectSaver : public HtmlObjectSaver
{
public:
	DefaultHtmlObjectSaver(const Ткст& outdir_, const Ткст& namebase_, int imtolerance_, Zoom z_)
	: outdir(outdir_), namebase(namebase_), z(z_), imtolerance(imtolerance_), im(0) {}

	virtual Ткст GetHtml(const RichObject& object, const Ткст& link);

private:
	Ткст outdir;
	Ткст namebase;
	Zoom z;
	int imtolerance;
	int im;
};

Ткст DefaultHtmlObjectSaver::GetHtml(const RichObject& object, const Ткст& link)
{
	ТкстБуф html;
	Ткст имя;
	имя << namebase << "_" << im++ << ".png";
	Размер psz = object.GetPixelSize();
	Размер sz = z * object.дайРазм();
	if(абс(100 * (psz.cx - sz.cx) / sz.cx) < imtolerance)
		sz = psz;
	PNGEncoder png;
	png.сохраниФайл(приставьИмяф(outdir, имя), object.ToImage(psz));
	Ткст el = "</a>";
	if(пусто_ли(link)) {
		if(psz.cx * psz.cy != 0)
			html << "<a href=\"" << имя << "\">";
		else
			el.очисть();
	}
	else
		html << "<a href=\"" << link << "\">";
	html << фмт("<img src=\"%s\" border=\"0\" alt=\"\" style=\"width: %d`px; height: %d`px\">",
	               имя, sz.cx, sz.cy);
	html << el;
	return Ткст(html);
}

Ткст EncodeHtml(const RichText& text, Индекс<Ткст>& css,
                  const ВекторМап<Ткст, Ткст>& links,
                  const ВекторМап<Ткст, Ткст>& labels,
                  const Ткст& outdir, const Ткст& namebase, Zoom z,
                  const ВекторМап<Ткст, Ткст>& escape, int imt)
{
	DefaultHtmlObjectSaver default_saver(outdir, namebase, imt, z);
	return AsHtml(text, text.GetStyles(), css, links, labels, z, escape, default_saver);
}

Ткст EncodeHtml(const RichText& text, Индекс<Ткст>& css,
                  const ВекторМап<Ткст, Ткст>& links,
                  const ВекторМап<Ткст, Ткст>& labels,
                  HtmlObjectSaver& object_saver, Zoom z,
                  const ВекторМап<Ткст, Ткст>& escape)
{
	return AsHtml(text, text.GetStyles(), css, links, labels, z, escape, object_saver);
}

Ткст AsCss(Индекс<Ткст>& ss)
{
	Ткст css;
	for(int i = 0; i < ss.дайСчёт(); i++) {
		css << "." + фмтЦелАльфа(i + 1);
		css << "{" << ss[i] << "}\r\n";
	}
	return css;
}

Ткст MakeHtml(const char *title, const Ткст& css, const Ткст& body)
{
	Ткст h =
		"<HTML>\r\n"
	    "<HEAD>\t\n"
	    "<META HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html; charset=utf-8\">\t\n"
	    "<META NAME=\"Generator\" CONTENT=\"U++ HTML Пакет\">\t\n"
	    "<TITLE>" + Ткст(title) + "</TITLE>\r\n"
	;
	if(!пусто_ли(css))
		h << "<STYLE TYPE=\"text/css\"><!--\r\n"
		  << css << "\r\n-->\r\n</STYLE>\r\n";
	h << "</HEAD><BODY>" << body << "</BODY>";
	return h;
}

}
