#include "SSH.h"

namespace ДинрусРНЦП {

проц SFtpStream::устПоз(дол pos)
{
	sftp->перейди(handle, pos);
}

проц SFtpStream::устРазмПотока(дол size)
{
	// TODO
}

бцел SFtpStream::читай(дол at, ук укз, бцел size)
{
	устПоз(at);
	цел n = sftp->дай(handle, укз, size);
	if(sftp->ошибка_ли()) устОш();
	return бцел(n);
}

проц SFtpStream::пиши(дол at, const ук данные, бцел size)
{
	устПоз(at);
	sftp->помести(handle, данные, size);
	if(sftp->ошибка_ли()) устОш();
}

проц SFtpStream::открой()
{
	if(handle) {
		слей();
		sftp->открой(handle);
		handle = nullptr;
	}
}

бул SFtpStream::открыт() const
{
	return handle;
}

бул SFtpStream::открой(SFtp& sftp_, кткст0 имяф, бцел mode, цел acm)
{
	if(открыт())
		открой();
	sftp = &sftp_;
	цел iomode = mode & ~SHAREMASK;
	handle = sftp->открой(имяф,
						iomode == READ
							? SFtp::READ
							: iomode == CREATE
								? SFtp::READ|SFtp::WRITE|SFtp::CREATE|SFtp::TRUNCATE
								: SFtp::READ|SFtp::WRITE,
						acm
						);
				
	if(handle) {
		SFtpAttrs attrs;
		if(!sftp->GetAttrs(handle, attrs)) {
			sftp->открой(handle);
			handle = nullptr;
			return false;
		}
		иницОткр(mode, attrs.filesize);
	}
	return handle;
}

SFtpStream::SFtpStream(SFtp& sftp, кткст0 имяф, бцел mode, цел acm)
: SFtpStream()
{
	открой(sftp, имяф, mode, acm);
}

SFtpStream::SFtpStream()
: sftp(nullptr)
, handle(nullptr)
{
}

SFtpStream::~SFtpStream()
{
	открой();
}

};