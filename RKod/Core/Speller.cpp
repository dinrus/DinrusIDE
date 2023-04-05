#include "Core.h"

namespace РНЦПДинрус {

#define LLOG(x) // LOG(x)

struct SpellBlock : Движимое<SpellBlock> {
	Ткст first;
	int    offset;
	int    ctrl_len;
	int    text_len;
};

struct Speller {
	Ткст        данные;
	byte          charset;
	const char   *voc[256];
	int           dict;
	struct Строка : Движимое<Строка> {
		const byte *begin;
		const byte *end;
	};
	ВекторМап<dword, Строка> line;
	Индекс<ШТкст> user;

	Ткст            path;
	Массив<SpellBlock> block;
	
	bool     SetOld(const Ткст& данные);
	void     очисть()                        { данные.очисть(); path.очисть(); }
	operator bool() const                   { return !данные.пустой() || path.дайСчёт(); }
	bool     CheckOld(const ШТкст& wstr) const;

	Ткст дай(int offset, int len);
};

bool Speller::SetOld(const Ткст& _data)
{
	данные = _data;
	const char *s = данные;
	if(s >= данные.стоп()) {
		данные.очисть();
		return false;
	}
	charset = *s++;
	s++;// reserved for prefixes
	dict = *s++;
	for(int i = 0; i < 256 - dict; i++) {
		if(s >= данные.стоп()) {
			данные.очисть();
			return false;
		}
		voc[i] = s;
		while(*s) s++;
		s++;
	}
	line.очисть();
	while(s < данные.стоп()) {
		if(s + 8 >= данные.стоп()) {
			данные.очисть();
			return false;
		}
		int код_ = Peek32le(s);
		s += 4;
		int len = Peek32le(s);
		s += 4;
		Строка& l = line.добавь(код_);
		l.begin = (const byte *)s;
		s += len;
		l.end = (const byte *)s;
	};
	return true;
}

bool Speller::CheckOld(const ШТкст& wstr) const
{
	int len = wstr.дайДлину();
	if(len < 2)
		return true;
	if(len < 64) {
		Ткст w = изЮникода(wstr, charset);
		Ткст wl = изЮникода(впроп(wstr), charset);
		int i;
		if(len == 2) {
			w.конкат(127);
			wl.конкат(127);
		}
		i = line.найди(впроп(wl[0], charset) +
		              (впроп(wl[1], charset) << 8) +
		              (впроп(wl[2], charset) << 16));
		if(i >= 0) {
			const byte *s = line[i].begin;
			const byte *e = line[i].end;
			Ткст q;
			while(s < e)
				if(*s < dict) {
					if(q == w || q == wl)
						return true;
					q.обрежь(*s++);
				}
				else {
					ПРОВЕРЬ(*s >= dict);
					const char *x = voc[(int)*s++ - dict];
					q.конкат(x);
				}
			if(q == w || q == wl)
				return true;
		}
	}
	return user.найди(wstr) >= 0;;
}

static Ткст sUserFile(int lang)
{
	return конфигФайл(LNGAsText(lang) + ".usp");
}

Ткст spell_path;

void SetSpellPath(const Ткст& p)
{
	spell_path = p;
}

static Ткст sZet(ФайлВвод& in, int offset, int len)
{
	in.перейди(offset);
	return ZDecompress(in.дай(len));
}

void DoSpellerPath(Ткст& pp, Ткст dir)
{
	for(;;) {
		pp << dir << ';';
		Ткст d = дайПапкуФайла(dir);
		if(d == dir) break;
		dir = d;
	}
}

Speller *sGetSpeller(int lang)
{
	static МассивМап<int, Speller> speller;
	int q = speller.найди(lang);
	if(q < 0) {
		Ткст pp = spell_path;
#ifdef PLATFORM_COCOA
		Ткст Contents = дайПапкуФайла(дайПапкуФайла(дайФПутьИсп()));
		DoSpellerPath(pp, Contents + "/Resources");
		DoSpellerPath(pp, Contents + "/SharedSupport");
#endif
		DoSpellerPath(pp, дайФайлИзПапкиИсп("speller"));
		DoSpellerPath(pp, конфигФайл("speller"));
		pp << spell_path << ';' << getenv("LIB") << ';' << getenv("PATH") << ';';
#ifdef PLATFORM_POSIX
		pp << "/usr/local/share/upp/speller;/usr/local/share/upp;/usr/share/upp/speller;/usr/share/upp";
#endif
		Ткст path = дайФайлПоПути(впроп(LNGAsText(lang)) + ".udc", pp);
		if(пусто_ли(path))
			return NULL;
		ФайлВвод in(path);
		if(!in)
			return NULL;
		q = speller.дайСчёт();
		Speller& f = speller.добавь(lang);
		ФайлВвод user(sUserFile(lang));
		while(!user.кф_ли()) {
			Ткст s = user.дайСтроку();
			if(!s.пустой())
				f.user.добавь(вУтф32(s));
		}
		if(in.дай() != 255)
			f.SetOld(загрузиФайл(path));
		else {
			f.path = path;
			int n = in.дай32();
			LLOG("Found dictionary file " << path << " blocks " << n);
			if(n > 0 && n < 100000) {
				for(int i = 0; i < n; i++) {
					SpellBlock& b = f.block.добавь();
					b.first = in.дай(in.дай());
					b.ctrl_len = in.дай32();
					b.text_len = in.дай32();
				}
				if(in.кф_ли())
					f.block.очисть();
				else {
					int off = (int)in.дайПоз();
					for(int i = 0; i < n; i++) {
						SpellBlock& b = f.block[i];
						b.offset = off;
						off += b.ctrl_len + b.text_len;
					}
				}
			}
		}
	}
	return &speller[q];
}

bool SpellWordRaw(const ШТкст& wrd, int lang, Вектор<Ткст> *withdia)
{
	Speller *f = sGetSpeller(lang);
	if(!f)
		return true;
	if(f->данные.дайСчёт())
		return f->CheckOld(wrd);
	Ткст awrd = взаг(вАски(wrd).вТкст());
	Ткст t1 = вУтф8(wrd);
	Ткст t2 = вУтф8(впроп(wrd));
	for(int i = 0;; i++) {
		if(i + 1 >= f->block.дайСчёт() || awrd <= f->block[i + 1].first) {
			for(;;) {
				if(i >= f->block.дайСчёт())
					return f->user.найди(wrd) >= 0;;
				LLOG("Spell block " << i << ": " << f->block[i].first);
				const SpellBlock& b = f->block[i++];
				if(b.first > awrd) {
					LLOG("  --- end");
					return f->user.найди(wrd) >= 0;;
				}
				ФайлВвод in(f->path);
				Ткст ctrl = sZet(in, b.offset, b.ctrl_len);
				Ткст text = sZet(in, b.offset + b.ctrl_len, b.text_len);
				in.закрой();
				Ткст w;
				const char *s = ctrl;
				const char *e = ctrl.стоп();
				const char *t = text;
				const char *te = text.стоп();
				while(s < e && t < te) {
					w.обрежь(*s++);
					while(*t)
						w.конкат(*t++);
					if(w == t1 || w == t2)
						return true;
					if(withdia && t2 == впроп(вАски(w.вШТкст()).вТкст()))
						withdia->добавь(w);
					t++;
				}
			}
		}
	}
	return f->user.найди(wrd) >= 0;;
}

bool AllSpellerWords(int lang, Врата<Ткст> iter)
{
	Speller *f = sGetSpeller(lang);
	if(!f)
		return false;
	for(int i = 0; i < f->block.дайСчёт(); i++) {
		const SpellBlock& b = f->block[i++];
		ФайлВвод in(f->path);
		Ткст ctrl = sZet(in, b.offset, b.ctrl_len);
		Ткст text = sZet(in, b.offset + b.ctrl_len, b.text_len);
		in.закрой();
		Ткст w;
		const char *s = ctrl;
		const char *e = ctrl.стоп();
		const char *t = text;
		const char *te = text.стоп();
		while(s < e && t < te) {
			w.обрежь(*s++);
			while(*t)
				w.конкат(*t++);
			if(iter(w))
				return true;
			t++;
		}
	}
	return false;
}

struct SpellKey : Движимое<SpellKey> {
	int     lang;
	ШТкст wrd;
	
