#ifndef _plugin_lzma_lzma_h_
#define _plugin_lzma_lzma_h_

#include <Core/Core.h>

namespace РНЦП {

int64  LZMACompress(Поток& out, Поток& in, int64 size, Gate2<int64, int64> progress = false, int lvl = 6);
int64  LZMACompress(Поток& out, Поток& in, Gate2<int64, int64> progress = false, int lvl = 6);
Ткст LZMACompress(const void *data, int64 len, Gate2<int64, int64> progress = false, int lvl = 6);
Ткст LZMACompress(const Ткст& s, Gate2<int64, int64> progress = false, int lvl = 6);

int64  LZMADecompress(Поток& out, Поток& in, Gate2<int64, int64> progress = false);
Ткст LZMADecompress(const void *data, int64 len, Gate2<int64, int64> progress = false);
Ткст LZMADecompress(const Ткст& s, Gate2<int64, int64> progress = false);

bool   LZMACompressFile(const char *dstfile, const char *srcfile, Gate2<int64, int64> progress = false, int lvl = 6);
bool   LZMACompressFile(const char *srcfile, Gate2<int64, int64> progress, int lvl = 6);
bool   LZMADecompressFile(const char *dstfile, const char *srcfile, Gate2<int64, int64> progress = false);
bool   LZMADecompressFile(const char *srcfile, Gate2<int64, int64> progress);

}

#endif
