#ifndef _lz4_lz4_h_
#define _lz4_lz4_h_

#include <Core/Core.h>

#include <Core/lib/lz4.h>

namespace РНЦП {

enum {
	LZ4F_MAGIC       = 0x184D2204,

	LZ4F_VERSIONMASK = 0b11000000,
	LZ4F_VERSION     = 0b01000000,
	LZ4F_BLOCKINDEPENDENCE = (1 << 5),
	LZ4F_BLOCKCHECKSUM     = (1 << 4),
	LZ4F_CONTENTSIZE       = (1 << 3),
	LZ4F_CONTENTCHECKSUM   = (1 << 2),
	
	LZ4F_MAXSIZEMASK      = 0x70,
    LZ4F_MAXSIZE_64KB     = 0x40,
    LZ4F_MAXSIZE_256KB    = 0x50,
    LZ4F_MAXSIZE_1024KB   = 0x60,
    LZ4F_MAXSIZE_4096KB   = 0x70,
};

class LZ4CompressStream : public Поток  {
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

	enum { BLOCK_BYTES = 1024 * 1024 };
	
	ххХэшПоток xxh;

	bool          concurrent;
    
    void          размести();
	void          иниц();
	void          FlushOut();

public:
	void Co(bool b = true);
	void открой(Поток& out_);

	LZ4CompressStream();
	LZ4CompressStream(Поток& out) : LZ4CompressStream() { открой(out); }
	~LZ4CompressStream();
};

class LZ4DecompressStream : public Поток {
public:
	virtual   bool  открыт() const;

protected:
	virtual   int   _прекрати();
	virtual   int   _получи();
	virtual   dword _получи(void *data, dword size);

private:
	Поток        *in;
	struct Workblock {
		Буфер<char> c, d; // compressed, decompressed data
		int   clen = 0, dlen = 0; // compressed, decompressed len
		
		void очисть() { c.очисть(); d.очисть(); }
	};
	Workblock wb[16];
	int       count; // count of workblocks fetched
	int       ii; // next workblock to be read
	int       dlen; // length of current workblock
	
	enum { BLOCK_BYTES = 1024*1024 };
	
	ххХэшПоток xxh;
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

	void Co(bool b = true)                                  { concurrent = b; }

	LZ4DecompressStream();
	LZ4DecompressStream(Поток& in) : LZ4DecompressStream() { открой(in); }
	~LZ4DecompressStream();
};

int64  LZ4Compress(Поток& out, Поток& in, Врата<int64, int64> progress = Null);
int64  LZ4Decompress(Поток& out, Поток& in, Врата<int64, int64> progress = Null);
Ткст LZ4Compress(Поток& in, Врата<int64, int64> progress = Null);
Ткст LZ4Decompress(Поток& in, Врата<int64, int64> progress = Null);
Ткст LZ4Compress(const void *data, int64 len, Врата<int64, int64> progress = Null);
Ткст LZ4Compress(const Ткст& s, Врата<int64, int64> progress = Null);
Ткст LZ4Decompress(const void *data, int64 len, Врата<int64, int64> progress = Null);
Ткст LZ4Decompress(const Ткст& s, Врата<int64, int64> progress = Null);

int64  CoLZ4Compress(Поток& out, Поток& in, Врата<int64, int64> progress = Null);
int64  CoLZ4Decompress(Поток& out, Поток& in, Врата<int64, int64> progress = Null);
Ткст CoLZ4Compress(Поток& in, Врата<int64, int64> progress = Null);
Ткст CoLZ4Decompress(Поток& in, Врата<int64, int64> progress = Null);
Ткст CoLZ4Compress(const void *data, int64 len, Врата<int64, int64> progress = Null);
Ткст CoLZ4Compress(const Ткст& s, Врата<int64, int64> progress = Null);
Ткст CoLZ4Decompress(const void *data, int64 len, Врата<int64, int64> progress = Null);
Ткст CoLZ4Decompress(const Ткст& s, Врата<int64, int64> progress = Null);

bool IsLZ4(Поток& s);

};

#endif
