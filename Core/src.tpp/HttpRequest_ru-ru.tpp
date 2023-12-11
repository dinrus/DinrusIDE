topic "Класс HttpRequest";
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
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Класс HttpRequest]]}}&]
[s3; &]
[s1;:HttpRequest`:`:class: [@(0.0.255)3 class][3 _][*3 HttpRequest][3 _:_][@(0.0.255)3 public][3 _
][*@3;3 TcpSocket]&]
[s2;%RU-RU Этот класс выполняет синхронные 
и асинхронные запросы по протоколу 
HTTP.&]
[s3;%RU-RU &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Публичных Членов]]}}&]
[s3; &]
[s5;:HttpRequest`:`:WhenContent: [_^Callback2^ Event]<[@(0.0.255) const]_[@(0.0.255) void]_
`*, [@(0.0.255) int]>_[* WhenContent]&]
[s2;%RU-RU Определяет функцию`-`"потребителя`" 
для контента из ответа HTTP. Если определён, 
то HttpRequest использует это событие вывода, 
вместо сохранения выводного контента 
в String, которую можно читать методом 
GetContent. Заметьте, что только `"полноценный`" 
контент, определённый как контент 
в ответе с кодом состояния в диапазоне 
200 .. 299, отправляется в WhenContent `- это 
подавляет проблемы с несколькими 
запросами, из`-за перенаправления 
или авторизации. Такое поведение 
можно изменить с помощью AllContent `- в 
таком случае код`-`"клиент`", вероятнее 
всего, использует WhenStart, чтобы отделять 
ответы на конкретные запросы.&]
[s3; &]
[s4; &]
[s5;:HttpRequest`:`:WhenStart: [_^Callback^ Event<>]_[* WhenStart]&]
[s2;%RU-RU Вызывается всякий раз, когда HttpRequest 
начинает попытку нового запроса `- 
в это входят редирекция (перенаправление), 
авторизация или повторные попытки 
при ошибках. Даёт возможность клиентскоиму 
ПО перезапустить обработку, напр., 
чтобы удалить частично загруженный 
файл.&]
[s3; &]
[s4; &]
[s5;:HttpRequest`:`:WhenDo: [_^Callback^ Event<>]_[* WhenDo]&]
[s2;%RU-RU Вызывается всякий раз, когда завершается
 процедура Do. Применимо для показа 
прогресса или прерывания процедур 
через ГИП (GUI).&]
[s3; &]
[s4; &]
[s5;:Upp`:`:HttpRequest`:`:WhenAuthenticate: [_^Upp`:`:Gate^ Gate<>]_[* WhenAuthenticate]&]
[s2;%RU-RU Вызывается, когда запрос возвращает 
код 401 (неавторизованный доступ). Обратный 
вызов должен проверить возвращённые 
заголовочники и, при возможности, 
 предоставить авторизационный заголовочник
 (через Authorization). В этом случае, должно 
вернуться true, указывающее на то, что 
проблема авторизации успешно разрешена 
`- HttpRequest затем попытается повторить 
запрос.&]
[s3; &]
[s4; &]
[s5;:HttpRequest`:`:MaxHeaderSize`(int`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* Ma
xHeaderSize]([@(0.0.255) int]_[*@3 m])&]
[s2;%RU-RU Определяет максимальный размер 
заголовочника ответа (дефолт равен 
1000000). Возвращает `*this.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:MaxContentSize`(int`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* M
axContentSize]([@(0.0.255) int]_[*@3 m])&]
[s2;%RU-RU Определяет максимальный размер 
контента ответа, который может быть 
сохранён в HttpRequest, а потом извлечён 
оттуда методом GetContent (дефолт равен 
10000000). Заметьте, что этот предел не 
применяется,  если данные контента 
обрабатываются с помощью обрвызова 
WhenContent. Возвращает `*this.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:MaxRedirect`(int`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* MaxR
edirect]([@(0.0.255) int]_[*@3 n])&]
[s2;%RU-RU Определяет максимальное число 
перенаправлений (код 3xx) (дефолт равен 
10). Возвращает `*this.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:MaxRetries`(int`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* MaxRe
tries]([@(0.0.255) int]_[*@3 n])&]
[s2;%RU-RU Определяет максимальное число 
повторных попыток при ошибках (дефолт 
равен 3). Возвращает `*this.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:RequestTimeout`(int`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* R
equestTimeout]([@(0.0.255) int]_[*@3 ms])&]
[s2;%RU-RU Определяет общее допустимое время 
для запроса в миллисекундах (дефолт 
равен.120000 `- две минуты). Возвращает 
`*this.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:ChunkSize`(int`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* ChunkS
ize]([@(0.0.255) int]_[*@3 n])&]
[s2;%RU-RU Определяет максимальный размер 
блока данных контента для обработки 
(дефолт равен 4096). Возвращает `*this.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:HttpRequest`:`:AllContent`(bool`): [_^Upp`:`:HttpRequest^ HttpRequest][@(0.0.255) `&
]_[* AllContent]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%RU-RU Если активен, WhenContent получает 
контент от всех ответов, даже если 
они просто перенаправления или тексты 
авторизации.&]
[s3;%RU-RU &]
[s4; &]
[s5;:HttpRequest`:`:Method`(int`,const char`*`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&
]_[* Method]([@(0.0.255) int]_[*@3 m], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 custom`_na
me]_`=_NULL)&]
[s2;%RU-RU Устанавливает метод запроса HTTP. 
[%-*@3 m] может быть одним из METHOD`_GET, METHOD`_POST, 
METHOD`_HEAD, METHOD`_PUT, METHOD`_DELETE, METHOD`_TRACE, METHOD`_OPTIONS, 
METHOD`_CONNECT, METHOD`_PATCH. Дефолтно установлен 
METHOD`_GET. Если [%-*@3 custom`_name] не равно NULL, 
HttpRequest ведёт себя как при выполнении 
метода [%-*@3 m], но при запросе HTTP [%-*@3 custom`_name] 
используется в качестве метода. Возвращает 
`*this.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:GET`(`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* GET]()&]
[s2;%RU-RU То же, что и Method(METHOD`_GET) (дефолт). 
Возвращает `*this.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:POST`(`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* POST]()&]
[s2;%RU-RU То же, что и Method(METHOD`_POST). Возвращает 
`*this.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:HEAD`(`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* HEAD]()&]
[s2;%RU-RU То же, что и Method(METHOD`_HEAD). Возвращает 
`*this.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:PUT`(`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* PUT]()&]
[s2;%RU-RU То же, что и Method(METHOD`_PUT). Возвращает 
`*this.&]
[s3;%RU-RU &]
[s4; &]
[s5;:HttpRequest`:`:DEL`(`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* DEL]()&]
[s2;%RU-RU То же, что и Method(METHOD`_DELETE). Возвращает 
`*this. Заметьте, что этот метод не назван 
`"DELETE`", во избежании конфликта с именем 
из интерфеса Win32 #define.&]
[s3; &]
[s4; &]
[s5;:HttpRequest`:`:TRACE`(`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* TRACE]()&]
[s2;%RU-RU То же, что и Method(METHOD`_TRACE). Возвращает 
`*this.&]
[s3; &]
[s4; &]
[s5;:HttpRequest`:`:OPTIONS`(`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* OPTIONS]()&]
[s2;%RU-RU То же, что и Method(METHOD`_OPTIONS). Возвращает 
`*this.&]
[s3; &]
[s4; &]
[s5;:HttpRequest`:`:CONNECT`(`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* CONNECT]()&]
[s2;%RU-RU То же, что и Method(METHOD`_CONNECT). Возвращает 
`*this.&]
[s3; &]
[s4; &]
[s5;:HttpRequest`:`:PATCH`(`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* PATCH]()&]
[s2;%RU-RU То же, что и Method(METHOD`_PATCH). Возвращает 
`*this.&]
[s3; &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:Host`(const String`&`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* H
ost]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 h])&]
[s2;%RU-RU Устанавливает хост сервера. Возвращает 
`*this.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:Port`(int`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* Port]([@(0.0.255) i
nt]_[*@3 p])&]
[s2;%RU-RU Устанавливает порт сервера. Возвращает 
`*this.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:SSL`(bool`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* SSL]([@(0.0.255) b
ool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%RU-RU Активирует режим SSL. Возвращает 
`*this.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:Path`(const String`&`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* P
ath]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 p])&]
[s2;%RU-RU Устанавливает путь запроса. Возвращает 
`*this.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:HttpRequest`:`:Authorization`(const Upp`:`:String`&`): [_^Upp`:`:HttpRequest^ H
ttpRequest][@(0.0.255) `&]_[* Authorization]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&
]_[*@3 h])&]
[s2;%RU-RU Устанавливает заглавие авторизации
 (текст между `"Authorization: `" и `"`\r`\n`" в заглавие). Предназначается для использования 
при обратном вызове WhenAuthenticate, чтобы предоставлять информацию логина.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:User`(const String`&`,const String`&`): [_^HttpRequest^ HttpRequest
][@(0.0.255) `&]_[* User]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 u], 
[@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 p])&]
[s2;%RU-RU Устанавливает пользователя и 
пароль, согласно [^http`:`/`/en`.wikipedia`.org`/wiki`/Basic`_access`_authentication^ Б
азовой аутентикации доступа]. Возвращает 
`*this.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:Digest`(`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* Digest]()&]
[s2;%RU-RU Заставляет HttpRequest использовать 
[^http`:`/`/en`.wikipedia`.org`/wiki`/Digest`_access`_authentication^ Дайджес
т`-аутентикацию доступа]. `- не отправляя 
базовой аутентикации. Возвращает 
`*this.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:HttpRequest`:`:Digest`(const Upp`:`:String`&`,const Upp`:`:String`&`): [_^Upp`:`:HttpRequest^ H
ttpRequest][@(0.0.255) `&]_[* Digest]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&
]_[*@3 u], [@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 p])&]
[s2;%RU-RU То же, что и User([%-*@3 u] , [%-*@3 p]).Digest().&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:HttpRequest`:`:SetDigest`(const Upp`:`:String`&`): [_^Upp`:`:HttpRequest^ H
ttpRequest][@(0.0.255) `&]_[* SetDigest]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&
]_[*@3 d])&]
[s2;%RU-RU Устанавливает дайджест. Это обычно 
выполняется HttpRequest, при обработке 
кода ответа 401. Возвращает `*this.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:Url`(const char`*`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* Url
]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 url])&]
[s2;%RU-RU Устанавливает улр запроса [%-*@3 url]. 
[%-*@3 url] парсируется, чтобы получить 
имя хоста, порт и путь, а затем вызываются 
методы Host, Port и Path. Если [%-*@3 url] начинается 
на `"https`", то активируется режим SSL 
посредством метода SSL. Возвращает 
`*this.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:UrlVar`(const char`*`,const String`&`): [_^HttpRequest^ HttpRequest
][@(0.0.255) `&]_[* UrlVar]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], 
[@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 data])&]
[s5;:HttpRequest`:`:operator`(`)`(const char`*`,const String`&`): [_^HttpRequest^ HttpR
equest][@(0.0.255) `&]_[* operator()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], 
[@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 data])&]
[s2;%RU-RU Добавляет пару имя/значение к 
пути в формате, который используется 
формами HTML при методе `"GET`". Возвращает 
`*this.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:PostData`(const String`&`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&
]_[* PostData]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 pd])&]
[s2;%RU-RU Устанавливает данные, постируемые 
с запросом. Возвращает `*this.&]
[s3;%RU-RU &]
[s4; &]
[s5;:HttpRequest`:`:PostStream`(Stream`&`,int64`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&
]_[* PostStream]([_^Stream^ Stream][@(0.0.255) `&]_[*@3 s], [_^int64^ int64]_[*@3 len]_`=_Nul
l)&]
[s2;%RU-RU Устанавливает поток данных, постируемых
 с запросом. [%-*@3 len] `- число байтов, отправляемы
х из потока, если Null, то отправляются 
все (остальные) данные из потока. Возвращает 
`*this.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:HttpRequest`:`:GetPostData`(`)const: [_^Upp`:`:String^ String]_[* GetPostData
]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает значение, установленное 
посредством PostData.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:HttpRequest`:`:ResolveDigestAuthentication`(`): [@(0.0.255) bool]_[* ResolveD
igestAuthentication]()&]
[s2;%RU-RU Этот метод присваивается WhenAuthenticate 
дефолтно. Предоставляет стандартную 
дайджест`-аутентикацию HTTP.&]
[s3; &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:PostUData`(const String`&`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&
]_[* PostUData]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 pd])&]
[s2;%RU-RU Устанавливает данные, постируемые 
с запросом, perform as url`-encoding of data. Возвращает 
`*this.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:Post`(const String`&`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* P
ost]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 data])&]
[s2;%RU-RU То же, что и POST().PostData([%-*@3 data]). Возвращает 
`*this.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:Post`(const char`*`,const String`&`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&
]_[* Post]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], [@(0.0.255) const]_[_^String^ Str
ing][@(0.0.255) `&]_[*@3 data])&]
[s2;%RU-RU Устанавливает метод для POST и пару 
имя/значение для данных поста в формате, 
используемом формами HTML, с методом 
`"POST`". Возвращает `*this.&]
[s3;%RU-RU &]
[s4; &]
[s5;:HttpRequest`:`:Part`(const char`*`,const String`&`,const char`*`,const char`*`): [_^HttpRequest^ H
ttpRequest][@(0.0.255) `&]_[* Part]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], 
[@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 data], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 content`_type]_`=_NULL, [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 filename]_
`=_NULL)&]
[s2;%RU-RU При первом вызове HttpRequest устанавливается
 в режим multipart (несколько частей); автоматичес
ки генерируется граница уникальной 
части. [%-*@3 id] `- имя части, [%-*@3 data] `- контент. 
Если [%-*@3 content`_type] не равен NULL и не пуст, 
он определяет заголовочник части 
`"Content`-Type`". [%-*@3 filename] может использоваться 
для определения части `"filename`" заголовочника 
части `"Content`-Disposition`".&]
[s3;%RU-RU &]
[s4; &]
[s5;:HttpRequest`:`:ClearPost`(`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* ClearPost
]()&]
[s2;%RU-RU Опорожняет все данные Post и устанавливает
 метод GET.&]
[s3; &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:Headers`(const String`&`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&
]_[* Headers]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 h])&]
[s2;%RU-RU Устанавливает текст, содержащий 
заголовочники HTTP, добавляемые после 
полей заголовочников, сгенерированных 
HttpRequest. Возвращает `*this.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:ClearHeaders`(`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* ClearH
eaders]()&]
[s2;%RU-RU То же, что и Headers(Null).&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:AddHeaders`(const String`&`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&
]_[* AddHeaders]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 h])&]
[s2;%RU-RU Добавляет текст, содержащий заголовочни
ки HTTP, помещаемые после полей заголовочников
, сгенерированных HttpRequest. Возвращает 
`*this.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:Header`(const char`*`,const String`&`): [_^HttpRequest^ HttpRequest
][@(0.0.255) `&]_[* Header]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], 
[@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 data])&]
[s2;%RU-RU Добавляет в заголовочники единичную 
пару имя/значение. Возвращает `*this.&]
[s3;%RU-RU &]
[s4; &]
[s5;:HttpRequest`:`:Cookie`(const HttpCookie`&`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&
]_[* Cookie]([@(0.0.255) const]_[_^HttpCookie^ HttpCookie][@(0.0.255) `&]_[*@3 c])&]
[s2;%RU-RU Устанавливает куки, которые будут 
отправляться при последующих запросах. 
Ряд `"Member`" HttpCookie игнорируется. Куки 
отсылаются, только когда путь и домен 
совпадают (можно установить в Null, 
чтобы совпадало всегда).&]
[s3;%RU-RU &]
[s4; &]
[s5;:HttpRequest`:`:Cookie`(const String`&`,const String`&`,const String`&`,const String`&`): [_^HttpRequest^ H
ttpRequest][@(0.0.255) `&]_[* Cookie]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_
[*@3 id], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 value], 
[@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 domain]_`=_Null, 
[@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 path]_`=_Null)&]
[s2;%RU-RU Устанавливает куки, которые будут 
отправляться при последующих запросах. 
Ряд `"Member`" HttpCookie игнорируется. Куки 
отсылаются, только когда путь и домен 
совпадают (можно установить в Null, 
чтобы совпадало всегда).&]
[s3;%RU-RU &]
[s4; &]
[s5;:HttpRequest`:`:CopyCookies`(const HttpRequest`&`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&
]_[* CopyCookies]([@(0.0.255) const]_[_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[*@3 r])&]
[s2;%RU-RU Копирует куки из другого HttpRequest.&]
[s3;%RU-RU &]
[s4; &]
[s5;:HttpRequest`:`:ClearCookies`(`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* ClearC
ookies]()&]
[s2;%RU-RU Очищает все куки.&]
[s3; &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:StdHeaders`(bool`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* StdH
eaders]([@(0.0.255) bool]_[*@3 sh])&]
[s2;%RU-RU Определяет, должен ли HttpRequest предоставля
ть стандартные поля заголовочника 
http (URL, Host, Connection, Accept, Accept`-Encoding, User`-Agent, 
Content`-Length, Content`-Type). Дефолт равен yes 
(да). Возвращает `*this.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:NoStdHeaders`(`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* NoStdH
eaders]()&]
[s2;%RU-RU То же, что и StdHeaders(false).&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:Accept`(const String`&`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_
[* Accept]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 a])&]
[s2;%RU-RU Устанавливает значение поля `"Accept`" 
в порции заголовочников, сгенерированной 
HttpRequest. Дефолт равен `"`*/`*`". Возвращает 
`*this.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:UserAgent`(const String`&`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&
]_[* UserAgent]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 a])&]
[s2;%RU-RU Задаёт поле запроса HTTP `"User`-Agent`". 
Дефолт равен `"U`+`+ HTTP request`". Возвращает 
`*this.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:ContentType`(const String`&`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&
]_[* ContentType]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 a])&]
[s2;%RU-RU Задаёт поле запроса HTTP `"Content`-Type`". 
Возвращает `*this.&]
[s3;%RU-RU &]
[s4; &]
[s5;:HttpRequest`:`:KeepAlive`(bool`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* KeepA
live]([@(0.0.255) bool]_[*@3 ka]_`=_[@(0.0.255) true])&]
[s2;%RU-RU Если активен, поле запроса HTTP 
`"Connection`" равно `"keep`-alive`", иначе `"close`". 
Возвращает `*this.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:Proxy`(const String`&`,int`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&
]_[* Proxy]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 host], 
[@(0.0.255) int]_[*@3 port])&]
[s2;%RU-RU Определяет прокси http. Возвращает 
`*this.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:Proxy`(const char`*`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_[* P
roxy]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 p])&]
[s2;%RU-RU Определяет прокси http, [%-*@3 p] должно 
содержать `"hostname:port`". Если порт отсутствует, 
он устанавливается в 80. Возвращает 
`*this.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:ProxyAuth`(const String`&`,const String`&`): [_^HttpRequest^ HttpRe
quest][@(0.0.255) `&]_[* ProxyAuth]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 u
], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 p])&]
[s2;%RU-RU Определяет аутентикацию по username 
и password для прокси. Возвращает `*this.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:SSLProxy`(const String`&`,int`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&
]_[* SSLProxy]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 host], 
[@(0.0.255) int]_[*@3 port])&]
[s2;%RU-RU Определяет прокси для подключения 
SSL. Возвращает `*this.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:SSLProxy`(const char`*`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&]_
[* SSLProxy]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 p])&]
[s2;%RU-RU Определяет прокси для подключения 
SSL, [%-*@3 p] должно содержать `"hostname:port`". 
Если порт отсутствует, он устанавливается 
в 8080. Возвращает `*this.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:SSLProxyAuth`(const String`&`,const String`&`): [_^HttpRequest^ Htt
pRequest][@(0.0.255) `&]_[* SSLProxyAuth]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[*@3 u], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 p])&]
[s2;%RU-RU Определяет аутентикацию по username 
и password для прокси SSL. Возвращает `*this.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:HttpRequest`:`:SSLProxyGET`(bool`): [_^Upp`:`:HttpRequest^ HttpRequest][@(0.0.255) `&
]_[* SSLProxyGET]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%RU-RU Заставляет SSL использовать тип 
прокси GET вместо метода CONNECT.&]
[s3;%RU-RU &]
[s4; &]
[s5;:HttpRequest`:`:CommonProxy`(const String`&`,int`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&
]_[* CommonProxy]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 host], 
[@(0.0.255) int]_[*@3 port])&]
[s2;%RU-RU Определяет как SSL, так и не`-SSL прокси, 
с одинаковыми [%-*@3 host] и [%-*@3 port]. То же, 
что и Proxy(host, port); SSLProxy(host, port). Возвращает 
`*this.&]
[s3;%RU-RU &]
[s4; &]
[s5;:HttpRequest`:`:CommonProxy`(const char`*`): [_^HttpRequest^ HttpRequest][@(0.0.255) `&
]_[* CommonProxy]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 p])&]
[s2;%RU-RU Определяет как SSL, так и не`-SSL прокси, 
с одинаковым сервером. То же, что и 
Proxy(p); SSLProxy(p), включая дефолтные определения 
портов. Возвращает `*this.&]
[s3;%RU-RU &]
[s4; &]
[s5;:HttpRequest`:`:CommonProxyAuth`(const String`&`,const String`&`): [_^HttpRequest^ H
ttpRequest][@(0.0.255) `&]_[* CommonProxyAuth]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[*@3 u], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 p])&]
[s2;%RU-RU Определяет аутентикацию по username 
и password, как для SSL, так и для не`-SSL прокси. 
То же, что и ProxyAuth(u, p); SSLProxyAuth(u, p). Возвращает 
`*this.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:IsSocketError`(`)const: [@(0.0.255) bool]_[* IsSocketError]()_[@(0.0.255) c
onst]&]
[s2;%RU-RU Возвращает true, если имелась ошибка 
на уровне сокета.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:IsHttpError`(`)const: [@(0.0.255) bool]_[* IsHttpError]()_[@(0.0.255) c
onst]&]
[s2;%RU-RU Возвращает true, если имелась ошибка 
на уровне протокола HTTP.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:IsError`(`)const: [@(0.0.255) bool]_[* IsError]()_[@(0.0.255) const]&]
[s2;%RU-RU То же, что и IsSocketError() `|`| IsHttpError().&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:GetErrorDesc`(`)const: [_^String^ String]_[* GetErrorDesc]()_[@(0.0.255) c
onst]&]
[s2;%RU-RU Возвращает описание ошибки.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:ClearError`(`): [@(0.0.255) void]_[* ClearError]()&]
[s2;%RU-RU Удаляет все ошибки.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:GetHeader`(const char`*`): [_^String^ String]_[* GetHeader]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 id])&]
[s5;:HttpRequest`:`:operator`[`]`(const char`*`): [_^String^ String]_[* operator`[`]]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 id])&]
[s2;%RU-RU Возвращает поле заголовочника 
ответа. [%-*@3 id] должен быть в нижнем 
регистре.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:GetRedirectUrl`(`): [_^String^ String]_[* GetRedirectUrl]()&]
[s2;%RU-RU Возвращает обработанное поле 
`"Location`" заголовочника ответа HTTP.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:HttpRequest`:`:HasContentLength`(`): [@(0.0.255) bool]_[* HasContentLength]()
&]
[s2;%RU-RU Возвращает true, если у полученного 
заголовочника HTTP имеется поле `"content`-length`".&]
[s3; &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:GetContentLength`(`): [@(0.0.255) int64]_[* GetContentLength]()&]
[s2;%RU-RU Возвращает длину контента, как 
указано в звголовочнике ответа HTTP, 
либо 0, если не указано, или заглавие 
ещё не загружено.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:GetStatusCode`(`)const: [@(0.0.255) int]_[* GetStatusCode]()_[@(0.0.255) c
onst]&]
[s2;%RU-RU Если запрос окончен, возвращает 
код состояния HTTP.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:GetReasonPhrase`(`)const: [_^String^ String]_[* GetReasonPhrase]()_[@(0.0.255) c
onst]&]
[s2;%RU-RU Если запрос окончен, возвращает 
фразу `"резона`" HTTP (reason phrase).&]
[s3;%RU-RU &]
[s4; &]
[s5;:HttpRequest`:`:GetHttpHeader`(`)const: [@(0.0.255) const]_[_^HttpHeader^ HttpHeader][@(0.0.255) `&
]_[* GetHttpHeader]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает HttpHeader ответа.&]
[s3; &]
[s4; &]
[s5;:HttpRequest`:`:GetCookie`(const char`*`): [_^String^ String]_[* GetCookie]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 id])&]
[s2;%RU-RU Возвращает значение куки [%-*@3 id] 
ответа.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:GetContent`(`)const: [_^String^ String]_[* GetContent]()_[@(0.0.255) co
nst]&]
[s5;:HttpRequest`:`:operator`~`(`)const: [_^String^ String]_[* operator`~]()_[@(0.0.255) co
nst]&]
[s5;:HttpRequest`:`:operator String`(`)const: [* operator_String]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает текущий результат 
сбора контента.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:ClearContent`(`): [@(0.0.255) void]_[* ClearContent]()&]
[s2;%RU-RU Очищает итоговый контент, уменьшая 
занимаемую память. Может вызываться 
в любое время.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:Do`(`): [@(0.0.255) bool]_[* Do]()&]
[s2;%RU-RU Продвигает запрос. Максимальная 
продолжительность определена Socket`::Timeout. 
Возвращает true, если обработка не завершена.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:HttpRequest`:`:GetWaitEvents`(`): [_^Upp`:`:dword^ dword]_[* GetWaitEvents]()
&]
[s2;%RU-RU Возвращает комбинацию флагов 
WAIT`_READ и WAIT`_WRITE, указывая на причину 
блокировки прогрессирования запроса 
HTTP. Может использоваться с SocketWaitEvent.&]
[s3; &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:GetPhase`(`)const: [@(0.0.255) int]_[* GetPhase]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает текущую фазу обработки 
запроса, что`-либо из BEGIN, START, DNS, SSLPROXYREQUEST, 
SSLPROXYRESPONSE, SSLHANDSHAKE, REQUEST, HEADER, BODY, CHUNK`_HEADER, 
CHUNK`_BODY, TRAILER, FINISHED, FAILED.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:GetPhaseName`(`)const: [_^String^ String]_[* GetPhaseName]()_[@(0.0.255) c
onst]&]
[s2;%RU-RU Возвращает текстовое описание 
текущей фазы запроса.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:InProgress`(`)const: [@(0.0.255) bool]_[* InProgress]()_[@(0.0.255) con
st]&]
[s2;%RU-RU Возвращает true, если запрос всё 
ещё осуществляется.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:IsFailure`(`)const: [@(0.0.255) bool]_[* IsFailure]()_[@(0.0.255) const
]&]
[s2;%RU-RU Запрос неудался. Имелась ошибка 
коммуникации или протокола HTTP. Удачно 
завершённые запросы, с кодом 2xx, считаются 
удачными.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:IsSuccess`(`)const: [@(0.0.255) bool]_[* IsSuccess]()_[@(0.0.255) const
]&]
[s2;%RU-RU Удачный запрос, завершённый кодом 
2xx. Заметьте, что IsSuccess не комплементарен 
с IsFailure (запрос может, напр., заканчиваться 
кодом 404, который означает, что как 
IsFailure(), [/ так и] IsSuccess() , равны false.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:Execute`(`): [_^String^ String]_[* Execute]()&]
[s2;%RU-RU Выполняет весь запрос, возвращает 
итоговый контент при успехе, либо 
String`::GetVoid() при неудаче.&]
[s3;%RU-RU &]
[s4; &]
[s5;:HttpRequest`:`:New`(`): [@(0.0.255) void]_[* New]()&]
[s2;%RU-RU Начинает новый запрос HTTP. В отличие 
от Clear, не меняет никаких уже существующих 
настроек. Не прерывает подключения 
к серверу по TCP. Новый запрос использует 
куки, полученные при предыдущих запросах.&]
[s3; &]
[s4; &]
[s5;:HttpRequest`:`:NewRequest`(`): [@(0.0.255) void]_[* NewRequest]()&]
[s2;%RU-RU Начинает новый запрос HTTP, не прерывая 
подключения к серверу. Устанавливает 
все настройки в исходное состояние, 
за исключением полученных куки, `- 
они отправляются с новым запросом.&]
[s3; &]
[s4; &]
[s5;:HttpRequest`:`:Clear`(`): [@(0.0.255) void]_[* Clear]()&]
[s2;%RU-RU Устанавливает запрос HTTP в начальное 
дефолтное состояние (то же, что вновь 
построенный объект), включая куки. 
Прерывает подключение к серверу TCP.&]
[s3; &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:HttpRequest`(`): [* HttpRequest]()&]
[s2;%RU-RU Дефолтный конструктор.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:HttpRequest`(const char`*`): [* HttpRequest]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 url])&]
[s2;%RU-RU Конструирует HttpRequest и вызывает 
Url([%-*@3 url]).&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:HttpRequest`:`:Trace`(bool`): [@(0.0.255) static] [@(0.0.255) void]_[* Trace]([@(0.0.255) b
ool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%RU-RU Активирует/дезактивирует глобальное 
логгирование для HttpRequest`'ов (дефолтно 
неактивен).&]
[s3;%RU-RU ]]