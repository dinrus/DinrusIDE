#include "RichText.h"

#define LLOG(x) DLOG(x)

namespace РНЦП {

RichObjectType::RichObjectType() {}
RichObjectType::~RichObjectType() {}

Ткст RichObjectType::GetCreateName() const
{
	return Null;
}

Размер RichObjectType::StdDefaultSize(const Значение& data, Размер maxsize, void * context) const
{
	if(пусто_ли(data)) return Размер(0, 0);
	Размер psz = GetPhysicalSize(data, context);
	if((psz.cx | psz.cy) == 0)
		psz = 625 * GetPixelSize(data, context) / 100;
	Размер sz;
	for(int i = 1; i < 10000; i++) {
		sz = psz / i;
		if(sz.cx <= maxsize.cx && sz.cy <= maxsize.cy)
			break;
	}
	return sz;
}

Размер RichObjectType::GetDefaultSize(const Значение& data, Размер maxsize, void *context) const
{
	return GetDefaultSize(data, maxsize);
}

Размер RichObjectType::GetDefaultSize(const Значение& data, Размер maxsize) const
{
	return StdDefaultSize(data, maxsize, NULL);
}

Значение RichObjectType::читай(const Ткст& s) const
{
	return s;
}

Ткст RichObjectType::пиши(const Значение& v) const
{
	return v;
}

bool RichObjectType::текст_ли() const
{
	return false;
}

bool RichObjectType::прими(PasteClip& clip)
{
	return false;
}

Значение RichObjectType::читай(PasteClip& clip)
{
	return Null;
}

Ткст RichObjectType::GetClipFmts() const
{
	return Null;
}

Ткст RichObjectType::GetClip(const Значение& data, const Ткст& fmt) const
{
	return Null;
}

void RichObjectType::Menu(Бар& bar, RichObject& data, void *context) const
{
	Menu(bar, data);
}

void RichObjectType::Menu(Бар& bar, RichObject& data) const {}

void RichObjectType::DefaultAction(RichObject& data, void *context) const
{
	DefaultAction(data);
}

void RichObjectType::DefaultAction(RichObject& data) const {}

Размер RichObjectType::GetPhysicalSize(const Значение& data, void *context) const
{
	return GetPhysicalSize(data);
}

Размер RichObjectType::GetPhysicalSize(const Значение& data) const { return Размер(0, 0); }

Размер RichObjectType::GetPixelSize(const Значение& data, void *context) const
{ 
	return GetPixelSize(data);
}

Размер RichObjectType::GetPixelSize(const Значение& data) const { return Размер(1, 1); }

void RichObjectType::рисуй(const Значение& data, Draw& w, Размер sz, void *context) const
{
	рисуй(data, w, sz);
}

void RichObjectType::рисуй(const Значение& data, Draw& w, Размер sz) const {}

Рисунок RichObjectType::ToImage(int64 serial_id, const Значение& data, Размер sz) const
{
	return ToImage(serial_id, data, sz, NULL);
}

Рисунок RichObjectType::ToImage(int64, const Значение& data, Размер sz, void *context) const
{
	ImageAnyDraw iw(sz);
	iw.DrawRect(sz, SColorPaper());
	рисуй(data, iw, sz, context);
	return iw;
}

Ткст RichObjectType::GetLink(const Значение& data, Точка pt, Размер sz) const
{
	return GetLink(data, pt, sz, NULL);
}

Ткст RichObjectType::GetLink(const Значение& data, Точка pt, Размер sz, void *context) const
{
	return Null;
}

void RichObject::InitSize(int cx, int cy, void *context)
{
	Размер sz;
	Размер phsz = 600 * GetPixelSize() / 96; // 100% size...
	if(cx || cy)
		sz = дайСоотношение(phsz, cx, cy);
	else
		sz = phsz;
	if(sz.cx > 2000 || sz.cy > 2000)
		sz = sz.cx > sz.cy ? дайСоотношение(phsz, 2000, 0)
		                   : дайСоотношение(phsz, 0, 2000);
	устРазм(sz);
}

ВекторМап<Ткст, RichObjectType *>& RichObject::вКарту()
{
	static ВекторМап<Ткст, RichObjectType *> h;
	return h;
}

void RichObject::NewSerial()
{
	INTERLOCKED {
		static int64 s;
		serial = ++s;
	}
}

void RichObject::регистрируй(const char *имя, RichObjectType *тип)
{
	проверьНаОН();
	вКарту().найдиДобавь(имя, тип);
}

void RichObject::рисуй(Draw& w, Размер sz, void *context) const
{
	if(тип)
		тип->рисуй(data, w, sz, context);
	else {
		w.DrawRect(sz, SColorFace());
		DrawFrame(w, sz, SColorText());
		w.DrawText(2, 2, type_name);
	}
}

Рисунок RichObject::ToImage(Размер sz, void *context) const
{
	if(тип)
		return тип->ToImage(serial, data, sz, context);
	else {
		ImageAnyDraw w(sz);
		рисуй(w, sz, context);
		return w;
	}
}

struct UnknownRichObject : RichObjectType {
	virtual Ткст GetTypeName(const Значение&) const;
};

Ткст UnknownRichObject::GetTypeName(const Значение&) const
{
	return Null;
}

const RichObjectType& RichObject::дайТип() const
{
	if(тип)
		return *тип;
	return Single<UnknownRichObject>();
}

void RichObject::устДанные(const Значение& v)
{
	data = v;
	NewSerial();
}

void RichObject::AdjustPhysicalSize()
{
	if(physical_size.cx == 0 || physical_size.cy == 0)
		physical_size = 600 * pixel_size / 96;
}

void   RichObject::уст(RichObjectType *_type, const Значение& _data, Размер maxsize, void *context)
{
	очисть();
	тип = _type;
	if(тип) {
		data = _data;
		physical_size = тип->GetPhysicalSize(data, context);
		pixel_size = тип->GetPixelSize(data, context);
		size = тип->GetDefaultSize(data, maxsize, context);
		AdjustPhysicalSize();
	}
	NewSerial();
}

bool   RichObject::уст(const Ткст& _type_name, const Значение& _data, Размер maxsize, void *context)
{
	NewSerial();
	type_name = _type_name;
	RichObjectType *t = вКарту().дай(type_name, NULL);
	if(t) {
		уст(t, _data, maxsize, context);
		return true;
	}
	else {
		очисть();
		data = _data;
		physical_size = pixel_size = size = Размер(64, 64);
	}
	return false;
}

bool   RichObject::читай(const Ткст& _type_name, const Ткст& _data, Размер sz, void *context)
{
	type_name = _type_name;
	RichObjectType *t = вКарту().дай(type_name, NULL);
	if(t) {
		очисть();
		тип = t;
		data = тип->читай(_data);
		physical_size = тип->GetPhysicalSize(data, context);
		pixel_size = тип->GetPixelSize(data, context);
		AdjustPhysicalSize();
		size = sz;
		NewSerial();
		return true;
	}
	data = _data;
	physical_size = pixel_size = size = sz;
	NewSerial();
	return false;
}

Ткст RichObject::GetTypeName() const
{
	return тип ? тип->GetTypeName(data) : type_name;
}

void   RichObject::очисть()
{
	serial = 0;
	keepratio = true;
	тип = NULL;
	data = Значение();
	size = physical_size = pixel_size = Размер(0, 0);
	ydelta = 0;
	type_name.очисть();
}

RichObject::RichObject()
{
	очисть();
}

RichObject::RichObject(RichObjectType *тип, const Значение& data, Размер maxsize)
{
	уст(тип, data, maxsize);
}

RichObject::RichObject(const Ткст& тип, const Значение& data, Размер maxsize)
{
	уст(тип, data, maxsize);
}

struct RichObjectTypeDrawingCls : public RichObjectType
{
	virtual Ткст GetTypeName(const Значение&) const;
	virtual Размер   GetPhysicalSize(const Значение& data) const;
	virtual Размер   GetPixelSize(const Значение& data) const;
	virtual void   рисуй(const Значение& data, Draw& w, Размер sz) const;
	virtual Значение  читай(const Ткст& s) const;
	virtual Ткст пиши(const Значение& v) const;

