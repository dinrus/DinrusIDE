#ifndef _plugin_zstd_zstd_h_
#define _plugin_zstd_zstd_h_

#include <Core/Core.h>

#define ZSTD_STATIC_LINKING_ONLY
#include "lib/zstd.h"

namespace РНЦП {

class ZstdCompressStream : public Поток  {
public:
	virtual   void  закрой();
	virtual   bool  открыт() const;

protected:
	virtual   void  _помести(int w);
	virtual   void  _помести(const void *data, dword size);
	
	Поток      *out;
	
	Буфер<byte> буфер;
	Буфер<byte> outbuf;
	Буфер<int>  outsz;

	enum { BLOCK_BYTES = 1024*1024 };
	
	int           level;
	
	bool          concurrent;
    
    void          размести();
	void          иниц();
	void          FlushOut();

public:
	void Co(bool b = true);
	void открой(Поток& out, int level = 1);

	ZstdCompressStream();
	ZstdCompressStream(Поток& out, int level = 1) : ZstdCompressStream() { открой(out, level); }
	~ZstdCompressStream();
};

class ZstdDecompressStream : public Поток {
public:
	virtual   bool  открыт() const;

protected:
	virtual   int   _прекрати();
	virtual   int   _получи();
	virtual   dword _получи(void *data, dword size);

private:
	Поток        *in;
	struct Workblock {
		Ткст       compressed_data; // can contain more frames
		int          frame_at, frame_sz; // position and compressed size of frame
		int          decompressed_sz;
		Буфер<char> decompressed_data; // decompressed data
		bool         irregular_d = false; // d reallocated to accomodate bigger result
		
		const void *FramePtr() { return ~compressed_data + frame_at; }
		void очисть()           { compressed_data.очисть(); decompressed_data.очисть(); irregular_d = false; }
	};
	
	Ткст compressed_data; // буфер to store compressed data
	int    compressed_at; // where are we in above буфер
	
	Workblock wb[16];
	int       count; // count of workblocks fetched
	int       ii; // next workblock to be read
	int       dlen; // length of current workblock
	
	enum { BLOCK_BYTES = 1024*1024 }; // expected decompressed size
	
	int          maxblock;
	int          blockchksumsz;
	byte         lz4hdr;
	bool         eof;
	
	bool         concurrent;

    void          TryHeader();

	void          иниц();
	bool          следщ();
	void          Fetch();
	bool          Ended() const { return ошибка_ли() || in->ошибка_ли() || ptr == rdlim && ii == count && eof; }

public:	
	bool открой(Поток& in);
	void Co(bool b = true)                                    { concurrent = b; }

	ZstdDecompressStream();
	ZstdDecompressStream(Поток& in) : ZstdDecompressStream() { открой(in); }
	~ZstdDecompressStream();
};

int64 ZstdCompress(Поток& out, Поток& in, Врата<int64, int64> progress = Null);
int64 ZstdDecompress(Поток& out, Поток& in, Врата<int64, int64> progress = Null);
Ткст ZstdCompress(const void *data, int64 len, Врата<int64, int64> progress = Null);
Ткст ZstdCompress(const Ткст& s, Врата<int64, int64> progress = Null);
Ткст ZstdDecompress(const void *data, int64 len, Врата<int64, int64> progress = Null);
Ткст ZstdDecompress(const Ткст& s, Врата<int64, int64> progress = Null);

int64 CoZstdCompress(Поток& out, Поток& in, Врата<int64, int64> progress = Null);
int64 CoZstdDecompress(Поток& out, Поток& in, Врата<int64, int64> progress = Null);
Ткст CoZstdCompress(const void *data, int64 len, Врата<int64, int64> progress = Null);
Ткст CoZstdCompress(const Ткст& s, Врата<int64, int64> progress = Null);
Ткст CoZstdDecompress(const void *data, int64 len, Врата<int64, int64> progress = Null);
Ткст CoZstdDecompress(const Ткст& s, Врата<int64, int64> progress = Null);

bool IsZstd(Поток& s);

}

#endif
