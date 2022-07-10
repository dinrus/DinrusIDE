#include "Sql.h"

namespace РНЦП {

SqlId SqlId::Of(const char *of) const
{
	return Ткст().конкат() << of << (char)SQLC_OF << вТкст();
}

SqlId SqlId::Of(SqlId id) const
{
	return id.пусто_ли() ? вТкст() : Ткст().конкат() << id.вТкст() << (char)SQLC_OF << вТкст();
}

void SqlId::PutOf0(Ткст& s, const SqlId& b) const
{
	s << вТкст() << (char)SQLC_OF << ~b;
}

void SqlId::PutOf(Ткст& s, const SqlId& b) const
{
	s << (char)SQLC_COMMA;
	PutOf0(s, b);
}

SqlId SqlId::operator[](const SqlId& id) const
{
	return Of(id);
}

SqlId SqlId::As(const char *as) const
{
	return id.пусто_ли() ? вТкст() : Ткст().конкат() << вТкст() << (char)SQLC_AS << as;
}

SqlId SqlId::operator()(SqlId p) const
{
	Ткст x;
	PutOf0(x, p);
	return x;
}

SqlId SqlId::operator [] (int i) const
{
	return вТкст() + фмтЦел(i);
}

SqlId SqlId::operator&(const SqlId& s) const
{
	return вТкст() + "$" + s.вТкст();
}

SqlId SqlId::operator()(const S_info& table) const
{
	Ткст x;
	for(int i = 0; i < table.дайСчёт(); i++)
		if(i)
			PutOf(x, table.дайИд(i));
		else
			PutOf0(x, table.дайИд(i));
	return x;
}

Ткст SqlS::operator()() const
{
	return Ткст().конкат() << '(' << text << ')';
}

Ткст SqlS::operator()(int at) const
{
	return at > priority ? operator()() : text;
}

Ткст SqlS::operator()(int at, byte cond) const
{
	if(at <= priority)
		return text;
	ТкстБуф out;
	out << SqlCode(cond, "(")() << text << SqlCode(cond, ")")();
	return Ткст(out);
}

void SqlS::иниц(const SqlS& a, const char *o, int olen, const SqlS& b, int pr, int prb)
{
	ТкстБуф s;
	if(a.priority < pr) {
		s.конкат('(');
		s.конкат(~a);
		s.конкат(')');
	}
	else
		s.конкат(~a);
	s.конкат(o, olen);
	if(b.priority < prb) {
		s.конкат('(');
		s.конкат(~b);
		s.конкат(')');
	}
	else
		s.конкат(~b);
	text = s;
	priority = pr;
}

SqlVal SqlVal::As(const char *as) const {
	SqlVal v;
	v.SetHigh(Ткст().конкат() << text << (char)SQLC_AS << '\t' << as << '\t');
	return v;
}

SqlVal SqlVal::As(const SqlId& id) const {
	return As(~id.вТкст());
}

SqlVal::SqlVal(const Ткст& x) {
	if(РНЦП::пусто_ли(x))
		устПусто();
	else
		SetHigh(SqlFormat(x));
}

SqlVal::SqlVal(const char *s) {
	if(s && *s)
		SetHigh(SqlFormat(s));
	else
		устПусто();
}

SqlVal::SqlVal(int x) {
	if(РНЦП::пусто_ли(x))
		устПусто();
	else
		SetHigh(SqlFormat(x));
}

SqlVal::SqlVal(int64 x) {
	if(РНЦП::пусто_ли(x))
		устПусто();
	else
		SetHigh(SqlFormat(x));
}

SqlVal::SqlVal(double x) {
	if(РНЦП::пусто_ли(x))
		устПусто();
	else
		SetHigh(SqlFormat(x));
}

SqlVal::SqlVal(Дата x) {
	if(РНЦП::пусто_ли(x))
		устПусто();
	else
		SetHigh(SqlFormat(x));
}

SqlVal::SqlVal(Время x) {
	if(РНЦП::пусто_ли(x))
		устПусто();
	else
		SetHigh(SqlFormat(x));
}

SqlVal::SqlVal(const Значение& x) {
	if(РНЦП::пусто_ли(x))
		устПусто();
	else
		SetHigh(SqlFormat(x));
}

SqlVal::SqlVal(const Обнул&) {
	устПусто();
}

SqlVal::SqlVal(const SqlId& id) {
	SetHigh(id.Quoted());
}

SqlVal::SqlVal(const SqlId& (*id)())
{
	SetHigh((*id)().Quoted());
}

SqlVal::SqlVal(const SqlCase& x) {
	SetHigh(~x);
}

SqlVal operator-(const SqlVal& a) {
	return SqlVal('-' + a(SqlS::UNARY), SqlS::UNARY);
}

SqlVal operator+(const SqlVal& a, const SqlVal& b) {
	return SqlVal(a, " + ", b, SqlS::ADD);
}

SqlVal operator-(const SqlVal& a, const SqlVal& b) {
	return SqlVal(a," - ", b, SqlS::ADD, SqlS::ADD + 1);
}

SqlVal operator*(const SqlVal& a, const SqlVal& b) {
	return SqlVal(a, " * ", b, SqlS::MUL);
}

SqlVal operator/(const SqlVal& a, const SqlVal& b) {
	return SqlVal(a, " / ", b, SqlS::MUL, SqlS::MUL + 1);
}

SqlVal operator%(const SqlVal& a, const SqlVal& b) {
	return SqlFunc("mod", a, b);
}

SqlVal operator|(const SqlVal& a, const SqlVal& b) {
	return SqlVal(a, SqlCode(ORACLE|PGSQL|SQLITE3, "||")(" + "), b, SqlS::MUL);
}

SqlVal& operator+=(SqlVal& a, const SqlVal& b)     { return a = a + b; }
SqlVal& operator-=(SqlVal& a, const SqlVal& b)     { return a = a - b; }
SqlVal& operator*=(SqlVal& a, const SqlVal& b)     { return a = a * b; }
SqlVal& operator/=(SqlVal& a, const SqlVal& b)     { return a = a / b; }
SqlVal& operator%=(SqlVal& a, const SqlVal& b)     { return a = a % b; }
SqlVal& operator|=(SqlVal& a, const SqlVal& b)     { return a = a | b; }

SqlVal SqlFunc(const char *имя, const SqlVal& a) {
	return SqlVal(Ткст().конкат() << имя << '(' << ~a << ')', SqlS::FN);
}

SqlVal SqlFunc(const char *n, const SqlVal& a, const SqlVal& b) {
	return SqlVal(Ткст(n).конкат() << '(' + ~a << ", " << ~b << ')', SqlS::FN);
}

SqlVal SqlFunc(const char *n, const SqlVal& a, const SqlVal& b, const SqlVal& c) {
	return SqlVal(Ткст(n).конкат() << '(' << ~a << ", " << ~b << ", " << ~c << ')', SqlS::FN);
}

SqlVal SqlFunc(const char *n, const SqlVal& a, const SqlVal& b, const SqlVal& c, const SqlVal& d) {
	return SqlVal(Ткст(n).конкат() << '(' << ~a << ", " << ~b << ", " << ~c << ", " << ~d << ')', SqlS::FN);
}

SqlVal SqlFunc(const char *имя, const SqlSet& set) {
	return SqlVal(имя + set(), SqlS::FN);
}

SqlBool SqlBoolFunc(const char *имя, const SqlBool& a) {
	return SqlBool(Ткст().конкат() << имя << '(' << ~a << ')', SqlS::FN);
}

SqlBool SqlBoolFunc(const char *n, const SqlBool& a, const SqlBool& b) {
	return SqlBool(Ткст(n).конкат() << '(' + ~a << ", " << ~b << ')', SqlS::FN);
}

SqlBool SqlBoolFunc(const char *n, const SqlBool& a, const SqlBool& b, const SqlBool& c) {
	return SqlBool(Ткст(n).конкат() << '(' << ~a << ", " << ~b << ", " << ~c << ')', SqlS::FN);
}

SqlBool SqlBoolFunc(const char *n, const SqlBool& a, const SqlBool& b, const SqlBool& c, const SqlBool& d) {
	return SqlBool(Ткст(n).конкат() << '(' << ~a << ", " << ~b << ", " << ~c << ", " << ~d << ')', SqlS::FN);
}

SqlVal Decode(const SqlVal& exp, const SqlSet& variants) {
	ПРОВЕРЬ(!variants.пустой());
	Вектор<SqlVal> v = variants.разбей();
	ПРОВЕРЬ(v.дайСчёт() > 1);
	SqlCase cs(exp == v[0], v[1]);
	int i;
	for(i = 2; i + 1 < v.дайСчёт(); i += 2)
		cs(exp == v[i], v[i + 1]);
	if(i < v.дайСчёт())
		cs(v[i]);
	else
		cs(Null);
	return SqlVal(SqlCode(ORACLE, "decode("  + ~exp  + ", " + ~variants + ')')('(' + ~cs + ')'),
	              SqlS::FN);
}

SqlVal Distinct(const SqlVal& exp) {
	return SqlVal("distinct " + exp(SqlS::ADD), SqlS::UNARY);
}

SqlSet Distinct(const SqlSet& columns) {
	return SqlSet("distinct " + ~columns, SqlSet::SET);
}

SqlVal All(const SqlVal& exp) {
	return SqlVal("all " + exp(SqlS::ADD), SqlS::UNARY);
}

SqlSet All(const SqlSet& columns) {
	return SqlSet("all " + ~columns, SqlSet::SET);
}

SqlVal Count(const SqlVal& exp)
{
	return SqlFunc("count", exp);
}

SqlVal Count(const SqlSet& exp)
{
	return SqlFunc("count", exp);
}

SqlId SqlAll()
{
	return SqlId("*");
}

SqlVal SqlCountRows()
{
	return Count(SqlAll());
}

SqlVal Descending(const SqlVal& exp) {
	return SqlVal(exp(SqlS::ADD) + " desc", SqlS::UNARY);
}

SqlVal SqlMax(const SqlVal& exp) {
	return SqlFunc("max", exp);
}

SqlVal SqlMin(const SqlVal& exp) {
	return SqlFunc("min", exp);
}

SqlVal Sqlсумма(const SqlVal& exp) {
	return SqlFunc("sum", exp);
}

SqlVal Avg(const SqlVal& a) {
	return SqlFunc("avg", a);
}

SqlVal Abs(const SqlVal& a) {
	return SqlFunc("абс", a);
}

SqlVal Stddev(const SqlVal& a) {
	return SqlFunc("stddev", a);
}

SqlVal Variance(const SqlVal& a) {
	return SqlFunc("variance", a);
}

SqlVal Greatest(const SqlVal& a, const SqlVal& b) {
	return SqlVal(SqlCode(MSSQL, '(' + ~Case(a < b, b)(a) + ')')
	                     (~SqlFunc(SqlCode(SQLITE3, "max")("greatest"), a, b)),
	              SqlS::FN);
}

SqlVal Least(const SqlVal& a, const SqlVal& b) {
	return SqlVal(SqlCode(MSSQL, '(' + ~Case(a > b, b)(a) + ')')
	                     (~SqlFunc(SqlCode(SQLITE3, "min")("least"), a, b)),
	              SqlS::FN);
}

SqlVal преобразуйНабСим(const SqlVal& exp, const SqlVal& charset) { //TODO Dialect!
	if(exp.пустой()) return exp;
	return exp.пустой() ? exp : SqlFunc("convert", exp, charset);
}

SqlVal ConvertAscii(const SqlVal& exp) {
	return SqlVal(SqlCode(MSSQL, Ткст().конкат() << "((" << ~exp << ") collate SQL_Latin1_General_Cp1251_CS_AS)")
			             (~преобразуйНабСим(exp, "US7ASCII")), SqlS::FN); // This is Oracle really, TODO: добавь other dialects
}

SqlVal Upper(const SqlVal& exp) {
	return exp.пустой() ? exp : SqlFunc("upper", exp);
}

SqlVal Lower(const SqlVal& exp) {
	return exp.пустой() ? exp : SqlFunc("lower", exp);
}

SqlVal длина(const SqlVal& exp) {
	return exp.пустой() ? exp : SqlFunc(SqlCode(MSSQL, "len")("length"), exp);
}

SqlVal UpperAscii(const SqlVal& exp) {
	return exp.пустой() ? exp : Upper(ConvertAscii(exp));
}

SqlVal Substr(const SqlVal& a, const SqlVal& b) {
	return SqlFunc("SUBSTR", a, b);
}

SqlVal Substr(const SqlVal& a, const SqlVal& b, const SqlVal& c)
{
	return SqlFunc(SqlCode(MSSQL, "substring")("substr"), a, b, c);
}

SqlVal Instr(const SqlVal& a, const SqlVal& b) {
	return SqlFunc("INSTR", a, b);
}

SqlVal Wild(const char* s) {
	Ткст result;
	for(char c; (c = *s++) != 0;)
		if(c == '*')
			result << '%';
		else if(c == '?')
			result << '_';
		else if(c == '.' && *s == 0)
			return result;
		else
			result << c;
	result << '%';
	return result;
}

SqlVal SqlDate(const SqlVal& year, const SqlVal& month, const SqlVal& day) {
	return SqlVal(SqlCode(ORACLE, ~SqlFunc("to_date", year|"."|month|"."|day, "SYYYY.MM.DD")) // Similiar to common, but keep proved version to be sure...
	                     (MSSQL, ~SqlFunc("datefromparts", year, month, day))
	                     ("to_date(to_char(" + ~day + ", '00') || to_char(" + ~month +
	                               ", '00') || to_char(" + ~year + ",'9999'), 'DDMMYYYY')"),
	SqlS::FN);
}

SqlVal AddMonths(const SqlVal& date, const SqlVal& months) {//TODO Dialect!
	return SqlFunc("add_months", date, months);
}

SqlVal LastDay(const SqlVal& date) {//TODO Dialect!
	return SqlFunc("last_day", date);
}

SqlVal MonthsBetween(const SqlVal& date1, const SqlVal& date2) {//TODO Dialect!
	return SqlFunc("months_between", date1, date2);
}

SqlVal NextDay(const SqlVal& date) {//TODO Dialect!
	return SqlFunc("next_day", date);
}

SqlVal SqlCurrentDate() {
	return SqlVal(SqlCode(SQLITE3, "date('now')")
	                     ("current_date"), SqlVal::HIGH);
}

SqlVal SqlCurrentTime() {
	return SqlVal(SqlCode(SQLITE3, "datetime('now')")
	                     ("current_timestamp"), SqlVal::HIGH);
}

SqlVal Cast(const char* тип, const SqlId& a) {
	return SqlFunc(тип, a);
}

SqlVal SqlNvl(const SqlVal& a, const SqlVal& b) {
	return SqlFunc(SqlCode
						(PGSQL, "coalesce")
						(MY_SQL|SQLITE3, "ifnull")
						(MSSQL, "isnull")
						("nvl"),
					a, b);
}

SqlVal SqlNvl(const SqlVal& a) {
	return Nvl(a, SqlVal(0));
}

SqlVal Coalesce(const SqlVal& exp1, const SqlVal& exp2) {
	return SqlFunc("coalesce", exp1, exp2);
}

SqlVal Coalesce(const SqlVal& exp1, const SqlVal& exp2, const SqlVal& exp3) {
	return SqlFunc("coalesce", exp1, exp2, exp3);
}

SqlVal Coalesce(const SqlVal& exp1, const SqlVal& exp2, const SqlVal& exp3, const SqlVal& exp4) {
	return SqlFunc("coalesce", exp1, exp2, exp3, exp4);
}

SqlVal Coalesce(const SqlSet& exps) {
	return SqlFunc("coalesce", exps);
}

SqlVal Prior(const SqlId& a) {
	return SqlVal("prior " + a.Quoted(), SqlS::UNARY);
}

SqlVal NextVal(const SqlId& a) {
	return SqlVal(SqlCode
	                 (PGSQL, "nextval('" + a.вТкст() + "')")
	                 (MSSQL, "next значение for " + a.Quoted())
	                 (a.Quoted() + ".NEXTVAL")
	              , SqlS::HIGH);
}

SqlVal CurrVal(const SqlId& a) {
	return SqlVal(SqlCode
				    (PGSQL, "currval('" + a.вТкст() + "')")
				    (a.Quoted() + ".CURRVAL")
				  , SqlS::HIGH);
}

SqlVal SqlTxt(const char *s)
{
	SqlVal v;
	v.SetHigh(s);
	return v;
}

SqlVal SqlRowNum()
{
	return SqlTxt("ROWNUM");
}

SqlVal SqlArg() {
	return SqlTxt("?");
}

SqlVal OuterJoin(const SqlId& col)
{
	return SqlId(col.Quoted() + "(+)");
}

SqlVal SqlBinary(const char *s, int l)
{
	SqlVal x;
	x.SetHigh(SqlFormatBinary(s, l));
	return x;
}

SqlVal SqlBinary(const Ткст& data)
{
	return SqlBinary(~data, data.дайСчёт());
}

}
