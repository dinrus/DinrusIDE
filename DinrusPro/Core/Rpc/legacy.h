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
проц   SetXmlRpcMethodFilter(Ткст (*фильтр)(const Ткст& methodname))
{
	SetRpcMethodFilter(фильтр);
}

inline
бул   XmlRpcPerform(TcpSocket& http, кткст0 группа)
{
	return RpcPerform(http, группа);
}

inline
бул   XmlRpcServerLoop(цел port, кткст0 группа = NULL)
{
	return RpcServerLoop(port, группа);
}

inline
проц   ThrowXmlRpcError(цел код_, кткст0 s)
{
	ThrowRpcError(код_, s);
}

inline
проц   ThrowXmlRpcError(кткст0 s)
{
	ThrowRpcError(s);
}

inline
проц LogXmlRpcRequests(бул b = true)
{
	LogRpcRequests(b);
}

inline
проц SetXmlRpcServerTrace(Поток& s, цел level = 1)
{
	SetRpcServerTrace(s, level);
}
