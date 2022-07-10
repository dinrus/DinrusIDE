#ifndef SQLCTRL_H
#define SQLCTRL_H

#include <Sql/Sql.h>
#include <CtrlLib/CtrlLib.h>


namespace РНЦП {

int  SqlError(const char *text, const char *Ошибка, const char *statement, bool retry = false);
int  SqlError(const char *text, const SqlSession& session, bool retry = false);
int  SqlError(const char *text, const Sql& sql APPSQLCURSOR, bool retry = false);

#ifdef PLATFORM_WIN32
int  SqlError(HWND parent, const char *text, const char *Ошибка, const char *statement, bool retry = false);
int  SqlError(HWND parent, const char *text, const SqlSession& session, bool retry = false);
int  SqlError(HWND parent, const char *text, const Sql& sql APPSQLCURSOR, bool retry = false);
#endif

bool   DisplayError(const SqlSession& session, const char *msg = NULL);
bool   ShowError(SqlSession& session, const char *msg = NULL);
bool   ErrorRollback(SqlSession& session, const char *emsg = NULL);
bool   OkCommit(SqlSession& session, const char *emsg = NULL);

bool   DisplayError(const Sql& sql, const char *msg = NULL);
bool   ShowError(Sql& sql, const char *msg = NULL);
bool   ErrorRollback(Sql& sql, const char *emsg = NULL);
bool   OkCommit(Sql& sql, const char *emsg = NULL);

#ifndef NOAPPSQL
bool   DisplayError(const char *msg = NULL);
bool   ShowError(const char *msg = NULL);
bool   ErrorRollback(const char *emsg = NULL);
bool   OkCommit(const char *emsg = NULL);
#endif

void SqlLoad(МапПреобр& cv, Sql& sql);
void SqlLoad(МапПреобр& cv, const SqlSelect& set, SqlSession& ss APPSQLSESSION);
#ifndef NOAPPSQL
void operator*=(МапПреобр& cv, const SqlSelect& set);
#endif

void SqlLoad(СписокБроса& dl, Sql& sql);
void SqlLoad(СписокБроса& dl, const SqlSelect& set, SqlSession& ss APPSQLSESSION);
#ifndef NOAPPSQL
void operator*=(СписокБроса& cv, const SqlSelect& set);
#endif

class SqlOption : public Опция {
public:
	virtual void   устДанные(const Значение& data);
	virtual Значение  дайДанные() const;
};

class SqlNOption : public SqlOption {
	virtual Значение  дайДанные() const;
};

class SqlArray : public КтрлМассив {
public:
	virtual void  устДанные(const Значение& v);
	virtual Значение дайДанные() const;
	virtual bool  прими();

protected:
	virtual bool  UpdateRow();
	virtual void  RejectRow();

private:
	SqlSession *ssn;
	SqlId       table;
	Значение       fkv;
	SqlId       fk;
	SqlBool     where;
	SqlSet      orderby;
	int         querytime;
	int         count;
	int64       offset;
	bool        lateinsert;
	bool        goendpostquery;
	bool        autoinsertid;
	bool        updatekey;

	SqlBool     GetWhere();
#ifndef NOAPPSQL
	SqlSession& Session()                                  { return ssn ? *ssn : SQL.GetSession(); }
#else
	SqlSession& Session()                                  { ПРОВЕРЬ(ssn); return *ssn; }
#endif

	bool        PerformInsert();
	bool        PerformDelete();

	void      Inserting();

public:
	Callback                           WhenPreQuery;
	Callback                           WhenPostQuery;
	Врата<const ВекторМап<Ид, Значение>&>  WhenFilter;

	void      StdBar(Бар& menu);
	bool      CanInsert() const;
	void      StartInsert();
	void      StartDuplicate();
	void      DoRemove();

	void      SetSession(SqlSession& _session)             { ssn = &_session; }

	void      Join(SqlId fk, КтрлМассив& master);
	void      Join(КтрлМассив& master);

	void      Query();
	void      AppendQuery(SqlBool where);
	void      Query(SqlBool where)                         { SetWhere(where); Query(); }
	void      ReQuery();
	void      ReQuery(SqlBool where)                       { SetWhere(where); ReQuery(); }

	void      Limit(int _offset, int _count)               { offset = _offset; count = _count; }
	void      Limit(int count)                             { Limit(0, count); }

