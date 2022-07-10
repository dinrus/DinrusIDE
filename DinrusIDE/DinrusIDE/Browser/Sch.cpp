#include "Browser.h"

Ткст   SchId(СиПарсер& p, Ткст& rr)
{
	p.сим('(');
	Ткст id = p.читайИд();
	rr << "SqlId " << id << "(\"" << id << "\");";
	return id;
}

Ткст читайИд(СиПарсер& p, Ткст& rr)
{
	Ткст id = SchId(p, rr);
	p.сим(',');
	if(p.число_ли())
		p.читайЧисло();
	p.сим(',');
	if(p.число_ли())
		p.читайЧисло();
	p.сим(')');
	return id;
}

Вектор<Ткст> PreprocessSchFile(const char *фн)
{
	Ткст s = загрузиФайл(фн);
	СиПарсер p(s);
	Ткст r = "using namespace РНЦП;";
	Ткст rr = "using namespace РНЦП;";
	Ткст S_name;
	int line;
	while(!p.кф_ли())
		try {
			line = p.дайСтроку();
			if(p.ид("TABLE") || p.ид("TABLE_") || p.ид("TYPE") || p.ид("TYPE_")) {
				S_name = "S_" + SchId(p, rr);
				r << "struct " << S_name << " {";
			}
			else
			if(p.ид("TABLE_I") || p.ид("TABLE_I_") || p.ид("TYPE_I") || p.ид("TYPE_I_")) {
				S_name = "S_" + SchId(p, rr);
				p.сим(',');
				Ткст S_base = "S_" + p.читайИд();
				r << "struct " << S_name << " : " << S_base << " {";
			}
			else
			if(p.ид("TABLE_II") || p.ид("TABLE_II_") || p.ид("TYPE_II") || p.ид("TYPE_II_")) {
				S_name = "S_" + SchId(p, rr);
				p.сим(',');
				Ткст S_base = "S_" + p.читайИд();
				p.сим(',');
				Ткст S_base2 = "S_" + p.читайИд();
				r << "struct " << S_name << " : " << S_base << ", " << S_base2 << " {";
			}
			else
			if(p.ид("TABLE_III") || p.ид("TABLE_III_") || p.ид("TYPE_III") || p.ид("TYPE_III_")) {
				S_name = "S_" + SchId(p, rr);
				p.сим(',');
				Ткст S_base = "S_" + p.читайИд();
				p.сим(',');
				Ткст S_base2 = "S_" + p.читайИд();
				p.сим(',');
				Ткст S_base3 = "S_" + p.читайИд();
				r << "struct " << S_name << " : " << S_base << ", " << S_base2 << ", " << S_base3 << " {";
			}
			else
			if(p.ид("END_TABLE") || p.ид("END_TYPE"))
				r << "\t"
					"static const char           TableName[];"
					"static const SqlSet&        ColumnSet();"
					"static SqlSet               ColumnSet(const Ткст& prefix);"
					"static SqlSet               Of(SqlId table);"
					"static const Вектор<SqlId>& GetColumnIds();"
					""
					"void                        очисть();"
					""
					"void                        FieldLayoutRaw(FieldOperator& f, const Ткст& prefix = Ткст());"
					"void                        FieldLayout(FieldOperator& f);"
					"operator                    Fields();"
					""
					"bool                        operator==(const " << S_name << "& x) const;"
					"bool                        operator!=(const " << S_name << "& x) const;"
					"Ткст                      вТкст() const;"
					""
					"int                         дайСчёт() const;"
					"SqlId                       дайИд(int i) const;"
					"Реф                         дайРеф(int i);"
					"Реф                         дайРеф(const SqlId& id);"
					"Значение                       дай(const SqlId& id) const;"
					"Значение                       дай(int i) const;"
					"МапЗнач                    дай() const;"
					"void                        уст(int i, const Значение& v);"
					"void                        уст(const SqlId& id, const Значение& v);"
					"void                        уст(const МапЗнач& m);"
					<< S_name << "();"
					<< S_name << "(const МапЗнач& m);"
					"};";
			else
			if(p.ид("LONGRAW") || p.ид("LONGRAW_") || p.ид("BLOB") || p.ид("BLOB_") ||
			   p.ид("STRING_") || p.ид("STRING") || p.ид("CLOB") || p.ид("CLOB_"))
				r << "\tString " << читайИд(p, rr) << ";";
			else
			if(p.ид("INT") || p.ид("INT_") || p.ид("ISERIAL"))
				r << "\tint " << читайИд(p, rr) << ";";
			else
			if(p.ид("INT64") || p.ид("INT64_") || p.ид("SERIAL") || p.ид("BIGSERIAL"))
				r << "\tint64 " << читайИд(p, rr) << ";";
			else
			if(p.ид("DOUBLE") || p.ид("DOUBLE_"))
				r << "\tdouble " << читайИд(p, rr) << ";";
			else
			if(p.ид("TIME") || p.ид("TIME_"))
				r << "\tTime " << читайИд(p, rr) << ";";
			else
			if(p.ид("DATE") || p.ид("DATE_"))
				r << "\tDate " << читайИд(p, rr) << ";";
			else
			if(p.ид("BIT") || p.ид("BIT_") || p.ид("BOOL") || p.ид("BOOL_"))
				r << "\tbool " << читайИд(p, rr) << ";";
			else
			if(p.ид("INT_ARRAY") || p.ид("INT_ARRAY_"))
				r << "\tint " << читайИд(p, rr) << "[1];";
			else
			if(p.ид("DOUBLE_ARRAY") || p.ид("DOUBLE_ARRAY_"))
				r << "\tdouble " << читайИд(p, rr) << "[1];";
			else
			if(p.ид("TIME_ARRAY") || p.ид("TIME_ARRAY_"))
				r << "\tTime " << читайИд(p, rr) << "[1];";
			else
			if(p.ид("DATE_ARRAY") || p.ид("DATE_ARRAY_"))
				r << "\tDate " << читайИд(p, rr) << "[1];";
			else
			if(p.ид("BOOL_ARRAY") || p.ид("BOOL_ARRAY_") || p.ид("BIT_ARRAY") || p.ид("BIT_ARRAY_"))
				r << "\tbool " << читайИд(p, rr) << "[1];";
			else
			if(p.ид("STRING_ARRAY") || p.ид("STRING_ARRAY_"))
				r << "\tbool " << читайИд(p, rr) << "[1];";
			else
			if(p.ид("SEQUENCE") || p.ид("SEQUENCE_"))
				читайИд(p, rr);
			else
				p.пропустиТерм();
			line = p.дайСтроку() - line;
			for(int i = 0; i < line; ++i) {
				r << '\n';
				rr << '\n';
			}
		}
		catch(СиПарсер::Ошибка)
		{}
	return pick(Вектор<Ткст>() << r << rr);
}
