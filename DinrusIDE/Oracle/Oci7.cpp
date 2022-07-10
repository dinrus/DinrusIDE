#include "Oracle7.h"
#include "OciCommon.h"

/*
extern "C" {
#define dword _dword
typedef byte text;
#include <oratypes.h>
#include <ocidfn.h>
#include <ocidem.h>
#include <ociapr.h>
#undef dword
};
*/

namespace РНЦП {

#ifdef PLATFORM_WIN32
	#define DLLFILENAME "ociw32.dll"
#else
	#define DLLFILENAME "libclntsh.so"
#endif

#define DLIMODULE   OCI7
#define DLIHEADER   <Oracle/Oci7.dli>
#include <Core/dli_source.h>

void OCI7SetDllPath(Ткст oci7_path, T_OCI7& oci7)
{
	static Ткст dflt_name;
	if(пусто_ли(dflt_name))
		dflt_name = oci7.GetLibName();
	if(oci7_path != oci7.GetLibName())
		oci7.SetLibName(Nvl(oci7_path, dflt_name));
}

class OCI7Connection : public Link<OCI7Connection>, public OciSqlConnection {
protected:
	virtual void        SetParam(int i, OracleRef r);
	virtual void        SetParam(int i, const Значение& r);
	virtual bool        выполни();
	virtual int         GetRowsProcessed() const;
	virtual bool        Fetch();
	virtual void        дайКолонку(int i, Реф f) const;
	virtual void        Cancel();
	virtual SqlSession& GetSession() const;
	virtual Ткст      GetUser() const;
	virtual Ткст      вТкст() const;
	virtual            ~OCI7Connection();

protected:
/*
	enum {
		O_VARCHAR2 =   1,
		O_NUMBER   =   2,
		O_LONG     =   8,
		O_ROWID    =  11,
		O_DATE     =  12,
		O_RAW      =  23,
		O_LONGRAW  =  24,
		O_CHAR     =  96,
		O_MLSLABEL = 105,
	};
*/

	struct Param {
		int16          тип;
		word           len;
		sb2            ind;
//		Вектор<Значение>  dynamic;
//		bool           is_dynamic;
///		bool           dyna_full;
//		int            dyna_vtype;
//		int            dyna_width;
		OCI7Connection *refcursor;
		union {
			byte      *ptr;
			byte       буфер[8];
		};

		byte *Данные()                 { return len > 8u ? ptr : буфер; }
		const byte *Данные() const     { return len > 8u ? ptr : буфер; }
		bool  пусто_ли() const         { return ind < 0; }
		void  освободи();
//		void  DynaFlush();

		Param()                      { len = 0; refcursor = 0; }
		~Param()                     { освободи(); }
	};

	struct Колонка {
		int     тип;
		int     len;
		byte   *data;
		sb2    *ind;
		ub2    *rl;
		ub2    *rc;
		bool    lob;

		Колонка()          { data = NULL; }
		~Колонка();
	};

 	mutable cda_def cda;

	Массив<Param>   param;
	Массив<Колонка>  column;

	Ткст         longparam;
	int            frows;
	unsigned       fetched;
	int            fetchtime;
	int            fi;
	bool           eof;
//	Вектор<int>    dynamic_param;
//	int            dynamic_pos;
//	int            dynamic_rows;

	T_OCI7&        oci7;
	Oracle7       *session;

	Param&    PrepareParam(int i, int тип, int len, int reserve /*, int dynamic_vtype*/);
	void      SetParam(int i, const Ткст& s);
	void      SetRawParam(int i, const Ткст& s);
	void      SetParam(int i, int integer);
	void      SetParam(int i, double d);
	void      SetParam(int i, Дата d);
	void      SetParam(int i, Время d);
	void      SetParam(int i, Sql& cursor);

	void      добавьКолонку(int тип, int len, bool lob = false);
	void      дайКолонку(int i, Ткст& s) const;
	void      дайКолонку(int i, int& n) const;
	void      дайКолонку(int i, double& d) const;
	void      дайКолонку(int i, Дата& d) const;
	void      дайКолонку(int i, Время& t) const;

