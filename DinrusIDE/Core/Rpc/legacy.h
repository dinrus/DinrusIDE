enum {
	XMLRPC_SERVER_JSON_ERROR = RPC_SERVER_JSON_ERROR,
	XMLRPC_SERVER_JSON_REQUEST_ERROR = RPC_SERVER_JSON_REQUEST_ERROR,
	XMLRPC_UNKNOWN_METHOD_ERROR = RPC_UNKNOWN_METHOD_ERROR,
	XMLRPC_SERVER_PARAM_ERROR = RPC_SERVER_PARAM_ERROR,
	XMLRPC_SERVER_PROCESSING_ERROR = RPC_SERVER_PROCESSING_ERROR,

	XMLRPC_SERVER_XML_ERROR = RPC_SERVER_XML_ERROR,

	XMLRPC_CLIENT_HTTP_ERROR = RPC_CLIENT_HTTP_ERROR,
	XMLRPC_CLIENT_XML_ERROR = RPC_CLIENT_XML_ERROR,
	XMLRPC_CLIENT_JSON_ERROR = RPC_CLIENT_JSON_ERROR,
	XMLRPC_CLIENT_RESPONSE_ERROR = RPC_CLIENT_RESPONSE_ERROR,
};

#define XMLRPC_METHOD(x)  RPC_METHOD(x)

#define XMLRPC_GMETHOD(x, группа) RPC_GMETHOD(x, группа)

typedef RpcError XmlRpcError;

typedef RpcData XmlRpcData;

inline
void   SetXmlRpcMethodFilter(Ткст (*filter)(const Ткст& methodname))
{
	SetRpcMethodFilter(filter);
}

inline
bool   XmlRpcPerform(TcpSocket& http, const char *группа)
{
	return RpcPerform(http, группа);
}

inline
bool   XmlRpcServerLoop(int port, const char *группа = NULL)
{
	return RpcServerLoop(port, группа);
}

inline
void   ThrowXmlRpcError(int code, const char *s)
{
	ThrowRpcError(code, s);
}

inline
void   ThrowXmlRpcError(const char *s)
{
	ThrowRpcError(s);
}

inline
void LogXmlRpcRequests(bool b = true)
{
	LogRpcRequests(b);
}

inline
void SetXmlRpcServerTrace(Поток& s, int level = 1)
{
	SetRpcServerTrace(s, level);
}
