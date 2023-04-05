class Crc32Stream : public ПотокВывода {
	dword crc;

	virtual  void  выведи(const void *данные, dword size);

public:
	dword  финиш()            { слей(); return crc; }
	operator dword()           { return финиш(); }
	void   очисть();
	
	Crc32Stream()              { очисть(); }
};

dword CRC32(const void *ptr, dword count);
dword CRC32(const Ткст& s);

class Zlib {
	enum { NONE, DEFLATE, INFLATE };

	z_stream      z;
	Буфер<Bytef> output;
	int           chunk;
	int           mode;
	dword         crc;
	int           total;
	int           compression_level;
	bool          docrc;
	bool          hdr;
	bool          Ошибка;
	bool          gzip;
	bool          gzip_header_done;
	bool          gzip_footer;
	Ткст        footer;
	Ткст        gzip_hs;
	Ткст        gzip_name;
	Ткст        gzip_comment;
	Ткст        out;
	
	void          PutOut(const void *ptr, int size);
	void          Pump(bool finish);
	void          старт();
	void          освободи();
	void          Put0(const char *ptr, int size);
	int           GzipHeader(const char *ptr, int size);
	void          иниц();

public:
	Событие<const void *, int>  WhenOut;
	
	void Compress();
	void Decompress();
	void помести(const void *ptr, int size);
	void помести(const Ткст& s)              { помести(~s, s.дайСчёт()); }
	void стоп();
	void очисть();
	
	const Ткст& дай() const              { return out; }
	operator const Ткст&() const         { return out; }
	const Ткст& operator~() const        { return out; }
	void   ClearOut()                      { out.очисть(); }

	dword  GetCRC() const                  { return crc; }
	bool   ошибка_ли() const                 { return Ошибка; }
	Ткст GetGZipName() const             { return gzip_name; }
	Ткст GetGZipComment() const          { return gzip_comment; }
	
	Zlib& GZip(bool gzip_ = true)          { gzip = gzip_; return *this; }
	Zlib& Header(bool hdr_ = true)         { hdr = hdr_; return *this; }
	Zlib& NoHeader()                       { return Header(false); }
	Zlib& CRC(bool b = true)               { docrc = b; return *this; }
	Zlib& NoCRC()                          { return CRC(false); }
	Zlib& ChunkSize(int n);
	Zlib& Level(int compression_lvl)       { compression_level = compression_lvl; return *this; }

	Zlib();
	~Zlib();
};

class ZCompressStream : public ВыхФильтрПоток {
	Zlib         z;

public:
	void открой(Поток& out)                 { z.Compress(); уст(out, z); }

	dword  GetCRC() const                  { return z.GetCRC(); }
	
	Zlib& GZip(bool gzip_ = true)          { return z.GZip(); }
	Zlib& Header(bool hdr_ = true)         { return z.Header(); }
	Zlib& NoHeader()                       { return Header(false); }
	Zlib& CRC(bool b = true)               { return z.CRC(); }
	Zlib& NoCRC()                          { return CRC(false); }
	Zlib& ChunkSize(int n)                 { return z.ChunkSize(n); }
	Zlib& Level(int compression_lvl)       { return z.Level(compression_lvl); }

	ZCompressStream()                      {}
	ZCompressStream(Поток& out)           { открой(out); }
	~ZCompressStream()                     { закрой(); }
};

class ZDecompressStream : public ВхоФильтрПоток {
	Zlib         z;

public:
	void открой(Поток& in)                  { z.Decompress(); уст(in, z); }

	dword  GetCRC() const                  { return z.GetCRC(); }
	Ткст GetGZipName() const             { return z.GetGZipName(); }
	Ткст GetGZipComment() const          { return z.GetGZipComment(); }
	
	Zlib& GZip(bool gzip_ = true)          { return z.GZip(); }
	Zlib& Header(bool hdr_ = true)         { return z.Header(); }
	Zlib& NoHeader()                       { return Header(false); }
	Zlib& CRC(bool b = true)               { return z.CRC(); }
	Zlib& NoCRC()                          { return CRC(false); }
	Zlib& ChunkSize(int n)                 { return z.ChunkSize(n); }
	Zlib& Level(int compression_lvl)       { return z.Level(compression_lvl); }

	ZDecompressStream()                    {}
	ZDecompressStream(Поток& out)         { открой(out); }
	~ZDecompressStream()                   { закрой(); }
};

int64  копируйПоток(Поток& dest, Поток& ист, int64 count, Врата<int64, int64> progress, int chunk_size = 65536);

int64  ZCompress(Поток& out, Поток& in, int64 size, Врата<int64, int64> progress = Null, bool hdr = true);
int64  ZCompress(Поток& out, Поток& in, Врата<int64, int64> progress = Null);
Ткст ZCompress(const void *данные, int64 len, Врата<int64, int64> progress = Null);
Ткст ZCompress(const Ткст& s, Врата<int64, int64> progress = Null);

