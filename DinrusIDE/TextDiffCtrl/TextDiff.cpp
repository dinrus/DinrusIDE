#include "TextDiffCtrl.h"

namespace РНЦП {

template <class I>
static int CompareGetCount(I a, I b, int max_count)
{
	if(max_count <= 0 || *a != *b)
		return 0;
	int left;
	for(left = max_count; --left > 0;)
		if(*++a != *++b)
			return max_count - left;
	return max_count;
}

Вектор<Ткст> дайМапСтрок(Поток& stream)
{
	Вектор<Ткст> out;
	int emp = 0;
	if(stream.открыт())
		while(!stream.кф_ли()) {
			Ткст s = stream.дайСтроку();
			const char *p = s, *e = s.стоп(), *f = e;
			while(e > p && /*(byte)e[-1] != 9 && */(byte)e[-1] < ' ')
				e--;
			if(e == p)
				emp++;
			else
			{
				while(emp-- > 0)
					out.добавь(Null);
				if(e != f)
					s.обрежь(int(e - p));
				out.добавь(s);
				emp = 0;
			}
		}
	return out;
}

Вектор<Ткст> дайМапСтрокФайла(const Ткст& path)
{
	ФайлВвод fi(path);
	return дайМапСтрок(fi);
}

Вектор<Ткст> дайМапСтрокТкст(const Ткст& s)
{
	ТкстПоток ss(s);
	return дайМапСтрок(ss);
}

class TextComparator
{
public:
	TextComparator(const Вектор<Ткст>& f1, const Вектор<Ткст>& f2);

	Массив<ТекстСекция>    GetSections() const;

private:
	bool                  найди(int start1, int end1, int start2, int end2, int& best_match, int& best_count) const;
	void                  разбей(Массив<ТекстСекция>& dest, int start1, int end1, int start2, int end2) const;

private:
	Вектор<Индекс<dword>>  hash1;
	Вектор<Индекс<dword>>  hash2;
	const Вектор<Ткст>& file1;
	const Вектор<Ткст>& file2;
};

Массив<ТекстСекция> сравниМапыСтрок(const Вектор<Ткст>& s1, const Вектор<Ткст>& s2)
{
	return TextComparator(s1, s2).GetSections();
}

static void CalcHash(Вектор<Индекс<dword>>& hash, const Вектор<Ткст>& file, int limit)
{
	{ // 1st row
		Индекс<dword>& first = hash.добавь();
		for(int i = 0; i < file.дайСчёт(); i++)
			first.добавь(FoldHash(дайХэшЗнач(file[i])));
	}
	static const int prime[] =
	{
		3,  5,  7,   11,  13,  17,  19,  21,
		23, 29, 31,  37,  41,  43,  47,  51,
		53, 61, 67,  71,  73,  79,  83,  87,
		89, 97, 101, 103, 107, 109, 113, 117,
	};
	const int *pp = prime;
	for(int l = 1; l < limit; l <<= 1) {
		Индекс<dword>& nhash = hash.добавь();
		const Индекс<dword>& ohash = hash[hash.дайСчёт() - 2];
		int pri = *pp++;
		int t;
		for(t = l; t < ohash.дайСчёт(); t++)
			nhash.добавь(ohash[t - l] + pri * ohash[t]);
		for(t -= l; t < ohash.дайСчёт(); t++)
			nhash.добавь(ohash[t]);
	}
}

TextComparator::TextComparator(const Вектор<Ткст>& f1, const Вектор<Ткст>& f2)
: file1(f1), file2(f2)
{
	int limit = min(f1.дайСчёт(), f2.дайСчёт());
	CalcHash(hash1, f1, limit);
	CalcHash(hash2, f2, limit);
}

static bool CompareSection(const ТекстСекция& ta, const ТекстСекция& tb)
{
	return ta.start1 < tb.start1 || ta.start1 == tb.start1 && ta.start2 < tb.start2;
}

Массив<ТекстСекция> TextComparator::GetSections() const
{
	Массив<ТекстСекция> output;
	разбей(output, 0, file1.дайСчёт(), 0, file2.дайСчёт());
	сортируй(output, &CompareSection);
	return output;
}

static int GetHashLevel(int min_count, int hash_count)
{
	int l = 0;
	hash_count--;
	while(min_count > 1 && l < hash_count)
	{
		min_count >>= 1;
		l++;
	}
	return l;
}

bool TextComparator::найди(int start1, int end1, int start2, int end2, int& best_match, int& best_count) const
{
	ПРОВЕРЬ(end1 > start1 && end2 > start2);
	bool done = false;
	const Ткст *f1 = file1.старт() + start1;
	int len1 = end1 - start1;
	int lvl = GetHashLevel(best_count + 1, hash1.дайСчёт());
	int chunk = 1 << lvl;
	int last = max(best_count - chunk + 1, 0);
	const Индекс<dword> *hp1 = &hash1[lvl];
	const Индекс<dword> *hp2 = &hash2[lvl];
	const dword *h1 = hp1->begin() + start1;

	int i = hp2->найди(*h1);
	while(i >= 0)
		if(i + best_count >= end2)
			return done;
		else {
			if(i >= start2 && h1[last] == (*hp2)[i + last]) {
				int top = min(len1, end2 - i);
				int hc = CompareGetCount(h1, hp2->begin() + i, top) + chunk - 1;
				int cnt = CompareGetCount(f1, file2.begin() + i, min(hc, top));
				if(cnt > best_count) {
					best_count = cnt;
					best_match = i;
					done = true;
					last = best_count - chunk + 1;
					if(best_count + 1 >= 2 * chunk)
					{
						lvl = GetHashLevel(best_count + 1, hash1.дайСчёт());
						chunk = 1 << lvl;
						last = best_count - chunk + 1;
						hp1 = &hash1[lvl];
						hp2 = &hash2[lvl];
						h1 = hp1->begin() + start1;
						int oi = i;
						for(i = hp2->найди(*h1); i >= 0 && i <= oi; i = hp2->найдиСледщ(i))
							;
						continue;
					}
				}
			}
			i = hp2->найдиСледщ(i);
		}
	return done;
}

void TextComparator::разбей(Массив<ТекстСекция>& dest, int start1, int end1, int start2, int end2) const
{
	ПРОВЕРЬ(start1 <= end1 && start2 <= end2);
	while(start1 < end1 && start2 < end2) {
		int new1 = -1, new2 = -1, count = 0;
		for(int i = start1; i + count < end1; i++)
			if(найди(i, end1, start2, end2, new2, count))
				new1 = i;
		if(count == 0)
			break; // no match at all
		ПРОВЕРЬ(new1 >= start1 && new1 + count <= end1);
		ПРОВЕРЬ(new2 >= start2 && new2 + count <= end2);
		dest.добавь(ТекстСекция(new1, count, new2, count, true));
		if(new1 - start1 >= end1 - new1 - count) { // head is longer - recurse for tail
			разбей(dest, new1 + count, end1, new2 + count, end2);
			end1 = new1;
			end2 = new2;
		}
		else { // tail is longer - recurse for head
			разбей(dest, start1, new1, start2, new2);
			start1 = new1 + count;
			start2 = new2 + count;
		}
		ПРОВЕРЬ(start1 <= end1 && start2 <= end2);
	}
	if(start1 < end1 || start2 < end2)
		dest.добавь(ТекстСекция(start1, end1 - start1, start2, end2 - start2, false));
}

};
