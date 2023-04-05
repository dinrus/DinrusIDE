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

    class DirEntry : public Движимое<DirEntry> {
        friend class SFtp;
        public:
            Ткст дайИмя() const                  { return имяф; }
            int64  дайУид() const                   { return (a->flags & LIBSSH2_SFTP_ATTR_UIDGID) ? a->uid : -1; }
            int64  дайГид() const                   { return (a->flags & LIBSSH2_SFTP_ATTR_UIDGID) ? a->gid : -1; }
            int64  дайРазм() const                  { return (a->flags & LIBSSH2_SFTP_ATTR_SIZE) ? a->filesize : -1; }
            Время   GetLastModified() const          { return (a->flags & LIBSSH2_SFTP_ATTR_ACMODTIME) ? TimeFromUTC(a->mtime) : Null; }
            Время   GetLastAccessed() const          { return (a->flags & LIBSSH2_SFTP_ATTR_ACMODTIME) ? TimeFromUTC(a->atime) : Null; }
            dword  GetPermissions() const           { return a->permissions; }
            SFtpAttrs& GetAttrs()                   { return *a; }

            const SFtpAttrs& operator~() const      { return *a; }
            SFtpAttrs&  operator*()                 { return *a; }
            operator bool() const                   { return valid; }

            bool файл_ли() const                     { return LIBSSH2_SFTP_S_ISREG(a->permissions); }
            bool директория_ли() const                { return LIBSSH2_SFTP_S_ISDIR(a->permissions); }
            bool симссылка_ли() const                  { return LIBSSH2_SFTP_S_ISLNK(a->permissions); }
            bool особый_ли() const                  { return LIBSSH2_SFTP_S_ISCHR(a->permissions); }
            bool IsBlock() const                    { return LIBSSH2_SFTP_S_ISBLK(a->permissions); }
            bool IsPipe() const                     { return LIBSSH2_SFTP_S_ISFIFO(a->permissions); }
            bool IsSocket() const                   { return LIBSSH2_SFTP_S_ISSOCK(a->permissions); }
            bool IsReadable() const                 { return CanMode(IRUSR, IRGRP, IROTH); }
            bool IsWriteable() const                { return CanMode(IWUSR, IWGRP, IWOTH); }
            bool толькочтен_ли() const                 { return IsReadable() && !IsWriteable(); }
            bool экзе_ли() const               { return !директория_ли() && CanMode(IXUSR, IXGRP, IXOTH); }

            Ткст вТкст() const;
            ИнфОФС::ИнфОФайле ToFileInfo() const;

            void сериализуй(Поток& s);
            
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
            bool CanMode(dword u, dword g, dword o) const;
            void обнули();

            bool valid;
            Ткст имяф;
            Один<SFtpAttrs> a;
     };
    using DirList = Вектор<DirEntry>;

public:
    SFtp&                   Timeout(int ms)                                         { ssh->timeout = ms; return *this; }
    SFtp&                   ChunkSize(int sz)                                       { ssh->chunk_size = clamp(sz, 1, INT_MAX); return *this; }

    // Файл
    SFtpHandle              открой(const Ткст& path, dword flags, long mode);
    SFtpHandle              OpenRead(const Ткст& path)                            { return открой(path, READ, IRALL); }
    SFtpHandle              OpenWrite(const Ткст& path)                           { return открой(path, CREATE | WRITE, IRALL | IWUSR); }
    void                    открой(SFtpHandle handle);
    bool                    Rename(const Ткст& oldpath, const Ткст& newpath);
    bool                    Delete(const Ткст& path);
    bool                    синх(SFtpHandle handle);
    SFtp&                   перейди(SFtpHandle handle, int64 position);
    int64                   дайПоз(SFtpHandle handle);

    // читай/пиши
    int                     дай(SFtpHandle handle, void *ptr, int size = INT_MAX);
    int                     помести(SFtpHandle handle, const void *ptr, int size);
    bool                    сохраниФайл(const char *path, const Ткст& данные);
    Ткст                  загрузиФайл(const char *path);
    bool                    сохраниФайл(const char *path, Поток& in);
    bool                    загрузиФайл(Поток& out, const char *path);
    
    int                     GetDone() const                                         { return done; }
    
    // Directory
    SFtpHandle              OpenDir(const Ткст& path);
    bool                    MakeDir(const Ткст& path, long mode);
    bool                    RemoveDir(const Ткст& path);
    bool                    ListDir(SFtpHandle handle, DirList& list);
    bool                    ListDir(const Ткст& path, DirList& list);
    // Symlink
    bool                    MakeLink(const Ткст& orig, Ткст& target)            { return SymLink(orig, target, LIBSSH2_SFTP_SYMLINK); }
    bool                    ReadLink(const Ткст& path, Ткст& target)            { return SymLink(path, target, LIBSSH2_SFTP_READLINK); }
    bool                    RealizePath(const Ткст& path, Ткст& target)         { return SymLink(path, target, LIBSSH2_SFTP_REALPATH); }

    Ткст                  GetDefaultDir()                                         { Ткст s; return RealizePath(".", s) ? s : Ткст::дайПроц(); }
    
    // Attributes
    bool                    GetAttrs(SFtpHandle handle, SFtpAttrs& attrs);
    bool                    GetAttrs(const Ткст& path, SFtpAttrs& attrs);
    bool                    устАтры(SFtpHandle handle, const SFtpAttrs& attrs);
    bool                    устАтры(const Ткст& path, const SFtpAttrs& attrs);
    DirEntry                GetInfo(const Ткст& path);
    bool                    устИнфо(const DirEntry& entry)                          { return устАтры(entry.дайИмя(), ~entry); }
    int64                   дайРазм(const Ткст& path)                             { return QueryAttr(path, SFTP_ATTR_SIZE); }
    bool                    устРазм(const Ткст& path, int64 size)                 { return ModifyAttr(path, SFTP_ATTR_SIZE, size); }
    Время                    GetLastModifyTime(const Ткст& path)                   { return QueryAttr(path, SFTP_ATTR_LAST_MODIFIED); }
    bool                    SetLastModifyTime(const Ткст& path, const Время& time) { return ModifyAttr(path, SFTP_ATTR_LAST_MODIFIED, time); }
    Время                    дайВремяПоследнДоступа(const Ткст& path)                   { return QueryAttr(path, SFTP_ATTR_LAST_ACCESSED); }
    bool                    SetLastAccessTime(const Ткст& path, const Время& time) { return ModifyAttr(path, SFTP_ATTR_LAST_ACCESSED, time); }

    // Tests
    bool                    файлЕсть(const Ткст& path)                          { return QueryAttr(path, SFTP_ATTR_FILE); }
    bool                    дирЕсть(const Ткст& path)                     { return QueryAttr(path, SFTP_ATTR_DIRECTORY); }
    bool                    SymLinkExists(const Ткст& path)                       { return QueryAttr(path, SFTP_ATTR_SYMLINK); }
    bool                    SocketExists(const Ткст& path)                        { return QueryAttr(path, SFTP_ATTR_SOCKET); }
    bool                    PipeExists(const Ткст& path)                          { return QueryAttr(path, SFTP_ATTR_PIPE); }
    bool                    BlockExists(const Ткст& path)                         { return QueryAttr(path, SFTP_ATTR_BLOCK); }
    bool                    SpecialFileExists(const Ткст& path)                   { return QueryAttr(path, SFTP_ATTR_SPECIAL); }

    Врата<int64, int64>      WhenProgress;
    
    SFtp(SshSession& session);
    virtual ~SFtp();

    SFtp(SFtp&&) = default;

