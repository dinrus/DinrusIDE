topic "Структура HttpHeader";
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
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Структура HttpHeader]]}}&]
[s3; &]
[s1;:HttpHeader`:`:struct: [@(0.0.255)3 struct][3 _][*3 HttpHeader]&]
[s2;%RU-RU Эта простая структура упрощает 
разбор заголовочников HTTP request и response. 
Также она может разбирать запросы 
SCGI.&]
[s3;%RU-RU &]
[s4; &]
[s5;:HttpHeader`:`:first`_line: [_^String^ String]_[* first`_line]&]
[s2;%RU-RU Первая строка request/response (запроса/ответа) 
`- контент этой строки для запроса 
и ответа различен.&]
[s3; &]
[s4; &]
[s5;:HttpHeader`:`:f3: [_^String^ String]_[* f1, f2, f3]&]
[s2;%RU-RU Первая, вторая и третья части 
первой строки. Контент различен для 
запроса и ответа.&]
[s3; &]
[s4; &]
[s5;:HttpHeader`:`:fields: [_^VectorMap^ VectorMap]<[_^String^ String], 
[_^String^ String]>_[* fields]&]
[s2;%RU-RU Поля Http в виде пар ключ`-значение. 
Ключи преобразуются в нижний регистр. 
Заметьте, что один и тот же ключ может 
присутствовать несколько раз.&]
[s3; &]
[s4; &]
[s5;:HttpHeader`:`:cookies: [_^VectorMap^ VectorMap]<[_^String^ String], 
[_^HttpCookie^ HttpCookie]>_[* cookies]&]
[s2;%RU-RU Набор из куков запроса http (заполняется 
в зависимости от заголовочников Set`-Cookie, 
встречающихся при парсинге).&]
[s3; &]
[s4; &]
[s5;:HttpHeader`:`:scgi: [@(0.0.255) bool]_[* scgi]&]
[s2;%RU-RU Указывает, что этот заголовочник 
`- результат парсинга scgi.&]
[s3; &]
[s4; &]
[s5;:HttpHeader`:`:operator`[`]`(const char`*`)const: [_^String^ String]_[* operator`[`]](
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id])_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает первое поле заголовочника 
http с ключом [%-*@3 id] (должен быть в нижнем 
регистре) или пустую строку, если 
такое поле отсутствует.&]
[s3;%RU-RU &]
[s4; &]
[s5;:HttpHeader`:`:GetCookie`(const char`*`)const: [_^String^ String]_[* GetCookie]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 id])_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает значение первого куки 
ответа (установленное посредством 
Set`-Cookie) с именем [%-*@3 id].&]
[s3;%RU-RU &]
[s4; &]
[s5;:HttpHeader`:`:Response`(String`&`,int`&`,String`&`)const: [@(0.0.255) bool]_[* Respo
nse]([_^String^ String][@(0.0.255) `&]_[*@3 protocol], [@(0.0.255) int`&]_[*@3 code], 
[_^String^ String][@(0.0.255) `&]_[*@3 reason])_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает поля первой строки 
ответа http.&]
[s3;%RU-RU &]
[s4; &]
[s5;:HttpHeader`:`:Request`(String`&`,String`&`,String`&`)const: [@(0.0.255) bool]_[* Req
uest]([_^String^ String][@(0.0.255) `&]_[*@3 method], [_^String^ String][@(0.0.255) `&]_[*@3 u
ri], [_^String^ String][@(0.0.255) `&]_[*@3 version])_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает поля первой строки 
запроса http.&]
[s3;%RU-RU &]
[s4; &]
[s5;:HttpHeader`:`:GetProtocol`(`)const: [_^String^ String]_[* GetProtocol]()_[@(0.0.255) c
onst]&]
[s2;%RU-RU Возвращает часть `"протокол`" первой 
строки ответа.&]
[s3; &]
[s4; &]
[s5;:HttpHeader`:`:GetCode`(`)const: [@(0.0.255) int]_[* GetCode]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает часть `"код возврата`" 
первой строки ответа.&]
[s3; &]
[s4; &]
[s5;:HttpHeader`:`:GetReason`(`)const: [_^String^ String]_[* GetReason]()_[@(0.0.255) const
]&]
[s2;%RU-RU Возвращает часть `"reson`" первой 
строки ответа.&]
[s3; &]
[s4; &]
[s5;:HttpHeader`:`:GetMethod`(`)const: [_^String^ String]_[* GetMethod]()_[@(0.0.255) const
]&]
[s2;%RU-RU Возвращает часть `"method`" первой 
строки ответа.&]
[s3; &]
[s4; &]
[s5;:HttpHeader`:`:GetURI`(`)const: [_^String^ String]_[* GetURI]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает часть `"URI`" первой строки 
запроса.&]
[s3; &]
[s4; &]
[s5;:HttpHeader`:`:GetVersion`(`)const: [_^String^ String]_[* GetVersion]()_[@(0.0.255) con
st]&]
[s2;%RU-RU Возвращает часть `"version`" первой 
строки запроса.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:HttpHeader`:`:HasContentLength`(`)const: [@(0.0.255) bool]_[* HasContentLengt
h]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает true, если у заголовочника 
есть поле `"content`-length`".&]
[s3; &]
[s4; &]
[s5;:HttpHeader`:`:GetContentLength`(`)const: [_^int64^ int64]_[* GetContentLength]()_[@(0.0.255) c
onst]&]
[s2;%RU-RU Возвращает длину контента, указанную 
в заголовочнике, или 0, если не указана 
или заголовочник ещё не загружен.&]
[s3; &]
[s4; &]
[s5;:HttpHeader`:`:Clear`(`): [@(0.0.255) void]_[* Clear]()&]
[s2;%RU-RU Очищает данные.&]
[s3; &]
[s4; &]
[s5;:HttpHeader`:`:ParseAdd`(const String`&`): [@(0.0.255) bool]_[* ParseAdd]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[*@3 hdrs])&]
[s2;%RU-RU Разбирает заголовочник (все строки 
заголовочника) в [* first`_line ]и [* fields].&]
[s3;%RU-RU &]
[s4; &]
[s5;:HttpHeader`:`:Parse`(const String`&`): [@(0.0.255) bool]_[* Parse]([@(0.0.255) const]_
[_^String^ String][@(0.0.255) `&]_[*@3 hdrs])&]
[s2;%RU-RU Вызывает Clear и разбирает заголовочник 
(все строки заголовочника) в [* first`_line 
]и [* fields].&]
[s3;%RU-RU &]
[s4; &]
[s5;:HttpHeader`:`:ParseSCGI`(const String`&`): [@(0.0.255) bool]_[* ParseSCGI]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[*@3 scgi`_hdr])&]
[s2;%RU-RU Разбирает заголовочник SCGI, в итоге 
он выглядит как обычный заголовочник 
HTTP.&]
[s3;%RU-RU &]
[s4; &]
[s5;:HttpHeader`:`:Read`(TcpSocket`&`): [@(0.0.255) bool]_[* Read]([_^TcpSocket^ TcpSocket][@(0.0.255) `&
]_[*@3 socket])&]
[s2;%RU-RU Считывает заголовочник из сокета 
[%-*@3 socket] и разбирает его. Запросы SCGI 
детектируются методом по первому 
символу запроса `-  если это цифра, 
то предполагается SCGI и заголовочник 
парсируется как SCGI.&]
[s3;%RU-RU &]
[s4; &]
[s5;:HttpHeader`:`:HttpHeader`(`): [* HttpHeader]()&]
[s2;%RU-RU Дефолтный конструктор.&]
[s0;%RU-RU ]]