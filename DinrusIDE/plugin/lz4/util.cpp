#include "lz4.h"

namespace РНЦП {
	
void sCompressStreamCopy_(Поток& out, Поток& in, Врата<int64, int64> progress, Поток& orig_in, int64 insz);

static int64 sLZ4Compress(Поток& out, Поток& in, int64 size, Врата<int64, int64> progress, bool co)
{
	LZ4CompressStream outs(out);
	if(co)
		outs.Co();
	sCompressStreamCopy_(outs, in, progress, in, size);
	outs.закрой();
	if(!out.ошибка_ли() && !outs.ошибка_ли())
		return out.дайРазм();
	return -1;
}

static int64 sLZ4Decompress(Поток& out, Поток& in, int64 size, Врата<int64, int64> progress, bool co)
{
	LZ4DecompressStream ins(in);
	if(co)
		ins.Co();
	sCompressStreamCopy_(out, ins, progress, in, size);
	ins.закрой();
	if(!out.ошибка_ли() && !ins.ошибка_ли())
		return out.дайРазм();
	return -1;
}

int64 LZ4Compress(Поток& out, Поток& in, Врата<int64, int64> progress)
{
	return sLZ4Compress(out, in, in.GetLeft(), progress, false);
}

int64 LZ4Decompress(Поток& out, Поток& in, Врата<int64, int64> progress)
{
	return sLZ4Decompress(out, in, in.GetLeft(), progress, false);
}

Ткст LZ4Compress(Поток& in, Врата<int64, int64> progress)
{
	ТкстПоток out;
	return LZ4Compress(out, in, progress) < 0 ? Ткст::дайПроц() : out.дайРез();
}

Ткст LZ4Decompress(Поток& in, Врата<int64, int64> progress)
{
	ТкстПоток out;
	return LZ4Decompress(out, in, progress) < 0 ? Ткст::дайПроц() : out.дайРез();
}

Ткст LZ4Compress(const void *data, int64 len, Врата<int64, int64> progress)
{
	ПамЧтенПоток in(data, len);
	return LZ4Compress(in, progress);
}

Ткст LZ4Compress(const Ткст& s, Врата<int64, int64> progress)
{
	return LZ4Compress(~s, s.дайДлину(), progress);
}

Ткст LZ4Decompress(const void *data, int64 len, Врата<int64, int64> progress)
{
	ПамЧтенПоток in(data, len);
	return LZ4Decompress(in, progress);
}

Ткст LZ4Decompress(const Ткст& s, Врата<int64, int64> progress)
{
	return LZ4Decompress(~s, s.дайДлину(), progress);
}

int64 CoLZ4Compress(Поток& out, Поток& in, Врата<int64, int64> progress)
{
	return sLZ4Compress(out, in, in.GetLeft(), progress, true);
}

int64 CoLZ4Decompress(Поток& out, Поток& in, Врата<int64, int64> progress)
{
	return sLZ4Decompress(out, in, in.GetLeft(), progress, true);
}

Ткст CoLZ4Compress(Поток& in, Врата<int64, int64> progress)
{
	ТкстПоток out;
	return CoLZ4Compress(out, in, progress) < 0 ? Ткст::дайПроц() : out.дайРез();
}

Ткст CoLZ4Decompress(Поток& in, Врата<int64, int64> progress)
{
	ТкстПоток out;
	return CoLZ4Decompress(out, in, progress) < 0 ? Ткст::дайПроц() : out.дайРез();
}

Ткст CoLZ4Compress(const void *data, int64 len, Врата<int64, int64> progress)
{
	ТкстПоток out;
	ПамЧтенПоток in(data, len);
	return CoLZ4Compress(out, in, progress) < 0 ? Ткст::дайПроц() : out.дайРез();
}

Ткст CoLZ4Compress(const Ткст& s, Врата<int64, int64> progress)
{
	return CoLZ4Compress(~s, s.дайДлину(), progress);
}

Ткст CoLZ4Decompress(const void *data, int64 len, Врата<int64, int64> progress)
{
	ТкстПоток out;
	ПамЧтенПоток in(data, len);
	return CoLZ4Decompress(out, in, progress) < 0 ? Ткст::дайПроц() : out.дайРез();
}

Ткст CoLZ4Decompress(const Ткст& s, Врата<int64, int64> progress)
{
	return CoLZ4Decompress(~s, s.дайДлину(), progress);
}

};