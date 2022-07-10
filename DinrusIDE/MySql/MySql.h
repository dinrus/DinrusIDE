#ifndef __mysql_h__
#define __mysql_h__

// debian: sudo apt-get install libmysqlclient-dev

#ifndef flagNOMYSQL

#include <Sql/Sql.h>
#include <mysql.h>

#ifndef MYSQL_PORT // missing in mariadb client
#define MYSQL_PORT 3306
#endif

namespace РНЦП {

const char *MySqlReadString(const char *s, Ткст& stmt);

bool MySqlPerformScript(const Ткст& text, StatementExecutor& se, Врата<int, int> progress_canceled = Null);
#ifdef NOAPPSQL
bool MySqlUpdateSchema(const SqlSchema& sch, int i, StatementExecutor& se);
#else
bool MySqlUpdateSchema(const SqlSchema& sch, int i, StatementExecutor& se = SQLStatementExecutor());
#endif

Ткст MySqlTextType(int n);

class MySqlSession : public SqlSession {
public:
	virtual bool           открыт() const;
	virtual RunScript      GetRunScript() const             { return &MySqlPerformScript; }
	virtual Вектор<Ткст> EnumUsers();
	virtual Вектор<Ткст> EnumDatabases();
	virtual Вектор<Ткст> EnumTables(Ткст database);

protected:
	virtual SqlConnection *CreateConnection();

private:
	MYSQL *mysql;
	Ткст username;
	double lastid;
	int    level;

	Ткст connect_user;
	Ткст connect_password;
	Ткст connect_database;
	Ткст connect_host;
	int    connect_port;
	Ткст connect_socket;

	bool MysqlQuery(const char *query);
	bool DoConnect();
	
	friend class MySqlConnection;
	typedef MySqlSession ИМЯ_КЛАССА;

public:
	Врата<>               WhenReconnect;

	bool Connect(const char *user = NULL, const char *password = NULL, const char *database = NULL,
		         const char *host = NULL, int port = MYSQL_PORT, const char *socket = NULL);
	bool открой(const char *connect);
	bool Reconnect();
	void закрой();

	Ткст   GetUser()   { return username; }

	operator MYSQL *()   { return mysql; }

	virtual void   старт();
	virtual void   Commit();
	virtual void   Rollback();
	virtual int    GetTransactionLevel() const;
	
	void    AutoReconnect()   { WhenReconnect = THISBACK(Reconnect); }

	MySqlSession()       { mysql = NULL; Dialect(MY_SQL); }
	~MySqlSession()      { закрой(); }
};

}

#endif

#endif