	void      устОш();
	bool      GetColumnInfo();

	void      очисть();

    OCI7Connection(Oracle7& s);

	friend class Oracle7;
};

//template MoveType<OCI7Connection::Param>;
//template MoveType<OCI7Connection::Колонка>;

void OCI7Connection::Param::освободи() {
	if(len != (word)-1 && len > sizeof(буфер))
		delete[] ptr;
	len = 0;
}

/*
void OCI7Connection::Param::DynaFlush() {
	Значение v;
	if(ind == 0) {
		const byte *p = Данные();
		switch(тип) {
		case SQLT_INT:
			v = *(const int *)p;
			break;

		case SQLT_FLT:
			v = *(const double *)p;
			break;

		case SQLT_STR:
			v = Ткст((const char *)p);
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
*/

OCI7Connection::Колонка::~Колонка() {
	if(data) {
		delete[] data;
		delete[] ind;
		delete[] rl;
		delete[] rc;
	}
}

OCI7Connection::Param& OCI7Connection::PrepareParam(int i, int тип, int len, int reserve /*, int dvtype*/) {
	Param& p = param.по(i);
//	p.dyna_vtype = dvtype;
//	p.is_dynamic = (dvtype != VOID_V);
///	p.dyna_full = false;
//	p.dyna_width = len;
	p.ind = 0;
	if(p.тип != тип || p.len < len || reserve == 0 && p.len != len) {
		p.освободи();
		parse = true;
	}
	if(!p.len) {
		p.len = len + reserve;
		if(p.len > 8)
			p.ptr = new byte[p.len];
		p.тип = -1;
		parse = true;
	}
	if(p.тип != тип) {
		p.тип = тип;
		parse = true;
	}
	return p;
}

void OCI7Connection::SetParam(int i, const Ткст& s) {
	int l = s.дайДлину();
	Param& p = PrepareParam(i, SQLT_STR, l + 1, 256 /*, VOID_V*/);
	memcpy(p.Данные(), s, l + 1);
	p.ind = l ? 0 : -1;
}

void OCI7Connection::SetRawParam(int i, const Ткст& s) {
	PrepareParam(i, SQLT_LBI, 0, 0 /*, VOID_V*/);
	longparam = s;
}

void OCI7Connection::SetParam(int i, int integer) {
	Param& p = PrepareParam(i, SQLT_INT, sizeof(int), 0 /*, VOID_V*/);
	*(int *) p.Данные() = integer;
	p.ind = пусто_ли(integer) ? -1 : 0;
}

void OCI7Connection::SetParam(int i, double d) {
	Param& p = PrepareParam(i, SQLT_FLT, sizeof(double), 0 /*, VOID_V*/);
	*(double *) p.Данные() = d;
	p.ind = пусто_ли(d) ? -1 : 0;
}

void OCI7Connection::SetParam(int i, Дата d) {
	Param& w = PrepareParam(i, SQLT_DAT, 7, 0 /*, VOID_V*/);
	w.ind = OciEncodeDate(w.Данные(), d) ? 0 : -1;
}

void OCI7Connection::SetParam(int i, Время t) {
	Param& w = PrepareParam(i, SQLT_DAT, 7, 0 /*, VOID_V*/);
	w.ind = OciEncodeTime(w.Данные(), t) ? 0 : -1;
}

void OCI7Connection::SetParam(int i, OracleRef r) {
	NEVER(); // OCI7 dynamic's are currently not allowed
//	PrepareParam(i, r.GetOraType(), r.GetMaxLen(), 0, r.дайТип());
}

class Oracle7RefCursorStub : public SqlSource {
public:
	Oracle7RefCursorStub(SqlConnection *cn) : cn(cn) {}