	SqlArray& SetTable(SqlId _table)                       { table = _table; return *this; }
	SqlArray& SetTable(SqlId table, SqlId ключ)             { добавьКлюч(ключ); return SetTable(table); }
	SqlArray& SetWhere(SqlBool _where)                     { where = _where; return *this;  }
	SqlArray& SetOrderBy(SqlSet _orderby)                  { orderby = _orderby; return *this; }
	SqlArray& SetOrderBy(const SqlVal& a)                  { return SetOrderBy(SqlSet(a)); }
	SqlArray& SetOrderBy(const SqlVal& a, const SqlVal& b) { return SetOrderBy(SqlSet(a, b)); }
	SqlArray& SetOrderBy(const SqlVal& a, const SqlVal& b, const SqlVal& c)
	                                                       { return SetOrderBy(SqlSet(a, b, c)); }
	SqlArray& GoEndPostQuery(bool b = true)                { goendpostquery = b; return *this; }
	SqlArray& AutoInsertId(bool b = true)                  { autoinsertid = b; return *this; }
	SqlArray& AppendingAuto()                              { Appending(); return AutoInsertId(); }
	SqlArray& UpdateKey(bool b =  true)                    { updatekey = b; return *this; }

	void      очисть();
	void      переустанов();

	typedef   SqlArray ИМЯ_КЛАССА;

	SqlArray();
};

class SqlCtrls : public ИдКтрлы {
public:
	SqlCtrls& operator()(SqlId id, Ктрл& ctrl)       { добавь(id, ctrl); return *this; }
	void      Table(Ктрл& dlg, SqlId table);
	SqlCtrls& operator()(Ктрл& dlg, SqlId table)     { Table(dlg, table); return *this; }
	SqlSet    уст() const;
	operator  SqlSet() const                         { return уст(); }
	void      читай(Sql& sql);
	bool      Fetch(Sql& sql);
	bool      грузи(Sql& sql, SqlSelect select)       { sql * select; return Fetch(sql); }
	bool      грузи(Sql& sql, SqlId table, SqlBool where);
#ifndef NOAPPSQL
	bool      Fetch()                                { return Fetch(SQL); }
	bool      грузи(SqlSelect select)                 { return грузи(SQL, select); }
	bool      грузи(SqlId table, SqlBool where);
#endif
	void      вставь(SqlInsert& insert) const;
	void      Update(SqlUpdate& update) const;
	void      UpdateModified(SqlUpdate& update) const;
	SqlInsert вставь(SqlId table) const;
	SqlUpdate Update(SqlId table) const;
	SqlUpdate UpdateModified(SqlId table) const;

	SqlId       operator()(int i) const              { return элт[i].id; }
	SqlId       дайКлюч(int i) const                  { return элт[i].id; }

//deprecated:
	Callback  operator<<=(Callback cb);
};

class SqlDetail : public СтатичПрям {
public:
	virtual void  устДанные(const Значение& v);
	virtual Значение дайДанные() const;
	virtual bool  прими();

private:
	SqlSession *ssn;
	SqlCtrls    ctrls;
	SqlId       table;
	Значение       fkval;
	SqlId       fk;
	bool        present;
	bool        autocreate;

#ifndef NOAPPSQL
	SqlSession& Session()                          { return ssn ? *ssn : SQL.GetSession(); }
#else
	SqlSession& Session()                          { ПРОВЕРЬ(ssn); return *ssn; }
#endif

	void        Query();

public:
	Callback    WhenPostQuery;

	bool        IsPresent() const                  { return present; }
	bool        создай();
	bool        Delete();

	void        SetSession(SqlSession& _session)   { ssn = &_session; }

	SqlDetail&  добавь(SqlId id, Ктрл& ctrl);
	SqlDetail&  operator()(SqlId id, Ктрл& ctrl)   { return добавь(id, ctrl); }

	SqlDetail&  Join(SqlId fk, КтрлМассив& master);
	SqlDetail&  Join(КтрлМассив& master);
	SqlDetail&  SetTable(SqlId _table)             { table = _table; return *this; }
	SqlDetail&  AutoCreate(bool b = true)          { autocreate = b; return *this; }

	void        переустанов();

	SqlDetail();
};

void        SqlViewValue(const Ткст& title, const Ткст& значение);

void        SQLCommander(SqlSession& session);
#ifndef NOAPPSQL
inline void SQLCommander() { SQLCommander(SQL.GetSession()); }
#endif
void        SQLObjectTree(SqlSession& session APPSQLSESSION);

ИНИЦИАЛИЗУЙ(SqlCtrl)

}

#endif
