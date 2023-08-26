topic "Класс StatusBar и связанные с ним виджеты";
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
[s0; [*+150 StatusBar и связанные классы/процедуры]&]
[s3;%- &]
[ {{10000@3 [s0; [*@(229)4 Класс InfoCtrl]]}}&]
[s3; &]
[s1;:InfoCtrl`:`:class:%- [@(0.0.255)3 class][3 _][*3 InfoCtrl][3 _:_][@(0.0.255)3 public][3 _][*@3;3 F
rameLR][@(0.0.255)3 <][_^Ctrl^3 Ctrl][@(0.0.255)3 >][3 _]&]
[s0; &]
[s2; 
@@image:643&143
(A2cAFwAAAOn/AAAAAHic7dXNCYAwDAVgJ3YPR3ADD+I8riB0hwoeBJO2pjZtI7zwDhJKfz6pumN3CNIq6zIj6XjvOVr3F2c8QAMa0MwGaDXQhqtIhw8gzcLwOSUdI2h8S3znicGfxcizpGMELbaZ2mixteSdvmjBK9AGLXj1LIi9oklwYrZadNbEVNCqHuGe046YIpriEdKf/e5iaTSX8+vX3RVZZXhWsGMHDQEa0ID2r3A0jxIUuEpqnDYkNye2qVgt)
&]
[s0; &]
[s9; InfoCtrl `- простой статический контрол, 
представляющий собой информационное 
поле. Предназначен для использования 
в качестве Фрейма StatusBar `- в БрусСтатуса 
можно добавить несколько InfoCtrl`'ов 
в качестве Фреймов, заполняя его слева 
или справа.&]
[s9; InfoCtrl `- способен отображать несколько 
секций определённой ширины, определённых 
либо как Значение или как PaintRect. Он 
также может отображать временный 
контент некоторое количество времени.&]
[s0; &]
[s0;%- [%RU-RU/ Производный от][%RU-RU  ]FrameLR<Ctrl>&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Детали Конструктора]]}}&]
[s3;%- &]
[s5;:InfoCtrl`:`:InfoCtrl`(`):%- [* InfoCtrl]()&]
[s2; Конструктор. Устанавливает ThinInsetFrame 
как Frame 0, и `"Готов`" как дефолтный текст.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Список Публичных Методов]]}}&]
[s3;%- &]
[s5;:InfoCtrl`:`:Set`(int`,const PaintRect`&`,int`):%- [@(0.0.255) void]_[* Set]([@(0.0.255) i
nt]_[*@3 tab], [@(0.0.255) const]_[_^PaintRect^ PaintRect][@(0.0.255) `&]_[*@3 info], 
[@(0.0.255) int]_[*@3 width])&]
[s5;:InfoCtrl`:`:Set`(int`,const Value`&`,int`):%- [@(0.0.255) void]_[* Set]([@(0.0.255) in
t]_[*@3 tab], [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 info], 
[@(0.0.255) int]_[*@3 width])&]
[s2; Устанавливает (при необходимости 
создаёт) секцию InfoCtrl`'а.&]
[s7; [%-*C@3 tab]-|Индекс секции.&]
[s7; [%-*C@3 info]-|Контент секции. Контент Значения 
преобразуется в текст, посредством 
AsString, если это не Рисунок, который 
отображается в том виде, в каком есть.&]
[s7; [%-*C@3 width]-|Ширина секции в пикселях. 
Отрицательное значение указывает, 
что секция должна быть размещена 
в определённом количестве пикселей 
от правого бордюра InfoCtrl.&]
[s3; &]
[s4;%- &]
[s5;:InfoCtrl`:`:Set`(const PaintRect`&`):%- [@(0.0.255) void]_[* Set]([@(0.0.255) const]_[_^PaintRect^ P
aintRect][@(0.0.255) `&]_[*@3 info])&]
[s5;:InfoCtrl`:`:Set`(const Value`&`):%- [@(0.0.255) void]_[* Set]([@(0.0.255) const]_[_^Value^ V
alue][@(0.0.255) `&]_[*@3 info])&]
[s2; Устанавливает единичную секцию, 
преобразующую весь InfoCtrl.&]
[s7; [%-*C@3 info]-|Контент секции.&]
[s3; &]
[s4;%- &]
[s5;:InfoCtrl`:`:Temporary`(const PaintRect`&`,int`):%- [@(0.0.255) void]_[* Temporary]([@(0.0.255) c
onst]_[_^PaintRect^ PaintRect][@(0.0.255) `&]_[*@3 info], [@(0.0.255) int]_[*@3 timeoout]_`=
_[@3 2000])&]
[s5;:InfoCtrl`:`:Temporary`(const Value`&`,int`):%- [@(0.0.255) void]_[* Temporary]([@(0.0.255) c
onst]_[_^Value^ Value][@(0.0.255) `&]_[*@3 info], [@(0.0.255) int]_[*@3 timeout]_`=_[@3 2000])
&]
[s2; Устанавливает временный контент, 
отображаемый вместо определённых 
секций.&]
[s7; [%-*C@3 info]-|Контент.&]
[s7; [%-*C@3 timeoout]-|Промежуток времени временного 
контента.&]
[s3; &]
[s4;%- &]
[s5;:InfoCtrl`:`:EndTemporary`(`):%- [@(0.0.255) void]_[* EndTemporary]()&]
[s2; Немедленно прекращает показ временного 
контента.&]
[s3; &]
[s4;%- &]
[s5;:InfoCtrl`:`:GetTabCount`(`)const:%- [@(0.0.255) int]_[* GetTabCount]()_[@(0.0.255) con
st]&]
[s7; [*/ Возвратное значение]-|Число секций.&]
[s3; &]
[s4;%- &]
[s5;:InfoCtrl`:`:operator`=`(const String`&`):%- [@(0.0.255) void]_[* operator`=]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[*@3 s])&]
[s2; То же, что и Set(s).&]
[s7; [%-*C@3 s]-|Контент единичной секции, преобразующи
й весь InfoCtrl.&]
[s3; &]
[s4;%- &]
[s5;:InfoCtrl`:`:SetDefault`(const String`&`):%- [_^InfoCtrl^ InfoCtrl][@(0.0.255) `&]_[* S
etDefault]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 d])&]
[s2; Устанавливает дефолтный текст `-это 
значение используется вместо значений 
Null, переданных методу Set.&]
[s7; [%-*C@3 d]-|Дефолтный текст.&]
[s7; [*/ Возвратное значение]-|`*this для сцепления.&]
[s3; &]
[s4;%- &]
[s5;:InfoCtrl`:`:Left`(int`):%- [_^InfoCtrl^ InfoCtrl][@(0.0.255) `&]_[* Left]([@(0.0.255) in
t]_[*@3 w])&]
[s2; Настраивает InfoCtrl на размещение [* слева], 
при добавлении в качестве фрейма 
с заданной шириной.&]
[s7; [%-*C@3 w]-|Ширина InfoCtrl.&]
[s7; [*/ Возвратное значение]-|`*this для сцепления.&]
[s3; &]
[s4;%- &]
[s5;:InfoCtrl`:`:Right`(int`):%- [_^InfoCtrl^ InfoCtrl][@(0.0.255) `&]_[* Right]([@(0.0.255) i
nt]_[*@3 w])&]
[s2; Настраивает InfoCtrl на размещение [* справа], 
при добавлении в качестве фрейма 
с заданной шириной.&]
[s7; [%-*C@3 w]-|Ширина InfoCtrl.&]
[s7; [*/ Возвратное значение]-|`*this для сцепления.&]
[s3; &]
[s0; &]
[s0; &]
[s0;%- &]
[ {{10000@3 [s0; [*@(229)4 Класс StatusBar]]}}&]
[s3; &]
[s1;:StatusBar`:`:class:%- [@(0.0.255)3 class][3 _][*3 StatusBar][3 _:_][@(0.0.255)3 public][3 _][*@3;3 I
nfoCtrl]&]
[s0; &]
[s2; 
@@image:1268&131
(A8sAFQAAAOv/AAAAAHic7Zk9jsMgEEZ9/xtsGaXZYstEylG22CbpVuIOThHJIgzzEzAMWB96RWRbBr48YJSE/98AQBvWdXUfw1D8fH8BmY+cgWBUMPcxDA4EqwGCqUCwGiCYCgSrAYKpuAu2LIt7CMVAMJUawZaoFQ/gMIJlo0iu1MdlyZD20qhfC5WCcXMsDmcuNsGyUdDvvcWsqUvJ50b9GmkhmGUFJbufbwjFZI9IOiM5rnq4t8lj6INFsNvlfLuc6MPqsuUmONoqK4YKFueQLKvOgsmHdTesgl3zgslbE5e2YOZEdgUimNdRZd+pxtzBtse4Hcyya6lRzy6YWnF1E0x4+bCCZR82HpH2qOeyKzBFvj2Hvfh0gfeEE2yru4KtBgvvjgnnJnfFZfqVxIIJc6Q57DgGIVhhJN2QBLueXne5GmxfprMr4IdWA4Iz8a1sDbYvEOyQuP9VNDUQTCV2Jlt3Bb4GAxBMJRXsve7qWYPNyIpmaMYwH/c/75GiHb89Ac30xxw=)
&]
[s0; &]
[s9; StatusBar это Frame Ctrl, предназначенный 
для использования как информационный 
брус (`"строка состояния`"), расположенная 
внизу главного окна приложения.&]
[s9; StatusBar наследует от InfoCtrl и сохраняет 
всю функциональность базового класса. 
Добавлен `"size`-grip`" для изменения размера 
окна (если это поддерживается платформой) 
и изменено поведение как Фрейма, чтобы 
помещать его в нижней части родительского 
окна. Для добавления дополнительных 
полей в StatusBar, используются виджеты 
InfoCtrl.&]
[s0; &]
[s0;%- [%RU-RU/ Производный от][%RU-RU  ][^topic`:`/`/CtrlLib`/src`/StatusBar`_ru`-ru`#InfoCtrl`:`:class^ I
nfoCtrl]&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Детали Конструктора]]}}&]
[s3;%- &]
[s5;:StatusBar`:`:StatusBar`(`):%- [* StatusBar]()&]
[s2; Конструктор. Устанавливает высоту, 
основываясь на текущем системном 
дефолтном шрифте. Если отсутствует 
глобальный обратный вызов WhenHelpDefault() 
для этого Бруса, он определяется так, 
что Bar использует StatusBar, чтобы отображать 
информационные тексты Menu и ToolBar`'а.&]
[s3; &]
[s4;%- &]
[s5;:StatusBar`:`:`~StatusBar`(`):%- [@(0.0.255) `~][* StatusBar]()&]
[s2; Дефолтный деструктор .&]
[s3;%- &]
[s0; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Список Публичных Методов]]}}&]
[s3;%- &]
[s5;:StatusBar`:`:operator`=`(const String`&`):%- [@(0.0.255) virtual] 
[@(0.0.255) void]_[* operator`=]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 s])
&]
[s2; То же, что и Set(s).&]
[s7; [%-*C@3 s]-|Контент StatusBar`'а.&]
[s3; &]
[s4;%- &]
[s5;:StatusBar`:`:operator Callback1`<const String`&`>`(`):%- [* operator_Callback1<con
st_String`&>]()&]
[s7; [*/ Возвратное значение]-|Обратный вызов, 
который вызывает Set для StatusBar`'а.&]
[s3; &]
[s4;%- &]
[s5;:StatusBar`:`:Height`(int`):%- [_^StatusBar^ StatusBar][@(0.0.255) `&]_[* Height]([@(0.0.255) i
nt]_[*@3 `_cy])&]
[s2; Устанавливает высоту БрусаСостояния.&]
[s7; [%-*C@3 `_cy]-|Высота.&]
[s7; [*/ Возвратное значение]-|`*this для сцепления.&]
[s3; &]
[s4;%- &]
[s5;:StatusBar`:`:NoSizeGrip`(`):%- [_^StatusBar^ StatusBar][@(0.0.255) `&]_[* NoSizeGrip](
)&]
[s2; Не отображать `"size grip`".&]
[s7; [*/ Возвратное значение]-|`*this для сцепления.&]
[s3; &]
[s0; &]
[s0; &]
[s0;%- &]
[ {{10000@3 [s0; [*@(229)4 Класс ProgressInfo]]}}&]
[s3; &]
[s1;:ProgressInfo`:`:class:%- [@(0.0.255)3 class][3 _][*3 ProgressInfo]&]
[s9; Этот класс предоставляет простой 
инструмент для отображения информации 
о прогрессе в БрусеСостояния (или, 
в целом, в InfoCtrl). Он подключается к 
объекту InfoCtrl и заменяет его контент 
на индикатор прогресса.&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Детали Конструктора]]}}&]
[s3;%- &]
[s5;:ProgressInfo`:`:ProgressInfo`(`):%- [* ProgressInfo]()&]
[s2; Конструктор .&]
[s3; &]
[s4;%- &]
[s5;:ProgressInfo`:`:ProgressInfo`(InfoCtrl`&`):%- [* ProgressInfo]([_^InfoCtrl^ InfoCtrl
][@(0.0.255) `&]_[*@3 f])&]
[s2; Конструктор .&]
[s7; [%-*C@3 f]-|Целевой InfoCtrl.&]
[s3; &]
[s4;%- &]
[s5;:ProgressInfo`:`:`~ProgressInfo`(`):%- [@(0.0.255) `~][* ProgressInfo]()&]
[s2; Деструктор. Устанавливает Null целевому 
InfoCtrl.&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Список Публичных Методов]]}}&]
[s3;%- &]
[s5;:ProgressInfo`:`:Text`(const String`&`):%- [_^ProgressInfo^ ProgressInfo][@(0.0.255) `&
]_[* Text]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 s])&]
[s2; Устанавливает текст, отображаемый 
в качестве ярлыка индикатора прогресса.&]
[s7; [%-*C@3 s]-|Текст.&]
[s7; [*/ Возвратное значение]-|`*this для сцепления.&]
[s3; &]
[s4;%- &]
[s5;:ProgressInfo`:`:TextWidth`(int`):%- [_^ProgressInfo^ ProgressInfo][@(0.0.255) `&]_[* T
extWidth]([@(0.0.255) int]_[*@3 cx])&]
[s2; Устанавливает ширину текста. Ноль 
обозначает, что ширина должна быть 
определена, исходя из текста.&]
[s7; [%-*C@3 cx]-|Ширина в пикселях.&]
[s7; [*/ Возвратное значение]-|`*this для сцепления.&]
[s3; &]
[s4;%- &]
[s5;:ProgressInfo`:`:Width`(int`):%- [_^ProgressInfo^ ProgressInfo][@(0.0.255) `&]_[* Width
]([@(0.0.255) int]_[*@3 `_cx])&]
[s2; Устанавливает ширину индикатора 
прогресса. Дефолт равен 200.&]
[s7; [%-*C@3 `_cx]-|Ширина в пикселях.&]
[s7; [*/ Возвратное значение]-|`*this для сцепления.&]
[s3; &]
[s4;%- &]
[s5;:ProgressInfo`:`:Placement`(int`):%- [_^ProgressInfo^ ProgressInfo][@(0.0.255) `&]_[* P
lacement]([@(0.0.255) int]_[*@3 `_tabi])&]
[s2; Может использоваться для размещения 
индикатора прогресса в определённой 
секции InfoCtrl`'а.&]
[s7; [%-*C@3 `_tabi]-|Индекс секции. Ярлык прогресса 
помещается в [@3 `_tabi], индикатор прогресса 
`- в [@3 `_tabi] `+ 1.&]
[s7; [*/ Возвратное значение]-|`*this для сцепления.&]
[s3; &]
[s4;%- &]
[s5;:ProgressInfo`:`:Info`(InfoCtrl`&`):%- [_^ProgressInfo^ ProgressInfo][@(0.0.255) `&]_
[* Info]([_^InfoCtrl^ InfoCtrl][@(0.0.255) `&]_[*@3 `_info])&]
[s2; Определет целевой InfoCtrl.&]
[s7; [%-*C@3 `_info]-|InfoCtrl. Должен существовать 
в течение жизни ProgressInfo.&]
[s7; [*/ Возвратное значение]-|`*this для сцепления.&]
[s3; &]
[s4;%- &]
[s5;:ProgressInfo`:`:Total`(int`):%- [_^ProgressInfo^ ProgressInfo][@(0.0.255) `&]_[* Total
]([@(0.0.255) int]_[*@3 `_total])&]
[s2; Устанавливает общее число щагов 
прогресса.&]
[s7; [%-*C@3 `_total]-|Общее число.&]
[s7; [*/ Возвратное значение]-|`*this для сцепления.&]
[s3; &]
[s4;%- &]
[s5;:ProgressInfo`:`:Set`(int`,int`):%- [_^ProgressInfo^ ProgressInfo][@(0.0.255) `&]_[* Se
t]([@(0.0.255) int]_[*@3 `_pos], [@(0.0.255) int]_[*@3 `_total])&]
[s2; Устанавливает новую информацию о 
прогрессе.&]
[s7; [%-*C@3 `_pos]-|Текущий прогресс.&]
[s7; [%-*C@3 `_total]-|Общее число шагов прогресса.&]
[s7; [*/ Возвратное значение]-|`*this для сцепления.&]
[s3; &]
[s4;%- &]
[s5;:ProgressInfo`:`:Set`(int`):%- [@(0.0.255) void]_[* Set]([@(0.0.255) int]_[*@3 `_pos])&]
[s2; Устанавливает новую информацию о 
прогрессе.&]
[s7; [%-*C@3 `_pos]-|Текущий прогресс. Общее число 
шагов установлено методом Total.&]
[s3; &]
[s4;%- &]
[s5;:ProgressInfo`:`:Get`(`)const:%- [@(0.0.255) int]_[* Get]()_[@(0.0.255) const]&]
[s7; [*/ Возвратное значение]-|Текущий прогрессs.&]
[s3; &]
[s4;%- &]
[s5;:ProgressInfo`:`:operator`=`(int`):%- [@(0.0.255) void]_[* operator`=]([@(0.0.255) int]_
[*@3 p])&]
[s2; То же, что и Set(p).&]
[s3; &]
[s4;%- &]
[s5;:ProgressInfo`:`:operator`+`+`(`):%- [@(0.0.255) void]_[* operator`+`+]()&]
[s2; То же, что и Set(Get() `+ 1)&]
[s3; &]
[s4;%- &]
[s5;:ProgressInfo`:`:operator int`(`):%- [* operator_int]()&]
[s7; [*/ Возвратное значение]-|Get().&]
[s3; &]
[s0; ]]