	virtual SqlConnection *CreateConnection() { return cn; }

private:
	SqlConnection         *cn;
};

void OCI7Connection::SetParam(int i, Sql& cursor) {
	Param& w = PrepareParam(i, SQLT_CUR, -1, 0 /*, VOID_V*/);
	w.refcursor = new OCI7Connection(*session);
	Oracle7RefCursorStub stub(w.refcursor);
	w.ptr = (byte *)&w.refcursor -> cda;
	w.ind = 0;
}

void  OCI7Connection::SetParam(int i, const Значение& q) {
	if(q.пусто_ли())
		SetParam(i, Ткст(""));
	else
		switch(q.дайТип()) {
		case 34:
			SetRawParam(i, SqlRaw(q));
			break;
		case STRING_V:
		case WSTRING_V:
			SetParam(i, Ткст(q));
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
			NEVER();
		}
}

void  OCI7Connection::добавьКолонку(int тип, int len, bool lob) {
	Колонка& q = column.добавь();
	q.тип = тип;
	q.len = len;
	q.data = new byte[frows * len];
	q.ind = new sb2[frows];
	q.rl = new ub2[frows];
	q.rc = new ub2[frows];
	q.lob = lob;
}

bool OCI7Connection::выполни() {
	int time = msecs();
	session->PreExec();
	int args = 0;
	if(parse) {
		Ткст cmd;
		args = OciParse(statement, cmd, this, session);
		Cancel();
		if(oci7.oparse(&cda, (OraText *)(const char *)cmd, -1, 0, 2) != 0) {
			устОш();
			return false;
		}
		while(param.дайСчёт() < args)
			SetParam(param.дайСчёт(), Ткст());
		param.обрежь(args);
//		dynamic_param.очисть();
  		for(int i = 0; i < args; i++) {
			Param& p = param[i];
			if(p.тип == SQLT_LBI) {
				Ткст h = фмт(":%d", i + 1);
				if(oci7.obindps(&cda, 0, (byte *)~h, h.дайДлину(), NULL,
					            max(1, longparam.дайДлину()), p.тип,
								0, NULL, NULL, NULL, 0, 0, 0, 0, 0, NULL, NULL, 0, 0)) {
					устОш();
					return false;
				}
			}
    		else
			if(oci7.obndrn(&cda, i + 1, (OraText *)p.Данные(), p.len, p.тип,
					       -1, &p.ind, NULL, -1, -1) != 0) {
				устОш();
				return false;
			}
//			if(p.is_dynamic)
//				dynamic_param.добавь(i);
		}
	}
	for(;;) {
//		dword time;
//		if(session->IsTraceTime())
//			time = GetTickCount();
		oci7.oexec(&cda);
//		if(session->IsTraceTime() && session->GetTrace())
//			*session->GetTrace() << спринтф("--------------\nexec %d ms:\n", GetTickCount() - time);
		if(cda.rc == 0)
			break;
		if(cda.rc != 3129) {
			устОш();
			longparam.очисть();
			return false;
		}
		ub4 l = longparam.дайДлину();
		oci7.osetpi(&cda, OCI_LAST_PIECE, (void *)~longparam, &l);
	}

//	if(!dynamic_param.пустой()) {
//		dynamic_pos = -1;
//		for(int i = 0; i < dynamic_param.дайСчёт(); i++)
//			param[dynamic_param[i]].DynaFlush();
//		dynamic_rows = param[dynamic_param[0]].dynamic.дайСчёт();
//	}

	if(parse) {
		if(!GetColumnInfo())
			return false;
		for(int i = 0; i < args; i++)
			if(param[i].refcursor && !param[i].refcursor -> GetColumnInfo())
				return false;
	}
	else {
		fetched = 0;
		fetchtime = 0;
		fi = -1;
		eof = false;
	}
	longparam.очисть();
	session->PostExec();
	if(Поток *s = session->GetTrace()) {
		if(session->IsTraceTime())
			*s << фмт("----- exec %d ms\n", msecs(time));
	}
	return true;
}

bool OCI7Connection::GetColumnInfo()
{
	fetched = 0;
	fetchtime = 0;
	fi = -1;
	eof = false;
	info.очисть();
	column.очисть();
	frows = fetchrows;
	Вектор<int> tp;
	int i;
	for(i = 0;; i++) {
		char h[256];
		sb4 width;
		sb2 тип, prec, scale, null;
		sb4 hlen = 255;
		if(oci7.odescr((cda_def *)&cda, i + 1, &width, &тип, (sb1 *)h, &hlen, NULL, &prec,
				       &scale, &null))
			break;
		SqlColumnInfo& ii = info.добавь();
		ii.width = width;
		ii.precision = prec;
		ii.scale = scale;
		h[hlen] = '\0';
		ii.имя = h;
		ii.имя = взаг(обрежьПраво(ii.имя));
		ii.binary = (тип == SQLT_LBI || тип == SQLT_BLOB);
		tp.добавь(тип);
		if(тип == SQLT_LBI || тип == SQLT_BLOB || тип == SQLT_CLOB)
			frows = 1;
	}
	for(i = 0; i < info.дайСчёт(); i++) {
		SqlColumnInfo& ii = info[i];
		switch(tp[i]) {
		case SQLT_NUM:
			ii.тип = DOUBLE_V;
			добавьКолонку(SQLT_FLT, sizeof(double));
			break;
		case SQLT_DAT:
			ii.тип = TIME_V;
			добавьКолонку(SQLT_DAT, 7);
			break;
		case SQLT_LBI:
			ii.тип = STRING_V;
			добавьКолонку(SQLT_LBI, 4096);
			break;
		case SQLT_CLOB:
			ii.тип = STRING_V;
			добавьКолонку(SQLT_LNG, 65520, true);
			break;
		case SQLT_BLOB:
			ii.тип = STRING_V;
			добавьКолонку(SQLT_LBI, 65520, true);
			break;
		default:
			ii.тип = STRING_V;
			добавьКолонку(SQLT_STR, ii.width ? ii.width + 1 : longsize);
			break;
		}
	}
	for(i = 0; i < column.дайСчёт(); i++) {
		Колонка& q = column[i];
		if(oci7.odefin(&cda, i + 1, (OraText *)q.data, q.len, q.тип, -1, q.ind, NULL, -1, -1, q.rl, q.rc)) {
			устОш();
			return false;
		}
	}
	parse = false;
	return true;
}

int OCI7Connection::GetRowsProcessed() const {
	return cda.rpc;
}

bool OCI7Connection::Fetch() {
	ПРОВЕРЬ(!parse);
	if(parse || eof) return false;
//	if(!dynamic_param.пустой()) // dynamic pseudo-fetch
//		return (dynamic_pos < dynamic_rows && ++dynamic_pos < dynamic_rows);
	if(frows == 1) {
		fi = 0;
		if(oci7.ofetch(&cda)) {
			if(cda.rc != 1403)
				устОш();
			return false;
		}
		fetched++;
		return true;
	}
	if(fi < 0 || fetched >= cda.rpc) {
		bool tt = session->IsTraceTime();
		int fstart = tt ? msecs() : 0;
		if(oci7.ofen(&cda, frows) && cda.rc != 1403) {
			eof = true;
			устОш();
			return false;
		}
		if(tt) {
			fetchtime += msecs(fstart);
			int added = fetched + cda.rpc;
			if(Поток *s = session->GetTrace())
				*s << фмтЧ("----- fetch(%d) in %d ms, %8n ms/rec, %2n rec/s\n",
					added, fetchtime,
					fetchtime / max<double>(added, 1),
					added * 1000.0 / max<double>(fetchtime, 1));
		}
		if(fetched >= cda.rpc) {
			eof = true;
			return false;
		}
		fi = 0;
	}
	else
		fi++;

	fetched++;
	return true;
}

void OCI7Connection::дайКолонку(int i, Ткст& s) const
{
//	if(!dynamic_param.пустой()) {
//		s = param[dynamic_param[i]].dynamic[dynamic_pos];
//		return;
//	}
	const Колонка& c = column[i];
	const byte *data = c.data + fi * c.len;
	int ind = c.ind[fi];
	if(c.тип == SQLT_STR)
		if(ind < 0)
			s = Null;
		else
			s = (const char *) data;
	else if(c.тип == SQLT_LBI || c.тип == SQLT_LNG) {
		if(ind == -1)
			s = Null;
		else {
			s = Ткст(data, *c.rl);
			if(!c.lob) {
				Буфер<byte> буфер(32768);
				ub4 len;
				while(!oci7.oflng(&cda, i + 1, буфер, 32768, c.тип, &len, s.дайДлину()) && len)
					s.конкат(буфер, len);
				if(s.дайРазмест() - s.дайДлину() > 32768)
					s.сожми();
			}
		}
	}
	else
		NEVER();
}

void OCI7Connection::дайКолонку(int i, int& n) const {
//	if(!dynamic_param.пустой()) {
//		n = param[dynamic_param[i]].dynamic[dynamic_pos];
//		return;
//	}
	const Колонка& c = column[i];
	byte *data = c.data + fi * c.len;
	if(c.ind[fi] < 0)
		n = INT_NULL;
	else
		switch(c.тип) {
		case SQLT_INT:
			n = *(int *) data;
			break;
		case SQLT_FLT:
			n = int(*(double *) data);
			break;
		default:
			ПРОВЕРЬ(0);
		}
}

void OCI7Connection::дайКолонку(int i, double& d) const {
//	if(!dynamic_param.пустой()) {
//		d = param[dynamic_param[i]].dynamic[dynamic_pos];
//		return;
//	}
	const Колонка& c = column[i];
	byte *data = c.data + fi * c.len;
	if(c.ind[fi] < 0)
		d = Null;
	else
		switch(c.тип) {
		case SQLT_INT:
			d = *(int *) data;
			break;
		case SQLT_FLT:
			d = *(double *) data;
			break;
		default:
			ПРОВЕРЬ(0);
		}
}

void OCI7Connection::дайКолонку(int i, Дата& d) const {
//	if(!dynamic_param.пустой()) {
//		d = param[dynamic_param[i]].dynamic[dynamic_pos];
//		return;
//	}
	const Колонка& c = column[i];
	const byte *data = c.data + fi * c.len;
	ПРОВЕРЬ(c.тип == SQLT_DAT);
	d = (c.ind[fi] < 0 ? Дата(Null) : OciDecodeDate(data));
}

void OCI7Connection::дайКолонку(int i, Время& t) const {
//	if(!dynamic_param.пустой()) {
//		t = param[dynamic_param[i]].dynamic[dynamic_pos];
//		return;
//	}
	const Колонка& c = column[i];
	const byte *data = c.data + fi * c.len;
	ПРОВЕРЬ(c.тип == SQLT_DAT);
	t = (c.ind[fi] < 0 ? Время(Null) : OciDecodeTime(data));
}

void  OCI7Connection::дайКолонку(int i, Реф f) const {
//	if(!dynamic_param.пустой()) {
//		f.устЗначение(param[dynamic_param[i]].dynamic[dynamic_pos]);
//		return;
//	}
	switch(f.дайТип()) {
		case STRING_V: {
			Ткст s;
			дайКолонку(i, s);
			f.устЗначение(s);
			break;
		}
		case INT_V: {
			int d;
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
			Время d;
			дайКолонку(i, d);
			f.устЗначение(d);
			break;
		}
		case VALUE_V: {
			switch(column[i].тип) {
				case SQLT_STR:
				case SQLT_LBI:
				case SQLT_LNG: {
					Ткст s;
					дайКолонку(i, s);
					f = Значение(s);
					break;
				}
				case SQLT_INT: {
					int n;
					дайКолонку(i, n);
					f = Значение(n);
					break;
				}
				case SQLT_FLT: {
					double d;
					дайКолонку(i, d);
					f = Значение(d);
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

void OCI7Connection::Cancel() {
	oci7.ocan(&cda);
	parse = true;
}

void OCI7Connection::устОш() {
	int code = cda.rc;
	if(code == 0 || code == 1002) return;
	session->устОш(session->GetErrorMsg(code), вТкст());
	parse = true;
}

SqlSession& OCI7Connection::GetSession() const {
	ПРОВЕРЬ(session);
	return *session;
}

Ткст OCI7Connection::GetUser() const {
	ПРОВЕРЬ(session);
	return session->user;
}

Ткст OCI7Connection::вТкст() const
{
	Ткст lg;
	bool quotes = false;
	int argn = 0;
	for(const char *q = statement; *q; q++) {
		if(*q== '\'' && q[1] != '\'')
			quotes = !quotes;
		if(!quotes && *q == '?') {
			if(argn < param.дайСчёт()) {
				const Param& m = param[argn++];
				if(m.тип == SQLT_LBI)
					lg.конкат("<дайРяд data>");
				else
				if(m.пусто_ли())
					lg << "Null";
				else
				switch(m.тип) {
				case SQLT_STR:
					lg.конкат('\'');
					lg += (const char *) m.Данные();
					lg.конкат('\'');
					break;
				case SQLT_INT:
					lg << *(const int *) m.Данные();
					break;
				case SQLT_FLT:
					lg << *(const double *) m.Данные();
					break;
				case SQLT_DAT:
//					const byte *data = m.Данные();
					lg << OciDecodeTime(m.Данные());
					//(int)data[3] << '.' << (int)data[2] << '.' <<
					//	  int(data[0] - 100) * 100 + data[1] - 100;
					break;
				}
			}
			else
				lg += "<not supplied>";
		}
		else
			lg += *q;
	}
	return lg;
}

OCI7Connection::OCI7Connection(Oracle7& s) : session(&s), oci7(s.oci7) {
	eof = true;
	memset(&cda, 0, sizeof(cda));
	CHECK(!oci7.oopen(&cda, (cda_def *) (session->lda), 0, -1, -1, 0, -1));
	линкПосле(&s.clink);
}

void OCI7Connection::очисть() {
	if(session) {
		oci7.oclose(&cda);
		session = NULL;
	}
}

OCI7Connection::~OCI7Connection() {
	очисть();
}

bool Oracle7::открой(const Ткст& connect) {
	закрой();
	::memset(lda, 0, sizeof(lda));
	::memset(hda, 0, sizeof(hda));
	if(!oci7.грузи()) {
		устОш(t_("Ошибка loading OCI7 Oracle database client library."), t_("Connecting to server"));
		return false;
	}
	int code = oci7.olog((cda_def *)lda, hda, (OraText *)(const char *)connect, -1,
	                     NULL, -1, NULL, -1, OCI_LM_DEF);
	if(code) {
		устОш(GetErrorMsg(code), t_("Connecting to database server"));
		return false;
	}
	connected = true;
	level = 0;
	сотриОш();
	user = Sql(*this).выдели("USER from DUAL");
	return true;
}

Ткст Oracle7::GetErrorMsg(int code) const {
	byte h[520];
	*h = 0;
	Ткст r;
	oci7.oerhms((cda_def *)lda, (sb2)code, (OraText *)h, 512);
	for(byte *s = h; *s; s++)
		r.конкат(*s < ' ' ? ' ' : *s);
	r << "(code " << code << ")";
	return r;
}

SqlConnection *Oracle7::CreateConnection() {
	return new OCI7Connection(*this);
}

void Oracle7::закрой() {
	SessionClose();
	while(!clink.пустой()) {
		clink.дайСледщ()->очисть();
		clink.дайСледщ()->отлинкуй();
	}
	if(connected) {
		oci7.ologof((cda_def *)lda);
		connected = false;
	}
}

Ткст Oracle7::Spn() {
	return фмт("TRANSACTION_LEVEL_%d", level);
}

void   Oracle7::PreExec() {
	bool ac = level == 0 && tmode == NORMAL;
	if(autocommit != ac) {
		autocommit = ac;
		Поток *s = GetTrace();
		if(autocommit) {
			if(s) *s << "%autocommit on;\n";
			oci7.ocon((cda_def *)lda);
		}
		else {
			if(s) *s << "%autocommit off;\n";
			oci7.ocof((cda_def *)lda);
		}
	}
}

void   Oracle7::PostExec() {
}

void   Oracle7::старт() {
	if(Поток *s = GetTrace())
		*s << user << "(OCI7) -> StartTransaction(level " << level << ")\n";
	level++;
//	сотриОш();
	if(level > 1)
		Sql(*this).выполни("savepoint " + Spn());
}

void   Oracle7::Commit() {
	int time = msecs();
//	ПРОВЕРЬ(tmode == ORACLE || level > 0);
	if(level)
		level--;
//	else
//		сотриОш();
	if(level == 0) {
		oci7.ocom((cda_def *)lda);
//		if(Поток *s = GetTrace())
//			*s << "%commit;\n";
	}
	if(Поток *s = GetTrace())
		*s << фмтЧ("----- %s (OCI7) -> Commit(level %d) %d ms\n", user, level, msecs(time));
}

void   Oracle7::Rollback() {
	ПРОВЕРЬ(tmode == ORACLE || level > 0);
	if(level > 1)
		Sql(*this).выполни("rollback to savepoint " + Spn());
	else {
		oci7.orol((cda_def *)lda);
		if(Поток *s = GetTrace())
			*s << "%rollback;\n";
	}
	if(level)
		level--;
//	else
//		сотриОш();
	if(Поток *s = GetTrace())
		*s << user << "(OCI7) -> Rollback(level " << level << ")\n";
}

Ткст Oracle7::Savepoint() {
	static dword i;
	i = (i + 1) & 0x8fffffff;
	Ткст s = спринтф("SESSION_SAVEPOINT_%d", i);
	Sql(*this).выполни("savepoint " + s);
	return s;
}

void   Oracle7::RollbackTo(const Ткст& savepoint) {
	Sql(*this).выполни("rollback to savepoint " + savepoint);
}

bool   Oracle7::открыт() const {
	return connected;
}

Oracle7::Oracle7(T_OCI7& oci7_)
: oci7(oci7_)
{
	connected = false;
	autocommit = false;
	level = 0;
	tmode = NORMAL;
	Dialect(ORACLE);
}

Oracle7::~Oracle7() {
//	ПРОВЕРЬ(level == 0);
	закрой();
}

Вектор<Ткст> Oracle7::EnumUsers()
{
	Sql cursor(*this);
	return OracleSchemaUsers(cursor);
}

Вектор<Ткст> Oracle7::EnumDatabases()
{
	Sql cursor(*this);
	return OracleSchemaUsers(cursor);
}

Вектор<Ткст> Oracle7::EnumTables(Ткст database)
{
	Sql cursor(*this);
	return OracleSchemaTables(cursor, database);
}

Вектор<Ткст> Oracle7::EnumViews(Ткст database)
{
	Sql cursor(*this);
	return OracleSchemaViews(cursor, database);
}

Вектор<Ткст> Oracle7::EnumSequences(Ткст database)
{
	Sql cursor(*this);
	return OracleSchemaSequences(cursor, database);
}

Вектор<Ткст> Oracle7::EnumPrimaryKey(Ткст database, Ткст table)
{
	Sql cursor(*this);
	return OracleSchemaPrimaryKey(cursor, database, table);
}

Ткст Oracle7::EnumRowID(Ткст database, Ткст table)
{
	Sql cursor(*this);
	return OracleSchemaRowID(cursor, database, table);
}

Вектор<Ткст> Oracle7::EnumReservedWords()
{
	return OracleSchemaReservedWords();
}

}
