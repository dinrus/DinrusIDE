// NOTE: Because of clash with Windows rpc.h, Core/Core.h has to be included before this header
// For the same reason implementation files are in 'imp' subdirectory

#ifndef Rpc_Rpc_h
#define Rpc_Rpc_h

namespace РНЦПДинрус {

enum {
	RPC_SERVER_JSON_ERROR = -32700,    // Parse Ошибка
	RPC_SERVER_JSON_REQUEST_ERROR = -32600, // Invalid Request
	RPC_UNKNOWN_METHOD_ERROR = -32601, // Method not found
	RPC_SERVER_PARAM_ERROR = -32602, // Invalid params
	RPC_SERVER_PROCESSING_ERROR = -32000, // Server Ошибка

	RPC_SERVER_XML_ERROR,

	RPC_CLIENT_HTTP_ERROR = -1000000,
	RPC_CLIENT_XML_ERROR,
	RPC_CLIENT_JSON_ERROR,
	RPC_CLIENT_RESPONSE_ERROR,
};

struct ValueTypeMismatch {};

Ткст FormatIso8601(Время t);
Время   ScanIso8601(const Ткст& p);
Значение  JsonRpcData(const Значение& v);

struct RawJsonText {
	Ткст json;
};

void ValueCheck(bool b);

void ValueGet(int& n, const Значение& v);
void ValueGet(int64& n, const Значение& v);
void ValueGet(bool& b, const Значение& v);
void ValueGet(Ткст& s, const Значение& v);
void ValueGet(double& x, const Значение& v);
void ValueGet(Дата& x, const Значение& v);
void ValueGet(Время& x, const Значение& v);
void ValueGet(Значение& t, const Значение& v);
void ValueGet(МассивЗнач& va, const Значение& v);
void ValueGet(МапЗнач& vm, const Значение& v);

template <class T>
void ValueGet(Массив<T>& x, const Значение& v)
{
	ValueCheck(массивЗнач_ли(v));
	МассивЗнач va = v;
	x.устСчёт(va.дайСчёт());
	for(int i = 0; i < va.дайСчёт(); i++)
		ValueGet(x[i], va[i]);
}

template <class T>
void ValueGet(Вектор<T>& x, const Значение& v)
{
	ValueCheck(массивЗнач_ли(v));
	МассивЗнач va = v;
	x.устСчёт(va.дайСчёт());
	for(int i = 0; i < va.дайСчёт(); i++)
		ValueGet(x[i], va[i]);
}

template <class T>
void ValueGet(МассивМап<Ткст, T>& x, const Значение& v)
{
	ValueCheck(мапЗнач_ли(v));
	МапЗнач vm = v;
	const Индекс<Значение>& k = vm.дайКлючи();
	МассивЗнач va = vm.дайЗначения();
	x.очисть();
	for(int i = 0; i < k.дайСчёт(); i++)
		ValueGet(x.добавь(k[i]), va[i]);
}

template <class T>
void ValueGet(ВекторМап<Ткст, T>& x, const Значение& v)
{
	ValueCheck(мапЗнач_ли(v));
	МапЗнач vm = v;
	const Индекс<Значение>& k = vm.дайКлючи();
	МассивЗнач va = vm.дайЗначения();
	x.очисть();
	for(int i = 0; i < k.дайСчёт(); i++)
		ValueGet(x.добавь(k[i]), va[i]);
}

template <class T>
void ValueGet(МассивМап<int, T>& x, const Значение& v)
{
	ValueCheck(мапЗнач_ли(v));
	МапЗнач vm = v;
	const Индекс<Значение>& k = vm.дайКлючи();
	МассивЗнач va = vm.дайЗначения();
	x.очисть();
	for(int i = 0; i < k.дайСчёт(); i++)
		ValueGet(x.добавь(atoi(какТкст(k[i]))), va[i]);
}

template <class T>
void ValueGet(ВекторМап<int, T>& x, const Значение& v)
{
	ValueCheck(мапЗнач_ли(v));
	МапЗнач vm = v;
	const Индекс<Значение>& k = vm.дайКлючи();
	МассивЗнач va = vm.дайЗначения();
	x.очисть();
	for(int i = 0; i < k.дайСчёт(); i++)
		ValueGet(x.добавь(atoi(какТкст(k[i]))), va[i]);
}

void ValuePut(Значение& v, int n);
void ValuePut(Значение& v, int64 n);
void ValuePut(Значение& v, const Ткст& s);
void ValuePut(Значение& v, const char *s);
void ValuePut(Значение& v, double x);
void ValuePut(Значение& v, bool x);
void ValuePut(Значение& v, const Дата& x);
void ValuePut(Значение& v, const Время& x);
void ValuePut(Значение& v, const Значение& t);
void ValuePut(Значение& v, const МассивЗнач& va);
void ValuePut(Значение& v, const МапЗнач& vm);
void ValuePut(Значение& v, const Json& json);

template <class T>
Значение AsXmlRpcValue(const T& x)
{
	Значение vs;
	ValuePut(vs, x);
	return vs;
}

template <class T>
void ValuePut(Значение& v, const Массив<T>& x)
{
	МассивЗнач va;
	for(int i = 0; i < x.дайСчёт(); i++)
		va.добавь(AsXmlRpcValue(x[i]));
	v = va;
}

template <class T>
void ValuePut(Значение& v, const Вектор<T>& x)
{
	МассивЗнач va;
	for(int i = 0; i < x.дайСчёт(); i++)
		va.добавь(AsXmlRpcValue(x[i]));
	v = va;
}

template <class T>
void ValuePut(Значение& v, const МассивМап<Ткст, T>& x)
{
	МапЗнач vm;
	for(int i = 0; i < x.дайСчёт(); i++)
		if(!x.отлинкован(i))
			vm.добавь(x.дайКлюч(i), AsXmlRpcValue(x[i]));
	v = vm;
}

template <class T>
void ValuePut(Значение& v, const ВекторМап<Ткст, T>& x)
{
	МапЗнач vm;
	for(int i = 0; i < x.дайСчёт(); i++)
		if(!x.отлинкован(i))
			vm.добавь(x.дайКлюч(i), AsXmlRpcValue(x[i]));
	v = vm;
}

template <class T>
void ValuePut(Значение& v, const МассивМап<int, T>& x)
{
	МапЗнач vm;
	for(int i = 0; i < x.дайСчёт(); i++)
		if(!x.отлинкован(i))
			vm.добавь(какТкст(x.дайКлюч(i)), AsXmlRpcValue(x[i]));
	v = vm;
}

template <class T>
void ValuePut(Значение& v, const ВекторМап<int, T>& x)
{
	МапЗнач vm;
	for(int i = 0; i < x.дайСчёт(); i++)
		if(!x.отлинкован(i))
			vm.добавь(какТкст(x.дайКлюч(i)), AsXmlRpcValue(x[i]));
	v = vm;
}

Значение      ParseXmlRpcValue(ПарсерРяр& p);
Значение      ParseXmlRpcParam(ПарсерРяр& p);
МассивЗнач ParseXmlRpcParams(ПарсерРяр& p);

struct RpcGet {
	Значение v;
	
