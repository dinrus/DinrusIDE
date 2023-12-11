topic "Вспомогательные функции Интернет-формата";
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
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Вспомогательные функции Интернет`-фор
мата]]}}&]
[s3; &]
[s5;:WwwFormat`(Time`): [_^String^ String]_[* WwwFormat]([_^Time^ Time]_[*@3 tm])&]
[s2;%RU-RU Возвращает время Time в формате, 
определенном в RFC2822, широко используемом 
в HTTP и других протоколах Интернета, 
типа `"Sun, 15 Apr 2012 16:00:25 GMT`".&]
[s3;%RU-RU &]
[s4; &]
[s5;:ScanWwwTime`(const char`*`,Time`&`): [@(0.0.255) bool]_[* ScanWwwTime]([@(0.0.255) con
st]_[@(0.0.255) char]_`*[*@3 s], [_^Time^ Time][@(0.0.255) `&]_[*@3 tm])&]
[s2;%RU-RU Сканирует формат времени Time, как 
определено в RFC2822, возвращает true при 
успехе.&]
[s3;%RU-RU &]
[s4; &]
[s5;:ScanWwwTime`(const char`*`): [_^Time^ Time]_[* ScanWwwTime]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 s])&]
[s2;%RU-RU Сканирует формат времени Time, как 
определено в RFC2822, возвращает Null при 
неудаче.&]
[s3;%RU-RU &]
[s4; &]
[s5;:MIMECharsetName`(byte`): [_^String^ String]_[* MIMECharsetName]([_^byte^ byte]_[*@3 char
set])&]
[s2;%RU-RU Возвращает набсим U`+`+ [%-*@3 charset], 
форматированный как требуется в соответстви
и с MIME.&]
[s3;%RU-RU &]
[s4; &]
[s5;:UrlEncode`(const char`*`,const char`*`): [_^String^ String]_[* UrlEncode]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 s], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 end])&]
[s5;:UrlEncode`(const char`*`,int`): [_^String^ String]_[* UrlEncode]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 s], [@(0.0.255) int]_[*@3 len])&]
[s5;:UrlEncode`(const String`&`): [_^String^ String]_[* UrlEncode]([@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 s])&]
[s2;%RU-RU Кодирует данные, как требуется 
согласно [^http`:`/`/www`.w3`.org`/TR`/html401`/interact`/forms`.html`#h`-17`.13`.4`.1^ M
IME type application/x`-www`-form`-urlencoded]. Алфавитные 
символы ASCII и символы `'.`', `'`-`', `'`_`' 
передаются непосредственно, пробел 
представляется как `'`+`', а всё прочее 
как %HH, где HH `- 16`-ричное число с двумя 
числами.&]
[s3;%RU-RU &]
[s4; &]
[s5;:UrlDecode`(const char`*`,const char`*`): [_^String^ String]_[* UrlDecode]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 s], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 end])&]
[s5;:UrlDecode`(const char`*`,int`): [_^String^ String]_[* UrlDecode]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 s], [@(0.0.255) int]_[*@3 len])&]
[s5;:UrlDecode`(const String`&`): [_^String^ String]_[* UrlDecode]([@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 s])&]
[s2;%RU-RU Декодирует данные, как требуется 
согласно [^http`:`/`/www`.w3`.org`/TR`/html401`/interact`/forms`.html`#h`-17`.13`.4`.1^ M
IME type application/x`-www`-form`-urlencoded] (смотрите 
UrlEncode).&]
[s3;%RU-RU &]
[s4; &]
[s5;:QPEncode`(const char`*`): [_^String^ String]_[* QPEncode]([@(0.0.255) const]_[@(0.0.255) c
har`*]_[*@3 s])&]
[s2;%RU-RU Кодирует строку в формат [^http`:`/`/en`.wikipedia`.org`/wiki`/Quoted`-printable^ q
uoted`-printable].&]
[s3;%RU-RU &]
[s4; &]
[s5;:QPDecode`(const char`*`,bool`): [_^String^ String]_[* QPDecode]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 s], [@(0.0.255) bool]_[*@3 underscore`_to`_space]_`=_[@(0.0.255) false])&]
[s2;%RU-RU Декодирует строку из формата 
[^http`:`/`/en`.wikipedia`.org`/wiki`/Quoted`-printable^ quoted`-printable]. 
Если [%-*@3 s] [%-*@3 underscore`_to`_space] равно true, 
функция заменяет `'`_`' на пробел `' `' 
(как требуется согласно [^http`:`/`/en`.wikipedia`.org`/wiki`/MIME`#Encoded`-Word^ e
ncoded`-word]).&]
[s3;%RU-RU &]
[s4; &]
[s5;:Base64Encode`(const char`*`,const char`*`): [_^String^ String]_[* Base64Encode]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 b], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 e])&]
[s5;:Base64Encode`(const char`*`,int`): [_^String^ String]_[* Base64Encode]([@(0.0.255) con
st]_[@(0.0.255) char]_`*[*@3 s], [@(0.0.255) int]_[*@3 len])&]
[s5;:Base64Encode`(const String`&`): [_^String^ String]_[* Base64Encode]([@(0.0.255) const]_
[_^String^ String][@(0.0.255) `&]_[*@3 data])&]
[s2;%RU-RU Кодирует двоичные данные [%-*@3 data] 
в формат [^http`:`/`/en`.wikipedia`.org`/wiki`/Base64^ Base64].&]
[s3;%RU-RU &]
[s4; &]
[s5;:Base64Decode`(const char`*`,const char`*`): [_^String^ String]_[* Base64Decode]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 b], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 e])&]
[s5;:Base64Decode`(const char`*`,int`): [_^String^ String]_[* Base64Decode]([@(0.0.255) con
st]_[@(0.0.255) char]_`*[*@3 s], [@(0.0.255) int]_[*@3 len])&]
[s5;:Base64Decode`(const String`&`): [_^String^ String]_[* Base64Decode]([@(0.0.255) const]_
[_^String^ String][@(0.0.255) `&]_[*@3 data])&]
[s2;%RU-RU Декодирует двоичные данные [%-*@3 data] 
из формата [^http`:`/`/en`.wikipedia`.org`/wiki`/Base64^ Base64].&]
[s3; &]
[s4; &]
[s5;:DeHtml`(const char`*`): [_^String^ String]_[* DeHtml]([@(0.0.255) const]_[@(0.0.255) cha
r]_`*[*@3 s])&]
[s2;%RU-RU Эскапирует символы `'<`', `'>`', `'`&`', 
`'`\`"`' и символы < 31 как сущности html, 
а символ 31 как `"`&nbsp;`".&]
[s3;%RU-RU &]
[s4; &]
[s5;:GetMIMETypes`(`): [@(0.0.255) const]_[_^Index^ Index]<[_^String^ String]>`&_[* GetMIMETy
pes]()&]
[s2;%RU-RU Возвращает список типов MIME (известных 
U`+`+).&]
[s3; &]
[s4; &]
[s5;:FileExtToMIME`(const String`&`): [_^String^ String]_[* FileExtToMIME]([@(0.0.255) cons
t]_[_^String^ String][@(0.0.255) `&]_[*@3 ext])&]
[s2;%RU-RU Преобразует расширение файла 
в тип MIME. [%-*@3 ext] может, но не обязательно 
должно, начинаться с `'.`' (она удаляется 
перед поиском).&]
[s3;%RU-RU &]
[s4; &]
[s5;:MIMEToFileExt`(const String`&`): [_^String^ String]_[* MIMEToFileExt]([@(0.0.255) cons
t]_[_^String^ String][@(0.0.255) `&]_[*@3 mime])&]
[s2;%RU-RU Преобразует тип MIME в наиболее 
вероятное расширение файла (без `'.`' 
в начале).&]
[s3;%RU-RU &]
[s4; &]
[s5;:HttpResponse`(TcpSocket`&`,bool`,int`,const char`*`,const char`*`,const String`&`,const char`*`,bool`): [@(0.0.255) b
ool]_[* HttpResponse]([_^TcpSocket^ TcpSocket][@(0.0.255) `&]_[*@3 socket], 
[@(0.0.255) bool]_[*@3 scgi], [@(0.0.255) int]_[*@3 code], [@(0.0.255) const]_[@(0.0.255) cha
r]_`*[*@3 phrase], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 content`_type]_`=_NULL, 
[@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 data]_`=_Null, 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 server]_`=_NULL, [@(0.0.255) bool]_[*@3 gzip]_`=
_[@(0.0.255) false])&]
[s2;%RU-RU Эта функция посылает простой 
ответ HTTP через сокет [%-*@3 socket]. Если 
[%-*@3 scgi] равно true, то первая строка ответа 
форматируется для SCGI (остальные одинаковые 
для обычного HTTP и SCGI). [%-*@3 code] и [%-*@3 phrase] 
устанавливают первую строку ответа, 
[%-*@3 content`_type] устанавливает соотстветствующее
 заглавие http (если NULL, он не отправляется
). Если [%-*@3 data] не пуст, отправляется 
заглавие `"content`-length`", а после него 
отправляются даные [%-*@3 data]. [%-*@3 server] 
устанавливает поле `"server`" заглавия 
http, если он отстутствует, то устанавливается
 как `"U`+`+ based server`". Если [%-*@3 gzip] равно 
true, функция добавляет заглавие 
`"content`-encoding: gzip`", но в действительности 
НЕ помещает контент в архив zip (это 
зависит от вызывающего).&]
[s3; &]
[s0;%RU-RU ]]