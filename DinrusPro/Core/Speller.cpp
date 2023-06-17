#include <DinrusPro/DinrusCore.h>

#define LLOG(x) // LOG(x)

struct SpellBlock : Движ<SpellBlock> {
	Ткст first;
	цел    offset;
	цел    ctrl_len;
	цел    text_len;
};

struct Speller {
	Ткст        данные;
	ббайт          charset;
	const сим   *voc[256];
	цел           dict;
	struct Строка : Движ<Строка> {
		const ббайт *begin;
		const ббайт *end;
	};
	ВекторМап<бцел, Строка> line;
	Индекс<ШТкст> user;

	Ткст            path;
	Массив<SpellBlock> block;
	
	бул     SetOld(const Ткст& данные);
	проц     очисть()                        { данные.очисть(); path.очисть(); }
	operator бул() const                   { return !данные.пустой() || path.дайСчёт(); }
	бул     CheckOld(const ШТкст& wstr) const;

	Ткст дай(цел offset, цел len);
};

бул Speller::SetOld(const Ткст& _data)
{
	данные = _data;
	кткст0 s = данные;
	if(s >= данные.стоп()) {
		данные.очисть();
		return false;
	}
	charset = *s++;
	s++;// reserved for prefixes
	dict = *s++;
	for(цел i = 0; i < 256 - dict; i++) {
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
		цел код_ = подбери32лэ(s);
		s += 4;
		цел len = подбери32лэ(s);
		s += 4;
		Строка& l = line.добавь(код_);
		l.begin = (const ббайт *)s;
		s += len;
		l.end = (const ббайт *)s;
	};
	return true;
}

бул Speller::CheckOld(const ШТкст& wstr) const
{
	цел len = wstr.дайДлину();
	if(len < 2)
		return true;
	if(len < 64) {
		Ткст w = изЮникода(wstr, charset);
		Ткст wl = изЮникода(впроп(wstr), charset);
		цел i;
		if(len == 2) {
			w.кат(127);
			wl.кат(127);
		}
		i = line.найди(впроп(wl[0], charset) +
		              (впроп(wl[1], charset) << 8) +
		              (впроп(wl[2], charset) << 16));
		if(i >= 0) {
			const ббайт *s = line[i].begin;
			const ббайт *e = line[i].end;
			Ткст q;
			while(s < e)
				if(*s < dict) {
					if(q == w || q == wl)
						return true;
					q.обрежь(*s++);
				}
				else {
					ПРОВЕРЬ(*s >= dict);
					кткст0 x = voc[(цел)*s++ - dict];
					q.кат(x);
				}
			if(q == w || q == wl)
				return true;
		}
	}
	return user.найди(wstr) >= 0;;
}

static Ткст sUserFile(цел lang)
{
	return конфигФайл(LNGAsText(lang) + ".usp");
}

Ткст spell_path;

проц SetSpellPath(const Ткст& p)
{
	spell_path = p;
}

static Ткст sZet(ФайлВвод& in, цел offset, цел len)
{
	in.перейди(offset);
	return ZDecompress(in.дай(len));
}

проц DoSpellerPath(Ткст& pp, Ткст dir)
{
	for(;;) {
		pp << dir << ';';
		Ткст d = дайПапкуФайла(dir);
		if(d == dir) break;
		dir = d;
	}
}

