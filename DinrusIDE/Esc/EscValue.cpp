#include <Esc/Esc.h>


namespace РНЦП {

#define LTIMING(x)  // RTIMING(x)

Ткст EscTypeName(int sv_type)
{
	switch(sv_type)
	{
	case ESC_VOID:   return "void";
	case ESC_DOUBLE: return "double";
	case ESC_INT64:  return "int64";
	case ESC_ARRAY:  return "array";
	case ESC_MAP:    return "map";
	case ESC_LAMBDA: return "lambda";
	default:         return фмт("unknown(%d)", sv_type);
	}
}

int EscValue::total;
int EscValue::max_total = 1000000;

void EscValue::освободи()
{
	LTIMING("освободи");
	if(тип == ESC_ARRAY)
		array->отпусти();
	if(тип == ESC_MAP)
		map->отпусти();
	if(тип == ESC_LAMBDA)
		lambda->отпусти();
	тип = ESC_VOID;
	hash = 0;
}

EscValue::~EscValue()
{
	LTIMING("~EscValue");
	total--;
	освободи();
}

void EscValue::присвой(const EscValue& s)
{
	LTIMING("присвой");
	тип = s.тип;
	hash = s.hash;
	switch(тип) {
	case ESC_ARRAY:
		array = s.array;
		array->Retain();
		break;
	case ESC_MAP:
		map = s.map;
		map->Retain();
		break;
	case ESC_LAMBDA:
		lambda = s.lambda;
		lambda->Retain();
		break;
	case ESC_DOUBLE:
		number = s.number;
		break;
	case ESC_INT64:
		i64 = s.i64;
		break;
	}
}

EscValue& EscValue::operator=(const EscValue& s)
{
	LTIMING("Sval=Sval");
	освободи();
	присвой(s);
	return *this;
}

EscValue::EscValue(const EscValue& s)
{
	LTIMING("Sval(Sval)");
	присвой(s);
	total++;
}

EscLambda& EscValue::CreateLambda()
{
	освободи();
	lambda = new EscLambda;
	тип = ESC_LAMBDA;
	hash = 0;
	return *lambda;
}

const EscLambda& EscValue::GetLambda() const
{
	ПРОВЕРЬ(IsLambda());
	return *lambda;
}

int EscValue::дайСчёт() const
{
	switch(тип) {
	case ESC_VOID:
		return 0;
	case ESC_ARRAY:
		return array->array.дайСчёт();
	case ESC_MAP:
		return map->count;
	}
	return 1;
}

hash_t EscValue::дайХэшЗнач() const
{
	LTIMING("дайХэшЗнач");
	if(hash == 0) {
		switch(тип) {
		case ESC_VOID:
			hash = 1;
			break;
		case ESC_DOUBLE:
			hash = РНЦП::дайХэшЗнач(number) | 0x80000;
			break;
		case ESC_INT64:
			hash = РНЦП::дайХэшЗнач(i64) | 0x80000;
			break;
		case ESC_ARRAY:
			for(int i = 0; i < array->array.дайСчёт(); i++)
				hash = hash ^ array->array[i].дайХэшЗнач();
			hash |= 0x40000;
			break;
		case ESC_MAP:
			for(int i = 0; i < map->map.дайСчёт(); i++)
				if(!map->map[i].проц_ли())
					hash ^= map->map.дайКлюч(i).дайХэшЗнач() ^ map->map[i].дайХэшЗнач();
			hash |= 0x8000000;
			break;
		case ESC_LAMBDA:
			hash = РНЦП::дайХэшЗнач(lambda->code) | 0x4000000;
			break;
		}
	}
	return hash;
}

template <class T>
bool EqVector(const T& a, const T& b)
{
	if(a.дайСчёт() != b.дайСчёт())
		return false;
	for(int i = 0; i < a.дайСчёт(); i++)
		if(a[i] != b[i])
			return false;
	return true;
}

bool EscValue::operator==(const EscValue& a) const
{
	LTIMING("operator==");
	int i;
	switch(тип) {
	case ESC_VOID:
		return a.тип == ESC_VOID;
	case ESC_INT64:
		if(a.тип == ESC_INT64)
			return i64 == a.i64;
	case ESC_DOUBLE:
		return a.число_ли() && GetNumber() == a.GetNumber();
	case ESC_ARRAY:
		if(a.тип != ESC_ARRAY) return false;
		if(array->array.дайСчёт() != a.array->array.дайСчёт())
			return false;
		return EqVector(array->array, a.array->array);
	case ESC_MAP: {
		if(a.тип != ESC_MAP) return false;
		const ВекторМап<EscValue, EscValue>& x = дайМап();
		const ВекторМап<EscValue, EscValue>& y = a.дайМап();
		for(i = 0; i < x.дайСчёт(); i++)
			if(!x.отлинкован(i)) {
				int q = y.найди(x.дайКлюч(i));
				if(q < 0)
					return false;
				if(x[i] != y[q])
					return false;
			}
		for(i = 0; i < y.дайСчёт(); i++)
			if(!y.отлинкован(i))
				if(x.найди(y.дайКлюч(i)) < 0)
					return false;
		return true;
	}
	case ESC_LAMBDA:
		return lambda->code == a.lambda->code &&
		       EqVector(lambda->arg, a.lambda->arg) && EqVector(lambda->inout, a.lambda->inout);
	}
	return false;
}

Ткст EscValue::вТкст(int maxlen, int indent_step, bool hex, int indent) const
{
	Ткст ind(' ', indent);
	ТкстБуф r;
	Ткст s;
	int i;
	switch(тип) {
	case ESC_DOUBLE:
		{
			s = ind;
			if((int64)number == number)
				s << фмтЦел64((int64)number);
			else
				s << фмт("%.8g", number);
			if(hex && FitsInInt64(number) && (int64)number == number)
				s << " 0x" << фмт64Гекс((int64)number);
			if(hex && number >= 0 && number < 65536 && (int)number == number)
				s << ' ' << какТкстСи(вУтф8(ШТкст((int)number, 1)));
			return s;
		}
	case ESC_INT64:
		{
			s = ind;
			s << фмтЦел64(i64);
			if(hex)
				s << " 0x" << фмт64Гекс(i64);
			if(hex && i64 >= 0 && i64 < 65536)
				s << ' ' << какТкстСи(вУтф8(ШТкст((int)i64, 1)));
			return s;
		}
	case ESC_ARRAY:
		{
			const Вектор<EscValue>& a = дайМассив();
			int i;
			int c = min(100, a.дайСчёт());
			for(i = 0; i < a.дайСчёт(); i++) {
				if(!a[i].цел_ли())
					break;
				int c = a[i].дайЦел();
				if(c >= 32 && c < 256)
					s.конкат(c);
				else
					break;
			}
			if(i < a.дайСчёт()) {
				r << ind << "[ ";
				for(i = 0; i < c; i++) {
					if(i)
						r << ", ";
					r << array->array[i].вТкст(maxlen, indent_step, hex, 0);
				}
				r << " ]";
				if(r.дайДлину() >= maxlen) {
					r.очисть();
					r << ind << "[\n";
					for(i = 0; i < c; i++) {
						if(i)
							r << ",\n";
						r << array->array[i].вТкст(maxlen, indent_step, hex, indent + indent_step);
					}
					r << '\n' << ind << "]";
				}
				s = r;
			}
			else {
				r << ind << '\"' << s << '\"';
				s = r;
			}
			if(a.дайСчёт() > 100)
				s << ind << "\n...more than 100 elements";
			return s;
		}
	case ESC_LAMBDA:
		r << ind << "@(";
		for(i = 0; i < lambda->arg.дайСчёт(); i++) {
			if(i)
				r << ", ";
			if(lambda->inout[i])
				r << "&";
			r << lambda->arg[i];
		}
		r << ")\n" << lambda->code;
		return Ткст(r);
	case ESC_MAP:
		r << ind << "{ ";
		int c = min(map->map.дайСчёт(), 100);
		bool q = false;
		for(i = 0; i < c; i++) {
			if(q)
				r << ", ";
			if(!map->map.отлинкован(i)) {
				r << map->map.дайКлюч(i).вТкст(maxlen, indent_step, hex, 0)
				  << ":" << map->map[i].вТкст(maxlen, indent_step, hex, 0);
				q = true;
			}
		}
		r << " }";
		if(r.дайДлину() >= maxlen) {
			r.очисть();
			r << ind << "{\n";
			q = false;
			for(i = 0; i < c; i++) {
				if(q)
					r << ",\n";
				if(!map->map.отлинкован(i)) {
					r << map->map.дайКлюч(i).вТкст(maxlen, indent_step, hex, indent + indent_step)
					  << ":\n" << map->map[i].вТкст(maxlen, indent_step, hex, indent + indent_step);
					q = true;
				}
			}
			r << '\n' << ind << "}";
		}
		s = r;
		if(map->map.дайСчёт() > 100)
			s << ind << "\n...more than 100 elements";
		return s;
	}
	return "void";
}

double EscValue::GetNumber() const
{
	if(тип == ESC_INT64)
		return (double)i64;
	if(тип == ESC_DOUBLE)
		return number;
	return 0;
}

int64 EscValue::дайЦел64() const
{
	if(тип == ESC_INT64)
		return i64;
	if(тип == ESC_DOUBLE && number >= (double)INT64_MIN && number <= (double)INT64_MAX)
		return (int64)number;
	return 0;
}

bool EscValue::цел_ли() const
{
	if(IsInt64()) {
		int64 n = дайЦел64();
		return n >= INT_MIN && n <= INT_MAX;
	}
	if(число_ли()) {
		double n = GetNumber();
		return n >= INT_MIN && n <= INT_MAX;
	}
	return false;
}

int EscValue::дайЦел() const
{
	return цел_ли() ? (int)дайЦел64() : 0;
}

bool IsTrue(const EscValue& a)
{
	if(a.число_ли())
		return a.GetNumber();
	return a.дайСчёт();
}

}
