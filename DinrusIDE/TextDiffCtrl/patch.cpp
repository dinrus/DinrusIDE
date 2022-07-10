#include "TextDiffCtrl.h"

namespace РНЦП {

bool Patch::загрузи0(Поток& in, Progress& pi)
{
	pi.устТекст("Загружается файл патча");
	target_dir.очисть();
	file.очисть();
	Ткст ln0;
	Ткст ln = in.дайСтроку();
	bool ok = false; // at least one hunk was loaded
	for(;;) {
		while(!ln.обрежьСтарт("--- ")) {
			if(pi.StepCanceled())
				return false;
			if(in.кф_ли())
				return ok;
			ln = in.дайСтроку();
		}
		ln = обрежьЛево(ln);
		int q = ln.найди('\t');
		if(q >= 0)
			ln.обрежь(q);
		Ткст фн = UnixPath(обрежьЛево("\"", обрежьПраво("\"", ln)));
		Массив<Chunk>& fp = file.дайДобавь(фн);
		while(!ln.начинаетсяС("@@")) {
			if(pi.StepCanceled())
				return false;
			if(in.кф_ли())
				return ok;
			ln0 = ln = in.дайСтроку();
		}
		while(ln.обрежьСтарт("@@")) {
			if(pi.StepCanceled())
				return false;
			int line = atoi(ln);
			if(line <= 0) {
				Chunk& ch = fp.добавь();
				ch.src << ln0 << "\n";
				ch.line = max(-line - 1, 0);
				for(;;) {
					if(pi.StepCanceled())
						return false;
					if(in.кф_ли())
						return ok;
					ln0 = ln = in.дайСтроку();
					if(*ln == ' ') {
						ch.orig.добавь(ln.середина(1));
						ch.patch.добавь(ln.середина(1));
					}
					else
					if(*ln == '+')
						ch.patch.добавь(ln.середина(1));
					else
					if(*ln == '-')
						ch.orig.добавь(ln.середина(1));
					else
						break;
					ok = true;
					ch.src << ln << "\n";
				}
			}
		}
	}

	return ok;
}

bool Patch::грузи(Поток& in, Progress& pi)
{
	common_path.очисть();
	if(!загрузи0(in, pi))
		return false;
	const Вектор<Ткст>& h = file.дайКлючи();
	if(h.дайСчёт() == 0)
		return false;
	common_path = дайПапкуФайла(h[0]);
	for(int i = 1; i < h.дайСчёт(); i++)
		common_path.обрежь(MatchLen(common_path, h[i]));
	common_path.обрежьКонец("/");
	common_path.обрежьСтарт("/");
	if(common_path.дайСчёт())
		for(int i = 0; i < h.дайСчёт(); i++)
			file.устКлюч(i, h[i].середина(common_path.дайСчёт() + 1));
	return true;
}

int Patch::MatchLen(const Ткст& a, const Ткст& b)
{
	int n = min(a.дайДлину(), b.дайДлину());
	int q = 0;
	for(int i = 0; i < n; i++) {
		if(a[i] != b[i])
			return q;
		if(a[i] == '/')
			q = i;
	}
	return a[n] || b[n] ? b[n] == '/' || a[n] == '/' ? n : q : n;
}

int Patch::MatchCount(const char *dir)
{
	int count = 0;
	for(const Ткст& p : file.дайКлючи()) {
		if(файлЕсть(приставьИмяф(dir, p)))
			count++;
	}
	return count;
}

bool Patch::MatchFiles(const Вектор<Ткст>& dir, Progress& pi)
{
	pi.устТекст("Сравниваются директории");
	int best = 0;
	Ткст com_path = common_path;
	com_path.замени("\\", "/");
	if(dir.дайСчёт())
		for(int pass = 0; pass < 2; pass++) {
			for(;;) {
				for(Ткст d : dir) {
					while(d.дайСчёт() > 3) {
						if(pi.StepCanceled())
							return false;
						Ткст dir = приставьИмяф(d, com_path);
						int n = MatchCount(dir);
						if(n > best) {
							best = n;
							target_dir = dir;
						}
						d = дайПапкуФайла(d);
					}
				}
				int q = com_path.найди('/');
				if(q >= 0)
					com_path = com_path.середина(q + 1);
				else
					break;
			}
			com_path = дайПапкуФайла(dir[0]);
		}
	return best;
}

bool Patch::грузи(const char *фн, Progress& pi)
{
	ФайлВвод in(фн);
	return грузи(in, pi);
}

Ткст Patch::GetPatch(int i) const
{
	Ткст r;
	for(const Chunk& ch : file[i])
		r << ch.src;
	return r;
}

Ткст Patch::GetPatchedFile(int i, const Ткст& text) const
{
	if(дайДлинуф(дайПуть(i)) > 4 * 1024 * 1024)
		return Ткст::дайПроц();
	Вектор<Ткст> lines;
	bool crlf = true;
	Ткст path = дайПуть(i);
	if(файлЕсть(path) || пусто_ли(text)) {
		Ткст s = Nvl(text, загрузиФайл(дайПуть(i)));
		crlf = s.найди('\r') >= 0;
		lines = разбей(фильтруй(s, [](int c) { return c == '\r' ? 0 : c; }), '\n', false);
	}

	const Массив<Chunk>& chs = file[i];
	Вектор<int> ch_pos;

	for(int i = 0; i < chs.дайСчёт(); i++) {
		const Chunk& ch = chs[i];
		int pos = -1;
		for(int d = 0; d < 200; d++) { // 200 lines of tolerance in both directions when matching the patch
			auto сверь = [&](int li) {
				if(li < 0)
					return false;
				for(int i = 0; i < ch.orig.дайСчёт(); i++) {
					if(li + i >= lines.дайСчёт() || ch.orig[i] != lines[li + i])
						return false;
				}
				return true;
			};
			if(сверь(ch.line + d)) {
				pos = ch.line + d;
				break;
			}
			if(d && сверь(ch.line - d)) {
				pos = ch.line - d;
				break;
			}
		}
		
		if(pos < 0 || ch_pos.дайСчёт() && ch_pos.верх() > pos)
			return Ткст::дайПроц();
		ch_pos.добавь(pos);
	}
	
	for(int i = 0; i < ch_pos.дайСчёт(); i++) {
		const Chunk& ch = chs[i];
		lines.удали(ch_pos[i], ch.orig.дайСчёт());
		lines.вставь(ch_pos[i], ch.patch);
		int added = ch.patch.дайСчёт() - ch.orig.дайСчёт(); // negative if removed
		for(int j = i + 1; j < ch_pos.дайСчёт(); j++)
			ch_pos[j] += added;
	}
	
	return Join(lines, crlf ? "\r\n" : "\n");
}

};