int64  ZDecompress(Поток& out, Поток& in, int64 size, Врата<int64, int64> progress = Null, bool hdr = true);
int64  ZDecompress(Поток& out, Поток& in, Врата<int64, int64> progress = Null);
Ткст ZDecompress(const void *данные, int64 len, Врата<int64, int64> progress = Null);
Ткст ZDecompress(const Ткст& s, Врата<int64, int64> progress = Null);

int64  GZCompress(Поток& out, Поток& in, int64 size, Врата<int64, int64> progress = Null);
int64  GZCompress(Поток& out, Поток& in, Врата<int64, int64> progress = Null);
Ткст GZCompress(const void *данные, int len, Врата<int64, int64> progress = Null);
Ткст GZCompress(const Ткст& s, Врата<int64, int64> progress = Null);

int64  GZDecompress(Поток& out, Поток& in, int64 size, Врата<int64, int64> progress = Null);
int64  GZDecompress(Поток& out, Поток& in, Врата<int64, int64> progress = Null);
Ткст GZDecompress(const void *данные, int len, Врата<int64, int64> progress = Null);
Ткст GZDecompress(const Ткст& s, Врата<int64, int64> progress = Null);

bool   GZCompressFile(const char *dstfile, const char *srcfile, Врата<int64, int64> progress = Null);
bool   GZCompressFile(const char *srcfile, Врата<int64, int64> progress = Null);

bool   GZDecompressFile(const char *dstfile, const char *srcfile, Врата<int64, int64> progress = Null);
bool   GZDecompressFile(const char *srcfile, Врата<int64, int64> progress = Null);

inline
bool   GZCompressFile(const Ткст& dstfile, const Ткст& srcfile, Врата<int64, int64> progress = Null)
{ return GZCompressFile(~dstfile, ~srcfile, progress); }
inline
bool   GZCompressFile(const Ткст& srcfile, Врата<int64, int64> progress = Null)
{ return GZCompressFile(~srcfile, progress); }

inline
bool   GZDecompressFile(const Ткст& dstfile, const Ткст& srcfile, Врата<int64, int64> progress = Null)
{ return GZDecompressFile(~dstfile, ~srcfile, progress); }
inline
bool   GZDecompressFile(const Ткст& srcfile, Врата<int64, int64> progress = Null)
{ return GZDecompressFile(~srcfile, progress); }

/// Backward compatibility:

Врата<int64, int64> AsGate64(Врата<int, int> gate);

inline int ZCompress(Поток& out, Поток& in, Лаз2<int, int> progress)        { return (int)ZCompress(out, in, AsGate64(progress)); }
inline Ткст ZCompress(const void *данные, int len, Лаз2<int, int> progress)   { return ZCompress(данные, len, AsGate64(progress)); }
inline Ткст ZCompress(const Ткст& s, Лаз2<int, int> progress)             { return ZCompress(s, AsGate64(progress)); }

inline int    ZDecompress(Поток& out, Поток& in, Лаз2<int, int> progress)   { return (int)ZDecompress(out, in, AsGate64(progress)); }
inline Ткст ZDecompress(const Ткст& s, Лаз2<int, int> progress)           { return ZDecompress(s, AsGate64(progress)); }
inline Ткст ZDecompress(const void *данные, int len, Лаз2<int, int> progress) { return ZDecompress(данные, len, AsGate64(progress)); }

inline int    GZCompress(Поток& out, Поток& in, int size, Лаз2<int, int> progress) { return (int)GZCompress(out, in, size, AsGate64(progress)); }
inline int    GZCompress(Поток& out, Поток& in, Лаз2<int, int> progress)           { return (int)GZCompress(out, in, AsGate64(progress)); }
inline Ткст GZCompress(const void *данные, int len, Лаз2<int, int> progress)         { return GZCompress(данные, len, AsGate64(progress)); }
inline Ткст GZCompress(const Ткст& s, Лаз2<int, int> progress)                   { return GZCompress(s, AsGate64(progress)); }

inline int    GZDecompress(Поток& out, Поток& in, int size, Лаз2<int, int> progress) { return (int)GZDecompress(out, in, size, AsGate64(progress)); }
inline int    GZDecompress(Поток& out, Поток& in, Лаз2<int, int> progress)           { return (int)GZDecompress(out, in, AsGate64(progress)); }
inline Ткст GZDecompress(const void *данные, int len, Лаз2<int, int> progress)         { return GZDecompress(данные, len, AsGate64(progress)); }
inline Ткст GZDecompress(const Ткст& s, Лаз2<int, int> progress)                   { return GZDecompress(s, AsGate64(progress)); }

Ткст сожмиБыстро(const void *s, int sz);
Ткст сожмиБыстро(const Ткст& s);
Ткст разожмиБыстро(const Ткст& данные);
