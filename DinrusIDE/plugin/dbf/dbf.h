#ifndef __dbf_dbf__
#define __dbf_dbf__

#include <Core/Core.h>

namespace РНЦП {

#define DBFCOL(x) (#x, x)

int StrBool(const char* s);

class DbfStream
{
public:
	class Field : Движимое<Field>
	{
	public:
		Field(const char *имя = 0, char тип = 0, byte width = 0, byte decimal = 0);

		Ткст       фмт(Значение значение, byte charset) const;

		static Field устТекст(const char *имя, int width);
		static Field Number(const char *имя, int width = 19, int decimals = 10);
		static Field Integer(const char *имя, int width = 10) { return Number(имя, width, 0); }
		static Field Дата(const char *имя);
		static Field Logical(const char *имя);

		int          дайТип() const;
		void         сериализуй(Поток& stream);

		int          offset;
		Ткст       имя;
		char         тип;
		byte         width;
		byte         decimal;
	};

	DbfStream();
	DbfStream(const char* filename, bool write = false, byte charset = CHARSET_CP852, bool delete_share = false);
	~DbfStream() { закрой(); }

	bool            открой(const char *filename, bool write = false, byte charset = CHARSET_CP852, bool delete_share = false);
	bool            создай(const char *filename, const Массив<Field>& fields, byte charset = CHARSET_CP852, bool delete_share = false);
	bool            закрой();

	static bool     Check(const char *filename, bool write = false);

	bool            открыт() const                           { return dbf.открыт(); }
	Поток&         GetDBF()                                 { return dbf; }
	Поток&         GetDBT()                                 { return dbt; }
	bool            толькочтен_ли() const                       { return !(dbf.дайСтиль() & STRM_WRITE); }
	bool            ошибка_ли() const                          { return dbf.ошибка_ли(); }
	Время            дайФВремя() const                      { return dbf.дайВремя(); }
	int64           дайРазмФайла() const                      { return dbf.дайРазм(); }

	int             GetFieldCount() const                    { return fields.дайСчёт(); }
	const Field&    GetField(int Индекс) const                { return fields[Индекс]; }
	int             GetFieldIndex(Ткст field) const        { return fields.найди(field); }

	const МассивМап<Ткст, Field>& GetFields() const         { return fields; }

	void            SetRowCount(int rows);
	int             GetRowCount() const                      { return rows; }

	byte            дайНабсим() const                       { return charset; }

	void            перейди(int row);
	int             дайПоз() const                           { return row_index; }
	int             GetNextPos() const                       { return next_row_index; }

	bool            Fetch(int row); // true = valid, false = deleted
	bool            Fetch();        // true = end of file
	bool            пригоден() const                          { return record[0] == ' '; }
	bool            IsDeleted() const                        { return !пригоден(); }

	void            слей();        // flush current record and record count

	Значение           дайЭлт(int i) const;
	Значение           дайЭлт(Ткст column) const;

	void            PutItem(int i, Значение значение);
	void            PutItem(Ткст column, Значение значение);

	Вектор<Значение>   FetchRow(int row);
	Вектор<Значение>   FetchRow();
	void            WriteRow(int row, const Вектор<Значение>& values);
	void            WriteRow(const Вектор<Значение>& values);
	void            WriteRow();

	Значение           operator [] (int i) const                { return дайЭлт(i); }
	Значение           operator [] (Ткст column) const        { return дайЭлт(column); }

	Ткст          вТкст() const;
	void            DumpData(Поток& stream);

protected:
	Значение           GetItemInt(int i) const;
	Значение           GetItemDouble(int i) const;
	Значение           GetItemString(int i) const;
	Значение           GetItemDateShort(int i) const;
	Значение           GetItemDateLong(int i) const;
	Значение           GetItemLogical(int i) const;
	Значение           GetItemMemoString(int i) const;
	Значение           GetItemMemoBinary(int i) const;
	Значение           GetItemMemo(int i, bool binary) const;
	Значение           GetItemEmpty(int i) const;
	Значение           GetItemDouble8(int i) const;
	Значение           GetItemInt4(int i) const;
	Значение           GetItemDateTime8(int i) const;

	void            устНабсим(byte charset);
	void            FlushRecord();
	bool            DoOpen(byte charset);
	bool            DoCreate(const Массив<Field>& _fields, byte charset);
	bool            StreamHeader(bool full);
	void            FlushRow();
	void            FlushHeader();
	void            DoFields();

	ФайлПоток      dbf;
	mutable ФайлПоток dbt;
	mutable ФайлПоток fpt;
	МассивМап<Ткст, Field> fields;
	struct FieldProc : Движимое<FieldProc>
	{
		FieldProc(Значение (DbfStream::*proc)(int) const = NULL) : proc(proc) {}
		Значение (DbfStream::*proc)(int) const;
	};

	Вектор<FieldProc> field_read;
	Вектор<byte>    record; // current record

	int             rows;
	int             data_offset;
	int             row_index; // current row
	int             next_row_index; // row to fetch next
	int             dbt_block_size; // memo block size
	int             fpt_block_size;
	byte            charset;
//	word            codepage_uni[256];
//	char            codepage_map[256];
//	char            codepage_rev[256];
//	bool            codepage_cv;
	bool            dirty;
	bool            dirty_header;
	bool            has_memo;
	byte            version;
};

}

#endif
