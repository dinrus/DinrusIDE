topic "Поддержка набора символов (гарнитур)";
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_} 
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Поддержка набора символов 
(гарнитур)]]}}&]
[s9;%RU-RU В идеальном мире каждый текстовый 
ресурс мог бы быть закодирован в кодировке 
Unicode или UTF`-8, но на практике нам приходится 
иметь дело со множеством 8`-битных 
кодировок. U`+`+ имеет [^topic`:`/`/Core`/src`/CharSet`_ru`-ru`#AddCharSet`(const char`*`,const word`*`,byte`)^ р
асширяемую] поддержку различных кодировок 
(наборов символов, `"набсимов`", `"charsets`"). 
Для выражения набсима прямо определены 
следующие константы (имена говорят 
сами за себя):&]
[s0; &]
[ {{3333:3333:3334f4;g0;^ [s0; [* CHARSET`_ISO8859`_1]&]
[s0; [* CHARSET`_ISO8859`_2]&]
[s0; [* CHARSET`_ISO8859`_3]&]
[s0; [* CHARSET`_ISO8859`_4]&]
[s0; [* CHARSET`_ISO8859`_5]&]
[s0; [* CHARSET`_ISO8859`_6]&]
[s0; [* CHARSET`_ISO8859`_7]&]
[s0; [* CHARSET`_ISO8859`_8]&]
[s0; [* CHARSET`_ISO8859`_9]]
:: [s0; [* CHARSET`_ISO8859`_10]&]
[s0; [* CHARSET`_ISO8859`_13]&]
[s0; [* CHARSET`_ISO8859`_14]&]
[s0; [* CHARSET`_ISO8859`_15]&]
[s0; [* CHARSET`_ISO8859`_16]&]
[s0; [* CHARSET`_WIN1250]&]
[s0; [* CHARSET`_WIN1251]&]
[s0; [* CHARSET`_WIN1252]&]
[s0; [* CHARSET`_WIN1253]]
:: [s0; [* CHARSET`_WIN1254]&]
[s0; [* CHARSET`_WIN1255]&]
[s0; [* CHARSET`_WIN1256]&]
[s0; [* CHARSET`_WIN1257]&]
[s0; [* CHARSET`_WIN1258]&]
[s0; [* CHARSET`_KOI8`_R]&]
[s0; [* CHARSET`_CP852]&]
[s0; [* CHARSET`_MJK]&]
[s0; [* CHARSET`_CP850]]}}&]
[s9; Ещё есть несколько особых значений 
набсима&]
[s0; &]
[s5;:CHARSET`_DEFAULT: [* CHARSET`_DEFAULT]&]
[s2;%RU-RU Представляяет `"дефолтный`" набсим. 
Дефолтный набсим можно установить 
с помощью SetDefaultCharset. Он используется 
вместо `"реального`" набсима во многих 
связанных с набсимом операциях (и 
обычно используется как дефолтное 
значение параметра). Гарантированно 
равен 0.&]
[s3; &]
[s4; &]
[s5;:CHARSET`_TOASCII: [* CHARSET`_TOASCII]&]
[s2;%RU-RU Это набсим, используемый при преобразова
нии&]
[s3; &]
[s4; &]
[s5;:CHARSET`_UTF8: [* CHARSET`_UTF8]&]
[s2;%RU-RU Кодировка UTF`-8.&]
[s3;%RU-RU &]
[s4; &]
[s5;:DEFAULTCHAR: [@(0.0.255) enum]_[* DEFAULTCHAR]_`=_[@3 0]x1f_&]
[s2;%RU-RU Это специальное значение используется 
как итог преобразования на месте 
символов, которых не существует в 
целевом набсиме.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Функций]]}}&]
[s3;^byte^ &]
[s5;:GetDefaultCharset`(`): [_^byte^ byte]_[* GetDefaultCharset]()&]
[s2;%RU-RU Возвращает текущий дефолтный 
набсим.&]
[s3; &]
[s4; &]
[s5;:SetDefaultCharset`(byte`): [@(0.0.255) void]_[* SetDefaultCharset]([_^byte^ byte]_[*@3 c
harset])&]
[s2;%RU-RU Устанавливает дефолтный [%-*@3 charset]. 
Это для поддержки `"legacy`"`-приложения; 
новое приложение должно постоянно 
пользоваться UTF8.&]
[s3;%RU-RU &]
[s4; &]
[s5;:ResolveCharset`(byte`): [_^byte^ byte]_[* ResolveCharset]([_^byte^ byte]_[*@3 charset])&]
[s2;%RU-RU Если [%-*@3 charset].равен DEFAULT`_CHARSET, возвращаетс
я GetDefaultCharset(), в ином случае возвращается 
[%-*@3 charset].&]
[s3;%RU-RU &]
[s4; &]
[s5;:AddCharSet`(const char`*`,const word`*`,byte`): [_^byte^ byte]_[* AddCharSet]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 name], [@(0.0.255) const]_[_^word^ word]_`*[*@3 table], 
[_^byte^ byte]_[*@3 systemcharset]_`=_CHARSET`_DEFAULT)&]
[s2;%RU-RU Добавляет новый набсим с названием 
[%-*@3 name]. [%-*@3 table] должна указывать на 
128 элементов и содержать UNICODE code`-points 
для значений символов 128`-255. Коды символов, 
не определённые в UNICODE или в новом 
набсиме, должны устанавливаться в 
CUNDEF. [%-*@3 systemcharset  ]может содержать эквивалентн
ый `"типичный`" набсим хостовой платформы, 
как опциональную вспомогательную 
информацию. Возвращает  код для нового 
набсима. [%-*@3 table] должна существовать 
до завершения программы (сохраняется 
только указатель на неё).&]
[s3;%RU-RU &]
[s4; &]
[s5;:AddCharSetE`(const char`*`,word`*`,byte`): [_^byte^ byte]_[* AddCharSetE]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 name], [_^word^ word]_`*[*@3 table], [_^byte^ byte]_[*@3 syste
mcharset]_`=_CHARSET`_DEFAULT)&]
[s2;%RU-RU Аналогично [^topic`:`/`/Core`/src`/CharSet`_ru`-ru`#AddCharSetE`(const char`*`,word`*`,byte`)^ A
ddCharSet], но любые значения CUNDEF в таблице 
[%-*@3 table] заменяются в символах в особом 
приватном диапазоне 0xee00`-0xeeff. U`+`+ использует 
эту область как `"unicode error escape`", mapping 
there делает возможным без потерь преобразовыв
ать в Юникод и обратно, даже если некоторым 
символам не присвоен кодпойнт. Таблица 
[%-*@3 table] должна существовать до завершения 
программы (сохраняется только указатель 
на неё).&]
[s3;%RU-RU &]
[s4; &]
[s5;:CharsetName`(byte`): [@(0.0.255) const]_[@(0.0.255) char]_`*[* CharsetName]([_^byte^ byt
e]_[*@3 charset])&]
[s2;%RU-RU Возвращает название кода набсима 
[%-*@3 charset].&]
[s3;%RU-RU &]
[s4; &]
[s5;:CharsetCount`(`): [@(0.0.255) int]_[* CharsetCount]()&]
[s2;%RU-RU Возвращает общее число набсимов 
(UTF`-8 исключается). Это гарантирует, 
что числовое значение кода набсима 
для `"реального`" набсима находится 
в диапазоне 1...CharsetCount().&]
[s3; &]
[s4; &]
[s5;:CharsetByName`(const char`*`): [@(0.0.255) int]_[* CharsetByName]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 name])&]
[s2;%RU-RU Пытается найти код набсима по 
имени [%-*@3 name]. Сравнение нерегистрочувствител
ьно, игнорируются все символы, кроме 
цифр и `"альф`". Если набсим неидентифицирован
, возвращается 0 (что то же самое, что 
и CHARSET`_DEFAULT).&]
[s3;%RU-RU &]
[s4; &]
[s5;:SystemCharset`(byte`): [_^byte^ byte]_[* SystemCharset]([_^byte^ byte]_[*@3 charset])&]
[s2;%RU-RU Пытается получить типичный для 
хрстплатформы набсим [%-*@3 charset]. Если 
неудачно, возвращает 0.&]
[s3;%RU-RU &]
[s4; &]
[s5;:ToUnicode`(int`,byte`): [@(0.0.255) int]_[* ToUnicode]([@(0.0.255) int]_[*@3 chr], 
[_^byte^ byte]_[*@3 charset])&]
[s2;%RU-RU Преобразует 8`-битный кодированный 
символ в unicode. [%-*@3 charset ][%- не может быть] 
CHARSET`_UTF8.&]
[s3;%RU-RU &]
[s4; &]
[s5;:FromUnicode`(wchar`,byte`,int`): [@(0.0.255) int]_[* FromUnicode]([_^wchar^ wchar]_[*@3 w
chr], [_^byte^ byte]_[*@3 charset], [@(0.0.255) int]_[*@3 defchar]_`=_DEFAULTCHAR)&]
[s2;%RU-RU Преобразует символ Юникода в 
8`-битную кодировку. Если кодпойнта 
не существет в заданном [%-*@3 charset], возвращает 
[%-*@3 defchar]. [%-*@3 charset ]не может быть  CHARSET`_UTF8.&]
[s3;%RU-RU &]
[s4; &]
[s5;:ToUnicode`(wchar`*`,const char`*`,int`,byte`): [@(0.0.255) void]_[* ToUnicode]([_^wchar^ w
char]_`*[*@3 ws], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s], [@(0.0.255) int]_[*@3 n], 
[_^byte^ byte]_[*@3 charset])&]
[s2;%RU-RU Преобразует массив из 8`-битных 
символов в UNICODE. Оба массива, [%-*@3 ws] 
и [%-*@3 s], должны иметь (как минимум) [%-*@3 n] 
элементов. [%-*@3 charset ]не может быть CHARSET`_UTF8.&]
[s3;%RU-RU &]
[s4; &]
[s5;:FromUnicode`(char`*`,const wchar`*`,int`,byte`,int`): [@(0.0.255) void]_[* FromUnico
de]([@(0.0.255) char]_`*[*@3 s], [@(0.0.255) const]_[_^wchar^ wchar]_`*[*@3 ws], 
[@(0.0.255) int]_[*@3 n], [_^byte^ byte]_[*@3 charset], [@(0.0.255) int]_[*@3 defchar]_`=_DEF
AULTCHAR)&]
[s2;%RU-RU Преобразует массив из 8`-битных 
символов в UNICODE. Оба массива, [%-*@3 ws] 
и [%-*@3 s], должны иметь (как минимум) [%-*@3 n] 
элементов. [%-*@3 charset ]не может быть CHARSET`_UTF8.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:ConvertCharset`(char`*`,byte`,const char`*`,byte`,int`): [@(0.0.255) void]_[* Conver
tCharset]([@(0.0.255) char]_`*[*@3 t], [_^byte^ byte]_[*@3 tcharset], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s], [_^byte^ byte]_[*@3 scharset], 
[@(0.0.255) int]_[*@3 n])&]
[s2;%RU-RU Преобразует массив из 8`-битных 
символов [%-*@3 s] с кодировкой [%-*@3 scharset] 
в другой 8`-битный массив с кодировкой 
[%-*@3 tcharset]. Оба массива должны иметь 
(как минимум) [%-*@3 n] элементов. Ни [%-*@3 tcharset], 
ни[%-*@3  scharset ]не должны быть в кодровке 
CHARSET`_UTF8.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:ToUnicode`(const String`&`,byte`): [_^WString^ WString]_[* ToUnicode]([@(0.0.255) cons
t]_[_^String^ String][@(0.0.255) `&]_[*@3 src], [_^byte^ byte]_[*@3 charset])&]
[s2;%RU-RU Преобразует [%-*@3 src], кодированный 
в [%-*@3 charset], в UNICODE. [%-*@3 charset] [*/ может ]быть 
в CHARSET`_UTF8. Неполноценные байты `"error`-escaped`" 
с помощью приватного диапазона 0xEExx.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:ToUnicode`(const char`*`,int`,byte`): [_^WString^ WString]_[* ToUnicode]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 src], [@(0.0.255) int]_[*@3 n], [_^byte^ byte]_[*@3 charset])&]
[s2;%RU-RU Преобразует [%-*@3 n] символов, начиная 
с [%-*@3 src], кодированных в [%-*@3 charset], в UNICODE. 
[%-*@3 charset] [*/ может] быть CHARSET`_UTF8. Неполноценные 
байты `"error`-escaped`" с помощью приватного 
диапазона 0xEExx.&]
[s3;%RU-RU &]
[s4; &]
[s5;:FromUnicodeBuffer`(const wchar`*`,int`,byte`,int`): [_^String^ String]_[* FromUnicod
eBuffer]([@(0.0.255) const]_[_^wchar^ wchar]_`*[*@3 src], [@(0.0.255) int]_[*@3 len], 
[_^byte^ byte]_[*@3 charset]_`=_CHARSET`_DEFAULT, [@(0.0.255) int]_[*@3 defchar]_`=_DEFAU
LTCHAR)&]
[s2;%RU-RU Преобразует [%-*@3 len] символов UNICODE 
из [%-*@3 src] в 8`-битную кодировку [%-*@3 charset]. 
[%-*@3 charset] [*/ может ]быть CHARSET`_UTF8. `"Error`-escape`" 
символы 0xEExx преобразуются в xx байтов. 
Если code`-point не существует в целевой 
кодировке, [%-*@3 defchar ][%- используется 
как результат преобразования].&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:FromUnicodeBuffer`(const Upp`:`:wchar`*`): [_^Upp`:`:String^ String]_[* FromU
nicodeBuffer]([@(0.0.255) const]_[_^Upp`:`:wchar^ wchar]_`*[*@3 src])&]
[s2;%RU-RU То же, что и FromUnicodeBuffer([%-*@3 src] , wstrlen([%-*@3 src])).&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:FromUnicode`(const WString`&`,byte`,int`): [_^String^ String]_[* FromUnicode]([@(0.0.255) c
onst]_[_^WString^ WString][@(0.0.255) `&]_[*@3 src], [_^byte^ byte]_[*@3 charset]_`=_CHARSE
T`_DEFAULT, [@(0.0.255) int]_[*@3 defchar]_`=_DEFAULTCHAR)&]
[s2;%RU-RU Преобразует UNICODE [%-*@3 src] в 8`-битную 
кодировку [%-*@3 charset]. [%-*@3 charset] [*/ может 
]быть CHARSET`_UTF8. `"Error`-escape`" символы 0xEExx 
преобразуются в xx байтов.  Если code`-point 
не существует в целевой кодировке, 
[%-*@3 defchar ][%- используется как результат 
преобразования].&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:ToCharset`(byte`,const String`&`,byte`,int`): [_^String^ String]_[* ToCharset]([_^byte^ b
yte]_[*@3 charset], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 s], 
[_^byte^ byte]_[*@3 scharset]_`=_CHARSET`_DEFAULT, [@(0.0.255) int]_[*@3 defchar]_`=_DEFA
ULTCHAR)&]
[s2;%RU-RU Преобразует [%-*@3 src], кодированный 
в [%-*@3 scharset], в [%-*@3 charset]. [%-*@3 charset] [*/ может 
]быть CHARSET`_UTF8. Могут использоваться 
`"Error`-escape`" символы, если один из набсимов 
в CHARSET`_UTF8. Если code`-point не существует 
в целевой кодировке, [%-*@3 defchar ][%- используется 
как результат преобразования].&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:IsLetter`(int`): [@(0.0.255) bool]_[* IsLetter]([@(0.0.255) int]_[*@3 c])&]
[s2;%RU-RU Возвращает true, если [%-*@3 c] < 2048 и 
он представляет собой букву.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:IsUpper`(int`): [@(0.0.255) bool]_[* IsUpper]([@(0.0.255) int]_[*@3 c])&]
[s2;%RU-RU Возвращает true, если [%-*@3 c] < 2048 и 
это `"код`-пойнт`" UNICODE в верхнем регистре.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:IsLower`(int`): [@(0.0.255) bool]_[* IsLower]([@(0.0.255) int]_[*@3 c])&]
[s2;%RU-RU Возвращает true, если [%-*@3 c] < 2048  и 
это `"код`-пойнт`" UNICODE в нижнем регистре.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:ToUpper`(int`): [@(0.0.255) int]_[* ToUpper]([@(0.0.255) int]_[*@3 c])&]
[s2;%RU-RU Если [%-*@3 c] < 2048 и нижнего регистра, 
возвращается соответствующий символ 
UNICODE верхнего регистра, иначе возвращается 
[%-*@3 c].&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:ToLower`(int`): [@(0.0.255) int]_[* ToLower]([@(0.0.255) int]_[*@3 c])&]
[s2;%RU-RU Если [%-*@3 c] < 2048 и в верхнем регистре, 
то возвращается соответствующий 
символ UNICODE нижнего регистра, иначе 
возвращается [%-*@3 c].&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:ToAscii`(int`): [@(0.0.255) int]_[* ToAscii]([@(0.0.255) int]_[*@3 c])&]
[s2;%RU-RU Возвращает UNICODE [%-*@3 c] `'преобразованным`' 
в базовый ASCII. Преобразование происходит 
с удалением всех диакритических знаков 
(меток). Если такое преобразование 
невозможно, возвращается 32 (пробел).&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:ToUpperAscii`(int`): [@(0.0.255) char]_[* ToUpperAscii]([@(0.0.255) int]_[*@3 c])
&]
[s2;%RU-RU То же, что и ToUpper(ToAscii([%-*@3 c]) (но быстрее).&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:ToLowerAscii`(int`): [@(0.0.255) char]_[* ToLowerAscii]([@(0.0.255) int]_[*@3 c])
&]
[s2;%RU-RU То же, что и ToLower(ToAscii([%-*@3 c]) (но быстрее).&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:IsLetter`(char`): [@(0.0.255) bool]_[* IsLetter]([@(0.0.255) char]_[*@3 c])&]
[s5;:IsLetter`(signed char`): [@(0.0.255) bool]_[* IsLetter]([@(0.0.255) signed]_[@(0.0.255) c
har]_[*@3 c])&]
[s2;%RU-RU Возвращает [^topic`:`/`/Core`/src`/CharSet`$en`-us`#IsLetter`(int`)^ I
sLetter]((byte)[%-*@3 c]).&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:IsUpper`(char`): [@(0.0.255) bool]_[* IsUpper]([@(0.0.255) char]_[*@3 c])&]
[s5;:IsUpper`(signed char`): [@(0.0.255) bool]_[* IsUpper]([@(0.0.255) signed]_[@(0.0.255) ch
ar]_[*@3 c])&]
[s2;%RU-RU Возвращает [^topic`:`/`/Core`/src`/CharSet`$en`-us`#IsUpper`(int`)^ I
sUpper]((byte)[%-*@3 c]).&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:IsLower`(char`): [@(0.0.255) bool]_[* IsLower]([@(0.0.255) char]_[*@3 c])&]
[s5;:IsLower`(signed char`): [@(0.0.255) bool]_[* IsLower]([@(0.0.255) signed]_[@(0.0.255) ch
ar]_[*@3 c])&]
[s2;%RU-RU Возвращает [^topic`:`/`/Core`/src`/CharSet`$en`-us`#IsLower`(int`)^ I
sLower]((byte)[%-*@3 c]).&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:ToUpper`(char`): [@(0.0.255) int]_[* ToUpper]([@(0.0.255) char]_[*@3 c])&]
[s5;:ToUpper`(signed char`): [@(0.0.255) int]_[* ToUpper]([@(0.0.255) signed]_[@(0.0.255) cha
r]_[*@3 c])&]
[s2;%RU-RU Возвращает [^topic`:`/`/Core`/src`/CharSet`$en`-us`#ToUpper`(int`)^ T
oUpper]((byte)[%-*@3 c]).&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:ToLower`(char`): [@(0.0.255) int]_[* ToLower]([@(0.0.255) char]_[*@3 c])&]
[s5;:ToLower`(signed char`): [@(0.0.255) int]_[* ToLower]([@(0.0.255) signed]_[@(0.0.255) cha
r]_[*@3 c])&]
[s2;%RU-RU Возвращает [^topic`:`/`/Core`/src`/CharSet`$en`-us`#ToLower`(int`)^ T
oLower]((byte)[%-*@3 c]).&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:ToAscii`(char`): [@(0.0.255) int]_[* ToAscii]([@(0.0.255) char]_[*@3 c])&]
[s5;:ToAscii`(signed char`): [@(0.0.255) int]_[* ToAscii]([@(0.0.255) signed]_[@(0.0.255) cha
r]_[*@3 c])&]
[s2;%RU-RU Возвращает [^topic`:`/`/Core`/src`/CharSet`$en`-us`#ToAscii`(int`)^ T
oAscii]((byte)[%-*@3 c]).&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:ToUpperAscii`(signed char`): [@(0.0.255) char]_[* ToUpperAscii]([@(0.0.255) sig
ned]_[@(0.0.255) char]_[*@3 c])&]
[s2;%RU-RU То же, что и ToUpper(ToAscii([%-*@3 c])).&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:ToLowerAscii`(signed char`): [@(0.0.255) char]_[* ToLowerAscii]([@(0.0.255) sig
ned]_[@(0.0.255) char]_[*@3 c])&]
[s2;%RU-RU То же, что и ToLower(ToAscii([%-*@3 c])).&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:ToUpperAscii`(char`): [@(0.0.255) char]_[* ToUpperAscii]([@(0.0.255) char]_[*@3 c
])&]
[s2;%RU-RU То же, что и ToUpper(ToAscii([%-*@3 c])).&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:ToLowerAscii`(char`): [@(0.0.255) char]_[* ToLowerAscii]([@(0.0.255) char]_[*@3 c
])&]
[s2;%RU-RU То же, что и ToLower(ToAscii([%-*@3 c])).&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:IsLetter`(wchar`): [@(0.0.255) bool]_[* IsLetter]([_^wchar^ wchar]_[*@3 c])&]
[s2;%RU-RU Возвращает [^topic`:`/`/Core`/src`/CharSet`$en`-us`#IsLetter`(int`)^ I
sLetter]([%-*@3 c]).&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:IsUpper`(wchar`): [@(0.0.255) bool]_[* IsUpper]([_^wchar^ wchar]_[*@3 c])&]
[s2;%RU-RU Возвращает [^topic`:`/`/Core`/src`/CharSet`$en`-us`#IsUpper`(int`)^ I
sUpper]([%-*@3 c]).&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:IsLower`(wchar`): [@(0.0.255) bool]_[* IsLower]([_^wchar^ wchar]_[*@3 c])&]
[s2;%RU-RU Возвращает [^topic`:`/`/Core`/src`/CharSet`$en`-us`#IsLower`(int`)^ I
sLower]([%-*@3 c]).&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:ToUpper`(wchar`): [@(0.0.255) int]_[* ToUpper]([_^wchar^ wchar]_[*@3 c])&]
[s2;%RU-RU Возвращает [^topic`:`/`/Core`/src`/CharSet`$en`-us`#ToUpper`(int`)^ T
oUpper]([%-*@3 c]).&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:ToLower`(wchar`): [@(0.0.255) int]_[* ToLower]([_^wchar^ wchar]_[*@3 c])&]
[s2;%RU-RU Возвращает [^topic`:`/`/Core`/src`/CharSet`$en`-us`#ToLower`(int`)^ T
oLower]([%-*@3 c]).&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:ToAscii`(wchar`): [@(0.0.255) int]_[* ToAscii]([_^wchar^ wchar]_[*@3 c])&]
[s2;%RU-RU Возвращает [^topic`:`/`/Core`/src`/CharSet`$en`-us`#ToAscii`(int`)^ T
oAscii]([%-*@3 c]).&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:ToUpperAscii`(Upp`:`:wchar`): [@(0.0.255) char]_[* ToUpperAscii]([_^Upp`:`:wchar^ w
char]_[*@3 c])&]
[s2;%RU-RU То же, что и ToUpper(ToAscii([%-*@3 c])).&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:ToLowerAscii`(Upp`:`:wchar`): [@(0.0.255) char]_[* ToLowerAscii]([_^Upp`:`:wchar^ w
char]_[*@3 c])&]
[s2;%RU-RU То же, что и ToLower(ToAscii([%-*@3 c])).&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:IsDigit`(int`): [@(0.0.255) bool]_[* IsDigit]([@(0.0.255) int]_[*@3 c])&]
[s2;%RU-RU Возвращает true, если [%-*@3 c] цифра: 
[%-*@3 c] >`= `'0`' `&`& [%-*@3 c] <`= `'9`'.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:IsAlpha`(int`): [@(0.0.255) bool]_[* IsAlpha]([@(0.0.255) int]_[*@3 c])&]
[s2;%RU-RU Возвращает true, если [%-*@3 c].алфавитный 
символ [*/ ASCII]: [%-*@3 c] >`= `'A`' `&`& [%-*@3 c] <`= `'Z`' 
`|`| [%-*@3 c] >`= `'a`' `&`& [%-*@3 c] <`= `'z`'.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:IsAlNum`(int`): [@(0.0.255) bool]_[* IsAlNum]([@(0.0.255) int]_[*@3 c])&]
[s2;%RU-RU Возвращает true, если [%-*@3 c].либо цифра, 
либо алфавитный символ [*/ ASCII].&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:IsLeNum`(int`): [@(0.0.255) bool]_[* IsLeNum]([@(0.0.255) int]_[*@3 c])&]
[s2;%RU-RU Возвращает true, если [%-*@3 c].цифра 
или буква [*/ UNICODE] < 2048.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:IsPunct`(int`): [@(0.0.255) bool]_[* IsPunct]([@(0.0.255) int]_[*@3 c])&]
[s2;%RU-RU Возвращает true, если: [%-*@3 c] !`= `' `' 
`&`& !IsAlNum([%-*@3 c]).&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:IsSpace`(int`): [@(0.0.255) bool]_[* IsSpace]([@(0.0.255) int]_[*@3 c])&]
[s2;%RU-RU Возвращает true, если [%-*@3 c ][%- один 
из символов] `' `', `'`\f`', `'`\n`', `'`\r`', `'`\v`', 
`'`\t`'.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:IsXDigit`(int`): [@(0.0.255) bool]_[* IsXDigit]([@(0.0.255) int]_[*@3 c])&]
[s2;%RU-RU Возвращает true, если [%-*@3 c].16`-ричная 
цифра (0`-9, a`-f, A`-F).&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:IsDoubleWidth`(int`): [@(0.0.255) bool]_[* IsDoubleWidth]([@(0.0.255) int]_[*@3 c
])&]
[s2;%RU-RU Возвращает true, если [%-*@3 c] символ 
UNICODE двойной ширины (например, идеограф 
CJK).&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:Utf8ToAscii`(const Upp`:`:String`&`): [_^Upp`:`:String^ String]_[* Utf8ToAsci
i]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 src])&]
[s2;%RU-RU Возвращает UTF`-8 String, `'преобразованной`' 
в базовый ASCII. Преоьазование выполняется 
с удалением любых Преобразование 
выполняется с удалением любых диакритически
х меток. Если такое преобразование 
невозможно, возвращает 32 (пробел).&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:Utf8ToUpperAscii`(const Upp`:`:String`&`): [_^Upp`:`:String^ String]_[* Utf8T
oUpperAscii]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 src])&]
[s2;%RU-RU То же, что и ToUpper(ToAscii([%-*@3 src]), но 
быстрее.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:Utf8ToLowerAscii`(const Upp`:`:String`&`): [_^Upp`:`:String^ String]_[* Utf8T
oLowerAscii]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 src])&]
[s2;%RU-RU То же, что и ToLower(ToAscii([%-*@3 src]), но 
быстрее.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:ToUpper`(wchar`*`,const wchar`*`,int`): [@(0.0.255) void]_[* ToUpper]([_^wchar^ wchar]_
`*[*@3 t], [@(0.0.255) const]_[_^wchar^ wchar]_`*[*@3 s], [@(0.0.255) int]_[*@3 len])&]
[s2;%RU-RU Преобразует массив UNICODE в верхний 
регистр.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:ToLower`(wchar`*`,const wchar`*`,int`): [@(0.0.255) void]_[* ToLower]([_^wchar^ wchar]_
`*[*@3 t], [@(0.0.255) const]_[_^wchar^ wchar]_`*[*@3 s], [@(0.0.255) int]_[*@3 len])&]
[s2;%RU-RU Преобразует массив UNICODE в нижний 
регистр.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:ToAscii`(wchar`*`,const wchar`*`,int`): [@(0.0.255) void]_[* ToAscii]([_^wchar^ wchar]_
`*[*@3 t], [@(0.0.255) const]_[_^wchar^ wchar]_`*[*@3 s], [@(0.0.255) int]_[*@3 len])&]
[s2;%RU-RU Массив UNICODE в базовый ASCII (смотрите 
[^topic`:`/`/Core`/src`/CharSet`_ru`-ru`#ToAscii`(int`)^ ToAscii]).&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:ToUpper`(wchar`*`,int`): [@(0.0.255) void]_[* ToUpper]([_^wchar^ wchar]_`*[*@3 s], 
[@(0.0.255) int]_[*@3 len])&]
[s2;%RU-RU Преобразует массив UNICODE в верхний 
регистр.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:ToLower`(wchar`*`,int`): [@(0.0.255) void]_[* ToLower]([_^wchar^ wchar]_`*[*@3 s], 
[@(0.0.255) int]_[*@3 len])&]
[s2;%RU-RU Преобразует массив UNICODE в нижний 
регистр.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:ToAscii`(wchar`*`,int`): [@(0.0.255) void]_[* ToAscii]([_^wchar^ wchar]_`*[*@3 s], 
[@(0.0.255) int]_[*@3 len])&]
[s2;%RU-RU Массив UNICODE в базовый ASCII (смотрите 
[^topic`:`/`/Core`/src`/CharSet`_ru`-ru`#ToAscii`(int`)^ ToAscii]).&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:IsLetter`(int`,byte`): [@(0.0.255) bool]_[* IsLetter]([@(0.0.255) int]_[*@3 c], 
[_^byte^ byte]_[*@3 charset])&]
[s2;%RU-RU Возвращает true, если символ [%-*@3 c], 
кодированный с использованием 8`-битного 
набсима [%-*@3 charset], явяется буквой.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:IsUpper`(int`,byte`): [@(0.0.255) bool]_[* IsUpper]([@(0.0.255) int]_[*@3 c], 
[_^byte^ byte]_[*@3 charset])&]
[s2;%RU-RU Возвращает true, если символ [%-*@3 c], 
кодированный с использованием 8`-битного 
набсима [%-*@3 charset], явяется буквой верхнего 
регистра (т.е. прописной).&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:IsLower`(int`,byte`): [@(0.0.255) bool]_[* IsLower]([@(0.0.255) int]_[*@3 c], 
[_^byte^ byte]_[*@3 charset])&]
[s2; Возвращает  true, если символ [*@3 c], кодированный
 в 8`-битном [*@3 charset], является буквой 
нижнего регистра.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:ToUpper`(int`,byte`): [@(0.0.255) int]_[* ToUpper]([@(0.0.255) int]_[*@3 c], 
[_^byte^ byte]_[*@3 charset])&]
[s2;%RU-RU Преобразует символ [%-*@3 c], кодированный 
с использованием 8`-битного набсима, 
в верхний регистр, если это буква, 
иначе возвращает его неизменённым.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:ToLower`(int`,byte`): [@(0.0.255) int]_[* ToLower]([@(0.0.255) int]_[*@3 c], 
[_^byte^ byte]_[*@3 charset])&]
[s2;%RU-RU Преобразует символ [%-*@3 c], кодированный 
с использованием 8`-битного набсима, 
в нижний регистр, если это буква, иначе 
возвращает его неизменённым.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:ToAscii`(int`,byte`): [@(0.0.255) int]_[* ToAscii]([@(0.0.255) int]_[*@3 c], 
[_^byte^ byte]_[*@3 charset])&]
[s2; Преобразует символ [*@3 c] , кодированный 
с использованием 8`-битного набсима 
в символ базовой ASCI, удаляя диакритические 
метки. Если [*@3 c] не буква, возвращает 
его неизменённым.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:ToUpper`(char`*`,const char`*`,int`,byte`): [@(0.0.255) void]_[* ToUpper]([@(0.0.255) c
har]_`*[*@3 t], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s], [@(0.0.255) int]_[*@3 len], 
[_^byte^ byte]_[*@3 charset]_`=_CHARSET`_DEFAULT)&]
[s2; [%RU-RU Преобразует массив ][*@3 s ][%RU-RU длиной 
][*@3 len][%RU-RU  символов с кодировкой ][*@3 charset][%RU-RU  
в верхний регистр (используя ][%RU-RU^topic`:`/`/Core`/src`/CharSet`_ru`-ru`#IsUpper`(int`,byte`)^ T
oUpper][%RU-RU ). Результат сохраняется в 
][*@3 t].&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:ToLower`(char`*`,const char`*`,int`,byte`): [@(0.0.255) void]_[* ToLower]([@(0.0.255) c
har]_`*[*@3 t], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s], [@(0.0.255) int]_[*@3 len], 
[_^byte^ byte]_[*@3 charset]_`=_CHARSET`_DEFAULT)&]
[s2; Преобразует массив [*@3 s ]длиной [*@3 len] 
символов с кодировкой [*@3 charset] в нижний 
регистр (используя [^topic`:`/`/Core`/src`/CharSet`_ru`-ru`#IsLower`(int`,byte`)^ T
oLower]). [%RU-RU  Результат сохраняется в 
][*@3 t].&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:ToAscii`(char`*`,const char`*`,int`,byte`): [@(0.0.255) void]_[* ToAscii]([@(0.0.255) c
har]_`*[*@3 t], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s], [@(0.0.255) int]_[*@3 len], 
[_^byte^ byte]_[*@3 charset]_`=_CHARSET`_DEFAULT)&]
[s2; Преобразует массив [*@3 s ]длиной [*@3 len] 
символов с кодировкой [*@3 charset] в базовую 
ASCII (используя [^topic`:`/`/Core`/src`/CharSet`_ru`-ru`#ToAscii`(int`,byte`)^ T
oAscii]). [%RU-RU Результат сохраняется в 
][*@3 t].&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:ToUpper`(char`*`,int`,byte`): [@(0.0.255) void]_[* ToUpper]([@(0.0.255) char]_`*[*@3 s],
 [@(0.0.255) int]_[*@3 len], [_^byte^ byte]_[*@3 charset]_`=_CHARSET`_DEFAULT)&]
