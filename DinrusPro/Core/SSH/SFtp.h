class SFtp : public Ssh {
public:
    enum Flags {
        READ        = LIBSSH2_FXF_READ,
        WRITE       = LIBSSH2_FXF_WRITE,
        APPEND      = LIBSSH2_FXF_APPEND,
        CREATE      = LIBSSH2_FXF_CREAT,
        TRUNCATE    = LIBSSH2_FXF_TRUNC,
        EXCLUSIVE   = LIBSSH2_FXF_EXCL
     };

    enum Permissions {
        IRUSR = LIBSSH2_SFTP_S_IRUSR,
        IWUSR = LIBSSH2_SFTP_S_IWUSR,
        IXUSR = LIBSSH2_SFTP_S_IXUSR,
        IRWXU = LIBSSH2_SFTP_S_IRWXU,
        IRGRP = LIBSSH2_SFTP_S_IRGRP,
        IWGRP = LIBSSH2_SFTP_S_IWGRP,
        IXGRP = LIBSSH2_SFTP_S_IXGRP,
        IRWXG = LIBSSH2_SFTP_S_IRWXG,
        IROTH = LIBSSH2_SFTP_S_IROTH,
        IWOTH = LIBSSH2_SFTP_S_IWOTH,
        IXOTH = LIBSSH2_SFTP_S_IXOTH,
        IRWXO = LIBSSH2_SFTP_S_IRWXO,
        IRALL = IRUSR | IRGRP | IROTH,
        IWALL = IWUSR | IWGRP | IWOTH,
        IXALL = IXUSR | IXGRP | IXOTH,
        IRWXA = IRALL | IWALL | IXALL
    };

    class DirEntry : public Движ<DirEntry> {
        friend class SFtp;
        public:
            Ткст дайИмя() const                  { return имяф; }
            дол  дайУид() const                   { return (a->flags & LIBSSH2_SFTP_ATTR_UIDGID) ? a->uid : -1; }
            дол  дайГид() const                   { return (a->flags & LIBSSH2_SFTP_ATTR_UIDGID) ? a->gid : -1; }
            дол  дайРазм() const                  { return (a->flags & LIBSSH2_SFTP_ATTR_SIZE) ? a->filesize : -1; }
            Время   GetLastModified() const          { return (a->flags & LIBSSH2_SFTP_ATTR_ACMODTIME) ? времяИзУВИ(a->mtime) : Null; }
            Время   GetLastAccessed() const          { return (a->flags & LIBSSH2_SFTP_ATTR_ACMODTIME) ? времяИзУВИ(a->atime) : Null; }
            бцел  GetPermissions() const           { return a->permissions; }
            SFtpAttrs& GetAttrs()                   { return *a; }

            const SFtpAttrs& operator~() const      { return *a; }
            SFtpAttrs&  operator*()                 { return *a; }
            operator бул() const                   { return valid; }

            бул файл_ли() const                     { return LIBSSH2_SFTP_S_ISREG(a->permissions); }
            бул директория_ли() const                { return LIBSSH2_SFTP_S_ISDIR(a->permissions); }
            бул симссылка_ли() const                  { return LIBSSH2_SFTP_S_ISLNK(a->permissions); }
            бул спец_ли() const                  { return LIBSSH2_SFTP_S_ISCHR(a->permissions); }
            бул IsBlock() const                    { return LIBSSH2_SFTP_S_ISBLK(a->permissions); }
            бул IsPipe() const                     { return LIBSSH2_SFTP_S_ISFIFO(a->permissions); }
            бул IsSocket() const                   { return LIBSSH2_SFTP_S_ISSOCK(a->permissions); }
            бул IsReadable() const                 { return CanMode(IRUSR, IRGRP, IROTH); }
            бул IsWriteable() const                { return CanMode(IWUSR, IWGRP, IWOTH); }
            бул толькочтен_ли() const                 { return IsReadable() && !IsWriteable(); }
            бул экзе_ли() const               { return !директория_ли() && CanMode(IXUSR, IXGRP, IXOTH); }

            Ткст вТкст() const;
            ИнфОФС::ИнфОФайле ToFileInfo() const;

            проц сериализуй(Поток& s);
            
            DirEntry(const Ткст& path);
            DirEntry(const Ткст& path, const SFtpAttrs& attrs);
            DirEntry()                              { обнули();  }
            DirEntry(const Обнул&)                 { обнули();  }

            DirEntry(DirEntry&&) = default;
            DirEntry& operator=(DirEntry&&) = default;
            
            // Deprecated stuff.
            [[deprecated("Will be removed with U++ 2021.2.")]]
            Ткст ToXml() const;
            
        private:
            бул CanMode(бцел u, бцел g, бцел o) const;
            проц обнули();

            бул valid;
            Ткст имяф;
            Один<SFtpAttrs> a;
     };
    using DirList = Вектор<DirEntry>;

public:
    SFtp&                   Timeout(цел ms)                                         { ssh->timeout = ms; return *this; }
    SFtp&                   ChunkSize(цел sz)                                       { ssh->chunk_size = clamp(sz, 1, INT_MAX); return *this; }

    // Файл
    SFtpHandle              открой(const Ткст& path, бцел flags, long mode);
    SFtpHandle              OpenRead(const Ткст& path)                            { return открой(path, READ, IRALL); }
    SFtpHandle              OpenWrite(const Ткст& path)                           { return открой(path, CREATE | WRITE, IRALL | IWUSR); }
    проц                    открой(SFtpHandle handle);
    бул                    Rename(const Ткст& oldpath, const Ткст& newpath);
    бул                    Delete(const Ткст& path);
    бул                    синх(SFtpHandle handle);
    SFtp&                   перейди(SFtpHandle handle, дол position);
    дол                   дайПоз(SFtpHandle handle);

    // читай/пиши
    цел                     дай(SFtpHandle handle, ук укз, цел size = INT_MAX);
    цел                     помести(SFtpHandle handle, кук укз, цел size);
    бул                    сохраниФайл(кткст0 path, const Ткст& данные);
    Ткст                  загрузиФайл(кткст0 path);
    бул                    сохраниФайл(кткст0 path, Поток& in);
    бул                    загрузиФайл(Поток& out, кткст0 path);
    
    цел                     GetDone() const                                         { return done; }
    
    // Directory
    SFtpHandle              OpenDir(const Ткст& path);
    бул                    MakeDir(const Ткст& path, long mode);
    бул                    RemoveDir(const Ткст& path);
    бул                    ListDir(SFtpHandle handle, DirList& list);
    бул                    ListDir(const Ткст& path, DirList& list);
    // Symlink
    бул                    MakeLink(const Ткст& orig, Ткст& target)            { return SymLink(orig, target, LIBSSH2_SFTP_SYMLINK); }
    бул                    ReadLink(const Ткст& path, Ткст& target)            { return SymLink(path, target, LIBSSH2_SFTP_READLINK); }
    бул                    RealizePath(const Ткст& path, Ткст& target)         { return SymLink(path, target, LIBSSH2_SFTP_REALPATH); }

    Ткст                  GetDefaultDir()                                         { Ткст s; return RealizePath(".", s) ? s : Ткст::дайПроц(); }
    
    // Attributes
    бул                    GetAttrs(SFtpHandle handle, SFtpAttrs& attrs);
    бул                    GetAttrs(const Ткст& path, SFtpAttrs& attrs);
    бул                    устАтры(SFtpHandle handle, const SFtpAttrs& attrs);
    бул                    устАтры(const Ткст& path, const SFtpAttrs& attrs);
    DirEntry                GetInfo(const Ткст& path);
    бул                    устИнфо(const DirEntry& entry)                          { return устАтры(entry.дайИмя(), ~entry); }
    дол                   дайРазм(const Ткст& path)                             { return QueryAttr(path, SFTP_ATTR_SIZE); }
    бул                    устРазм(const Ткст& path, дол size)                 { return ModifyAttr(path, SFTP_ATTR_SIZE, size); }
    Время                    GetLastModifyTime(const Ткст& path)                   { return QueryAttr(path, SFTP_ATTR_LAST_MODIFIED); }
    бул                    устПоследнийModifyTime(const Ткст& path, const Время& time) { return ModifyAttr(path, SFTP_ATTR_LAST_MODIFIED, time); }
    Время                    дайВремяПоследнДоступа(const Ткст& path)                   { return QueryAttr(path, SFTP_ATTR_LAST_ACCESSED); }
    бул                    устПоследнийAccessTime(const Ткст& path, const Время& time) { return ModifyAttr(path, SFTP_ATTR_LAST_ACCESSED, time); }

    // Tests
    бул                    файлЕсть(const Ткст& path)                          { return QueryAttr(path, SFTP_ATTR_FILE); }
    бул                    дирЕсть(const Ткст& path)                     { return QueryAttr(path, SFTP_ATTR_DIRECTORY); }
    бул                    SymLinkExists(const Ткст& path)                       { return QueryAttr(path, SFTP_ATTR_SYMLINK); }
    бул                    SocketExists(const Ткст& path)                        { return QueryAttr(path, SFTP_ATTR_SOCKET); }
    бул                    PipeExists(const Ткст& path)                          { return QueryAttr(path, SFTP_ATTR_PIPE); }
    бул                    BlockExists(const Ткст& path)                         { return QueryAttr(path, SFTP_ATTR_BLOCK); }
    бул                    SpecialFileExists(const Ткст& path)                   { return QueryAttr(path, SFTP_ATTR_SPECIAL); }

    Врата<дол, дол>      WhenProgress;
    
    SFtp(SshSession& session);
    virtual ~SFtp();

    SFtp(SFtp&&) = default;

private:
    бул                    иниц() override;
    проц                    выход() override;

    цел                     FStat(SFtpHandle handle, SFtpAttrs& a, бул set);
    цел                     LStat(const Ткст& path, SFtpAttrs& a, цел тип);
    Значение                   QueryAttr(const Ткст& path, цел attr);
    бул                    ModifyAttr(const Ткст& path, цел attr, const Значение& v);
    бул                    SymLink(const Ткст& path, Ткст& target, цел тип);
    цел                     читай(SFtpHandle handle, ук укз, цел size);
    цел                     пиши(SFtpHandle handle, кук укз, цел size);
    бул                    CopyData(Поток& приёмник, Поток& ист, дол maxsize = ЦЕЛ64_МАКС);
  
    Один<LIBSSH2_SFTP*>      sftp_session;
    цел                     done;

    enum FileAttributes {
        SFTP_ATTR_FILE,
        SFTP_ATTR_DIRECTORY,
        SFTP_ATTR_SYMLINK,
        SFTP_ATTR_SOCKET,
        SFTP_ATTR_PIPE,
        SFTP_ATTR_BLOCK,
        SFTP_ATTR_SPECIAL,
        SFTP_ATTR_UID,
        SFTP_ATTR_GID,
        SFTP_ATTR_PERMISSIONS,
        SFTP_ATTR_SIZE,
        SFTP_ATTR_LAST_MODIFIED,
        SFTP_ATTR_LAST_ACCESSED
    };
};

