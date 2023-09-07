topic "Классы, производные от EditField";
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
[s0; [*@3+150 Классы, производные от EditField]&]
[s9; Виджеты, редактирующие значения 
при текстовом представлении, основаны 
на EditField и соответствующем классе 
[^topic`:`/`/Core`/src`/Convert`_ru`-ru`#Convert`:`:class^ Convert]. 
Большинство из них определено посредством 
простых шаблонных классов EditValue, EditMinMax 
и EditMinMaxNotNull:&]
[s3; &]
[s0;%- &]
[ {{10000@3 [s0;%- [*@(229)4 Класс EditValue]]}}&]
[s3; &]
[s1;:noref:%- [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@4;3 DataType][3 , 
][@(0.0.255)3 class][3 _][*@3;3 Cv][@(0.0.255)3 >]&]
[s1;:EditValue`:`:class:%- [@(0.0.255) class]_[* EditValue]_:_[@(0.0.255) public]_[*@3 EditFie
ld], [@(0.0.255) public]_[*@3 Cv]&]
[s9; Этот класс предоставляет композицию 
EditField и специфичного класса [^topic`:`/`/Core`/src`/Convert`_ru`-ru`#Convert`:`:class^ C
onvert]. Преимущество создание производного 
от Convert состоит в том, что все методы 
установки Convert (типа установки значения 
min/max) доступны прямо, без использования 
какого`-либо кода`-склейки.&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Детали Конструктора]]}}&]
[s3;%- &]
[s5;:EditValue`:`:EditValue`(`):%- [* EditValue]()&]
[s2; Конструктор присваивает класс [^topic`:`/`/Core`/src`/Convert`_ru`-ru`#Convert`:`:class^ C
onvert] (фактически, `*this) в качестве конвертера 
EditField.&]
[s3; &]
[s3; &]
[s0;%- &]
[ {{10000@3 [s0;%- [*@(229)4 Класс EditMinMax]]}}&]
[s3; &]
[s1;:noref:%- [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@4;3 DataType][3 , 
][@(0.0.255)3 class][3 _][*@4;3 Cv][@(0.0.255)3 >]&]
[s1;:EditMinMax`:`:class:%- [@(0.0.255) class]_[* EditMinMax]_:_[@(0.0.255) public]_[*@3 EditV
alue][@(0.0.255) <][*@4 DataType], [*@4 Cv][@(0.0.255) >]_&]
[s9; Класс`-обёртка для конвертеров, предоставля
ющих свойства Min, Max и NotNull.&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Детали Конструктора]]}}&]
[s3;%- &]
[s5;:EditMinMax`:`:EditMinMax`(`):%- [* EditMinMax]()&]
[s2; Дефолтный конструктор.&]
[s3; &]
[s4;%- &]
[s5;:EditMinMax`:`:EditMinMax`(DataType`,DataType`):%- [* EditMinMax]([*@4 DataType]_[*@3 m
in], [*@4 DataType]_[*@3 max])&]
[s2; Этот контруктор устанавливает свойства 
[* Min] и [* Max].&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Список Публичных Методов]]}}&]
[s3;%- &]
[s5;:EditMinMax`:`:Min`(DataType`):%- [_^EditMinMax^ EditMinMax][@(0.0.255) `&]_[* Min]([*@4 D
ataType]_[*@3 min])&]
[s5;:EditMinMax`:`:Max`(DataType`):%- [_^EditMinMax^ EditMinMax][@(0.0.255) `&]_[* Max]([*@4 D
ataType]_[*@3 max])&]
[s5;:EditMinMax`:`:NotNull`(bool`):%- [_^EditMinMax^ EditMinMax][@(0.0.255) `&]_[* NotNull](
[@(0.0.255) bool]_[*@3 nn]_`=_[@(0.0.255) true])&]
[s2; Устанавливает свойства [* Min], [* Max] и 
[* NotNull]. Фактически, единственным назначением 
этих методов является изменение возвратного
 значения на корректно типированный 
`*this.&]
[s3; &]
[s0; &]
[s0; &]
[s0;%- &]
[ {{10000@3 [s0;%- [*@(229)4 Класс EditMinMaxNotNull]]}}&]
[s3; &]
[s1;:noref:%- [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@4;3 DataType][3 , 
][@(0.0.255)3 class][3 _][*@4;3 Cv][@(0.0.255)3 >]&]
[s1;:EditMinMaxNotNull`:`:class:%- [@(0.0.255) class]_[* EditMinMaxNotNull]_:_[@(0.0.255) pu
blic]_[*@3 EditValue][@(0.0.255) <][*@4 DataType], [*@4 Cv][@(0.0.255) >]_&]
[s9; Подобен шаблонному классу EditMinMax, 
но конструирует свойство [* NotNull].&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Детали Конструктора]]}}&]
[s3;%- &]
[s5;:EditMinMaxNotNull`:`:EditMinMaxNotNull`(`):%- [* EditMinMaxNotNull]()&]
[s2; Дефолтный конструктор.&]
[s3; &]
[s4;%- &]
[s5;:EditMinMaxNotNull`:`:EditMinMaxNotNull`(DataType`,DataType`):%- [* EditMinMaxNotNu
ll]([*@4 DataType]_[*@3 min], [*@4 DataType]_[*@3 max])&]
[s2; Этот контруктор устанавливает свойства 
[* Min] и [* Max].&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Список Публичных Методов]]}}&]
[s3;%- &]
[s5;:EditMinMaxNotNull`:`:Min`(DataType`):%- [_^EditMinMaxNotNull^ EditMinMaxNotNull][@(0.0.255) `&
]_[* Min]([*@4 DataType]_[*@3 min])&]
[s5;:EditMinMaxNotNull`:`:Max`(DataType`):%- [_^EditMinMaxNotNull^ EditMinMaxNotNull][@(0.0.255) `&
]_[* Max]([*@4 DataType]_[*@3 max])&]
[s5;:EditMinMaxNotNull`:`:NotNull`(bool`):%- [_^EditMinMaxNotNull^ EditMinMaxNotNull][@(0.0.255) `&
]_[* NotNull]([@(0.0.255) bool]_[*@3 nn]_`=_[@(0.0.255) true])&]
[s2; Устанавливает свойства [* Min], [* Max] и 
[* NotNull]. Фактически, единственным назначением 
этих методов является изменение возвратного
 значения на корректно типированный 
`*this.&]
[s3; &]
[s0; &]
[s0; &]
[s0; &]
[s0; Основан на EditMinMax и EditMinMaxNotNull, базовые 
редакторы значений определены как 
typedefs:&]
[s3; &]
[s5;:EditDate`:`:typedef:%- [@(0.0.255) typedef]_[_^EditMinMax^ EditMinMax][@(0.0.255) <][_^Date^ D
ate], [_^ConvertDate^ ConvertDate][@(0.0.255) >]_[* EditDate]&]
[s2; Поле редактиррования для значений 
дат.&]
[s3;%- &]
[s4;%- &]
[s5;:EditDateNotNull`:`:typedef:%- [@(0.0.255) typedef]_[_^EditMinMaxNotNull^ EditMinMaxN
otNull][@(0.0.255) <][_^Date^ Date], [_^ConvertDate^ ConvertDate][@(0.0.255) >]_[* EditDate
NotNull]&]
[s2; Поле редактирования для значений 
дат не null.&]
[s3;%- &]
[s4;%- &]
[s5;:EditDouble`:`:typedef:%- [@(0.0.255) typedef]_[_^EditMinMax^ EditMinMax][@(0.0.255) <d
ouble], [_^ConvertDouble^ ConvertDouble][@(0.0.255) >]_[* EditDouble]&]
[s2; Поле редактирования для значений 
double.&]
[s3;%- &]
[s4;%- &]
[s5;:EditDoubleNotNull`:`:typedef:%- [@(0.0.255) typedef]_[_^EditMinMaxNotNull^ EditMinMa
xNotNull][@(0.0.255) <double], [_^ConvertDouble^ ConvertDouble][@(0.0.255) >]_[* EditDoub
leNotNull]&]
[s2; Поле редактирования для значений 
double не null.&]
[s3;%- &]
[s4;%- &]
[s5;:EditInt`:`:typedef:%- [@(0.0.255) typedef]_[_^EditMinMax^ EditMinMax][@(0.0.255) <int],
 [_^ConvertInt^ ConvertInt][@(0.0.255) >]_[* EditInt]&]
[s2; Поле редактирования для значений 
integer (целое).&]
[s3;%- &]
[s4;%- &]
[s5;:EditIntNotNull`:`:typedef:%- [@(0.0.255) typedef]_[_^EditMinMaxNotNull^ EditMinMaxNo
tNull][@(0.0.255) <int], [_^ConvertInt^ ConvertInt][@(0.0.255) >]_[* EditIntNotNull]&]
[s2; Поле редактирования для значений 
integer не null.&]
[s3;%- &]
[s4;%- &]
[s5;:EditInt64`:`:typedef:%- [@(0.0.255) typedef]_[_^EditMinMax^ EditMinMax][@(0.0.255) <][_^int64^ i
nt64], [_^ConvertInt64^ ConvertInt64][@(0.0.255) >]_[* EditInt64]&]
[s2; Поле редактирования для значений 
64`-битный integer (целое).&]
[s3;%- &]
[s4;%- &]
[s5;:EditTime`:`:typedef:%- [@(0.0.255) typedef]_[_^EditMinMax^ EditMinMax][@(0.0.255) <][_^Time^ T
ime], [_^ConvertTime^ ConvertTime][@(0.0.255) >]_[* EditTime]&]
[s2; Поле редактирования для значений 
time (время).&]
[s3;%- &]
[s4;%- &]
[s5;:EditTimeNotNull`:`:typedef:%- [@(0.0.255) typedef]_[_^EditMinMaxNotNull^ EditMinMaxN
otNull][@(0.0.255) <][_^Time^ Time], [_^ConvertTime^ ConvertTime][@(0.0.255) >]_[* EditTime
NotNull]&]
[s2; Поле редактирования для значений 
не null time.&]
[s3;%- &]
[s0;%- &]
[s0; &]
[s0; &]
[s0; Некоторые редакторы значений основаны 
на EditMinMax и EditMinMaxNotNull:&]
[s3; &]
[s0;%- &]
[ {{10000@3 [s0;%- [*@(229)4 Класс EditString]]}}&]
[s3; &]
[s1;:EditString`:`:class:%- [@(0.0.255)3 class][3 _][*3 EditString][3 _:_][@(0.0.255)3 public][3 _
][*@3;3 EditValue][@(0.0.255)3 <][_^WString^3 WString][3 , ][_^ConvertString^3 ConvertStrin
g][@(0.0.255)3 >][3 _]&]
[s9; EditString не основан на EditMinMax, как как 
всемто минимальных и максимальных 
значений, он реализует констрейнт 
`"максимальное число символов`".&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Детали Конструктора]]}}&]
[s3;%- &]
[s5;:EditString`:`:EditString`(`):%- [* EditString]()&]
[s2; Дефолтный конструктор.&]
[s3; &]
[s4;%- &]
[s5;:EditString`:`:EditString`(int`):%- [* EditString]([@(0.0.255) int]_[*@3 maxlen])&]
[s2; Этот конструктор устанавливает [%-*@3 maxlen] 
как минимальную длину содержимого 
текста.&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Список Публичных Методов]]}}&]
[s3;%- &]
[s5;:EditString`:`:MaxLen`(int`):%- [_^EditString^ EditString][@(0.0.255) `&]_[* MaxLen]([@(0.0.255) i
nt]_[*@3 maxlen])&]
[s2; Устанавливает [%-*@3 maxlen] как максимальную 
длину содержимого текста.[%-*@3 maxlen] Возвращает 
`*this.&]
[s3; &]
[s4;%- &]
[s5;:EditString`:`:NotNull`(bool`):%- [_^EditString^ EditString][@(0.0.255) `&]_[* NotNull](
[@(0.0.255) bool]_[*@3 nn]_`=_[@(0.0.255) true])&]
[s2;%- [%RU-RU Устанавливает свойство ][* NotNull].&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:EditString`:`:TrimLeft`(bool`):%- [_^Upp`:`:EditString^ EditString][@(0.0.255) `&
]_[* TrimLeft]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s5;:Upp`:`:EditString`:`:TrimRight`(bool`):%- [_^Upp`:`:EditString^ EditString][@(0.0.255) `&
]_[* TrimRight]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s5;:Upp`:`:EditString`:`:TrimBoth`(bool`):%- [_^Upp`:`:EditString^ EditString][@(0.0.255) `&
]_[* TrimBoth]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; Пробелы слева/справа/с обеих сторон 
удаляются.&]
[s0; &]
[s0; &]
[s0;%- &]
[ {{10000@3 [s0;%- [*@(229)4 Класс EditStringNotNull]]}}&]
[s3; &]
[s1;:EditStringNotNull`:`:class:%- [@(0.0.255)3 class][3 _][*3 EditStringNotNull][3 _:_][@(0.0.255)3 p
ublic][3 _][*@3;3 EditString]&]
[s9; Этот класс устанавливает свойство 
NotNull в конструкторах.&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Детали Конструктора]]}}&]
[s3;%- &]
[s5;:EditStringNotNull`:`:EditStringNotNull`(`):%- [* EditStringNotNull]()&]
[s2; Дефолтный конструктор.&]
[s3; &]
[s4;%- &]
[s5;:EditStringNotNull`:`:EditStringNotNull`(int`):%- [* EditStringNotNull]([@(0.0.255) i
nt]_[*@3 maxlen])&]
[s2; Этот конструктор устанавливает [%-*@3 maxlen] 
как максимальную длину содержимого 
текста.&]
[s3; &]
[s0; &]
[s0; &]
[s0;%- &]
[ {{10000@3 [s0;%- [*@(229)4 Класс EditIntSpin]]}}&]
[s3; &]
[s1;:EditIntSpin`:`:class:%- [@(0.0.255)3 class][3 _][*3 EditIntSpin][3 _:_][@(0.0.255)3 public][3 _
][*@3;3 EditInt]&]
[s9; Это редактор добавляет спин`-кнопки 
классу EditInt.&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Детали Конструктора]]}}&]
[s3;%- &]
[s5;:EditIntSpin`:`:EditIntSpin`(`):%- [* EditIntSpin]()&]
[s2; Дефолтный конструктор. Спин`-кнопки 
включены.&]
[s3; &]
[s4;%- &]
[s5;:EditIntSpin`:`:EditIntSpin`(int`,int`):%- [* EditIntSpin]([@(0.0.255) int]_[*@3 min], 
[@(0.0.255) int]_[*@3 max])&]
[s2; Этот конструктор устанавливает значения 
[%-*@3 min] и [%-*@3 max]. Спин`-кнопки включены.&]
[s3; &]
[s4;%- &]
[s5;:EditIntSpin`:`:`~EditIntSpin`(`):%- [@(0.0.255) `~][* EditIntSpin]()&]
[s2; Дефолтный деструктор .&]
[s3;%- &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Список Публичных Методов]]}}&]
[s3;%- &]
[s5;:EditIntSpin`:`:ShowSpin`(bool`):%- [_^EditIntSpin^ EditIntSpin][@(0.0.255) `&]_[* Show
Spin]([@(0.0.255) bool]_[*@3 s]_`=_[@(0.0.255) true])&]
[s2; Показывает/скрывает спин`-кнопки.&]
[s3; &]
[s4;%- &]
[s5;:EditIntSpin`:`:SetInc`(int`):%- [_^EditIntSpin^ EditIntSpin][@(0.0.255) `&]_[* SetInc](
[@(0.0.255) int]_[*@3 `_inc])&]
[s2; Устнанавливает фактор инкрементации 
на [%-*@3 `_inc].&]
[s3; &]
[s4;%- &]
[s5;:EditIntSpin`:`:GetInc`(`)const:%- [@(0.0.255) int]_[* GetInc]()_[@(0.0.255) const]&]
[s2; Возвращает фактор инкрементации.&]
[s3;%- &]
[s0; &]
[s0; &]
[s0;%- &]
[ {{10000@3 [s0;%- [*@(229)4 Класс EditDoubleSpin]]}}&]
[s3; &]
[s1;:EditDoubleSpin`:`:class:%- [@(0.0.255)3 class][3 _][*3 EditDoubleSpin][3 _:_][@(0.0.255)3 p
ublic][3 _][*@3;3 EditDouble]&]
[s9; Этот редактор добавляет спин`-кнопки 
классу EditDouble.&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Детали Конструктора]]}}&]
[s3;%- &]
[s5;:EditDoubleSpin`:`:EditDoubleSpin`(double`):%- [* EditDoubleSpin]([@(0.0.255) double]_
[*@3 inc]_`=_[@3 0][@(0.0.255) .][@3 1])&]
[s2; Устанавливает инкремент спина в 
[%-*@3 inc]. Спин`-кнопки включены.&]
[s3; &]
[s4;%- &]
[s5;:EditDoubleSpin`:`:EditDoubleSpin`(double`,double`,double`):%- [* EditDoubleSpin]([@(0.0.255) d
ouble]_[*@3 min], [@(0.0.255) double]_[*@3 max], [@(0.0.255) double]_[*@3 inc]_`=_[@3 0][@(0.0.255) .
][@3 1])&]
[s2; Этот конструктор устанавливает значения 
[%-*@3 min], [%-*@3 max] и [%-*@3 inc]. Спин`-кнопки включены.&]
[s3; &]
[s4;%- &]
[s5;:EditDoubleSpin`:`:`~EditDoubleSpin`(`):%- [@(0.0.255) `~][* EditDoubleSpin]()&]
[s2; Дефолтный деструктор .&]
[s3;%- &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Список Публичных Методов]]}}&]
[s3;%- &]
[s5;:EditDoubleSpin`:`:SetInc`(double`):%- [_^EditDoubleSpin^ EditDoubleSpin][@(0.0.255) `&
]_[* SetInc]([@(0.0.255) double]_[*@3 `_inc]_`=_[@3 0][@(0.0.255) .][@3 1])&]
[s2; Устанавливает инкремент спина в 
[%-*@3 `_inc].&]
[s3; &]
[s4;%- &]
[s5;:EditDoubleSpin`:`:GetInc`(`)const:%- [@(0.0.255) double]_[* GetInc]()_[@(0.0.255) cons
t]&]
[s2; Возвращает текущий инкремент спина.&]
[s3; &]
[s4;%- &]
[s5;:EditDoubleSpin`:`:ShowSpin`(bool`):%- [_^EditDoubleSpin^ EditDoubleSpin][@(0.0.255) `&
]_[* ShowSpin]([@(0.0.255) bool]_[*@3 s]_`=_[@(0.0.255) true])&]
[s2; Показывает/скрывает спин`-кнопки.&]
[s3; &]
[s0; &]
[s0; &]
[s0;%- &]
[ {{10000@3 [s0;%- [*@(229)4 Класс EditDoubleNotNullSpin]]}}&]
[s3; &]
[s1;:EditDoubleNotNullSpin`:`:class:%- [@(0.0.255)3 class][3 _][*3 EditDoubleNotNullSpin][3 _
:_][@(0.0.255)3 public][3 _][*@3;3 EditDoubleSpin]&]
[s9; Этот редактор добавляет спин`-кнопки 
классу EditDoubleNotNull.&]
[s3;%- &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Детали Конструктора]]}}&]
[s3; &]
[s5;:EditDoubleNotNullSpin`:`:EditDoubleNotNullSpin`(double`):%- [* EditDoubleNotNullSp
in]([@(0.0.255) double]_[*@3 inc]_`=_[@3 0][@(0.0.255) .][@3 1])&]
[s2; Устанавливает инкремент спина в 
[%-*@3 inc]. Спин`-кнопки включены.&]
[s3; &]
[s4;%- &]
[s5;:EditDoubleNotNullSpin`:`:EditDoubleNotNullSpin`(double`,double`,double`):%- [* Edi
tDoubleNotNullSpin]([@(0.0.255) double]_[*@3 min], [@(0.0.255) double]_[*@3 max], 
[@(0.0.255) double]_[*@3 inc]_`=_[@3 0][@(0.0.255) .][@3 1])&]
[s2; Этот конструктор устанавливает значения 
[%-*@3 min], [%-*@3 max] и [%-*@3 inc]. Спин`-кнопки включены.&]
[s3; &]
[s0; ]]