#include "CppBase.h"
#include "Internal.h"

namespace РНЦП {

#define LLOG(x)
#define LTIMING(x)  // DTIMING(x)

ScopeInfo::ScopeInfo(const CppBase& base, int scopei)
	: scopei(scopei), base(base)
{
	LTIMING("ScopeInfo(const CppBase& base, int scopei)");
	иниц();
}

ScopeInfo::ScopeInfo(int scopei, const CppBase& base)
	: scopei(scopei), base(base)
{
	LTIMING("ScopeInfo(int scopei, const CppBase& base)");
	иниц();
}

ScopeInfo::ScopeInfo(const CppBase& base, const Ткст& scope)
	: scopei(base.найди(scope)), base(base)
{
	LTIMING("ScopeInfo(const CppBase& base, const Ткст& scope)");
	иниц();
}

ScopeInfo::ScopeInfo(const ScopeInfo& f)
	: base(f.base)
{
	LTIMING("ScopeInfo copy contructor");
	scopes <<= f.scopes;
	bvalid = nvalid = false;
	scopei = f.scopei;
}

void ScopeInfo::иниц()
{
	bvalid = nvalid = false;
}

void ScopeInfo::Bases(int i, Вектор<int>& g)
{ // recursively retrieve all base classes
	if(base.IsType(i)) {
		const Массив<CppItem>& n = base[i];
		for(int i = 0; i < n.дайСчёт(); i++) {
			const CppItem& im = n[i];
			if(im.IsType()) {
				const char *q = im.qptype;
				const char *b = q;
				for(;;) {
					if(*q == ';' || *q == '\0') {
						if(b < q) {
							Ткст h = Ткст(b, q);
							int q = h.найди('<');
							if(q >= 0)
								h.обрежь(q);
							h = обрежьОба(h);
							int nq = base.найди(h);
							if(nq >= 0)
								g.добавь(nq);
						}
						if(*q == '\0')
							break;
						q++;
						b = q;
					}
					else
						q++;
				}
			}
		}
	}
}

const Вектор<Ткст>& ScopeInfo::GetBases()
{
	LTIMING("GetBases");
	if(!bvalid) {
		bvalid = true;
		baselist.очисть();
		if(scopei < 0)
			return baselist;
		Вектор<int> b;
		Индекс<int> bi;
		Bases(scopei, b);
		while(b.дайСчёт()) {
			Вектор<int> bb;
			for(int i = 0; i < b.дайСчёт(); i++) {
				int q = b[i];
				if(bi.найди(q) < 0) {
					bi.добавь(q);
					Bases(b[i], bb);
				}
			}
			b = pick(bb);
		}
		for(int i = 0; i < bi.дайСчёт(); i++)
			baselist.добавь(base.дайКлюч(bi[i]) + "::");
	}
	return baselist;
}

const Вектор<Ткст>& ScopeInfo::GetScopes(const Ткст& usings_)
{
	LTIMING("GetScopes");
	if(!nvalid || usings != usings_) {
		usings = usings_;
		nvalid = true;
		scopes.очисть();
		if(scopei >= 0) {
			Ткст nn = base.дайКлюч(scopei);
			while(nn.дайСчёт()) {
				if(nn[0] == ':' && nn.дайСчёт() == 2) {
					scopes.добавь(nn);
					return scopes;
				}
				scopes.добавь(nn + "::");
				int q = nn.найдирек(':');
				nn.обрежь(max(0, q - 1));
			}
		}
		scopes.добавь("");
		Вектор<Ткст> h = разбей(usings, ';');
		for(int i = 0; i < h.дайСчёт(); i++)
			scopes.добавь(h[i] + "::");
	}
	return scopes;
}

}
