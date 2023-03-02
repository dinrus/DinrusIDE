#include <Core/Core.h>
#include <Core/Rpc/Rpc.h>

#define LLOG(x)  //  DLOG(x)

namespace РНЦПДинрус {

typedef void (*RpcFnPtr)(RpcData&);

static СтатическийСтопор RpcMapMutex;

ВекторМап<Ткст, RpcFnPtr>& RpcMap(const char *группа)
{
	static ВекторМап<Ткст, ВекторМап< Ткст, void (*)(RpcData&) > > mm;
	return mm.дайДобавь(группа);
}

void регистрируй(const char *имя, void (*method)(RpcData&), const char *группа)
{
	Стопор::Замок __(RpcMapMutex);
	RpcMap(группа).добавь(имя, method);
}

RpcFnPtr RpcMapGet(const char *группа, const char *имя)
{
	Стопор::Замок __(RpcMapMutex);
	return RpcMap(группа).дай(имя, NULL);
}

Ткст (*sRpcMethodFilter)(const Ткст& methodname);

void SetRpcMethodFilter(Ткст (*filter)(const Ткст& methodname))
{
	sRpcMethodFilter = filter;
}

void ThrowRpcError(int code, const char *s)
{
	RpcError e;
	e.code = code;
	e.text = s;
	throw e;
}

void ThrowRpcError(const char *s)
{
	ThrowRpcError(RPC_SERVER_PROCESSING_ERROR, s);
}

static Поток *rpc_trace, *suppressed_rpc_trace;
static int rpc_trace_level;
static bool rpc_trace_compress = true;

void SetRpcServerTrace(Поток& s, int level)
{
	rpc_trace = &s;
	rpc_trace_level = level;
}

void SetRpcServerTraceCompress(bool compress)
{
	rpc_trace_compress = compress;
}

void StopRpcServerTrace()
{
	rpc_trace = NULL;
}

void SuppressRpcServerTraceForMethodCall()
{
	suppressed_rpc_trace = rpc_trace;
	rpc_trace = NULL;
}

bool CallRpcMethod(RpcData& данные, const char *группа, Ткст methodname, const Ткст& request)
{
	LLOG("method имя: " << methodname);
	if(sRpcMethodFilter)
		methodname = (*sRpcMethodFilter)(methodname);
	if(rpc_trace) {
		*rpc_trace << "RPC Request:\n";
		if(rpc_trace_compress)
			*rpc_trace << сожмиЛог(request);
		else
			*rpc_trace << request;
		*rpc_trace << '\n';
	}
	void (*фн)(RpcData&) = RpcMapGet(группа, methodname);
	if(!фн)
		return false;
	(*фн)(данные);
	return true;
}

struct XmlRpcDo {
	TcpSocket& http;
	RpcData    данные;
	Ткст     request;
	Ткст     группа;
	Ткст     methodname;
	Значение      ид;
	bool       json;
	bool       shorted;

	Ткст XmlResult();
	Ткст DoXmlRpc();
	Ткст JsonRpcError(int code, const char *text, const Значение& ид);
	Ткст JsonResult();
	Ткст ProcessJsonRpc(const Значение& v);
	Ткст DoJsonRpc();
	Ткст RpcExecute();
	void   RpcResponse(const Ткст& r);
	void   EndRpc();
	bool   Perform();
	