	hash_t дайХэшЗнач() const { return комбинируйХэш(lang, wrd); }
	bool operator==(const SpellKey& b) const { return lang == b.lang && wrd == b.wrd; }
};

struct SpellMaker : LRUCache<bool, SpellKey>::Делец {
	SpellKey k;
	
	SpellKey Ключ() const  { return k; }
	int    сделай(bool& r) const {
		r = SpellWordRaw(k.wrd, k.lang);
		return 1;
	}
};

static LRUCache<bool, SpellKey> speller_cache;

bool SpellWord(const ШТкст& ws, int lang)
{
	speller_cache.сожмиСчёт(2000);
	SpellMaker m;
	m.k.lang = lang;
	m.k.wrd = ws;
	return speller_cache.дай(m);
}

bool SpellWord(const wchar *ws, int len, int lang)
{
	return SpellWord(ШТкст(ws, len), lang);
}

void SpellerAdd(const ШТкст& w, int lang)
{
	if(!SpellWord(w, w.дайСчёт(), lang)) {
		Speller *f = sGetSpeller(lang);
		if(f) {
			ФайлДоп fa(sUserFile(lang));
			fa.PutLine(вУтф8(w));
			f->user.добавь(w);
			speller_cache.очисть();
		}
	}
}

struct WordDistanceTester {
	byte pchars[256];
	byte wchars[256];
	byte pairs[256 * 256];
	int  used_pairs[256]; // store used positions in pairs to be able to clear them at exit
	int  used_pairs_count; // count of pairs
	
