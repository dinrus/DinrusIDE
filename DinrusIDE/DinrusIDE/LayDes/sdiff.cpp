#include "LayDes.h"

static inline int sMatchLen(const char *a, const char *al, const char *b, const char *bl)
{
	const char *q = a;
	if(al - a < bl - b)
		while(a < al && *a == *b) {
			a++;
			b++;
		}
	else
		while(b < bl && *a == *b) {
			a++;
			b++;
		}
	return int(a - q);
}

unsigned HashPos(byte *s)
{
	return s[0] + (s[1] << 8) + (s[2] << 16) + (s[3] << 24);
}

void DiffPacker::SetMaster(const Ткст& m)
{
	master = m;
	mapn = master.дайДлину() | 5;
	prev.размести(master.дайДлину(), -1);
	map.размести(mapn, -1);
	for(int i = master.дайДлину() - 4; i >= 0; --i) {
		int hp = HashPos((byte *)~master + i) % mapn;
		prev[i] = map[hp];
		map[hp] = i;
	}
}

Ткст DiffPacker::Pack(const Ткст& data)
{
	Ткст result;
	const char *masterend = master.стоп();
	const char *dataend = data.стоп();
	const char *s = data;
	while(s < dataend) {
		int pos = 0;
		int ml = 0;
		int p = map[HashPos((byte *)s) % mapn];
		int ni = INT_MAX;
		while(p >= 0 && ml < ni) {
			int l = sMatchLen(s, dataend, ~master + p, masterend);
			if(l > ml) {
				pos = p;
				ml = l;
			}
			p = prev[p];
			ni >>= 1;
		}
		if(ml > 65535)
			ml = 65535;
		if(pos <= 255 && ml <= 255 && ml > 3) {
			result.конкат(0xfb);
			result.конкат(pos);
			result.конкат(ml);
			s += ml;
		}
		else
		if(pos <= 65535 && ml <= 255 && ml > 4) {
			result.конкат(0xfc);
			result.конкат(LOBYTE(pos));
			result.конкат(HIBYTE(pos));
			result.конкат(ml);
			s += ml;
		}
		else
		if(pos <= 65535 && ml > 5) {
			result.конкат(0xfd);
			result.конкат(LOBYTE(pos));
			result.конкат(HIBYTE(pos));
			result.конкат(LOBYTE(ml));
			result.конкат(HIBYTE(ml));
			s += ml;
		}
		else
		if(ml > 7) {
			result.конкат(0xfe);
			result.конкат(LOBYTE(LOWORD(pos)));
			result.конкат(HIBYTE(LOWORD(pos)));
			result.конкат(LOBYTE(HIWORD(pos)));
			result.конкат(HIBYTE(HIWORD(pos)));
			result.конкат(LOBYTE(ml));
			result.конкат(HIBYTE(ml));
			s += ml;
		}
		else {
			if((byte)*s >= 0xfb)
				result.конкат(0xff);
			result.конкат(*s++);
		}
	}
	result.сожми();
	return result;
}

Ткст DiffPack(const Ткст& master, const Ткст& data)
{
	DiffPacker p;
	p.SetMaster(master);
	return p.Pack(data);
}

Ткст DiffUnpack(const Ткст& master, const Ткст& pack)
{
	Ткст result;
	const char *s = pack;
	while(s < pack.стоп()) {
		int c = (byte)*s++;
		if(c == 0xfb) {
			ПРОВЕРЬ(s <= pack.стоп() - 2);
			byte p = (byte)*s++;
			byte l = (byte)*s++;
			result.конкат(~master + p, l);
		}
		else
		if(c == 0xfc) {
			ПРОВЕРЬ(s <= pack.стоп() - 3);
			byte pl = (byte)*s++;
			byte ph = (byte)*s++;
			byte ln = (byte)*s++;
			result.конкат(~master + MAKEWORD(pl, ph), ln);
		}
		else
		if(c == 0xfd) {
			ПРОВЕРЬ(s <= pack.стоп() - 4);
			byte pl = (byte)*s++;
			byte ph = (byte)*s++;
			byte ll = (byte)*s++;
			byte lh = (byte)*s++;
			result.конкат(~master + MAKEWORD(pl, ph), MAKEWORD(ll, lh));
		}
		else
		if(c == 0xfe) {
			ПРОВЕРЬ(s <= pack.стоп() - 5);
			byte pll = (byte)*s++;
			byte plh = (byte)*s++;
			byte phl = (byte)*s++;
			byte phh = (byte)*s++;
			byte ll = (byte)*s++;
			byte lh = (byte)*s++;
			result.конкат(~master + MAKELONG(MAKEWORD(pll, plh), MAKEWORD(phl, phh)),
			           MAKEWORD(ll, lh));
		}
		else
		if(c == 0xff)
			result.конкат(*s++);
		else
			result.конкат(c);
	}
	return result;
}
