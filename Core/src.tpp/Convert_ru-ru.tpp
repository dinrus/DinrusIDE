topic "Класс Convert";
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
[{_}%RU-RU 
[ {{10000@(113.42.0) [s0; [*@7;4 Класс Convert]]}}&]
[s3;%- &]
[s1;:Convert`:`:class:%- [@(0.0.255)3 class][3 _][*3 Convert]&]
[s9; Производные от Convert классы служат 
как двунаправленные конвертеры Value`-Value 
(Значение`- Значение). Одно направление 
представлено методом Format, и, как правило, 
преобразует Value в текстовое представление. 
Противоположное направление представлено 
методом Scan.&]
[s9; Если преобразование невозможно (например, 
из`-за неверного текстового представления), 
методы класса должны возвращать `"ErrorValue`", 
указывая на проблему.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Список Публичных Членов]]}}&]
[s3;%- &]
[s5;:Convert`:`:Format`(const Value`&`)const:%- [@(0.0.255) virtual] 
[_^Value^ Value]_[* Format]([@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 q])_[@(0.0.255) c
onst]&]
[s2; [%- Преобразует данное Value. В дефолтной 
реализации используется] Value`::ToString 
для преобразование заданного Значения.&]
[s7; [%-*C@3 q]-|Значение.&]
[s7; [*/ Возвратное значение]-|Преобразованное 
Значение.&]
[s3; &]
[s4;%- &]
[s5;:Convert`:`:Scan`(const Value`&`)const:%- [@(0.0.255) virtual] 
[_^Value^ Value]_[* Scan]([@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 text])_[@(0.0.255) c
onst]&]
[s2; Преобразует Value в противоположном 
направлении. Дефолтная реализация 
возвращает [@3 text] (без всякого преобразования)
.&]
[s7; [%-*C@3 text]-|Значение.&]
[s7; [*/ Возвратное значение]-|Преобразованное 
Значение.&]
[s3; &]
[s4;%- &]
[s5;:Convert`:`:Filter`(int`)const:%- [@(0.0.255) virtual] [@(0.0.255) int]_[* Filter]([@(0.0.255) i
nt]_[*@3 chr])_[@(0.0.255) const]&]
[s2; Настраивает символы, допустимые 
при текстовом представлении Значения.&]
[s7; [%-*C@3 chr]-|Настраиваемый символ.&]
[s7; [*/ Возвратное значение]-|Настроенный 
символ. Ноль указывает на то, что символ 
отвергнут. Filter может менять входные 
символы (например, преобразуя в заглавные, 
меняя `',`' на `'.`', и т.п.). Дефолтная реализация 
возвращает неизменённый [@3 chr].&]
[s3; &]
[s4;%- &]
[s5;:Convert`:`:operator`(`)`(const Value`&`)const:%- [_^Value^ Value]_[* operator()]([@(0.0.255) c
onst]_[_^Value^ Value][@(0.0.255) `&]_[*@3 q])_[@(0.0.255) const]&]
[s2; Вызывает метод Format. Функторный синтаксис 
вспомогательного характера.&]
[s7; [%-*C@3 q]-|Значение.&]
[s7; [*/ Возвратное значение]-|Преобразованное 
Значение.&]
[s0;%- &]
[ {{10000@(113.42.0) [s0; [*@7;4 ConvertDate]]}}&]
[s3;%- &]
[s1;:ConvertDate`:`:class:%- [@(0.0.255)3 class][3 _][*3 ConvertDate][3 _:_][@(0.0.255)3 public][3 _
][*@3;3 Convert]&]
[s9; Преобразователь дат. Текстовый формат 
даты зависит от действительных региональных
 настроек.&]
