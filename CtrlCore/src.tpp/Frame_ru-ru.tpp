topic "Классы Frame (\"Рамка\")";
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
[{_}%RU-RU 
[ {{10000t/25b/25@3 [s0;%- [*@(229)4 Классы Frame (`"Рамка`")]]}}&]
[s9; [^topic`:`/`/CtrlCore`/srcdoc`/AboutFrames`_ru`-ru^ Фреймы] 
`- объекты, производные от класса CtrlFrame, 
оформляющие внешний вид и функциональность 
области, расположенной между внешней 
границей Ctrl`'а и его обзором.&]
[s3; &]
[s0; &]
[s0; &]
[s0;%- &]
[ {{10000t/25b/25@3 [s0;%- [*@(229)4 Класс CtrlFrame]]}}&]
[s3; &]
[s1;:CtrlFrame`:`:class:%- [@(0.0.255)3 class][3 _][*3 CtrlFrame]&]
[s9; Определение интерфейса фреймовых 
(`"рамочных`") классов.&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Список Публичных Членов]]}}&]
[s3;%- &]
[s5;:CtrlFrame`:`:`~CtrlFrame`(`):%- [@(0.0.255) `~][* CtrlFrame]()&]
[s2; Пустой виртуальный деструктор.&]
[s3; &]
[s4;%- &]
[s5;:CtrlFrame`:`:FrameLayout`(Rect`&`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* FrameL
ayout]([_^Rect^ Rect][@(0.0.255) `&]_[*@3 r])_`=_[@3 0]&]
[s2; На этот метод фрейм реагирует определением 
своей собственной выкладки (при необходимос
ти) и уменьшением размера прямоугольника 
обзора Ctrl`'а.&]
[s7; [%-*C@3 r]-|Ссылка на текущий прямоугольник 
Ctrl`'а. При перевычисление Ctrl`'ом своей 
выкладки, оно начинается с Rect`'а, эквивалентн
ого его внешнему размеру (GetRect().Size()). 
Затем вызывает FrameLayout всех своих фреймов 
(начиная с фрейма 0), а итоговый Rect 
являет собой размер обзорной площади 
Ctrl`'а.&]
[s3; &]
[s4;%- &]
[s5;:CtrlFrame`:`:FrameAddSize`(Size`&`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* Frame
AddSize]([_^Size^ Size][@(0.0.255) `&]_[*@3 sz])_`=_[@3 0]&]
[s2; Добавляет размер этого фрейма к текущему 
внешнему размеру Ctrl`'а. Используется 
для вычисления внешнего размера Ctrl`'а 
при заданном размере обзорной площади 
(view).&]
[s7; [%-*C@3 sz]-|Ссылка на действительный размер 
Ctrl`'а.&]
[s3; &]
[s4;%- &]
[s5;:CtrlFrame`:`:FramePaint`(Draw`&`,const Rect`&`):%- [@(0.0.255) virtual] 
[@(0.0.255) void]_[* FramePaint]([_^Draw^ Draw][@(0.0.255) `&]_[*@3 w], 
[@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 r])&]
[s2; Отрисовывает эту рамку. Дефолтная 
реализация пустая.&]
[s7; [%-*C@3 w]-|Draw.&]
[s7; [%-*C@3 r]-|Внешний периметр рамки (это 
тот же прямоугольник, что был указан 
в последней FrameLayout).&]
[s3; &]
[s4;%- &]
[s5;:CtrlFrame`:`:FrameAdd`(Ctrl`&`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* FrameAdd](
[_^Ctrl^ Ctrl][@(0.0.255) `&]_[*@3 parent])&]
[s2; Вызывается при добавлении рамки 
в этот Ctrl. Frame может использовать его 
для добавления своих подктрлов к 
родителю. Дефолтная реализация пустая.&]
[s7; [%-*C@3 parent]-|Parent Ctrl.&]
[s3; &]
[s4;%- &]
[s5;:CtrlFrame`:`:FrameRemove`(`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* FrameRemove](
)&]
[s2; Вызывается при удалении рамки из 
этого Ctrl. Frame может использовать его 
для удаления своих подктрлов из родителя. 
Дефолтная реализация пустая.&]
[s3; &]
[s4;%- &]
[s5;:CtrlFrame`:`:OverPaint`(`)const:%- [@(0.0.255) virtual] [@(0.0.255) int]_[* OverPaint](
)_[@(0.0.255) const]&]
[s2; Этот метод может вернуть ненулевое 
число, представляющее кромку расширения 
отрисовки прямоугольника контрола: 
рамка может рисоваться над этой кромкой, 
несмотря на тот факт, что она не принадлежит 
к прямоугольнику Ctrl`'а. Это используется 
для определённых поверхностных эффектов 
(типа сияния вокруг EditField). Дефолтная 
реализация равна нулю.&]
[s3; &]
[s0; &]
[s0; &]
[s0; &]
[ {{10000t/25b/25@3 [s0;%- [*@(229)4 Стандартные статические рамки]]}}&]
[s9; U`+`+ определяет несколько стандартных 
статических фреймов. Эти рамки в основном 
используются для определения (или 
изменения) внешнего вида границ Ctrl`'ов. 
Все они получены как ссылка на единый 
глобальный экземпляр единой глобальной 
функцией и могут присваиваться неограниченн
ому числу контролов.&]
[s9; Внешний вид некоторых из них можно 
изменить соответственно текущему 
`"look`&feel`" ОС.&]
[s3; &]
[s0; &]
[ {{3967:1202:1908:2923h1;@(204) [s0; Функция]
:: [s0; Меняется через look`&feel]
:: [s0; Внешний вид]
:: [s0; Комментарий]
::@2 [s0; CtrlFrame`&_[* NullFrame]()]
:: [s0; Нет.]
:: [s0; 
@@image:612&237
(A2IAJgAAANr/AAAAAHic7dBBCQAgAMDA/g2tINhBKwzBlzcuwdYcCwAAntmf5VLJpZJLJZdKLpVcKrlUcqnkUsmlkksll0oulVwquVRyqeRSyaWSS6W7SwAAEB2VfOL1)
]
:: [s0; [1 Дефолтный Frame для Ctrl.]]
:: [s0; CtrlFrame`&_[* InsetFrame]()]
:: [s0; Нет.]
:: [s0; 
@@image:612&237
(A2IAJgAAANr/AAAAAHic7dHBDYAgFERB++/ADjxYkC2Y0ANawgaiRJnNXDnwfjmPAgAAj9m3dR611uZKyxy7P9tZqU4wlZKplEylZColUymZSslUSqZSMpWSqZRMpWQqJVMpmUrJVErWWant4Rc1Vxp94bc3/FIAAPzbBXs9SD0=)
]
:: [s0; ]
:: [s0; CtrlFrame`&_[* OutsetFrame]()]
:: [s0; Нет.]
:: [s0; 
@@image:612&237
(A2IAJgAAANr/AAAAAHic7dKxDYNAEEXB678Dd0DggtwCEj1gOuDpILt5mnSTrz323wEAALxhjDF9e67Rd/tY6TYrlaxUslLJSiUrlaxUslLJSiUrlaxUslLJSiUrlaxUslLp4UrX+SKmVxqLNf1LAABQ/AG7lTfP)
]
:: [s0; ]
:: [s0; CtrlFrame`&_[* ButtonFrame]()]
:: [s0; Нет.]
:: [s0; 
@@image:612&237
(A2IAJgAAANr/AAAAAHic7dGxDYAwEATB778DOiCgIDJiJPcAlHCyBQSe06Sf7LdzbwAA8JprplVVd6Vjjm3rMljp5x9/MpWSqZRMpWQqJVMpmUrJVEqmUjKVkqmUTKVkKiVTKZlKyVRKNljpOZ9Ed6WabH2VAAAgdAN14TM/)
]
:: [s0; ]
:: [s0; CtrlFrame`&_[* ThinInsetFrame]()]
:: [s0; Нет.]
:: [s0; 
@@image:612&237
(A2IAJgAAANr/AAAAAHic7dBBDYAwAARB/xJwwANBWCCph2Jh0wR4MJcRcNlxnQMAAB5z7Nt/zDmXK80/TaUylcpUKlOpTKUylcpUKlOpTKUylcpUKlOpTKUylcpUKlOpTKWytUpfv357a5UAACC6AbZWe+U=)
]
:: [s0; ]
:: [s0; CtrlFrame`&_[* ThinOutsetFrame]()]
:: [s0; Нет.]
:: [s0; 
@@image:612&237
(A2IAJgAAANr/AAAAAHic7dAxDYBAAARB/xJwQIEgLJC8B8ABm0+ofi7TXrPjOgcAAPzmXmnHvqn0OZXKVCpTqUylMpXKVCpTqUylMpXKVCpTqUylMpXKVCpTqUylMpXKpiu9x6XMVQIAgOgB/DB5xQ==)
]
:: [s0; ]
:: [s0; CtrlFrame`&_[* BlackFrame]()]
:: [s0; Нет.]
:: [s0; 
@@image:612&237
(A2IAJgAAANr/AAAAAHic7dBRDYAwFMBA/CvEAsk8gIVmC1/vmlPQ9dwLAAB+cw1r+9I7JpdKLpVcKrlUcqnkUsmlkksll0oulVwquVRyqeRSyaWSSyWXSieXRrV3CQAAog9J6pkj)
]
:: [s0; ]
:: [s0; CtrlFrame`&_[* FieldFrame]()]
:: [s0; Да.]
:: [s0; 
@@image:612&237
(A2IAJgAAANr/AAAAAHic7dBBDYAwAMBAx/jADHqwQDIPzEIzwmvXnIKO5x4AAPCb47y2snzp3SaXSi6VXCq5VHKp5FLJpZJLJZdKLpVcKrlUcqnkUsmlkksll0pfLm1l7RIAAEQTozHbhg==)
]
:: [s0; [1 Подходит для границ всех Ctrl`'ов, что`-либо 
показывающих, типа EditField или ArrayCtrl.]]
:: [s0; CtrlFrame`&_[* TopSeparatorFrame]()]
:: [s0; Да.]
:: [s0; 
@@image:612&237
(A2IAJgAAANr/AAAAAHic7dBRCQAhEEDB/hGugR9XwwjCXQTBDlpBBGFh5zEJ3ujfAACAa97ypHJ2aSbr7NLfaiouueSSS9G45JJLLkXjkksuuRSNSy655FI0Lt27BAAAmxZrvoWI)
]
:: [s0; ]
:: [s0; CtrlFrame`&_[* BottomSeparatorFrame]()]
:: [s0; Да.]
:: [s0; 
@@image:612&237
(A2IAJgAAANr/AAAAAHic7dBRCQAhEEDB/hGugR9XwwjCXQTBDlpBBGFh5zEJ3ujfAACAa/5WU3HJJZdcisYll1xyKRqXXHLJpWhccskll6Jx6d6ltzypnF2ayTq7BAAAmxaTOIWI)
]
:: [s0; ]
:: [s0; CtrlFrame`&_[* LeftSeparatorFrame]()]
:: [s0; Да.]
:: [s0; 
@@image:612&237
(A2IAJgAAANr/AAAAAHic7dBBCcAwFAUw/xLqYIfamITCKmFQD78Wdhk8aCAKst5nAQDAb/rVqmqO+xCWLFmylMaSJUuW0liyZMlSGkuWLFlKY8mSpfwlAAD4aAPkY4SY)
]
:: [s0; ]
:: [s0; CtrlFrame`&_[* RightSeparatorFrame]()]
:: [s0; Да.]
:: [s0; 
@@image:612&237
(A2IAJgAAANr/AAAAAHic7dBRCYAwGAbA/hFssAdrLMJAIwzW4V8FX4RPPLgEt+a1AADgNffoP3G2o6osWbJkKYclS5YspbFkyZKlNJYsWbKUxpIlS19ZAgCAhzabQ4SY)
]
:: [s0; ]}}&]
[s3; &]
[s0; &]
[s0; &]
[s0;%- &]
[ {{10000t/25b/25@3 [s0;%- [*@(229)4 Простые шаблоны рамок Ctrl`'ов]]}}&]
[s3;%- &]
[s1;:noref:%- [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@3;3 T][3 >]&]
[s1;:FrameCtrl`:`:class:%- [@(0.0.255) class]_[* FrameCtrl]_:_[@(0.0.255) public]_[*@3 T], 
[@(0.0.255) public]_[*@3 CtrlFrame]&]
[s2; Это класс`-основа простых рамок Ctrl`'а 
`- фреймов, которые помещают единственный 
Ctrl на какой`-то кромке родительского 
контрола.&]
[s2; &]
[s2; Этот класс, как правило, перегружает 
виртуальные методы FrameAdd и FrameRemove класса 
[^topic`:`/`/CtrlCore`/src`/Frame`_ru`-ru`#CtrlFrame`:`:class^ CtrlFrame] 
так, что они добавляют/удаляют `'this`' 
из списка отпрысков родителя.&]
[s2; &]
[s7; [%-*C@4 T]-|Тип контрола Ctrl.&]
[s0; &]
[s0;%- [%RU-RU/ Производный от][%RU-RU  ]T, [^topic`:`/`/CtrlCore`/src`/Frame`_ru`-ru`#CtrlFrame`:`:class^@(0.0.255) C
trlFrame]&]
[s0;^topic`:`/`/CtrlCore`/src`/Frame`$en`-us`#`:`:CtrlFrame`:`:class CtrlFrame^@(0.0.255)%- &]
[s0;^topic`:`/`/CtrlCore`/src`/Frame`$en`-us`#`:`:CtrlFrame`:`:class CtrlFrame^@(0.0.255)%- &]
[s5;:FrameCtrl`:`:GetParent`(`):%- [_^Ctrl^ Ctrl]_`*[* GetParent]()&]
[s2; Возвращает родителя, к которому прикреплён 
этот Ctrl.&]
[s3;%- &]
[s4; &]
[s1;:noref:%- [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@4;3 T][3 >]&]
[s1;:FrameLR`:`:class:%- [@(0.0.255) class]_[* FrameLR]_:_[@(0.0.255) public]_[*@3 FrameCtrl]<
[*@4 T]>_&]
[s2; Этот класс расширяет класс CtrlFrame 
атрибутом width (`"ширина`") и служит как 
класс`-основа классов, помещающих 
Ctrl слева или справа от родительской 
рамки. Width инициализуется в 0. 0, и показывает, 
что ширина равна высоте.&]
[s2; &]
[s7; [%-*C@4 T]-|Тип контрола.&]
[s0; &]
[s0;%- [%RU-RU/ Производный от][%RU-RU  ][^topic`:`/`/CtrlCore`/src`/Frame`_ru`-ru`#FrameCtrl`:`:class^ F
rameCtrl<T>] &]
[s0; &]
[s0; &]
[s0; &]
[s5;:FrameLR`:`:Width`(int`):%- [_^FrameLR^ FrameLR][@(0.0.255) `&]_[* Width]([@(0.0.255) int
]_[*@3 `_cx])&]
[s2; Устанавливает новую ширину.&]
[s7; [%-*C@3 `_cx]-|Ширина.&]
[s7; [*/ Возвратное значение]-|`*this.&]
[s0;3 &]
[s5;:FrameLR`:`:GetWidth`(`)const:%- [@(0.0.255) int]_[* GetWidth]()_[@(0.0.255) const]&]
[s7; [*/ Возвратное значение]-|Текущая ширина.&]
[s3; &]
[s4; &]
[s1;:noref:%- [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@4;3 T][3 >]&]
[s1;:FrameLeft`:`:class:%- [@(0.0.255) class]_[* FrameLeft]_:_[@(0.0.255) public]_[*@3 FrameLR
]<[*@4 T]>_&]
[s2; Этот класс помещает Ctrl, с левой стороны 
родителя, как рамку.&]
[s7; [%-*C@4 T]-|Тип контрола.&]
[s0; &]
[s0;%- [%RU-RU/ Производный от][%RU-RU  ][^topic`:`/`/CtrlCore`/src`/Frame`_ru`-ru`#FrameLR`:`:class^ F
rameLR<T>] &]
[s3; &]
[s4; &]
[s1;:noref:%- [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@4;3 T][3 >]&]
[s1;:FrameRight`:`:class:%- [@(0.0.255) class]_[* FrameRight]_:_[@(0.0.255) public]_[*@3 Frame
LR]<[*@4 T]>_&]
[s2; Этот класс помещает Ctrl, с правой стороны 
родителя, как рамку.&]
[s7; [%-*C@4 T]-|Тип контрола.&]
[s0; &]
[s0;%- [%RU-RU/ Производный от][%RU-RU  ][^topic`:`/`/CtrlCore`/src`/Frame`_ru`-ru`#FrameLR`:`:class^ F
rameLR<T>] &]
[s3; &]
[s1;:noref:%- [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@4;3 T][3 >]&]
[s1;:FrameTB`:`:class:%- [@(0.0.255) class]_[* FrameTB]_:_[@(0.0.255) public]_[*@3 FrameCtrl]<
[*@4 T]>_&]
[s2; Этот класс расширяет класс CtrlFrame 
атрибутом высоты и служит как класс`-основа 
для классов, помещающих Ctrl в качестве 
верхней или нижней стороны рамки 
родительского Ctrl`'а. Height (`"высота`") 
инициализируется в 0. 0, показывая, 
что высота равна ширине.&]
[s7; [%-*C@4 T]-|Тип контрола.&]
[s0; &]
[s0;%- [%RU-RU/ Производный от][%RU-RU  ][^topic`:`/`/CtrlCore`/src`/Frame`_ru`-ru`#FrameCtrl`:`:class^ F
rameCtrl<T>] &]
[s3; &]
[s0; &]
[s0; &]
[s5;:FrameTB`:`:Height`(int`):%- [_^FrameTB^ FrameTB][@(0.0.255) `&]_[* Height]([@(0.0.255) i
nt]_[*@3 `_cy])&]
[s2; Устанавливает новую высоту.&]
[s7; [%-*C@3 `_cy]-|Высота.&]
[s7; [*/ Возвратное значение]-|`*this.&]
[s0; &]
[s5;:FrameTB`:`:GetHeight`(`)const:%- [@(0.0.255) int]_[* GetHeight]()_[@(0.0.255) const]&]
[s7; [*/ Возвратное значение]-|Текущая высота.&]
[s3; &]
[s4; &]
[s1;:noref:%- [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@4;3 T][3 >]&]
[s1;:FrameTop`:`:class:%- [@(0.0.255) class]_[* FrameTop]_:_[@(0.0.255) public]_[*@3 FrameTB]<
[*@4 T]>_&]
[s2; Этот класс помещает Ctrl как рамку 
с верхней стороны родителя.&]
[s7; [%-*C@4 T]-|Тип контрола.&]
[s0; &]
[s0;%- [%RU-RU/ Производный от][%RU-RU  ][^topic`:`/`/CtrlCore`/src`/Frame`_ru`-ru`#FrameTB`:`:class^ F
rameTB<T>] &]
[s3; &]
[s4; &]
[s1;:noref:%- [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@4;3 T][3 >]&]
[s1;:FrameBottom`:`:class:%- [@(0.0.255) class]_[* FrameBottom]_:_[@(0.0.255) public]_[*@3 Fra
meTB]<[*@4 T]>_&]
[s2; Этот класс помещает Ctrl как рамку 
снизу родителя.&]
[s7; [%-*C@4 T]-|Тип контрола.&]
[s0; &]
[s0;%- [%RU-RU/ Производный от][%RU-RU  ][^topic`:`/`/CtrlCore`/src`/Frame`_ru`-ru`#FrameTB`:`:class^ F
rameTB<T>] &]
[s3; &]
[s0; &]
[s0; &]
[s0;%- &]
[ {{10000t/25b/25@3 [s0;%- [*@(229)4 Вспомогательные функции рамок]]}}&]
[s9; Следующие функции служат в качестве 
вспомогательных для реализации метода 
FrameLayout класса CtrlFrame, помещая какой`-либо 
Ctr сбоку от родительского Ctrl`'а. Они 
настраивают заданный Rect (параметр 
FrameLayout`'а) , а также меняют позицию 
заданного Ctrl`'а.&]
[s3; &]
[s0; &]
[s5;:LayoutFrameLeft`(Rect`&`,Ctrl`*`,int`):%- [@(0.0.255) void]_[* LayoutFrameLeft]([_^Rect^ R
ect][@(0.0.255) `&]_[*@3 r], [_^Ctrl^ Ctrl]_`*[*@3 ctrl], [@(0.0.255) int]_[*@3 cx])&]
[s2; Помещает ctrl с левой стороны родительского 
Ctrl`'а.&]
[s7; [%-*C@3 r]-|Текущий прямоугольник родительского 
контрола.&]
[s7; [%-*C@3 ctrl]-|Помещаемый контрол.&]
[s7; [%-*C@3 cx]-|Требуемая ширина.&]
[s3; &]
[s4;%- &]
[s5;:LayoutFrameRight`(Rect`&`,Ctrl`*`,int`):%- [@(0.0.255) void]_[* LayoutFrameRight]([_^Rect^ R
ect][@(0.0.255) `&]_[*@3 r], [_^Ctrl^ Ctrl]_`*[*@3 ctrl], [@(0.0.255) int]_[*@3 cx])&]
[s2; Помещает контрол с правой стороны 
родительского контрола.&]
[s7; [%-*C@3 r]-|Текущий прямоугольник родительского 
контрола.&]
[s7; [%-*C@3 ctrl]-|Помещаемый контрол.&]
[s7; [%-*C@3 cx]-|Требуемая ширина.&]
[s3; &]
[s4;%- &]
[s5;:LayoutFrameTop`(Rect`&`,Ctrl`*`,int`):%- [@(0.0.255) void]_[* LayoutFrameTop]([_^Rect^ R
ect][@(0.0.255) `&]_[*@3 r], [_^Ctrl^ Ctrl]_`*[*@3 ctrl], [@(0.0.255) int]_[*@3 cy])&]
[s2; Помещает контрол ctrl с верхней стороны 
родительского контрола.&]
[s7; [%-*C@3 r]-|Текущий прямоугольник родительского 
контрола.&]
[s7; [%-*C@3 ctrl]-|Помещаемый контрол.&]
[s7; [%-*C@3 cy]-|Требуемая высота.&]
[s3; &]
[s4;%- &]
[s5;:LayoutFrameBottom`(Rect`&`,Ctrl`*`,int`):%- [@(0.0.255) void]_[* LayoutFrameBottom](
[_^Rect^ Rect][@(0.0.255) `&]_[*@3 r], [_^Ctrl^ Ctrl]_`*[*@3 ctrl], [@(0.0.255) int]_[*@3 cy])&]
[s2; Помещает контрол ctrl с нижней стороны 
родительского контрола.&]
[s7; [%-*C@3 r]-|Текущий прямоугольник родительского 
контрола.&]
[s7; [%-*C@3 ctrl]-|Помещаемый контрол.&]
[s7; [%-*C@3 cy]-|Требуемая высота.&]
[s3; &]
[s0; ]]