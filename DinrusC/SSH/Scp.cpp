#include "SSH.h"

namespace РНЦПДинрус {

#define LLOG(x)       do { if(SSH::sTrace) RLOG(SSH::дайИмя(ssh->otype, ssh->oid) << x); } while(false)
#define LDUMPHEX(x)	  do { if(SSH::sTraceVerbose) RDUMPHEX(x); } while(false)

bool Scp::OpenRead(const Ткст& path, ScpAttrs& attrs)
{
	обнули(attrs);
	return пуск([=, &attrs]() mutable {
		LIBSSH2_CHANNEL *ch = libssh2_scp_recv2(ssh->session, path, &attrs);
		if(!ch && !WouldBlock()) {
			LLOG("Unable to open file " << path);
			выведиОш(-1);
		}
		if(ch) {
			channel = сделайОдин<LIBSSH2_CHANNEL*>(ch);
			LLOG("Файл " << path << " opened.");
		}
		return ch;
	});
}

bool Scp::OpenWrite(const Ткст& path, int64 size, long mode)
{
	return пуск([=]() mutable {
		LIBSSH2_CHANNEL *ch = libssh2_scp_send64(ssh->session, path, mode, size, 0, 0);
		if(!ch && !WouldBlock()) {
			LLOG("Unable to open file " << path);
			выведиОш(-1);
		}
		if(ch) {
			channel = сделайОдин<LIBSSH2_CHANNEL*>(ch);
			LLOG("Файл " << path << " opened.");
		}
		return ch;
	});
}

bool Scp::грузи(Поток& s, ScpAttrs a, int64 maxsize)
{
	bool nowait = false;
	int64 done_ = 0;
	int64 size  = a.st_size;
	Ткст msg;
	
	if(size < 0 || size >= maxsize) {
		msg = "Invald stream size.";
	}
	else {
		WhenProgress(0, size);
		Буфер<byte> chunk(ssh->chunk_size);
		while(done_ < size && !кф_ли() && !ошибка_ли()) {
			int n = дай(chunk, (int) min<int64>(size - done_, ssh->chunk_size));
			if(n > 0) {
				done_ += n;
				s.помести(chunk, n);
				if((nowait = WhenProgress(done_, size))) {
					msg = "Файл transfer is aborted.";
					break;
				}
			}
		}
	}
	return Shut(msg, nowait);
}

bool Scp::сохрани(Поток& s)
{
	bool nowait = false;
	int64 done_ = 0;
	int64 size  = s.дайРазм();
	Ткст msg;
	
	WhenProgress(0, size);
	Буфер<byte> chunk(ssh->chunk_size);
	while(done_ < size && !кф_ли() && !ошибка_ли()) {
		int l = s.дай(chunk, (int) min<int64>(size - done_, ssh->chunk_size));
		int n = помести(chunk, l);
		if(n > 0) {
			done_ += n;
			if(n < l)
				s.перейди(n);
			if((nowait = WhenProgress(done_, size))) {
				msg = "Файл transfer is aborted.";;
				break;
			}
		}
	}
	return Shut(msg, nowait);
}

bool Scp::сохраниФайл(const char *path, const Ткст& данные)
{
	ТкстПоток s(данные);
	return OpenWrite(path, s.дайРазм()) && сохрани(s);
}

Ткст Scp::загрузиФайл(const char *path)
{
	ТкстПоток s;
	ScpAttrs attrs;
	if(OpenRead(path, attrs))
		грузи(s, attrs, INT_MAX);
	return pick(s.дайРез());
}

bool Scp::сохраниФайл(const char *path, Поток& in)
{
	 return OpenWrite(path, in.дайРазм()) && сохрани(in);
}

bool Scp::загрузиФайл(Поток& out, const char *path)
{
	ScpAttrs attrs;
	return OpenRead(path, attrs) && грузи(out, attrs);
}
}