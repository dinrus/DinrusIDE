#include <DinrusPro/DinrusCore.h>
#include <RKod/Rpc/Rpc.h>

#define LLOG(x)  //  DLOG(x)

typedef проц (*RpcFnPtr)(RpcData&);

static СтатическийСтопор RpcMapMutex;

ВекторМап<Ткст, RpcFnPtr>& RpcMap(кткст0 группа)
{
	static ВекторМап<Ткст, ВекторМап< Ткст, проц (*)(RpcData&) > > mm;
	return mm.дайДобавь(группа);
}

проц регистрируй(кткст0 имя, проц (*method)(RpcData&), кткст0 группа)
{
	Стопор::Замок __(RpcMapMutex);
	RpcMap(группа).добавь(имя, method);
}

RpcFnPtr RpcMapGet(кткст0 группа, кткст0 имя)
{
	Стопор::Замок __(RpcMapMutex);
	return RpcMap(группа).дай(имя, NULL);
}

Ткст (*sRpcMethodFilter)(const Ткст& methodname);

проц SetRpcMethodFilter(Ткст (*фильтр)(const Ткст& methodname))
{
	sRpcMethodFilter = фильтр;
}

проц ThrowRpcError(цел код_, кткст0 s)
{
	RpcError e;
	e.код_ = код_;
	e.text = s;
	throw e;
}

проц ThrowRpcError(кткст0 s)
{
	ThrowRpcError(RPC_SERVER_PROCESSING_ERROR, s);
}

static Поток *rpc_trace, *suppressed_rpc_trace;
static цел rpc_trace_level;
static бул rpc_trace_compress = true;

проц SetRpcServerTrace(Поток& s, цел level)
{
	rpc_trace = &s;
	rpc_trace_level = level;
}

проц SetRpcServerTraceCompress(бул compress)
{
	rpc_trace_compress = compress;
}

проц StopRpcServerTrace()
{
	rpc_trace = NULL;
}

проц SuppressRpcServerTraceForMethodCall()
{
	suppressed_rpc_trace = rpc_trace;
	rpc_trace = NULL;
}

бул CallRpcMethod(RpcData& данные, кткст0 группа, Ткст methodname, const Ткст& request)
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
	проц (*фн)(RpcData&) = RpcMapGet(группа, methodname);
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
	бул       json;
	бул       shorted;

	Ткст XmlResult();
	Ткст DoXmlRpc();
	Ткст ДжейсонRpcError(цел код_, кткст0 text, const Значение& ид);
	Ткст ДжейсонResult();
	Ткст ProcessДжейсонRpc(const Значение& v);
	Ткст DoДжейсонRpc();
	Ткст RpcExecute();
	проц   RpcResponse(const Ткст& r);
	проц   EndRpc();
	бул   Perform();
	
	XmlRpcDo(TcpSocket& http, кткст0 группа);
};

XmlRpcDo::XmlRpcDo(TcpSocket& http, кткст0 группа)
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
		return FormatXmlRpcError(e.код_, e.text);
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

Ткст XmlRpcDo::ДжейсонRpcError(цел код_, кткст0 text, const Значение& ид)
{
	Джейсон m;
	m("jsonrpc", "2.0");
	МапЗнач err;
	err.добавь("код_", код_);
	err.добавь("message", text);
	m("Ошибка", err);
	m("ид", ид);
	return m;
}

Ткст XmlRpcDo::ДжейсонResult()
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
				return ДжейсонRpcError(RPC_SERVER_PROCESSING_ERROR, "Processing Ошибка: " + e, ид);
			}
			result = ДжейсонRpcData(va[0]);
		}
		Джейсон json;
		json("jsonrpc", "2.0");
		if(result.является<RawДжейсонText>())
			json.CatRaw("result", result.To<RawДжейсонText>().json);
		else
			json("result", result);
		json("ид", ид);
		return json;
	}
	return ДжейсонRpcError(RPC_UNKNOWN_METHOD_ERROR, "Method not found", ид);
}

Ткст XmlRpcDo::ProcessДжейсонRpc(const Значение& v)
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
			return ДжейсонResult();
		}
		return ДжейсонRpcError(RPC_UNKNOWN_METHOD_ERROR, "Method not found", ид);
	}
	catch(RpcError e) {
		LLOG("Processing Ошибка: " << e.text);
		if(rpc_trace)
			*rpc_trace << "Processing Ошибка: " << e.text << '\n';
		return ДжейсонRpcError(e.код_, e.text, ид);
	}
	catch(ValueTypeMismatch) {
		LLOG("ValueTypeMismatch at parameter " << данные.ii);
		if(rpc_trace)
			*rpc_trace << "ValueTypeMismatch at parameter " << данные.ii << '\n';
		return ДжейсонRpcError(RPC_SERVER_PARAM_ERROR, "Invalid params", ид);
	}
}

Ткст XmlRpcDo::DoДжейсонRpc()
{
	try {
		Значение v = разбериДжейсон(request);
		if(v.является<МапЗнач>())
			return ProcessДжейсонRpc(v);
		if(v.является<МассивЗнач>()) {
			МассивДжейсон a;
			for(цел i = 0; i < v.дайСчёт(); i++)
				a.CatRaw(ProcessДжейсонRpc(v[i]));
			return v.дайСчёт() ? ~a : Ткст();
		}
	}
	catch(СиПарсер::Ошибка e) {}	
	return какДжейсон(ДжейсонRpcError(RPC_SERVER_JSON_ERROR, "Parse Ошибка", Null));
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
		r = DoДжейсонRpc();
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

проц XmlRpcDo::RpcResponse(const Ткст& r)
{
	LLOG("--------- Server response:\n" << r << "=============");
	Ткст response;
	Ткст ts = WwwFormat(дайВремяУВИ());
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

проц XmlRpcDo::EndRpc()
{
	RpcResponse(json ? ДжейсонResult() : XmlResult());
}

проц RpcData::EndRpc()
{
	if(rpc) {
		rpc->EndRpc();
		rpc = NULL;
	}
}

бул XmlRpcDo::Perform()
{
	LLOG("=== Accepted connection ===================================================");
	HttpHeader hdr;
	if(hdr.читай(http) && hdr.GetMethod() == "POST") {
		цел len = atoi(hdr["content-length"]);
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

бул RpcPerform(TcpSocket& http, кткст0 группа)
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

бул RpcServerLoop(цел port, кткст0 группа)
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