	XmlRpcDo(TcpSocket& http, const char *группа);
};

XmlRpcDo::XmlRpcDo(TcpSocket& http, const char *группа)
:	http(http), группа(группа)
{
	shorted = false;
}

Ткст XmlRpcDo::XmlResult()
{
	Ткст r = XmlHeader();
	r << "<methodResponse>\r\n";
	if(массивЗнач_ли(данные.out)) {
		МассивЗнач va = данные.out;
		if(va.дайСчёт() && ошибка_ли(va[0])) {
			LLOG("ProcessingError");
			Ткст e = дайТекстОш(данные.out[0]);
			if(rpc_trace)
				*rpc_trace << "Processing Ошибка: " << e << '\n';
			return FormatXmlRpcError(RPC_SERVER_PROCESSING_ERROR, "Processing Ошибка: " + e);
		}
		r << FormatXmlRpcParams(данные.out, false);
	}
	r << "\r\n</methodResponse>\r\n";
	return r;
}

Ткст XmlRpcDo::DoXmlRpc()
{
	ПарсерРяр p(request);
	try {
		p.ReadPI();
		p.передайТэг("methodCall");
		p.передайТэг("methodName");
		methodname = p.читайТекст();
		p.передайКонец();
		данные.in = ParseXmlRpcParams(p);
		if(!CallRpcMethod(данные, группа, methodname, request))
			return FormatXmlRpcError(RPC_UNKNOWN_METHOD_ERROR, "\'" + methodname + "\' method is unknown");
		if(!данные.rpc && !shorted)
			return Null;
		return XmlResult();
	}
	catch(RpcError e) {
		LLOG("Processing Ошибка: " << e.text);
		if(rpc_trace)
			*rpc_trace << "Processing Ошибка: " << e.text << '\n';
		return FormatXmlRpcError(e.code, e.text);
	}
	catch(ОшибкаРяр e) {
		LLOG("ОшибкаРяр " << e << ": " << p.дайУк());
		if(rpc_trace)
			*rpc_trace << "ОшибкаРяр: " << e << '\n';
		return FormatXmlRpcError(RPC_SERVER_XML_ERROR, "XML Ошибка: " + e);
	}
	catch(ValueTypeMismatch) {
		LLOG("ValueTypeMismatch at parameter " << данные.ii);
		if(rpc_trace)
			*rpc_trace << "ValueTypeMismatch at parameter " << данные.ii << '\n';
		return FormatXmlRpcError(RPC_SERVER_PARAM_ERROR, "Parameter mismatch at parameter " + какТкст(данные.ii));
	}
	return Null;
}

Ткст XmlRpcDo::JsonRpcError(int code, const char *text, const Значение& ид)
{
	Json m;
	m("jsonrpc", "2.0");
	МапЗнач err;
	err.добавь("code", code);
	err.добавь("message", text);
	m("Ошибка", err);
	m("ид", ид);
	return m;
}

Ткст XmlRpcDo::JsonResult()
{
	if(массивЗнач_ли(данные.out)) {
		МассивЗнач va = данные.out;
		Значение result = Null;
		if(va.дайСчёт()) {
			if(ошибка_ли(va[0])) {
				LLOG("ProcessingError");
				Ткст e = дайТекстОш(данные.out[0]);
				if(rpc_trace)
					*rpc_trace << "Processing Ошибка: " << e << '\n';
				return JsonRpcError(RPC_SERVER_PROCESSING_ERROR, "Processing Ошибка: " + e, ид);
			}
			result = JsonRpcData(va[0]);
		}
		Json json;
		json("jsonrpc", "2.0");
		if(result.является<RawJsonText>())
			json.CatRaw("result", result.To<RawJsonText>().json);
		else
			json("result", result);
		json("ид", ид);
		return json;
	}
	return JsonRpcError(RPC_UNKNOWN_METHOD_ERROR, "Method not found", ид);
}

Ткст XmlRpcDo::ProcessJsonRpc(const Значение& v)
{
	LLOG("Parsed JSON request: " << v);
	ид = v["ид"];
	methodname = какТкст(v["method"]);
	Значение param = v["params"];
	if(param.является<МапЗнач>())
		данные.in_map = param;
	else
		данные.in = param;
	try {
		if(CallRpcMethod(данные, группа, methodname, request)) {
			if(!данные.rpc && !shorted)
				return Null;
			return JsonResult();
		}
		return JsonRpcError(RPC_UNKNOWN_METHOD_ERROR, "Method not found", ид);
	}
	catch(RpcError e) {
		LLOG("Processing Ошибка: " << e.text);
		if(rpc_trace)
			*rpc_trace << "Processing Ошибка: " << e.text << '\n';
		return JsonRpcError(e.code, e.text, ид);
	}
	catch(ValueTypeMismatch) {
		LLOG("ValueTypeMismatch at parameter " << данные.ii);
		if(rpc_trace)
			*rpc_trace << "ValueTypeMismatch at parameter " << данные.ii << '\n';
		return JsonRpcError(RPC_SERVER_PARAM_ERROR, "Invalid params", ид);
	}
}

Ткст XmlRpcDo::DoJsonRpc()
{
	try {
		Значение v = ParseJSON(request);
		if(v.является<МапЗнач>())
			return ProcessJsonRpc(v);
		if(v.является<МассивЗнач>()) {
			JsonArray a;
			for(int i = 0; i < v.дайСчёт(); i++)
				a.CatRaw(ProcessJsonRpc(v[i]));
			return v.дайСчёт() ? ~a : Ткст();
		}
	}
	catch(СиПарсер::Ошибка e) {}	
	return AsJSON(JsonRpcError(RPC_SERVER_JSON_ERROR, "Parse Ошибка", Null));
}

Ткст XmlRpcDo::RpcExecute()
{
	json = false;
	try {
		СиПарсер p(request);
		json = p.сим('{') || p.сим('[');
	}
	catch(СиПарсер::Ошибка) {}

	Ткст r;
	Ткст mn;
	ТаймСтоп tm;
	if(json)
		r = DoJsonRpc();
	else
	    r = DoXmlRpc();

	if(rpc_trace) {
		mn << " (" << tm.прошло() << " ms)";
		if(rpc_trace_level == 0)
			*rpc_trace << "Rpc " << mn << " finished OK \n";
		else {
			if(rpc_trace_compress)
				*rpc_trace << "Rpc " << mn << " response:\n" << сожмиЛог(r) << '\n';
			else
				*rpc_trace << "Rpc " << mn << " response:\n" << r << '\n';
		}
	}
	if(suppressed_rpc_trace) {
		if(!rpc_trace)
			rpc_trace = suppressed_rpc_trace;
		suppressed_rpc_trace = NULL;
	}
	return r;
}

void XmlRpcDo::RpcResponse(const Ткст& r)
{
	LLOG("--------- Server response:\n" << r << "=============");
	Ткст response;
	Ткст ts = WwwFormat(GetUtcTime());
	response <<
		"HTTP/1.0 200 OK\r\n"
		"Дата: " << ts << "\r\n"
		"Server: U++ RPC server\r\n"
		"Content-длина: " << r.дайСчёт() << "\r\n"
		"Connection: close\r\n"
		"Content-Type: application/" << (json ? "json" : "xml") << "\r\n\r\n"
		<< r;
	LLOG(response);
	if(r.дайСчёт())
		http.помести(response);
}

void XmlRpcDo::EndRpc()
{
	RpcResponse(json ? JsonResult() : XmlResult());
}

void RpcData::EndRpc()
{
	if(rpc) {
		rpc->EndRpc();
		rpc = NULL;
	}
}

bool XmlRpcDo::Perform()
{
	LLOG("=== Accepted connection ===================================================");
	HttpHeader hdr;
	if(hdr.читай(http) && hdr.GetMethod() == "POST") {
		int len = atoi(hdr["content-length"]);
		if(len > 0 && len < 1024 * 1024 * 1024) {
			request = http.дайВсе(len);
			данные.peeraddr = http.GetPeerAddr();
			данные.rpc = this;
			Ткст r = RpcExecute();
			if(данные.rpc)
				RpcResponse(r);
			return true;
		}
	}
	http.помести("HTTP/1.0 400 Bad request\r\n"
	         "Server: U++\r\n\r\n");
	return false;
}

bool RpcPerform(TcpSocket& http, const char *группа)
{
	return XmlRpcDo(http, группа).Perform();
}

Ткст RpcExecuteShorted(const Ткст& request_)
{
	HttpRequest dummy;
	XmlRpcDo h(dummy, "");
	h.request = request_;
	h.shorted = true;
	h.данные.peeraddr = "127.0.0.1";
	return h.RpcExecute();
}

bool RpcServerLoop(int port, const char *группа)
{
	TcpSocket rpc;
	if(!rpc.Listen(port, 5))
		return false;
	for(;;) {
		TcpSocket http;
		if(http.прими(rpc))
			RpcPerform(http, группа);
	}
}

}
