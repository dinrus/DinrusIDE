topic "Функции Split, Join, Merge";
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
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Split, Join, Merge]]}}&]
[s0;i448;a25;kKO9;@(0.0.255) &]
[s0;%RU-RU [* Утилитные Функции для разбиения 
и объединения String`'ов и WString`'ов.]&]
[s0;*%RU-RU &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Функций]]}}&]
[s3; &]
[s5;:Split`(int`,const char`*`,const char`*`(`*`)`(const char`*`)`,bool`): [_^Vector^ V
ector]<[_^String^ String]>_[* Split]([@(0.0.255) int]_[*@3 maxcount], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s], [@(0.0.255) const]_[@(0.0.255) char]_`*_(`*
[*@3 text`_filter])([@(0.0.255) const]_[@(0.0.255) char]_`*), [@(0.0.255) bool]_[*@3 ignore
empty]_`=_[@(0.0.255) true])&]
[s5;:Split`(int`,const char`*`,int`(`*`)`(int`)`,bool`): [_^Vector^ Vector]<[_^String^ St
ring]>_[* Split]([@(0.0.255) int]_[*@3 maxcount], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s
], [@(0.0.255) int]_(`*[*@3 filter])([@(0.0.255) int]), [@(0.0.255) bool]_[*@3 ignoreempty]_
`=_[@(0.0.255) true])&]
[s5;:Split`(int`,const char`*`,int`,bool`): [_^Vector^ Vector]<[_^String^ String]>_[* Split
]([@(0.0.255) int]_[*@3 maxcount], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s], 
[@(0.0.255) int]_[*@3 chr], [@(0.0.255) bool]_[*@3 ignoreempty]_`=_[@(0.0.255) true])&]
[s5;:Split`(int`,const char`*`,const char`*`,bool`): [_^Vector^ Vector]<[_^String^ String
]>_[* Split]([@(0.0.255) int]_[*@3 maxcount], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s],
 [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 text], [@(0.0.255) bool]_[*@3 ignoreempty]_`=_
[@(0.0.255) true])&]
[s5;:Split`(const char`*`,const char`*`(`*`)`(const char`*`)`,bool`): [_^Vector^ Vector
]<[_^String^ String]>_[* Split]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s], 
[@(0.0.255) const]_[@(0.0.255) char]_`*_(`*[*@3 text`_filter])([@(0.0.255) const]_[@(0.0.255) c
har]_`*), [@(0.0.255) bool]_[*@3 ignoreempty]_`=_[@(0.0.255) true])&]
[s5;:Split`(const char`*`,int`(`*`)`(int`)`,bool`): [_^Vector^ Vector]<[_^String^ String]>
_[* Split]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s], [@(0.0.255) int]_(`*[*@3 filter])(
[@(0.0.255) int]), [@(0.0.255) bool]_[*@3 ignoreempty]_`=_[@(0.0.255) true])&]
[s5;:Split`(const char`*`,int`,bool`): [_^Vector^ Vector]<[_^String^ String]>_[* Split]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 s], [@(0.0.255) int]_[*@3 chr], [@(0.0.255) bool]_[*@3 ignoree
mpty]_`=_[@(0.0.255) true])&]
[s5;:Split`(const char`*`,const char`*`,bool`): [_^Vector^ Vector]<[_^String^ String]>_[* S
plit]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s], [@(0.0.255) const]_[@(0.0.255) char]_
`*[*@3 text], [@(0.0.255) bool]_[*@3 ignoreempty]_`=_[@(0.0.255) true])&]
[s5;:Split`(int`,const wchar`*`,const wchar`*`(`*`)`(const wchar`*`)`,bool`): [_^Vector^ V
ector]<[_^WString^ WString]>_[* Split]([@(0.0.255) int]_[*@3 maxcount], 
[@(0.0.255) const]_[_^wchar^ wchar]_`*[*@3 s], [@(0.0.255) const]_[_^wchar^ wchar]_`*_(`*[*@3 t
ext`_filter])([@(0.0.255) const]_wchar_`*), [@(0.0.255) bool]_[*@3 ignoreempty]_`=_[@(0.0.255) t
rue])&]
[s5;:Split`(int`,const wchar`*`,int`(`*`)`(int`)`,bool`): [_^Vector^ Vector]<[_^WString^ W
String]>_[* Split]([@(0.0.255) int]_[*@3 maxcount], [@(0.0.255) const]_[_^wchar^ wchar]_`*[*@3 s
], [@(0.0.255) int]_(`*[*@3 filter])([@(0.0.255) int]), [@(0.0.255) bool]_[*@3 ignoreempty]_
`=_[@(0.0.255) true])&]
[s5;:Split`(int`,const wchar`*`,int`,bool`): [_^Vector^ Vector]<[_^WString^ WString]>_[* Sp
lit]([@(0.0.255) int]_[*@3 maxcount], [@(0.0.255) const]_[_^wchar^ wchar]_`*[*@3 s], 
[@(0.0.255) int]_[*@3 chr], [@(0.0.255) bool]_[*@3 ignoreempty]_`=_[@(0.0.255) true])&]
[s5;:Split`(int`,const wchar`*`,const wchar`*`,bool`): [_^Vector^ Vector]<[_^WString^ WSt
ring]>_[* Split]([@(0.0.255) int]_[*@3 maxcount], [@(0.0.255) const]_[_^wchar^ wchar]_`*[*@3 s
], [@(0.0.255) const]_[_^wchar^ wchar]_`*[*@3 text], [@(0.0.255) bool]_[*@3 ignoreempty]_`=
_[@(0.0.255) true])&]
[s5;:Split`(const wchar`*`,const wchar`*`(`*`)`(const wchar`*`)`,bool`): [_^Vector^ Vec
tor]<[_^WString^ WString]>_[* Split]([@(0.0.255) const]_[_^wchar^ wchar]_`*[*@3 s], 
[@(0.0.255) const]_[_^wchar^ wchar]_`*_(`*[*@3 text`_filter])([@(0.0.255) const]_wchar_`*
), [@(0.0.255) bool]_[*@3 ignoreempty]_`=_[@(0.0.255) true])&]
[s5;:Split`(const wchar`*`,int`(`*`)`(int`)`,bool`): [_^Vector^ Vector]<[_^WString^ WStri
ng]>_[* Split]([@(0.0.255) const]_[_^wchar^ wchar]_`*[*@3 s], [@(0.0.255) int]_(`*[*@3 filter
])([@(0.0.255) int]), [@(0.0.255) bool]_[*@3 ignoreempty]_`=_[@(0.0.255) true])&]
[s5;:Split`(const wchar`*`,int`,bool`): [_^Vector^ Vector]<[_^WString^ WString]>_[* Split](
[@(0.0.255) const]_[_^wchar^ wchar]_`*[*@3 s], [@(0.0.255) int]_[*@3 chr], 
[@(0.0.255) bool]_[*@3 ignoreempty]_`=_[@(0.0.255) true])&]
[s5;:Split`(const wchar`*`,const wchar`*`,bool`): [_^Vector^ Vector]<[_^WString^ WString]>
_[* Split]([@(0.0.255) const]_[_^wchar^ wchar]_`*[*@3 s], [@(0.0.255) const]_[_^wchar^ wchar]_
`*[*@3 text], [@(0.0.255) bool]_[*@3 ignoreempty]_`=_[@(0.0.255) true])&]
[s2;%RU-RU Разбивает текст [%-*@3 s] на подтексты, 
расположенные между разграничителями. 
Разграничитель (Delimiter) может быть 
определён как единичный символ [%-*@3 chr], 
функция фильтрации текста [%-*@3 text`_filter 
](возвращает позицию за разграничителем 
или NULL, если разграничитель не на 
текущем символе), функцию фильтрации 
символов [%-*@3 filter] (возвращает всё, кроме 
нуля, для символа`-разграничителя) 
или как текст [%-*@3 text]. Если [%-*@3 ignoreempty] 
равно true (дефолт), пустые подтексты 
игнорируются. [%-*@3 maxcount] может определять 
верхний предел числа подтекстов.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Join`(const Vector`<String`>`&`,const String`&`,bool`): [_^String^ String]_[* Join](
[@(0.0.255) const]_[_^Vector^ Vector]<[_^String^ String]>`&_[*@3 im], 
[@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 delim], [@(0.0.255) bool]_[*@3 igno
reempty]_`=_[@(0.0.255) false])&]
[s5;:Join`(const Vector`<WString`>`&`,const WString`&`,bool`): [_^WString^ WString]_[* Jo
in]([@(0.0.255) const]_[_^Vector^ Vector]<[_^WString^ WString]>`&_[*@3 im], 
[@(0.0.255) const]_[_^WString^ WString][@(0.0.255) `&]_[*@3 delim], [@(0.0.255) bool]_[*@3 ig
noreempty]_`=_[@(0.0.255) false])&]
[s2;%RU-RU Объединяет тексты из [%-*@3 im], вставляя 
между ними разграничитель [%-*@3 delim]. 
Если [%-*@3 ignoreempty] равно true, пустые тексты 
игнорируются. Заметьте, что дефолтное 
значение [%-*@3 ignoreempty] противоположно 
тому, которое в Split.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:SplitTo`(const char`*`,int`,bool`,String`&`.`.`.`): [@(0.0.255) bool]_[* SplitTo]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 s], [@(0.0.255) int]_[*@3 delim], [@(0.0.255) bool]_[*@3 ignor
eempty], [_^String^ String][@(0.0.255) `&]_[*@3 p1][@(0.0.255) ...])&]
[s5;:SplitTo`(const char`*`,int`,String`&`.`.`.`): [@(0.0.255) bool]_[* SplitTo]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 s], [@(0.0.255) int]_[*@3 delim], [_^String^ String][@(0.0.255) `&
]_[*@3 p1][@(0.0.255) ...])&]
[s5;:SplitTo`(const char`*`,int`(`*`)`(int`)`,String`&`.`.`.`): [@(0.0.255) bool]_[* Spli
tTo]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s], [@(0.0.255) int]_(`*[*@3 filter])([@(0.0.255) i
nt]), String[@(0.0.255) `&]_[*@3 p1][@(0.0.255) ...])&]
[s5;:SplitTo`(const char`*`,const char`*`,bool`,String`&`.`.`.`): [@(0.0.255) bool]_[* Sp
litTo]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s], [@(0.0.255) const]_[@(0.0.255) char]_
`*[*@3 delim], [@(0.0.255) bool]_[*@3 ignoreempty], [_^String^ String][@(0.0.255) `&]_[*@3 p1
][@(0.0.255) ...])&]
[s5;:SplitTo`(const char`*`,const char`*`,String`&`.`.`.`): [@(0.0.255) bool]_[* SplitTo](
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 d
elim], [_^String^ String][@(0.0.255) `&]_[*@3 p1][@(0.0.255) ...])&]
[s5;:SplitTo`(const wchar`*`,int`,bool`,WString`&`.`.`.`): [@(0.0.255) bool]_[* SplitTo](
[@(0.0.255) const]_[_^wchar^ wchar]_`*[*@3 s], [@(0.0.255) int]_[*@3 delim], 
[@(0.0.255) bool]_[*@3 ignoreempty], [_^WString^ WString][@(0.0.255) `&]_[*@3 p1][@(0.0.255) .
..])&]
[s5;:SplitTo`(const wchar`*`,int`,WString`&`.`.`.`): [@(0.0.255) bool]_[* SplitTo]([@(0.0.255) c
onst]_[_^wchar^ wchar]_`*[*@3 s], [@(0.0.255) int]_[*@3 delim], [_^WString^ WString][@(0.0.255) `&
]_[*@3 p1][@(0.0.255) ...])&]
[s5;:SplitTo`(const wchar`*`,int`(`*`)`(int`)`,WString`&`.`.`.`): [@(0.0.255) bool]_[* Sp
litTo]([@(0.0.255) const]_[_^wchar^ wchar]_`*[*@3 s], [@(0.0.255) int]_(`*[*@3 filter])([@(0.0.255) i
nt]), WString[@(0.0.255) `&]_[*@3 p1][@(0.0.255) ...])&]
[s5;:SplitTo`(const wchar`*`,const wchar`*`,bool`,WString`&`.`.`.`): [@(0.0.255) bool]_
[* SplitTo]([@(0.0.255) const]_[_^wchar^ wchar]_`*[*@3 s], [@(0.0.255) const]_[_^wchar^ wchar
]_`*[*@3 delim], [@(0.0.255) bool]_[*@3 ignoreempty], [_^WString^ WString][@(0.0.255) `&]_[*@3 p
1][@(0.0.255) ...])&]
[s5;:SplitTo`(const wchar`*`,const wchar`*`,WString`&`.`.`.`): [@(0.0.255) bool]_[* Split
To]([@(0.0.255) const]_[_^wchar^ wchar]_`*[*@3 s], [@(0.0.255) const]_[_^wchar^ wchar]_`*[*@3 d
elim], [_^WString^ WString][@(0.0.255) `&]_[*@3 p1][@(0.0.255) ...])&]
[s2;%RU-RU Разбивает текст на одну или более 
целевых подстрок, вставляя их в строчные 
переменные (текущая реализация поддерживает
 до 8 выводных строк). Возвращает true, 
если исходный текст содержит достаточно 
подстрок. Разграничитель может определяться
 как единичный символ [%-*@3 chr], функция 
фильтровки символов [%-*@3 filter] (возвращает 
не ноль для символа`-разграничителя) 
или как текст [%-*@3 text]. Если [%-*@3 ignoreempty] 
равно true (дефолт), пустые подтексты 
игнорируются. [%-*@3 maxcount] может определять 
верхний предел числа подтекстов.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Merge`(const char`*`,String`&`.`.`.`): [_^String^ String]_[* Merge]([@(0.0.255) const]_
[@(0.0.255) char]_`*[*@3 delim], [_^String^ String][@(0.0.255) `&]_[*@3 p1][@(0.0.255) ...])&]
[s5;:Merge`(const wchar`*`,WString`&`.`.`.`): [_^WString^ WString]_[* Merge]([@(0.0.255) co
nst]_[_^wchar^ wchar]_`*[*@3 delim], [_^WString^ WString][@(0.0.255) `&]_[*@3 p1][@(0.0.255) .
..])&]
[s2;%RU-RU `"Маржирует`" подстроки. Возвращает 
исходные строки, конкатенированные 
с постановкой разграничителя между 
ними, однако пустые строки игнорируются 
(означает, что Merge(`";`", `"1`", `"`") приводит 
к `"1`", а не к `"1;`").&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:MergeWith`(String`&`,const char`*`,String`&`.`.`.`): [@(0.0.255) void]_[* MergeWith](
[_^String^ String][@(0.0.255) `&]_[*@3 dest], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 del
im], [_^String^ String][@(0.0.255) `&]_[*@3 p1][@(0.0.255) ...])&]
[s5;:MergeWith`(WString`&`,const wchar`*`,WString`&`.`.`.`): [@(0.0.255) void]_[* MergeWi
th]([_^WString^ WString][@(0.0.255) `&]_[*@3 dest], [@(0.0.255) const]_[_^wchar^ wchar]_`*[*@3 d
elim], [_^WString^ WString][@(0.0.255) `&]_[*@3 p1][@(0.0.255) ...])&]
[s2;%RU-RU `"Маржирует`" подстроки с dest. [%-*@3 dest] 
и исходные строки конкатенируются 
с постановкой между ними разграничителя, 
однако пустые строки игнорируются.&]
[s3;%RU-RU ]]