	template <class T>
	operator T() const { T x; ValueGet(x, v); return x; }

	template <class T>
	T дай() const { T x; ValueGet(x, v); return x; } // Ugly workaround for MSC compiler bug

	Ткст вТкст() const { return v.вТкст(); }
};

struct XmlRpcDo;

struct RpcData {
	Ткст     peeraddr;
	МассивЗнач in;
	int        ii;
	МапЗнач   in_map;
	МассивЗнач out;
	МапЗнач   out_map;

	int      GetRemainingCount() const  { return in.дайСчёт() - ii; }
	bool     естьЕщё() const            { return GetRemainingCount() > 0; }

	Значение дай()                         { if(ii >= in.дайСчёт()) return Значение(); return in[ii++]; }
	
	template <class T>
	RpcData& operator>>(T& x)           { ValueGet(x, дай()); return *this; }
	
	RpcGet   operator++(int)            { RpcGet h; h.v = дай(); return h; }
	
	RpcGet   operator[](const char *ид) { RpcGet h; h.v = in_map[ид]; return h; }
	
	template <class T>
	RpcData& operator<<(const T& x)     { ПРОВЕРЬ(out_map.дайСчёт() == 0); Значение v; ValuePut(v, x); out.добавь(v); return *this; }

	template <class T>
	void        уст(int i, const T& x)  { ПРОВЕРЬ(out_map.дайСчёт() == 0); Значение v; ValuePut(v, x); out.уст(i, v); }

	template <class T>
	void operator=(const T& x)          { out.очисть(); operator<<(x); }

	void        переустанов()                 { in.очисть(); out.очисть(); ii = 0; }
	
	void        EndRpc();

	RpcData() { ii = 0; rpc = NULL; }

private:
	friend struct XmlRpcDo;

