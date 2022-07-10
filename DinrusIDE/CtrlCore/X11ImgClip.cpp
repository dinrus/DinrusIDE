#include "CtrlCore.h"

#ifdef GUI_X11

#include <plugin/bmp/bmp.h>

namespace РНЦП {

const char *ClipFmtsImage()
{
	static const char *q;
	ONCELOCK {
		static Ткст s(ClipFmt<Рисунок>() + ";image/png");
		q = s;
	}
	return q;
}

bool AcceptImage(PasteClip& clip)
{
	return clip.прими(ClipFmt<Рисунок>()) || clip.прими("image/png");
}

Рисунок GetImage(PasteClip& clip)
{
	Рисунок m;
	if(прими<Рисунок>(clip)) {
		грузиИзТкст(m, ~clip);
		if(!m.пустой())
			return m;
	}
	if(clip.прими("image/png"))
		return StreamRaster::LoadStringAny(~clip);
	return Null;
}

Рисунок ReadClipboardImage()
{
	return GetImage(Ктрл::Clipboard());
}

static Ткст sBmp(const Значение& data)
{
	Рисунок img = data;
	return BMPEncoder().SaveString(img);
}

static Ткст sImg(const Значение& data)
{
	Рисунок img = data;
	return сохраниКакТкст(const_cast<Рисунок&>(img));
}

Ткст GetImageClip(const Рисунок& img, const Ткст& fmt)
{
	if(img.пустой())
		return Null;
	if(fmt == "image/bmp")
		return BMPEncoder().SaveString(img);
	if(fmt == ClipFmt<Рисунок>())
		return сохраниКакТкст(const_cast<Рисунок&>(img));
	return Null;
}

void AppendClipboardImage(const Рисунок& img)
{
	if(img.пустой()) return;
	AppendClipboard(ClipFmt<Рисунок>(), img, sImg);
	AppendClipboard("image/bmp", img, sBmp);
}

void приставь(ВекторМап<Ткст, ClipData>& data, const Рисунок& img)
{
	data.добавь(ClipFmt<Рисунок>(), ClipData(img, sImg));
	data.добавь("image/bmp", ClipData(img, sBmp));
}

}

#endif
