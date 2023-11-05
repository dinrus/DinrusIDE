topic "Класс TcpSocket";
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_} 
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Класс TcpSocket]]}}&]
[s3; &]
[s1;:TcpSocket`:`:class: [@(0.0.255)3 class][3 _][*3 TcpSocket]&]
[s2;%RU-RU Этот класс представляет сокет 
TCP/IP. Он расширяет базовую семантику 
сокетов, допуская неблокируемые или 
ограниченные по времени операции.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Публичных Методов]]}}&]
[s3; &]
[s5;:TcpSocket`:`:WhenWait: [_^topic`:`/`/Core`/src`/Callbacks`$en`-us`#Callback`:`:class^ C
allback]_[* WhenWait]&]
[s2;%RU-RU Если этот обрвыз определён, он 
периодически вызывается при выполнении 
TcpSocket`'ом любых операций, с периобом, 
установленным посредством WaitStep (дефолт 
10 мс / 100 гц). Предназначен для интеракции 
с пользователем в приложениях.&]
[s3; &]
[s4; &]
[s5;:TcpSocket`:`:GetHostName`(`): [@(0.0.255) static] [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ S
tring]_[* GetHostName]()&]
[s2;%RU-RU Возвращает имя компьютера.&]
[s3; &]
[s4; &]
[s5;:TcpSocket`:`:GetDone`(`)const: [@(0.0.255) int]_[* GetDone]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает число байтов, обработанное 
во время текущей операции; предназначен 
для вызова из процедуры WhenWait&]
[s3; &]
[s4; &]
[s5;:TcpSocket`:`:IsOpen`(`)const: [@(0.0.255) bool]_[* IsOpen]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает true, если сокет открыт.&]
[s3; &]
[s4; &]
[s5;:TcpSocket`:`:IsEof`(`)const: [@(0.0.255) bool]_[* IsEof]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает true, если вводных данных 
для обработки больше не осталось. 
Также возвращает true, если сокет не 
открыт, имелась ошибка или сокет был 
прерван.&]
[s3; &]
[s4; &]
[s5;:TcpSocket`:`:IsError`(`)const: [@(0.0.255) bool]_[* IsError]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает true, если какая`-л. предыдущая 
операция отчиталась об ошибке. В таком 
случае, все последующие запросы игнорируютс
я.&]
[s3; &]
[s4; &]
[s5;:TcpSocket`:`:ClearError`(`): [@(0.0.255) void]_[* ClearError]()&]
[s2;%RU-RU Очищает состояние ошибки.&]
[s3; &]
[s4; &]
[s5;:TcpSocket`:`:GetError`(`)const: [@(0.0.255) int]_[* GetError]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает код ошибки. Эти коды 
определены либо в SOCKET API, или это может 
быть `-1 для прочих ошибок.&]
[s3; &]
[s4; &]
[s5;:TcpSocket`:`:GetErrorDesc`(`)const: [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ S
tring]_[* GetErrorDesc]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает описание ошибки.&]
[s3; &]
[s4; &]
[s5;:TcpSocket`:`:Abort`(`): [@(0.0.255) void]_[* Abort]()&]
[s2;%RU-RU Устанавливает TcpSocket в прерванное 
состояние, в котором ингнорируются 
все последующие запросы. Может вызываться 
из процедуры WhenWait.&]
[s3; &]
[s4; &]
[s5;:TcpSocket`:`:IsAbort`(`)const: [@(0.0.255) bool]_[* IsAbort]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает true, если TcpSocket в прерванном 
состоянии.&]
[s3; &]
[s4; &]
[s5;:TcpSocket`:`:ClearAbort`(`): [@(0.0.255) void]_[* ClearAbort]()&]
[s2;%RU-RU Очищает состояние прерванности.&]
[s3; &]
[s4; &]
[s5;:TcpSocket`:`:IsTimeout`(`)const: [@(0.0.255) bool]_[* IsTimeout]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает true, если последняя 
операция просрочена (в таймауте).&]
[s3; &]
[s4; &]
[s5;:TcpSocket`:`:GetSOCKET`(`)const: SOCKET_[* GetSOCKET]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает хэндл сокета. Note that 
all TcpSocket sockets are non`-blocking from host OS perspective.&]
[s3; &]
[s4; &]
[s5;:TcpSocket`:`:GetPeerAddr`(`)const: [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ S
tring]_[* GetPeerAddr]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает адрес пира.&]
[s3; &]
[s4; &]
[s5;:TcpSocket`:`:Attach`(SOCKET`): [@(0.0.255) void]_[* Attach](SOCKET_[*@3 socket])&]
[s2;%RU-RU Attaches [%-*@3 socket] to TcpSocket. [%-*@3 socket] must 
be in non`-blocking state.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:TcpSocket`:`:Connect`(const char`*`,int`): [@(0.0.255) bool]_[* Connect]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 host], [@(0.0.255) int]_[*@3 port])&]
[s2;%RU-RU Connects socket to server at [%-*@3 host]:[%-*@3 port]. This 
operation is blocking with respect to resolving host name. Возвращает 
true when connection process is successfully started.&]
[s3; &]
[s4;%RU-RU &]
[s5;:TcpSocket`:`:Connect`(IpAddrInfo`&`): [@(0.0.255) bool]_[* Connect]([_^topic`:`/`/Core`/src`/IpAddrInfo`$en`-us`#IpAddrInfo`:`:class^ I
pAddrInfo][@(0.0.255) `&]_[*@3 info])&]
[s2;%RU-RU Connects socket to server found at [%-*@3 info]. Non`-blocking.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TcpSocket`:`:WaitConnect`(`): [@(0.0.255) bool]_[* WaitConnect]()&]
[s2;%RU-RU After Connect returns true, WaitConnect waits for connection 
to be established. Note that it is only necessary to use WaitConnect 
if you want to intercept errors before sending/recieving data.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TcpSocket`:`:Listen`(int`,int`,bool`,bool`,void`*`): [@(0.0.255) bool]_[* Listen]([@(0.0.255) i
nt]_[*@3 port], [@(0.0.255) int]_[*@3 listen`_count]_`=_[@3 5], [@(0.0.255) bool]_[*@3 ipv6]_
`=_[@(0.0.255) false], [@(0.0.255) bool]_[*@3 reuse]_`=_[@(0.0.255) true], 
[@(0.0.255) void`*]_[*@3 addr]_`=_NULL)&]
[s5;:TcpSocket`:`:Listen`(const IpAddrInfo`&`,int`,int`,bool`,bool`): [@(0.0.255) bool]_
[* Listen]([@(0.0.255) const]_[_^topic`:`/`/Core`/src`/IpAddrInfo`$en`-us`#IpAddrInfo`:`:class^ I
pAddrInfo][@(0.0.255) `&]_[*@3 addr], [@(0.0.255) int]_[*@3 port], [@(0.0.255) int]_[*@3 list
en`_count]_`=_[@3 5], [@(0.0.255) bool]_[*@3 ipv6]_`=_[@(0.0.255) false], 
[@(0.0.255) bool]_[*@3 reuse]_`=_[@(0.0.255) true])&]
[s2;%RU-RU Starts a listening server socket at [%-*@3 port] with input 
queue [%-*@3 listen`_count]. [%-*@3 ipv6] sets the socket to IPv6 
mode, [%-*@3 reuse] sets [^http`:`/`/www`.kernel`.org`/doc`/man`-pages`/online`/pages`/man7`/socket`.7`.html^ S
O`_REUSEADDR] socket option. [%-*@3 addr] can be used to specify 
on what interface to listen on. As last void`* parameter, it 
has to be pointer to uint32 that is dereferenced and assigned 
to sockaddr`_in`::sin`_addr.s`_addr for ipv6`=`=true and/or in6`_addr 
to be dereferenced and assigned to sockaddr`_in6`::sin6`_addr 
for ipv6`=`=true.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:TcpSocket`:`:Accept`(TcpSocket`&`): [@(0.0.255) bool]_[* Accept]([_^topic`:`/`/Core`/src`/TcpSocket`$en`-us`#TcpSocket`:`:class^ T
cpSocket][@(0.0.255) `&]_[*@3 listen`_socket])&]
[s2;%RU-RU Принимает подключение от [%-*@3 listen`_socket].&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:TcpSocket`:`:Close`(`): [@(0.0.255) void]_[* Close]()&]
[s2;%RU-RU Закрывает сокет.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:TcpSocket`:`:Shutdown`(`): [@(0.0.255) void]_[* Shutdown]()&]
[s2;%RU-RU Performs shutdown for write operations. Normally not needed.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:TcpSocket`:`:NoDelay`(`): [@(0.0.255) void]_[* NoDelay]()&]
[s2;%RU-RU Устанавливает опцию TCP`_NODELAY.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:TcpSocket`:`:Linger`(int`): [@(0.0.255) void]_[* Linger]([@(0.0.255) int]_[*@3 msecs])&]
[s2;%RU-RU Устанавливает опцию SO`_LINGER в [%-*@3 msecs]. 
Если [%-*@3 msecs] равно Null, отключает SO`_LINGER.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:TcpSocket`:`:NoLinger`(`): [@(0.0.255) void]_[* NoLinger]()&]
[s2;%RU-RU То же, что и Linger(Null).&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:TcpSocket`:`:Wait`(dword`): [@(0.0.255) bool]_[* Wait]([_^topic`:`/`/Core`/src`/PrimitiveDataTypes`$en`-us`#Upp`:`:dword`:`:typedef^ d
word]_[*@3 events])&]
[s2;%RU-RU Waits for at most timeout for [%-*@3 events], which can 
be a combination of WAIT`_READ (wait for more input bytes available), 
WAIT`_WRITE (wait till it is possible to write something to socket). 
Wait also always returns when socket exception happens. Возвращает 
true if wait was successful (data can be written/read after the 
wait), false on timeout.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:TcpSocket`:`:WaitRead`(`): [@(0.0.255) bool]_[* WaitRead]()&]
[s2;%RU-RU То же, что и Wait(WAIT`_READ).&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:TcpSocket`:`:WaitWrite`(`): [@(0.0.255) bool]_[* WaitWrite]()&]
[s2;%RU-RU То же, что и Wait(WAIT`_WRITE).&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:TcpSocket`:`:Peek`(`): [@(0.0.255) int]_[* Peek]()&]
[s5;:TcpSocket`:`:Term`(`): [@(0.0.255) int]_[* Term]()&]
[s2;%RU-RU Возвращает the next input byte without actually 
removing it from input queue. It at most waits for specified 
timeout for it, if there is still none, returns `-1.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:TcpSocket`:`:Get`(`): [@(0.0.255) int]_[* Get]()&]
[s2;%RU-RU Reads the next input byte. It at most waits for specified 
timeout for it, if there is still none, returns `-1.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:TcpSocket`:`:Get`(void`*`,int`): [@(0.0.255) int]_[* Get]([@(0.0.255) void]_`*[*@3 buffe
r], [@(0.0.255) int]_[*@3 len])&]
[s2;%RU-RU Reads at most [%-*@3 len] bytes into [%-*@3 buffer], trying 
to do so at most for specified timeout. Возвращает 
the number of bytes actually read.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:TcpSocket`:`:Get`(int`): [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ S
tring]_[* Get]([@(0.0.255) int]_[*@3 len])&]
[s2;%RU-RU Reads at most [%-*@3 len] bytes, trying to do so at most 
for specified timeout. Возвращает a String with read 
data.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TcpSocket`:`:Put`(const void`*`,int`): [@(0.0.255) int]_[* Put]([@(0.0.255) const]_[@(0.0.255) v
oid]_`*[*@3 s], [@(0.0.255) int]_[*@3 len])&]
[s2;%RU-RU Writes at most [%-*@3 len] bytes from [%-*@3 buffer], trying 
to do so at most for specified timeout. Возвращает 
the number of bytes actually written.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:TcpSocket`:`:Put`(const String`&`): [@(0.0.255) int]_[* Put]([@(0.0.255) const]_[_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ S
tring][@(0.0.255) `&]_[*@3 s])&]
[s2;%RU-RU Writes [%-*@3 s], trying to do so at most for specified 
timeout. Возвращает the number of bytes actually written.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:TcpSocket`:`:GetAll`(void`*`,int`): [@(0.0.255) bool]_[* GetAll]([@(0.0.255) void]_`*[*@3 b
uffer], [@(0.0.255) int]_[*@3 len])&]
[s2;%RU-RU Reads exactly [%-*@3 len] bytes into [%-*@3 buffer]. If such 
number of bytes cannot be read until timeout, returns false and 
sets timeout error for TcpSocket.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:TcpSocket`:`:GetAll`(int`): [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ S
tring]_[* GetAll]([@(0.0.255) int]_[*@3 len])&]
[s2;%RU-RU Reads exactly [%-*@3 len] bytes. If such number of bytes 
cannot be read until timeout, returns String`::GetVoid() and sets 
timeout error for TcpSocket.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:TcpSocket`:`:GetLine`(int`): [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ S
tring]_[* GetLine]([@(0.0.255) int]_[*@3 maxlen]_`=_[@3 65536])&]
[s2;%RU-RU Reads single line (ended with `'`\n`', `'`\r`' is ignored). 
If the whole line cannot be read within timeout or line length 
is longer than [%-*@3 maxlen] sets error and returns String`::GetVoid().&]
[s3;%RU-RU &]
[s4; &]
[s5;:TcpSocket`:`:PutAll`(const void`*`,int`): [@(0.0.255) bool]_[* PutAll]([@(0.0.255) con
st]_[@(0.0.255) void]_`*[*@3 s], [@(0.0.255) int]_[*@3 len])&]
[s2;%RU-RU Outputs exactly [%-*@3 len] bytes. If such number of bytes 
cannot be written in time specified by timeout, sets error and 
returns false.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:TcpSocket`:`:PutAll`(const String`&`): [@(0.0.255) bool]_[* PutAll]([@(0.0.255) const]_
[_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ String][@(0.0.255) `&]_[*@3 s
])&]
[s2;%RU-RU Outputs the whole String. If such number of bytes cannot 
be written in time specified by timeout, sets error and returns 
false.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:TcpSocket`:`:StartSSL`(`): [@(0.0.255) bool]_[* StartSSL]()&]
[s2;%RU-RU Устанавливает TcpSocket to SSL mode and starts 
SSL handshake. Core/SSL must be present in project. Возвращает 
 true, если SSL could have been started. Handshake is not 
finished until SSLHandshake returns false.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:TcpSocket`:`:IsSSL`(`)const: [@(0.0.255) bool]_[* IsSSL]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает true, если TcpSocket находится 
в режиме SSL.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:TcpSocket`:`:SSLHandshake`(`): [@(0.0.255) dword]_[* SSLHandshake]()&]
[s2;%RU-RU Attempts the progress on SSL handshake for at most timeout 
period. Возвращает a combination of WAIT`_READ and 
WAIT`_WRITE if SSL handshake is (still) in progress, indicating 
whether the process needs to read or write more bytes from the 
socket. Возвращает 0 if handshake is finished.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:TcpSocket`:`:SSLCertificate`(const String`&`,const String`&`,bool`): [@(0.0.255) v
oid]_[* SSLCertificate]([@(0.0.255) const]_[_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ S
tring][@(0.0.255) `&]_[*@3 cert], [@(0.0.255) const]_[_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ S
tring][@(0.0.255) `&]_[*@3 pkey], [@(0.0.255) bool]_[*@3 asn1])&]
[s2;%RU-RU Устанавливает сертификат SSL. Должен 
вызываться до StartSSL. (Note that clients usually 
do not need certificates, this is usually used on accepting sockets.)&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:TcpSocket`:`:SSLServerNameIndication`(const Upp`:`:String`&`): [@(0.0.255) v
oid]_[* SSLServerNameIndication]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&
]_[*@3 name])&]
[s2;%RU-RU Устанавливает [^https`:`/`/cs`.wikipedia`.org`/wiki`/Server`_Name`_Indication^ S
NI] для подключения SSL.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:TcpSocket`:`:GetSSLInfo`(`)const: [@(0.0.255) const]_[_^topic`:`/`/Core`/src`/TcpSocket`$en`-us`#SSLInfo`:`:struct^ S
SLInfo]_`*[* GetSSLInfo]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает information about established (after 
handshake) SSL connection or NULL if such information is not 
available.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:TcpSocket`:`:Timeout`(int`): [_^topic`:`/`/Core`/src`/TcpSocket`$en`-us`#TcpSocket`:`:class^ T
cpSocket][@(0.0.255) `&]_[* Timeout]([@(0.0.255) int]_[*@3 ms])&]
[s2;%RU-RU Устанавливает таймаут для всех 
операций. Zero means that all operations return immediately 
(in that case it is usually a good idea to perform some sort 
of external blocking on socket or socket group using e.g. SocketWaitEvent). 
Null means operations are blocking (but they still can invoke 
WhenProgress periodically if defined). Other values specify a 
number of milliseconds. Note: It is possible to adjust timeout 
before any single TcpSocket operation. Возвращает `*this. 
Default value is Null, which means TcpSocket is blocking.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:TcpSocket`:`:GetTimeout`(`)const: [@(0.0.255) int]_[* GetTimeout]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает current timeout.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TcpSocket`:`:GlobalTimeout`(int`): [_^topic`:`/`/Core`/src`/TcpSocket`$en`-us`#TcpSocket`:`:class^ T
cpSocket][@(0.0.255) `&]_[* GlobalTimeout]([@(0.0.255) int]_[*@3 ms])&]
[s2;%RU-RU Устанавливает `"глобальный таймаут`". 
This timeout is in effect over a whole range of operations, until 
it is canceled by calling this method with Null parameter or 
by setting a new global timeout. If global timeout is exceeded, 
operation during which it happened fails and socket error code 
is set to ERROR`_GLOBAL`_TIMEOUT.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TcpSocket`:`:NoGlobalTimeout`(`): [_^topic`:`/`/Core`/src`/TcpSocket`$en`-us`#TcpSocket`:`:class^ T
cpSocket][@(0.0.255) `&]_[* NoGlobalTimeout]()&]
[s2;%RU-RU То же, что и GlobalTimeout(Null).&]
[s3; &]
[s4;%RU-RU &]
[s5;:TcpSocket`:`:Blocking`(`): [_^topic`:`/`/Core`/src`/TcpSocket`$en`-us`#TcpSocket`:`:class^ T
cpSocket][@(0.0.255) `&]_[* Blocking]()&]
[s2;%RU-RU То же, что и Timeout(Null). Возвращает 
`*this. This is the default value.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:TcpSocket`:`:IsBlocking`(`): [@(0.0.255) bool]_[* IsBlocking]()&]
[s2;%RU-RU То же, что и IsNull(GetTimeout()).&]
[s3; &]
[s4; &]
[s5;:TcpSocket`:`:WaitStep`(int`): [_^topic`:`/`/Core`/src`/TcpSocket`$en`-us`#TcpSocket`:`:class^ T
cpSocket][@(0.0.255) `&]_[* WaitStep]([@(0.0.255) int]_[*@3 ms])&]
[s2;%RU-RU Устанавливаетthe periodicity of calling WhenWait 
in millisecond between calls. Default is 10ms (100hz).&]
[s3;%RU-RU &]
[s4; &]
[s5;:TcpSocket`:`:GetWaitStep`(`)const: [@(0.0.255) int]_[* GetWaitStep]()_[@(0.0.255) cons
t]&]
[s2;%RU-RU Retruns current periodicity of calling WhenWait.&]
[s3; &]
[s4;%RU-RU &]
[s5;:TcpSocket`:`:TcpSocket`(`): [* TcpSocket]()&]
[s5;:TcpSocket`:`:`~TcpSocket`(`): [@(0.0.255) `~][* TcpSocket]()&]
[s2;%RU-RU Конструктор , деструктор.&]
[s3;%RU-RU &]
[s0; &]
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Структура SSLInfo]]}}&]
[s0;%RU-RU &]
[s1;:SSLInfo`:`:struct: [@(0.0.255)3 struct][3 _][*3 SSLInfo]&]
[s2;%RU-RU Эта структура используется для 
передачи информации об установленном 
подключении SSL.&]
[s3; &]
[s4; &]
[s5;:SSLInfo`:`:cipher: [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ Strin
g]_[* cipher]&]
[s2;%RU-RU Используемый шифр.&]
[s3; &]
[s4; &]
[s5;:SSLInfo`:`:cert`_avail: [@(0.0.255) bool]_[* cert`_avail]&]
[s2;%RU-RU Сертификат доступен.&]
[s3; &]
[s4; &]
[s5;:SSLInfo`:`:cert`_subject: [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ S
tring]_[* cert`_subject]&]
[s2;%RU-RU Имя субъекта.&]
[s3; &]
[s4; &]
[s5;:SSLInfo`:`:cert`_issuer: [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ S
tring]_[* cert`_issuer]&]
[s2;%RU-RU Имя издателя.&]
[s3; &]
[s4; &]
[s5;:SSLInfo`:`:cert`_notbefore: [_^topic`:`/`/Core`/src`/DateTime`$en`-us`#Date`:`:struct^ D
ate]_[* cert`_notbefore]&]
[s2;%RU-RU До этой даты сертификат недействителен.&]
[s3; &]
[s4; &]
[s5;:SSLInfo`:`:cert`_notafter: [_^topic`:`/`/Core`/src`/DateTime`$en`-us`#Date`:`:struct^ D
ate]_[* cert`_notafter]&]
[s2;%RU-RU После этой даты сертификат недействител
ен.&]
[s3; &]
[s4; &]
[s5;:SSLInfo`:`:cert`_version: [@(0.0.255) int]_[* cert`_version]&]
[s2;%RU-RU Версия сертификата.&]
[s3; &]
[s4; &]
[s5;:SSLInfo`:`:cert`_serial: [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ S
tring]_[* cert`_serial]&]
[s2;%RU-RU Серийный номер сертификата.&]
[s3; ]]