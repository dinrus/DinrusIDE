#include <Draw/Draw.h>
#include <setjmp.h>
#include "jpg.h"
#define HAVE_BOOLEAN
#define boolean int
#undef FAR
extern "C" {
#include "lib/jinclude.h"
#include "lib/jpeglib.h"
#include "lib/jerror.h"
}
#undef XINT32


namespace РНЦП {

#define LLOG(x)  // LOG(x)

static void NoOutput(j_common_ptr cinfo)
{
}

enum 
{
	STREAM_BUF_SIZE = 16384,
	ROW_BUF_SIZE    = 16384,
};

enum {
	EXIF_BYTE      =  1, // An 8-bit unsigned integer
	EXIF_ASCII     =  2, // An 8-bit byte containing one 7-bit ASCII code. The final byte is terminated with NULL
	EXIF_SHORT     =  3, // A 16-bit (2-byte) unsigned integer
	EXIF_LONG      =  4,  // A 32-bit (4-byte) unsigned integer
	EXIF_RATIONAL  =  5, // Two LONGs. The first LONG is the numerator and the second LONG expresses the denominator
	EXIF_UNDEFINED =  7, // An 8-bit byte that can take any значение depending on the field definition
	EXIF_SLONG     =  9, // A 32-bit (4-byte) signed integer (2's complement notation)
	EXIF_SRATIONAL = 10, // Two SLONGs. The first SLONG is the numerator and the second SLONG is the denominator
};

struct jpg_stream_destination_mgr
{
	jpeg_destination_mgr  pub;
	Поток               *stream;
	JOCTET               *буфер;
};

typedef jpg_stream_destination_mgr *stream_dest_ptr;

static void init_destination(j_compress_ptr cinfo)
{
	stream_dest_ptr dest = (stream_dest_ptr)cinfo->dest;
	dest->буфер = (JOCTET *)
		(*cinfo->mem->alloc_small)((j_common_ptr)cinfo, JPOOL_IMAGE,
			STREAM_BUF_SIZE * sizeof(JOCTET));
	dest->pub.next_output_byte = dest->буфер;
	dest->pub.free_in_buffer = STREAM_BUF_SIZE;
}

static boolean empty_output_buffer(j_compress_ptr cinfo)
{
	stream_dest_ptr dest = (stream_dest_ptr)cinfo->dest;
	dest->stream->помести(dest->буфер, STREAM_BUF_SIZE * sizeof(JOCTET));
	dest->pub.next_output_byte = dest->буфер;
	dest->pub.free_in_buffer = STREAM_BUF_SIZE;
	return true;
}

static void term_destination(j_compress_ptr cinfo)
{
	stream_dest_ptr dest = (stream_dest_ptr)cinfo->dest;
	size_t done = STREAM_BUF_SIZE - dest->pub.free_in_buffer;
	if(done > 0)
		dest->stream->помести(dest->буфер, (dword)(done * sizeof(JOCTET)));
}

static void jpeg_stream_dest(j_compress_ptr cinfo, Поток& stream)
{
	if(cinfo->dest == NULL)
		cinfo->dest = (jpeg_destination_mgr *)
			(*cinfo->mem->alloc_small)((j_common_ptr)cinfo, JPOOL_PERMANENT,
				sizeof(jpg_stream_destination_mgr));

	stream_dest_ptr dest = (stream_dest_ptr)cinfo->dest;
	dest->pub.init_destination    = init_destination;
	dest->pub.empty_output_buffer = empty_output_buffer;
	dest->pub.term_destination    = term_destination;
	dest->pub.next_output_byte    = NULL;
	dest->pub.free_in_buffer      = 0;
	dest->stream                  = &stream;
	dest->буфер                  = NULL;
}

struct jpg_stream_source_mgr
{
	jpeg_source_mgr pub;
	Поток         *stream;
	JOCTET         *буфер;
	bool            start_of_file;
};

typedef jpg_stream_source_mgr *stream_src_ptr;

static void init_source(j_decompress_ptr cinfo)
{
	stream_src_ptr src = (stream_src_ptr)cinfo->src;
	src->start_of_file = true;
}

static boolean fill_input_buffer(j_decompress_ptr cinfo)
{
	stream_src_ptr src = (stream_src_ptr)cinfo->src;
	size_t nbytes = src->stream->дай(src->буфер, STREAM_BUF_SIZE * sizeof(JOCTET));
	if(nbytes == 0)
	{
		if(src->start_of_file)
			ERREXIT(cinfo, JERR_INPUT_EMPTY);
		WARNMS(cinfo, JWRN_JPEG_EOF);
		/* вставь a fake EOI marker */
		src->буфер[0] = (JOCTET) 0xFF;
		src->буфер[1] = (JOCTET) JPEG_EOI;
		nbytes = 2;
	}

	src->pub.next_input_byte = src->буфер;
	src->pub.bytes_in_buffer = nbytes;
	src->start_of_file       = false;

	return true;
}

static void skip_input_data(j_decompress_ptr cinfo, long num_bytes)
{
	stream_src_ptr src = (stream_src_ptr)cinfo->src;
	int above = (int)(num_bytes - src->pub.bytes_in_buffer);
	if(above < 0)
	{ // we're still within the input буфер
		src->pub.next_input_byte += num_bytes;
		src->pub.bytes_in_buffer = -above;
	}
	else
	{
		src->stream->перейди(src->stream->дайПоз() + above);
		src->start_of_file = false;
		src->pub.next_input_byte = NULL;
		src->pub.bytes_in_buffer = 0;
	}
}

static void term_source(j_decompress_ptr cinfo)
{
	stream_src_ptr src = (stream_src_ptr)cinfo->src;
	src->pub.next_input_byte = NULL;
	src->pub.bytes_in_buffer = 0;
}

static void jpeg_stream_src(j_decompress_ptr cinfo, Поток& stream)
{
	stream_src_ptr src = (stream_src_ptr)cinfo->src;
	if(src == NULL)
	{
		cinfo->src = (jpeg_source_mgr *)
			(*cinfo->mem->alloc_small)((j_common_ptr)cinfo, JPOOL_PERMANENT,
				sizeof(jpg_stream_source_mgr));
		src = (stream_src_ptr)cinfo->src;
		src->буфер = (JOCTET *)(*cinfo->mem->alloc_small)((j_common_ptr)cinfo,
			JPOOL_PERMANENT, STREAM_BUF_SIZE * sizeof(JOCTET));
	}

	src->pub.init_source       = init_source;
	src->pub.fill_input_buffer = fill_input_buffer;
	src->pub.skip_input_data   = skip_input_data;
	src->pub.resync_to_restart = jpeg_resync_to_restart;
	src->pub.term_source       = term_source;
	src->pub.bytes_in_buffer   = 0;
	src->pub.next_input_byte   = NULL;
	src->stream                = &stream;
}

struct jpeg_longjmp_error_mgr : public jpeg_error_mgr {
	jmp_buf jmpbuf;
};

static void error_exit(j_common_ptr cinfo)
{
	(*cinfo->err->output_message)(cinfo);
	jpeg_longjmp_error_mgr *jlem = (jpeg_longjmp_error_mgr *)cinfo->err;
	longjmp(jlem->jmpbuf, 1);
}

class JPGRaster::Данные {
	friend class JPGRaster;

public:
	Данные(JPGRaster& owner);
	~Данные();

