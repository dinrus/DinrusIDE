#include "SSH.h"

namespace ДинрусРНЦП {

#define LLOG(x)       do { if(SSH::sTrace) RLOG(SSH::дайИмя(ssh->otype, ssh->oid) << x); } while(false)
#define LDUMPHEX(x)	  do { if(SSH::sTraceVerbose) RDUMPHEX(x); } while(false)

бул Scp::OpenRead(const Ткст& path, ScpAttrs& attrs)
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

бул Scp::OpenWrite(const Ткст& path, дол size, long mode)
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

бул Scp::грузи(Поток& s, ScpAttrs a, дол maxsize)
{
	бул nowait = false;
	дол done_ = 0;
	дол size  = a.st_size;
	Ткст msg;
	
	if(size < 0 || size >= maxsize) {
		msg = "Invald stream size.";
	}
	else {
		WhenProgress(0, size);
		Буфер<ббайт> chunk(ssh->chunk_size);
		while(done_ < size && !кф_ли() && !ошибка_ли()) {
			цел n = дай(chunk, (цел) мин<дол>(size - done_, ssh->chunk_size));
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

бул Scp::сохрани(Поток& s)
{
	бул nowait = false;
	дол done_ = 0;
	дол size  = s.дайРазм();
	Ткст msg;
	
	WhenProgress(0, size);
	Буфер<ббайт> chunk(ssh->chunk_size);
	while(done_ < size && !кф_ли() && !ошибка_ли()) {
		цел l = s.дай(chunk, (цел) мин<дол>(size - done_, ssh->chunk_size));
		цел n = помести(chunk, l);
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

бул Scp::сохраниФайл(кткст0 path, const Ткст& данные)
{
	ТкстПоток s(данные);
	return OpenWrite(path, s.дайРазм()) && сохрани(s);
}

Ткст Scp::загрузиФайл(кткст0 path)
{
	ТкстПоток s;
	ScpAttrs attrs;
	if(OpenRead(path, attrs))
		грузи(s, attrs, INT_MAX);
	return pick(s.дайРез());
}

бул Scp::сохраниФайл(кткст0 path, Поток& in)
{
	 return OpenWrite(path, in.дайРазм()) && сохрани(in);
}

бул Scp::загрузиФайл(Поток& out, кткст0 path)
{
	ScpAttrs attrs;
	return OpenRead(path, attrs) && грузи(out, attrs);
}
}