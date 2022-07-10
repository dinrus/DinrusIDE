#include "zip.h"

namespace РНЦП {

void UnZip::ReadDir()
{
	Ошибка = true;
	
	file.очисть();
	current = 0;

	int entries = -1;
	int offset;

	int64 zipsize = zip->дайРазм();
	int64 pos = zipsize - 1; //22;
	zip->перейди(max((int64)0, zip->дайРазм() - 4000)); // Precache end of zip
	zip->дай();
	while(pos >= max((int64)0, zip->дайРазм() - 65536)) {
		zip->сотриОш();
		zip->перейди(pos);
		if(zip->Get32le() == 0x06054b50) {
			zip->Get16le();  // number of this disk
			zip->Get16le();  // number of the disk with the start of the central directory
			int h = zip->Get16le(); // total number of entries in the central directory on this disk
			entries = zip->Get16le(); // total number of entries in the central directory
			if(h != entries) // Multiple disks not supported
				return;
			zip->Get32le(); // size of the central directory
			offset = zip->Get32le(); //offset of start of central directory with respect to the starting disk number
			int commentlen = zip->Get16le();
			if(zip->дайПоз() + commentlen == zipsize)
				break;
		}
		pos--;
	}
	if(entries < 0)
		return;

	zip->перейди(offset);
	for(int i = 0; i < entries; i++) {
		if(zip->Get32le() != 0x02014b50 && zip->кф_ли())
			return;
		Файл& f = file.добавь();
		zip->Get16le();
		zip->Get16le();
		f.bit = zip->Get16le();  // general purpose bit flag
		f.method = zip->Get16le();
		f.time = zip->Get32le();
		f.crc = zip->Get32le();
		f.csize = zip->Get32le();
		f.usize = zip->Get32le();
		int fnlen = zip->Get16le();
		int extralen = zip->Get16le(); // extra field length              2 bytes
		int commentlen = zip->Get16le(); // file comment length             2 bytes
		if(zip->Get16le() != 0) // disk number start               2 bytes
			return; // Multiple disks not supported
		zip->Get16le(); // internal file attributes
		zip->Get32le(); // external file attributes
		f.offset = zip->Get32le();
		f.path = zip->дай(fnlen);
		zip->SeekCur(extralen + commentlen);
		if(zip->кф_ли() || zip->ошибка_ли())
			return;
	}
	
	Ошибка = false;
}

Время UnZip::GetZipTime(dword dt)
{
	Время time;
	time.year = int16(((dt >> 25) & 0x7f) + 1980);
	time.month = byte((dt >> 21) & 0x0f);
	time.day = byte((dt >> 16) & 0x1f);
	time.hour = byte((dt >> 11) & 0x1f);
	time.minute = byte((dt >> 5) & 0x3f);
	time.second = byte((dt << 1) & 0x3e);
	return time;
}

int64 zPress(Поток& out, Поток& in, int64 size, Врата<int64, int64> progress, bool gzip,
             bool compress, dword *crc, bool hdr);

bool UnZip::ReadFile(Поток& out, Врата<int, int> progress)
{
	if(Ошибка)
		return false;
	if(папка_ли()) {
		current++;
		return true;
	}
	Ошибка = true;
	if(current >= file.дайСчёт())
		return false;
	const Файл& f = file[current];
	zip->перейди(f.offset);
	if(zip->Get32le() != 0x04034b50)
		return false;
	zip->Get16le();
	zip->Get16le(); // пропусти header, use info from centrall dir
	zip->Get16le();
	zip->Get32le();
	zip->Get32le();
	zip->Get32le();
	zip->Get32le();
	dword filelen = zip->Get16le();
	dword extralen = zip->Get16le();
	zip->SeekCur(filelen + extralen);
	dword crc;
	dword l;
	if(f.method == 0) {
		Буфер<byte> temp(65536);
		int loaded;
		int count = f.csize;
		Crc32Stream crc32;
		while(count > 0 && (loaded = zip->дай(temp, (int)min<int64>(count, 65536))) > 0) {
			out.помести(temp, loaded);
			crc32.помести(temp, loaded);
			count -= loaded;
		}
		if(count > 0)
			return false;
		l = f.csize;
		crc = crc32;
	}
	else
	if(f.method == 8)
		l = (int)zPress(out, *zip, f.csize, AsGate64(progress), false, false, &crc, false);
	else
		return false;
	if(crc != f.crc || l != f.usize)
		return false;
	current++;
	Ошибка = false;
	return true;
}

Ткст UnZip::ReadFile(Врата<int, int> progress)
{
	ТкстПоток ss;
	return ReadFile(ss, progress) ? ss.дайРез() : Ткст::дайПроц();
}

Ткст UnZip::ReadFile(const char *path, Врата<int, int> progress)
{
    for(int i = 0; i < file.дайСчёт(); i++)
        if(file[i].path == path) {
            перейди(i);
            return ReadFile(progress);
        }

    return Ткст::дайПроц();
}

void UnZip::создай(Поток& _zip)
{
	zip = &_zip;
	ReadDir();
}

UnZip::UnZip(Поток& in)
{
	создай(in);
}

UnZip::UnZip()
{
	Ошибка = true;
	zip = NULL;
}

UnZip::~UnZip() {}

}