	bool создай(Поток& stream);
	void строй();
	void освободи();
	Raster::Info GetInfo();
	Raster::Строка дайСтроку(int line);
	void ScanMetaData();
	void ScanExifData(const char *begin, const char *end);
	Значение GetMetaData(Ткст id);
	void EnumMetaData(Вектор<Ткст>& id_list);
	Ткст GetThumbnail();

private:
	bool иниц();

	int     Exif16(const char *s, const char *end) { return s + 2 < end ? exif_big_endian ? Peek16be(s) : Peek16le(s) : 0; }
	int     Exif32(const char *s, const char *end) { return s + 4 < end ? exif_big_endian ? Peek32be(s) : Peek32le(s) : 0; }
	double  ExifF5(const char *s, const char *end);

	enum IFD_TYPE { BASE_IFD, GPS_IFD };
	int  ExifDir(const char *begin, const char *end, int offset, IFD_TYPE тип);

private:
	JPGRaster& owner;
	Поток *stream;
	int64 stream_fpos;

	RasterFormat формат;
	КЗСА palette[256];

	ВекторМап<Ткст, Значение> metadata;

	jpeg_decompress_struct   cinfo;
	jpeg_longjmp_error_mgr  *jerr;

	Размер size;
	Размер dot_size;
	int row_bytes;
	int row_bytes_4;
	bool finish;
	bool exif_big_endian;

