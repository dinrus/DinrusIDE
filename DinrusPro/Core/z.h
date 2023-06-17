class Crc32Stream : public ПотокВывода {
	бцел crc;

	virtual  проц  выведи(кук данные, бцел size);

public:
	бцел  финиш()            { слей(); return crc; }
	operator бцел()           { return финиш(); }
	проц   очисть();
	
	Crc32Stream()              { очисть(); }
};

бцел CRC32(кук укз, бцел count);
бцел CRC32(const Ткст& s);

class Zlib {
	enum { NONE, DEFLATE, INFLATE };

	z_stream      z;
	Буфер<Bytef> output;
	цел           chunk;
	цел           mode;
	бцел         crc;
	цел           total;
	цел           compression_level;
	бул          docrc;
	бул          hdr;
	бул          Ошибка;
	бул          gzip;
	бул          gzip_header_done;
	бул          gzip_footer;
	Ткст        footer;
	Ткст        gzip_hs;
	Ткст        gzip_name;
	Ткст        gzip_comment;
	Ткст        out;
	
	проц          PutOut(кук укз, цел size);
	проц          Pump(бул finish);
	проц          старт();
	проц          освободи();
	проц          Put0(кткст0 укз, цел size);
	цел           GzipHeader(кткст0 укз, цел size);
	проц          иниц();

public:
	Событие<const проц *, цел>  WhenOut;
	
	проц Compress();
	проц Decompress();
	проц помести(кук укз, цел size);
	проц помести(const Ткст& s)              { помести(~s, s.дайСчёт()); }
	проц стоп();
	проц очисть();
	
	const Ткст& дай() const              { return out; }
	operator const Ткст&() const         { return out; }
	const Ткст& operator~() const        { return out; }
	проц   ClearOut()                      { out.очисть(); }

	бцел  GetCRC() const                  { return crc; }
	бул   ошибка_ли() const                 { return Ошибка; }
	Ткст GetGZipName() const             { return gzip_name; }
	Ткст GetGZipComment() const          { return gzip_comment; }
	
	Zlib& GZip(бул gzip_ = true)          { gzip = gzip_; return *this; }
	Zlib& Header(бул hdr_ = true)         { hdr = hdr_; return *this; }
	Zlib& NoHeader()                       { return Header(false); }
	Zlib& CRC(бул b = true)               { docrc = b; return *this; }
	Zlib& NoCRC()                          { return CRC(false); }
	Zlib& ChunkSize(цел n);
	Zlib& Level(цел compression_lvl)       { compression_level = compression_lvl; return *this; }

	Zlib();
	~Zlib();
};

class ZCompressStream : public ВыхФильтрПоток {
	Zlib         z;

public:
	проц открой(Поток& out)                 { z.Compress(); уст(out, z); }

	бцел  GetCRC() const                  { return z.GetCRC(); }
	
	Zlib& GZip(бул gzip_ = true)          { return z.GZip(); }
	Zlib& Header(бул hdr_ = true)         { return z.Header(); }
	Zlib& NoHeader()                       { return Header(false); }
	Zlib& CRC(бул b = true)               { return z.CRC(); }
	Zlib& NoCRC()                          { return CRC(false); }
	Zlib& ChunkSize(цел n)                 { return z.ChunkSize(n); }
	Zlib& Level(цел compression_lvl)       { return z.Level(compression_lvl); }

	ZCompressStream()                      {}
	ZCompressStream(Поток& out)           { открой(out); }
	~ZCompressStream()                     { закрой(); }
};

class ZDecompressStream : public ВхоФильтрПоток {
	Zlib         z;

public:
	проц открой(Поток& in)                  { z.Decompress(); уст(in, z); }

	бцел  GetCRC() const                  { return z.GetCRC(); }
	Ткст GetGZipName() const             { return z.GetGZipName(); }
	Ткст GetGZipComment() const          { return z.GetGZipComment(); }
	
	Zlib& GZip(бул gzip_ = true)          { return z.GZip(); }
	Zlib& Header(бул hdr_ = true)         { return z.Header(); }
	Zlib& NoHeader()                       { return Header(false); }
	Zlib& CRC(бул b = true)               { return z.CRC(); }
	Zlib& NoCRC()                          { return CRC(false); }
	Zlib& ChunkSize(цел n)                 { return z.ChunkSize(n); }
	Zlib& Level(цел compression_lvl)       { return z.Level(compression_lvl); }

	ZDecompressStream()                    {}
	ZDecompressStream(Поток& out)         { открой(out); }
	~ZDecompressStream()                   { закрой(); }
};

дол  копируйПоток(Поток& приёмник, Поток& ист, дол count, Врата<дол, дол> progress, цел chunk_size = 65536);

дол  ZCompress(Поток& out, Поток& in, дол size, Врата<дол, дол> progress = Null, бул hdr = true);
дол  ZCompress(Поток& out, Поток& in, Врата<дол, дол> progress = Null);
Ткст ZCompress(кук данные, дол len, Врата<дол, дол> progress = Null);
Ткст ZCompress(const Ткст& s, Врата<дол, дол> progress = Null);

