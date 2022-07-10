#include "Oracle8.h"
#include "OciCommon.h"

namespace РНЦП {

#define LLOG(x) RLOG(x)

//#define DLLFILENAME "ora803.dll"
#ifdef PLATFORM_WIN32
	#define DLLFILENAME "oci.dll"
#else
	#define DLLFILENAME "libclntsh.so"
#endif

#define DLIMODULE   OCI8
#define DLIHEADER   <Oracle/Oci8.dli>
#include <Core/dli_source.h>

void OCI8SetDllPath(Ткст oci8_path, T_OCI8& oci8)
{
	static Ткст dflt_name;
	if(пусто_ли(dflt_name))
		dflt_name = oci8.GetLibName();
	if(oci8_path != oci8.GetLibName())
		oci8.SetLibName(Nvl(oci8_path, dflt_name));
}

static Ткст OciError(T_OCI8& oci8, OCIError *errhp, int *code, bool utf8_session)
{
	if(code) *code = Null;
	if(!oci8) return t_("Ошибка running OCI8 Oracle connection dynamic library.");
	if(!errhp) return t_("Unknown Ошибка.");
	OraText errbuf[512];
	strcpy((char *)errbuf, t_("(unknown Ошибка)"));
	sb4 errcode;
	oci8.OCIErrorGet(errhp, 1, NULL, &errcode, errbuf, sizeof(errbuf), OCI_HTYPE_ERROR);
	if(code) *code = errcode;
	Ткст out = (const char *) errbuf;
	if(utf8_session)
		out = вНабсим(CHARSET_DEFAULT, out, CHARSET_UTF8);
	return out;
}

bool Oracle8::AllocOciHandle(void *hp, int тип) {
	LLOG("AllocOciHandle(тип " << тип << "), envhp = " << фмтЦелГекс(envhp));
	*(dvoid **)hp = NULL;
	return oci8 && !oci8.OCIHandleAlloc(envhp, (dvoid **)hp, тип, 0, NULL);
}

void Oracle8::FreeOciHandle(void *hp, int тип) {
	LLOG("FreeOciHandle(" << фмтЦелГекс(hp) << ", тип " << тип << ")");
	if(oci8 && hp) oci8.OCIHandleFree(hp, тип);
}

void Oracle8::SetOciError(Ткст text, OCIError *from_errhp)
{
	int errcode;
	Ткст msg = OciError(oci8, from_errhp, &errcode, utf8_session);
	устОш(msg, text, errcode, NULL, OciErrorClass(errcode));
}

class OCI8Connection : public Link<OCI8Connection>, public OciSqlConnection {
protected:
	virtual void        SetParam(int i, const Значение& r);
	virtual void        SetParam(int i, OracleRef r);
	virtual bool        выполни();
	virtual int         GetRowsProcessed() const;
	virtual bool        Fetch();
	virtual void        дайКолонку(int i, Реф f) const;
	virtual void        Cancel();
	virtual SqlSession& GetSession() const;
	virtual Ткст      GetUser() const;
	virtual Ткст      вТкст() const;
	virtual            ~OCI8Connection();

	bool                BulkExecute(const char *stmt, const Вектор< Вектор<Значение> >& param_rows);

	struct Элемент {
		T_OCI8&        oci8;
		int            тип;
		int            total_len;
		Буфер<ub2>    len;
		Буфер<sb2>    ind;
//		ub2            rl;
		ub2            rc;
		Вектор<Значение>  dynamic;
		bool           is_dynamic;
		bool           dyna_full;
		int            dyna_vtype;
		ub4            dyna_width;
		ub4            dyna_len;
		int            array_count;
		OCILobLocator *lob;
		OCIBind       *bind;
		OCIDefine     *define;
		OCI8Connection *refcursor;
		union {
			byte          *ptr;
			byte           буфер[8];
		};

		byte *Данные()                 { return total_len > (int)sizeof(буфер) ? ptr : буфер; }
		const byte *Данные() const     { return total_len > (int)sizeof(буфер) ? ptr : буфер; }
		bool  пусто_ли() const         { return ind[0] < 0; }
		bool  размести(int count, OCIEnv *envhp, int тип, int len, int res = 0);
		void  очисть();
		void  DynaFlush();

		static sb4 DynaIn(dvoid *ictxp, OCIBind *bindp,
			ub4 iter, ub4 Индекс, dvoid **bufpp, ub4 *alenp, ub1 *piecep, dvoid **indp) {
			Элемент *self = reinterpret_cast<Элемент *>(ictxp);
			ПРОВЕРЬ(self -> bind == bindp);
			*bufpp = NULL;
			*alenp = 0;
			*indp = NULL;
			*piecep = OCI_ONE_PIECE;
			return OCI_CONTINUE;
		}

		static sb4 DynaOut(dvoid *octxp, OCIBind *bindp,
			ub4 iter, ub4 Индекс, dvoid **bufpp, ub4 **alenp, ub1 *piecep, dvoid **indp, ub2 **rcodep) {
			Элемент *self = reinterpret_cast<Элемент *>(octxp);
			ПРОВЕРЬ(self -> bind == bindp);
			return self -> Out(iter, Индекс, bufpp, alenp, piecep, indp, rcodep);
		}

		sb4 Out(ub4 iter, ub4 Индекс, dvoid **bufpp, ub4 **alenp, ub1 *piecep, dvoid **indp, ub2 **rcodep);

		Элемент(T_OCI8& oci8);
		~Элемент();
	};

	Oracle8           *session;
	T_OCI8&            oci8;
	OCIStmt           *stmthp;
	OCIError          *errhp;
	Массив<Элемент>        param;
	Массив<Элемент>        column;
	Ткст             parsed_cmd;
	Вектор<int>        dynamic_param;
	int                dynamic_pos;
	int                dynamic_rows;
	int                fetched;
	int                fetchtime;
	bool               refcursor;

	OCISvcCtx *SvcCtx() const        { ПРОВЕРЬ(session); return session->svchp; }

	Элемент&     PrepareParam(int i, int тип, int len, int reserve, int dynamic_vtype);
	void      SetParam(int i, const Ткст& s);
	void      SetParam(int i, const ШТкст& s);
	void      SetParam(int i, int integer);
	void      SetParam(int i, int64 integer);
	void      SetParam(int i, double d);
	void      SetParam(int i, Дата d);
	void      SetParam(int i, Время d);
	void      SetParam(int i, Sql& refcursor);
	void      SetRawParam(int i, const Ткст& s);

	void      добавьКолонку(int тип, int len);
	void      дайКолонку(int i, Ткст& s) const;
	void      дайКолонку(int i, ШТкст& s) const;
	void      дайКолонку(int i, int& n) const;
	void      дайКолонку(int i, int64& n) const;
	void      дайКолонку(int i, double& d) const;
	void      дайКолонку(int i, Дата& d) const;
	void      дайКолонку(int i, Время& t) const;

	void      устОш();
	bool      GetColumnInfo();

	void      очисть();

	OCI8Connection(Oracle8& s);