	int дай(const Ткст& w, const Ткст& p);
	
	WordDistanceTester();
};

WordDistanceTester::WordDistanceTester()
{
	обнули(pchars);
	обнули(wchars);
	обнули(pairs);
	обнули(used_pairs);
	used_pairs_count = 0;
}

int WordDistanceTester::дай(const Ткст& p, const Ткст& w)
{
	if(абс(p.дайДлину() - w.дайДлину()) > 4 || w.дайСчёт() > 200 || p.дайСчёт() > 200)
		return INT_MAX;
	
	ВекторМап<int, int> chars;
	for(int i = 0; i < p.дайСчёт(); i++)
		pchars[(byte)p[i]]++;
	
	int score = 0;
	Ткст pp;
	ВекторМап<int, int> found;
	for(int i = 0; i < w.дайСчёт(); i++) {
		int c = (byte)w[i];
		if(pchars[c]) {
			pchars[c]--;
			wchars[c]++;
			score++;
			LLOG("Letter " << (char)w[i]);
		}
	}

	int first = 0;
	used_pairs_count = 0;
	for(int i = 0; i < p.дайСчёт(); i++) {
		int c = p[i];
		if(wchars[c]) {
			wchars[c]--;
			if(first) {
				LLOG("Pair " << (char)first << (char)c);
				int pairi = MAKEWORD(first, c);
				pairs[pairi]++;
				used_pairs[used_pairs_count++] = pairi;
			}
			first = c;
		}
	}

	for(int i = 1; i < w.дайСчёт(); i++) {
		int pairi = MAKEWORD(w[i - 1], w[i]);
		if(pairs[pairi]) {
			pairs[pairi]--;
			score++;
		}
	}
	
	score -= абс(p.дайДлину() - w.дайДлину());

	обнули(pchars);
	обнули(wchars);
	for(int i = 0; i < used_pairs_count; i++)
		pairs[used_pairs[i]] = 0;
	
	return -score;
}

Вектор<Ткст> SpellerFindCloseWords(int lang, const Ткст& w, int n)
{
	Вектор<Ткст> r;
	Вектор<int>    min_distance;
	if(n < 1)
		return r;
	Ткст aw = взаг(вАски(w));
	Один<WordDistanceTester> tester;
	tester.создай();
	AllSpellerWords(lang, [&] (Ткст h) -> bool {
		if(абс(h.дайДлину() - w.дайДлину()) < 5) {
			int d = tester->дай(утф8ВАскиЗаг(h), aw);
			if(min_distance.дайСчёт() == 0) {
				min_distance.добавь(d);
				r.добавь(h);
			}
			else
			if(d <= min_distance.верх()) {
				int ii = min_distance.дайСчёт() - 1;
				while(ii > 0 && d < min_distance[ii - 1])
					ii--;
				min_distance.вставь(ii, d);
				r.вставь(ii, h);
				if(r.дайСчёт() > n) {
					r.обрежь(n);
					min_distance.обрежь(n);
				}
			}
		}
		return false;
	});
	return r;
}

};
