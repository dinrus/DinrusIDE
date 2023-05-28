// NOTE: Because of clash with Windows rpc.h, RKod/Core.h has to be included before this header
// For the same reason implementation files are in 'imp' subdirectory

#ifndef Rpc_Rpc_h
#define Rpc_Rpc_h

namespace ДинрусРНЦП {

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

проц ValueCheck(бул b);

проц ValueGet(цел& n, const Значение& v);
проц ValueGet(дол& n, const Значение& v);
проц ValueGet(бул& b, const Значение& v);
проц ValueGet(Ткст& s, const Значение& v);
проц ValueGet(дво& x, const Значение& v);
проц ValueGet(Дата& x, const Значение& v);
проц ValueGet(Время& x, const Значение& v);
проц ValueGet(Значение& t, const Значение& v);
проц ValueGet(МассивЗнач& va, const Значение& v);
проц ValueGet(МапЗнач& vm, const Значение& v);

template <class T>
проц ValueGet(Массив<T>& x, const Значение& v)
{
	ValueCheck(массивЗнач_ли(v));
	МассивЗнач va = v;
	x.устСчёт(va.дайСчёт());
	for(цел i = 0; i < va.дайСчёт(); i++)
		ValueGet(x[i], va[i]);
}

template <class T>
проц ValueGet(Вектор<T>& x, const Значение& v)
{
	ValueCheck(массивЗнач_ли(v));
	МассивЗнач va = v;
	x.устСчёт(va.дайСчёт());
	for(цел i = 0; i < va.дайСчёт(); i++)
		ValueGet(x[i], va[i]);
}

template <class T>
проц ValueGet(МассивМап<Ткст, T>& x, const Значение& v)
{
	ValueCheck(мапЗнач_ли(v));
	МапЗнач vm = v;
	const Индекс<Значение>& k = vm.дайКлючи();
	МассивЗнач va = vm.дайЗначения();
	x.очисть();
	for(цел i = 0; i < k.дайСчёт(); i++)
		ValueGet(x.добавь(k[i]), va[i]);
}

template <class T>
проц ValueGet(ВекторМап<Ткст, T>& x, const Значение& v)
{
	ValueCheck(мапЗнач_ли(v));
	МапЗнач vm = v;
	const Индекс<Значение>& k = vm.дайКлючи();
	МассивЗнач va = vm.дайЗначения();
	x.очисть();
	for(цел i = 0; i < k.дайСчёт(); i++)
		ValueGet(x.добавь(k[i]), va[i]);
}

template <class T>
проц ValueGet(МассивМап<цел, T>& x, const Значение& v)
{
	ValueCheck(мапЗнач_ли(v));
	МапЗнач vm = v;
	const Индекс<Значение>& k = vm.дайКлючи();
	МассивЗнач va = vm.дайЗначения();
	x.очисть();
	for(цел i = 0; i < k.дайСчёт(); i++)
		ValueGet(x.добавь(atoi(какТкст(k[i]))), va[i]);
}

template <class T>
проц ValueGet(ВекторМап<цел, T>& x, const Значение& v)
{
	ValueCheck(мапЗнач_ли(v));
	МапЗнач vm = v;
	const Индекс<Значение>& k = vm.дайКлючи();
	МассивЗнач va = vm.дайЗначения();
	x.очисть();
	for(цел i = 0; i < k.дайСчёт(); i++)
		ValueGet(x.добавь(atoi(какТкст(k[i]))), va[i]);
}

проц ValuePut(Значение& v, цел n);
проц ValuePut(Значение& v, дол n);
проц ValuePut(Значение& v, const Ткст& s);
проц ValuePut(Значение& v, кткст0 s);
проц ValuePut(Значение& v, дво x);
проц ValuePut(Значение& v, бул x);
проц ValuePut(Значение& v, const Дата& x);
проц ValuePut(Значение& v, const Время& x);
проц ValuePut(Значение& v, const Значение& t);
проц ValuePut(Значение& v, const МассивЗнач& va);
проц ValuePut(Значение& v, const МапЗнач& vm);
проц ValuePut(Значение& v, const Json& json);

template <class T>
Значение AsXmlRpcValue(const T& x)
{
	Значение vs;
	ValuePut(vs, x);
	return vs;
}

template <class T>
проц ValuePut(Значение& v, const Массив<T>& x)
{
	МассивЗнач va;
	for(цел i = 0; i < x.дайСчёт(); i++)
		va.добавь(AsXmlRpcValue(x[i]));
	v = va;
}

template <class T>
проц ValuePut(Значение& v, const Вектор<T>& x)
{
	МассивЗнач va;
	for(цел i = 0; i < x.дайСчёт(); i++)
		va.добавь(AsXmlRpcValue(x[i]));
	v = va;
}

template <class T>
проц ValuePut(Значение& v, const МассивМап<Ткст, T>& x)
{
	МапЗнач vm;
	for(цел i = 0; i < x.дайСчёт(); i++)
		if(!x.отлинкован(i))
			vm.добавь(x.дайКлюч(i), AsXmlRpcValue(x[i]));
	v = vm;
}

template <class T>
проц ValuePut(Значение& v, const ВекторМап<Ткст, T>& x)
{
	МапЗнач vm;
	for(цел i = 0; i < x.дайСчёт(); i++)
		if(!x.отлинкован(i))
			vm.добавь(x.дайКлюч(i), AsXmlRpcValue(x[i]));
	v = vm;
}

template <class T>
проц ValuePut(Значение& v, const МассивМап<цел, T>& x)
{
	МапЗнач vm;
	for(цел i = 0; i < x.дайСчёт(); i++)
		if(!x.отлинкован(i))
			vm.добавь(какТкст(x.дайКлюч(i)), AsXmlRpcValue(x[i]));
	v = vm;
}

template <class T>
проц ValuePut(Значение& v, const ВекторМап<цел, T>& x)
{
	МапЗнач vm;
	for(цел i = 0; i < x.дайСчёт(); i++)
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
	цел        ii;
	МапЗнач   in_map;
	МассивЗнач out;
	МапЗнач   out_map;

	цел      GetRemainingCount() const  { return in.дайСчёт() - ii; }
	бул     естьЕщё() const            { return GetRemainingCount() > 0; }

	Значение дай()                         { if(ii >= in.дайСчёт()) return Значение(); return in[ii++]; }
	
	template <class T>
	RpcData& operator>>(T& x)           { ValueGet(x, дай()); return *this; }
	
	RpcGet   operator++(цел)            { RpcGet h; h.v = дай(); return h; }
	
	RpcGet   operator[](кткст0 ид) { RpcGet h; h.v = in_map[ид]; return h; }
	
	template <class T>
	RpcData& operator<<(const T& x)     { ПРОВЕРЬ(out_map.дайСчёт() == 0); Значение v; ValuePut(v, x); out.добавь(v); return *this; }

	template <class T>
	проц        уст(цел i, const T& x)  { ПРОВЕРЬ(out_map.дайСчёт() == 0); Значение v; ValuePut(v, x); out.уст(i, v); }

	template <class T>
	проц operator=(const T& x)          { out.очисть(); operator<<(x); }

	проц        переустанов()                 { in.очисть(); out.очисть(); ii = 0; }
	
	проц        EndRpc();

	RpcData() { ii = 0; rpc = NULL; }

private:
	friend struct XmlRpcDo;

	XmlRpcDo *rpc;
};

Ткст FormatXmlRpcValue(const Значение& _v, бул supports_i8);
Ткст FormatXmlRpcParam(const Значение& param, бул supports_i8);
Ткст FormatXmlRpcParams(const МассивЗнач& params, бул supports_i8);

Ткст FormatXmlRpcError(цел код_, кткст0 text);

проц   регистрируй(кткст0 имя, проц (*method)(RpcData&), кткст0 группа = NULL);

#define RPC_METHOD(имя) проц имя(RpcData& rpc); ИНИЦБЛОК { регистрируй(#имя, имя); } проц имя(RpcData& rpc)
#define RPC_GMETHOD(имя, группа) проц имя(RpcData& rpc); ИНИЦБЛОК { регистрируй(#имя, имя, группа); } проц имя(RpcData& rpc)

struct RpcError {
	цел    код_;
	Ткст text;
};

проц   SetRpcMethodFilter(Ткст (*фильтр)(const Ткст& methodname));
бул   RpcPerform(TcpSocket& http, кткст0 группа);
бул   RpcServerLoop(цел port, кткст0 группа = NULL);

проц   ThrowRpcError(цел код_, кткст0 s);
проц   ThrowRpcError(кткст0 s);

class RpcRequest : public HttpRequest {
	бул       shorted;
	RpcData    данные;
	Ткст     method;
	Ткст     Ошибка;
	Ткст     faultString;
	цел        faultCode;
	бул       shouldExecute;
	бул       json, notification;
	бул       supports_i8;
	Ткст     protocol_version;
	проц       иниц();

public:
	RpcRequest& Method(кткст0 имя);

