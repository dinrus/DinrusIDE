#include "IconDes.h"

namespace РНЦП {

class AlphaImageInfo
{
public:
	enum ENCODING { COLOR_RLE, MONO_RLE, MONO_PACKED };

	AlphaImageInfo(Размер size = Размер(0, 0), int encoding = COLOR_RLE, Точка hotspot = Точка(0, 0))
		: encoding(encoding), size(size), hotspot(hotspot) {}

	void               сериализуй(Поток& stream);

public:
	int                encoding;
	Размер               size;
	Точка              hotspot;
};

Ткст AlphaToRLE(const Рисунок& aa)
{
	Ткст result;
	for(int y = 0; y < aa.дайВысоту(); y++) {
		result.конкат(PackRLE(aa[y], aa.дайШирину()));
		result.конкат(0x80);
	}
	return result;
}

Рисунок RLEToAlpha(const Ткст& rle, Размер sz)
{
	ImageBuffer ib(sz);
	const byte *s = rle;
	for(int y = 0; y < sz.cy; y++)
		if((const char *)s < rle.стоп())
			s = UnpackRLE(ib[y], s, sz.cx) + 1;
		else
			memset(ib[y], 0, sz.cx * sizeof(КЗСА));
	return ib;
}

void AlphaImageInfo::сериализуй(Поток& stream)
{
	int version = 1;
	stream / version;
	if(version >= 1)
		stream % size % hotspot % encoding;
}

void ScanIML(СиПарсер& parser, Массив<ImlImage>& out_images,
             ВекторМап<Ткст, Ткст>& out_settings)
{
	Ткст имя, bid;
	bool exp = false;
	while(!parser.кф_ли())
	{
		if((bid = parser.читайИд()) == "IMAGE_META")
		{
			parser.сим('(');
			if(parser.ткст_ли())
				имя = parser.читайТкст();
			else
				имя = parser.читайИд();
			parser.передайСим(',');
			Ткст значение = parser.читайТкст();
			parser.передайСим(')');
			out_settings.добавь(имя, значение);
			if(значение == "exp")
				exp = true;
		}
		else if(bid == "IMAGE_BEGIN" && parser.сим('(') && !пусто_ли(имя = parser.читайИд()) && parser.сим(')'))
		{
			Ткст encoded_data;
			out_settings.дайДобавь("wince_16bit", "0");
			Ткст id;
			bool first = true;
			while((id = parser.читайИд()) == "IMAGE_SCAN" && parser.сим('('))
			{
				bool first_in_row = true;
				while(parser.сим_ли('\"'))
				{
					Ткст scan = parser.читай1Ткст();
					if(!first && first_in_row)
						encoded_data.конкат('\x80');
					first_in_row = first = false;
					encoded_data.конкат(scan);
				}
				if(!parser.сим(')'))
					break;
			}
			AlphaImageInfo image;
			bool accepted = false;
			if(parser.сим('(') && parser.читайИд() == имя && parser.сим(',')) {
				if(id == "IMAGE_END"
				&& (image.size.cx = parser.читайЦел()) > 0 && parser.сим(',')
				&& (image.size.cy = parser.читайЦел()) > 0 && parser.сим(')')) {
					accepted = true;
				}
				else if(id == "IMAGE_PACKED" && parser.сим_ли('\"')) {
					Ткст d = parser.читай1Ткст();
					if(parser.сим(')'))
					{
						ТкстПоток ss(d);
						ss % image;
						if(!ss.ошибка_ли())
							accepted = true;
					}
				}
			}
			if(имя.дайДлину() >= 6 && !memcmp(имя, "_java_", 6))
				accepted = false;

			if(accepted) {
				if(имя.дайДлину() >= 4 && !memcmp(имя, "im__", 4))
					имя = Null;

				Рисунок m = RLEToAlpha(encoded_data, image.size);
				ImageBuffer ib(m);
				ib.SetHotSpot(image.hotspot);
				m = ib;
				ImlImage& c = out_images.добавь();
				c.имя = имя;
				c.image = m;
				c.exp = exp;
				exp = false;
			}
		}
		else if(bid == "IMAGE_BEGIN16" && parser.сим('(') && !пусто_ли(имя = parser.читайИд()) && parser.сим(')'))
		{ //TODO: FIX THESE!!!
			out_settings.дайДобавь("wince_16bit", "1");
			Ткст encoded_data;
			Ткст id;
			bool first = true;
			while((id = parser.читайИд()) == "IMAGE_SCAN16" && parser.сим('(') && parser.сим('L'))
			{
				bool first_in_row = true;
				while(parser.сим('\"'))
				{
					СиПарсер::Поз pos = parser.дайПоз();
					const char *end;
					end = pos.ptr; // TODO - remove
					Ткст scan; // TODO = GetUnicodeScan(pos.ptr, &end);
					pos.ptr = end;
					parser.устПоз(pos);
					if(!parser.сим('\"'))
						break;
					if(!first && first_in_row)
						encoded_data.конкат('\x80');
					first_in_row = first = false;
					encoded_data.конкат(scan);
				}
				if(!parser.сим(')'))
					break;
			}
			AlphaImageInfo idata;
			bool accepted = false;
			if(id == "IMAGE_END16" && parser.сим('(') && parser.читайИд() == имя && parser.сим(',')
			&& (idata.size.cx = parser.читайЦел()) > 0 && parser.сим(',')
			&& (idata.size.cy = parser.читайЦел()) > 0 && parser.сим(',')
			&& !пусто_ли(idata.hotspot.x = parser.читайЦел()) && parser.сим(',')
			&& !пусто_ли(idata.hotspot.y = parser.читайЦел()) && parser.сим(')'))
			{
				accepted = true;
			}

			if(accepted)
			{
				if(имя.дайДлину() >= 4 && !memcmp(имя, "im__", 4))
					имя = Null;

				Рисунок m = RLEToAlpha(encoded_data, idata.size);
				ImageBuffer ib(m);
				ib.SetHotSpot(idata.hotspot);
				m = ib;
				ImlImage& c = out_images.добавь();
				c.имя = имя;
				c.image = m;
				c.exp = exp;
				exp = false;
			}
		}
		else
			break;
	}
}

bool LoadIml(const Ткст& data, Массив<ImlImage>& img, int& формат)
{
	СиПарсер p(data);
	формат = 0;
	try {
		bool premultiply = !p.ид("PREMULTIPLIED");
		Вектор<Ткст> имя;
		Вектор<bool> exp;
		while(p.ид("IMAGE_ID")) {
			p.передайСим('(');
			Ткст n;
			if(p.ид_ли()) {
				n = p.читайИд();
				if(n.начинаетсяС("im__", 4))
					n = Null;
				p.передайСим(')');
			}
			else
				while(!p.кф_ли()) {
					if(p.сим(')'))
						break;
					p.пропустиТерм();
				}
			имя.добавь(n);
			bool e = false;
			if(p.ид("IMAGE_META")) {
				p.передайСим('(');
				e = p.читайТкст() == "exp";
				if(p.сим(',') && p.ткст_ли())
					p.читайТкст();
				p.передайСим(')');
			}
			exp.добавь(e);
		}
		int ii = 0;
		while(p.ид("IMAGE_BEGIN_DATA")) {
			Ткст data;
			while(p.ид("IMAGE_DATA")) {
				p.передайСим('(');
				for(int j = 0; j < 32; j++) {
					if(j) p.передайСим(',');
					data.конкат(p.читайЦел());
				}
				p.передайСим(')');
			}
			p.передайИд("IMAGE_END_DATA");
			p.передайСим('(');
			int zlen = p.читайЦел();
			p.передайСим(',');
			int count = p.читайЦел();
			p.передайСим(')');

			data.обрежь(zlen);
			Вектор<ImageIml> m = UnpackImlData(data, data.дайСчёт());
			if(m.дайСчёт() != count || ii + count > имя.дайСчёт())
				p.выведиОш("");
			for(int i = 0; i < count; i++) {
				ImlImage& c = img.добавь();
				(ImageIml &)c = m[i];
				c.имя = имя[ii];
				c.exp = exp[ii++];
				c.имя.обрежьКонец("__DARK");
				c.имя.обрежьКонец("__UHD");
				if(premultiply)
					c.image = Premultiply(c.image);
			}
		}
		if(!p.кф_ли())
			p.выведиОш("");
	}
	catch(СиПарсер::Ошибка) {
		try {
			СиПарсер p(data);
			Массив<ImlImage> m;
			ВекторМап<Ткст, Ткст> s;
			ScanIML(p, img, s);
			if(img.дайСчёт())
				формат = 1;
		}
		catch(...) {
			return false;
		}
	}
	return true;
}

static void PutOctalString(Поток& out, const char *b, const char *e, const Ткст& eol, bool split = false)
{
	out.помести('\"');
	int64 start = out.дайПоз();
	while(b < e) {
		if(split && out.дайПоз() >= start + 200u) {
			out << "\"" << eol << "\t\"";
			start = out.дайПоз();
		}
		if((byte)*b >= ' ' && *b != '\x7F' && *b != '\xFF') {
			if(*b == '\\' || *b == '\"' || *b == '\'')
				out.помести('\\');
			out.помести(*b++);
		}
		else if(цифра_ли(b[1]))
			out.помести(спринтф("\\%03o", (byte)*b++));
		else
			out.помести(спринтф("\\%o", (byte)*b++));
	}
	out.помести('\"');
}

Ткст SaveIml(const Массив<ImlImage>& iml, int формат, const Ткст& eol) {
	ТкстПоток out;
	if(формат == 1) {
		for(int i = 0; i < iml.дайСчёт(); i++) {
			const ImlImage& c = iml[i];
			if(c.exp)
				out << "IMAGE_META(\"exp\", \"\")" << eol;
			Ткст имя = c.имя;
			Рисунок буфер = c.image;
			if(пусто_ли(имя))
				имя = "im__" + целТкт(i);
			out.PutLine(фмт("IMAGE_BEGIN(%s)", имя));
			int last = 0;
			for(int i = 0; i < буфер.дайВысоту(); i++) {
				Ткст scan = PackRLE(буфер[i], буфер.дайШирину());
				if(!scan.пустой() || i == 0) // force at least 1 scan
				{
					for(; last < i; last++)
						out.PutLine("\tIMAGE_SCAN(\"\")");
					out.помести("\tIMAGE_SCAN(");
					PutOctalString(out, scan.старт(), scan.стоп(), eol, true);
					out << ")" << eol;
					last = i + 1;
				}
			}
			out.помести("IMAGE_PACKED(");
			out.помести(имя);
			out.помести(", ");
			ТкстПоток datastrm;
			Размер size = буфер.дайРазм();
			Точка hotspot = буфер.GetHotSpot();
			int encoding = AlphaImageInfo::COLOR_RLE;
			int version = 1;
			datastrm / version;
			datastrm % size % hotspot % encoding;
			ПРОВЕРЬ(!datastrm.ошибка_ли());
			Ткст s = datastrm.дайРез();
			PutOctalString(out, s.старт(), s.стоп(), eol);
			out << ")" << eol;
		}
	}
	else {
		out << "PREMULTIPLIED" << eol;
		Индекс<Ткст> std_name;
		for(int i = 0; i < iml.дайСчёт(); i++) {
			const ImlImage& c = iml[i];
			if(c.image.GetResolution() == IMAGE_RESOLUTION_STANDARD)
				std_name.добавь(c.имя);
		}
		for(int i = 0; i < iml.дайСчёт(); i++) {
			const ImlImage& c = iml[i];
			out << "IMAGE_ID(" << c.имя;
			if(c.flags & IML_IMAGE_FLAG_UHD)
				out << "__UHD";
			if(c.flags & IML_IMAGE_FLAG_DARK)
				out << "__DARK";
			out << ")";
			if(c.exp)
				out << " IMAGE_META(\"exp\", \"\")" << eol;
			out << eol;
		}
		int ii = 0;
		while(ii < iml.дайСчёт()) {
			int bl = 0;
			int bn = 0;
			Вектор<ImageIml> bimg;
			while(bl < 4096 && ii < iml.дайСчёт()) {
				const ImlImage& c = iml[ii++];
				ImageIml& m = bimg.добавь();
				m.image = c.image;
				m.flags = c.flags;
				if(c.flags & IML_IMAGE_FLAG_UHD)
					SetResolution(m.image, IMAGE_RESOLUTION_UHD);
				if(c.flags & (IML_IMAGE_FLAG_FIXED|IML_IMAGE_FLAG_FIXED_SIZE))
					SetResolution(m.image, IMAGE_RESOLUTION_NONE);
				bl += c.image.дайДлину();
				bn++;
			}
			Ткст bs = PackImlData(bimg);
			out << eol << "IMAGE_BEGIN_DATA" << eol;
			bs.конкат(0, ((bs.дайСчёт() + 31) & ~31) - bs.дайСчёт());
			const byte *s = bs;
			for(int n = bs.дайСчёт() / 32; n--;) {
				out << "IMAGE_DATA(";
				for(int j = 0; j < 32; j++) {
					if(j) out << ',';
					out << (int)*s++;
				}
				out << ")" << eol;
			}
			out << "IMAGE_END_DATA(" << bs.дайСчёт() << ", " << bn << ")" << eol;
		}
	}
	return out.дайРез();
}

}