	friend class Oracle8;
};

void OCI8Connection::Элемент::очисть() {
	ind.очисть();
	len.очисть();
	if(тип == SQLT_BLOB || тип == SQLT_CLOB)
		oci8.OCIDescriptorFree((dvoid *)lob, OCI_DTYPE_LOB);
	else
	if(total_len > (int)sizeof(буфер))
		delete[] ptr;
	total_len = 0;
	lob = NULL;
	dynamic.очисть();
	dyna_full = false;
	dyna_vtype = VOID_V;
	dyna_width = 0;
}

OCI8Connection::Элемент::Элемент(T_OCI8& oci8_)
: oci8(oci8_)
{
	lob = NULL;
	bind = NULL;
	define = NULL;
	total_len = 0;
	refcursor = 0;
	dyna_full = false;
	is_dynamic = false;
}

OCI8Connection::Элемент::~Элемент() {
	очисть();
}

bool OCI8Connection::Элемент::размести(int _count, OCIEnv *envhp, int _type, int _len, int res) {
	if(_type == тип && total_len >= _len && array_count >= _count) return false;
	очисть();
	тип = _type;
	total_len = _len + res;
	array_count = _count;
	if(тип == SQLT_BLOB || тип == SQLT_CLOB)
		oci8.OCIDescriptorAlloc(envhp, (dvoid **) &lob, OCI_DTYPE_LOB, 0, NULL);
	if(total_len > (int)sizeof(буфер))
		ptr = new byte[total_len];
	ind.размести(_count);
	ind[0] = -1;
	len.размести(_count);
	len[0] = _len;
	return true;
}

void OCI8Connection::Элемент::DynaFlush() {
	if(dyna_full) {
		dyna_full = false;
		Значение v;
		if(ind[0] == 0) {
			if(dyna_len > dyna_width)
				dyna_width = dyna_len;
			const byte *p = Данные();
			switch(тип) {
			case SQLT_INT:
				if(dyna_len == sizeof(int))
					v = *(const int *)p;
				else if(dyna_len == sizeof(int64))
					v = *(const int64 *)p;
				break;

			case SQLT_FLT:
				v = *(const double *)p;
				break;

			case SQLT_STR:
				v = Ткст((const char *)p, dyna_len);
				break;

			case SQLT_DAT:
				v = OciDecodeTime(p);
				break;

			case SQLT_CLOB:
			case SQLT_BLOB:
			default:
				NEVER();
				break;
			}
		}
		dynamic.добавь(v);
	}
}

sb4 OCI8Connection::Элемент::Out(ub4 iter, ub4 Индекс, dvoid **bufpp, ub4 **alenp, ub1 *piecep, dvoid **indp, ub2 **rcodep) {
	DynaFlush();
	*bufpp = Данные();
	*alenp = &dyna_len;
	dyna_len = total_len;
	*piecep = OCI_ONE_PIECE;
	ind.размести(1);
	ind[0] = 0;
	*indp = ind;
	rc = 0;
	*rcodep = &rc;
	dyna_full = true;
	return OCI_CONTINUE;
}

OCI8Connection::Элемент& OCI8Connection::PrepareParam(int i, int тип, int len, int res, int dynamic_vtype) {
	while(param.дайСчёт() <= i)
		param.добавь(new Элемент(oci8));
	Элемент& p = param[i];
	if(p.размести(1, session -> envhp, тип, len, res))
		parse = true;
	p.dyna_vtype = dynamic_vtype;
	p.is_dynamic = (dynamic_vtype != VOID_V);
	return p;
}

void OCI8Connection::SetParam(int i, const Ткст& s) {
	Ткст rs = (session->utf8_session ? вНабсим(CHARSET_UTF8, s) : s);
	int l = rs.дайДлину();
	Элемент& p = PrepareParam(i, SQLT_STR, l + 1, 100, VOID_V);
	memcpy(p.Данные(), rs, l + 1);
	p.ind[0] = l ? 0 : -1;
}

void OCI8Connection::SetParam(int i, const ШТкст& s) {
	Ткст rs = (session->utf8_session ? вУтф8(s) : s.вТкст());
	int l = rs.дайДлину();
	Элемент& p = PrepareParam(i, SQLT_STR, l + 1, 100, VOID_V);
	memcpy(p.Данные(), rs, l + 1);
	p.ind[0] = l ? 0 : -1;
}

void OCI8Connection::SetParam(int i, int integer) {
	Элемент& p = PrepareParam(i, SQLT_INT, sizeof(int), 0, VOID_V);
	*(int *) p.Данные() = integer;
	p.ind[0] = пусто_ли(integer) ? -1 : 0;
}

void OCI8Connection::SetParam(int i, int64 integer) {
	Элемент& p = PrepareParam(i, SQLT_INT, sizeof(int64), 0, VOID_V);
	*(int64 *) p.Данные() = integer;
	p.ind[0] = пусто_ли(integer) ? -1 : 0;
}

void OCI8Connection::SetParam(int i, double d) {
	Элемент& p = PrepareParam(i, SQLT_FLT, sizeof(double), 0, VOID_V);
	*(double *) p.Данные() = d;
	p.ind[0] = пусто_ли(d) ? -1 : 0;
}

void OCI8Connection::SetParam(int i, Дата d) {
	Элемент& w = PrepareParam(i, SQLT_DAT, 7, 0, VOID_V);
	w.ind[0] = (OciEncodeDate(w.Данные(), d) ? 0 : -1);
}

void OCI8Connection::SetParam(int i, Время t) {
	Элемент& w = PrepareParam(i, SQLT_DAT, 7, 0, VOID_V);
	w.ind[0] = (OciEncodeTime(w.Данные(), t) ? 0 : -1);
}

void OCI8Connection::SetParam(int i, OracleRef r) {
	PrepareParam(i, r.GetOraType(), r.GetMaxLen(), 0, r.дайТип());
}

void OCI8Connection::SetRawParam(int i, const Ткст& s) {
	int l = s.дайДлину();
	Элемент& p = PrepareParam(i, SQLT_LBI, l, 0, VOID_V);
	memcpy(p.Данные(), s, l);
	p.ind[0] = l ? 0 : -1;
}

/*
class Oracle8RefCursorStub : public SqlSource {
public:
	Oracle8RefCursorStub(SqlConnection *cn) : cn(cn) {}
	virtual SqlConnection *CreateConnection() { return cn; }

private:
	SqlConnection *cn;
};
*/

void OCI8Connection::SetParam(int i, Sql& rc) {
	Элемент& w = PrepareParam(i, SQLT_RSET, -1, 0, VOID_V);
	w.refcursor = new OCI8Connection(*session);
	w.refcursor -> refcursor = true;
	*(OCIStmt **)w.Данные() = w.refcursor -> stmthp;
	w.ind[0] = 0;
//	Oracle8RefCursorStub stub(w.refcursor);
	прикрепи(rc, w.refcursor);
//	rc = Sql(stub);
}

void  OCI8Connection::SetParam(int i, const Значение& q) {
	if(q.пусто_ли())
		SetParam(i, Ткст());
	else
		switch(q.дайТип()) {
			case SQLRAW_V:
				SetRawParam(i, SqlRaw(q));
				break;
			case STRING_V:
				SetParam(i, Ткст(q));
				break;
			case WSTRING_V:
				SetParam(i, ШТкст(q));
				break;
			case BOOL_V:
			case INT_V:
				SetParam(i, int(q));
				break;
			case INT64_V:
			case DOUBLE_V:
				SetParam(i, double(q));
				break;
			case DATE_V:
				SetParam(i, Дата(q));
				break;
			case TIME_V:
				SetParam(i, (Время)q);
				break;
			default:
				if(IsTypeRaw<Sql *>(q)) {
					SetParam(i, *ValueTo<Sql *>(q));
					break;
				}
				if(OracleRef::IsValue(q)) {
					SetParam(i, OracleRef(q));
					break;
				}
				NEVER();
		}
}

void OCI8Connection::добавьКолонку(int тип, int len) {
	column.добавь(new Элемент(oci8)).размести(1, session -> envhp, тип, len);
}

bool OCI8Connection::BulkExecute(const char *stmt, const Вектор< Вектор<Значение> >& param_rows)
{
	ПРОВЕРЬ(session);

	int time = msecs();
	int args = 0;
	Ткст cvt_stmt = вНабсим(session->utf8_session
		? CHARSET_UTF8 : CHARSET_DEFAULT, stmt, CHARSET_DEFAULT);
	if((args = OciParse(cvt_stmt, parsed_cmd, this, session)) < 0)
		return false;

	session->statement = parsed_cmd;
	int nrows = param_rows.дайСчёт();

	if(Поток *s = session->GetTrace()) {
		*s << "BulkExecute(#" << nrows << " rows)\n";
		for(int r = 0; r < nrows; r++) {
			const Вектор<Значение>& row = param_rows[r];
			*s << "[row #" << r << "] ";
			bool quotes = false;
			int argn = 0;
			for(const char *q = cvt_stmt; *q; q++) {
				if(*q== '\'' && q[1] != '\'')
					quotes = !quotes;
				if(!quotes && *q == '?') {
					if(argn < row.дайСчёт())
						*s << SqlCompile(ORACLE, SqlFormat(row[argn++]));
					else
						*s << t_("<not supplied>");
				}
				else
					*s << *q;
			}
			*s << "\n";
		}
		*s << "//BulkExecute\n";
	}

	if(oci8.OCIStmtPrepare(stmthp, errhp, (byte *)~parsed_cmd, parsed_cmd.дайДлину(), OCI_NTV_SYNTAX,
		OCI_DEFAULT)) {
		устОш();
		return false;
	}

	for(int a = 0; a < args; a++) {
		int max_row_len = 1;
		int sql_type = 0;
		for(int r = 0; r < nrows; r++) {
			Значение v = (a < param_rows[r].дайСчёт() ? param_rows[r][a] : Значение());
			int len = 0;
			int vt = v.дайТип();
			if(!пусто_ли(v)) {
				if(номТипЗначЧисло_ли(vt)) {
					if((vt == INT_V || vt == INT64_V || vt == BOOL_V)
					&& (!sql_type || sql_type == SQLT_INT)) {
						sql_type = SQLT_INT;
						len = (vt == INT64_V ? sizeof(int64) : sizeof(int));
					}
					else if(!sql_type || sql_type == SQLT_INT || sql_type == SQLT_FLT) {
						sql_type = SQLT_FLT;
						len = sizeof(double);
					}
					else {
						RLOG("invalid тип combination in BulkExecute: " << sql_type << " <- number");
					}
				}
				else if(датаВремя_ли(v)) {
					if(!sql_type || sql_type == SQLT_DAT) {
						sql_type = SQLT_DAT;
						len = 7;
					}
					else {
						RLOG("invalid тип combination in BulkExecute: " << sql_type << " <- date/time");
					}
				}
				else if(ткст_ли(v)) {
					if(!sql_type || sql_type == SQLT_STR) {
						sql_type = SQLT_STR;
						if(session->utf8_session) {
							ШТкст wstr(v);
							len = 1 + длинаУтф8(wstr, wstr.дайДлину());
						}
						else
							len = 1 + Ткст(v).дайДлину();
					}
					else {
						RLOG("invalid тип combination in BulkExecute: " << sql_type << " <- string");
					}
				}
				else {
					RLOG("invalid data тип: " << v.дайТип());
				}
			}
			if(len > max_row_len)
				max_row_len = len;
		}

		if(sql_type == 0)
			sql_type = SQLT_STR;
		int sum_len = nrows * max_row_len;
		Элемент& p = param.добавь(new Элемент(oci8));
		p.размести(nrows, session->envhp, sql_type, sum_len, 0);
		p.dyna_vtype = VOID_V;
		p.is_dynamic = false;
		sb2 *indp = p.ind;
		ub2 *lenp = p.len;

		switch(sql_type) {
			case SQLT_INT: {
				ПРОВЕРЬ(sum_len >= nrows * (int)sizeof(max_row_len));
				if(max_row_len == sizeof(int)) {
					int *datp = (int *)p.Данные();
					for(int r = 0; r < nrows; r++) {
						int i = (param_rows[r].дайСчёт() > a ? (int)param_rows[r][a] : (int)Null);
						*datp++ = i;
						*indp++ = пусто_ли(i) ? -1 : 0;
						*lenp++ = sizeof(int);
					}
				}
				else if(max_row_len == sizeof(int64)) {
					int64 *datp = (int64 *)p.Данные();
					for(int r = 0; r < nrows; r++) {
						int64 i = (param_rows[r].дайСчёт() > a ? (int64)param_rows[r][a] : (int64)Null);
						*datp++ = i;
						*indp++ = пусто_ли(i) ? -1 : 0;
						*lenp++ = sizeof(int64);
					}
				}
				break;
			}
			case SQLT_FLT: {
				ПРОВЕРЬ(sum_len >= nrows * (int)sizeof(double));
				double *datp = (double *)p.Данные();
				for(int r = 0; r < nrows; r++) {
					double d = (param_rows[r].дайСчёт() > a ? (double)param_rows[r][a] : (double)Null);
					*datp++ = d;
					*indp++ = пусто_ли(d) ? -1 : 0;
					*lenp++ = sizeof(double);
				}
				break;
			}
			case SQLT_DAT: {
				ПРОВЕРЬ(sum_len >= nrows * 7);
				byte *datp = p.Данные();
				for(int r = 0; r < nrows; r++) {
					Время d = (param_rows[r].дайСчёт() > a ? (Время)param_rows[r][a] : (Время)Null);
					*indp++ = OciEncodeTime(datp, d) ? 0 : -1;
					datp += 7;
					*lenp++ = 7;
				}
				break;
			}
			case SQLT_STR: {
				byte *datp = p.Данные();
				for(int r = 0; r < nrows; r++) {
					Ткст s;
					if(session->utf8_session)
						s = вУтф8(param_rows[r].дайСчёт() > a ? (ШТкст)param_rows[r][a] : ШТкст());
					else
						s = (param_rows[r].дайСчёт() > a ? (Ткст)param_rows[r][a] : Ткст());
					*indp++ = (пусто_ли(s) ? -1 : 0);
					int rawlen = s.дайДлину() + 1;
					*lenp++ = rawlen;
					memcpy(datp, s, rawlen);
					datp += max_row_len;
				}
				ПРОВЕРЬ((int)(datp - (byte *)p.Данные()) <= sum_len);
				break;
			}
			default: {
				RLOG("unsupported SQL тип: " << sql_type);
			}
		}

		if(oci8.OCIBindByPos(stmthp, &p.bind, errhp, a + 1, p.Данные(), max_row_len, p.тип,
			p.ind, p.len, NULL, nrows, NULL, OCI_DEFAULT)) {
			устОш();
			return false;
		}
	}

	if(oci8.OCIStmtExecute(SvcCtx(), stmthp, errhp, nrows, 0, NULL, NULL,
		session->StdMode() && session->level == 0 ? OCI_COMMIT_ON_SUCCESS : OCI_DEFAULT)) {
		устОш();
		session->PostError();
		return false;
	}

	if(Поток *s = session->GetTrace()) {
		if(session->IsTraceTime())
			*s << фмт("----- exec %d ms\n", msecs(time));
	}
	return true;
}

bool Oracle8::BulkExecute(const SqlStatement& stmt, const Вектор< Вектор<Значение> >& param_rows)
{
	if(param_rows.пустой())
		return true;
	return BulkExecute(stmt.дай(ORACLE), param_rows);
}

bool OCI8Connection::выполни() {
	ПРОВЕРЬ(session);
	int time = msecs();
	int args = 0;
	if(parse) {
//		Cancel();
		Ткст cvt_stmt = вНабсим(session->utf8_session
			? CHARSET_UTF8 : CHARSET_DEFAULT, statement, CHARSET_DEFAULT);
		if((args = OciParse(cvt_stmt, parsed_cmd, this, session)) < 0)
			return false;
		ub4 fr = fetchrows;

		oci8.OCIAttrSet(stmthp, OCI_HTYPE_STMT, &fr, 0, OCI_ATTR_PREFETCH_ROWS, errhp);
		if(oci8.OCIStmtPrepare(stmthp, errhp, (byte *)~parsed_cmd, parsed_cmd.дайДлину(), OCI_NTV_SYNTAX,
			              OCI_DEFAULT)) {
			устОш();
			return false;
		}

		while(param.дайСчёт() < args)
			SetParam(param.дайСчёт(), Ткст());
		param.обрежь(args);
		dynamic_param.очисть();
		for(int i = 0; i < args; i++) {
			Элемент& p = param[i];
			if(oci8.OCIBindByPos(stmthp, &p.bind, errhp, i + 1, p.Данные(), p.total_len, p.тип,
			p.ind, NULL, NULL, 0, NULL, p.is_dynamic ? OCI_DATA_AT_EXEC : OCI_DEFAULT)) {
				устОш();
				return false;
			}
			if(p.is_dynamic) {
				dynamic_param.добавь(i);
				if(oci8.OCIBindDynamic(p.bind, errhp, &p, &Элемент::DynaIn, &p, &Элемент::DynaOut)) {
					устОш();
					return false;
				}
			}
		}
	}
	ub2 тип;
	if(oci8.OCIAttrGet(stmthp, OCI_HTYPE_STMT, &тип, NULL, OCI_ATTR_STMT_TYPE, errhp)) {
		устОш();
		return false;
	}

	if(oci8.OCIStmtExecute(SvcCtx(), stmthp, errhp, тип != OCI_STMT_SELECT, 0, NULL, NULL,
		              session->StdMode() && session->level == 0 ? OCI_COMMIT_ON_SUCCESS : OCI_DEFAULT)) {
		устОш();
		session->PostError();
		return false;
	}

	if(!dynamic_param.пустой()) {
		dynamic_pos = -1;
		for(int i = 0; i < dynamic_param.дайСчёт(); i++)
			param[dynamic_param[i]].DynaFlush();
		dynamic_rows = param[dynamic_param[0]].dynamic.дайСчёт();
	}

	if(parse) {
		if(!GetColumnInfo())
			return false;
		for(int i = 0; i < param.дайСчёт(); i++)
			if(param[i].refcursor && !param[i].refcursor -> GetColumnInfo())
				return false;
	}
	fetched = 0;
	fetchtime = 0;
	if(Поток *s = session->GetTrace()) {
		*s << вТкст() << '\n';
		if(session->IsTraceTime())
			*s << фмт("----- exec %d ms\n", msecs(time));
	}
	return true;
}

bool OCI8Connection::GetColumnInfo() {
	info.очисть();
	column.очисть();
	ub4 argcount;
	if(oci8.OCIAttrGet(stmthp, OCI_HTYPE_STMT, &argcount, 0, OCI_ATTR_PARAM_COUNT, errhp) != OCI_SUCCESS) {
		устОш();
		return false;
	}
	if(!dynamic_param.пустой()) {
		for(int i = 0; i < dynamic_param.дайСчёт(); i++) {
			const Элемент& p = param[dynamic_param[i]];
			SqlColumnInfo& ci = info.добавь();
			ci.имя = фмт("#%d", i + 1);
			ci.тип = p.dyna_vtype;
			ci.width = p.dyna_width;
			ci.precision = Null;
			ci.scale = Null;
			ci.binary = (p.тип == SQLT_LBI || p.тип == SQLT_BLOB);
		}
		parse = false;
		return true;
	}
	for(ub4 i = 1; i <= argcount; i++) {
		OCIParam *pd;
		if(oci8.OCIParamGet(stmthp, OCI_HTYPE_STMT, errhp, (dvoid **)&pd, i) != OCI_SUCCESS) {
			устОш();
			return false;
		}
		char *имя;
		ub4 name_len;
		ub2 тип, width;
		ub2 prec;
		sb1 scale;
		oci8.OCIAttrGet(pd, OCI_DTYPE_PARAM, &тип, NULL,  OCI_ATTR_DATA_TYPE, errhp);
		oci8.OCIAttrGet(pd, OCI_DTYPE_PARAM, &width, NULL, OCI_ATTR_DATA_SIZE, errhp);
		oci8.OCIAttrGet(pd, OCI_DTYPE_PARAM, &имя, &name_len, OCI_ATTR_NAME, errhp);
		oci8.OCIAttrGet(pd, OCI_DTYPE_PARAM, &prec, NULL,  OCI_ATTR_PRECISION, errhp);
		oci8.OCIAttrGet(pd, OCI_DTYPE_PARAM, &scale, NULL,  OCI_ATTR_SCALE, errhp);
		SqlColumnInfo& ii = info.добавь();
		ii.width = width;
		ii.precision = prec;
		ii.scale = scale;
		ii.имя = взаг(обрежьПраво(Ткст(имя, name_len)));
		ii.binary = false;
		bool blob = false;
		switch(тип) {
		case SQLT_NUM:
			ii.тип = DOUBLE_V;
			добавьКолонку(SQLT_FLT, sizeof(double));
			break;
		case SQLT_DAT:
			ii.тип = TIME_V;
			добавьКолонку(SQLT_DAT, 7);
			break;
		case SQLT_BLOB:
			ii.тип = ORA_BLOB_V;
			добавьКолонку(SQLT_BLOB, sizeof(OCILobLocator *));
			blob = true;
			ii.binary = true;
			break;
		case SQLT_CLOB:
			ii.тип = ORA_CLOB_V;
			добавьКолонку(SQLT_CLOB, sizeof(OCILobLocator *));
			blob = true;
			break;
		case SQLT_RDD:
			ii.тип = STRING_V;
			добавьКолонку(SQLT_STR, 64);
			break;
		case SQLT_TIMESTAMP: // тип 187
			ii.тип = STRING_V;
			добавьКолонку(SQLT_STR, 30); 
			break;
		default:
			ii.тип = STRING_V;
			добавьКолонку(SQLT_STR, ii.width ? ii.width + 1 : longsize);
			break;
		}
		Элемент& c = column.верх();
		oci8.OCIDefineByPos(stmthp, &c.define, errhp, i,
			blob ? (void *)&c.lob : (void *)c.Данные(), blob ? -1 : c.total_len,
			c.тип, c.ind, c.len, NULL, OCI_DEFAULT);
	}
	parse = false;
	return true;
}

int OCI8Connection::GetRowsProcessed() const {
	if(!dynamic_param.пустой())
		return dynamic_pos + 1;
	ub4 rp = 0;
	oci8.OCIAttrGet(stmthp, OCI_HTYPE_STMT, &rp, NULL, OCI_ATTR_ROW_COUNT, errhp);
	return rp;
}

bool OCI8Connection::Fetch() {
	ПРОВЕРЬ(!parse);
	if(parse) return false;
	if(!dynamic_param.пустой()) // dynamic pseudo-fetch
		return (dynamic_pos < dynamic_rows && ++dynamic_pos < dynamic_rows);
	int fstart = msecs();
	sword status = oci8.OCIStmtFetch(stmthp, errhp, 1, OCI_FETCH_NEXT, OCI_DEFAULT);
	bool ok = false;
	if(status == OCI_SUCCESS || status == OCI_SUCCESS_WITH_INFO) {
		fetchtime += msecs(fstart);
		++fetched;
		ok = true;
	}
	else if(status != OCI_NO_DATA)
		устОш();
	if(Поток *s = session->GetTrace()) {
		if(!ok || fetched % 100 == 0 && fetchtime)
			*s << фмтЧ("----- fetch(%d) in %d ms, %8n ms/rec, %2n rec/s\n",
				fetched, fetchtime,
				fetchtime / max<double>(fetched, 1),
				fetched * 1000.0 / max<double>(fetchtime, 1));
	}
	return ok;
}

void OCI8Connection::дайКолонку(int i, Ткст& s) const {
	if(!dynamic_param.пустой()) {
		s = param[dynamic_param[i]].dynamic[dynamic_pos];
		return;
	}
	const Элемент& c = column[i];
	if(c.тип == SQLT_BLOB || c.тип == SQLT_CLOB) {
		if (sizeof(int) < sizeof(uintptr_t)){ // CPU_64
			int64 handle;
			дайКолонку(i, handle);
			if(!пусто_ли(handle)) {
				if(c.тип == SQLT_CLOB && session->utf8_session) {
					OracleClob clob(*session, handle);
					s = изЮникода(clob.читай());
				}
				else {
					OracleBlob blob(*session, handle);
					s = загрузиПоток(blob);
				}
			}
			else
				s = Null;
			return;
		}
		int handle;
		дайКолонку(i, handle);
		if(!пусто_ли(handle)) {
			if(c.тип == SQLT_CLOB && session->utf8_session) {
				OracleClob clob(*session, handle);
				s = изЮникода(clob.читай());
			}
			else {
				OracleBlob blob(*session, handle);
				s = загрузиПоток(blob);
			}
		}
		else
			s = Null;
		return;
	}
	if(c.ind[0] < 0)
		s = Null;
	else {
		ПРОВЕРЬ(c.тип == SQLT_STR);
		s = (char *) c.Данные();
		if(session->utf8_session)
			s = вНабсим(CHARSET_DEFAULT, s, CHARSET_UTF8);
	}
}

void OCI8Connection::дайКолонку(int i, ШТкст& ws) const {
	if(!dynamic_param.пустой()) {
		ws = param[dynamic_param[i]].dynamic[dynamic_pos];
		return;
	}
	const Элемент& c = column[i];
	if(c.тип == SQLT_BLOB || c.тип == SQLT_CLOB) {
		if (sizeof(int) < sizeof(uintptr_t)){ // CPU_64
			int64 handle;
			дайКолонку(i, handle);
			if(!пусто_ли(handle)) {
				if(session->utf8_session && c.тип == SQLT_CLOB) {
					OracleClob clob(*session, handle);
					ws = clob.читай();
				}
				else {
					OracleBlob blob(*session, handle);
					Ткст s = загрузиПоток(blob);
					ws = s.вШТкст();
				}
			}
			else
				ws = Null;
			return;
		}
		int handle;
		дайКолонку(i, handle);
		if(!пусто_ли(handle)) {
			if(session->utf8_session && c.тип == SQLT_CLOB) {
				OracleClob clob(*session, handle);
				ws = clob.читай();
			}
			else {
				OracleBlob blob(*session, handle);
				Ткст s = загрузиПоток(blob);
				ws = s.вШТкст();
			}
		}
		else
			ws = Null;
		return;
	}
	Ткст s;
	if(c.ind[0] < 0)
		s = Null;
	else {
		ПРОВЕРЬ(c.тип == SQLT_STR);
		s = (char *) c.Данные();
	}
	if(session->utf8_session)
		ws = вУтф32(s);
	else
		ws = s.вШТкст();
}

void OCI8Connection::дайКолонку(int i, double& n) const {
	if(!dynamic_param.пустой()) {
		n = param[dynamic_param[i]].dynamic[dynamic_pos];
		return;
	}
	const Элемент& c = column[i];
	if(c.ind[0] < 0)
		n = DOUBLE_NULL;
	else {
		ПРОВЕРЬ(c.тип == SQLT_FLT || c.тип == SQLT_BLOB || c.тип == SQLT_CLOB);
		n = c.тип == SQLT_BLOB || c.тип == SQLT_CLOB ? (int)(uintptr_t)c.lob : *(double *) c.Данные();
	}
}

void OCI8Connection::дайКолонку(int i, int& n) const {
	if(!dynamic_param.пустой()) {
		n = param[dynamic_param[i]].dynamic[dynamic_pos];
		return;
	}
	const Элемент& c = column[i];
	if(c.ind[0] < 0)
		n = Null;
	else if(c.тип == SQLT_BLOB || c.тип == SQLT_CLOB) {
		ПРОВЕРЬ(sizeof(int) >= sizeof(uintptr_t)); // won't work in 64-bit mode
		n = (int)(uintptr_t)c.lob;
	}
	else if(c.тип == SQLT_INT) {
		if(c.len[0] == sizeof(int))
			n = *(int *)c.Данные();
		else if(c.len[0] == sizeof(int64))
			n = (int)*(int64 *)c.Данные();
		else {
			NEVER();
			n = Null;
		}
	}
	else if(c.тип == SQLT_FLT) {
		n = (int)*(double *)c.Данные();
	}
	else {
		NEVER();
		n = Null;
	}
}

void OCI8Connection::дайКолонку(int i, int64& n) const {
	if(!dynamic_param.пустой()) {
		n = param[dynamic_param[i]].dynamic[dynamic_pos];
		return;
	}
	const Элемент& c = column[i];
	if(c.ind[0] < 0)
		n = Null;
	else if(c.тип == SQLT_BLOB || c.тип == SQLT_CLOB)
		n = (int64)(uintptr_t)c.lob;
	else if(c.тип == SQLT_INT) {
		if(c.len[0] == sizeof(int))
			n = *(int *)c.Данные();
		else if(c.len[0] == sizeof(int64))
			n = *(int64 *)c.Данные();
		else {
			NEVER();
			n = Null;
		}
	}
	else if(c.тип == SQLT_FLT) {
		n = (int64)*(double *)c.Данные();
	}
	else {
		NEVER();
		n = Null;
	}
}

void OCI8Connection::дайКолонку(int i, Дата& d) const {
	if(!dynamic_param.пустой()) {
		d = param[dynamic_param[i]].dynamic[dynamic_pos];
		return;
	}
	const Элемент& c = column[i];
	if(c.ind[0] < 0)
		d = Null; // d.year = d.month = d.day = 0;
	else {
		ПРОВЕРЬ(c.тип == SQLT_DAT);
		const byte *data = c.Данные();
		d = OciDecodeDate(data);
	}
}

void OCI8Connection::дайКолонку(int i, Время& t) const {
	if(!dynamic_param.пустой()) {
		t = param[dynamic_param[i]].dynamic[dynamic_pos];
		return;
	}
	const Элемент& c = column[i];
	if(c.ind[0] < 0)
		t = Null; // t.year = t.month = t.day = 0;
	else {
		ПРОВЕРЬ(c.тип == SQLT_DAT);
		const byte *data = c.Данные();
		t = OciDecodeTime(data);
	}
}

void  OCI8Connection::дайКолонку(int i, Реф f) const {
	if(!dynamic_param.пустой()) {
		f.устЗначение(param[dynamic_param[i]].dynamic[dynamic_pos]);
		return;
	}
	switch(f.дайТип()) {
		case WSTRING_V: {
			ШТкст ws;
			дайКолонку(i, ws);
			f.устЗначение(ws);
			break;
		}
		case STRING_V: {
			Ткст s;
			дайКолонку(i, s);
			f.устЗначение(s);
			break;
		}
		case BOOL_V:
		case INT_V: {
			int d;
			дайКолонку(i, d);
			f.устЗначение(d);
			break;
		}
		case INT64_V: {
			int64 d;
			дайКолонку(i, d);
			f.устЗначение(d);
			break;
		}
		case DOUBLE_V: {
			double d;
			дайКолонку(i, d);
			f.устЗначение(d);
			break;
		}
		case DATE_V: {
			Дата d;
			дайКолонку(i, d);
			f.устЗначение(d);
			break;
		}
		case TIME_V: {
			Время t;
			дайКолонку(i, t);
			f.устЗначение(t);
			break;
		}
		case VALUE_V: {
			switch(column[i].тип) {
				case SQLT_STR:
				/*case SQLT_RDD:*/ {
					Ткст s;
					дайКолонку(i, s);
					f.устЗначение(s);
					break;
				}
				case SQLT_BLOB:
				case SQLT_CLOB:
				case SQLT_INT: {
					int64 d;
					дайКолонку(i, d);
					f.устЗначение(d);
					break;
				}
				case SQLT_FLT: {
					double d;
					дайКолонку(i, d);
					f.устЗначение(d);
					break;
				}
				case SQLT_DAT: {
					Время m;
					дайКолонку(i, m);
					if(m.hour || m.minute || m.second)
						f = Значение(m);
					else
						f = Значение(Дата(m));
					break;
				}
				default: {
					NEVER();
				}
			}
			break;
		}
		default: {
			NEVER();
		}
	}
}

void OCI8Connection::Cancel() {
	oci8.OCIBreak(SvcCtx(), errhp);
	parse = true;
}

void OCI8Connection::устОш() {
	if(session)
		session->SetOciError(statement, errhp);
	parse = true;
}

SqlSession& OCI8Connection::GetSession() const {
	ПРОВЕРЬ(session);
	return *session;
}

Ткст OCI8Connection::GetUser() const {
	ПРОВЕРЬ(session);
	return session->user;
}

Ткст OCI8Connection::вТкст() const {
	Ткст lg;
	bool quotes = false;
	int argn = 0;
	for(const char *q = statement; *q; q++) {
		if(*q== '\'' && q[1] != '\'')
			quotes = !quotes;
		if(!quotes && *q == '?') {
			if(argn < param.дайСчёт()) {
				const Элемент& m = param[argn++];
				if(m.пусто_ли())
					lg << "Null";
				else
				switch(m.тип) {
				case SQLT_STR:
					lg.конкат('\'');
					lg += (const char *) m.Данные();
					lg.конкат('\'');
					break;
				case SQLT_FLT:
					lg << *(const double *) m.Данные();
					break;
				case SQLT_DAT:
					lg << OciDecodeTime(m.Данные());
//					const byte *data = m.Данные();
//					lg << (int)data[3] << '.' << (int)data[2] << '.' <<
//						  int(data[0] - 100) * 100 + data[1] - 100;
					break;
				}
			}
			else
				lg += t_("<not supplied>");
		}
		else
			lg += *q;
	}
	return lg;
}

static int conn_count = 0;

OCI8Connection::OCI8Connection(Oracle8& s)
: session(&s)
, oci8(s.oci8)
{
	LLOG("OCI8Connection construct, #" << ++conn_count << " total");
	refcursor = false;
	if(!session->AllocOciHandle(&stmthp, OCI_HTYPE_STMT)
	|| !session->AllocOciHandle(&errhp, OCI_HTYPE_ERROR))
		session->устОш(t_("Ошибка initializing connection"), t_("OCI8 connection"));
	линкПосле(&s.clink);
}

void OCI8Connection::очисть() {
	if(session) {
		if(refcursor) {
			OCIStmt *aux = 0;
			if(!session -> AllocOciHandle(&aux, OCI_HTYPE_STMT)) {
				int errcode;
				Ткст err = OciError(oci8, errhp, &errcode, session->utf8_session);
				session->устОш(err, t_("Closing reference cursor"), errcode, NULL, OciErrorClass(errcode));
			}
			static char close[] = "begin close :1; end;";
			bool err = false;
			OCIBind *bind = 0;
			err = oci8.OCIStmtPrepare(aux, errhp, (OraText *)close, (ub4)strlen(close), OCI_NTV_SYNTAX, OCI_DEFAULT)
				|| oci8.OCIBindByPos(aux, &bind, errhp, 1, &stmthp, 0, SQLT_RSET, 0, 0, 0, 0, 0, OCI_DEFAULT)
				|| oci8.OCIStmtExecute(SvcCtx(), aux, errhp, 1, 0, 0, 0, OCI_DEFAULT);
			if(err)
				устОш();
			session -> FreeOciHandle(aux, OCI_HTYPE_STMT);
		}
		session -> FreeOciHandle(stmthp, OCI_HTYPE_STMT);
		session -> FreeOciHandle(errhp, OCI_HTYPE_ERROR);
		session = NULL;
	}
}

OCI8Connection::~OCI8Connection() {
	очисть();
	LLOG("OCI8Connection destruct, #" << --conn_count << " left");
}

SqlConnection *Oracle8::CreateConnection() {
	return new OCI8Connection(*this);
}

bool   Oracle8::открыт() const {
	return svchp;
}

bool Oracle8::открой(const Ткст& connect_string, bool use_objects, Ткст *warn) {
	Ткст имя, pwd, server;
	const char *b = connect_string, *p = b;
	while(*p && *p != '/' && *p != '@')
		p++;
	имя = Ткст(b, p);
	if(*p == '/') {
		b = ++p;
		while(*p && *p != '@')
			p++;
		pwd = Ткст(b, p);
	}
	if(*p == '@')
		server = ++p;
	return Login(имя, pwd, server, use_objects, warn);
}

static void OCIInitError(Oracle8& ora, Ткст infn)
{
	ora.Logoff();
	ora.устОш(фмтЧ(t_("Ошибка initializing OCI8 library (%s)"), infn),
		t_("Connecting to Oracle database."), 0, NULL, Sql::CONNECTION_BROKEN);
}

bool Oracle8::Login(const char *имя, const char *pwd, const char *db, bool use_objects, Ткст *warn) {
	LLOG("Oracle8::Login");
	level = 0;
	Logoff();
	сотриОш();
	user = взаг(Ткст(имя));
	LLOG("Loading OCI8 library");
	if(!oci8.грузи()) {
		устОш(t_("Ошибка loading OCI8 Oracle connection dynamic library."),
			t_("Connecting to Oracle database."), 0, NULL, Sql::CONNECTION_BROKEN);
		return false;
	}
	LLOG("OCI8 loaded -> OCIInitialize, OCIEnvInit");
	int accessmode = (use_objects ? OCI_OBJECT : 0)
#if defined(_MULTITHREADED) || defined(PLATFORM_POSIX)
		| OCI_THREADED
#endif
	;
	
	utf8_session = false;
	if(!disable_utf8_mode
	&& oci8.OCIEnvNlsCreate) {
		if(oci8.OCIEnvNlsCreate(&envhp, accessmode, 0, 0, 0, 0, 0, 0,
		OCI_NLS_NCHARSET_ID_AL32UT8, OCI_NLS_NCHARSET_ID_AL32UT8) != OCI_SUCCESS
		&& oci8.OCIEnvNlsCreate(&envhp, accessmode, 0, 0, 0, 0, 0, 0,
		OCI_NLS_NCHARSET_ID_UT8, OCI_NLS_NCHARSET_ID_UT8) != OCI_SUCCESS)
			LLOG("OCI8: Ошибка on initialization utf8 NLS");
		else
			utf8_session = true;
	}
	if(!utf8_session) {
		if(oci8.OCIEnvCreate) {
			if(oci8.OCIEnvCreate(&envhp, accessmode, 0, 0, 0, 0, 0, 0)) {
				OCIInitError(*this, "OCIEnvCreate");
				return false;
			}
		} else {
			if(oci8.OCIInitialize(accessmode, 0, 0, 0, 0)) {
				OCIInitError(*this, "OCIInitialize");
				return false;
			}
			if(oci8.OCIEnvInit(&envhp, OCI_DEFAULT, 0, 0)) {
				OCIInitError(*this, "OCIEnvInit");
				return false;
			}
		}
	}
	if(!AllocOciHandle(&errhp, OCI_HTYPE_ERROR)) {
		OCIInitError(*this, "OCI_HTYPE_ERROR");
		return false;
	}
	if(!AllocOciHandle(&svchp, OCI_HTYPE_SVCCTX)) {
		OCIInitError(*this, "OCI_HTYPE_SVCCTX");
		return false;
	}
	if(!AllocOciHandle(&srvhp, OCI_HTYPE_SERVER)) {
		OCIInitError(*this, "OCI_HTYPE_SERVER");
		return false;
	}
	if(!AllocOciHandle(&seshp, OCI_HTYPE_SESSION)) {
		OCIInitError(*this, "OCI_HTYPE_SESSION");
		return false;
	}
	LLOG("Attributes allocated -> OCIServerAttach");
	if(oci8.OCIServerAttach(srvhp, errhp, (byte *)db, (sb4)strlen(db), 0)) {
		SetOciError(фмтЧ(t_("Connecting to server '%s'"), db), errhp);
		Logoff();
		return false;
	}
	LLOG("Server attached -> OCIAttrSet, OCISessionBegin");
	in_server = true;
	sword retcode;
	if(oci8.OCIAttrSet(svchp, OCI_HTYPE_SVCCTX, srvhp, 0, OCI_ATTR_SERVER, errhp)
	|| oci8.OCIAttrSet(seshp, OCI_HTYPE_SESSION, (byte *)имя, (ub4)strlen(имя), OCI_ATTR_USERNAME, errhp)
	|| oci8.OCIAttrSet(seshp, OCI_HTYPE_SESSION, (byte *)pwd, (ub4)strlen(pwd), OCI_ATTR_PASSWORD, errhp)
	|| oci8.OCIAttrSet(svchp, OCI_HTYPE_SVCCTX, seshp, 0, OCI_ATTR_SESSION, errhp)
	|| (retcode = oci8.OCISessionBegin(svchp, errhp, seshp, OCI_CRED_RDBMS, OCI_DEFAULT)) != OCI_SUCCESS
	&& retcode != OCI_SUCCESS_WITH_INFO) {
		SetOciError(t_("Connecting to Oracle database."), errhp);
		Logoff();
		return false;
	}
	if(retcode == OCI_SUCCESS_WITH_INFO && warn) {
		int errcode;
		*warn = OciError(oci8, errhp, &errcode, utf8_session);
	}
	LLOG("Session attached, user = " + GetUser());
	in_session = true;
	return true;
}

void Oracle8::Logoff() {
	SessionClose();
	LOG("Oracle8::Logoff, #" << conn_count << " connections pending");
	while(!clink.пустой()) {
		clink.дайСледщ()->очисть();
		clink.дайСледщ()->отлинкуй();
		LOG("-> #" << conn_count << " connections left");
	}
	if(in_session)
	{
		in_session = false;
		LLOG("OCISessionEnd");
		oci8.OCISessionEnd(svchp, errhp, seshp, OCI_DEFAULT);
	}
	if(in_server)
	{
		in_server = false;
		LLOG("OCIServerDetach");
		oci8.OCIServerDetach(srvhp, errhp, OCI_DEFAULT);
	}
	FreeOciHandle(seshp, OCI_HTYPE_SESSION);
	seshp = NULL;
	FreeOciHandle(srvhp, OCI_HTYPE_SERVER);
	srvhp = NULL;
	FreeOciHandle(svchp, OCI_HTYPE_SVCCTX);
	svchp = NULL;
	FreeOciHandle(errhp, OCI_HTYPE_ERROR);
	errhp = NULL;
	if(envhp) {
		FreeOciHandle(envhp, OCI_HTYPE_ENV);
		envhp = NULL;
		if(!oci8.OCIEnvCreate)
			oci8.OCITerminate(OCI_DEFAULT);
	}
}

void   Oracle8::PostError() {
}

Ткст Oracle8::Spn() {
	return фмт("TRANSACTION_LEVEL_%d", level);
}

void   Oracle8::старт() {
	if(Поток *s = GetTrace())
		*s << user << "(OCI8) -> StartTransaction(level " << level << ")\n";
	level++;
//	сотриОш();
	if(level > 1)
		Sql(*this).выполни("savepoint " + Spn());
}

void   Oracle8::Commit() {
	int time = msecs();
	ПРОВЕРЬ(tmode == ORACLE || level > 0);
	if(level)
		level--;
//	else
//		сотриОш();
	if(level == 0) {
		oci8.OCITransCommit(svchp, errhp, OCI_DEFAULT);
//		if(Поток *s = GetTrace())
//			*s << "%commit;\n";
	}
	if(Поток *s = GetTrace())
		*s << фмтЧ("----- %s (OCI8) -> Commit(level %d) %d ms\n", user, level, msecs(time));
}

void   Oracle8::Rollback() {
	ПРОВЕРЬ(tmode == ORACLE || level > 0);
	if(level > 1)
		Sql(*this).выполни("rollback to savepoint " + Spn());
	else {
		oci8.OCITransRollback(svchp, errhp, OCI_DEFAULT);
//		if(Поток *s = GetTrace())
//			*s << "%rollback;\n";
	}
	if(level)
		level--;
//	else
//		сотриОш();
	if(Поток *s = GetTrace())
		*s << user << "(OCI8) -> Rollback(level " << level << ")\n";
}

Ткст Oracle8::Savepoint() {
	static dword i;
	i = (i + 1) & 0x8fffffff;
	Ткст s = спринтф("SESSION_SAVEPOINT_%d", i);
	Sql(*this).выполни("savepoint " + s);
	return s;
}

void   Oracle8::RollbackTo(const Ткст& savepoint) {
	Sql(*this).выполни("rollback to savepoint " + savepoint);
}

Oracle8::Oracle8(T_OCI8& oci8_)
: oci8(oci8_)
{
	level = 0;
	envhp = NULL;
	errhp = NULL;
	srvhp = NULL;
	seshp = NULL;
	svchp = NULL;
	tmode = NORMAL;
	disable_utf8_mode = false;
	in_session = in_server = utf8_session = false;
	Dialect(ORACLE);
}

Oracle8::~Oracle8() {
	Logoff();
}

Вектор<Ткст> Oracle8::EnumUsers()
{
	Sql cursor(*this);
	return OracleSchemaUsers(cursor);
}

Вектор<Ткст> Oracle8::EnumDatabases()
{
	Sql cursor(*this);
	return OracleSchemaUsers(cursor);
}

Вектор<Ткст> Oracle8::EnumTables(Ткст database)
{
	Sql cursor(*this);
	return OracleSchemaTables(cursor, database);
}

Вектор<Ткст> Oracle8::EnumViews(Ткст database)
{
	Sql cursor(*this);
	return OracleSchemaViews(cursor, database);
}

Вектор<Ткст> Oracle8::EnumSequences(Ткст database)
{
	Sql cursor(*this);
	return OracleSchemaSequences(cursor, database);
}

Вектор<Ткст> Oracle8::EnumPrimaryKey(Ткст database, Ткст table)
{
	Sql cursor(*this);
	return OracleSchemaPrimaryKey(cursor, database, table);
}

Ткст Oracle8::EnumRowID(Ткст database, Ткст table)
{
	Sql cursor(*this);
	return OracleSchemaRowID(cursor, database, table);
}

Вектор<Ткст> Oracle8::EnumReservedWords()
{
	return OracleSchemaReservedWords();
}

bool Oracle8::BulkExecute(const char *stmt, const Вектор< Вектор<Значение> >& param_rows)
{
	if(!*stmt || param_rows.пустой())
		return true;
	return OCI8Connection(*this).BulkExecute(stmt, param_rows);
}

void OracleBlob::SetStreamSize(int64 pos) {
	ПРОВЕРЬ(pos <= дайРазмПотока());
	if(pos < (int)дайРазмПотока())
		session->oci8.OCILobTrim(session->svchp, session->errhp, locp, (int)pos);
}

dword OracleBlob::читай(int64 at, void *ptr, dword size) {
	ПРОВЕРЬ(открыт() && (style & STRM_READ) && session);
	ПРОВЕРЬ(at == (dword)at);
	ub4 n = size;
	if(session->oci8.OCILobRead(session->svchp, session->errhp, locp, &n, (dword)at + 1, ptr, size,
		NULL, NULL, 0, SQLCS_IMPLICIT) != OCI_SUCCESS) return 0;
	return n;
}

void OracleBlob::пиши(int64 at, const void *ptr, dword size) {
	ПРОВЕРЬ(открыт() && (style & STRM_WRITE) && session);
	ПРОВЕРЬ(at == (dword)at);
	if(!size)
		return;
	ub4 n = size;
	int res = session->oci8.OCILobWrite(session->svchp, session->errhp, locp, &n, (dword)at + 1, (void *)ptr, size,
		OCI_ONE_PIECE, NULL, NULL, 0, SQLCS_IMPLICIT);
	if(res != OCI_SUCCESS || n != size)
	{
		RLOG("OracleBlob::пиши(" << at << ", " << size << "): res = " << res << ", n = " << n);
		устОш();
	}
}

void OracleBlob::присвой(Oracle8& s, int64 blob) {
	session = &s;
	locp = (OCILobLocator *)(ptrdiff_t)blob;
	ub4 n;
	OpenInit(READWRITE,
		session->oci8.OCILobGetLength(session->svchp, session->errhp, locp, &n) == OCI_SUCCESS ? n : 0);
}


void OracleBlob::присвой(const Sql& sql, int64 blob) {
	Oracle8 *session = dynamic_cast<Oracle8 *>(&sql.GetSession());
	ПРОВЕРЬ(session);
	присвой(*session, blob);
}

bool OracleBlob::открыт() const {
	return locp;
}

void OracleBlob::закрой() {
	if(locp) слей();
	locp = NULL;
}

OracleBlob::OracleBlob(const Sql& sql, int64 blob) {
	присвой(sql, blob);
}

OracleBlob::OracleBlob(Oracle8& session, int64 blob) {
	присвой(session, blob);
}

OracleBlob::OracleBlob() {
	locp = NULL;
	session = NULL;
}

OracleBlob::~OracleBlob() {
	закрой();
}

void OracleClob::SetLength(int sz)
{
	SetStreamSize(sz << 1);
}

void OracleClob::SetStreamSize(int64 pos) {
	ПРОВЕРЬ(pos <= дайРазмПотока());
	if(pos < (int)дайРазмПотока())
		session->oci8.OCILobTrim(session->svchp, session->errhp, locp, (int)(pos >> 1));
}

dword OracleClob::читай(int64 at, void *ptr, dword size) {
	ПРОВЕРЬ(открыт() && (style & STRM_READ) && session);
	ПРОВЕРЬ(at == (dword)at);
	int full_bytes = 0;
	bool utf8 = session->IsUtf8Session();
	ub4 readpos = (ub4)(at >> 1);
	ub4 readsize = (ub4)(((at + size + 1) & ~1) - (readpos << 1));
	ub4 read16 = (readsize + 1) >> 1;
	ub4 bufsize = min(utf8 ? 4 * (int)read16 : (int)read16, 16384);
	Буфер<char> charbuf(bufsize);
	while(read16 > 0) {
		ub4 nchars = read16;
		sword res = session->oci8.OCILobRead(session->svchp, session->errhp, locp,
			&nchars, readpos + 1, charbuf, bufsize,
			NULL, NULL, 0, SQLCS_IMPLICIT);
		if(res != OCI_SUCCESS && res != OCI_NEED_DATA)
			return full_bytes;
		ШТкст unibuf;
		if(utf8)
			unibuf = вУтф32(charbuf, nchars);
		else
			unibuf = вЮникод(charbuf, nchars, CHARSET_DEFAULT);
		int ulen = unibuf.дайДлину();
		int uoff = (int)(at & 1);
		int upart = min((int)size, 2 * ulen - uoff);
		memcpy(ptr, (const byte *)~unibuf + uoff, upart);
		ptr = (byte *)ptr + upart;
		at += upart;
		size -= upart;
		read16 -= ulen;
		full_bytes += upart;
		if(res != OCI_NEED_DATA)
			break;
	}
	return full_bytes;
}

void OracleClob::пиши(int64 at, const void *ptr, dword size) {
	ПРОВЕРЬ(открыт() && (style & STRM_WRITE) && session);
	ПРОВЕРЬ(at == (dword)at);
	if(!size)
		return;
	bool utf8 = session->IsUtf8Session();
	if(at & 1) {
		char auxbuf[2];
		читай(at - 1, auxbuf, 1);
		auxbuf[1] = *(byte *)ptr;
		word wch = Peek16le(auxbuf);
		Ткст chrbuf = (utf8 ? вУтф8(wch) : Ткст(изЮникода(wch, CHARSET_DEFAULT), 1));
		ub4 n = chrbuf.дайДлину();
		sword res = session->oci8.OCILobWrite(session->svchp, session->errhp, locp,
			&n, (dword)(at >> 1) + 1, (dvoid *)chrbuf.старт(), chrbuf.дайДлину(),
			OCI_ONE_PIECE, NULL, NULL, 0, SQLCS_IMPLICIT);
		if(res != OCI_SUCCESS || n != 1) {
			устОш();
			return;
		}
		
		ptr = (byte *)ptr + 1;
		size--;
		at++;
	}
	
	if(size >> 1) {
		ub4 nchars = size >> 1;
		Ткст chrbuf;
		if(utf8)
			chrbuf = вУтф8((const wchar *)ptr, nchars);
		else
			chrbuf = изЮникода((const wchar *)ptr, nchars, CHARSET_DEFAULT);
		ub4 n = chrbuf.дайДлину();
		sword res = session->oci8.OCILobWrite(session->svchp, session->errhp, locp,
			&n, (dword)(at >> 1) + 1, (dvoid *)chrbuf.старт(), chrbuf.дайДлину(),
			OCI_ONE_PIECE, NULL, NULL, 0, SQLCS_IMPLICIT);
		if(res != OCI_SUCCESS || n != nchars) {
			устОш();
			return;
		}
		ptr = (byte *)ptr + (nchars << 1);
		at += (nchars << 1);
		size -= (nchars << 1);
	}
	
	if(size & 1) {
		char auxbuf[2];
		ub4 pos16 = (ub4)(at >> 1);
		читай(at, auxbuf + 1, 1);
		auxbuf[0] = *((byte *)ptr + size - 1);
		word wch = Peek16le(auxbuf);
		Ткст chrbuf = (utf8 ? вУтф8(wch) : Ткст(изЮникода(wch, CHARSET_DEFAULT), 1));
		ub4 n = chrbuf.дайДлину();
		sword res = session->oci8.OCILobWrite(session->svchp, session->errhp, locp,
			&n, pos16 + 1, (dvoid *)chrbuf.старт(), chrbuf.дайДлину(),
			OCI_ONE_PIECE, NULL, NULL, 0, SQLCS_IMPLICIT);
		if(res != OCI_SUCCESS || n != 1) {
			устОш();
			return;
		}
	}
}

ШТкст OracleClob::читай()
{
	ШТкстБуф out;
	int nchars = (int)(GetLeft() >> 1);
	out.устСчёт(nchars);
	перейди(0);
	Поток::дай(out, 2 * nchars);
	return ШТкст(out);
}

void OracleClob::пиши(const ШТкст& w)
{
	помести(w, 2 * w.дайДлину());
}

void OracleClob::присвой(Oracle8& s, int64 blob) {
	session = &s;
	locp = (OCILobLocator *)(ptrdiff_t)blob;
	ub4 n;
	OpenInit(READWRITE,
		session->oci8.OCILobGetLength(session->svchp, session->errhp, locp, &n) == OCI_SUCCESS
		? 2 * n : 0);
}


void OracleClob::присвой(const Sql& sql, int64 blob) {
	Oracle8 *session = dynamic_cast<Oracle8 *>(&sql.GetSession());
	ПРОВЕРЬ(session);
	присвой(*session, blob);
}

bool OracleClob::открыт() const {
	return locp;
}

void OracleClob::закрой() {
	if(locp) слей();
	locp = NULL;
}

OracleClob::OracleClob(const Sql& sql, int64 blob) {
	присвой(sql, blob);
}

OracleClob::OracleClob(Oracle8& session, int64 blob) {
	присвой(session, blob);
}

OracleClob::OracleClob() {
	locp = NULL;
	session = NULL;
}

OracleClob::~OracleClob() {
	закрой();
}

}
