#include "Debuggers.h"

#ifdef PLATFORM_WIN32

uint32 Pdb::дайРегистр32(const Контекст& ctx, int sym)
{
	switch(sym) {
#define РЕГ_ЦПБ(sym, context_var, kind, имя, flags) case sym: return ctx.context32.context_var;
		#include "i386.cpu"
#undef РЕГ_ЦПБ
	}
	return 0;
}

#ifdef CPU_64
uint64 Pdb::дайРегистр64(const Контекст& ctx, int sym)
{
	switch(sym) {
#define РЕГ_ЦПБ(sym, context_var, kind, имя, flags) case sym: return ctx.context64.context_var;
		#include "amd64.cpu"
#undef РЕГ_ЦПБ
	}
	return 0;
}
#endif

const ВекторМап<int, Pdb::РегистрЦпб>& Pdb::дайСписокРегистров()
{
	static ВекторМап<int, РегистрЦпб> r32;
	ONCELOCK {
#define РЕГ_ЦПБ(sym_, context_var, kind_, name_, flags_) { РегистрЦпб& r = r32.добавь(sym_); r.sym = sym_; r.kind = kind_; r.имя = name_; r.flags = flags_; }
		#include "i386.cpu"
#undef РЕГ_ЦПБ
	}
#ifdef CPU_64
	static ВекторМап<int, РегистрЦпб> r64;
	ONCELOCK {
#define РЕГ_ЦПБ(sym_, context_var, kind_, name_, flags_) { РегистрЦпб& r = r64.добавь(sym_); r.sym = sym_; r.kind = kind_; r.имя = name_; r.flags = flags_; }
		#include "amd64.cpu"
#undef РЕГ_ЦПБ
	}
	return win64 ? r64 : r32;
#else
	return r32;
#endif
}

uint64 Pdb::дайРегистрЦпб(const Контекст& ctx, int sym)
{
	int q = дайСписокРегистров().найди(sym);
	if(q < 0)
		return 0;
	const РегистрЦпб& r = дайСписокРегистров()[q];
#ifdef CPU_64
	uint64 val = win64 ? дайРегистр64(ctx, sym) : дайРегистр32(ctx, sym);
#else
	uint64 val = дайРегистр32(ctx, sym);
#endif
	switch(r.kind) {
	case REG_L:
		return LOBYTE(val);
	case REG_H:
		return HIBYTE(val);
	case REG_X:
		return LOWORD(val);
	case REG_E:
		return LODWORD(val);
	}
	return val;
}

#endif