дол  ZDecompress(Поток& out, Поток& in, дол size, Врата<дол, дол> progress = Null, бул hdr = true);
дол  ZDecompress(Поток& out, Поток& in, Врата<дол, дол> progress = Null);
Ткст ZDecompress(кук данные, дол len, Врата<дол, дол> progress = Null);
Ткст ZDecompress(const Ткст& s, Врата<дол, дол> progress = Null);

дол  GZCompress(Поток& out, Поток& in, дол size, Врата<дол, дол> progress = Null);
дол  GZCompress(Поток& out, Поток& in, Врата<дол, дол> progress = Null);
Ткст GZCompress(кук данные, цел len, Врата<дол, дол> progress = Null);
Ткст GZCompress(const Ткст& s, Врата<дол, дол> progress = Null);

дол  GZDecompress(Поток& out, Поток& in, дол size, Врата<дол, дол> progress = Null);
дол  GZDecompress(Поток& out, Поток& in, Врата<дол, дол> progress = Null);
Ткст GZDecompress(кук данные, цел len, Врата<дол, дол> progress = Null);
Ткст GZDecompress(const Ткст& s, Врата<дол, дол> progress = Null);

бул   GZCompressFile(кткст0 dstfile, кткст0 srcfile, Врата<дол, дол> progress = Null);
бул   GZCompressFile(кткст0 srcfile, Врата<дол, дол> progress = Null);

бул   GZDecompressFile(кткст0 dstfile, кткст0 srcfile, Врата<дол, дол> progress = Null);
бул   GZDecompressFile(кткст0 srcfile, Врата<дол, дол> progress = Null);

inline
бул   GZCompressFile(const Ткст& dstfile, const Ткст& srcfile, Врата<дол, дол> progress = Null)
{ return GZCompressFile(~dstfile, ~srcfile, progress); }
inline
бул   GZCompressFile(const Ткст& srcfile, Врата<дол, дол> progress = Null)
{ return GZCompressFile(~srcfile, progress); }

inline
бул   GZDecompressFile(const Ткст& dstfile, const Ткст& srcfile, Врата<дол, дол> progress = Null)
{ return GZDecompressFile(~dstfile, ~srcfile, progress); }
inline
бул   GZDecompressFile(const Ткст& srcfile, Врата<дол, дол> progress = Null)
{ return GZDecompressFile(~srcfile, progress); }

/// Backward compatibility:

Врата<дол, дол> AsGate64(Врата<цел, цел> gate);

inline цел ZCompress(Поток& out, Поток& in, Лаз2<цел, цел> progress)        { return (цел)ZCompress(out, in, AsGate64(progress)); }
inline Ткст ZCompress(кук данные, цел len, Лаз2<цел, цел> progress)   { return ZCompress(данные, len, AsGate64(progress)); }
inline Ткст ZCompress(const Ткст& s, Лаз2<цел, цел> progress)             { return ZCompress(s, AsGate64(progress)); }

inline цел    ZDecompress(Поток& out, Поток& in, Лаз2<цел, цел> progress)   { return (цел)ZDecompress(out, in, AsGate64(progress)); }
inline Ткст ZDecompress(const Ткст& s, Лаз2<цел, цел> progress)           { return ZDecompress(s, AsGate64(progress)); }
inline Ткст ZDecompress(кук данные, цел len, Лаз2<цел, цел> progress) { return ZDecompress(данные, len, AsGate64(progress)); }

inline цел    GZCompress(Поток& out, Поток& in, цел size, Лаз2<цел, цел> progress) { return (цел)GZCompress(out, in, size, AsGate64(progress)); }
inline цел    GZCompress(Поток& out, Поток& in, Лаз2<цел, цел> progress)           { return (цел)GZCompress(out, in, AsGate64(progress)); }
inline Ткст GZCompress(кук данные, цел len, Лаз2<цел, цел> progress)         { return GZCompress(данные, len, AsGate64(progress)); }
inline Ткст GZCompress(const Ткст& s, Лаз2<цел, цел> progress)                   { return GZCompress(s, AsGate64(progress)); }

inline цел    GZDecompress(Поток& out, Поток& in, цел size, Лаз2<цел, цел> progress) { return (цел)GZDecompress(out, in, size, AsGate64(progress)); }
inline цел    GZDecompress(Поток& out, Поток& in, Лаз2<цел, цел> progress)           { return (цел)GZDecompress(out, in, AsGate64(progress)); }
inline Ткст GZDecompress(кук данные, цел len, Лаз2<цел, цел> progress)         { return GZDecompress(данные, len, AsGate64(progress)); }
inline Ткст GZDecompress(const Ткст& s, Лаз2<цел, цел> progress)                   { return GZDecompress(s, AsGate64(progress)); }

Ткст сожмиБыстро(кук s, цел sz);
Ткст сожмиБыстро(const Ткст& s);
Ткст разожмиБыстро(const Ткст& данные);
