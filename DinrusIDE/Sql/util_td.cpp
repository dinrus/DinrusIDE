#include "Sql.h"

namespace РНЦП {

void td_scalar(SqlSet& set, const Ткст& prefix, const char *x) {
	set.конкат(SqlId(prefix + x));
}

void td_array(SqlSet& set, const Ткст& prefix, const char *x, int cnt) {
	Ткст имя = prefix + x;
	for(int i = 0; i < cnt; i++)
		set.конкат(SqlId(имя + фмт("%d", i)));
}

void td_var(SqlSet& set, const Ткст& prefix, const char *x, SqlSet (*f)(const Ткст&)) {
	set.конкат((*f)(prefix + x + '$'));
}

void td_shrink(Ткст *array, int cnt) {
	Ткст *lim = array + cnt;
	while(array < lim) {
		array->сожми();
		array++;
	}
}

struct NfEqual : FieldOperator {
	МассивЗнач va;
	
	virtual void Field(const char *имя, Реф f) {
		va.добавь(f);
	}
};

bool EqualFields(Fields a, Fields b)
{
	NfEqual fa, fb;
	a(fa);
	b(fb);
	return fa.table == fb.table && fa.va == fb.va;
}

struct NfAsString : FieldOperator {
	Ткст text;
	
	virtual void Field(const char *имя, Реф f) {
		text << "\n\t" << имя << '=' << (Значение)f;
	}
};


Ткст какТкст(Fields a)
{
	NfAsString x;
	a(x);
	return x.text;
}

}