	int next_line;
};

void JPGRaster::Данные::строй()
{
	stream = NULL;
	size = dot_size = Null;
	next_line = 0;
	finish = false;

	jpeg_create_decompress(&cinfo);
}

JPGRaster::Данные::Данные(JPGRaster& owner_)
: owner(owner_)
{
	jerr = (jpeg_longjmp_error_mgr *)malloc(sizeof(jpeg_longjmp_error_mgr));
	строй();
}

void JPGRaster::Данные::освободи()
{
	if(setjmp(jerr->jmpbuf))
		return;
	if(finish)
		jpeg_abort_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
}

JPGRaster::Данные::~Данные()
{
	освободи();
	free(jerr);
}

bool JPGRaster::Данные::создай(Поток& stream_)
{
	stream = &stream_;
	stream_fpos = stream->дайПоз();
	cinfo.err = jpeg_std_error(jerr);
	cinfo.err->output_message = &NoOutput;
	cinfo.dct_method = JDCT_IFAST;
	jerr->error_exit = error_exit;

	return иниц();
}

void JPGRaster::Данные::ScanMetaData()
{
	if(!metadata.пустой())
		return;
	for(jpeg_saved_marker_ptr p = cinfo.marker_list; p; p = p->next) {
		Ткст data(p->data, p->data_length);
		Ткст ключ;
		if(p->marker == JPEG_COM)
			ключ = "COM";
		else if(p->marker >= JPEG_APP0 && p->marker <= JPEG_APP0 + 15) {
			ключ = фмт("APP%d", p->marker - JPEG_APP0);
			if(p->marker == JPEG_APP0 + 1 && !memcmp(data, "Exif\0\0", 6))
				ScanExifData(data.дайОбх(6), data.end());
		}
		if(metadata.найди(ключ) >= 0) {
			for(int i = 1;; i++) {
				Ткст suffix;
				suffix << ключ << '$' << i;
				if(metadata.найди(suffix) < 0) {
					ключ = suffix;
					break;
				}
			}
		}
		metadata.добавь(ключ, data);
	}
}

double JPGRaster::Данные::ExifF5(const char *s, const char *end)
{
	unsigned num = Exif32(s + 0, end);
	unsigned den = Exif32(s + 4, end);
	return num / (double)(den ? den : 1);
}

int JPGRaster::Данные::ExifDir(const char *begin, const char *end, int offset, IFD_TYPE тип)
{
	const char *e = begin + offset;
	int nitems = Exif16(e, end);
//	puts(фмтЧ("directory %08x: %d items", dir, nitems));
	e += 2;
	for(int i = 0; i < nitems; i++, e += 12) {
		int tag = Exif16(e, end);
		int fmt = Exif16(e + 2, end);
		int count = Exif32(e + 4, end);
		static const int fmtlen[] = {
			1, 1, 2, 4, 8, 1, 1, 2, 4, 8, 4, 8
		};
		int len = 0;
		if(fmt > 0 && fmt <= __countof(fmtlen))
			len = fmtlen[fmt - 1] * count;
		const char *data = e + 8;
		if(len > 4)
			data = begin + Exif32(data, end);
//		puts(фмтЧ("[%d]: tag %04x fmt %d, count %d, data %s",
//			i, tag, fmt, count, BinHexEncode(data, data + len)));
		if(тип == BASE_IFD) {
			if(tag == 0x112)
				metadata.добавь("orientation", Exif16(data, end));
			if(tag == 0x132)
				metadata.добавь("DateTime", Ткст(data, 20));
			if(tag == 0x9003)
				metadata.добавь("DateTimeOriginal", Ткст(data, 20));
			if(tag == 0x9004)
				metadata.добавь("DateTimeDigitized", Ткст(data, 20));
			if(tag == 0x8825) {
				int offset = Exif32(data, end);
	//			puts(фмтЧ("GPS IFD at %08x", offset));
				ExifDir(begin, end, offset, GPS_IFD);
			}
		}
		else if(тип == GPS_IFD) {
			if((tag == 2 || tag == 4) && fmt == EXIF_RATIONAL && count == 3) {
				metadata.добавь(tag == 2 ? "GPSLatitude" : "GPSLongitude",
					ExifF5(data + 0, end) + ExifF5(data + 8, end) / 60 + ExifF5(data + 16, end) / 3600);
//				puts(фмтЧ("GPSLatitude: %n %n %n", n1, n2, n3));
			}
			else if(tag == 6 && fmt == EXIF_RATIONAL && count == 1)
				metadata.добавь("GPSAltitude", ExifF5(data, end));
			else if(tag == 16 && fmt == EXIF_ASCII && count == 2 && *data)
				metadata.добавь("GPSImgDirectionRef", Ткст(*data, 1));
			else if(tag == 17 && fmt == EXIF_RATIONAL && count == 1)
				metadata.добавь("GPSImgDirection", ExifF5(data + 0, end));
		}
	}
	int nextoff = Exif32(e, end);
//	puts(фмтЧ("next offset = %08x", nextoff));
	return nextoff;
}

void JPGRaster::Данные::ScanExifData(const char *begin, const char *end)
{
	const char *p = begin;
	if(p[0] == 'I' && p[1] == 'I')
		exif_big_endian = false;
	else if(p[0] == 'M' && p[1] == 'M')
		exif_big_endian = true;
	else
		return;
	for(int diroff = Exif32(p + 4, end); diroff && begin + diroff < end; diroff = ExifDir(begin, end, diroff, BASE_IFD))
		;
}

Значение JPGRaster::Данные::GetMetaData(Ткст id)
{
	ScanMetaData();
	return metadata.дай(id, Значение());
}

void JPGRaster::Данные::EnumMetaData(Вектор<Ткст>& id_list)
{
	ScanMetaData();
	id_list <<= metadata.дайКлючи();
}

Ткст JPGRaster::Данные::GetThumbnail()
{
	ScanMetaData();
	Ткст app1 = GetMetaData("APP1");
	if(!app1.дайСчёт())
		return Null;
	const char *begin = ~app1 + 6;
	const char *end = app1.стоп();
	const char *p = begin + Exif32(begin + 4, end);
	if(p + 2 >= end) return Null;
	p += Exif16(p, end) * 12 + 2;
	if(p + 4 >= end) return Null;
	p = begin + Exif32(p, end);
	if(p <= begin || p + 2 >= end) return Null;
	word count = Exif16(p, end);
	p += 2;
	dword offset = 0;
	dword len = 0;
	for(int n = 0; n < count; n++) {
		if(p + 12 >= end) return Null;
		if(Exif32(p + 4, end) == 1) {
			dword val = 0;
			switch(Exif16(p + 2, end)) {
			case 4:
			case 9:
				val = Exif32(p + 8, end);
				break;
			case 3:
			case 8:
				val = Exif16(p + 8, end);
				break;
			}
			if(val)
				switch(Exif16(p, end)) {
				case 0x201:
					offset = val;
					break;
				case 0x202:
					len = val;
					break;
				}
		}
		p += 12;
	}
	return offset && len && begin + offset + len < end ? Ткст(begin + offset, len) : Ткст();
}

bool JPGRaster::Данные::иниц()
{
	if(setjmp(jerr->jmpbuf))
		return false;
	jpeg_stream_src(&cinfo, *stream);
	jpeg_save_markers(&cinfo, JPEG_COM, 0xFFFF);
	for(int i = 0; i <= 15; i++)
		jpeg_save_markers(&cinfo, JPEG_APP0 + i, 0xFFFF);
	jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);

