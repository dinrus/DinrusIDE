#include <DinrusC/Core.h>
#include <DinrusC/Rpc/Rpc.h>

namespace РНЦПДинрус {

#define LLOG(x)  // LOG(x)

static bool sLogRpcCalls;
static bool sLogRpcCallsCompress = true;

void LogRpcRequests(bool b)
{
	sLogRpcCalls = b;
}

void LogRpcRequestsCompress(bool b)
{
	sLogRpcCallsCompress = b;
}

RpcRequest& RpcRequest::Url(const char *url)
{
	shorted = true;
	if(url && *url) {
		HttpRequest::Url(url);
		shorted = false;
	}
	shouldExecute = true;
	return *this;
}

RpcRequest& RpcRequest::Method(const char *имя)
{
	shouldExecute = true;
	method = имя;
	данные.переустанов();
	Ошибка.очисть();
	return *this;
}

void RpcRequest::иниц()
{
	ContentType("text/xml");
	RequestTimeout(30000);
	MaxRetries(0);
	json = false;
	supports_i8 = false;
}

RpcRequest::RpcRequest(const char *url)
{
	Url(url);
	иниц();
}

RpcRequest::RpcRequest()
{
	Url(NULL);
	иниц();
}

Ткст XmlRpcExecute(const Ткст& request, const char *группа, const char *peeraddr, bool& json);
Ткст XmlRpcExecute(const Ткст& request, const char *группа, const char *peeraddr);

RpcGet RpcRequest::Retry()
{
	сотриОш();
	shouldExecute = true;
	return выполни();
}

Значение JsonRpcData(const Значение& v)
{
	if(датаВремя_ли(v) && !пусто_ли(v))
		return FormatIso8601(v);
	return v;
}

Ткст RpcExecuteShorted(const Ткст& request_);

RpcGet RpcRequest::выполни()
{
	if(!shouldExecute)
		return RpcGet();
	shouldExecute = false;
	Ткст request;
	if(json) {
		ContentType("application/json");
		static Атомар ид;
		Json json;
		json("jsonrpc", "2.0")
		    ("method", method);
		if(данные.out.дайСчёт()) {
			JsonArray a;
			for(int i = 0; i < данные.out.дайСчёт(); i++) {
				const Значение& v = данные.out[i];
				if(v.является<RawJsonText>())
					a.CatRaw(v.To<RawJsonText>().json);
				else
					a << JsonRpcData(v);
			}
			json("params", a);
		}
		else
		if(данные.out_map.дайСчёт()) {
			Json m;
			for(int i = 0; i < данные.out_map.дайСчёт(); i++) {
				const Значение& v = данные.out_map.дайЗначение(i);
				Ткст ключ = (Ткст)данные.out_map.дайКлюч(i);
				if(v.является<RawJsonText>())
					m.CatRaw(ключ, v.To<RawJsonText>().json);
				else
					m(ключ, JsonRpcData(v));
			}
			json("params", m);
		}
		json("ид", ид);
		атомнИнк(ид);
		request = ~json;
	}
	else {
		ContentType("text/xml");
		request = XmlHeader();
		if(protocol_version.дайСчёт())
			request << "<!--protocolVersion=\"" << protocol_version << "\"-->\r\n";
		request << ТэгРяр("methodCall")(ТэгРяр("methodName")(method) + FormatXmlRpcParams(данные.out, supports_i8));
	}
	if(sLogRpcCalls) {
		if(sLogRpcCallsCompress)
			RLOG("=== XmlRpc call request:\n" << сожмиЛог(request));
		else
			RLOG("=== XmlRpc call request:\n" << request);
	}
	Ткст response;
	нов();
	if(shorted)
		response = RpcExecuteShorted(request);
	else
		response = пост(request).выполни();
	if(sLogRpcCalls) {
		if(sLogRpcCallsCompress)
			RLOG("=== XmlRpc call response:\n" << сожмиЛог(response));
		else
			RLOG("=== XmlRpc call response:\n" << response);
	}
	RpcGet h;
	if(пусто_ли(response)) {
		faultCode = RPC_CLIENT_HTTP_ERROR;
		faultString = GetErrorDesc();
		Ошибка = "Http request failed: " + faultString;
		LLOG(Ошибка);
		h.v = значОш(Ошибка);
		return h;
	}
	if(json) {
		try {
			Значение r = ParseJSON(response);
			if(мапЗнач_ли(r)) {
				МапЗнач m = r;
				Значение result = m["result"];
				if(!result.проц_ли()) {
					данные.in.очисть();
					данные.in.добавь(result);
					данные.ii = 0;
					h.v = result;
					return h;
				}
				Значение e = m["Ошибка"];
				if(мапЗнач_ли(e)) {
					Значение c = e["код_"];
					Значение m = e["message"];
					if(число_ли(c) && ткст_ли(m)) {
						faultCode = e["код_"];
						faultString = e["message"];
						Ошибка.очисть();
						Ошибка << "Failed '" << faultString << "' (" << faultCode << ')';
						LLOG(s);
						h.v = значОш(Ошибка);
						return h;
					}
				}
			}
			Ткст s;
			faultString = "Invalid response";
			faultCode = RPC_CLIENT_RESPONSE_ERROR;
			Ошибка = faultString;
			LLOG(Ошибка);
			h.v = значОш(Ошибка);
			return h;
		}
		catch(СиПарсер::Ошибка e) {
			Ткст s;
			faultString = e;
			faultCode = RPC_CLIENT_JSON_ERROR;
			Ошибка.очисть();
			Ошибка << "JSON Ошибка: " << faultString;
			LLOG(Ошибка);
			h.v = значОш(Ошибка);
			return h;
		}
	}
	else {
		ПарсерРяр p(response);
		try {
			p.ReadPI();
			while(p.коммент_ли())
				p.читайКоммент();
			p.передайТэг("methodResponse");
			if(p.Тэг("fault")) {
				Значение m = ParseXmlRpcValue(p);
				if(мапЗнач_ли(m)) {
					МапЗнач mm = m;
					faultString = mm["faultString"];
					faultCode = mm["faultCode"];
					Ошибка.очисть();
					Ошибка << "Failed '" << faultString << "' (" << faultCode << ')';
					LLOG(s);
					h.v = значОш(Ошибка);
					return h;
				}
			}
			else {
				данные.in = ParseXmlRpcParams(p);
				данные.ii = 0;
				p.передайКонец();
			}
		}
		catch(ОшибкаРяр e) {
			Ткст s;
			faultString = e;
			faultCode = RPC_CLIENT_XML_ERROR;
			Ошибка.очисть();
			Ошибка << "XML Ошибка: " << faultString;
			LLOG(Ошибка << ": " << p.дайУк());
			h.v = значОш(Ошибка);
			return h;
		}
		h.v = данные.in.дайСчёт() ? данные.in[0] : Null;
		return h;
	}
}

void RpcRequest::сотриОш()
{
	faultCode = 0;
	faultString.очисть();
	Ошибка.очисть();
}

}