	template <class T>
	RpcRequest& operator<<(const T& x)             { данные << x; return *this; }
	template <class T>
	проц        уст(цел i, const T& x)             { данные.уст(i, x); }
	template <class T>
	RpcRequest& Named(кткст0 ид, const T& x)  { данные.out_map.добавь(ид, x); return *this; }

	RpcGet      выполни();
	RpcGet      Retry();

	template <class T>
	бул operator>>(T& x)                          { if(выполни().v.ошибка_ли()) return false;
	                                                 try { данные >> x; } catch(ValueTypeMismatch) { return false; } return true; }

	RpcRequest& operator()(кткст0 method)     { Method(method); return *this; }

#define E__Templ(I)  class КОМБИНИРУЙ(T, I)
#define E__Decl(I)   const КОМБИНИРУЙ(T, I)& КОМБИНИРУЙ(p, I)
#define E__Param(I)  *this << КОМБИНИРУЙ(p, I)
#define E__Body(I) \
	template <__List##I(E__Templ)> \
	RpcRequest& operator()(кткст0 method, __List##I(E__Decl)) { \
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
	цел    GetFaultCode() const                                 { return faultCode; }
	Ткст дайОш() const                                     { return Ошибка; }
	Ткст GetMethod() const                                    { return method; }
	проц   сотриОш();
	
	RpcRequest& Url(кткст0 url);
	
	RpcRequest& JsonRpc()                                       { json = true; return *this; }
	RpcRequest& Notification()                                  { notification = true; return *this; }
	RpcRequest& SupportsI8()                                    { supports_i8 = true; protocol_version = "2.1"; return *this; }
	
	RpcRequest(кткст0 url);
	RpcRequest();
};

struct XmlRpcRequest : RpcRequest {
	XmlRpcRequest(кткст0 url) : RpcRequest(url) {}
	XmlRpcRequest() {}
};

struct JsonRpcRequest : RpcRequest {
	JsonRpcRequest(кткст0 url) : RpcRequest(url) { JsonRpc(); }
	JsonRpcRequest() { JsonRpc(); }
};

struct JsonRpcRequestNamed : RpcRequest {
	template <class T>
	JsonRpcRequestNamed& operator()(кткст0 ид, const T& x)   { Named(ид, x); return *this; }
	JsonRpcRequestNamed& operator()(кткст0 method)           { Method(method); return *this; }
	
	JsonRpcRequestNamed(кткст0 url) : RpcRequest(url)        { JsonRpc(); }
	JsonRpcRequestNamed()                                         { JsonRpc(); }
};

проц LogRpcRequests(бул b = true);
проц LogRpcRequestsCompress(бул b);

проц SetRpcServerTrace(Поток& s, цел level = 1);
проц SetRpcServerTraceCompress(бул compress);
проц StopRpcServerTrace();
проц SuppressRpcServerTraceForMethodCall();

#include "legacy.h"

}

#endif
