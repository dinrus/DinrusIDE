#ifndef ORACLE7_H
#define ORACLE7_H

#include <Sql/Sql.h>
#include "OraCommon.h"
#include "OciCommon.h"

namespace РНЦП {

#define DLLFILENAME "ociw32.dll"
#define DLIMODULE   OCI7
#define DLIHEADER   <Oracle/Oci7.dli>
#include <Core/dli_header.h>

class OCI7Connection;

void OCI7SetDllPath(Ткст oci7_path, T_OCI7& oci7 = OCI7_());

class Oracle7 : public SqlSession {
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

public:
	T_OCI7&                       oci7;

protected:
	virtual SqlConnection        *CreateConnection();

protected:
	friend class OCI7Connection;

	Link<OCI7Connection> clink;
	bool                 connected;
	byte                 lda[256];
	byte                 hda[512];
	Ткст               user;
	int                  level;
	bool                 autocommit;
	int                  tmode;

	Ткст         Spn();
	void           SetSp();
	Ткст         GetErrorMsg(int code) const;
	void           PreExec();
	void           PostExec();

public:
	bool открой(const Ткст& s);
	void закрой();

	enum TransactionMode {
		NORMAL,              // autocommit at level 0, no Commit or Rollback allowed at level 0
		ORACLE               // Oracle-style Commit and Rollback at level 0
	};

	void    SetTransactionMode(int mode)            { tmode = mode; }

	Oracle7(T_OCI7& oci7 = OCI7_());
	~Oracle7();
};

typedef Oracle7 OracleSession;

}

#endif