class SFtpStream : public БлокПоток {
protected:
    virtual  проц  устРазмПотока(дол size);
    virtual  бцел читай(дол at, ук укз, бцел size);
    virtual  проц  пиши(дол at, кук данные, бцел size);

public:
    virtual  проц  открой();
    virtual  бул  открыт() const;

protected:
    SFtp       *sftp;
    SFtpHandle  handle;

    проц      устПоз(дол pos);
    проц      иниц(дол size);

public:
    operator  бул() const                 { return открыт(); }

    бул       открой(SFtp& sftp, кткст0 имяф, бцел mode, цел acm = 0644);
    SFtpStream(SFtp& sftp, кткст0 имяф, бцел mode, цел acm = 0644);
    SFtpStream();
    ~SFtpStream();
    SFtpHandle дайУк() const            { return handle; }
};

class SFtpFileOut : public SFtpStream {
public:
    бул открой(SFtp& sftp, кткст0 фн, цел acm = 0644) { return SFtpStream::открой(sftp, фн, CREATE|NOWRITESHARE, acm); }

    SFtpFileOut(SFtp& sftp, кткст0 фн)    { открой(sftp, фн); }
    SFtpFileOut()                              {}
};

class SFtpFileAppend : public SFtpStream {
public:
    бул открой(SFtp& sftp, кткст0 фн)      { return SFtpStream::открой(sftp, фн, APPEND|NOWRITESHARE); }

    SFtpFileAppend(SFtp& sftp, кткст0 фн) { открой(sftp, фн); }
    SFtpFileAppend()                           {}
};

class SFtpFileIn : public SFtpStream {
public:
    бул открой(SFtp& sftp, кткст0 фн)      { return SFtpStream::открой(sftp, фн, READ); }

    SFtpFileIn(SFtp& sftp, кткст0 фн)     { открой(sftp, фн); }
    SFtpFileIn()                               {}
};

class SFtpFileSystemInfo : public ИнфОФС {
public:
    SFtpFileSystemInfo& Mount(SFtp& sftp)       { browser = &sftp; return *this; }
    цел дайСтиль() const override               { return STYLE_POSIX; }
    бул создайПапку(Ткст path, Ткст& Ошибка) const override;
    Массив<ИнфОФС::ИнфОФайле> найди(Ткст mask, цел max_count = INT_MAX, бул unmounted = false) const override;
    
    SFtpFileSystemInfo()                        { browser = nullptr; }
    SFtpFileSystemInfo(SFtp& sftp)              { Mount(sftp); }
    virtual ~SFtpFileSystemInfo()               {}
    
private:
    SFtp *browser = nullptr;
};

