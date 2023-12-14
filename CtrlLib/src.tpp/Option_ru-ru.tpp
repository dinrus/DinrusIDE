topic "Класс Option (Опция)";
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;ph2 $$9,9#13035079074754324216151401829390:normal]
[l321;C@5+75 $$10,10#20902679421464641399138805415013:code]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_}%RU-RU 
[ {{10000@3 [s0; [*@(229)4 Класс Option (Опция)]]}}&]
[s3;%- &]
[s1;:Option`:`:class:%- [@(0.0.255)3 class][3 _][*3 Option][3 _:_][@(0.0.255)3 public][3 _][*@3;3 Pu
sher]&]
[s0;%- &]
[s2; 
@@image:712&131
(A3IAFQAAAOv/AAAAAHic7ZhtioNADIa9U++059hTLsMigiwiYhFZytKfDQkVm8xk4li7FBJeZD7UZB6T/PA8hLPL5XK5DtDp9GHRv8f5XgJiX2ghhG+0pmnatv252zAMTvWJVIEnTKNUq0c7Ljx6+fr6AklHW10DsYAGAOu6Xidq9VmlqJb52nmWl1Hdf1IgRilKSJfCB6REdRxHp1pAFXgCPWDIkIIgUed5tlNlPUGOoyvyZrbCOoDSeeQ7ZbQpv8ydJWaFKqQoUSWwa6RbqbIt5XT6iuSgPBidWrykgrQ/q1ClDrDAJEHhA89ftIJctVBN3ZylKlMo5SWKLhukQjXlWlKVhT+iAc8LmpGqJQ2iMSvfKEo1dRYjVd2LTlUhKal2Xdf3PdQ7Q/qHdjTVrblqD8AyLqOazdUejbroUviE9Iq2vwPIrfVKllKqiqMhyS05ZWEog2zMWaqAdJomO9Uy2evofXXcfwCWtMav7HK5XC7XohsrJCxc)
 
@@image:1543&475
(A/cATAAAAAAAAAAAAHic7dpBbptAAIZR36mSz8NtOIzv4l3XXXWfG7Q2xYChhDBJBP55TyyIzeBJ9BkNKG9vAAAAAAAA0PvxMVtPE8rdAv6zROS8tNWRX6pTq7psN+2xa30+nevr1tNgl9ZFfm/p0fa1rjeu/PZ9EzbL1kW+r6z2NRt2a+Vy5b5YGYd1v7yPVjBNfpf29duL3SH92H7UZNmzdMLmJP2yqTloEPzc8Loaz4EgP3/9Hm3dW+tvPNuGBv089vuFcVPgZHdw7KDJS/Vc3dIJn17shnb788O7T37+Wk3/OLattk8WPvdu4dOVpvR7K8NVelNQ88N/8xvsDy6242v58gnf/ZR182FHijtfHFj8CLGtqTzymYczyyfsjhB5mrLOF4esvPGsh6V1a+PB8mCyGplfVMykNnfCx/ELn7JqPuzO9pGPb/ca/drj/bCf94crltFFfeaEVXUeHd7OZvbGc3E+7M4OIt+KMo/imyL/+P+unCZKfo0SIj+K74j8RYj8KA4cOUchcuKJnHgiJ57IiSdy4omceCInnsiJJ3LiiZx4IieeyIkncuKJnHgiJ57IiSdy4omceCInnsiJJ3LiiZx4IieeyIkncuKJnHgiJ57IiSdy4omceCInnsiJJ3LiiZx4IieeyIkncuKJnHgiJ57IiSdy4pVFrnNeRXGuOuclfDLUf8Nttp1vX/iVAQAAAACASH8BOEK7+g==)
&]
[s0; &]
[s9; Виджет, предоставляющий выбор из 
2`-х или, альтернативно, 3`-х состояний 
(true, false, Null). Значение Опции равно либо 
0, либо 1, либо Null. При его установке, 
string `"1`" интерпретируется как состояние 
true, прочие не`-Null строки `- как false. В 
зависимости от того, находится ли 
Опция в режиме NotNull, Null интерпретируется 
как false или как Null состояние. 0 всегда 
интерпретируется как false, число Null 
либо как Null (в режиме NotNull), либо как 
false (в режиме NoNotNull), другие цифры как 
true. Контрол также известен под названием 
[^https`:`/`/en`.wikipedia`.org`/wiki`/Checkbox^ checkbox].&]
[s9;%- [%RU-RU/ Производный от ][/^topic`:`/`/CtrlLib`/src`/Pusher`_ru`-ru`#Pusher`:`:class^ P
usher][/ .]&]
[s3;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Примерное использование]]}}&]
[s9; Наже приведён пример использования 
контрола Опция с обширными коментариями:&]
[s10; &]
[s0;l321; [C@5+75 #include <CtrlLib/CtrlLib.h>]&]
[s10; &]
[s10; using namespace Upp;&]
[s10; &]
[s10; struct MyAppWindow : TopWindow `{&]
[s10; -|Option option;&]
[s10; -|&]
[s10; -|MyAppWindow() `{&]
[s10; -|-|SetRect(0, 0, 640, 480);&]
[s10; -|-|Add(option.SetLabel(`"Опция`").LeftPosZ(20).TopPosZ(20));&]
[s10; -|-|&]
[s10; -|-|[@(28.127.0) // Дефолтно опция отключена 
`- чтобы изменить это,]&]
[s10; [@(28.127.0) -|-|// активируем её методом Set().]&]
[s10; [@(28.127.0) -|-|//]&]
[s10; [@(28.127.0) -|-|// Альтернативы:]&]
[s10; [@(28.127.0) -|-|//     `- option.Set(true);]&]
[s10; [@(28.127.0) -|-|//     `- option.Set(static`_cast<int>(true));]&]
[s10; [@(28.127.0) -|-|//     `- option.SetData(`"1`");]&]
[s10; -|-|option.Set(1);&]
[s10; -|-|&]
[s10; -|-|[@(28.127.0) // Прореагируем, если пользователь 
изменит значение опции.]&]
[s10; -|-|option << `[`=`] `{&]
[s10; -|-|-|Title(Format(`"Состояние Опции (Check`-box) 
изменён на %d`", option.Get()));&]
[s10; -|-|`};&]
[s10; -|`}&]
[s10; `};&]
[s10; &]
[s10; GUI`_APP`_MAIN&]
[s10; `{&]
[s10; -|MyAppWindow app;&]
[s10; -|app.Run();&]
[s10; &]
[s10; -|if(app.option)&]
[s10; -|-|PromptOK(`"Опция активирована!`");&]
[s10; -|else&]
[s10; -|-|PromptOK(`"Опция дезактивирована!`");&]
[s10; -|&]
[s10; -|[@(28.127.0) // Альтернативы:]&]
[s10; [@(28.127.0) -|//     `- app.option `=`= 1                     
 // активирована]&]
[s10; [@(28.127.0) -|//     `- app.option `=`= 0                     
 // дезактивирована]&]
[s10; [@(28.127.0) -|//     `- app.option `=`= static`_cast<int>(Null) 
// смешана(тристатный режим)]&]
[s10; [@(28.127.0) -|//]&]
[s10; [@(28.127.0) -|//     `- bool enabled `= static`_cast<bool>(app.option);]&]
[s10; [@(28.127.0) -|//]&]
[s10; [@(28.127.0) -|// Для получения значения также 
используется метод Get():]&]
[s10; [@(28.127.0) -|//     `- app.option.Get() `=`= 1]&]
[s10; [@(28.127.0) -|//     `- ...]&]
[s10; `}&]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Детали Конструктора]]}}&]
[s3;%- &]
[s5;:Option`:`:Option`(`):%- [* Option]()&]
[s2; Инициализует опцию в NotNull, 2`-статный 
режим, значение false и стандартный 
вид.&]
[s3; &]
[s4;%- &]
[s5;:Option`:`:`~Option`(`):%- [@(0.0.255) `~][* Option]()&]
[s2; Дефолтный деструктор.&]
[s3;%- &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Список Публичных Методов]]}}&]
[s3;%- &]
[s5;:Option`:`:Set`(int`):%- [_^Option^ Option][@(0.0.255) `&]_[* Set]([@(0.0.255) int]_[*@3 b])
&]
[s2; Устанавливает опцию в заданное значение 
(0, 1 или Null).&]
[s7; [%-*C@3 b]-|Состояние.&]
[s3;%- &]
[s4;%- &]
[s5;:Option`:`:Get`(`)const:%- [@(0.0.255) int]_[* Get]()_[@(0.0.255) const]&]
[s7; [*/ Возвратное значение]-|Текущее состояние 
Опции.&]
[s3; &]
[s4;%- &]
[s5;:Option`:`:operator int`(`)const:%- [* operator_int]()_[@(0.0.255) const]&]
[s7; [*/ Возвратное значение]-|[^topic`:`/`/CtrlLib`/src`/Option`$en`-us`#Option`:`:Get`(`)const^ G
et]()&]
[s3; &]
[s4;%- &]
[s5;:Option`:`:operator`=`(int`):%- [@(0.0.255) void]_[* operator`=]([@(0.0.255) int]_[*@3 b])
&]
[s2; То же, что и [^topic`:`/`/CtrlLib`/src`/Option`$en`-us`#Option`:`:Set`(int`)^ S
et](b).&]
[s7; [%-*C@3 b]-|Состояние.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Option`:`:EnableBox`(bool`):%- [@(0.0.255) void]_[* EnableBox]([@(0.0.255) bool
]_[*@3 b])&]
[s2; Активирует/дезактивирует все виджеты, 
являющиеся отпрысками родителя Опции 
и пересекающиеся с прямоугольником 
Опции.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Option`:`:EnableBox`(`):%- [@(0.0.255) void]_[* EnableBox]()&]
[s2; Активирует/дезактивирует все виджеты, 
являющиеся отпрысками родителя Опции 
и пересекающиеся с прямоугольником 
Опции, основываясь на её статусе.&]
[s3;%- &]
[s4;%- &]
[s5;:Option`:`:BlackEdge`(bool`):%- [_^Option^ Option][@(0.0.255) `&]_[* BlackEdge]([@(0.0.255) b
ool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; Активирует внешний вид, подходящий 
для помещения опции на белом фоне 
(SColorPaper), как в списке, в противоположность 
дефолтному представлению, подходящему 
для её помещения в диалоговом окне 
(SColorFace).&]
[s7; [%-*C@3 b]-|true, чтобы активировать.&]
[s7; [*/ Возвратное значение]-|`*this для сцепления.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Option`:`:IsBlackEdge`(`)const:%- [@(0.0.255) bool]_[* IsBlackEdge]()_[@(0.0.255) c
onst]&]
[s2; Возвращает true, если BlackEdge активен.&]
[s3;%- &]
[s4;%- &]
[s5;:Option`:`:SwitchImage`(bool`):%- [_^Option^ Option][@(0.0.255) `&]_[* SwitchImage]([@(0.0.255) b
ool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; Активирует внешнее представление, 
отрисовывающее Опцию с внешним видом 
[^topic`:`/`/CtrlLib`/src`/Switch`_ru`-ru`#Switch`:`:class^ Переключател
я].&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Option`:`:IsSwitchImage`(`)const:%- [@(0.0.255) bool]_[* IsSwitchImage]()_[@(0.0.255) c
onst]&]
[s2; Возвращает true, если SwitchImage активен.&]
[s3;%- &]
[s4;%- &]
[s5;:Option`:`:ThreeState`(bool`):%- [_^Option^ Option][@(0.0.255) `&]_[* ThreeState]([@(0.0.255) b
ool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; Активирует режим с тремя состояниями. 
Также активирует режим NoNotNull.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Option`:`:IsThreeState`(`)const:%- [@(0.0.255) bool]_[* IsThreeState]()_[@(0.0.255) c
onst]&]
[s2; Возвращает true, если ThreeState активен.&]
[s3;%- &]
[s4;%- &]
[s5;:Option`:`:NotNull`(bool`):%- [_^Option^ Option][@(0.0.255) `&]_[* NotNull]([@(0.0.255) b
ool]_[*@3 nn]_`=_[@(0.0.255) true])&]
[s2; Активирует режим NotNull `- значение 
Null, присвоенное Опции (через SetData), 
интерпретируется как false.&]
[s3; &]
[s4;%- &]
[s5;:Option`:`:NoNotNull`(`):%- [_^Option^ Option][@(0.0.255) `&]_[* NoNotNull]()&]
[s2; Активирует режим NoNotNull `- значение 
Null, присвоенное Опции, интерпретируется 
как Null.&]
[s7; [*/ Возвратное значение]-|`*this для сцепления.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Option`:`:IsNotNull`(`)const:%- [@(0.0.255) bool]_[* IsNotNull]()_[@(0.0.255) c
onst]&]
[s2; Возвращает true, если NotNull активен.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Option`:`:SetColor`(Upp`:`:Color`):%- [_^Upp`:`:Option^ Option][@(0.0.255) `&
]_[* SetColor]([_^Upp`:`:Color^ Color]_[*@3 c])&]
[s2; Устанавливает цвет текста ярлыка 
Option. Установка Null восстанавливает 
дефолтный цвет.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Option`:`:Box`(bool`):%- [_^Upp`:`:Option^ Option][@(0.0.255) `&]_[* Box]([@(0.0.255) b
ool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; Меняет визуальное представление 
на `"checked box`".&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Option`:`:AutoBox`(bool`):%- [_^Upp`:`:Option^ Option][@(0.0.255) `&]_[* AutoBo
x]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; Вызывает Box([%-*@3 b]) и активирует режим, 
когда все виджеты, являющиеся отпрысками 
родителя Опции и пересекающиеся в 
прямоугольником Опции, активируются/дезакти
вируются на основании статуса Опции.&]
[s3; &]
[s4;%- &]
[s5;:Option`:`:ShowLabel`(bool`):%- [_^Option^ Option][@(0.0.255) `&]_[* ShowLabel]([@(0.0.255) b
ool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; Переключает показ ярлыка опции (вкл/выкл).&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Option`:`:IsShowLabel`(`)const:%- [@(0.0.255) bool]_[* IsShowLabel]()_[@(0.0.255) c
onst]&]
[s0; Возвращает true, если ShowLabel активен.&]
[s0;%- &]
[ {{10000@3 [s0; [*@(229)4 Класс OptionBox]]}}&]
[s0;%- &]
[s1;:Upp`:`:OptionBox`:`:class:%- [@(0.0.255)3 class][3 _][*3 OptionBox][3 _:_][@(0.0.255)3 publ
ic][3 _][*@3;3 Option]&]
[s2; Опция с активным модификатором Box.&]
[s0;%- ]]