	switch(cinfo.output_components) {
		case 1: {
			формат.Set8();
			for(int i = 0; i < 256; i++) {
				КЗСА rgba;
				rgba.r = rgba.g = rgba.b = i;
				rgba.a = 255;
				palette[i] = rgba;
			}
			break;
		}
		case 3:
		case 4: {
			формат.Set24le(0xFF, 0xFF00, 0xFF0000);
			break;
		}
		default: {
			LLOG("JPGRaster: invalid number of components: " << (int)cinfo.output_components);
			return false;
		}
	}

	size.cx = cinfo.output_width;
	size.cy = cinfo.output_height;

	row_bytes = cinfo.output_components * size.cx;
	row_bytes_4 = (row_bytes + 3) & -4;

	double dot_scaling = (cinfo.density_unit == 1 ? 600 : cinfo.density_unit == 2 ? 600.0 / 2.54 : 0);
	if(dot_scaling && cinfo.X_density > 0) {
		dot_size.cx = fround(cinfo.image_width  * dot_scaling / cinfo.X_density);
		dot_size.cy = fround(cinfo.image_height * dot_scaling / cinfo.Y_density);
	}

	finish = true;
	return true;
}

Raster::Info JPGRaster::Данные::GetInfo()
{
	Raster::Info info;
	try {
		info.bpp = 24;
		info.colors = 0;
		info.dots = dot_size;
		info.hotspot = Точка(0, 0);
		info.kind = IMAGE_OPAQUE;
	}
	catch(Искл e) {
		LLOG(e);
	}
	return info;
}

