#include "PdfDraw.h"

namespace РНЦП {

TTFReader::TTFStream& TTFReader::TTFStream::operator%(byte& q) {
	дайРяд(&q, 1);
	return *this;
}

#ifdef CPU_LE
TTFReader::TTFStream& TTFReader::TTFStream::operator%(int32& q) {
	byte *h = (byte *)&q;
	*this % h[3] % h[2] % h[1] % h[0];
	return *this;
}

TTFReader::TTFStream& TTFReader::TTFStream::operator%(uint32& q) {
	byte *h = (byte *)&q;
	*this % h[3] % h[2] % h[1] % h[0];
	return *this;
}

TTFReader::TTFStream& TTFReader::TTFStream::operator%(int16& q) {
	byte *h = (byte *)&q;
	*this % h[1] % h[0];
	return *this;
}

TTFReader::TTFStream& TTFReader::TTFStream::operator%(uint16& q) {
	byte *h = (byte *)&q;
	*this % h[1] % h[0];
	return *this;
}
#else
TTFReader::TTFStream& TTFReader::TTFStream::operator%(int32& q) {
	byte *h = (byte *)&q;
	*this % h[0] % h[1] % h[2] % h[3];
	return *this;
}

TTFReader::TTFStream& TTFReader::TTFStream::operator%(uint32& q) {
	byte *h = (byte *)&q;
	*this % h[0] % h[1] % h[2] % h[3];
	return *this;
}

TTFReader::TTFStream& TTFReader::TTFStream::operator%(int16& q) {
	byte *h = (byte *)&q;
	*this % h[0] % h[1];
	return *this;
}

TTFReader::TTFStream& TTFReader::TTFStream::operator%(uint16& q) {
	byte *h = (byte *)&q;
	*this % h[0] % h[1];
	return *this;
}
#endif

void TTFReader::TTFStreamIn::перейди(int offset)
{
	ПРОВЕРЬ(offset >= 0 && offset < lim - beg);
	s = beg + offset;
}

int TTFReader::TTFStreamIn::дай8()
{
	if(s + 1 > lim)
		throw Fail();
	int q = (byte)*s;
	s++;
	return q;
}

int TTFReader::TTFStreamIn::дай16()
{
	if(s + 2 > lim)
		throw Fail();
	int q = ((byte)s[0] << 8) | (byte)s[1];
	s += 2;
	return q;

}

int TTFReader::TTFStreamIn::дай32()
{
	if(s + 4 > lim)
		throw Fail();
	int q = ((byte)s[0] << 24) | ((byte)s[1] << 16) | ((byte)s[2] << 8) | (byte)s[3];
	s += 2;
	return q;
}

Ткст TTFReader::TTFStreamIn::дай(int n)
{
	if(s + n > lim)
		throw Fail();
	Ткст r(s, n);
	s += n;
	return r;
}

void TTFReader::TTFStreamIn::дайРяд(void *ptr, int count) {
	if(s + count > lim)
		throw Fail();
	memcpy(ptr, s, count);
	s += count;
}

}
