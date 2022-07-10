#include "Core.h"

BinObjInfo::BinObjInfo()
{
}

void BinObjInfo::Block::Compress(Ткст& data)
{
	switch(encoding) {
	case BinObjInfo::Block::ENC_BZ2: data = BZ2Compress(data); break;
	case BinObjInfo::Block::ENC_ZIP: data = ZCompress(data); break;
	case BinObjInfo::Block::ENC_LZ4: data = LZ4Compress(data); break;
	case BinObjInfo::Block::ENC_LZMA: data = LZMACompress(data); break;
	case BinObjInfo::Block::ENC_ZSTD: data = ZstdCompress(data); break;
	}
}

void BinObjInfo::Parse(СиПарсер& binscript, Ткст base_dir)
{
	while(!binscript.кф_ли()) {
		Ткст binid = binscript.читайИд();
		bool ba = (binid == "BINARY_ARRAY");
		bool bm = (binid == "BINARY_MASK");
		if(binid == "BINARY" || ba || bm) {
			binscript.передайСим('(');
			Block blk;
			blk.scriptline = binscript.дайСтроку();
			blk.ident = binscript.читайИд();
			МассивМап<int, Block>& brow = blocks.дайДобавь(blk.ident);
			binscript.передайСим(',');
			blk.Индекс = -1;
			if(ba) {
				blk.flags |= Block::FLG_ARRAY;
				blk.Индекс = binscript.читайЦел();
				if(blk.Индекс < 0 || blk.Индекс > 1000000)
					binscript.выведиОш(фмт("неверный индекс массива: %d", blk.Индекс));
				binscript.передайСим(',');
			}
			else if(bm) {
				blk.flags |= Block::FLG_MASK;
				blk.Индекс = brow.дайСчёт();
			}
			Ткст file = binscript.читайТкст();
			if(binscript.ид("ZIP"))
				blk.encoding = Block::ENC_ZIP;
			else if(binscript.ид("BZ2"))
				blk.encoding = Block::ENC_BZ2;
			else if(binscript.ид("LZ4"))
				blk.encoding = Block::ENC_LZ4;
			else if(binscript.ид("LZMA"))
				blk.encoding = Block::ENC_LZMA;
			else if(binscript.ид("ZSTD"))
				blk.encoding = Block::ENC_ZSTD;
			binscript.передайСим(')');
			ФайлПоиск ff;
			Ткст searchpath = нормализуйПуть(file, base_dir);
			Ткст searchdir = дайДиректориюФайла(searchpath);
			Вектор<Ткст> files;
			Вектор<int64> lengths;
			if(ff.ищи(searchpath))
				do
					if(ff.файл_ли()) {
						files.добавь(ff.дайИмя());
						lengths.добавь(ff.дайДлину());
					}
				while(ff.следщ());
			if(files.пустой())
				binscript.выведиОш(фмт("'%s' не найден или не является файлом", file));
			if(!(blk.flags & Block::FLG_MASK) && files.дайСчёт() > 1)
				binscript.выведиОш(фмт("Несколько файлов лбнаружено (напр., %s, %s) в режиме единичного файла", files[0], files[1]));
			IndexSort(files, lengths);
			for(int i = 0; i < files.дайСчёт(); i++) {
				blk.file = приставьИмяф(searchdir, files[i]);
				blk.length = (int)lengths[i];
				int f = brow.найди(blk.Индекс);
				if(f >= 0)
					binscript.выведиОш(фмт("%s[%d] уже встречалось на строке %d", blk.ident, blk.Индекс, brow[f].scriptline));
				if(blk.Индекс < 0 && !brow.пустой() || blk.Индекс >= 0 && !brow.пустой() && brow[0].Индекс < 0)
					binscript.выведиОш(фмт("%s: смешение элементов массива и немассива", blk.ident));
				brow.добавь(blk.Индекс, blk);
				if(!(blk.flags & Block::FLG_MASK))
					break;
				blk.Индекс++;
			}
		}
		else
			binscript.выведиОш("ожидался идентификатор элемента двоичного сценария");
	}
}