Speller *sGetSpeller(цел lang)
{
	static МассивМап<цел, Speller> speller;
	цел q = speller.найди(lang);
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
			цел n = in.дай32();
			LLOG("Found dictionary file " << path << " blocks " << n);
			if(n > 0 && n < 100000) {
				for(цел i = 0; i < n; i++) {
					SpellBlock& b = f.block.добавь();
					b.first = in.дай(in.дай());
					b.ctrl_len = in.дай32();
					b.text_len = in.дай32();
				}
				if(in.кф_ли())
					f.block.очисть();
				else {
					цел off = (цел)in.дайПоз();
					for(цел i = 0; i < n; i++) {
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

бул SpellWordRaw(const ШТкст& wrd, цел lang, Вектор<Ткст> *withdia)
{
	Speller *f = sGetSpeller(lang);
	if(!f)
		return true;
	if(f->данные.дайСчёт())
		return f->CheckOld(wrd);
	Ткст awrd = взаг(вАски(wrd).вТкст());
	Ткст t1 = вУтф8(wrd);
	Ткст t2 = вУтф8(впроп(wrd));
	for(цел i = 0;; i++) {
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
				кткст0 s = ctrl;
				кткст0 e = ctrl.стоп();
				кткст0 t = text;
				кткст0 te = text.стоп();
				while(s < e && t < te) {
					w.обрежь(*s++);
					while(*t)
						w.кат(*t++);
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

бул AllSpellerWords(цел lang, Врата<Ткст> iter)
{
	Speller *f = sGetSpeller(lang);
	if(!f)
		return false;
	for(цел i = 0; i < f->block.дайСчёт(); i++) {
		const SpellBlock& b = f->block[i++];
		ФайлВвод in(f->path);
		Ткст ctrl = sZet(in, b.offset, b.ctrl_len);
		Ткст text = sZet(in, b.offset + b.ctrl_len, b.text_len);
		in.закрой();
		Ткст w;
		кткст0 s = ctrl;
		кткст0 e = ctrl.стоп();
		кткст0 t = text;
		кткст0 te = text.стоп();
		while(s < e && t < te) {
			w.обрежь(*s++);
			while(*t)
				w.кат(*t++);
			if(iter(w))
				return true;
			t++;
		}
	}
	return false;
}

struct SpellKey : Движ<SpellKey> {
	цел     lang;
	ШТкст wrd;
	
	т_хэш дайХэшЗнач() const { return КомбХэш(lang, wrd); }
	бул operator==(const SpellKey& b) const { return lang == b.lang && wrd == b.wrd; }
};

struct SpellMaker : КэшЛРУ<бул, SpellKey>::Делец {
	SpellKey k;
	
	SpellKey Ключ() const  { return k; }
	цел    сделай(бул& r) const {
		r = SpellWordRaw(k.wrd, k.lang);
		return 1;
	}
};

static КэшЛРУ<бул, SpellKey> speller_cache;

бул SpellWord(const ШТкст& ws, цел lang)
{
	speller_cache.сожмиСчёт(2000);
	SpellMaker m;
	m.k.lang = lang;
	m.k.wrd = ws;
	return speller_cache.дай(m);
}

бул SpellWord(const шим *ws, цел len, цел lang)
{
	return SpellWord(ШТкст(ws, len), lang);
}

проц SpellerAdd(const ШТкст& w, цел lang)
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
	ббайт pchars[256];
	ббайт wchars[256];
	ббайт pairs[256 * 256];
	цел  used_pairs[256]; // store used positions in pairs to be able to clear them at exit
	цел  used_pairs_count; // count of pairs
	
	цел дай(const Ткст& w, const Ткст& p);
	
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

цел WordDistanceTester::дай(const Ткст& p, const Ткст& w)
{
	if(абс(p.дайДлину() - w.дайДлину()) > 4 || w.дайСчёт() > 200 || p.дайСчёт() > 200)
		return INT_MAX;
	
	ВекторМап<цел, цел> chars;
	for(цел i = 0; i < p.дайСчёт(); i++)
		pchars[(ббайт)p[i]]++;
	
	цел score = 0;
	Ткст pp;
	ВекторМап<цел, цел> found;
	for(цел i = 0; i < w.дайСчёт(); i++) {
		цел c = (ббайт)w[i];
		if(pchars[c]) {
			pchars[c]--;
			wchars[c]++;
			score++;
			LLOG("Letter " << (сим)w[i]);
		}
	}

	цел first = 0;
	used_pairs_count = 0;
	for(цел i = 0; i < p.дайСчёт(); i++) {
		цел c = p[i];
		if(wchars[c]) {
			wchars[c]--;
			if(first) {
				LLOG("Pair " << (сим)first << (сим)c);
				цел pairi = MAKEWORD(first, c);
				pairs[pairi]++;
				used_pairs[used_pairs_count++] = pairi;
			}
			first = c;
		}
	}

	for(цел i = 1; i < w.дайСчёт(); i++) {
		цел pairi = MAKEWORD(w[i - 1], w[i]);
		if(pairs[pairi]) {
			pairs[pairi]--;
			score++;
		}
	}
	
	score -= абс(p.дайДлину() - w.дайДлину());

	обнули(pchars);
	обнули(wchars);
	for(цел i = 0; i < used_pairs_count; i++)
		pairs[used_pairs[i]] = 0;
	
	return -score;
}

Вектор<Ткст> SpellerFindCloseWords(цел lang, const Ткст& w, цел n)
{
	Вектор<Ткст> r;
	Вектор<цел>    min_distance;
	if(n < 1)
		return r;
	Ткст aw = взаг(вАски(w));
	Один<WordDistanceTester> tester;
	tester.создай();
	AllSpellerWords(lang, [&] (Ткст h) -> бул {
		if(абс(h.дайДлину() - w.дайДлину()) < 5) {
			цел d = tester->дай(утф8ВАскиЗаг(h), aw);
			if(min_distance.дайСчёт() == 0) {
				min_distance.добавь(d);
				r.добавь(h);
			}
			else
			if(d <= min_distance.верх()) {
				цел ii = min_distance.дайСчёт() - 1;
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
