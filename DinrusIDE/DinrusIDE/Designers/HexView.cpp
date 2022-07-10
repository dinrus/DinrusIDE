#include "Designers.h"

bool  FileIsBinary(const char *path)
{
	ФайлВвод in(path);
	if(in) {
		Ткст data = in.дай(150000);
		if(*data.последний() == '\r')
			data.конкат(in.дай());
		const char *end = data.стоп();
		const char *s = data;
		while(s < end) {
			int c = (byte)*s;
			if(c < 32) {
				if(c == '\r') {
					if(s[1] != '\n')
						return true;
					s += 2;
				}
				else
				if(c == '\t' || c == '\n')
					s++;
				else
					return true;
			}
			else
				s++;
		}
	}
	return false;
}

ВекторМап<Ткст, Tuple2<int64, int64>> FileHexView::pos;

void FileHexView::RestoreEditPos()
{
	auto *p = pos.найдиУк(filename);
	if(p) {
		SetSc(p->a);
		устКурсор(p->b);
	}
	else {
		SetSc(0);
		устКурсор(0);
	}
}

void FileHexView::SaveEditPos()
{
	auto& p = pos.дайДобавь(filename);
	p.a = GetSc();
	p.b = дайКурсор();
}

int64 FileHexView::Ключ() const
{
	return blk;
}

int FileHexView::сделай(Ткст& object) const
{
	ФайлВвод& in = const_cast<ФайлВвод&>(file);
	in.перейди(blk * FileHexView::BLKSIZE);
	object = in.дай(FileHexView::BLKSIZE);
	return 1;
}

int FileHexView::Байт(int64 addr)
{
	blk = addr >> BLKSHIFT;
	Ткст h = cache.дай(*this);
	cache.сожмиСчёт(10); // cache only 10 blocks (~150 KB)
	int ii = addr & (BLKSIZE - 1);
	return ii < h.дайСчёт() ? (byte)h[ii] : 0;
}

void FileHexView::открой(const char *path)
{
	filename = path;
	file.открой(path);
	устВсего(file.дайРазм());
}

FileHexView::FileHexView()
{
	InfoMode(2);
	грузиИзГлоба(*this, "FileHexView");
}

FileHexView::~FileHexView()
{
	сохраниВГлоб(*this, "FileHexView");
}

ИНИЦИАЛИЗАТОР(ГексОбзор)
{
	региструйГлобКонфиг("FileHexView");
	региструйГлобСериализуй("FileHexViewPos", [=](Поток& s) {
		int version = 0;
		s / version;
		s % FileHexView::pos;
	});
}
