#ifndef _zip_zip_h
#define _zip_zip_h

#include <Core/Core.h>

namespace РНЦП {

class UnZip {
	struct Файл : Движимое<Файл> {
		word   bit;
		Ткст path;
		dword  time;
		int    method;
		dword  crc;
		dword  csize;
		dword  usize;
		int64  offset;
	};
	
	Поток      *zip;
	bool         Ошибка;
	Вектор<Файл> file;
	int          current;

	void   ReadDir();

	static Время   GetZipTime(dword time);

public:
	bool   кф_ли() const          { return current >= file.дайСчёт(); }
	operator bool() const         { return !кф_ли() && !ошибка_ли(); }
	
	bool   ошибка_ли() const        { return Ошибка; }
	void   сотриОш()           { Ошибка = false; }

	int    дайСчёт() const       { return file.дайСчёт(); }
	Ткст дайПуть(int i) const   { return file[i].path; }
	bool   папка_ли(int i) const  { return *file[i].path.последний() == '/'; }
	bool   файл_ли(int i) const    { return !папка_ли(i); }
	int    дайДлину(int i) const { return file[i].usize; }
	Время   дайВремя(int i) const   { return GetZipTime(file[i].time); }

	void   перейди(int i)            { ПРОВЕРЬ(i >= 0 && i < file.дайСчёт()); current = i; }

	bool   папка_ли() const       { return папка_ли(current); }
	bool   файл_ли() const         { return !папка_ли(); }
	Ткст дайПуть() const        { return дайПуть(current); }
	int    дайДлину() const      { return дайДлину(current); }
	Время   дайВремя() const        { return дайВремя(current); }

	void   пропусти()                 { current++; }
	void   SkipFile()             { current++; }
	bool   ReadFile(Поток& out, Врата<int, int> progress = Null);
	Ткст ReadFile(Врата<int, int> progress = Null);

	Ткст ReadFile(const char *path, Врата<int, int> progress = Null);

	void   создай(Поток& in);
	void   закрой()                { file.очисть(); zip->закрой(); }

	UnZip(Поток& in);
	UnZip();
	virtual ~UnZip();
};

class FileUnZip : public UnZip {
	ФайлВвод zip;

public:
	bool создай(const char *имя);
	
	FileUnZip(const char *имя)                 { создай(имя); }
	FileUnZip()                                 {}
};

class MemUnZip : public UnZip {
	ПамЧтенПоток zip;

public:
	void создай(const void *ptr, int count);

	MemUnZip(const void *ptr, int count)       { создай(ptr, count); }
	MemUnZip();
};

class StringUnZip : public UnZip {
	ТкстПоток zip;

public:
	void создай(const Ткст& s);

	StringUnZip(const Ткст& s)                { создай(s); }
	StringUnZip();
};

class Zip {
	Поток *zip;

	struct Файл {
		Ткст path;
		dword  time;
		int    version;
		int    gpflag;
		int    method;
		dword  crc;
		dword  csize;
		dword  usize;
	};
	Массив<Файл> file;

	dword   done;

	Один<Zlib> pipeZLib;
	Crc32Stream crc32; // for uncompressed files
	bool        uncompressed;

	void WriteFile0(const void *ptr, int size, const char *path, Врата<int, int> progress, Время tm, int method);

	void FileHeader(const char *path, Время tm);

	void PutCompressed(const void *data, int size);
	
	typedef Zip ИМЯ_КЛАССА;

public:
	Callback WhenError;

	void BeginFile(const char *path, Время tm = дайСисВремя(), bool deflate = true);
	void BeginFile(ВыхФильтрПоток& oz, const char *path, Время tm = дайСисВремя(), bool deflate = true);
	void помести(const void *data, int size);
	void EndFile();
	bool IsFileOpened() const                 { return pipeZLib || uncompressed; }

	void WriteFolder(const char *path, Время tm = дайСисВремя());
	void WriteFile(const void *ptr, int size, const char *path, Врата<int, int> progress = Null, Время tm = дайСисВремя(), bool deflate = true);
	void WriteFile(const Ткст& s, const char *path, Врата<int, int> progress = Null, Время tm = дайСисВремя(), bool deflate = true);

	void создай(Поток& out);
	void финиш();
	
	bool ошибка_ли()                           { return zip && zip->ошибка_ли(); }

	dword  дайДлину() const                 { return done; }
	
	Zip();
	Zip(Поток& out);
	virtual ~Zip();
};

class FileZip : public Zip {
	ФайлВывод zip;

public:
	bool создай(const char *имя);
	bool финиш();
	
	FileZip(const char *имя)                 { создай(имя); }
	FileZip()                                 {}
	~FileZip()                                { финиш(); }
};

class StringZip : public Zip {
	ТкстПоток zip;

public:
	void   создай();
	Ткст финиш();

	StringZip()                               { создай(); }
};

}

#endif