Raster::Строка JPGRaster::Данные::дайСтроку(int line)
{
	byte *rowbuf = new byte[row_bytes_4];
	if(setjmp(jerr->jmpbuf))
		return Raster::Строка(rowbuf, &owner, true);

	ПРОВЕРЬ(line >= 0 && line < size.cy);
	if(line < next_line) {
		stream->перейди(stream_fpos);
		Поток *s = stream;
		освободи();
		строй();
		создай(*s);
		next_line = 0;
		LOG("перейди back");
	}
	JSAMPROW rowptr[] = { (JSAMPROW)rowbuf };
	while(next_line++ < line)
		jpeg_read_scanlines(&cinfo, rowptr, 1);
	jpeg_read_scanlines(&cinfo, rowptr, 1);

	if(cinfo.output_components == 4) { 
		/* Преобр CMYK scanline to дайКЗС */
		JSAMPLE k;
		for(int i = 0, j = 0; i < row_bytes_4; i++){			
			k = GETJSAMPLE(rowbuf[i+3]);
			rowbuf[j++] = GETJSAMPLE(rowbuf[i++]) * k / 255;
		    rowbuf[j++] = GETJSAMPLE(rowbuf[i++]) * k / 255;
		    rowbuf[j++] = GETJSAMPLE(rowbuf[i++]) * k / 255;
		}	
	}

	return Raster::Строка(rowbuf, &owner, true);
}

JPGRaster::JPGRaster()
{
}

JPGRaster::~JPGRaster()
{
}

bool JPGRaster::создай()
{
	ПРОВЕРЬ(sizeof(JSAMPLE) == sizeof(byte));
	data = new Данные(*this);
	return data->создай(GetStream());
}

Размер JPGRaster::дайРазм()
{
	return data->size;
}

