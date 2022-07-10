#include "dbf.h"

namespace РНЦП {

#define LLOG(x) // LOG(x)
#define LTIMING(x) // RTIMING(x)

inline int DeleteShareFlag()
{
#ifdef PLATFORM_WIN32
	return IsWinNT() ? ФайлПоток::DELETESHARE : 0;
#else
	return 0;
#endif
}

struct DbfCharset {
	byte dbf_code;
	byte charset;
};

static DbfCharset _dbfCharsets[] = {
//	{ 0x01, CHARSET_UNKNOWN }, // DOS USA, CP437
//	{ 0x02, CHARSET_UNKNOWN }, // DOS multilingual, CP850
	{ 0x03, CHARSET_WIN1252 }, // Windows ANSI, CP1252
//	{ 0x04, CHARSET_UNKNOWN }, // Standard Macintosh
	{ 0x64, CHARSET_CP852   }, // EE MS-DOS, CP852
//	{ 0x65, CHARSET_UNKNOWN }, // Nordic MS-DOS, CP865
//	{ 0x66, CHARSET_UNKNOWN }, // Russian MS-DOS, CP866
//	{ 0x67, CHARSET_UNKNOWN }, // Icelandic MS-DOS
	{ 0x68, CHARSET_MJK     }, // Kamenicky (Czech) MS-DOS
//	{ 0x69, CHARSET_UNKNOWN }, // Mazovia (Polish) MS-DOS
//	{ 0x6A, CHARSET_UNKNOWN }, // Greek MS-DOS (437G)
//	{ 0x6B, CHARSET_UNKNOWN }, // Turkish MS-DOS
//	{ 0x96, CHARSET_UNKNOWN }, // Russian Macintosh
//	{ 0x97, CHARSET_UNKNOWN }, // East European Macintosh
//	{ 0x98, CHARSET_UNKNOWN }, // Greek Macintosh
	{ 0xC8, CHARSET_WIN1250 },
//	{ 0xC9, CHARSET_UNKNOWN }, // Russian Windows
//	{ 0xCA, CHARSET_UNKNOWN }, // Turkish Windows
//	{ 0xCB, CHARSET_UNKNOWN }, // Greek Windows
};

static void sStreamIL(Поток& s, int& x)
{
	if(s.грузится())
		x = s.Get32le();
	else
		s.Put32le(x);
}

static void sStreamIW(Поток& s, int& x)
{
	if(s.грузится())
		x = s.Get16le();
	else
		s.Put16le(x);
}

static int scan_int(const char *p, const char *e = 0)
{
	LTIMING("scan_int");
	bool neg = false;
	if(p != e && (*p == '+' || *p == '-'))
		neg = (*p++ == '-');
	if(p == e || (byte)(*p - '0') >= 10)
		return Null;
	int val = *p++ - '0';
	while(p != e && (byte)(*p - '0') < 10)
		val = 10 * val + *p++ - '0';
	return neg ? -val : val;
}

static double scan_dbl(const char *p, const char *e = 0)
{
	LTIMING("scan_dbl");
	bool neg = false;
	if(p != e && (*p == '+' || *p == '-'))
		neg = (*p++ == '-');
	if(p == e || (byte)(*p - '0') >= 10)
		return Null;
	double mantissa = 0;
	char c;
	int exp = 0;
	while(p != e && (byte)(*p - '0') < 10)
		if((c = *p++) != '0') {
			if(exp) { mantissa *= ipow10(exp); exp = 0; }
			mantissa = 10 * mantissa + c - '0';
		}
		else
			exp++;
	int raise = exp;
	if(p != e && *p == '.') { // decimal part
		while(++p != e && (byte)((c = *p) - '0') < 10)
			if(c != '0') {
				if(raise) { mantissa *= ipow10(raise); exp -= raise; raise = 0; }
				exp--;
				mantissa = 10 * mantissa + c - '0';
			}
			else
				raise++;
	}
	if(p != e && (*p == 'E' || *p == 'e')) { // exponent
		int vexp = scan_int(p + 1, e);
		if(!пусто_ли(vexp))
			exp += vexp;
	}
	if(exp) {
		double e = ipow10(tabs(exp));
		mantissa = (exp > 0 ? mantissa * e : mantissa / e);
	}
	return neg ? -mantissa : mantissa;
}

static int StrBool(char c)
{
	if(c == 'A' || c == 'a' || c == 'Y' || c == 'y' || c == 'T' || c == 't' || c == '1')
		return 1;
	if(c == 'N' || c == 'n' || c == 'F' || c == 'f' || c == '0')
		return 0;
	return (int)Null;
}

DbfStream::Field::Field(const char *_name, char тип, byte width, byte decimal)
: offset(0), тип(тип), width(width), decimal(decimal)
{
	if(_name) {
		const char *p = reinterpret_cast<const char *>(memchr(_name, 0, 11));
		имя = Ткст(_name, p ? (int)(p - _name) : 11);
	}
	имя = взаг(имя);
}

Ткст DbfStream::Field::фмт(Значение значение, byte charset) const
{
	switch(тип) {
		case 'C': {
			if(charset == дайДефНабСим())
				return стдФормат(значение);
			ШТкст s;
			if(значение.дайТип() == WSTRING_V)
				s = значение;
			else
				s = стдФормат(значение).вШТкст();
			return изЮникода(s, charset);
		}

		case 'N':
		case 'F': {
			double v = значение;
			if(пусто_ли(v))
				return Null;
			Ткст out;
			if(decimal == 0) {
				if(width < 10)
					out = фмтЦел(fround(v));
				else
					out = FormatDoubleFix(v, 0);
				if(out.дайДлину() > width) {
					out = (v >= 0 ? "" : "-");
					out.конкат('9', width - out.дайДлину());
				}
			}
			else {
				if(v == 0 || ilog10(fabs(v)) + 2 <= width)
					out = FormatDoubleFix(v, decimal);
				if(out.пустой() || out.дайДлину() > width)
					out = FormatDoubleExp(v, min<int>(decimal, width - 6));
				ПРОВЕРЬ(out.дайДлину() <= width);
			}
			if(out.дайДлину() < width)
				out = Ткст(' ', width - out.дайДлину()) + out;
			return out;
		}

		case 'D': {
			РНЦП::Дата dt = значение;
			if(пусто_ли(dt))
				return Null;
			char p[9];
			p[8] = 0;
			p[7] = dt.day % 10 + '0';
			p[6] = dt.day / 10 + '0';
			p[5] = dt.month % 10 + '0';
			p[4] = dt.month / 10 + '0';
			int y = dt.year;
			p[3] = y % 10 + '0'; y /= 10;
			p[2] = y % 10 + '0'; y /= 10;
			p[1] = y % 10 + '0';
			p[0] = y / 10 + '0';
			return Ткст(p, 8);
		}

		case '!': {
			ТкстБуф temp;
			temp.устСчёт(4);
			Poke32le(~temp, (int)значение);
			return temp;
		}

		case '@': {
			Время t(значение);
			int dt = РНЦП::Дата(t) - РНЦП::Дата(-4713, 1, 1);
			int tm = 1000 * (t.second + 60 * (t.minute + 60 * t.hour));
			ТкстБуф temp;
			temp.устСчёт(8);
			Poke32le(~temp + 0, dt);
			Poke32le(~temp + 4, tm);
			return temp;
		}

		case 'O': {
			union {
				char s[8];
				double d;
			};
			d = значение;
			ТкстБуф temp;
			temp.устСчёт(8);
#ifdef CPU_LE
			*(double *)~temp = d;
#else
			temp[0] = s[7];
			temp[1] = s[6];
			temp[2] = s[5];
			temp[3] = s[4];
			temp[4] = s[3];
			temp[5] = s[2];
			temp[6] = s[1];
			temp[7] = s[0];
#endif
			return temp;
		}

		case 'L': {
			int cond = значение;
			if(пусто_ли(cond)) return "?";
			return cond ? "T" : "F";
		}

		default:  {
			NEVER();
			return Null;
		}
	}
}

DbfStream::Field DbfStream::Field::устТекст(const char *имя, int width)
{
	ПРОВЕРЬ(width >= 1 && width <= 255);
	return Field(имя, 'C', width);
}

DbfStream::Field DbfStream::Field::Number(const char *имя, int width, int decimals)
{
	ПРОВЕРЬ(width >= 1 && width <= 31);
	ПРОВЕРЬ(decimals >= 0 && decimals <= width - 1);
	return Field(имя, 'N', width, decimals);
}

DbfStream::Field DbfStream::Field::Дата(const char *имя)
{
	return Field(имя, 'D', 8);
}

DbfStream::Field DbfStream::Field::Logical(const char *имя)
{
	return Field(имя, 'L', 1);
}

int DbfStream::Field::дайТип() const
{
	switch(тип) {
	case 'N':
	case 'F': return (width <= 9 && decimal == 0 ? INT_V : DOUBLE_V);
	case 'O': return DOUBLE_V;
	case 'M':
	case 'G':
	case 'C': return STRING_V;
	case 'D': return DATE_V;
	case 'L': return INT_V;
	default:  return VOID_V;
	}
}

void DbfStream::Field::сериализуй(Поток& stream)
{
	char буфер[12];
	ZeroArray(буфер);
	if(stream.сохраняется())
		memcpy(буфер, имя, min(имя.дайДлину(), 10));
	stream.SerializeRaw((byte *)буфер, 11);
	if(stream.грузится()) {
		имя = буфер;
		имя = взаг(имя);
	}
	stream % тип;
	sStreamIL(stream, offset);
	stream % width % decimal;
	if(stream.сохраняется())
		stream.помести(0, 14);
	else
		stream.SeekCur(14);
}

DbfStream::DbfStream()
{
	version = 0x03;
}

DbfStream::DbfStream(const char *_file, bool write, byte _charset, bool _delete_share)
{
	version = 0x03;
	открой(_file, write, _charset, _delete_share);
}

bool DbfStream::Check(const char *filename, bool write)
{
	return DbfStream().открой(filename, write, CHARSET_DEFAULT);
}

bool DbfStream::открой(const char *file, bool write, byte _charset, bool _delete_share)
{
	закрой();
	LLOG("DbfStream::открой(" << file << ", write = " << write << ", charset = " << (int)_charset << ")");
	if(!dbf.открой(file, (write ? ФайлПоток::READWRITE : ФайлПоток::READ)
	| (_delete_share ? DeleteShareFlag() : 0))) {
		LLOG("-> open Ошибка");
		return false;
	}
	if(dbt.открой(форсируйРасш(file, ".dbt"),
		ФайлПоток::READ | (_delete_share ? DeleteShareFlag() : 0))) // writing MEMO's is not supported yet
		dbt.SetLoading();
	if(fpt.открой(форсируйРасш(file, ".fpt"),
		ФайлПоток::READ | (_delete_share ? DeleteShareFlag() : 0)))
		fpt.SetLoading();
	dbf.SetLoading();
	if(DoOpen(_charset))
		return true;
	LLOG("-> DoOpen Ошибка");
	закрой();
	return false;
}

bool DbfStream::создай(const char *filename, const Массив<Field>& _fields, byte _charset, bool _delete_share)
{
	закрой();
	if(!dbf.открой(filename, ФайлПоток::CREATE | (_delete_share ? DeleteShareFlag() : 0)))
		return false;
	if(DoCreate(_fields, _charset))
		return true;
	закрой();
	return false;
}

static inline bool IsValidType(char t)
{
	return t == 'C' || t == 'N' || t == 'D' || t == 'L' || t == 'M'
	|| t == 'F' || t == 'O' || t == 'G';
}

bool DbfStream::StreamHeader(bool full)
{
	ПРОВЕРЬ(dbf.открыт());
	data_offset = 32 + fields.дайСчёт() * 32 + 1;
	if(dbf.сохраняется()) {
		ПРОВЕРЬ(!толькочтен_ли());
		dbf.помести(version); // #0: version number - 03 without a DBT file
		Дата date = дайСисДату();
		dbf.помести(date.year - 1900); // #1 - 3: date of last update
		dbf.помести(date.month);
		dbf.помести(date.day);
	}
	else {
		version = dbf.дай8();
		dbf.дай8();
		dbf.дай16();
	}
	sStreamIL(dbf, rows);
	if(!full)
		return true;
	sStreamIW(dbf, data_offset);
	int row_width = record.дайСчёт();
	if(dbf.сохраняется() && row_width == 0) { // calculate row width
		row_width = 1;
		for(int i = 0; i < fields.дайСчёт(); i++)
			row_width += fields[i].width;
	}
	sStreamIW(dbf, row_width);
	if(dbf.сохраняется()) {
		dbf.помести(0, 17);
		int i = __countof(_dbfCharsets);
		while(--i >= 0 && _dbfCharsets[i].charset != charset)
			;
		dbf.помести(i >= 0 ? _dbfCharsets[i].dbf_code : 0);
	//	dbf.PutIL(lang_code);
		dbf.помести16(0);
	}
	else
		dbf.SeekCur(20);
	int field_count = fields.дайСчёт();
	if(dbf.грузится()) {
		if(rows < 0)
			return false;
		if(data_offset <= 32) // || (data_offset & 0x1F) != 1)
			return false; // invalid header size
		if(dbf.дайРазм() < (unsigned)data_offset)
			return false; // incomplete file header
		int rec_count = (int)((dbf.дайРазм() - data_offset) / row_width);
		rows = min(rows, rec_count); // physical record #
		field_count = (data_offset >> 5) - 1;
		fields.очисть();
		has_memo = false;
	}
	int offset = 1;
	Индекс<Ткст> used_names;
	for(int i = 0; i < field_count && offset < row_width; i++) {
		Field field;
		if(dbf.сохраняется()) {
			fields[i].offset = offset;
			field = fields[i];
		}
		field.сериализуй(dbf);
		if(dbf.ошибка_ли())
			return false;
		if(dbf.грузится()) {
			if(field.тип == 0)
				break;
			Ткст uname = взаг(вАски(field.имя, charset), charset);
			Ткст outname;
			for(const char *p = uname; *p; p++)
				if(IsAlNum(*p) || *p == '_')
					outname.конкат(*p);
			if(outname.пустой() || цифра_ли(*outname))
				outname.вставь(0, 'F');
			if(used_names.найди(outname) >= 0) {
				int i = 0;
				while(used_names.найди(outname + '_' + целТкт(++i)) >= 0)
					;
				outname << '_' << i;
			}
			used_names.добавь(field.имя = outname);
		}
		if(dbf.грузится() && field.имя != "_DBASELOCK" && IsValidType(field.тип)) {
			field.offset = offset;
			fields.добавь(field.имя, field);
			if(field.тип == 'M')
				has_memo = true;
		}
		offset += field.width;
	}
	byte b = 0x0D;
	dbf % b;
	ПРОВЕРЬ(dbf.дайПоз() <= (unsigned)data_offset);
	record.устСчёт(row_width);
	memset(record.старт(), ' ', record.дайСчёт());
	if(dbf.ошибка_ли())
		return false;
	if(has_memo) {
		if(dbt.открыт() && dbt.грузится()) { // read DBT header
			dbt.перейди(16);
			int b = dbt.дай();
			dbt_block_size = 512;
			if(b != 3) {
				dbt.перейди(20);
				dbt_block_size = (short)dbt.Get16le();
				if(dbt_block_size <= 0)
					dbt.закрой(); // has_memo = false;
			}
		}
		if(fpt.открыт() && fpt.грузится()) { // read FPT header
			// int nextfree = fpt.Get32be();
			fpt_block_size = fpt.Get32be();
			if(fpt_block_size <= 0 || fpt_block_size >= 1 << 24)
				fpt.закрой(); // has_memo = false;
		}
	}
	return true;
}

bool DbfStream::DoOpen(byte _charset)
{
	charset = _charset;
	dirty = false;
	dirty_header = false;
	row_index = Null;
	next_row_index = 0;
	if(!StreamHeader(true))
		return false;
	DoFields();
	return true;
}

bool DbfStream::DoCreate(const Массив<Field>& _fields, byte _charset)
{
	charset = _charset;
	rows = 0;
	dirty = false;
	dirty_header = false;
	row_index = 0;
	next_row_index = 0;
	has_memo = false;
	fields.очисть();
	for(int i = 0; i < _fields.дайСчёт(); i++)
		fields.добавь(_fields[i].имя, _fields[i]);
	if(!StreamHeader(true))
		return false;
	DoFields();
	return true;
}

void DbfStream::DoFields()
{
	field_read.очисть();
	field_read.устСчёт(fields.дайСчёт(), &DbfStream::GetItemEmpty);
	for(int i = 0; i < fields.дайСчёт(); i++) {
		const Field& fld = fields[i];
		switch(fld.тип) {
		case 'C': field_read[i] = &DbfStream::GetItemString; break;
		case 'N':
		case 'F': field_read[i] = (fld.width < 10 && fld.decimal == 0 ? &DbfStream::GetItemInt : &DbfStream::GetItemDouble); break;
		case 'O': field_read[i] = &DbfStream::GetItemDouble8; break;
		case '!':
		case '+': field_read[i] = &DbfStream::GetItemInt4; break;
		case '@': field_read[i] = &DbfStream::GetItemDateTime8; break;
		case 'D': field_read[i] = (fld.width < 10 ? &DbfStream::GetItemDateShort : &DbfStream::GetItemDateLong); break;
		case 'L': field_read[i] = &DbfStream::GetItemLogical; break;
		case 'M':
		case 'G': field_read[i] = &DbfStream::GetItemMemoString; break;
		case 'B': field_read[i] = &DbfStream::GetItemMemoBinary; break;
		}
	}
}

void DbfStream::SetRowCount(int count)
{
	if(rows > count) { // trim file
		dbf.устРазм(data_offset + count * record.дайСчёт());
		dirty_header = true;
	}
	else if(count > rows) { // extend file
		dbf.перейди(data_offset + rows * record.дайСчёт());
		dbf.помести(' ', (count - rows) * record.дайСчёт());
		dirty_header = true;
	}
	rows = count;
}

void DbfStream::слей()
{
	FlushRow();
	FlushHeader();
}

void DbfStream::FlushRow()
{
	ПРОВЕРЬ(открыт());
	if(dirty) {
		ПРОВЕРЬ(!толькочтен_ли());
		ПРОВЕРЬ(row_index >= 0);
		if(row_index > rows) {
			dbf.перейди(data_offset + rows * record.дайСчёт());
			dbf.помести(' ', (row_index - rows) * record.дайСчёт());
		}
		else
			dbf.перейди(data_offset + row_index * record.дайСчёт());
		dbf.помести(record.старт(), record.дайСчёт());
		if(row_index >= rows) {
			rows = row_index + 1;
			dirty_header = true;
		}
		dirty = false;
	}
}

void DbfStream::FlushHeader()
{
	ПРОВЕРЬ(открыт());
	if(dirty_header) {
		ПРОВЕРЬ(!толькочтен_ли());
		dbf.перейди(0);
		dbf.SetStoring();
		StreamHeader(false);
		dirty_header = false;
	}
}

bool DbfStream::закрой()
{
	if(открыт())
		слей();
	dbf.закрой();
	dbt.закрой();
	return !dbf.ошибка_ли() && (!has_memo || !dbt.ошибка_ли());
}

bool DbfStream::Fetch(int row)
{
	LTIMING("DbfStream::Fetch");
	ПРОВЕРЬ(открыт());
	if(dirty)
		FlushRow();
	next_row_index = row + 1;
	if(row == row_index)
		return *record;
	row_index = row;
	if(row < 0 || row >= rows) {
		*record.старт() = 0;
		return false;
	}

	dbf.перейди(data_offset + row * record.дайСчёт());
	byte *p = record.старт(), *e = record.стоп();
	int count = dbf.дай(p, (int)(e - p));
	if(count < record.дайСчёт())
		memset(p + count, ' ', record.дайСчёт() - count);
	if(count <= 0 || (*p != ' ' && *p != 0)) {
		*p = 0;
		return false;
	}
	*p = ' ';
/*
	if(codepage_cv)
		for(const char *page = codepage_map; ++p < e; *p = page[*p])
			;
*/
	dirty = false;
	return true;
}

void DbfStream::перейди(int row)
{
	if(row != row_index) {
		FlushRow();
		next_row_index = row;
	}
}

bool DbfStream::Fetch()
{
	int r = next_row_index;
	for(; r < rows; r++)
		if(Fetch(r))
			return true;
	return false;
}

Значение DbfStream::дайЭлт(int i) const
{
	LTIMING("DbfStream::дайЭлт");
	ПРОВЕРЬ(i >= 0 && i < fields.дайСчёт());
	return (this ->* (field_read[i].proc))(i);
}

void DbfStream::PutItem(int i, Значение значение)
{
	const Field& fld = fields[i];
	dirty = true;
	byte *p = record.старт() + fld.offset;
	if(пусто_ли(значение)) // empty значение is the same for all data types
		memset(p, ' ', fld.width);
	else {
		Ткст s = fld.фмт(значение, charset);
		int l = min<int>(s.дайДлину(), fld.width);
		memcpy(p, s, l);
		if(l < fld.width)
			memset(p + l, ' ', fld.width - l);
	}
}

void DbfStream::PutItem(Ткст n, Значение значение)
{
	int i = GetFieldIndex(n);
	if(i >= 0)
		PutItem(i, значение);
	else
		NEVER(); // column missing
}

Значение DbfStream::дайЭлт(Ткст имя) const
{
	int Индекс = GetFieldIndex(имя);
	if(Индекс >= 0)
		return дайЭлт(Индекс);
	else {
		NEVER();
		return значОш(фмтЧ("column '%s' not found", имя));
	}
}

Вектор<Значение> DbfStream::FetchRow()
{
	if(!Fetch())
		return Вектор<Значение>();
	return FetchRow(row_index);
}

Вектор<Значение> DbfStream::FetchRow(int row)
{
	LTIMING("DbfStream::FetchRow");
	Вектор<Значение> values;
	if(!Fetch(row))
		return values;
	values.устСчёт(fields.дайСчёт());
	for(int i = 0; i < fields.дайСчёт(); i++)
		values[i] = (this ->* (field_read[i].proc))(i);
	return values;
}

void DbfStream::WriteRow(int row, const Вектор<Значение>& values)
{
	ПРОВЕРЬ(row >= 0);
	FlushRow();
	if(values.дайСчёт() < fields.дайСчёт())
		Fetch(row);
	else {
		row_index = row;
		next_row_index = row_index + 1;
	}
	for(int i = 0, n = min(values.дайСчёт(), fields.дайСчёт()); i < n; i++)
		PutItem(i, values[i]);
}

void DbfStream::WriteRow(const Вектор<Значение>& values)
{
	WriteRow(next_row_index, values);
	перейди(row_index + 1);
}

void DbfStream::WriteRow()
{
	row_index = next_row_index;
	FlushRow();
	next_row_index = ++row_index;
}

Ткст DbfStream::вТкст() const
{
	Ткст result;
	result
		<< fields.дайСчёт() << " columns\n"
		<< rows << " rows\n\nColumns:\n";

	for(int i = 0; i < GetFieldCount(); i++) {
		const Field& field = GetField(i);
		result = фмт("%2i: %-12s '%c' %d", i, ~field.имя, field.тип, field.width);
		if(field.decimal)
			result << '.' << field.decimal;
		result << '\n';
	}
	return result << '\n';
}

void DbfStream::DumpData(Поток& stream)
{
	stream << *this; // dump layout

	// dump column headers
	stream << "ROWID ";
	int total = 6;
	Вектор<int> fw;

	int i;
	for(i = 0; i < fields.дайСчёт(); i++) {
		const Field& field = GetField(i);
		int width = max<int>(field.width, (int)strlen(field.имя));
		switch(field.тип) {
		case 'D': width = max(width, 10); break;
		}
		++width;
		stream << фмтЧ("%*<s", width, field.имя);
		fw.добавь() = width;
		total += width;
	}
	stream << "\n-";
	stream.помести('-', total - 2);
	stream << '\n';

	for(i = 0; i < rows; i++) {
		stream << фмтЧ("%5d ", i);
 		if(Fetch(i)) { // дисплей individual columns
			for(int j = 0; j < fields.дайСчёт(); j++)
				stream << фмтЧ("%*<vt", fw[j], дайЭлт(j));
		}
		stream << '\n';
	}
	stream << "\n\n";
}

Значение DbfStream::GetItemDouble8(int i) const
{
	const byte *p = record.старт() + fields[i].offset;
#ifdef CPU_BE
	union {
		char temp[8];
		double dbl;
	};
	temp[0] = p[7];
	temp[1] = p[6];
	temp[2] = p[5];
	temp[3] = p[4];
	temp[4] = p[3];
	temp[5] = p[2];
	temp[6] = p[1];
	temp[7] = p[0];
	return dbl;
#else
	return *(const double *)p;
#endif
}

Значение DbfStream::GetItemInt4(int i) const
{
	const byte *p = record.старт() + fields[i].offset;
	int v = Peek32le(p);
	v ^= 0x80000000;
	return v;
}

Значение DbfStream::GetItemDateTime8(int i) const
{
	const byte *p = record.старт() + fields[i].offset;
	int dt = Peek32le(p + 0);
	int tm = Peek32le(p + 4);
	Дата d = Дата(-4713, 1, 1) + dt;
	Время t = воВремя(d);
	tm /= 1000;
	t.second = tm % 60;
	tm /= 60;
	t.minute = tm % 60;
	tm /= 60;
	t.hour = tm;
	return t;
}

Значение DbfStream::GetItemInt(int i) const
{
	const byte *p = record.старт() + fields[i].offset, *e = p + fields[i].width;
	while(p < e && *p == ' ')
		p++;
	return scan_int((const char *)p, (const char *)e);
}

Значение DbfStream::GetItemDouble(int i) const
{
	const byte *p = record.старт() + fields[i].offset, *e = p + fields[i].width;
	while(p < e && *p == ' ')
		p++;
	return scan_dbl((const char *)p, (const char *)e);
}

Значение DbfStream::GetItemString(int i) const
{
	const byte *p = record.старт() + fields[i].offset, *e = p + fields[i].width;
	while(p < e && *p == ' ')
		p++;
	while(e > p && e[-1] == ' ')
		e--;
	Ткст s(p, (int)(e - p));
	if(charset == дайДефНабСим())
		return s;
	return вЮникод(s, charset);
}

Значение DbfStream::GetItemDateShort(int i) const
{
	const byte *p = record.старт() + fields[i].offset;
	Дата date;
	if(цифра_ли(*p)) {
		if(p[2] == '.') {
			date.day = 10 * p[0] + p[1] - 11 * '0';
			date.month = 10 * p[3] + p[4] - 11 * '0';
			date.year = 10 * p[6] + p[7] + 1900 - 11 * '0';
		}
		else {
			date.year = 1000 * p[0] + 100 * p[1] + 10 * p[2] + p[3] - 1111 * '0';
			date.month = 10 * p[4] + p[5] - 11 * '0';
			date.day = 10 * p[6] + p[7] - 11 * '0';
		}
	}
	return date;
}

Значение DbfStream::GetItemDateLong(int i) const
{
	const byte *p = record.старт() + fields[i].offset;
	if(цифра_ли(*p) && !цифра_ли(p[2])) {
		Дата date;
		date.day = 10 * p[0] + p[1] - 11 * '0';
		date.month = 10 * p[3] + p[4] - 11 * '0';
		date.year = 1000 * p[6] + 100 * p[7] + 10 * p[8] + p[9] - 1111 * '0';
		return date;
	}
	else
		return GetItemDateShort(i);
}

Значение DbfStream::GetItemLogical(int i) const
{
	return StrBool(record[fields[i].offset]);
}

Значение DbfStream::GetItemMemoString(int i) const
{
	return GetItemMemo(i, false);
}

Значение DbfStream::GetItemMemoBinary(int i) const
{
	return GetItemMemo(i, true);
}

Значение DbfStream::GetItemMemo(int i, bool binary) const
{
	if(!has_memo)
		return false;
	const byte *p = record.старт() + fields[i].offset, *e = p + fields[i].width;
	while(p < e && *p == ' ')
		p++;
	int block = scan_int((const char *)p, (const char *)e);
	if(!пусто_ли(block) && block > 0) {
		Ткст out;
		if(dbt.открыт()) {
			unsigned pos = block * dbt_block_size;
			if(pos >= dbt.дайРазм())
				return Значение();
			dbt.перейди(pos);
			if(dbt.GetIL() == 0x8FFFF) { // dBASE IV memo with explicit length
				unsigned len = dbt.GetIL();
				if(len <= 0 || len > dbt.GetLeft())
					return Значение();
				Буфер<byte> буфер(len);
				if(!dbt.дайВсе(буфер, len))
					return Значение();
				if(!binary) {
					byte *p;
					if((p = (byte *)memchr(буфер, '\0', len))) len = (unsigned)(p - буфер);
					if((p = (byte *)memchr(буфер, '\x1A', len))) len = (unsigned)(p - буфер);
				}
				out = Ткст(буфер, len);
			}
			else { // dBASE III memo with 1A1A separator
				Буфер<byte> буфер(dbt_block_size + 1);
				bool eof = false;
				for(int b; !eof && (b = dbt.дай(буфер, dbt_block_size + 1)) > 0;) {
					byte *p = буфер, *e = p + b;
					while((p = (byte *)memchr(p, '\x1A', e - p)) != 0)
						if(++p < e && *p++ == '\x1A') { // double EOF found
							e = p - 2;
							eof = true;
							break;
						}
					int l = (int)(e - буфер);
					if(!eof && l > dbt_block_size) {
						dbt.SeekCur(dbt_block_size - l);
						e = буфер + l;
					}
					if(l > 0)
						out.конкат(буфер, l);
				}
			}
		}
		else if(fpt.открыт()) {
			unsigned pos = block * fpt_block_size;
			if(pos >= fpt.дайРазм())
				return Значение();
			fpt.перейди(pos);
			// int fieldtype = fpt.Get32be();
			int len = fpt.Get32be();
			if(len > 0) {
				ТкстБуф outbuf(len);
				if(!fpt.дайВсе(outbuf, len))
					return Значение();
				out = outbuf;
			}
		}
		if(binary || charset == дайДефНабСим())
			return out;
		return вЮникод(out, charset);
	}
	return Значение();
}

Значение DbfStream::GetItemEmpty(int i) const
{
	return Значение();
}

}