[s2; Преобразует массив [*@3 s ]длиной [*@3 len] 
символов с кодировкой [*@3 charset] в верхний 
регистр (используя ToUpper). [%RU-RU Результат 
сохраняется в ][*@3 t].&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:ToLower`(char`*`,int`,byte`): [@(0.0.255) void]_[* ToLower]([@(0.0.255) char]_`*[*@3 s],
 [@(0.0.255) int]_[*@3 len], [_^byte^ byte]_[*@3 charset]_`=_CHARSET`_DEFAULT)&]
[s2; Преобразует массив [*@3 s ]длиной [*@3 len] 
символов с кодировкой [*@3 charset], в нижний 
регистр (используя ToLower). [%RU-RU Результат 
сохраняется обратно в ][*@3 s].&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:ToAscii`(char`*`,int`,byte`): [@(0.0.255) void]_[* ToAscii]([@(0.0.255) char]_`*[*@3 s],
 [@(0.0.255) int]_[*@3 len], [_^byte^ byte]_[*@3 charset]_`=_CHARSET`_DEFAULT)&]
[s2; Преобразует массив [*@3 s ]длиной [*@3 len] 
символов с кодировкой [*@3 charset] в базовую 
ASCIII (используя ToAscii).[%RU-RU Результат 
сохраняется обратно в ][*@3 s].&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:InitCaps`(const wchar`*`): [_^WString^ WString]_[* InitCaps]([@(0.0.255) const]_[_^wchar^ w
char]_`*[*@3 s])&]
[s2;%RU-RU Преобразует вводную строку с 
нулевым окончанием в UNICODE так, что 
первые буквы в каждом слове (которые 
рядом с пробелом) устанавливаются 
в верхних регистр (становятся заглавными), 
а остальные `- в нижний (прописные).&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:InitCaps`(const WString`&`): [_^WString^ WString]_[* InitCaps]([@(0.0.255) const]_[_^WString^ W
String][@(0.0.255) `&]_[*@3 s])&]
[s2;%RU-RU [%- Преобразует строку UNICODE так, 
что первые буквы в каждом слове (которые 
рядом с пробелом) ]устанавливаются 
в верхних регистр (становятся заглавными), 
а остальные `- в нижний (прописные).&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:ToUpper`(const WString`&`): [_^WString^ WString]_[* ToUpper]([@(0.0.255) const]_[_^WString^ W
String][@(0.0.255) `&]_[*@3 w])&]
[s2;%RU-RU Преобразует строку UNICODE в заглавные.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:ToLower`(const WString`&`): [_^WString^ WString]_[* ToLower]([@(0.0.255) const]_[_^WString^ W
String][@(0.0.255) `&]_[*@3 w])&]
[s2; Преобразует строку UNICODE в прописные.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:ToAscii`(const WString`&`): [_^WString^ WString]_[* ToAscii]([@(0.0.255) const]_[_^WString^ W
String][@(0.0.255) `&]_[*@3 w])&]
[s2; Преобразует строку UNICODE в базовую 
ASCII, удаляя диакритичекие метки.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:InitCaps`(const char`*`,byte`): [_^String^ String]_[* InitCaps]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 s], [_^byte^ byte]_[*@3 charset]_`=_CHARSET`_DEFAULT)&]
[s2; Преобразует вводную строку нулевого 
окончания, 8`-битную, кодированную 
в [*@3 charset] так, что первые буквы в каждом 
слове (которые рядом с пробелом) [%RU-RU устанавл
иваются в верхних регистр (становятся 
заглавными), а остальные `- в нижний 
(прописные).] [*@3 charset] может быть CHARSET`_UTF8.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:ToUpper`(const String`&`,byte`): [_^String^ String]_[* ToUpper]([@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 s], [_^byte^ byte]_[*@3 charset]_`=_CHARSET`_DEFAULT)&]
[s2; Преобразует вводную строку, 8`-битную, 
кодированную в [*@3 charset] в верхний регистр. 
[*@3 charset] может быть CHARSET`_UTF8.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:ToLower`(const String`&`,byte`): [_^String^ String]_[* ToLower]([@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 s], [_^byte^ byte]_[*@3 charset]_`=_CHARSET`_DEFAULT)&]
[s2; Преобразует вводную строку, 8`-битную, 
кодированную в [*@3 charset] в нижний регистр. 
[*@3 charset] может быть CHARSET`_UTF8.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:ToAscii`(const String`&`,byte`): [_^String^ String]_[* ToAscii]([@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 s], [_^byte^ byte]_[*@3 charset]_`=_CHARSET`_DEFAULT)&]
[s2; Преобразует вводную строку, 8`-битную, 
кодированную в [*@3 charset] в  базовую ASCII, 
удаляя диакритические метки. [*@3 charset] 
может быть CHARSET`_UTF8.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:ToUpperAscii`(const Upp`:`:String`&`,Upp`:`:byte`): [_^Upp`:`:String^ Strin
g]_[* ToUpperAscii]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 s], 
[_^Upp`:`:byte^ byte]_[*@3 charset])&]
[s2;%RU-RU То же, что и ToUpper(ToAscii([%-*@3 s]), [%-*@3 charset]), 
но быстрее.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:ToLowerAscii`(const Upp`:`:String`&`,Upp`:`:byte`): [_^Upp`:`:String^ Strin
g]_[* ToLowerAscii]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 s], 
[_^Upp`:`:byte^ byte]_[*@3 charset])&]
[s2;%RU-RU То же, что и ToLower(ToAscii([%-*@3 s]), [%-*@3 charset]), 
 но быстрее.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:ToUpper`(const char`*`,byte`): [_^String^ String]_[* ToUpper]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 s], [_^byte^ byte]_[*@3 charset]_`=_CHARSET`_DEFAULT)&]
[s2; Преобразует вводную строку с нулеввым 
окончанием, 8`-битную, в кодировке 
[*@3 charset], в верхний регистр. [*@3 charset] может 
быть CHARSET`_UTF8.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:ToLower`(const char`*`,byte`): [_^String^ String]_[* ToLower]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 s], [_^byte^ byte]_[*@3 charset]_`=_CHARSET`_DEFAULT)&]
[s2; Преобразует вводную строку с нулеввым 
окончанием, 8`-битную, в кодировке 
[*@3 charset], в нижний регистр. [*@3 charset] может 
быть CHARSET`_UTF8.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:ToAscii`(const char`*`,byte`): [_^String^ String]_[* ToAscii]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 s], [_^byte^ byte]_[*@3 charset]_`=_CHARSET`_DEFAULT)&]
[s2;%RU-RU [%- Преобразует вводную строку с 
нулеввым окончанием, 8`-битную, в кодировке 
][%-*@3 charset][%- , в базовый] ASCII, удаляя диакритическ
ие знаки. [%-*@3 charset ]может быть CHARSET`_UTF8.&]
[s3;%RU-RU &]
[s4; &]
[s5;:LoadStreamBOMW`(Stream`&`,byte`): [_^WString^ WString]_[* LoadStreamBOMW]([_^Stream^ S
tream][@(0.0.255) `&]_[*@3 in], [_^byte^ byte]_[*@3 def`_charset])&]
[s2;%RU-RU Считывает поток в строку UNICODE, 
допуская опциональную метку BOM UNICODE. 
Если она отсутствует, считается, что 
текст в [%-*@3 def`_charset].&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:LoadStreamBOMW`(Stream`&`): [_^WString^ WString]_[* LoadStreamBOMW]([_^Stream^ Stream][@(0.0.255) `&
]_[*@3 in])&]
[s2;%RU-RU Считывает поток в строку UNICODE, 
допуская опциональную метку BOM UNICODE. 
Если она отсутствует, считается, что 
текст в кодировке, определённой хостом. 
(например, установленный linux locale).&]
[s3;%RU-RU &]
[s4; &]
[s5;:LoadStreamBOM`(Stream`&`,byte`): [_^String^ String]_[* LoadStreamBOM]([_^Stream^ Strea
m][@(0.0.255) `&]_[*@3 in], [_^byte^ byte]_[*@3 def`_charset])&]
[s2;%RU-RU [%- Считывает поток в 8`-битную строку 
с дефолтно кодировкой, допуская опциональну
ю метку BOM UNICODE . Если она отсутствует, 
считается, что текст в ][%-*@3 def`_charset] 
.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:LoadStreamBOM`(Stream`&`): [_^String^ String]_[* LoadStreamBOM]([_^Stream^ Stream][@(0.0.255) `&
]_[*@3 in])&]
[s2;%RU-RU [%- Считывает поток в 8`-битную строку 
с дефолтно кодировкой, допуская опциональну
ю метку BOM UNICODE . Если она отсутствует, 
считается, что текст в ]кодировке, 
определённой хостом. (например, установленны
й linux locale).&]
[s3;%RU-RU &]
[s4; &]
[s5;:LoadFileBOMW`(const char`*`,byte`): [_^WString^ WString]_[* LoadFileBOMW]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 path], [_^byte^ byte]_[*@3 def`_charset])&]
[s2; Считывает файл в строку UNICODE (string), 
допуская опциональную метку BOM UNICODE. 
Если она отсутствует, считается, что 
текст в [*@3 def`_charset][%RU-RU . Если файл не 
читается], возвращается WString`::GetVoid().&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:LoadFileBOMW`(const char`*`): [_^WString^ WString]_[* LoadFileBOMW]([@(0.0.255) const]_
[@(0.0.255) char]_`*[*@3 path])&]
[s2; Считывает файл в строку UNICODE, допуская 
опциональную метку BOM UNICODE.Если она 
отсутствует, считается, что текст 
в [%RU-RU кодировке, определённой хостом 
(например, установленный linux locale).] 
Если файл не читается, возвращает 
WString`::GetVoid().&]
[s3;%RU-RU &]
[s4; &]
[s5;:LoadFileBOM`(const char`*`,byte`): [_^String^ String]_[* LoadFileBOM]([@(0.0.255) cons
t]_[@(0.0.255) char]_`*[*@3 path], [_^byte^ byte]_[*@3 def`_charset])&]
[s2; [%RU-RU Считывает файл в 8`-битную строку 
с дефолтной кодировкой, ]допуская 
опциональную метку BOM UNICODE. Если она 
отсутствует, считается, что текст 
в[%RU-RU  ][*@3 def`_charset][%RU-RU  . Если файл не читается, 
возвращает] String`::GetVoid().&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:LoadFileBOM`(const char`*`): [_^String^ String]_[* LoadFileBOM]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 path])&]
[s2; [%RU-RU Считывает файл в 8`-битную строку 
с дефолтной кодировкой, ]допуская 
опциональную метку BOM UNICODE.[%RU-RU  ]Если 
она отсутствует, считается, что текст 
в [%RU-RU кодировке, определённой хостом. 
(например, установленный linux locale). 
Если файл не читается, возвращает] 
String`::GetVoid().&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:SaveStreamBOM`(Stream`&`,const WString`&`): [@(0.0.255) bool]_[* SaveStreamBOM]([_^Stream^ S
tream][@(0.0.255) `&]_[*@3 out], [@(0.0.255) const]_[_^WString^ WString][@(0.0.255) `&]_[*@3 d
ata])&]
[s2;%RU-RU Сохраняет поток в 16`-битном формате 
UNICODE, с BOM header. При успехе возвращает 
true.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:SaveFileBOM`(const char`*`,const WString`&`): [@(0.0.255) bool]_[* SaveFileBOM]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 path], [@(0.0.255) const]_[_^WString^ WString][@(0.0.255) `&
]_[*@3 data])&]
[s2;%RU-RU Сохраняет файл в 16`-битном формате 
UNICODE, с BOM header. При успехе возвращает 
true.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:SaveStreamBOMUtf8`(Stream`&`,const String`&`): [@(0.0.255) bool]_[* SaveStreamBOMUtf
8]([_^Stream^ Stream][@(0.0.255) `&]_[*@3 out], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[*@3 data])&]
[s2;%RU-RU Сохраняет 8`-битную строку (string) 
в дефолтной кодировке в поток.[%-  ]При 
успехе возвращает true.&]
[s3; &]
[s4;%RU-RU &]
[s5;:SaveFileBOMUtf8`(const char`*`,const String`&`): [@(0.0.255) bool]_[* SaveFileBOMUtf
8]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 path], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[*@3 data])&]
[s2;%RU-RU Сохраняет 8`-битную строку в дефолтной 
кодировке в файл.[%-  ]При успехе возвращает 
true.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Utf8BOM`(Upp`:`:Stream`&`): [@(0.0.255) bool]_[* Utf8BOM]([_^Upp`:`:Stream^ Str
eam][@(0.0.255) `&]_[*@3 in])&]
[s2;%RU-RU Проверяет наличие и пропускает 
метку UTF`-8 BOM (МПБ УТФ`-8) в seekable Stream [%-*@3 in].&]
[s3;%RU-RU &]
[s4; &]
[s5;:FromUtf8`(const char`*`,int`): [_^WString^ WString]_[* FromUtf8]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 `_s], [@(0.0.255) int]_[*@3 len])&]
[s2;%RU-RU Преобразует UTF`-8 в строку UNICODE. 
Любые неправильные байты и цепочки 
преобразуются в приватный диапазон 
0xEExx. Депрекирован, используйте ToUtf16.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:FromUtf8`(const char`*`): [_^WString^ WString]_[* FromUtf8]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 `_s])&]
[s2;%RU-RU Преобразует строку UTF`-8 с нулевым 
окончанием в UNICODE. Любые неправильные 
байты и цепочки преобразуются в приватный 
диапазон 0xEExx. Депрекирован, используйте 
ToUtf16.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:FromUtf8`(const String`&`): [_^WString^ WString]_[* FromUtf8]([@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 s])&]
[s2;%RU-RU Преобразует строку UTF`-8 в UNICODE. 
Любые неправильные байты и цепочки 
преобразуются в приватный диапазон 
0xEExx. Депрекирован, используйте ToUtf16.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:utf8check`(const char`*`,int`): [@(0.0.255) bool]_[* utf8check]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 `_s], [@(0.0.255) int]_[*@3 len])&]
[s2;%RU-RU Проверяет, содержит ли массив 
валидную последовательность UTF`-8. 
Депрекирован, используйте CheckUtf8.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:utf8len`(const char`*`,int`): [@(0.0.255) int]_[* utf8len]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 s], [@(0.0.255) int]_[*@3 len])&]
[s2;%RU-RU Возвращает число символов UNICODE 
в тексте UTF`-8. Error`-escaped 0xEExx characters for 
ill`-formed parts of UTF`-8 are correctly accounted for. Депрекирован
, используйте Utf16Len.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:utf8len`(const char`*`): [@(0.0.255) int]_[* utf8len]([@(0.0.255) const]_[@(0.0.255) cha
r]_`*[*@3 s])&]
[s2;%RU-RU Возвращает число символов UNICODE 
в тексте UTF`-8 с нулевым окончанием. 
Error`-escaped 0xEExx characters for ill`-formed parts of UTF`-8 
are correctly accounted for. Депрекирован, используйте 
Utf16Len.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:lenAsUtf8`(const wchar`*`,int`): [@(0.0.255) int]_[* lenAsUtf8]([@(0.0.255) const]_[_^wchar^ w
char]_`*[*@3 s], [@(0.0.255) int]_[*@3 len])&]
[s2;%RU-RU Возвращает число байтов текста 
UNICODE, кодированного как UTF`-8. Депрекирован, 
используйте Utf8Len.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:lenAsUtf8`(const wchar`*`): [@(0.0.255) int]_[* lenAsUtf8]([@(0.0.255) const]_[_^wchar^ w
char]_`*[*@3 s])&]
[s2;%RU-RU Возвращает число байтов текста 
UNICODE с нулевым окончанием, кодированного 
как UTF`-8. Депрекирован, используйте 
Utf8Len.&]
[s0; ]]