[s9; &]
[s0;%- [%RU-RU Производный от ][^topic`:`/`/Core`/src`/Convert`_ru`-ru`#Convert`:`:class^ C
onvert]&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Детали Конструктора]]}}&]
[s3;%- &]
[s5;:ConvertDate`:`:ConvertDate`(Date`,Date`,bool`):%- [* ConvertDate]([_^Date^ Date]_[*@3 m
inval]_`=_Date`::Low(), [_^Date^ Date]_[*@3 maxval]_`=_Date`::High(), 
[@(0.0.255) bool]_[*@3 notnull]_`=_[@(0.0.255) false])&]
[s2; Конструктор.&]
[s7; [%-*C@3 minval]-|Минимальная допустимая дата.&]
[s7; [%-*C@3 maxval]-|Максимальная допустимая 
дата.&]
[s7; [%-*C@3 notnull]-|Запретить Null`'ы.&]
[s3; &]
[s4;%- &]
[s5;:ConvertDate`:`:`~ConvertDate`(`):%- [@(0.0.255) `~][* ConvertDate]()&]
[s2; Дефолтный конструктор.&]
[s3;%- &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Список Публичных Членов]]}}&]
[s3;%- &]
[s5;:ConvertDate`:`:MinMax`(Date`,Date`):%- [_^ConvertDate^ ConvertDate][@(0.0.255) `&]_[* M
inMax]([_^Date^ Date]_[*@3 `_min], [_^Date^ Date]_[*@3 `_max])&]
[s2; Устанавливает допустимый диапазон 
дат.&]
[s7; [%-*C@3 `_min]-|Минимальная дата.&]
[s7; [%-*C@3 `_max]-|Максимальная дата.&]
[s7; [*/ Возвратное значение]-|`*this.&]
[s3; &]
[s4;%- &]
[s5;:ConvertDate`:`:Min`(Date`):%- [_^ConvertDate^ ConvertDate][@(0.0.255) `&]_[* Min]([_^Date^ D
ate]_[*@3 `_min])&]
[s2; Устанавливает минимально допустимую 
дату.&]
[s7; [%-*C@3 `_min]-|Минимальная дата.&]
[s7; [*/ Возвратное значение]-|`*this.&]
[s3; &]
[s4;%- &]
[s5;:ConvertDate`:`:Max`(Date`):%- [_^ConvertDate^ ConvertDate][@(0.0.255) `&]_[* Max]([_^Date^ D
ate]_[*@3 `_max])&]
[s2; Устанавливает максимально допустимую 
дату.&]
[s7; [%-*C@3 `_max]-|Максимальная дата.&]
[s7; [*/ Возвратное значение]-|`*this.&]
[s3; &]
[s4;%- &]
[s5;:ConvertDate`:`:NotNull`(bool`):%- [_^ConvertDate^ ConvertDate][@(0.0.255) `&]_[* NotNu
ll]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; Позволяет/запрещает даты Null. (Даты 
Null представлены пустым текстом).&]
[s7; [%-*C@3 b]-|true, если надо запретить даты 
Null.&]
[s7; [*/ Возвратное значение]-|`*this.&]
[s3; &]
[s4;%- &]
[s5;:ConvertDate`:`:NoNotNull`(`):%- [_^ConvertDate^ ConvertDate][@(0.0.255) `&]_[* NoNotNu
ll]()&]
[s2; То же, что и [^topic`:`/`/Core`/src`/Convert`$en`-us`#ConvertDate`:`:NotNull`(bool`)^ N
otNull](false).&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:ConvertDate`:`:Default`(Upp`:`:Date`):%- [_^Upp`:`:ConvertDate^ ConvertDate
][@(0.0.255) `&]_[* Default]([_^Upp`:`:Date^ Date]_[*@3 d])&]
[s2; Когда текст пустой, Scan возвращает 
это значение.&]
[s3; &]
[s4;%- &]
[s5;:ConvertDate`:`:GetMin`(`)const:%- [_^Date^ Date]_[* GetMin]()_[@(0.0.255) const]&]
[s2; Возвращает минимально допустимую 
дату. Это максимум значения, установленного 
с помощью Min, и дефолтный минимум (GetDefaultMin).&]
[s3; &]
[s4;%- &]
[s5;:ConvertDate`:`:GetMax`(`)const:%- [_^Date^ Date]_[* GetMax]()_[@(0.0.255) const]&]
[s2; Возвращает верхний предел допустимых 
дат. Это минимум значения, установленного 
через Max, и дефолтный максимум (GetDefaultMax).&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:ConvertDate`:`:GetDefaultMin`(`):%- [@(0.0.255) static] 
[_^Upp`:`:Date^ Date]_[* GetDefaultMin]()&]
[s2; Возвращает дефолтный минимум.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:ConvertDate`:`:GetDefaultMax`(`):%- [@(0.0.255) static] 
[_^Upp`:`:Date^ Date]_[* GetDefaultMax]()&]
[s2; Возвращает дефолтный максимиум.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:ConvertDate`:`:SetDefaultMinMax`(Upp`:`:Date`,Upp`:`:Date`):%- [@(0.0.255) v
oid]_[* SetDefaultMinMax]([_^Upp`:`:Date^ Date]_[*@3 min], [_^Upp`:`:Date^ Date]_[*@3 max])
&]
[s2; Устанавливает значения дефолтных 
минимума и максимума. Дефолтные значения 
`- Date`::Low() и Date`::Hight().&]
[s3;%- &]
[s4;%- &]
[s5;:ConvertDate`:`:IsNotNull`(`)const:%- [@(0.0.255) bool]_[* IsNotNull]()_[@(0.0.255) con
st]&]
[s7; [*/ Возвратное значение]-|true, если Null`'ы 
запрещены.&]
[s0;%- &]
[ {{10000@(113.42.0) [s0; [*@7;4 Класс ConvertTime]]}}&]
[s3; &]
[s1;:ConvertTime`:`:class:%- [@(0.0.255)3 class][3 _][*3 ConvertTime][3 _:_][@(0.0.255)3 public][3 _
][*@3;3 Convert]&]
[s9; Преобразователь времени.&]
[s0; &]
[s0;%- [%RU-RU/ Производный от][%RU-RU  ][^topic`:`/`/Core`/src`/Convert`_ru`-ru`#Convert`:`:class^ C
onvert]&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Детали Конструктора]]}}&]
[s3;%- &]
[s5;:ConvertTime`:`:ConvertTime`(Time`,Time`,bool`):%- [* ConvertTime]([_^Time^ Time]_[*@3 m
inval]_`=_ToTime(Date`::Low()), [_^Time^ Time]_[*@3 maxval]_`=_ToTime(Date`::High()), 
[@(0.0.255) bool]_[*@3 notnull]_`=_[@(0.0.255) false])&]
[s2;%- Конструктор.&]
[s7; [%-*C@3 minval]-|Нижний лимит. По дефолту лимит 
отсутствует.&]
[s7; [%-*C@3 maxval]-|Верхний лимит. По дефолту 
лимит отсутствует.&]
[s7; [%-*C@3 notnull]-|true запрещает Null`'ы.&]
[s3;%- &]
[s4;%- &]
[s5;:ConvertTime`:`:`~ConvertTime`(`):%- [@(0.0.255) `~][* ConvertTime]()&]
[s2; Дефолтный конструктор.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Список Публичных Членов]]}}&]
[s3;%- &]
[s5;:ConvertTime`:`:MinMax`(Time`,Time`):%- [_^ConvertTime^ ConvertTime][@(0.0.255) `&]_[* M
inMax]([_^Time^ Time]_[*@3 `_min], [_^Time^ Time]_[*@3 `_max])&]
[s2;%- Лимиты времени.&]
[s7; [%-*C@3 `_min]-|Нижний лимит.&]
[s7; [%-*C@3 `_max]-|Верхний лимит.&]
[s7; [*/ Возвратное значение]-|`*this.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:ConvertTime`:`:Min`(Upp`:`:Time`):%- [_^Upp`:`:ConvertTime^ ConvertTime][@(0.0.255) `&
]_[* Min]([_^Upp`:`:Time^ Time]_[*@3 `_min])&]
[s2; Устанавливает нижний лимит.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:ConvertTime`:`:Max`(Upp`:`:Time`):%- [_^Upp`:`:ConvertTime^ ConvertTime][@(0.0.255) `&
]_[* Max]([_^Upp`:`:Time^ Time]_[*@3 `_max])&]
[s2; Устанавливает верхний лимит.&]
[s3; &]
[s4;%- &]
[s5;:ConvertTime`:`:NotNull`(bool`):%- [_^ConvertTime^ ConvertTime][@(0.0.255) `&]_[* NotNu
ll]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; Запрещает пустые строки String.&]
[s7; [%-*C@3 b]-|true, чтобы запретить.&]
[s7; [*/ Возвратное значение]-|`*this.&]
[s3; &]
[s4;%- &]
[s5;:ConvertTime`:`:NoNotNull`(`):%- [_^ConvertTime^ ConvertTime][@(0.0.255) `&]_[* NoNotNu
ll]()&]
[s2; То же, что и [^topic`:`/`/Core`/src`/Convert`$en`-us`#ConvertTime`:`:NotNull`(bool`)^ N
otNull](false).&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:ConvertTime`:`:Seconds`(bool`):%- [_^Upp`:`:ConvertTime^ ConvertTime][@(0.0.255) `&
]_[* Seconds]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; Если true (что по дефолту), Format возвращает 
время с секундами.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:ConvertTime`:`:NoSeconds`(`):%- [_^Upp`:`:ConvertTime^ ConvertTime][@(0.0.255) `&
]_[* NoSeconds]()&]
[s2; То же, что и Seconds(fale)&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:ConvertTime`:`:IsSeconds`(`)const:%- [@(0.0.255) bool]_[* IsSeconds]()_[@(0.0.255) c
onst]&]
[s2; Возвращает  значение, установленное 
через Seconds.&]
[s3;%- &]
[s4;%- &]
[s5;:ConvertTime`:`:TimeAlways`(bool`):%- [_^ConvertTime^ ConvertTime][@(0.0.255) `&]_[* Ti
meAlways]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; Если активен, Format всегда показывает 
время, даже если значение является 
датой или полночь. Дефолт равен false.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:ConvertTime`:`:IsTimeAlways`(`)const:%- [@(0.0.255) bool]_[* IsTimeAlways]()_
[@(0.0.255) const]&]
[s2; Возвращает значение, установленное 
через TimeAlways.&]
[s3;%- &]
[s4;%- &]
[s5;:ConvertTime`:`:DayEnd`(bool`):%- [_^ConvertTime^ ConvertTime][@(0.0.255) `&]_[* DayEnd
]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; Если активен, когда Scan сталкивается 
с датой без времени, он устанавливает 
время в 23:59:59. Дефолт равен false.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:ConvertTime`:`:IsDayEnd`(`)const:%- [@(0.0.255) bool]_[* IsDayEnd]()_[@(0.0.255) c
onst]&]
[s2; Возвращает значение, установленное 
посредством DayEnd.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:ConvertTime`:`:Default`(Upp`:`:Time`):%- [_^Upp`:`:ConvertTime^ ConvertTime
][@(0.0.255) `&]_[* Default]([_^Upp`:`:Time^ Time]_[*@3 d])&]
[s2; Устанавливает дефолтное время, возвращаемо
е от Scan, когда ввод пуст.&]
[s3; &]
[s4;%- &]
[s5;:ConvertTime`:`:GetMin`(`)const:%- [_^Time^ Time]_[* GetMin]()_[@(0.0.255) const]&]
[s2; Возвращает минимально допустимое 
время. Это максимальное значение, 
установленное посредством Min, и дефолтный 
минимум (GetDefaultMin).&]
[s3; &]
[s4;%- &]
[s5;:ConvertTime`:`:GetMax`(`)const:%- [_^Time^ Time]_[* GetMax]()_[@(0.0.255) const]&]
[s2; Возвращает верхний предео допустимого 
времени. Это минимальное значение, 
установленное посредством Max, и дефолтный 
максимум (GetDefaultMax).&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:ConvertTime`:`:GetDefaultMin`(`):%- [@(0.0.255) static] 
[_^Upp`:`:Time^ Time]_[* GetDefaultMin]()&]
[s2; Возвращает Date`::GetDefaultMin преобразованным 
в Time.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:ConvertTime`:`:GetDefaultMax`(`):%- [@(0.0.255) static] 
[_^Upp`:`:Time^ Time]_[* GetDefaultMax]()&]
[s2; Возвращает Date`::GetDefaultMax преобразованным 
в Time.&]
[s3;%- &]
[s4;%- &]
[s5;:ConvertTime`:`:IsNotNull`(`)const:%- [@(0.0.255) bool]_[* IsNotNull]()_[@(0.0.255) con
st]&]
[s7; [*/ Возвратное значение]-|true означает, 
что Null`'ы запрещены.&]
[s0;%- &]
[ {{10000@(113.42.0) [s0; [*@7;4 Класс ConvertDouble]]}}&]
[s3; &]
[s1;:ConvertDouble`:`:class:%- [@(0.0.255)3 class][3 _][*3 ConvertDouble][3 _:_][@(0.0.255)3 pub
lic][3 _][*@3;3 Convert]&]
[s9; Преобразователь цифр с плавающей 
запятой.&]
[s0; &]
[s0;%- [%RU-RU/ Производный от][%RU-RU  ][^topic`:`/`/Core`/src`/Convert`_ru`-ru`#Convert`:`:class^ C
onvert]&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Детали Конструктора]]}}&]
[s3;%- &]
[s5;:ConvertDouble`:`:ConvertDouble`(double`,double`,bool`):%- [* ConvertDouble]([@(0.0.255) d
ouble]_[*@3 minval]_`=_DOUBLE`_NULL`_LIM, [@(0.0.255) double]_[*@3 maxval]_`=_`-DOUBLE`_
NULL`_LIM, [@(0.0.255) bool]_[*@3 notnull]_`=_[@(0.0.255) false])&]
[s2; Конструктор .&]
[s7; [%-*C@3 minval]-|Нижний лимит `- по дефолту 
лимит отсутствует&]
[s7; [%-*C@3 maxval]-|Верхний лимит `- по дефолту 
лимит отсутствует.&]
[s7; [%-*C@3 notnull]-|Если true, Null`'ы не допускаются.&]
[s3; &]
[s4;%- &]
[s5;:ConvertDouble`:`:`~ConvertDouble`(`):%- [@(0.0.255) `~][* ConvertDouble]()&]
[s2; Дефолтный деструктор.&]
[s3;%- &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Список Публичных Членов]]}}&]
[s3;%- &]
[s5;:ConvertDouble`:`:Образец`(const char`*`):%- [_^ConvertDouble^ ConvertDouble
][@(0.0.255) `&]_[* Образец]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 p])&]
[s2; Тэг форматирования, используемый 
для преобразования в текстовое представлени
е (включает стандартные теги форматирования
 вывода, описание приведено в функции 
Format). Дефолным тэгом является [@4 %.10g]. 
После установки нового образца, ConvertDouble 
форматирует простое число`-пример 
(Format(1.1)) и, если находит символ `',`' 
в итоговой строке, метод Filter форсирует 
применение  `',`' вместо `'.`' для десятичной 
точки.&]
[s7; [%-*C@3 p]-|Образец.&]
[s7; [*/ Возвратное значение]-|`*this.&]
[s3; &]
[s4;%- &]
[s5;:ConvertDouble`:`:MinMax`(double`,double`):%- [_^ConvertDouble^ ConvertDouble][@(0.0.255) `&
]_[* MinMax]([@(0.0.255) double]_[*@3 `_min], [@(0.0.255) double]_[*@3 `_max])&]
[s2; Устанавливает минимальное и максимальное 
допустимые числа.&]
[s7; [%-*C@3 `_min]-|Нижний лимит.&]
[s7; [%-*C@3 `_max]-|Верхний лимит.&]
[s7; [*/ Возвратное значение]-|`*this.&]
[s3; &]
[s4;%- &]
[s5;:ConvertDouble`:`:Min`(double`):%- [_^ConvertDouble^ ConvertDouble][@(0.0.255) `&]_[* M
in]([@(0.0.255) double]_[*@3 `_min])&]
[s2; Устанавливае минимально допустимое 
число.&]
[s7; [%-*C@3 `_min]-|Нижний лимит.&]
[s7; [*/ Возвратное значение]-|`*this.&]
[s3; &]
[s4;%- &]
[s5;:ConvertDouble`:`:Max`(double`):%- [_^ConvertDouble^ ConvertDouble][@(0.0.255) `&]_[* M
ax]([@(0.0.255) double]_[*@3 `_max])&]
[s2; Устанавливает максимально допустимое 
число.&]
[s7; [%-*C@3 `_max]-|Верхний лимит.&]
[s7; [*/ Возвратное значение]-|`*this.&]
[s3; &]
[s4;%- &]
[s5;:ConvertDouble`:`:NotNull`(bool`):%- [_^ConvertDouble^ ConvertDouble][@(0.0.255) `&]_
[* NotNull]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; Разрешает/запрещает Null`'ы. (Null`'ы представлен
ы пустым текстом).&]
[s7; [%-*C@3 b]-|true, чтобы запретить.&]
[s7; [*/ Возвратное значение]-|`*this.&]
[s3; &]
[s4;%- &]
[s5;:ConvertDouble`:`:NoNotNull`(`):%- [_^ConvertDouble^ ConvertDouble][@(0.0.255) `&]_[* N
oNotNull]()&]
[s2; Эквивалентно [^topic`:`/`/Core`/src`/Convert`$en`-us`#ConvertDouble`:`:NotNull`(bool`)^ N
otNull](false).&]
[s3; &]
[s4;%- &]
[s5;:ConvertDouble`:`:GetMin`(`)const:%- [@(0.0.255) double]_[* GetMin]()_[@(0.0.255) const
]&]
[s7; [*/ Возвратное значение]-|Нижний лимит.&]
[s3; &]
[s4;%- &]
[s5;:ConvertDouble`:`:GetMax`(`)const:%- [@(0.0.255) double]_[* GetMax]()_[@(0.0.255) const
]&]
[s7; [*/ Возвратное значение]-|Верхний лимит.&]
[s3; &]
[s4;%- &]
[s5;:ConvertDouble`:`:IsNotNull`(`)const:%- [@(0.0.255) bool]_[* IsNotNull]()_[@(0.0.255) c
onst]&]
[s7; [*/ Возвратное значение]-|true, если Null`'ы 
запрещены.&]
[s0;%- &]
[ {{10000@(113.42.0) [s0; [*@7;4 Класс ConvertInt]]}}&]
[s3; &]
[s1;:ConvertInt`:`:class:%- [@(0.0.255)3 class][3 _][*3 ConvertInt][3 _:_][@(0.0.255)3 public][3 _
][*@3;3 Convert]&]
[s9; Преобразователь целых чисел.&]
[s0; &]
[s0;%- [%RU-RU/ Производный от][%RU-RU  ][^topic`:`/`/Core`/src`/Convert`_ru`-ru`#Convert`:`:class^ C
onvert]&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Детали Конструктора]]}}&]
[s3;%- &]
[s5;:ConvertInt`:`:ConvertInt`(int`,int`,bool`):%- [* ConvertInt]([@(0.0.255) int]_[*@3 min
val]_`=_`-INT`_MAX, [@(0.0.255) int]_[*@3 maxval]_`=_INT`_MAX, [@(0.0.255) bool]_[*@3 not
null]_`=_[@(0.0.255) false])&]
[s2; Конструктор .&]
[s7; [%-*C@3 minval]-|Нижний лимит. По дефолту лимит 
отсутствует.&]
[s7; [%-*C@3 maxval]-|Верхний лимит.По дефолту 
лимит отсутствует.&]
[s7; [%-*C@3 notnull]-|Если true, Null`'ы запрещены.&]
[s3; &]
[s4;%- &]
[s5;:ConvertInt`:`:`~ConvertInt`(`):%- [@(0.0.255) `~][* ConvertInt]()&]
[s2; Дефолтный деструктор.&]
[s3;%- &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Список Публичных Членов]]}}&]
[s3;%- &]
[s5;:ConvertInt`:`:MinMax`(int`,int`):%- [_^ConvertInt^ ConvertInt][@(0.0.255) `&]_[* MinMa
x]([@(0.0.255) int]_[*@3 `_min], [@(0.0.255) int]_[*@3 `_max])&]
[s2; Устанавливает  минимальное и максимальное 
допустимые числа..&]
[s7; [%-*C@3 `_min]-|Нижний лимит.&]
[s7; [%-*C@3 `_max]-|Верхний лимит.&]
[s7; [*/ Возвратное значение]-|`*this.&]
[s3; &]
[s4;%- &]
[s5;:ConvertInt`:`:Min`(int`):%- [_^ConvertInt^ ConvertInt][@(0.0.255) `&]_[* Min]([@(0.0.255) i
nt]_[*@3 `_min])&]
[s2; Устанавливает минимальное допустимое 
число.&]
[s7; [%-*C@3 `_min]-|Нижний лимит.&]
[s7; [*/ Возвратное значение]-|`*this.&]
[s3; &]
[s4;%- &]
[s5;:ConvertInt`:`:Max`(int`):%- [_^ConvertInt^ ConvertInt][@(0.0.255) `&]_[* Max]([@(0.0.255) i
nt]_[*@3 `_max])&]
[s2; Устанавливает максимальное допустимое 
число.&]
[s7; [%-*C@3 `_max]-|Верхний лимит.&]
[s7; [*/ Возвратное значение]-|`*this.&]
[s3; &]
[s4;%- &]
[s5;:ConvertInt`:`:NotNull`(bool`):%- [_^ConvertInt^ ConvertInt][@(0.0.255) `&]_[* NotNull](
[@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; Разрешает/запрещает Null`'ы. (Null`'ы представлен
ы пустым текстом).&]
[s7; [%-*C@3 b]-|true, чтобы запретить.&]
[s7; [*/ Возвратное значение]-|`*this.&]
[s3; &]
[s4;%- &]
[s5;:ConvertInt`:`:NoNotNull`(`):%- [_^ConvertInt^ ConvertInt][@(0.0.255) `&]_[* NoNotNull](
)&]
[s2; Эквивалентно [^topic`:`/`/Core`/src`/Convert`$en`-us`#ConvertInt`:`:NotNull`(bool`)^ N
otNull](false).&]
[s7; [*/ Возвратное значение]-|`*this.&]
[s3; &]
[s4;%- &]
[s5;:ConvertInt`:`:GetMin`(`)const:%- [@(0.0.255) int]_[* GetMin]()_[@(0.0.255) const]&]
[s7; [*/ Возвратное значение]-|Нижний лимит.&]
[s3; &]
[s4;%- &]
[s5;:ConvertInt`:`:GetMax`(`)const:%- [@(0.0.255) int]_[* GetMax]()_[@(0.0.255) const]&]
[s7; [*/ Возвратное значение]-|Верхний лимит.&]
[s3; &]
[s4;%- &]
[s5;:ConvertInt`:`:IsNotNull`(`)const:%- [@(0.0.255) bool]_[* IsNotNull]()_[@(0.0.255) cons
t]&]
[s7; [*/ Возвратное значение]-|true, если null 
запрещён.&]
[s0;%- &]
[ {{10000@(113.42.0) [s0; [*@7;4 Класс ConvertString]]}}&]
[s3; &]
[s1;:ConvertString`:`:class:%- [@(0.0.255)3 class][3 _][*3 ConvertString][3 _:_][@(0.0.255)3 pub
lic][3 _][*@3;3 Convert]&]
[s9; String`'овый `"преобразователь`". Конечно 
же, String уже является текстовым представление
м себя, но реальное назначение этого 
класса `- ввести ограничения на значения 
String.&]
[s0; &]
[s0;%- [%RU-RU/ Производный от][%RU-RU  ][^topic`:`/`/Core`/src`/Convert`_ru`-ru`#Convert`:`:class^ C
onvert]&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Детали Конструктора]]}}&]
[s3; &]
[s5;:ConvertString`:`:ConvertString`(int`,bool`):%- [* ConvertString]([@(0.0.255) int]_[*@3 m
axlen]_`=_INT`_MAX, [@(0.0.255) bool]_[*@3 notnull]_`=_[@(0.0.255) false])&]
[s2; Конструктор.&]
[s7; [%-*C@3 maxlen]-|Максимальная длина `- дефолтно 
неограничена.&]
[s7; [%-*C@3 notnull]-|Если true, пустые строки недопускаютс
я.&]
[s3; &]
[s4;%- &]
[s5;:ConvertString`:`:`~ConvertString`(`):%- [@(0.0.255) `~][* ConvertString]()&]
[s2; Дефолтный деструктор.&]
[s3;%- &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Список Публичных Членов]]}}&]
[s3; &]
[s5;:ConvertString`:`:MaxLen`(int`):%- [_^ConvertString^ ConvertString][@(0.0.255) `&]_[* M
axLen]([@(0.0.255) int]_[*@3 `_maxlen])&]
[s2; Устанавливает максимально допустимую 
длину String.&]
[s7; [%-*C@3 `_maxlen]-|Длина.&]
[s7; [*/ Возвратное значение]-|`*this.&]
[s3; &]
[s4;%- &]
[s5;:ConvertString`:`:GetMaxLength`(`)const:%- [@(0.0.255) int]_[* GetMaxLength]()_[@(0.0.255) c
onst]&]
[s7; [*/ Возвратное значение]-|Максимально 
допустимая длина String.&]
[s3; &]
[s4;%- &]
[s5;:ConvertString`:`:NotNull`(bool`):%- [_^ConvertString^ ConvertString][@(0.0.255) `&]_
[* NotNull]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; Запрещает пустые String`'и.&]
[s7; [%-*C@3 b]-|true, чтобы запретить.&]
[s7; [*/ Возвратное значение]-|`*this.&]
[s3; &]
[s4;%- &]
[s5;:ConvertString`:`:NoNotNull`(`):%- [_^ConvertString^ ConvertString][@(0.0.255) `&]_[* N
oNotNull]()&]
[s2; То же, что и [^topic`:`/`/Core`/src`/Convert`$en`-us`#ConvertString`:`:NotNull`(bool`)^ N
otNull](false).&]
[s3; &]
[s4;%- &]
[s5;:ConvertString`:`:IsNotNull`(`)const:%- [@(0.0.255) bool]_[* IsNotNull]()_[@(0.0.255) c
onst]&]
[s7; [*/ Возвратное значение]-|true значит, 
что пустые String`'и не допускаются.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:ConvertString`:`:TrimLeft`(bool`):%- [_^Upp`:`:ConvertString^ ConvertString
][@(0.0.255) `&]_[* TrimLeft]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s5;:Upp`:`:ConvertString`:`:TrimRight`(bool`):%- [_^Upp`:`:ConvertString^ ConvertStrin
g][@(0.0.255) `&]_[* TrimRight]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s5;:Upp`:`:ConvertString`:`:TrimBoth`(bool`):%- [_^Upp`:`:ConvertString^ ConvertString
][@(0.0.255) `&]_[* TrimBoth]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; Пробельные символы слева/справа/с 
обеих сторон перед проверкой числа 
символов или null`'ности удаляются.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:ConvertString`:`:IsTrimLeft`(`)const:%- [@(0.0.255) bool]_[* IsTrimLeft]()_[@(0.0.255) c
onst]&]
[s5;:Upp`:`:ConvertString`:`:IsTrimRight`(`)const:%- [@(0.0.255) bool]_[* IsTrimRight]()_
[@(0.0.255) const]&]
[s2; Проверяет, активен ли TrimLeft или TrimRight 
(или оба).&]
[s0;%- &]
[ {{10000@(113.42.0) [s0; [*@7;4 Класс FormatConvert]]}}&]
[s3; &]
[s1;:FormatConvert`:`:class:%- [@(0.0.255)3 class][3 _][*3 FormatConvert][3 _:_][@(0.0.255)3 pub
lic][3 _][*@3;3 Convert]&]
[s9; Этот однонаправленный класс Convert 
(реализовано только направление Format) 
преобразует единичное Value или ValueArray, 
применяя функцию Format и заданный образец 
форматировщика.&]
[s9; &]
[s0;%- [%RU-RU/ Производный от][%RU-RU  ][^topic`:`/`/Core`/src`/Convert`_ru`-ru`#Convert`:`:class^ C
onvert]&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Список Публичных Членов]]}}&]
[s3;%- &]
[s5;:FormatConvert`:`:SetFormat`(const char`*`):%- [@(0.0.255) void]_[* SetFormat]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 fmt])&]
[s2; Устанавливает образец форматировки.&]
[s7; [%-*C@3 fmt]-|Образец.&]
[s0;%- &]
[ {{10000@(113.42.0) [s0; [*@7;4 Класс JoinConvert]]}}&]
[s3; &]
[s1;:JoinConvert`:`:class:%- [@(0.0.255)3 class][3 _][*3 JoinConvert][3 _:_][@(0.0.255)3 public][3 _
][*@3;3 Convert]&]
[s9; Этот однонаправленный класс Convert 
(реализовано только направление Format) 
преобразует ValueArray, используя набор 
других экземпляров Convert. Итоговое 
текстовое представление создаётся 
путёт объединения ряда заданных элементов 
`- некоторые из них статические тексты, 
другие ссылаются на элементы входного 
ValueArray, преобразованного с помощью 
указанного Convert.&]
[s0; &]
[s0;%- [%RU-RU/ Производный от][%RU-RU  ][^topic`:`/`/Core`/src`/Convert`_ru`-ru`#Convert`:`:class^ C
onvert]&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Список Публичных Членов]]}}&]
[s3;%- &]
[s5;:JoinConvert`:`:Add`(const char`*`):%- [_^JoinConvert^ JoinConvert][@(0.0.255) `&]_[* A
dd]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 text])&]
[s2; Добавляет статический текст в список 
элементов.&]
[s7; [%-*C@3 text]-|Текст.&]
[s7; [*/ Возвратное значение]-|`*this.&]
[s3; &]
[s4;%- &]
[s5;:JoinConvert`:`:Add`(int`,const Convert`&`):%- [_^JoinConvert^ JoinConvert][@(0.0.255) `&
]_[* Add]([@(0.0.255) int]_[*@3 pos], [@(0.0.255) const]_[_^Convert^ Convert][@(0.0.255) `&]_
[*@3 cv])&]
[s2; Добавляет элемент, ссылающееся на 
входное значение, которое будет преобразова
но посредством указанного Convert.&]
[s7; [%-*C@3 pos]-|Индекс элемента входного ValueArray.&]
[s7; [%-*C@3 cv]-|Convert.&]
[s7; [*/ Возвратное значение]-|`*this.&]
[s3; &]
[s4;%- &]
[s5;:JoinConvert`:`:Add`(int`):%- [_^JoinConvert^ JoinConvert][@(0.0.255) `&]_[* Add]([@(0.0.255) i
nt]_[*@3 pos])&]
[s2; Добавляет элемент, ссылающийся на 
входное значение, который будет преобразова
н посредством дефолтного Convert.&]
[s7; [%-*C@3 pos]-|Индекс элемента входного ValueArray.&]
[s7; [*/ Возвратное значение]-|`*this.&]
[s3; &]
[s4;%- &]
[s5;:JoinConvert`:`:Add`(const Convert`&`):%- [_^JoinConvert^ JoinConvert][@(0.0.255) `&]_
[* Add]([@(0.0.255) const]_[_^Convert^ Convert][@(0.0.255) `&]_[*@3 cv])&]
[s2; Добавляет элемент, ссылающийся на 
входное значение, который будет преобразова
н посредством указанного Convert. Индекс 
входного элемента является индекс 
предыдущего входного элемента плюс 
1.&]
[s7; [%-*C@3 cv]-|Convert.&]
[s7; [*/ Возвратное значение]-|`*this.&]
[s3; &]
[s4;%- &]
[s5;:JoinConvert`:`:Add`(`):%- [_^JoinConvert^ JoinConvert][@(0.0.255) `&]_[* Add]()&]
[s2; Добавляет элемент, ссылающийся на 
входное значение, который будет преобразова
н посредством дефолтного Convert. Индекс 
входного элемента является индекс 
предыдущего входного элемента плюс 
1.&]
[s7; [*/ Возвратное значение]-|`*this.&]
[s0;%- &]
[ {{10000@(113.42.0) [s0; [*@7;4 Стандартные `"Конверты`"]]}}&]
[s3; &]
[s9; Стандартные конверты `- это простые 
глобальные функции, возвращающие 
константную ссылку на единичную глобальную 
переменную, представляющую определённый 
класс Convert. В следующей таблице дан 
список имён этих функций и соответствующие 
конструкторы классов Convert, используемые 
для создания глобальных переменных:&]
[s3; &]
[s0; &]
[s5;:StdConvert`(`):%- [@(0.0.255) const]_[_^Convert^ Convert][@(0.0.255) `&]_[* StdConvert](
)&]
[s2; [C^topic`:`/`/Core`/src`/Convert`$en`-us`#Convert`:`:class^1 Convert]&]
[s3;%- &]
[s4;%- &]
[s5;:StdConvertInt`(`):%- [@(0.0.255) const]_[_^ConvertInt^ ConvertInt][@(0.0.255) `&]_[* Std
ConvertInt]()&]
[s2; [C^topic`:`/`/Core`/src`/Convert`$en`-us`#ConvertInt`:`:class^1 ConvertInt]&]
[s3;%- &]
[s4;%- &]
[s5;:StdConvertIntNotNull`(`):%- [@(0.0.255) const]_[_^ConvertInt^ ConvertInt][@(0.0.255) `&
]_[* StdConvertIntNotNull]()&]
[s2; [C^topic`:`/`/Core`/src`/Convert`$en`-us`#ConvertInt`:`:class^1 ConvertInt][C1 (`-IN
T`_MAX, INT`_MAX, true)]&]
[s3;%- &]
[s4;%- &]
[s5;:StdConvertDouble`(`):%- [@(0.0.255) const]_[_^ConvertDouble^ ConvertDouble][@(0.0.255) `&
]_[* StdConvertDouble]()&]
[s2; [C^topic`:`/`/Core`/src`/Convert`$en`-us`#ConvertDouble`:`:class^1 ConvertDouble]&]
[s3;%- &]
[s4;%- &]
[s5;:StdConvertDoubleNotNull`(`):%- [@(0.0.255) const]_[_^ConvertDouble^ ConvertDouble][@(0.0.255) `&
]_[* StdConvertDoubleNotNull]()&]
[s2; [C^topic`:`/`/Core`/src`/Convert`$en`-us`#ConvertDouble`:`:class^1 ConvertDouble][C1 (
`-DOUBLE`_NULL`_LIM, DOUBLE`_NULL`_LIM, true)]&]
[s3;%- &]
[s4;%- &]
[s5;:StdConvertDate`(`):%- [@(0.0.255) const]_[_^ConvertDate^ ConvertDate][@(0.0.255) `&]_[* S
tdConvertDate]()&]
[s2; [C^topic`:`/`/Core`/src`/Convert`$en`-us`#ConvertDate`:`:class^1 ConvertDate]&]
[s3;%- &]
[s4;%- &]
[s5;:StdConvertDateNotNull`(`):%- [@(0.0.255) const]_[_^ConvertDate^ ConvertDate][@(0.0.255) `&
]_[* StdConvertDateNotNull]()&]
[s2; [C^topic`:`/`/Core`/src`/Convert`$en`-us`#ConvertDate`:`:class^1 ConvertDate][C1 (Da
te(0, 0, 0), Date(3000, 12, 31), true)]&]
[s3;%- &]
[s4;%- &]
[s5;:StdConvertTime`(`):%- [@(0.0.255) const]_[_^ConvertTime^ ConvertTime][@(0.0.255) `&]_[* S
tdConvertTime]()&]
[s2; [C^topic`:`/`/Core`/src`/Convert`$en`-us`#ConvertTime`:`:class^1 ConvertTime]&]
[s3;%- &]
[s4;%- &]
[s5;:StdConvertTimeNotNull`(`):%- [@(0.0.255) const]_[_^ConvertTime^ ConvertTime][@(0.0.255) `&
]_[* StdConvertTimeNotNull]()&]
[s2; [C^topic`:`/`/Core`/src`/Convert`$en`-us`#ConvertTime`:`:class^1 ConvertTime][C1 (Nu
ll, Null, true)]&]
[s3;%- &]
[s4;%- &]
[s5;:StdConvertString`(`):%- [@(0.0.255) const]_[_^ConvertString^ ConvertString][@(0.0.255) `&
]_[* StdConvertString]()&]
[s2; [C^topic`:`/`/Core`/src`/Convert`$en`-us`#ConvertString`:`:class^1 ConvertString]&]
[s3;%- &]
[s4;%- &]
[s5;:StdConvertStringNotNull`(`):%- [@(0.0.255) const]_[_^ConvertString^ ConvertString][@(0.0.255) `&
]_[* StdConvertStringNotNull]()&]
[s2; [C^topic`:`/`/Core`/src`/Convert`$en`-us`#ConvertString`:`:class^1 ConvertString][C1 (
INT`_MAX, true)]&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:LambdaConvert`(F`,S`,R`):%- [@(0.0.255) template]_<[@(0.0.255) typename]_[*@4 F
], [@(0.0.255) typename]_[*@4 S], [@(0.0.255) class]_[*@4 R]>&]
[s5;:Upp`:`:LambdaConvert`(F`,S`,R`):%- [@(0.0.255) const]_[@(0.0.255) auto`&]_[* LambdaCon
vert]([*@4 F]_[*@3 format], [*@4 S]_[*@3 scan], [*@4 R]_[*@3 filter])&]
[s5;b42;:Upp`:`:LambdaConvert`(F`,S`):%- [@(0.0.255) template]_<[@(0.0.255) typename]_[*@4 F
], [@(0.0.255) typename]_[*@4 S]>&]
[s5;:Upp`:`:LambdaConvert`(F`,S`):%- [@(0.0.255) const]_[@(0.0.255) auto`&]_[* LambdaConver
t]([*@4 F]_[*@3 format], [*@4 S]_[*@3 scan])&]
[s5;b42;:Upp`:`:LambdaConvert`(F`):%- [@(0.0.255) template]_<[@(0.0.255) typename]_[*@4 F]>
&]
[s5;:Upp`:`:LambdaConvert`(F`):%- [@(0.0.255) const]_[@(0.0.255) auto`&]_[* LambdaConvert](
[*@4 F]_[*@3 format])&]
[s2; Возвращает ссылку на статический 
экземпляр Convert, у которого определены 
Format/Scan/Filter как лямбда`-выражения.&]
[s0; ]]