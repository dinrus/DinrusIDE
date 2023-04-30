topic "Класс CParser";
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
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Класс CParser]]}}&]
[s3; &]
[s1;:CParser`:`:class: [@(0.0.255) class]_[* CParser]&]
[s9;%RU-RU CParser `-простой, но очень полезный 
лексический анализатор, подходящий 
для построения производных парсеров 
языков, с Си`-подобным синтаксисом.&]
[s9;%RU-RU CParser работает над текстом, расположенным
 в памяти, с окончанием `'`\0`', (не Stream). 
Поэтому, чтобы разобрать (парсировать) 
файл, вначале нужно загрузить кго 
в память. Текст должен существовать 
всё время его обработки CParser`'ом (никакой 
копии не делается).&]
[s9;%RU-RU Многие методы CParser могут выводить 
(`"выбрасывать`") исключение CParser`::Error, 
чтобы указать на ошибку при разборе 
заданного символа. При использовании 
CParser для построения конкретного парсера, 
также желательно использовать это 
исключение (предпочтительно через 
метод ThrowError), чтобы указывать на возникающие
 ошибки.&]
[s9;%RU-RU Процедуры обработки идентификаторов 
допускают ьуквы в кодировке ascii, цифры 
и символы `'`_`' и `'`$`'. `'`$`' `- нестандартен 
для Си, но допускается языками JavaSript, 
JSON, Java и большинством компиляторовt 
C/C`+`+  как расширение.&]
[s9;%RU-RU Объекты CParser`'а нетранферабильны 
(некопируемы).&]
[s9;%RU-RU &]
[s3;%RU-RU &]
[s0;%RU-RU &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Детали Конструктора]]}}&]
[s3; &]
[s5;:CParser`:`:CParser`(const char`*`): [* CParser]([@(0.0.255) const]_[@(0.0.255) char]_`*
[*@3 ptr])&]
[s2; [%RU-RU Конструирует ][%RU-RU* CParser, ][%RU-RU который 
может парсировать вводный буфер ][*@3 ptr].&]
[s3; &]
[s4; &]
[s5;:CParser`:`:CParser`(const char`*`,const char`*`,int`): [* CParser]([@(0.0.255) const
]_[@(0.0.255) char]_`*[*@3 ptr], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 fn], 
[@(0.0.255) int]_[*@3 line]_`=_[@3 1])&]
[s2;%RU-RU Конструирует [* CParser], с дополнительной 
информацией для текста. Дополнительную 
инфу можно использовать при отчёте 
об ошибках:&]
[s7;%RU-RU [%-*C@3 ptr]-|Указатель на вводный текст.&]
[s7;%RU-RU [%-*C@3 fn]-|Имя файла (фапктически может 
быть чем угодно, сохраняется только 
значение).&]
[s7;%RU-RU [%-*C@3 line]-|Номер первой строки.&]
[s3; &]
[s4; &]
[s5;:CParser`:`:CParser`(`): [* CParser]()&]
[s0;%RU-RU Конструирует [* CParser]. Вводный текст 
нужно присвоить с помощью метода 
[* SetPos].&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Публичных Членов]]}}&]
[s3; &]
[s5;:CParser`:`:ThrowError`(const char`*`): [@(0.0.255) void]_[* ThrowError]([@(0.0.255) co
nst]_[@(0.0.255) char]_`*[*@3 s])&]
[s2;%RU-RU Выбрасывает [^topic`:`/`/Core`/src`/CParser`_ru`-ru`#CParser`:`:Error`:`:struct^ C
Parser`::Error] с сообщением об ошибке [%-*@3 s].&]
[s3; &]
[s3;%RU-RU &]
[s4; &]
[s5;:CParser`:`:SkipSpaces`(bool`): [_^CParser^ CParser][@(0.0.255) `&]_[* SkipSpaces]([@(0.0.255) b
ool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%RU-RU Устанавливает режим пропуска 
пробелов. Если [%-*@3 b] равно true, устанавливает 
[* CParser ]в режим, при котором пробелы 
автоматически пропускаются. Первый 
пропуск выполняется, когда позиция 
в вводном (`"входящем`") тексте задаётся 
конструктором или через [^topic`:`/`/Core`/src`/CParser`_ru`-ru`#CParser`:`:SetPos`(const CParser`:`:Pos`&`)^ S
etPos], затем пропуск происходит после 
любого символа. Если [%-*@3 b] равно false, 
 [* CParser ]устанавливается в режим, когда 
пробелы не пропускаются автоматически, 
а требуется, чтобы пропуск выполнял 
метод [^topic`:`/`/Core`/src`/CParser`_ru`-ru`#CParser`:`:Spaces`(`)^ Spaces].
 Дефолт (т.е `"настройка по умолчанию`") 
равен true.&]
[s3;%RU-RU &]
[s4; &]
[s5;:CParser`:`:NoSkipSpaces`(`): [_^CParser^ CParser][@(0.0.255) `&]_[* NoSkipSpaces]()&]
[s2;%RU-RU То же, что и SkipSpaces(false).&]
[s3; &]
[s4; &]
[s5;:CParser`:`:UnicodeEscape`(bool`): [_^CParser^ CParser][@(0.0.255) `&]_[* UnicodeEscape
]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%RU-RU Активирует/дезактивирует, должен 
ли CParser распознавать `"искейп`"`-последователь
ности Юникода `\u и `\U в String. Эта опция 
по дефолту активна.&]
[s3; &]
[s4; &]
[s5;:CParser`:`:SkipComments`(bool`): [_^CParser^ CParser][@(0.0.255) `&]_[* SkipComments](
[@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%RU-RU Устанавливает поведение комментариев. 
Если активно, комментарии трактуются 
как пробелы (в общем, игнорируются).Это 
[*/ default]. Заметьте, что SkipComments должен 
быть вызван до всякого разбора`-парсинга.&]
[s3;%RU-RU &]
[s4; &]
[s5;:CParser`:`:NoSkipComments`(`): [_^CParser^ CParser][@(0.0.255) `&]_[* NoSkipComments](
)&]
[s2;%RU-RU То же, что и SkipComments(false).&]
[s3; &]
[s4; &]
[s5;:Upp`:`:CParser`:`:NestComments`(bool`): [_^Upp`:`:CParser^ CParser][@(0.0.255) `&]_[* N
estComments]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%RU-RU Если активен, CParser распознаёт 
`"гнездовые`" комментарии (напр ,`"[C /`* 
уровень1 /`* уровень2 `*/ `*/]`").&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:CParser`:`:NoNestComments`(`): [_^Upp`:`:CParser^ CParser][@(0.0.255) `&]_[* No
NestComments]()&]
[s2;%RU-RU То же, что и NestComments(false).&]
[s3; &]
[s4; &]
[s5;:CParser`:`:Spaces`(`): [@(0.0.255) bool]_[* Spaces]()&]
[s2;%RU-RU Пропускает пробелы. Возвращает 
[* true], если был пропущен пробел, [* false] 
в противном случае. Сохраняет перед 
продвижением вперёд позицию как `"указатель 
на пробел`", которую `"извлекает`" метод 
GetSpacePtr.&]
[s3; &]
[s4; &]
[s5;:CParser`:`:PeekChar`(`)const: [@(0.0.255) char]_[* PeekChar]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает текущий единичный 
символ.&]
[s3; &]
[s4; &]
[s5;:CParser`:`:GetChar`(`): [@(0.0.255) char]_[* GetChar]()&]
[s2;%RU-RU Продвигает вперёд позицию в вводном 
тексте, на один символ, и перед движением 
далее возвращает символ в этой позиции.&]
[s3; &]
[s4; &]
[s5;:CParser`:`:IsChar`(char`)const: [@(0.0.255) bool]_[* IsChar]([@(0.0.255) char]_[*@3 c])_
[@(0.0.255) const]&]
[s2;%RU-RU Тестирует, есть ли специфичный 
символ [%-*@3 c ]в текущей позиции.&]
[s3;%RU-RU &]
[s4; &]
[s5;:CParser`:`:IsChar2`(char`,char`)const: [@(0.0.255) bool]_[* IsChar2]([@(0.0.255) char]_
[*@3 c1], [@(0.0.255) char]_[*@3 c2])_[@(0.0.255) const]&]
[s2;%RU-RU Тестирует, есть ли специфичная 
пара символов ([%-*@3 c1], [%-*@3 c2]) в текущей 
позиции.&]
[s3;%RU-RU &]
[s4; &]
[s5;:CParser`:`:IsChar3`(char`,char`,char`)const: [@(0.0.255) bool]_[* IsChar3]([@(0.0.255) c
har]_[*@3 c1], [@(0.0.255) char]_[*@3 c2], [@(0.0.255) char]_[*@3 c3])_[@(0.0.255) const]&]
[s2;%RU-RU Тестирует на наличие специфичного 
символьного триплета ([%-*@3 c1], [%-*@3 c2], 
[%-*@3 c3]) в текущей позиции.&]
[s3; &]
[s4; &]
[s5;:CParser`:`:Char`(char`): [@(0.0.255) bool]_[* Char]([@(0.0.255) char]_[*@3 c])&]
[s2;%RU-RU Тестирует на единичный символ 
[%-*@3 c] в текущей позиции. Если есть совпадение,
 позиция продвигается вперёд и возвращается
 [* true.] Если совпадения не найдено, позиция 
остаётся неизменной и возвращается 
[* false].&]
[s3; &]
[s4; &]
[s5;:CParser`:`:Char2`(char`,char`): [@(0.0.255) bool]_[* Char2]([@(0.0.255) char]_[*@3 c1], 
[@(0.0.255) char]_[*@3 c2])&]
[s2;%RU-RU Тестирует на символьную паруr 
([%-*@3 c1], [%-*@3 c2]) в текущей позиции. Если 
есть совпадение, позиция продвигается 
вперёд на  два символа и возвращается 
[* true]. Если совпадения не обнаружено, 
позиция остаётся неизменной и возвращается 
[* false].&]
[s3; &]
[s4; &]
[s5;:CParser`:`:Char3`(char`,char`,char`): [@(0.0.255) bool]_[* Char3]([@(0.0.255) char]_[*@3 c
1], [@(0.0.255) char]_[*@3 c2], [@(0.0.255) char]_[*@3 c3])&]
[s2;%RU-RU Тестирует на символьный триплет 
([%-*@3 c1], [%-*@3 c2], [%-*@3 c3]) в текущей позиции. 
Если есть совпадение, позиция продвигается 
вперёд на три символа и возвращается 
[* true]. Если совпадения нет, позиция 
остаётся неизменной, возвращается 
[* false].&]
[s3; &]
[s4; &]
[s5;:CParser`:`:PassChar`(char`): [@(0.0.255) void]_[* PassChar]([@(0.0.255) char]_[*@3 c])&]
[s2;%RU-RU Вызывает [^topic`:`/`/Core`/src`/CParser`_ru`-ru`#CParser`:`:Char`(char`)^ C
har](c).Если возвращается false, выбрасывается 
ошибка.&]
[s3; &]
[s4; &]
[s5;:CParser`:`:PassChar2`(char`,char`): [@(0.0.255) void]_[* PassChar2]([@(0.0.255) char]_
[*@3 c1], [@(0.0.255) char]_[*@3 c2])&]
[s2;%RU-RU Вызывает [^topic`:`/`/Core`/src`/CParser`_ru`-ru`#CParser`:`:IsChar2`(char`,char`)const^ C
har2](c1, c2). Если возвращается [* false], выбрасываетс
я [^topic`:`/`/Core`/src`/CParser`_ru`-ru`#CParser`:`:Error`:`:struct^ CParser`::Er
ror].&]
[s3; &]
[s4; &]
[s5;:CParser`:`:PassChar3`(char`,char`,char`): [@(0.0.255) void]_[* PassChar3]([@(0.0.255) c
har]_[*@3 c1], [@(0.0.255) char]_[*@3 c2], [@(0.0.255) char]_[*@3 c3])&]
[s2;%RU-RU Вызывает [^topic`:`/`/Core`/src`/CParser`_ru`-ru`#CParser`:`:Char3`(char`,char`,char`)^ C
har3](c1, c2, c3). Если возвращается [* false], 
выбрасывается [^topic`:`/`/Core`/src`/CParser`_ru`-ru`#CParser`:`:Error`:`:struct^ C
Parser`::Error].&]
[s3; &]
[s4; &]
[s5;:CParser`:`:Id`(const char`*`): [@(0.0.255) bool]_[* Id]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 s])&]
[s2;%RU-RU Тестирует на заданный `"Си`-образный`" 
идентификатор [%-*@3 s]. Если есть совпадение, 
продвигает вперёд позицию на [* strlen](s) 
символов. Возвращает [* true] при совпадении 
и [* false] в противном случае.&]
[s3; &]
[s4; &]
[s5;:CParser`:`:PassId`(const char`*`): [@(0.0.255) void]_[* PassId]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 s])_[@(0.0.255) throw](Error)&]
[s2;%RU-RU Вызывает метод [^topic`:`/`/Core`/src`/CParser`_ru`-ru`#CParser`:`:Id`(const char`*`)^ I
d] с [%-*@3 s] в качестве параметра. Если 
он возвратит [* false], выводится [^topic`:`/`/Core`/src`/CParser`_ru`-ru`#CParser`:`:Error`:`:struct^ C
Parser`::Error].&]
[s3; &]
[s4; &]
[s5;:CParser`:`:IsId`(`)const: [@(0.0.255) bool]_[* IsId]()_[@(0.0.255) const]&]
[s2;%RU-RU Тестирует, есть ли в текущей позиции 
какой`-либо `"Си`-образный`" идентификатор.&]
[s3; &]
[s4; &]
[s5;:CParser`:`:IsId`(const char`*`)const: [@(0.0.255) bool]_[* IsId]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 s])_[@(0.0.255) const]&]
[s2;%RU-RU Тестирует, есть ли в текущей позиции 
`"Си`-образный`" идентификатор [%-*@3 s].&]
[s3;%RU-RU &]
[s4; &]
[s5;:CParser`:`:ReadId`(`): [_^String^ String]_[* ReadId]()&]
[s2;%RU-RU Читает `"Си`-образный`" идентификатор 
из текущей позиции. Если он отсутствует, 
выбрасывает [^topic`:`/`/Core`/src`/CParser`_ru`-ru`#CParser`:`:Error`:`:struct^ C
Parser`::Error].&]
[s3; &]
[s4; &]
[s5;:CParser`:`:ReadIdt`(`): [_^String^ String]_[* ReadIdt]()&]
[s2;%RU-RU Особый вариант [^topic`:`/`/Core`/src`/CParser`_ru`-ru`#CParser`:`:ReadId`(`)^ R
eadId], который `"считает`" различные 
не`-`"альфачисловые`" символы частью 
идентификатора, поскольку они формируют 
нормальный или шаблонный тип C`+`+.&]
[s3; &]
[s4; &]
[s5;:CParser`:`:IsInt`(`)const: [@(0.0.255) bool]_[* IsInt]()_[@(0.0.255) const]&]
[s2;%RU-RU Тестирует на `"интеграл`" (целочисленное) 
в текущей позиции `- это должна быть 
цифра, или знак `'`+`', или знак  `'`-`', 
за которым следует любое число пробелов 
и цифра.&]
[s3; &]
[s4; &]
[s5;:CParser`:`:Sgn`(`): [@(0.0.255) int]_[* Sgn]()&]
[s2;%RU-RU Если в текущей позиции есть символы 
`'`-`' или `'`+`', они пропускаются. Если 
`'`-`' был пропущен, возвращается `-1, 
иначе 1.&]
[s3; &]
[s4; &]
[s5;:CParser`:`:ReadInt`(`): [@(0.0.255) int]_[* ReadInt]()&]
[s2;%RU-RU Читает целочисленное в текущей 
позиции. Если [* IsInt] false, выводится [^topic`:`/`/Core`/src`/CParser`_ru`-ru`#CParser`:`:Error`:`:struct^ C
Parser`::Error.]&]
[s3; &]
[s4; &]
[s5;:CParser`:`:ReadInt`(int`,int`): [@(0.0.255) int]_[* ReadInt]([@(0.0.255) int]_[*@3 min],
 [@(0.0.255) int]_[*@3 max])&]
[s2;%RU-RU Выполняет ReadInt, затем проверяет 
итог как [%-*@3 min] <`= итог <`= [%-*@3 max]. Если 
не так, выводит [^topic`:`/`/Core`/src`/CParser`_ru`-ru`#CParser`:`:Error`:`:struct^ C
Parse][^topic`:`/`/Core`/src`/CParser`$en`-us`#CParser`:`:Error`:`:struct^ r`::Error
], иначе возвращает его.&]
[s3;%RU-RU &]
[s4; &]
[s5;:CParser`:`:ReadInt64`(`): [_^int64^ int64]_[* ReadInt64]()&]
[s2;%RU-RU Читает 64`-битное целочисленное 
из текущей позиции. Если [* IsInt ]даёт 
false, выбрасывает [^topic`:`/`/Core`/src`/CParser`_ru`-ru`#CParser`:`:Error`:`:struct^ C
Parser`::Error.]&]
[s3; &]
[s4; &]
[s5;:CParser`:`:ReadInt64`(int64`,int64`): [_^int64^ int64]_[* ReadInt64]([_^int64^ int64]_
[*@3 min], [_^int64^ int64]_[*@3 max])&]
[s2;%RU-RU Выполняет ReadInt64, затем проверяет 
итог на [%-*@3 min] <`= итог <`= [%-*@3 max]. Если 
не так, выводит [^topic`:`/`/Core`/src`/CParser`_ru`-ru`#CParser`:`:Error`:`:struct^ C
Parse][^topic`:`/`/Core`/src`/CParser`$en`-us`#CParser`:`:Error`:`:struct^ r`::Error
], иначе возвращает его.&]
[s3; &]
[s4; &]
[s5;:CParser`:`:IsNumber`(`)const: [@(0.0.255) bool]_[* IsNumber]()_[@(0.0.255) const]&]
[s2;%RU-RU Тестирует на беззначное число 
в текущей позиции `- допускается цифра.&]
[s3; &]
[s4; &]
[s5;:CParser`:`:IsNumber`(int`)const: [@(0.0.255) bool]_[* IsNumber]([@(0.0.255) int]_[*@3 ba
se])_[@(0.0.255) const]&]
[s2;%RU-RU Тестируется на беззначное число 
с указанной базой (основанием): должна 
быть цифра или буква `'A`' `- `'Z`' или `'a`' 
`- `'z`', а диапазон ограничен действительной 
[%-*@3 base ](например, для основания 12 допускаются
 буквы `'a`' `'A`' `'b`' `'B`').&]
[s3; &]
[s4; &]
[s5;:CParser`:`:ReadNumber`(int`): [_^uint32^ uint32]_[* ReadNumber]([@(0.0.255) int]_[*@3 ba
se]_`=_[@3 10])&]
[s2;%RU-RU Читает число с заданным численным 
основанием [%-*C@3 base]. Если [* IsNumber]([%-*@3 base]) 
даёт false, выводится [^topic`:`/`/Core`/src`/CParser`_ru`-ru`#CParser`:`:Error`:`:struct^ C
Parser`::Error.]&]
[s3; &]
[s4; &]
[s5;:CParser`:`:ReadNumber64`(int`): [_^uint64^ uint64]_[* ReadNumber64]([@(0.0.255) int]_[*@3 b
ase]_`=_[@3 10])&]
[s2;%RU-RU Читает 64`-битное беззначное число 
с заданным численным основанием [%-*@3 base].&]
[s3;%RU-RU &]
[s4; &]
[s5;:CParser`:`:IsDouble`(`)const: [@(0.0.255) bool]_[* IsDouble]()_[@(0.0.255) const]&]
[s2;%RU-RU Тестирует на наличие числа с плавающей 
запятой в текущей позиции: это должна 
быть либо цифра, либо знак `'`+`', либо 
знак `'`-`', за которым следует любое 
число пробелов и цифра.&]
[s3;%RU-RU &]
[s4; &]
[s5;:CParser`:`:IsDouble2`(`)const: [@(0.0.255) bool]_[* IsDouble2]()_[@(0.0.255) const]&]
[s2;%RU-RU Подобно IsDouble, но также допускается, 
что двойное число начинается с десятичной 
точки, типа `'.21`'.&]
[s3; &]
[s4; &]
[s5;:CParser`:`:ReadDouble`(`): [@(0.0.255) double]_[* ReadDouble]()&]
[s2;%RU-RU Читает число с плавающей запятой 
по лексическим правилам Си. Как исключение 
этих правил, ReadDouble также распознаёт 
форму, начинающуюся с десятичной 
точки, типа `".21`".&]
[s3; &]
[s4; &]
[s5;:CParser`:`:IsString`(`)const: [@(0.0.255) bool]_[* IsString]()_[@(0.0.255) const]&]
[s2;%RU-RU Тестирует на Си`-обазный строковый 
литерал в текущей позиции. То же, что 
и [* IsChar](`'`\`"`');&]
[s3; &]
[s4; &]
[s5;:CParser`:`:ReadOneString`(bool`): [_^String^ String]_[* ReadOneString]([@(0.0.255) boo
l]_[*@3 chkend]_`=_[@(0.0.255) true])&]
[s2;%RU-RU Читает Си`-образный строковый 
литерал из текущей позиции (следуя 
лексическим правилам Си, включая 
`"искейп`"`-коды). Литералы на разных 
строках не конкатенируются (в отличие 
от Си). Когда [%-*C@3 chkend] i `= [* false], [* ReadOneString 
]более permissive (`"добрый`"), так как допускает 
неоконченные строковые литералы: 
string также затем ограничивается концом 
строки или текста.&]
[s3; &]
[s4; &]
[s5;:CParser`:`:ReadString`(bool`): [_^String^ String]_[* ReadString]([@(0.0.255) bool]_[*@3 c
hkend]_`=_[@(0.0.255) true])&]
[s2;%RU-RU Читает Си`-образный строковый 
литерал из текущей позиции (следуя 
лексическим правилам Си, включая 
`"искейп`"`-коды). Литералы на разных 
строках конкатенируются (как в Си). 
Когда [%-*C@3 chkend] i `= [* false], [* ReadOneString ]более 
permissive (`"добрый`"), так как допускает 
неоконченные строковые литералы: 
string также затем ограничивается концом 
строки или текста.&]
[s3; &]
[s4; &]
[s5;:CParser`:`:ReadOneString`(int`,bool`): [_^String^ String]_[* ReadOneString]([@(0.0.255) i
nt]_[*@3 delim], [@(0.0.255) bool]_[*@3 chkend]_`=_[@(0.0.255) true])&]
[s2;%RU-RU Читает Си`-образный строковый 
литерал из текущей позиции (следуя 
лексическим правилам Си, включая 
`"искейп`"`-коды), с другим разграничителем 
[%-*C@3 delim, ]а не `'`\`". Литералы на разных 
строках не конкатенируются (в отличие 
от Си). Когда [%-*C@3 chkend] i `= [* false], [* ReadOneString 
]более permissive (`"добрый`"), так как допускает 
неоконченные строковые литералы: 
string также затем ограничивается концом 
строки или текста.&]
[s3; &]
[s4; &]
[s5;:CParser`:`:ReadString`(int`,bool`): [_^String^ String]_[* ReadString]([@(0.0.255) int]_
[*@3 delim], [@(0.0.255) bool]_[*@3 chkend]_`=_[@(0.0.255) true])&]
[s2;%RU-RU Читает Си`-образный строковый 
литерал из текущей позиции (следуя 
лексическим правилам Си, включая 
`"искейп`"`-коды), с другим разграничителем 
[%-*C@3 delim, ]а не `'`\`". Литералы на разных 
строках конкатенируются (как в Си). 
Когда [%-*C@3 chkend] i `= [* false], [* ReadOneString ]более 
permissive (`"добрый`"), так как допускает 
неоконченные строковые литералы: 
string также затем ограничивается концом 
строки или текста.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:CParser`:`:Skip`(`): [@(0.0.255) void]_[* Skip]()&]
[s2;%RU-RU Пропускает единичный символ. 
Десятичные числа, идентификаторы 
и строковые литералы пропускаются 
как цельные символы, иначе позиция 
ввода продвигается вперёд на 1 символ.&]
[s3; &]
[s4; &]
[s5;:CParser`:`:SkipTerm`(`): [@(0.0.255) void]_[* SkipTerm]()&]
[s2;%RU-RU То же, что и Skip, старое название.&]
[s3; &]
[s4; &]
[s5;:CParser`:`:GetPtr`(`)const: [@(0.0.255) const]_[@(0.0.255) char]_`*[* GetPtr]()_[@(0.0.255) c
onst]&]
[s2;%RU-RU Возвращает указатель на текущую 
позицию.&]
[s3; &]
[s4; &]
[s5;:CParser`:`:GetSpacePtr`(`)const: [@(0.0.255) const]_[@(0.0.255) char]_`*[* GetSpacePtr
]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает указатель на позицию 
последнего пробела перд текпоз (текущей), 
или текпоз, если пробела не было. Этот 
указатель устанавливается перед 
вызовом Space, Space вызывается после обработки 
каждой семы (токена)(покуда SkipWhitespaces 
равен false).&]
[s3; &]
[s4; &]
[s5;:CParser`:`:GetPos`(`)const: [_^CParser`:`:Pos^ Pos]_[* GetPos]()_[@(0.0.255) const]&]
[s2;%RU-RU Получает (выдаёт!) текущую позицию. 
Содержит указатель, а также номер 
строки и имя файла.&]
[s3; &]
[s4; &]
[s5;:CParser`:`:SetPos`(const CParser`:`:Pos`&`): [@(0.0.255) void]_[* SetPos]([@(0.0.255) c
onst]_[_^topic`:`/`/Core`/src`/CParser`$en`-us`#CParser`:`:Pos`:`:struct^ CParser`:
:Pos][@(0.0.255) `&]_[*@3 p])&]
[s2; [%RU-RU Устанавливает текпоз в ][*@3 p][%RU-RU . 
][*@3 p ]может быть получен от другого 
[* CParser].&]
[s3; &]
[s4; &]
[s5;:CParser`:`:IsEof`(`)const: [@(0.0.255) bool]_[* IsEof]()_[@(0.0.255) const]&]
[s2;%RU-RU Тестирует на конец вводного текста.&]
[s3; &]
[s4; &]
[s5;:CParser`:`:operator bool`(`)const: [* operator_bool]()_[@(0.0.255) const]&]
[s2; Возвращает [* true], если конец файла 
не достигнут, [* false ]иначе.&]
[s3; &]
[s4; &]
[s5;:CParser`:`:GetLine`(`)const: [@(0.0.255) int]_[* GetLine]()_[@(0.0.255) const]&]
[s2; Возвращает номер текущей строки.&]
[s3; &]
[s4; &]
[s5;:CParser`:`:GetColumn`(int`)const: [@(0.0.255) int]_[* GetColumn]([@(0.0.255) int]_[*@3 t
absize]_`=_[@3 4])_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает текущую колонку, с 
заданным размером табуляции [%-*@3 tabsize].&]
[s3;%RU-RU &]
[s4; &]
[s5;:CParser`:`:GetFileName`(`)const: [_^String^ String]_[* GetFileName]()_[@(0.0.255) cons
t]&]
[s2; Возвращает актуальное имя файла.&]
[s3;%RU-RU &]
[s3; &]
[s4; &]
[s5;:CParser`:`:LineInfoComment`(const String`&`,int`,int`): [@(0.0.255) static] 
[_^String^ String]_[* LineInfoComment]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_
[*@3 filename], [@(0.0.255) int]_[*@3 line]_`=_[@3 1], [@(0.0.255) int]_[*@3 column]_`=_[@3 1])
&]
[s2;%RU-RU Эта функция создаёт особый комментарий, 
который, при парсировании CParser`'ом, 
переключает имя файла и номер строки. 
Это должно быть полезно в ситуациях, 
когда парсированный (разобранный) 
текст на деле является результатом, 
например, операций включения каких`-то 
исходников, чтобы улучшить отчёт 
об ошибках. Такой коммент лексически 
рассматривается как комментарий. 
Коммент создаётся с использованием 
разграничителя начала/конца символов 
LINEINFO`_ESC (текущее значение равно `'`\2`').&]
[s3;%RU-RU &]
[s4; &]
[s5;:CParser`:`:GetLineInfoComment`(int`)const: [_^String^ String]_[* GetLineInfoComment](
[@(0.0.255) int]_[*@3 tabsize]_`=_[@3 4])_[@(0.0.255) const]&]
[s2;%RU-RU Вызывает LineInfoComment(GetFileName(), GetLine(), 
GetColumn([%-*@3 tabsize])): создаёт коммент, идентифициру
ющий текущую файловую позицию при 
дальнейшей обработке.&]
[s3; &]
[s4; &]
[s5;:CParser`:`:Set`(const char`*`,const char`*`,int`): [@(0.0.255) void]_[* Set]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 ptr], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 fn], 
[@(0.0.255) int]_[*@3 line]_`=_[@3 1])&]
[s2;%RU-RU Устанавливает новую строку ввода 
(с именем файла и номером строки).&]
[s3;%RU-RU &]
[s4; &]
[s5;:CParser`:`:Set`(const char`*`): [@(0.0.255) void]_[* Set]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 ptr])&]
[s2;%RU-RU Устанавливает новую вводную строку.&]
[s3;%RU-RU &]
[s0;%RU-RU &]
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 CParser`::Error]]}}&]
[s3; &]
[s1;:CParser`:`:Error`:`:struct: [@(0.0.255)3 struct][3 _][*3 Error][3 _:_][@(0.0.255)3 public][3 _
][*@3;3 Exc]&]
[s0; &]
[s0;%RU-RU Тип, используемый как исключение 
[^topic`:`/`/Core`/src`/CParser`_ru`-ru`#CParser`:`:class^ CParser]. 
Содержит единичную [^topic`:`/`/Core`/src`/String`_ru`-ru`#String`:`:class^ S
tring] с описанием ошибки.&]
[s0;/%RU-RU &]
[s0; [%RU-RU/ Производное от][%RU-RU  ][^topic`:`/`/Core`/src`/CParser`_ru`-ru`#Exc`:`:Exc`(`)^ E
xc]&]
[s3;%RU-RU &]
[s0;%RU-RU &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Детали Конструктора]]}}&]
[s3; &]
[s5;:Exc`:`:Exc`(`): [* Exc]()&]
[s2;%RU-RU Дефолтный конструктор. Сообщение 
об ошибке пустое.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Exc`:`:Exc`(const String`&`): [* Exc]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[*@3 desc])&]
[s2; [%RU-RU Конструирует ][%RU-RU* Error][%RU-RU  с ][*@3 desc] 
как сообщением об ошибке.&]
[s3;%RU-RU &]
[s0;%RU-RU &]
[s0;%RU-RU &]
[s0;%RU-RU &]
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 CParser`::Pos]]}}&]
[s3;%RU-RU &]
[s1;:CParser`:`:Pos`:`:struct: [@(0.0.255)3 struct][3 _][*3 Pos]&]
[s3; &]
[s5;:`:`:CParser`:`:Pos`:`:struct:%RU-RU Позиция в разбираемом 
тексте.&]
[s3;%RU-RU &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Детали Конструктора]]}}&]
[s3; &]
[s5;:CParser`:`:Pos`:`:Pos`(const char`*`,int`,String`): [* Pos]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 ptr]_`=_NULL, [@(0.0.255) int]_[*@3 line]_`=_[@3 1], [_^String^ String]_[*@3 fn]_
`=_Null)&]
[s2; Конструирует [* Pos, ]основываясь на 
указателе на буфер ввода, носере строки 
и имени файла.&]
[s7;%RU-RU [%-*@3 ptr]-|Указатель на позицию в водном 
файле &]
[s7;%RU-RU [%-*@3 line]-|Номер строки.&]
[s7;%RU-RU [%-*@3 fn]-|Имя файла.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Публичных Членов]]}}&]
[s3; &]
[s5;:CParser`:`:Pos`:`:ptr: [@(0.0.255) const]_[@(0.0.255) char]_`*[* ptr]&]
[s2;%RU-RU Указатель на позицию в вводном 
тексте &]
[s3;%RU-RU &]
[s4; &]
[s5;:CParser`:`:Pos`:`:wspc: [@(0.0.255) const]_[@(0.0.255) char]_`*[* wspc]&]
[s2;%RU-RU Указатель на позицию последнего 
пробела перед текущей позицией, либо 
на текущую позицию, если пробелов 
не было.&]
[s3; &]
[s4; &]
[s5;:CParser`:`:Pos`:`:lineptr: [@(0.0.255) const]_[@(0.0.255) char]_`*[* lineptr]&]
[s2;%RU-RU Указатель на начало последней 
строки.&]
[s3; &]
[s4; &]
[s5;:CParser`:`:Pos`:`:line: [@(0.0.255) int]_[* line]&]
[s2;%RU-RU Номер строки.&]
[s3;%RU-RU &]
[s4; &]
[s5;:CParser`:`:Pos`:`:fn: [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ St
ring]_[* fn]&]
[s2;%RU-RU Имя файла.&]
[s3;%RU-RU &]
[s4; &]
[s5;:CParser`:`:Pos`:`:GetColumn`(int`)const: [@(0.0.255) int]_[* GetColumn]([@(0.0.255) in
t]_[*@3 tabsize]_`=_[@3 4])_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает колонку, при заданном 
[%-*@3 tabsize].&]
[s0;%RU-RU &]
[s0;%RU-RU &]
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Форматирование `"Си`-образного`" 
строкового литерала]]}}&]
[s3;%RU-RU &]
[s0;%RU-RU Процедуры [* AsCString] производят Си`-образные
 литералы (совместимые с [^topic`:`/`/Core`/src`/CParser`_ru`-ru`#CParser`:`:class^ C
Parser]) из символьных данных:&]
[s0;%RU-RU &]
[s5;:AsCString`(const char`*`,const char`*`,int`,const char`*`,dword`): [_^String^ Stri
ng]_[* AsCString]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 end], [@(0.0.255) int]_[*@3 linemax]_`=_INT`_MAX, [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 linepfx]_`=_NULL, [_^dword^ dword]_[*@3 flags]_`=_[@3 0])&]
[s2;%RU-RU Создаёт `"Си`-образный`" литерал.&]
[s7;%RU-RU [%-*@3 s]-|Указатель на символы.&]
[s7;%RU-RU [%-*@3 end] -|Конец массива символов 
(Символы `'`\0`' допускаются внутри данных).&]
[s7;%RU-RU [%-*@3 linemax] -|Максимальная длина строки. 
Если она превышена, вставляется окончание 
`"`\`"`\n`" и [@3 linepfx ]и литерал продолжается 
на новой строке.&]
[s7;%RU-RU [%-*@3 linepfx]-|Указатель на текст с нулевым 
окончанием, вставляемый в начале 
строки, когда длина строки превышена.&]
[s7;%RU-RU [%-*@3 flags]-|комбинация флагов:&]
[s0;~~~1408;%RU-RU -|ASCSTRING`_SMART-|разбивает строку 
по строчкам, если слишком длинная&]
[s0;~~~1408;%RU-RU -|ASCSTRING`_OCTALHI-|искапирует символы 
>128&]
[s0;~~~1408;%RU-RU -|ASCSTRING`_JSON-|использовать нотацию 
JSON  для искейпов &]
[s0;~~~1408;%RU-RU -|(`\u0001 вместо `\001)&]
[s0;~~~1408;%RU-RU &]
[s7;%RU-RU [*/ Возвратное значение]-|Си`-образный 
литерал.&]
[s3;%RU-RU &]
[s4; &]
[s5;:AsCString`(const char`*`,int`,const char`*`,dword`): [_^String^ String]_[* AsCString
]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s], [@(0.0.255) int]_[*@3 linemax]_`=_INT`_MA
X, [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 linepfx]_`=_NULL, [_^dword^ dword]_[*@3 flag
s]_`=_[@3 0])&]
[s2;%RU-RU Создаёт Си`-образный литерал из 
символьных данных с нулевым окончанием. 
То же, что и [^topic`:`/`/Core`/src`/CParser`$en`-us`#AsCString`(const char`*`,const char`*`,int`,const char`*`,dword`)^ A
sCString]([%-*@3 s],[%- _][%-*@3 s][%- _]`+[%- _][* strlen]([%-*@3 s]),[%- _][%-*@3 linemax],[%- _][%-*@3 l
inepfx],[%- _][%-*@3 flags]).&]
[s3;%RU-RU &]
[s4; &]
[s5;:AsCString`(const String`&`,int`,const char`*`,dword`): [_^String^ String]_[* AsCStri
ng]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 s], [@(0.0.255) int]_[*@3 linem
ax]_`=_INT`_MAX, [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 linepfx]_`=_NULL, 
[_^dword^ dword]_[*@3 flags]_`=_[@3 0])&]
[s2;%RU-RU Создаёт Си`-образный литерал из 
[^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ String]. 
String может содержать символы ноль. 
То же, что и [^topic`:`/`/Core`/src`/CParser`$en`-us`#AsCString`(const char`*`,const char`*`,int`,const char`*`,dword`)^ A
sCString]([%-*@3 s].[^topic`:`/`/Core`/src`/String`$en`-us`#String0`:`:Begin`(`)const^ B
egin](),[%- _][%-*@3 s].[^topic`:`/`/Core`/src`/String`$en`-us`#String0`:`:End`(`)const^ E
nd](),[%- _][%-*@3 linemax],[%- _][%-*@3 linepfx],[%- _][%-*@3 flags]).&]
[s3;%RU-RU &]
[s0; ]]