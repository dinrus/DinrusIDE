struct ИнфОКлюче {
	const char *имя;
	dword ключ[4];
};

NTL_MOVEABLE(ИнфОКлюче)

void регистрируйПривязкуКлюча(const char *группа, const char *id, ИнфОКлюче& (*info)());

ИнфОКлюче& AK_NULL();

void   редактируйКлючи();

Ткст сохраниКлючи();
void   восстановиКлючи(const Ткст& data);

dword разбериОписКлюча(СиПарсер& p);

Ткст дайОпис(const ИнфОКлюче& f, bool parenthesis = true);

bool   сверь(const ИнфОКлюче& k, dword ключ);

inline
bool   сверь(ИнфОКлюче& (*k)(), dword ключ) { return сверь((*k)(), ключ); }