private:
    bool                    иниц() override;
    void                    выход() override;

    int                     FStat(SFtpHandle handle, SFtpAttrs& a, bool set);
    int                     LStat(const Ткст& path, SFtpAttrs& a, int тип);
    Значение                   QueryAttr(const Ткст& path, int attr);
    bool                    ModifyAttr(const Ткст& path, int attr, const Значение& v);
    bool                    SymLink(const Ткст& path, Ткст& target, int тип);
    int                     читай(SFtpHandle handle, void* ptr, int size);
    int                     пиши(SFtpHandle handle, const void* ptr, int size);
    bool                    CopyData(Поток& dest, Поток& ист, int64 maxsize = INT64_MAX);
  
    Один<LIBSSH2_SFTP*>      sftp_session;
    int                     done;

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
    virtual  void  устРазмПотока(int64 size);
    virtual  dword читай(int64 at, void *ptr, dword size);
    virtual  void  пиши(int64 at, const void *данные, dword size);

public:
    virtual  void  открой();
    virtual  bool  открыт() const;

protected:
    SFtp       *sftp;
    SFtpHandle  handle;

    void      устПоз(int64 pos);
    void      иниц(int64 size);

public:
    operator  bool() const                 { return открыт(); }

    bool       открой(SFtp& sftp, const char *имяф, dword mode, int acm = 0644);
    SFtpStream(SFtp& sftp, const char *имяф, dword mode, int acm = 0644);
    SFtpStream();
    ~SFtpStream();
    SFtpHandle дайУк() const            { return handle; }
};

class SFtpFileOut : public SFtpStream {
public:
    bool открой(SFtp& sftp, const char *фн, int acm = 0644) { return SFtpStream::открой(sftp, фн, CREATE|NOWRITESHARE, acm); }

    SFtpFileOut(SFtp& sftp, const char *фн)    { открой(sftp, фн); }
    SFtpFileOut()                              {}
};

class SFtpFileAppend : public SFtpStream {
public:
    bool открой(SFtp& sftp, const char *фн)      { return SFtpStream::открой(sftp, фн, APPEND|NOWRITESHARE); }

    SFtpFileAppend(SFtp& sftp, const char *фн) { открой(sftp, фн); }
    SFtpFileAppend()                           {}
};

class SFtpFileIn : public SFtpStream {
public:
    bool открой(SFtp& sftp, const char *фн)      { return SFtpStream::открой(sftp, фн, READ); }

    SFtpFileIn(SFtp& sftp, const char *фн)     { открой(sftp, фн); }
    SFtpFileIn()                               {}
};

class SFtpFileSystemInfo : public ИнфОФС {
public:
    SFtpFileSystemInfo& Mount(SFtp& sftp)       { browser = &sftp; return *this; }
    int дайСтиль() const override               { return STYLE_POSIX; }
    bool создайПапку(Ткст path, Ткст& Ошибка) const override;
    Массив<ИнфОФС::ИнфОФайле> найди(Ткст mask, int max_count = INT_MAX, bool unmounted = false) const override;
    
    SFtpFileSystemInfo()                        { browser = nullptr; }
    SFtpFileSystemInfo(SFtp& sftp)              { Mount(sftp); }
    virtual ~SFtpFileSystemInfo()               {}
    
private:
    SFtp *browser = nullptr;
};

