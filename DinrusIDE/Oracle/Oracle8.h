#ifndef ORACLE8_H
#define ORACLE8_H

#include <Sql/Sql.h>
#include "OraCommon.h"
#include "OciCommon.h"

namespace РНЦП {

#if defined(PLATFORM_WIN32) && defined(COMPILER_MSC)
#define __STDC__ 1
#endif

// selected definitions from OCI 8 headers
struct OCIEnv;
struct OCIServer;
struct OCISession;
struct OCISvcCtx;
struct OCIError;
struct OCILobLocator;

#define DLLFILENAME "oci.dll"
#define DLIMODULE   OCI8
#define DLIHEADER   <Oracle/Oci8.dli>
#include <Core/dli_header.h>

void OCI8SetDllPath(Ткст oci8_path, T_OCI8& oci8 = OCI8_());

//#define dword _dword
//#include <oci.h>
//#undef dword

class OCI8Connection;

class Oracle8 : public SqlSession {
public:
	virtual void                  старт();
	virtual void                  Commit();
	virtual void                  Rollback();

	virtual Ткст                Savepoint();
	virtual void                  RollbackTo(const Ткст& savepoint);

	virtual bool                  открыт() const;

	virtual RunScript             GetRunScript() const          { return &OraclePerformScript; }

	virtual Вектор<Ткст>        EnumUsers();
	virtual Вектор<Ткст>        EnumDatabases();
	virtual Вектор<Ткст>        EnumTables(Ткст database);
	virtual Вектор<Ткст>        EnumViews(Ткст database);
	virtual Вектор<Ткст>        EnumSequences(Ткст database);
	virtual Вектор<Ткст>        EnumPrimaryKey(Ткст database, Ткст table);
	virtual Ткст                EnumRowID(Ткст database, Ткст table);
	virtual Вектор<Ткст>        EnumReservedWords();

	bool                          BulkExecute(const char *stmt, const Вектор< Вектор<Значение> >& param_rows);
	bool                          BulkExecute(const SqlStatement& stmt, const Вектор< Вектор<Значение> >& param_rows);

public:
	T_OCI8&              oci8;

protected:
	virtual SqlConnection *CreateConnection();

private:
	bool                   AllocOciHandle(void *hp, int тип);
	void                   FreeOciHandle(void *hp, int тип);
	void                   SetOciError(Ткст text, OCIError *errhp);

private:
	friend class OCI8Connection;
	friend class OracleBlob;
	friend class OracleClob;

	OCIEnv              *envhp;
	OCIError            *errhp;
	OCIServer           *srvhp;
	OCISession          *seshp;
	OCISvcCtx           *svchp;
	Link<OCI8Connection> clink;
	bool                 in_server;
	bool                 in_session;
	bool                 utf8_session;
	bool                 disable_utf8_mode;
	Ткст               user;
	int                  level;
	int                  tmode;

	Ткст         Spn();
	void           PostError();
	bool           StdMode()   { return  tmode == NORMAL; }

public:
	bool Login(const char *имя, const char *password, const char *connect, bool use_objects, Ткст *warn = NULL);
	bool открой(const Ткст& connect_string, bool use_objects, Ткст *warn = NULL);
	void Logoff();

	enum TransactionMode {
		NORMAL,              // autocommit at level 0, no Commit or Rollback allowed at level 0
		ORACLE               // Oracle-style Commit and Rollback at level 0
	};

	void    SetTransactionMode(int mode)            { tmode = mode; }
	void    DisableUtf8Mode(bool dutf8 = true)      { disable_utf8_mode = dutf8; }
	
	bool    IsUtf8Session() const                   { return utf8_session; }
	
	Oracle8(T_OCI8& oci8 = OCI8_());
	~Oracle8();
};

class OracleBlob : public БлокПоток {
protected:
	virtual  void  SetStreamSize(int64 size);
	virtual  dword читай(int64 at, void *ptr, dword size);
	virtual  void  пиши(int64 at, const void *data, dword size);

public:
	virtual  bool  открыт() const;
	virtual  void  закрой();

protected:
	OCILobLocator   *locp;
	Oracle8         *session;

	void      иниц(dword size);

public:
	operator  bool() const                 { return открыт(); }

	void      присвой(const Sql& sql, int64 blob);
	void      присвой(Oracle8& session, int64 blob);

	OracleBlob();
	OracleBlob(const Sql& sql, int64 blob);
	OracleBlob(Oracle8& session, int64 blob);
	~OracleBlob();
};

class OracleClob : public БлокПоток {
protected:
	virtual  void  SetStreamSize(int64 size);
	virtual  dword читай(int64 at, void *ptr, dword size);
	virtual  void  пиши(int64 at, const void *data, dword size);

public:
	virtual  bool  открыт() const;
	virtual  void  закрой();

protected:
	OCILobLocator   *locp;
	Oracle8         *session;

	void      иниц(dword size);

public:
	operator  bool() const                 { return открыт(); }

	void      присвой(const Sql& sql, int64 blob);
	void      присвой(Oracle8& session, int64 blob);

	ШТкст   читай();
	void      пиши(const ШТкст& w);
	void      SetLength(int sz);

	OracleClob();
	OracleClob(const Sql& sql, int64 blob);
	OracleClob(Oracle8& session, int64 blob);
	~OracleClob();
};

#ifdef text
#undef text
#endif

}

#endif
