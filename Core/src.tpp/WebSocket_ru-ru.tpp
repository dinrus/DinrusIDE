topic "Класс WebSocket";
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
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Класс WebSocket]]}}&]
[s3; &]
[s1;:WebSocket`:`:class: [@(0.0.255)3 class][3 _][*3 WebSocket]&]
[s2;%RU-RU Предоставляет поддержку протокола 
WebSocket. &]
[s0;i448;a25;kKO9;:noref:@(0.0.255) &]
[s3;%RU-RU &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Публичных Методов]]}}&]
[s3; &]
[s5;:Upp`:`:WebSocket`:`:NonBlocking`(bool`): [_^Upp`:`:WebSocket^ WebSocket][@(0.0.255) `&
]_[* NonBlocking]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%RU-RU Если [%-*@3 b] равен true, активирует 
неблокируемый режим. По умолчанию 
(дефолтно) режим блокируем.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:WebSocket`:`:Headers`(const Upp`:`:String`&`): [_^Upp`:`:WebSocket^ WebSock
et][@(0.0.255) `&]_[* Headers]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_
[*@3 h])&]
[s2;%RU-RU Звменяет заголовочники запроса 
HTTP для метода Connect на [%-*@3 h]. Заметьте, 
что заголовочники `"Host:`" и `"Sec`-WebSocket`-Key:`"фиксир
ованы, так как должны быть сгенерированы 
во время Connect, и поэтому не должны 
находиться в [%-*@3 h]. Заголовки предоставляются как текст, разделённый посредством 
`"`\r`\n`". Весь текст должен завершаться на `"`\r`\n`".&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:WebSocket`:`:ClearHeaders`(`): [_^Upp`:`:WebSocket^ WebSocket][@(0.0.255) `&]_
[* ClearHeaders]()&]
[s2;%RU-RU Очищает заголовочники запроса HTTP для Connect.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:WebSocket`:`:AddHeaders`(const Upp`:`:String`&`): [_^Upp`:`:WebSocket^ WebS
ocket][@(0.0.255) `&]_[* AddHeaders]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&
]_[*@3 h])&]
[s2;%RU-RU Добавляет текст в заголовочники 
запроса HTTP для Connect.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:WebSocket`:`:Header`(const char`*`,const Upp`:`:String`&`): [_^Upp`:`:WebSocket^ W
ebSocket][@(0.0.255) `&]_[* Header]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], 
[@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 data])&]
[s2;%RU-RU Добавляет единичный заголовочник 
[%-*@3 id] со значением [%-*@3 data] к заголовочникам 
запроса HTTP для Connect.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:WebSocket`:`:GetHeaders`(`): [_^Upp`:`:String^ String]_[* GetHeaders]()&]
[s2;%RU-RU Возвращает текущие заголовочники 
запроса HTTP для Connect.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:WebSocket`:`:IsBlocking`(`)const: [@(0.0.255) bool]_[* IsBlocking]()_[@(0.0.255) c
onst]&]
[s2;%RU-RU Возвращает true, если WebSocket находится 
в режиме blocking.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:WebSocket`:`:IsError`(`)const: [@(0.0.255) bool]_[* IsError]()_[@(0.0.255) cons
t]&]
[s2;%RU-RU Возвращает true, если имелись ошибки.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:WebSocket`:`:GetError`(`)const: [_^Upp`:`:String^ String]_[* GetError]()_[@(0.0.255) c
onst]&]
[s2;%RU-RU Возвращает описание ошибки.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:WebSocket`:`:Accept`(Upp`:`:TcpSocket`&`): [@(0.0.255) bool]_[* Accept]([_^Upp`:`:TcpSocket^ T
cpSocket][@(0.0.255) `&]_[*@3 listen`_socket])&]
[s2;%RU-RU Принимает подключение от [%-*@3 listen`_socket]. 
Возвращает true при успехе.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:WebSocket`:`:Connect`(const Upp`:`:String`&`,const Upp`:`:String`&`,bool`,int`): [@(0.0.255) b
ool]_[* Connect]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 uri`_], 
[@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 host], 
[@(0.0.255) bool]_[*@3 ssl], [@(0.0.255) int]_[*@3 port])&]
[s5;:Upp`:`:WebSocket`:`:Connect`(const Upp`:`:String`&`,const Upp`:`:String`&`,bool`): [@(0.0.255) b
ool]_[* Connect]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 uri], 
[@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 host], 
[@(0.0.255) bool]_[*@3 ssl])&]
[s5;:Upp`:`:WebSocket`:`:Connect`(const Upp`:`:String`&`): [@(0.0.255) bool]_[* Connect](
[@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 url])&]
[s2;%RU-RU Инициализирует подключение к 
[%-*@3 url]. В режиме blocking возвращает false, 
если нельзя установить подключение, 
иначе true. Вариант, где [%-*@3 port] не указан, 
использует 440, если [%-*@3 ssl], и 80, если 
нет. Вариант с одним параметром [%-*@3 url] 
выводит все остальные параметры из 
[%-*@3 url].&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:WebSocket`:`:Do`(`): [@(0.0.255) void]_[* Do]()&]
[s2;%RU-RU Проводит все операции с сокетом 
в режиме non`-blocking Не может вызываться 
в режиме blocking.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:WebSocket`:`:Receive`(`): [_^Upp`:`:String^ String]_[* Receive]()&]
[s2;%RU-RU Получает сообщение. В режиме blocking 
ждёт, пока оно не станет доступным. 
В режиме non`-blocking возвращает либо сообщение, 
либо String`::GetVoid(), если его нет. В обеих 
случаях также возвращает String`::GetVoid() 
в исключительных ситуациях (ошибки, 
соединение закрыто и проч.). Заметьте, 
что в режиме non`-blocking Receive вызывает 
Do, поэтому не нужно вызывать его отдельно.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:WebSocket`:`:IsFin`(`)const: [@(0.0.255) bool]_[* IsFin]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает true, если у последнего 
полученного сообщения был установлен 
флаг FIN.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:WebSocket`:`:IsText`(`)const: [@(0.0.255) bool]_[* IsText]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает true, если последнее 
полученное сообщение было текстом.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:WebSocket`:`:IsBinary`(`)const: [@(0.0.255) bool]_[* IsBinary]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает  true, если если последнее 
полученное сообщение было текстом.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:WebSocket`:`:SendText`(const Upp`:`:String`&`): [@(0.0.255) void]_[* SendText
]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 data])&]
[s2;%RU-RU Sends a single frame (non`-fragmented) text message.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:WebSocket`:`:SendTextMasked`(const Upp`:`:String`&`): [@(0.0.255) void]_[* Se
ndTextMasked]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 data])&]
[s2;%RU-RU Отсылает по маске текстовое сообщение.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:WebSocket`:`:SendBinary`(const Upp`:`:String`&`): [@(0.0.255) void]_[* SendBi
nary]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 data])&]
[s2;%RU-RU Отсылает один фрейм (нефрагментированно
го) бинарного сообщения.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:WebSocket`:`:Ping`(const Upp`:`:String`&`): [@(0.0.255) void]_[* Ping]([@(0.0.255) c
onst]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 data])&]
[s2;%RU-RU Отсылает PING.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:WebSocket`:`:BeginText`(const Upp`:`:String`&`): [@(0.0.255) void]_[* BeginTe
xt]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 data])&]
[s2;%RU-RU Начинает многофреймное (фрагментированн
ое) текстовое сообщение.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:WebSocket`:`:BeginBinary`(const Upp`:`:String`&`): [@(0.0.255) void]_[* Begin
Binary]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 data])&]
[s2;%RU-RU Начинает многофреймное (фрагментированн
ое) бинарное сообщение.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:WebSocket`:`:Continue`(const Upp`:`:String`&`): [@(0.0.255) void]_[* Continue
]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 data])&]
[s2;%RU-RU Отсылает другой пакет (chunk) данных 
при многофремном (фрагментированном) 
сообщении.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:WebSocket`:`:Fin`(const Upp`:`:String`&`): [@(0.0.255) void]_[* Fin]([@(0.0.255) c
onst]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 data])&]
[s2;%RU-RU Отсылает финальный пакет данных 
при многофреймном (фрагментированном) 
сообщении.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:WebSocket`:`:Close`(const Upp`:`:String`&`,bool`): [@(0.0.255) void]_[* Close
]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 msg], 
[@(0.0.255) bool]_[*@3 wait`_reply])&]
[s2;%RU-RU Инициирует стандартное закрытие 
подключения, отсылая сообщение CLOSE 
[%-*@3 msg]. Если [%-*@3 wait`_reply] равно true, ждёт 
ответа от сервера.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:WebSocket`:`:IsOpen`(`)const: [@(0.0.255) bool]_[* IsOpen]()&]
[s2;%RU-RU Возвращает true, если сокет открыт.&]
[s3; &]
[s4;%RU-RU &]
[s5;:WebSocket`:`:IsClosed`(`)const: [@(0.0.255) bool]_[* IsClosed]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает true, если сокет закрыт.&]
[s4; &]
[s5;:Upp`:`:WebSocket`:`:GetWaitEvents`(`)const: [_^Upp`:`:dword^ dword]_[* GetWaitEvents
]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает комбинацию флагов 
WAIT`_READ и WAIT`_WRITE, указывая на причину 
блокировки операции WebSocket`'а. Может 
использоваться с SocketWaitEvent.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:WebSocket`:`:GetSOCKET`(`)const: [_^SOCKET^ SOCKET]_[* GetSOCKET]()_[@(0.0.255) c
onst]&]
[s2;%RU-RU Если есть ассоциированный сокет, 
возвращает его хэндл. Иначе NULL.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:WebSocket`:`:AddTo`(Upp`:`:SocketWaitEvent`&`): [@(0.0.255) void]_[* AddTo]([_^Upp`:`:SocketWaitEvent^ S
ocketWaitEvent][@(0.0.255) `&]_[*@3 e])&]
[s2;%RU-RU Добавляет WebSocket для ожидания в 
[%-_^Upp`:`:SocketWaitEvent^ SocketWaitEvent].&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:WebSocket`:`:Trace`(bool`): [@(0.0.255) void]_[* Trace]([@(0.0.255) bool]_[*@3 b])
&]
[s2;%RU-RU Активирует журнал трассировки.&]
[s3;%RU-RU &]
[s0;%RU-RU ]]