	XmlRpcDo *rpc;
};

Ткст FormatXmlRpcValue(const Значение& _v, bool supports_i8);
Ткст FormatXmlRpcParam(const Значение& param, bool supports_i8);
Ткст FormatXmlRpcParams(const МассивЗнач& params, bool supports_i8);

Ткст FormatXmlRpcError(int code, const char *text);

void   регистрируй(const char *имя, void (*method)(RpcData&), const char *группа = NULL);

#define RPC_METHOD(имя) void имя(RpcData& rpc); ИНИЦБЛОК { регистрируй(#имя, имя); } void имя(RpcData& rpc)
#define RPC_GMETHOD(имя, группа) void имя(RpcData& rpc); ИНИЦБЛОК { регистрируй(#имя, имя, группа); } void имя(RpcData& rpc)

struct RpcError {
	int    code;
	Ткст text;
};

void   SetRpcMethodFilter(Ткст (*filter)(const Ткст& methodname));
bool   RpcPerform(TcpSocket& http, const char *группа);
bool   RpcServerLoop(int port, const char *группа = NULL);

void   ThrowRpcError(int code, const char *s);
void   ThrowRpcError(const char *s);

class RpcRequest : public HttpRequest {
	bool       shorted;
	RpcData    данные;
	Ткст     method;
	Ткст     Ошибка;
	Ткст     faultString;
	int        faultCode;
	bool       shouldExecute;
	bool       json, notification;
	bool       supports_i8;
	Ткст     protocol_version;
	void       иниц();

public:
	RpcRequest& Method(const char *имя);

	template <class T>
	RpcRequest& operator<<(const T& x)             { данные << x; return *this; }
	template <class T>
	void        уст(int i, const T& x)             { данные.уст(i, x); }
	template <class T>
	RpcRequest& Named(const char *ид, const T& x)  { данные.out_map.добавь(ид, x); return *this; }

	RpcGet      выполни();
	RpcGet      Retry();

	template <class T>
	bool operator>>(T& x)                          { if(выполни().v.ошибка_ли()) return false;
	                                                 try { данные >> x; } catch(ValueTypeMismatch) { return false; } return true; }

	RpcRequest& operator()(const char *method)     { Method(method); return *this; }

#define E__Templ(I)  class КОМБИНИРУЙ(T, I)
#define E__Decl(I)   const КОМБИНИРУЙ(T, I)& КОМБИНИРУЙ(p, I)
#define E__Param(I)  *this << КОМБИНИРУЙ(p, I)
#define E__Body(I) \
	template <__List##I(E__Templ)> \
	RpcRequest& operator()(const char *method, __List##I(E__Decl)) { \
		Method(method); \
		__List##I(E__Param); \
		return *this; \
	}

	__Expand20(E__Body)

#undef E__Templ
#undef E__Decl
#undef E__Param
#undef E__Body

	Ткст GetFaultString() const                               { return faultString; }
	int    GetFaultCode() const                                 { return faultCode; }
	Ткст дайОш() const                                     { return Ошибка; }
	Ткст GetMethod() const                                    { return method; }
	void   сотриОш();
	
	RpcRequest& Url(const char *url);
	
	RpcRequest& JsonRpc()                                       { json = true; return *this; }
	RpcRequest& Notification()                                  { notification = true; return *this; }
	RpcRequest& SupportsI8()                                    { supports_i8 = true; protocol_version = "2.1"; return *this; }
	
	RpcRequest(const char *url);
	RpcRequest();
};

struct XmlRpcRequest : RpcRequest {
	XmlRpcRequest(const char *url) : RpcRequest(url) {}
	XmlRpcRequest() {}
};

struct JsonRpcRequest : RpcRequest {
	JsonRpcRequest(const char *url) : RpcRequest(url) { JsonRpc(); }
	JsonRpcRequest() { JsonRpc(); }
};

struct JsonRpcRequestNamed : RpcRequest {
	template <class T>
	JsonRpcRequestNamed& operator()(const char *ид, const T& x)   { Named(ид, x); return *this; }
	JsonRpcRequestNamed& operator()(const char *method)           { Method(method); return *this; }
	
	JsonRpcRequestNamed(const char *url) : RpcRequest(url)        { JsonRpc(); }
	JsonRpcRequestNamed()                                         { JsonRpc(); }
};

void LogRpcRequests(bool b = true);
void LogRpcRequestsCompress(bool b);

void SetRpcServerTrace(Поток& s, int level = 1);
void SetRpcServerTraceCompress(bool compress);
void StopRpcServerTrace();
void SuppressRpcServerTraceForMethodCall();

#include "legacy.h"

}

#endif