Raster::Info JPGRaster::GetInfo()
{
	ПРОВЕРЬ(data);
	Raster::Info info;
	info.kind = IMAGE_OPAQUE;
	if(data->cinfo.output_components == 1) {
		info.bpp = 8;
		info.colors = 256;
	}
	else {
		info.bpp = 24;
		info.colors = 0;
	}
	info.dots = data->dot_size;
	info.hotspot = Null;
	Значение v = GetMetaData("orientation");
	if(число_ли(v))
		info.orientation = clamp((int)v - 1, 0, 7);
	return info;
}

Значение JPGRaster::GetMetaData(Ткст id)
{
	ПРОВЕРЬ(data);
	return data->GetMetaData(id);
}

void JPGRaster::EnumMetaData(Вектор<Ткст>& id_list)
{
	ПРОВЕРЬ(data);
	data->EnumMetaData(id_list);
}

Raster::Строка JPGRaster::дайСтроку(int line)
{
	ПРОВЕРЬ(data);
	return data->дайСтроку(line);
}

const КЗСА *JPGRaster::GetPalette()
{
	ПРОВЕРЬ(data);
	return data->palette;
}

const RasterFormat *JPGRaster::дайФормат()
{
	ПРОВЕРЬ(data);
	return &data->формат;
}

class JPGEncoder::Данные {
public:
	Данные();
	~Данные();

	void старт(Поток& stream, Размер size, Размер dots, int quality);
	void WriteLineRaw(const byte *rgba);

private:
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	Размер size;
	int line;
};

JPGEncoder::Данные::Данные()
{
	cinfo.err = jpeg_std_error(&jerr);
	cinfo.err->output_message = &NoOutput;
	jpeg_create_compress(&cinfo);
	cinfo.dct_method = JDCT_IFAST;
}

JPGEncoder::Данные::~Данные()
{
	jpeg_destroy_compress(&cinfo);
}

void JPGEncoder::Данные::старт(Поток& stream, Размер size_, Размер dots, int quality)
{
	size = size_;

	jpeg_stream_dest(&cinfo, stream);

	cinfo.image_width = size.cx;
	cinfo.image_height = size.cy;
	cinfo.input_components = 3; // # of color components per pixel
	cinfo.in_color_space = JCS_RGB; // colorspace of input image

	jpeg_set_defaults(&cinfo);

	if(dots.cx || dots.cy)
	{ // set up image density
		cinfo.density_unit = 1; // dots per inch
		cinfo.X_density = dots.cx ? fround(size.cx * 600.0 / dots.cx) : 0;
		cinfo.Y_density = dots.cy ? fround(size.cy * 600.0 / dots.cy) : 0;
	}

	jpeg_set_quality(&cinfo, quality, true); // limit to baseline-JPEG values
	jpeg_start_compress(&cinfo, true);

	line = 0;

	ПРОВЕРЬ(sizeof(JSAMPLE) == sizeof(byte));
}

void JPGEncoder::Данные::WriteLineRaw(const byte *s)
{
	JSAMPROW rowptr[] = { (byte *)s };
	jpeg_write_scanlines(&cinfo, rowptr, 1);
	if(++line >= size.cy)
		jpeg_finish_compress(&cinfo);
}

JPGEncoder::JPGEncoder(int quality_)
: quality(quality_)
{
	формат.Set24le(0xff, 0xff00, 0xff0000);
}

JPGEncoder::~JPGEncoder()
{
}

int JPGEncoder::GetPaletteCount()
{
	return 0;
}

void JPGEncoder::старт(Размер sz)
{
	data = new Данные;
	data->старт(GetStream(), sz, GetDots(), quality);
}

void JPGEncoder::WriteLineRaw(const byte *s)
{
	data->WriteLineRaw(s);
}

Рисунок JPGRaster::GetExifThumbnail()
{
	ПРОВЕРЬ(data);
	ТкстПоток ss(data->GetThumbnail());
	return StreamRaster::LoadStringAny(ss);
}

bool IsJPG(StreamRaster *s)
{
	return dynamic_cast<JPGRaster *>(s);
}

ИНИЦИАЛИЗАТОР(JPGRaster)
{
	StreamRaster::регистрируй<JPGRaster>();
	SetIsJPGFn(IsJPG);
}

}
