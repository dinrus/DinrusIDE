#include "zstd.h"

namespace РНЦП {
	
void sCompressStreamCopy_(Поток& out, Поток& in, Врата<int64, int64> progress, Поток& orig_in, int64 insz);

static int64 sZstdCompress(Поток& out, Поток& in, int64 size, Врата<int64, int64> progress, bool co)
{
	ZstdCompressStream outs(out);
	if(co)
		outs.Co();
	sCompressStreamCopy_(outs, in, progress, in, size);
	outs.закрой();
	if(!out.ошибка_ли() && !outs.ошибка_ли())
		return out.дайРазм();
	return -1;
}

static int64 sZstdDecompress(Поток& out, Поток& in, int64 size, Врата<int64, int64> progress, bool co)
{
	ZstdDecompressStream ins(in);
	if(co)
		ins.Co();
	sCompressStreamCopy_(out, ins, progress, in, size);
	ins.закрой();
	if(!out.ошибка_ли() && !ins.ошибка_ли())
		return out.дайРазм();
	return -1;
}

int64 ZstdCompress(Поток& out, Поток& in, Врата<int64, int64> progress)
{
	return sZstdCompress(out, in, in.GetLeft(), progress, false);
}

int64 ZstdDecompress(Поток& out, Поток& in, Врата<int64, int64> progress)
{
	return sZstdDecompress(out, in, in.GetLeft(), progress, false);
}

Ткст ZstdCompress(const void *data, int64 len, Врата<int64, int64> progress)
{
	ТкстПоток out;
	ПамЧтенПоток in(data, len);
	return ZstdCompress(out, in, progress) < 0 ? Ткст::дайПроц() : out.дайРез();
}

Ткст ZstdCompress(const Ткст& s, Врата<int64, int64> progress)
{
	return ZstdCompress(~s, s.дайДлину(), progress);
}

Ткст ZstdDecompress(const void *data, int64 len, Врата<int64, int64> progress)
{
	ТкстПоток out;
	ПамЧтенПоток in(data, len);
	return ZstdDecompress(out, in, progress) < 0 ? Ткст::дайПроц() : out.дайРез();
}

Ткст ZstdDecompress(const Ткст& s, Врата<int64, int64> progress)
{
	return ZstdDecompress(~s, s.дайДлину(), progress);
}

int64 CoZstdCompress(Поток& out, Поток& in, Врата<int64, int64> progress)
{
	return sZstdCompress(out, in, in.GetLeft(), progress, true);
}

int64 CoZstdDecompress(Поток& out, Поток& in, Врата<int64, int64> progress)
{
	return sZstdDecompress(out, in, in.GetLeft(), progress, true);
}

Ткст CoZstdCompress(const void *data, int64 len, Врата<int64, int64> progress)
{
	ТкстПоток out;
	ПамЧтенПоток in(data, len);
	return CoZstdCompress(out, in, progress) < 0 ? Ткст::дайПроц() : out.дайРез();
}

Ткст CoZstdCompress(const Ткст& s, Врата<int64, int64> progress)
{
	return CoZstdCompress(~s, s.дайДлину(), progress);
}

Ткст CoZstdDecompress(const void *data, int64 len, Врата<int64, int64> progress)
{
	ТкстПоток out;
	ПамЧтенПоток in(data, len);
	return CoZstdDecompress(out, in, progress) < 0 ? Ткст::дайПроц() : out.дайРез();
}

Ткст CoZstdDecompress(const Ткст& s, Врата<int64, int64> progress)
{
	return CoZstdDecompress(~s, s.дайДлину(), progress);
}

};