	struct Данные
	{
		void    сериализуй(Поток& stream);

		Чертёж drawing;
		Размер    dot_size;
	};
};

RichObjectType *RichObjectTypeDrawing() { return &Single<RichObjectTypeDrawingCls>(); }

void RichObjectTypeDrawingCls::Данные::сериализуй(Поток& stream)
{
	int version = 1;
	stream % version % dot_size % drawing;
}

Ткст RichObjectTypeDrawingCls::GetTypeName(const Значение&) const
{
	return "Чертёж";
}

RichObject CreateDrawingObject(const Чертёж& dwg, Размер dot_size, Размер out_size)
{
	RichObjectTypeDrawingCls::Данные data;
	data.drawing = dwg;
	data.dot_size = dot_size;
	RichObject obj(RichObjectTypeDrawing(), RawToValue(data));
	if(!пусто_ли(out_size))
		obj.устРазм(out_size);
	return obj;
}

RichObject CreateDrawingObject(const Чертёж& dwg, int cx, int cy)
{
	Размер dsz = dwg.дайРазм();
	return CreateDrawingObject(dwg, dsz, cx || cy ? дайСоотношение(dsz, cx, cy) : dsz);
}

Размер RichObjectTypeDrawingCls::GetPixelSize(const Значение& data) const
{
	if(IsTypeRaw<Данные>(data))
		return ValueTo<Данные>(data).drawing.дайРазм(); // dot_size; TRC 08/04/04
	return Размер(0, 0);
}

Размер RichObjectTypeDrawingCls::GetPhysicalSize(const Значение& data) const
{
	if(IsTypeRaw<Данные>(data))
		return ValueTo<Данные>(data).dot_size;
	return Размер(0, 0);
}

Значение RichObjectTypeDrawingCls::читай(const Ткст& s) const
{
	Данные data;
	ТкстПоток ss(s);
	ss % data;
	if(ss.ошибка_ли())
		return Значение();
	return RawToValue(data);
}

Ткст RichObjectTypeDrawingCls::пиши(const Значение& v) const
{
	if(IsTypeRaw<Данные>(v))
	{
		ТкстПоток ss;
		ss % const_cast<Данные&>(ValueTo<Данные>(v));
		return ss;
	}
	return Null;
}

void RichObjectTypeDrawingCls::рисуй(const Значение& data, Draw& w, Размер sz) const
{
	w.DrawRect(sz, белый);
	if(IsTypeRaw<Данные>(data))
		w.DrawDrawing(Прям(sz), ValueTo<Данные>(data).drawing);
}

RichObject CreatePaintingObject(const Painting& pw, Размер dot_size, Размер out_size)
{
	return CreateDrawingObject(AsDrawing(pw), dot_size, out_size);
}

RichObject CreatePaintingObject(const Painting& pw, int cx, int cy)
{
	return CreateDrawingObject(AsDrawing(pw), cx, cy);
}

ИНИЦБЛОК {
	RichObject::регистрируй("Чертёж", &Single<RichObjectTypeDrawingCls>());
};

struct RichObjectTypePNGCls : public RichObjectType
{
	virtual Ткст GetTypeName(const Значение&) const;
	virtual Размер   GetPhysicalSize(const Значение& data) const;
	virtual Размер   GetPixelSize(const Значение& data) const;
	virtual void   рисуй(const Значение& data, Draw& w, Размер sz) const;
	virtual Значение  читай(const Ткст& s) const;
	virtual Ткст пиши(const Значение& v) const;
};

RichObjectType *RichObjectTypePNG() { return &Single<RichObjectTypePNGCls>(); }

Ткст RichObjectTypePNGCls::GetTypeName(const Значение&) const
{
	return "PING";
}

RichObject CreatePNGObject(const Рисунок& img, Размер dot_size, Размер out_size)
{
	RichObject obj(RichObjectTypePNG(), PNGEncoder().SaveString(img));
	if(!пусто_ли(out_size))
		obj.устРазм(out_size);
	return obj;
}

RichObject CreatePNGObject(const Рисунок& img, int cx, int cy)
{
	Размер dsz = img.дайРазм();
	return CreatePNGObject(img, dsz, cx || cy ? дайСоотношение(dsz, cx, cy) : dsz);
}

Размер RichObjectTypePNGCls::GetPixelSize(const Значение& data) const
{
	if(ткст_ли(data)) {
		ТкстПоток strm(data);
		Один<StreamRaster> ras = StreamRaster::OpenAny(strm);
		if(!!ras)
			return ras->дайРазм();
	}
	return Размер(0, 0);
}

Размер RichObjectTypePNGCls::GetPhysicalSize(const Значение& data) const
{
	if(ткст_ли(data)) {
		ТкстПоток strm(data);
		Один<StreamRaster> ras = StreamRaster::OpenAny(strm);
		if(!!ras)
			return ras->GetInfo().dots;
	}
	return Размер(0, 0);
}

Значение RichObjectTypePNGCls::читай(const Ткст& s) const
{
	return s;
}

Ткст RichObjectTypePNGCls::пиши(const Значение& v) const
{
	return v;
}

void RichObjectTypePNGCls::рисуй(const Значение& data, Draw& w, Размер sz) const
{
	if(ткст_ли(data)) {
		ТкстПоток strm(data);
		Один<StreamRaster> ras = StreamRaster::OpenAny(strm);
		if(!!ras) {
			w.DrawRect(sz, белый);
			w.DrawImage(Прям(sz), ras->GetImage());
			return;
		}
	}
	w.DrawRect(sz, светлоКрасный());
}

ИНИЦБЛОК {
	RichObject::регистрируй("PING", &Single<RichObjectTypePNGCls>());
};

}
