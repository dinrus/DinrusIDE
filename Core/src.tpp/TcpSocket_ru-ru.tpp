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
[s2;%RU-RU Возвращает хэндл сокета. Заметьте, 
что все сокеты TcpSocket неблокируемые 
с точки зрения хостовой ОС.&]
[s3; &]
[s4; &]
[s5;:TcpSocket`:`:GetPeerAddr`(`)const: [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ S
tring]_[* GetPeerAddr]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает адрес пира.&]
[s3; &]
[s4; &]
[s5;:TcpSocket`:`:Attach`(SOCKET`): [@(0.0.255) void]_[* Attach](SOCKET_[*@3 socket])&]
[s2;%RU-RU Прикрепляет сокет [%-*@3 socket] к TcpSocket. 
[%-*@3 socket] должен находиться в неблокируемом 
состоянии.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:TcpSocket`:`:Connect`(const char`*`,int`): [@(0.0.255) bool]_[* Connect]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 host], [@(0.0.255) int]_[*@3 port])&]
[s2;%RU-RU Подключает сокет к серверу по 
адресу [%-*@3 host]:[%-*@3 port]. Это блокируемая 
операция, так как связана с разрешением 
имени хоста. Возвращает true, если процесс 
подключения стартовал успешно.&]
[s3; &]
[s4;%RU-RU &]
[s5;:TcpSocket`:`:Connect`(IpAddrInfo`&`): [@(0.0.255) bool]_[* Connect]([_^topic`:`/`/Core`/src`/IpAddrInfo`$en`-us`#IpAddrInfo`:`:class^ I
pAddrInfo][@(0.0.255) `&]_[*@3 info])&]
[s2;%RU-RU Подключает сокет к серверу, найденному 
по [%-*@3 info]. Неблокируемо.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TcpSocket`:`:WaitConnect`(`): [@(0.0.255) bool]_[* WaitConnect]()&]
[s2;%RU-RU После того как Connect возвратит 
true, WaitConnect ждёт пока подключение установится
. Заметьте, что WaitConnect нужно использовать 
только тогда, если нужно перехвачивать 
ошибки до отправки/получения данных.&]
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
[s2;%RU-RU Стартует ожидающий (listening) сокет 
сервера по порту [%-*@3 port] с очередью 
ввода [%-*@3 listen`_count]. [%-*@3 ipv6] устанавливает 
сокет  режим IPv6, [%-*@3 reuse] устанавливает 
опцию сокета [^http`:`/`/www`.kernel`.org`/doc`/man`-pages`/online`/pages`/man7`/socket`.7`.html^ S
O`_REUSEADDR]. [%-*@3 addr] может использоваться 
для указания прослушивания на каком`-либо 
(конкретном) интерфейсе. Последний 
параметр, void`*, должен быть указателем 
на uint32, который разреферируется (dereferenced) 
и присваивается sockaddr`_in`::sin`_addr.s`_addr 
при ipv6`=`=true, и/или in6`_addr, разреферируемый 
и присваиваемый sockaddr`_in6`::sin6`_addr при 
ipv6`=`=true.&]
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
[s2;%RU-RU Выполняет shutdown для операций записи. 
Как правило, не требуется.&]
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
[s2;%RU-RU Ждёт в течении срока до таймаута 
событий [%-*@3 events], которые могут быть 
комбинацией WAIT`_READ (ждать ещё поступления 
вводных байтов), WAIT`_WRITE (ждать, пока 
запись в сокет станет возможной). 
Wait также всегда возвращает при ошибках 
сокета. Он возвращает true, если ожидание 
прошло удачно (данные могут записываться/чит
аться после ожидания), false `- когда 
наступает таймаут.&]
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
[s2;%RU-RU Возвращает следующий вводимый 
байт без действительного его удаления 
из очереди ввода. Ожидание протекает 
в течение указанного таймаута, если 
он не поступил до этого срока, возвращает 
`-1.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:TcpSocket`:`:Get`(`): [@(0.0.255) int]_[* Get]()&]
[s2;%RU-RU Читает следующий вводимый байт. 
Ожидание протекает в течение указанного 
таймаута, если он не поступил до этого 
срока, возвращает `-1.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:TcpSocket`:`:Get`(void`*`,int`): [@(0.0.255) int]_[* Get]([@(0.0.255) void]_`*[*@3 buffe
r], [@(0.0.255) int]_[*@3 len])&]
[s2;%RU-RU Считывает [%-*@3 len] байтов в буфер 
[%-*@3 buffer], пытаясь проделать это за период 
до таймаута. Возвращает число действительно
 считаных байтов.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:TcpSocket`:`:Get`(int`): [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ S
tring]_[* Get]([@(0.0.255) int]_[*@3 len])&]
[s2;%RU-RU Считывает максимум [%-*@3 len] байтов, 
пытаясь проделать это за период до 
таймаута. Возвращает String с прочитанными 
данными.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TcpSocket`:`:Put`(const void`*`,int`): [@(0.0.255) int]_[* Put]([@(0.0.255) const]_[@(0.0.255) v
oid]_`*[*@3 s], [@(0.0.255) int]_[*@3 len])&]
[s2;%RU-RU Записывает максимум [%-*@3 len] байтов 
из буфера [%-*@3 buffer], пытаясь проделать 
это за период до таймаута. Возвращает 
число действительно записанных байтов.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:TcpSocket`:`:Put`(const String`&`): [@(0.0.255) int]_[* Put]([@(0.0.255) const]_[_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ S
tring][@(0.0.255) `&]_[*@3 s])&]
[s2;%RU-RU Записывает [%-*@3 s], пытаясь проделать 
это за период до таймаута. Возвращает 
число действительно записанных байтов.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:TcpSocket`:`:GetAll`(void`*`,int`): [@(0.0.255) bool]_[* GetAll]([@(0.0.255) void]_`*[*@3 b
uffer], [@(0.0.255) int]_[*@3 len])&]
[s2;%RU-RU Считывает ровно [%-*@3 len] байтов в 
буфер [%-*@3 buffer]. Если считывание не удаётся 
до таймаута, возвращает false и устанавливает 
ошибку таймаута для TcpSocket.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:TcpSocket`:`:GetAll`(int`): [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ S
tring]_[* GetAll]([@(0.0.255) int]_[*@3 len])&]
[s2;%RU-RU Считывает ровно [%-*@3 len] байтов.Если 
считывание не удаётся до таймаута, 
возвращает String`::GetVoid() и устанавливает 
ошибку таймаута для  TcpSocket.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:TcpSocket`:`:GetLine`(int`): [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ S
tring]_[* GetLine]([@(0.0.255) int]_[*@3 maxlen]_`=_[@3 65536])&]
[s2;%RU-RU Считывает одну строку (заканчивающуюся 
на `'`\n`', `'`\r`' игнорируется). Если всю 
строку не удаётся прочесть до таймаута 
или её длина больше [%-*@3 maxlen], устанавливается 
ошибка и возвращается String`::GetVoid().&]
[s3;%RU-RU &]
[s4; &]
[s5;:TcpSocket`:`:PutAll`(const void`*`,int`): [@(0.0.255) bool]_[* PutAll]([@(0.0.255) con
st]_[@(0.0.255) void]_`*[*@3 s], [@(0.0.255) int]_[*@3 len])&]
[s2;%RU-RU Выводит ровно [%-*@3 len] байтов. Если 
за время до таймаута запись не удаётся, 
устанавливается ошибка и возвращается 
false.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:TcpSocket`:`:PutAll`(const String`&`): [@(0.0.255) bool]_[* PutAll]([@(0.0.255) const]_
[_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ String][@(0.0.255) `&]_[*@3 s
])&]
[s2;%RU-RU Выводит всю String. Если за время 
до таймаута запись не удаётся, устанавливает
ся ошибка и возвращается false.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:TcpSocket`:`:StartSSL`(`): [@(0.0.255) bool]_[* StartSSL]()&]
[s2;%RU-RU Устанавливает TcpSocket в режим SSL 
и стартует рукопожатие SSL. Core/SSL должно 
присутствовать в проекте. Возвращает 
true, если SSL удалось стартовать. Рукопожатие 
не закончено то тех пор, пока SSLHandshake 
не вернёт false.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:TcpSocket`:`:IsSSL`(`)const: [@(0.0.255) bool]_[* IsSSL]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает true, если TcpSocket находится 
в режиме SSL.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:TcpSocket`:`:SSLHandshake`(`): [@(0.0.255) dword]_[* SSLHandshake]()&]
[s2;%RU-RU Выполняет попытку получить прогресс 
с рукопожатием SSL в дотаймаутный период. 
Возвращает комбинацию WAIT`_READ и WAIT`_WRITE, 
если рукопожатие SSL (всё ещё) в прогрессе, 
указывающую на то, что процкссу нужно 
прочесть или записать из сокета ещё 
что`-либо. Возвращает 0, если рукопожатие 
окончено.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:TcpSocket`:`:SSLCertificate`(const String`&`,const String`&`,bool`): [@(0.0.255) v
oid]_[* SSLCertificate]([@(0.0.255) const]_[_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ S
tring][@(0.0.255) `&]_[*@3 cert], [@(0.0.255) const]_[_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ S
tring][@(0.0.255) `&]_[*@3 pkey], [@(0.0.255) bool]_[*@3 asn1])&]
[s2;%RU-RU Устанавливает сертификат SSL. Должен 
вызываться до StartSSL. (Заметьте, что 
клиентам, как правило, сертификаты 
не требуются, они обычто используются 
на принимающих сокетах.)&]
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
[s2;%RU-RU Возвращает информацию об установленном 
(после рукопожатия) подключении SSL 
или NULL, если такая информация недоступна.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:TcpSocket`:`:Timeout`(int`): [_^topic`:`/`/Core`/src`/TcpSocket`$en`-us`#TcpSocket`:`:class^ T
cpSocket][@(0.0.255) `&]_[* Timeout]([@(0.0.255) int]_[*@3 ms])&]
[s2;%RU-RU Устанавливает таймаут для всех 
операций. Null означает, что все операции 
возвращают немедленно (в этом случае 
неплохо бы выполнить некоторого рода 
внешнюю блокировку сокета или группы 
сокетов, напр.,. SocketWaitEvent). Null означает, 
что операции блокируемые (но всё же 
могут вызывать периодически WhenProgress, 
если он определён). Другие значения 
указывают количество миллисекунд. 
Заметка: таймаут можно настроить перед 
всякой единичной операцией с TcpSocket. 
Возвращает `*this. Дефолтное значение 
равно Null, что означает, что TcpSocket блокируемый
.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:TcpSocket`:`:GetTimeout`(`)const: [@(0.0.255) int]_[* GetTimeout]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает текущий таймаут.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TcpSocket`:`:GlobalTimeout`(int`): [_^topic`:`/`/Core`/src`/TcpSocket`$en`-us`#TcpSocket`:`:class^ T
cpSocket][@(0.0.255) `&]_[* GlobalTimeout]([@(0.0.255) int]_[*@3 ms])&]
[s2;%RU-RU Устанавливает `"глобальный таймаут`". 
Этот таймаут действует над всем диапазоном 
операций, пока его не отменит повторный 
вызов этого метода с параметром Null 
или не будет установлен новый глобальный 
таймаут. Если глобальный таймаут 
превышен, операция, при которой это 
происходит, не удаётся, а код ошибки 
сокета устанавливается в ERROR`_GLOBAL`_TIMEOUT.&]
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
`*this. Это дефолтное значение.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:TcpSocket`:`:IsBlocking`(`): [@(0.0.255) bool]_[* IsBlocking]()&]
[s2;%RU-RU То же, что и IsNull(GetTimeout()).&]
[s3; &]
[s4; &]
[s5;:TcpSocket`:`:WaitStep`(int`): [_^topic`:`/`/Core`/src`/TcpSocket`$en`-us`#TcpSocket`:`:class^ T
cpSocket][@(0.0.255) `&]_[* WaitStep]([@(0.0.255) int]_[*@3 ms])&]
[s2;%RU-RU Устанавливает периодичность 
вызова WhenWait в миллисекундах между 
вызовами. Дефолт равен 10 мс (100 гц).&]
[s3;%RU-RU &]
[s4; &]
[s5;:TcpSocket`:`:GetWaitStep`(`)const: [@(0.0.255) int]_[* GetWaitStep]()_[@(0.0.255) cons
t]&]
[s2;%RU-RU Возвращает текущую периодичность 
вызова WhenWait.&]
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