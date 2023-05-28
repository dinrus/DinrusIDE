#include "SSH.h"

namespace ДинрусРНЦП {

#define LLOG(x)       do { if(SSH::sTrace) RLOG(SSH::дайИмя(ssh->otype, ssh->oid) << x); } while(false)
#define LDUMPHEX(x)	  do { if(SSH::sTraceVerbose) RDUMPHEX(x); } while(false)

бул SFtp::иниц()
{
	if(!ssh->session)
		выведиОш(-1, "SSH session is invalid.");
	auto session = libssh2_sftp_init(ssh->session);
	if(!session && !WouldBlock())
		выведиОш(-1);
	if(session) {
		sftp_session = сделайОдин<LIBSSH2_SFTP*>(session);
		LLOG("Сессия успешно инициализована.");
	}
	return session;
}

проц SFtp::выход()
{
	if(!sftp_session)
		return;

	пуск([=]() mutable {
		if(WouldBlock(libssh2_sftp_shutdown(*sftp_session)))
			return false;
		ssh->init = false;
		sftp_session.очисть();
		LLOG("Сессия деинициализована.");
		return true;
	}, false);
}

цел SFtp::FStat(SFtpHandle handle, SFtpAttrs& a, бул set)
{
	цел rc = libssh2_sftp_fstat_ex(handle, &a, set);
	return WouldBlock(rc) ?  1 : rc;
}

цел SFtp::LStat(const Ткст& path, SFtpAttrs& a, цел тип)
{
	цел rc = libssh2_sftp_stat_ex(*sftp_session, ~path, path.дайДлину(), тип, &a);
	return WouldBlock(rc) ?  1 : rc;
}

SFtpHandle SFtp::открой(const Ткст& path, бцел flags, long mode)
{
	SFtpHandle h = nullptr;

	пуск([=, &h] () mutable {
		h = libssh2_sftp_open(*sftp_session, path, flags, mode);
		if(!h && !WouldBlock())
			выведиОш(-1);
		if(h)
			LLOG(фмт("Файл '%s' успешно открыт.", path));
		return h;
	});
	return h;
}

проц SFtp::открой(SFtpHandle handle)
{
	if(!handle)
		return;

	пуск([=] () mutable {
		цел rc = libssh2_sftp_close_handle(handle);
		if(!rc)	LLOG("Хэндл файла освобождён.");
		return !rc;
	}, false);
}

бул SFtp::Rename(const Ткст& oldpath, const Ткст& newpath)
{
	return пуск([=] () mutable {
		цел rc = libssh2_sftp_rename(*sftp_session, oldpath, newpath);
		if(!WouldBlock(rc) && rc != 0)
			выведиОш(rc);
		if(rc == 0)
			LLOG(фмт("'%s' успешно переименован в '%s'", oldpath, newpath));
		return !rc;
	});
}

бул SFtp::Delete(const Ткст& path)
{
	return пуск([=] () mutable {
		цел rc = libssh2_sftp_unlink(*sftp_session, path);
		if(!WouldBlock(rc) && rc != 0)
			выведиОш(rc);
		if(rc == 0)
			LLOG(фмт("Файл '%s' успешно удалён.", path));
		return !rc;
	});
}

бул SFtp::синх(SFtpHandle handle)
{
	return пуск([=] () mutable {
		цел rc = libssh2_sftp_fsync(handle);
		if(!WouldBlock(rc) && rc != 0)
			выведиОш(rc);
		if(rc == 0)
			LLOG("Файл успешно синхронизован на диск.");
		return !rc;
	});
}

SFtp& SFtp::перейди(SFtpHandle handle, дол position)
{
	INTERLOCKED
	{
		// LLOG("Seeking to offset: " << position);
		libssh2_sftp_seek64(handle, position);
	}
	return *this;
}

дол SFtp::дайПоз(SFtpHandle handle)
{
	дол pos = 0;

	INTERLOCKED
	{
		pos = libssh2_sftp_tell64(handle);
		// LLOG("Файл position: " << pos);
	};
	return pos;
}

цел SFtp::дай(SFtpHandle handle, ук укз, цел size)
{
	done = 0;

	пуск([=]() mutable {
		while(done < size && !таймаут_ли()) {
			цел rc = static_cast<цел>(
				libssh2_sftp_read(handle, (char*) укз + done, мин(size - done, ssh->chunk_size))
			);
			if(rc < 0) {
				if(!WouldBlock(rc))
					выведиОш(rc);
				return false;
			}
			else
			if(rc == 0) {
				LLOG("Получен КФ.");
				break;
			}
			done += rc;
			ssh->start_time = msecs();
			UpdateClient();
		}
		return true;
	});

	return GetDone();
}

цел SFtp::помести(SFtpHandle handle, const ук укз, цел size)
{
	done = 0;

	пуск([=]() mutable {
		while(done < size && !таймаут_ли()) {
			цел rc = static_cast<цел>(
				libssh2_sftp_write(handle, (const char*) укз + done, мин(size - done, ssh->chunk_size))
			);
			if(rc < 0) {
				if(!WouldBlock(rc))
					выведиОш(rc);
				return false;
			}
			else
			if(rc == 0) {
				LLOG("Получен КФ.");
				break;
			}
			done += rc;
			ssh->start_time = msecs();
			UpdateClient();
		}
		return true;
	});

	return GetDone();
}

бул SFtp::CopyData(Поток& dest, Поток& ист, дол maxsize)
{
	if(ошибка_ли())
		return false;

	дол size = ист.дайРазм(), count = 0;
	Буфер<ббайт> chunk(ssh->chunk_size, 0);

	WhenProgress(0, size);

	while(!ист.кф_ли()) {
		цел n = ист.дай(chunk, (цел) мин<дол>(size - count, ssh->chunk_size));
		if(n > 0) {
			dest.помести(chunk, n);
			if(dest.ошибка_ли()) {
				LLOG("Ошибка записи в поток. " + ист.дайТекстОш());
				return false;
			}
			count += n;
			if(WhenProgress(count, size)) {
				return false;
			}
		}
		if(ист.ошибка_ли()) {
			LLOG("Ошибка чтения из потока. " + ист.дайТекстОш());
			break;
		}
	}
	return !ист.ошибка_ли();
}

бул SFtp::сохраниФайл(кткст0 path, const Ткст& данные)
{
	ТкстПоток in(данные);
	SFtpFileOut out(*this, path);
	return CopyData(out, in);
}

Ткст SFtp::загрузиФайл(кткст0 path)
{
	ТкстПоток out;
	SFtpFileIn in(*this, path);
	CopyData(out, in, INT_MAX);
	return пикуй(out.дайРез());
}

бул SFtp::сохраниФайл(кткст0 path, Поток& in)
{
	SFtpFileOut out(*this, path);
	return CopyData(out, in);
}

бул SFtp::загрузиФайл(Поток& out, кткст0 path)
{
	SFtpFileIn in(*this, path);
	return CopyData(out, in);
}

SFtpHandle SFtp::OpenDir(const Ткст& path)
{
	SFtpHandle h = nullptr;

	пуск([=, &h] () mutable {
		h = libssh2_sftp_opendir(*sftp_session, path);
		if(!h && !WouldBlock())
			выведиОш(-1);
		if(h)
			LLOG(фмт("Directory '%s' is successfully opened.", path));
		return h;
	});
	return h;
}

бул SFtp::MakeDir(const Ткст& path, long mode)
{
	return пуск([=] () mutable {
		цел rc = libssh2_sftp_mkdir(*sftp_session, path, mode);
		if(!WouldBlock(rc) && rc != 0)
			выведиОш(rc);
		if(!rc)
			LLOG(фмт("Directory '%s' is succesfully created.", path));
		return !rc;
	});
}

бул SFtp::RemoveDir(const Ткст& path)
{
	return пуск([=] () mutable {
		цел rc = libssh2_sftp_rmdir(*sftp_session, path);
		if(!WouldBlock(rc) && rc != 0)
			выведиОш(rc);
		if(!rc)
			LLOG(фмт("Directory '%s' is succesfully deleted.", path));
		return !rc;
	});
}

бул SFtp::ListDir(SFtpHandle handle, DirList& list)
{
	SFtpAttrs attrs;
	return пуск([=, &attrs, &list] () mutable {
		char label[512];
		char longentry[512];

		while(InProgress()) {
			обнули(attrs);
			цел rc = libssh2_sftp_readdir_ex(
						handle,
						label, sizeof(label),
						longentry, sizeof(longentry),
						&attrs
			);
			if(rc < 0) {
				if(!WouldBlock(rc))
					выведиОш(rc);
				break;
			}
			else
			if(rc > 0) {
				DirEntry& entry	= list.добавь();
				entry.имяф	= label;
				*entry.a		= attrs;
				entry.valid		= true;
//				DUMP(entry);
			}
			if(rc == 0) {
				LLOG(фмт("Directory listing is successful. (%d entries)", list.дайСчёт()));
				return true;
			}
		}
		return false;
	});
}

бул SFtp::ListDir(const Ткст& path, DirList& list)
{
	SFtpHandle h = OpenDir(path);
	if(h) {
		бул b = ListDir(h, list);
		открой(h);
		if(!b) ssh->status = FAILED;
	}
	return !ошибка_ли();
}

бул SFtp::SymLink(const Ткст& path, Ткст& target, цел тип)
{
	Буфер<char> буфер(512, 0);

	if(тип == LIBSSH2_SFTP_SYMLINK)
		return пуск([=, &path, &буфер, &target] () mutable {
			цел rc = libssh2_sftp_symlink_ex(
						*sftp_session,
						path,
						path.дайДлину(),
						буфер,
						512,
						тип
					);
			if(!WouldBlock(rc) && rc != 0)
				выведиОш(rc);
			if(!rc) {
				target.уст(буфер, rc);
				LLOG(фмт("Symbolic link '%s' for path '%s' is successfult created.", target, path));
			}
			return !rc;
		});
	else
		return пуск([=, &path, &буфер, &target] () mutable {
			Буфер<char> sbuf(512, 0);
			цел rc = libssh2_sftp_symlink_ex(
						*sftp_session,
						path,
						path.дайДлину(),
						буфер,
						512,
						тип
					);
			if(!WouldBlock(rc) && rc < 0)
				выведиОш(rc);
			if(rc > 0) {
				target.уст(буфер, rc);
				LLOG("Symbolic link operation is successful. Target: " << target);
			}
			return rc > 0;
		});
}

SFtp::DirEntry SFtp::GetInfo(const Ткст& path)
{
	DirEntry finfo;

	if(!GetAttrs(path, *finfo))
		return Null;

	finfo.имяф = path;
	finfo.valid = true;

	return пикуй(finfo);
}

бул SFtp::GetAttrs(SFtpHandle handle, SFtpAttrs& attrs)
{
	return пуск([=, &attrs] () mutable {
		цел rc = FStat(handle, attrs, false);
		if(rc < 0)	выведиОш(rc);
		if(!rc)	LLOG("Атрибуты файла успешно обретены.");
		return !rc;
	});
}

бул SFtp::GetAttrs(const Ткст& path, SFtpAttrs& attrs)
{
	return пуск([=, &path, &attrs] () mutable {
		цел rc = LStat(path, attrs, LIBSSH2_SFTP_STAT);
		if(rc < 0)	выведиОш(rc);
		if(!rc) LLOG(фмт("Атрибуты файла '%s' успешно обретены.", path));
		return !rc;
	});
}

бул SFtp::устАтры(SFtpHandle handle, const SFtpAttrs& attrs)
{
	return пуск([=, &attrs] () mutable {
		цел rc = FStat(handle, const_cast<SFtpAttrs&>(attrs), true);
		if(rc < 0)	выведиОш(rc);
		if(!rc)	LLOG("Атрибуты файла успешно изменены.");
		return !rc;
	});
}

бул SFtp::устАтры(const Ткст& path, const SFtpAttrs& attrs)
{
	return пуск([=, &attrs] () mutable {
		цел rc = LStat(path,  const_cast<SFtpAttrs&>(attrs), LIBSSH2_SFTP_SETSTAT);
		if(rc < 0)	выведиОш(rc);
		if(!rc)	LLOG(фмт("Атрибуты файла '%s' успешно изменены.", path));
		return !rc;
	});
}

Значение SFtp::QueryAttr(const Ткст& path, цел attr)
{
	DirEntry finfo;
	Значение v;

	if(!GetAttrs(path, *finfo))
		return Null;

	switch(attr) {
		case SFTP_ATTR_FILE:
			v = finfo.файл_ли();
			break;
		case SFTP_ATTR_DIRECTORY:
			v = finfo.директория_ли();
			break;
		case SFTP_ATTR_SOCKET:
			v = finfo.IsSocket();
			break;
		case SFTP_ATTR_SYMLINK:
			v = finfo.симссылка_ли();
			break;
		case SFTP_ATTR_PIPE:
			v = finfo.IsPipe();
			break;
		case SFTP_ATTR_BLOCK:
			v = finfo.IsBlock();
			break;
		case SFTP_ATTR_SPECIAL:
			v = finfo.спец_ли();
			break;
		case SFTP_ATTR_SIZE:
			v = finfo.дайРазм();
			break;
		case SFTP_ATTR_LAST_MODIFIED:
			v = finfo.GetLastModified();
			break;
		case SFTP_ATTR_LAST_ACCESSED:
			v = finfo.GetLastAccessed();
			break;
		default:
			break;
	}
	return пикуй(v);
}

бул SFtp::ModifyAttr(const Ткст& path, цел attr, const Значение& v)
{
	DirEntry finfo;

	if(!GetAttrs(path, *finfo))
		return false;

	SFtpAttrs& attrs = *finfo;

	switch(attr) {
		case SFTP_ATTR_SIZE:
			attrs.flags |= LIBSSH2_SFTP_ATTR_SIZE;
			attrs.filesize = v.To<дол>();
			break;
		case SFTP_ATTR_LAST_MODIFIED:
			attrs.flags |= LIBSSH2_SFTP_ATTR_ACMODTIME;
			attrs.mtime = GetUTCSeconds(v);
			break;
		case SFTP_ATTR_LAST_ACCESSED:
			attrs.flags |= LIBSSH2_SFTP_ATTR_ACMODTIME;
			attrs.atime = GetUTCSeconds(v);
			break;
		default:
			break;
	}
	return устАтры(path, ~finfo);;
}

SFtp::SFtp(SshSession& session)
: Ssh()
, done(0)
{
	ssh->otype		= SFTP;
	ssh->session	= session.дайУк();
	ssh->socket		= &session.GetSocket();
	ssh->timeout	= session.GetTimeout();
	ssh->waitstep   = session.GetWaitStep();
	ssh->whenwait   = прокси(session.WhenWait);
}

SFtp::~SFtp()
{
	выход();
}

Ткст SFtp::DirEntry::вТкст() const
{
	if(!valid) return "<N/A>";
	static кткст0 hypen = "-", *r = "r", *w = "w", *x = "x";
	return фмт("%c%c%c%c%c%c%c%c%c%c %5<d %5<d %12>d %s %s %s",
				(файл_ли()
				? *hypen : (директория_ли()
				? 'd' : (симссылка_ли()
				? 'l' : (IsSocket()
				? 's' : (IsPipe()
				? 'p' : (IsBlock()
				? 'b' : (спец_ли()
				? 'c' : 'o' ))))))),
				((a->permissions & IRUSR) ? *r : *hypen),
				((a->permissions & IWUSR) ? *w : *hypen),
				((a->permissions & IXUSR) ? *x : *hypen),
				((a->permissions & IRGRP) ? *r : *hypen),
				((a->permissions & IWGRP) ? *w : *hypen),
				((a->permissions & IXGRP) ? *x : *hypen),
				((a->permissions & IROTH) ? *r : *hypen),
				((a->permissions & IWOTH) ? *w : *hypen),
				((a->permissions & IXOTH) ? *x : *hypen),
				дайУид(),
				дайГид(),
				дайРазм(),
				какТкст(GetLastModified()),
				какТкст(GetLastAccessed()),
				дайИмя());
}

Ткст SFtp::DirEntry::ToXml() const
{
	if(!valid) return ТэгРяр("<N/A>").устТекст("<N/A>");
	static кткст0 hypen = "-", *r = "r", *w = "w", *x = "x";
	return ТэгРяр("sftp:direntry")
			("тип", (файл_ли()
				? "file" : (директория_ли()
				? "directory" : (симссылка_ли()
				? "symlink" : (IsSocket()
				? "socket" : (IsPipe()
				? "pipe" : (IsBlock()
				? "block-СПЕЦ" : (спец_ли()
				? "character-СПЕЦ" : "other")
			)))))))
			("uid", какТкст(дайУид()))
			("gid", какТкст(дайГид()))
			("size", какТкст(дайРазм()))
			("modified", какТкст(GetLastModified()))
			("accessed", какТкст(GetLastAccessed()))
			("permissions", фмт("%c%c%c%c%c%c%c%c%c",
				((a->permissions & IRUSR) ? *r : *hypen),
				((a->permissions & IWUSR) ? *w : *hypen),
				((a->permissions & IXUSR) ? *x : *hypen),
				((a->permissions & IRGRP) ? *r : *hypen),
				((a->permissions & IWGRP) ? *w : *hypen),
				((a->permissions & IXGRP) ? *x : *hypen),
				((a->permissions & IROTH) ? *r : *hypen),
				((a->permissions & IWOTH) ? *w : *hypen),
				((a->permissions & IXOTH) ? *x : *hypen)
			))
			.устТекст(дайИмя());
}

ИнфОФС::ИнфОФайле SFtp::DirEntry::ToFileInfo() const
{
	ИнфОФС::ИнфОФайле fi;
	fi.имяф         = дайИмя();
	fi.length           = дайРазм();
	fi.unix_mode        = GetPermissions();
	fi.creation_time    = GetLastModified();
	fi.last_access_time = GetLastAccessed();
	fi.last_write_time  = GetLastModified();
	fi.is_file          = файл_ли();
	fi.is_directory     = директория_ли();
	fi.is_symlink       = симссылка_ли();
	fi.is_read_only     = толькочтен_ли();
	fi.root_style       = ИнфОФС::ROOT_FIXED;
	return пикуй(fi);
}

проц SFtp::DirEntry::сериализуй(Поток& s)
{
	ПРОВЕРЬ(a);

	s % a->flags;
	s % a->permissions;
	s % a->uid;
	s % a->gid;
	s % a->filesize;
	s % a->mtime;
	s % a->atime;
	s % имяф;
}

бул SFtp::DirEntry::CanMode(бцел u, бцел g, бцел o) const
{
	return (a->flags & LIBSSH2_SFTP_ATTR_PERMISSIONS)
		&& (a->permissions & o || a->permissions & g || a->permissions & u);
}

проц SFtp::DirEntry::обнули()
{
	a.создай();
	ДинрусРНЦП::обнули(*a);
	valid = false;
}

SFtp::DirEntry::DirEntry(const Ткст& path)
: DirEntry()
{
	имяф = path;
}

SFtp::DirEntry::DirEntry(const Ткст& path, const SFtpAttrs& attrs)
: DirEntry()
{
	имяф = path;
	*a = attrs;
}

Массив<ИнфОФС::ИнфОФайле> SFtpFileSystemInfo::найди(Ткст mask, цел max_count, бул unmounted) const
{
	ПРОВЕРЬ(browser);
	Массив<ИнфОФайле> fi;
	Ткст dir;
	if(!browser->InProgress()) {
		if(пусто_ли(mask))
			dir = browser->GetDefaultDir();
		else {
			Ткст s;
			dir = дайДиректориюФайла(browser->RealizePath(mask, s) ? s : mask);
		}
		бул haswc = HasWildcards(mask);
		if(!haswc && max_count == 1) { // A small optimization.
			SFtp::DirEntry e = browser->GetInfo(mask);
			if(e)
				fi.добавь(e.ToFileInfo());
		}
		else {
			Ткст pattern;
			SFtp::DirList ls;
			if(browser->ListDir(dir, ls)) {
				if(haswc)
					pattern = дайИмяф(mask);
				for(цел i = 0, j = 0; i < ls.дайСчёт() && j < max_count; i++) {
					const SFtp::DirEntry& e = ls[i];
					if(!haswc || PatternMatch(pattern, e.дайИмя())) {
						fi.добавь(e.ToFileInfo());
						j++;
					}
				}
			}
		}
	}
	return пикуй(fi);
}

бул SFtpFileSystemInfo::создайПапку(Ткст path, Ткст& Ошибка) const
{
	ПРОВЕРЬ(browser);
	if(!browser->MakeDir(path, 0))
		Ошибка = browser->GetErrorDesc();
	return !browser->ошибка_ли();
}
}
