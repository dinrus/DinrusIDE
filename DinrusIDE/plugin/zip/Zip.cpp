#include "zip.h"

namespace РНЦП {

void Zip::WriteFolder(const char *path, Время tm)
{
	Ткст p = UnixPath(path);
	if(*p.последний() != '/')
		p.конкат('/');
	WriteFile(~p, 0, p, Null, tm);
}

int64 zPress(Поток& out, Поток& in, int64 size, Врата<int64, int64> progress, bool gzip,
             bool compress, dword *crc, bool hdr);


void Zip::FileHeader(const char *path, Время tm)
{
	Файл& f = file.верх();
	f.path = UnixPath(path);
	zip->Put32le(0x04034b50);
	zip->Put16le(f.version);
	zip->Put16le(f.gpflag);
	zip->Put16le(f.method);
	zip->Put32le(f.time = (tm.day << 16) | (tm.month << 21) | ((tm.year - 1980) << 25) |
	                      (tm.hour << 11) | (tm.minute << 5) | (tm.second >> 1));
	ПРОВЕРЬ((f.gpflag & 0x8) == 0 || f.crc == 0);
	zip->Put32le(f.crc);
	ПРОВЕРЬ((f.gpflag & 0x8) == 0 || f.csize == 0);
	zip->Put32le(f.csize);
	ПРОВЕРЬ((f.gpflag & 0x8) == 0 || f.usize == 0);
	zip->Put32le(f.usize);
	zip->Put16le((word)strlen(f.path));
	zip->Put16le(0);
	zip->помести(f.path);
	done += 5*2 + 5*4 + f.path.дайСчёт();
}

void Zip::BeginFile(const char *path, Время tm, bool deflate)
{
	ПРОВЕРЬ(!IsFileOpened());
	if(deflate) {
		pipeZLib.создай();
		pipeZLib->WhenOut = THISBACK(PutCompressed);
		pipeZLib->GZip(false).CRC().NoHeader().Compress();
	}
	else {
		crc32.очисть();
		uncompressed = true;
	}
	Файл& f = file.добавь();
	f.version = 21;
	f.gpflag = 0x8;
	f.method = deflate ? 8 : 0;
	f.crc = 0;
	f.csize = 0;
	f.usize = 0;
	FileHeader(path, tm);
	if (zip->ошибка_ли()) WhenError();
}

void Zip::BeginFile(ВыхФильтрПоток& oz, const char *path, Время tm, bool deflate)
{
	BeginFile(path, tm, deflate);
	oz.фильтруй = THISBACK(помести);
	oz.стоп = THISBACK(EndFile);
}

void Zip::помести(const void *ptr, int size)
{
	ПРОВЕРЬ(IsFileOpened());
	Файл& f = file.верх();
	if(f.method == 0) {
		PutCompressed(ptr, size);
		crc32.помести(ptr, size);
	}
	else
		pipeZLib->помести(ptr, size);
	f.usize += size;
}

void Zip::EndFile()
{
	if(!IsFileOpened())
		return;
	Файл& f = file.верх();
	ПРОВЕРЬ(f.gpflag & 0x8);
	if(f.method == 0)
		zip->Put32le(f.crc = crc32);
	else {
		pipeZLib->стоп();
		zip->Put32le(f.crc = pipeZLib->GetCRC());
	}
	zip->Put32le(f.csize);
	zip->Put32le(f.usize);
	done += 3*4;
	pipeZLib.очисть();
	uncompressed = false;
	if(zip->ошибка_ли()) WhenError();
}

void Zip::PutCompressed(const void *ptr, int size)
{
	ПРОВЕРЬ(IsFileOpened());
	zip->помести(ptr, size);
	if (zip->ошибка_ли()) WhenError();
	done += size;
	file.верх().csize += size;
}

void Zip::WriteFile(const void *ptr, int size, const char *path, Врата<int, int> progress, Время tm, bool deflate)
{
	ПРОВЕРЬ(!IsFileOpened());
	if(!deflate) {
		BeginFile(path, tm, deflate);
		int done = 0;
		while(done < size) {
			if(progress(done, size))
				return;
			int chunk = min(size - done, 65536);
			помести((byte *)ptr + done, chunk);
			if(zip->ошибка_ли()) {
				WhenError();
				return;
			}
			done += chunk;
		}
		EndFile();
		return;
	}
	// following code could be implemented using BeginFile/помести/EndFile, but be conservative, keep proven code
	Файл& f = file.добавь();
	ТкстПоток ss;
	ПамЧтенПоток ms(ptr, size);

	f.usize = size;
	zPress(ss, ms, size, AsGate64(progress), false, true, &f.crc, false);

	Ткст data = ss.дайРез();
	const void *r = ~data;
	f.csize = data.дайДлину();

	f.version = 20;
	f.gpflag = 0;
	if(data.дайДлину() >= size) {
		r = ptr;
		f.csize = size;
		f.method = 0;
	}
	else
		f.method = 8;
	FileHeader(path, tm);
	zip->помести(r, f.csize);
	done += f.csize;
	if (zip->ошибка_ли()) WhenError();
}

void Zip::WriteFile(const Ткст& s, const char *path, Врата<int, int> progress, Время tm, bool deflate)
{
	WriteFile(~s, s.дайСчёт(), path, progress, tm, deflate);
}

void Zip::создай(Поток& out)
{
	финиш();
	done = 0;
	zip = &out;
}

void Zip::финиш()
{
	if(!zip)
		return;
	dword off = done;
	dword rof = 0;
	for(int i = 0; i < file.дайСчёт(); i++) {
		Файл& f = file[i];
		zip->Put32le(0x02014b50);
		zip->Put16le(20);
		zip->Put16le(f.version);
		zip->Put16le(f.gpflag);  // general purpose bit flag
		zip->Put16le(f.method);
		zip->Put32le(f.time);
		zip->Put32le(f.crc);
		zip->Put32le(f.csize);
		zip->Put32le(f.usize);
		zip->Put16le(f.path.дайСчёт());
		zip->Put16le(0); // extra field length              2 bytes
		zip->Put16le(0); // file comment length             2 bytes
		zip->Put16le(0); // disk number start               2 bytes
		zip->Put16le(0); // internal file attributes        2 bytes
		zip->Put32le(0); // external file attributes        4 bytes
		zip->Put32le(rof); // relative offset of local header 4 bytes
		rof+=5 * 2 + 5 * 4 + f.csize + f.path.дайСчёт() + (f.gpflag & 0x8 ? 3*4 : 0);
		zip->помести(f.path);
		done += 7 * 4 + 9 * 2 + f.path.дайСчёт();
	}
	zip->Put32le(0x06054b50);
	zip->Put16le(0);  // number of this disk
	zip->Put16le(0);  // number of the disk with the start of the central directory
	zip->Put16le(file.дайСчёт()); // total number of entries in the central directory on this disk
	zip->Put16le(file.дайСчёт()); // total number of entries in the central directory
	zip->Put32le(done - off); // size of the central directory
	zip->Put32le(off); //offset of start of central directory with respect to the starting disk number
	zip->Put16le(0);
	if (zip->ошибка_ли()) WhenError(); 
	zip = NULL;
}

Zip::Zip()
{
	done = 0;
	zip = NULL;
	uncompressed = false;
}

Zip::Zip(Поток& out)
{
	done = 0;
	zip = NULL;
	uncompressed = false;
	создай(out);
}

Zip::~Zip()
{
	финиш();
}

bool FileZip::создай(const char *имя)
{
	bool b = zip.открой(имя);
	Zip::создай(zip); // if there is Ошибка, we still need to have to dump data
	return b;
}

bool FileZip::финиш()
{
	if(zip.открыт()) {
		Zip::финиш();
		zip.закрой();
		return !zip.ошибка_ли();
	}
	return false;
}

void StringZip::создай()
{
	Zip::создай(zip);
}

Ткст StringZip::финиш()
{
	Zip::финиш();
	return zip.дайРез();
}

}
