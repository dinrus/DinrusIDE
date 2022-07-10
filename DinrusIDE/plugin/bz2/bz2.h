#ifndef __Plugin_BZ2__
#define __Plugin_BZ2__

#ifdef flagWIN32
	#include "lib/bzlib.h"
#else
	#include <bzlib.h>
#endif

namespace РНЦП {
	
namespace bz2 {
	class Lib {
		enum { NONE, DEFLATE, INFLATE };
	
		bz_stream     z;
		Буфер<char>  output;
		int           chunk;
		int           mode;
		int           total;
		int           compression_level;
		bool          Ошибка;
		bool          rdall;
		bool          eos;
		Ткст        out;
		
		void          устОш(bool v) { Ошибка = v; }
		void          PutOut(const void *ptr, int size);
		void          Pump(bool finish);
		void          старт();
		void          освободи();
		int           GzipHeader(const char *ptr, int size);
		void          иниц();
	
	public:
		Событие<const void *, int>  WhenOut;
		
		void Compress();
		void Decompress(bool all = true);
		void помести(const void *ptr, int size);
		void помести(const Ткст& s)              { помести(~s, s.дайСчёт()); }
		void стоп();
		void очисть();
		
		const Ткст& дай() const              { return out; }
		operator const Ткст&() const         { return out; }
		const Ткст& operator~() const        { return out; }
		void ClearOut()                        { out.очисть(); }
	
		bool   ошибка_ли() const                 { return Ошибка; }
		bool   IsEOS() const                   { return eos; }
		
		Lib& ChunkSize(int n);
		Lib& Level(int compression_lvl)        { compression_level = compression_lvl; return *this; }
	
		Lib();
		~Lib();
	};

	class CompressStream : public ВыхФильтрПоток {
		Lib z;
	
	public:
		void открой(Поток& out)                 { z.Compress(); уст(out, z); }
	
		Lib& ChunkSize(int n)                  { return z.ChunkSize(n); }
		Lib& Level(int compression_lvl)        { return z.Level(compression_lvl); }
	
		CompressStream()                       {}
		CompressStream(Поток& out)            { открой(out); }
		~CompressStream()                      { закрой(); }
	};
	
	class DecompressStream : public ВхоФильтрПоток {
		Lib z;
	
	public:
		void открой(Поток& in, bool all = true) { z.Decompress(all); уст(in, z); FilterEof = [=]() -> bool { return z.IsEOS(); }; }
		Lib& ChunkSize(int n)                  { return z.ChunkSize(n); }
	
		DecompressStream()                             {}
		DecompressStream(Поток& out, bool all = true) { открой(out, all); }
		~DecompressStream()                            { закрой(); }
	};
}

typedef bz2::CompressStream BZ2CompressStream;
typedef bz2::DecompressStream BZ2DecompressStream;

void          BZ2Compress(Поток& out, Поток& in, Врата<int, int> progress = Null);
void          BZ2Decompress(Поток& out, Поток& in, Врата<int, int> progress = Null);

Ткст        BZ2Compress(Поток& in, Врата<int, int> progress = Null);
Ткст        BZ2Decompress(Поток& in, Врата<int, int> progress = Null);

Ткст        BZ2Compress(const void *data, int64 len, Врата<int, int> progress);
Ткст        BZ2Decompress(const void *data, int64 len, Врата<int, int> progress = Null);

Ткст        BZ2Compress(const Ткст& data, Врата<int, int> progress = Null);
Ткст        BZ2Decompress(const Ткст& data, Врата<int, int> progress = Null);

}

#endif//__Plugin_Z__
