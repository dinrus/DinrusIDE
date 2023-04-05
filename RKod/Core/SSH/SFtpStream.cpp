#include "SSH.h"

namespace РНЦПДинрус {

void SFtpStream::устПоз(int64 pos)
{
	sftp->перейди(handle, pos);
}

void SFtpStream::устРазмПотока(int64 size)
{
	// TODO
}

dword SFtpStream::читай(int64 at, void *ptr, dword size)
{
	устПоз(at);
	int n = sftp->дай(handle, ptr, size);
	if(sftp->ошибка_ли()) устОш();
	return dword(n);
}

void SFtpStream::пиши(int64 at, const void *данные, dword size)
{
	устПоз(at);
	sftp->помести(handle, данные, size);
	if(sftp->ошибка_ли()) устОш();
}

void SFtpStream::открой()
{
	if(handle) {
		слей();
		sftp->открой(handle);
		handle = nullptr;
	}
}

bool SFtpStream::открыт() const
{
	return handle;
}

bool SFtpStream::открой(SFtp& sftp_, const char *имяф, dword mode, int acm)
{
	if(открыт())
		открой();
	sftp = &sftp_;
	int iomode = mode & ~SHAREMASK;
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

SFtpStream::SFtpStream(SFtp& sftp, const char *имяф, dword mode, int acm)
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