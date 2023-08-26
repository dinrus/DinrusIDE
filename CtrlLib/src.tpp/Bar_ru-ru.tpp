topic "Класс Bar (Брус)";
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
[ {{10000t/25b/25@3 [s0;%- [*@(229)4 Класс Bar (Брус)]]}}&]
[s3;%- &]
[s1;:Bar`:`:class:%- [@(0.0.255)3 class][3 _][*3 Bar][3 _:_][@(0.0.255)3 public][3 _][*@3;3 Ctrl]&]
[s9; Абстрактный класс`-основа для MenuBar 
и ToolBar, также предоставляющий средства 
для чтения клавиш`-акселераторов 
клавиатуры из процедур класса Bar.&]
[s9; &]
[s0; [/ Производный от] [^topic`:`/`/CtrlCore`/src`/Ctrl`_ru`-ru`#Ctrl`:`:class^ C
trl]&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Список Публичных Членов]]}}&]
[s3;%- &]
[s5;:Bar`:`:IsEmpty`(`)const:%- [@(0.0.255) virtual] [@(0.0.255) bool]_[* IsEmpty]()_[@(0.0.255) c
onst]_`=_[@3 0]&]
[s2; Возвращает true, если Bar пустой.&]
[s3; &]
[s4;%- &]
[s5;:Bar`:`:Separator`(`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* Separator]()_`=_[@3 0]&]
[s2; Добавляет видимый разделитель к 
этому Bar.&]
[s3; &]
[s4;%- &]
[s5;:Bar`:`:NilItem`(`):%- [@(0.0.255) static] [_^Bar`:`:Item^ Item][@(0.0.255) `&]_[* NilIte
m]()&]
[s2; Возвращает ссылку на `"произвольный`" 
(dummy) элемент `- все методы, вызываемые 
по отношению к этому элементу, игнорируются.
&]
[s0; &]
[s4; &]
[s5;:Bar`:`:Scan`(Callback1`<Bar`&`>`,dword`):%- [@(0.0.255) static] 
[@(0.0.255) bool]_[* Scan]([_^Callback1^ Callback1][@(0.0.255) <]Bar[@(0.0.255) `&>]_[*@3 pro
c], [_^dword^ dword]_[*@3 key])&]
[s2; Сканирует иерархию процедур бруса, 
тестируя на акселератор клавиши[*  
][%-*@3 key]. Если у любого элемента есть 
этот акселератор, вызывается ассоциированны
й обратный вызов действия элемента 
и возвращается true.&]
[s3; &]
[s4;%- &]
[s5;:Bar`:`:Break`(`):%- [@(0.0.255) void]_[* Break]()&]
[s2; Этот метод прерывает текущую строку 
(или колонку) бруса и начинает новую.&]
[s0; &]
[s4;%- &]
[s5;:Bar`:`:Gap`(int`):%- [@(0.0.255) void]_[* Gap]([@(0.0.255) int]_[*@3 size]_`=_[@3 8])&]
[s2; Добавляет зримый промежуток размером 
[%-*@3 size] пикселей к этому Bar.&]
[s3; &]
[s4;%- &]
[s5;:Bar`:`:GapRight`(`):%- [@(0.0.255) void]_[* GapRight]()&]
[s2; Добавляет промежуток, который помещает 
остальные элементы строки этого Bar 
в `"правый`" размер (right size).&]
[s3; &]
[s4;%- &]
[s5;:Bar`:`:Add`(Ctrl`&`):%- [@(0.0.255) void]_[* Add]([_^Ctrl^ Ctrl][@(0.0.255) `&]_[*@3 ctrl])
&]
[s5;:Bar`:`:Add`(Ctrl`&`,Size`):%- [@(0.0.255) void]_[* Add]([_^Ctrl^ Ctrl][@(0.0.255) `&]_[*@3 c
trl], [_^Size^ Size]_[*@3 sz])&]
[s5;:Bar`:`:Add`(Ctrl`&`,int`,int`):%- [@(0.0.255) void]_[* Add]([_^Ctrl^ Ctrl][@(0.0.255) `&
]_[*@3 ctrl], [@(0.0.255) int]_[*@3 cx], [@(0.0.255) int]_[*@3 cy]_`=_[@3 0])&]
[s5;:Bar`:`:Add`(bool`,Ctrl`&`):%- [@(0.0.255) void]_[* Add]([@(0.0.255) bool]_[*@3 en], 
[_^Ctrl^ Ctrl][@(0.0.255) `&]_[*@3 ctrl])&]
[s5;:Bar`:`:Add`(bool`,Ctrl`&`,Size`):%- [@(0.0.255) void]_[* Add]([@(0.0.255) bool]_[*@3 en],
 [_^Ctrl^ Ctrl][@(0.0.255) `&]_[*@3 ctrl], [_^Size^ Size]_[*@3 sz])&]
[s5;:Bar`:`:Add`(bool`,Ctrl`&`,int`,int`):%- [@(0.0.255) void]_[* Add]([@(0.0.255) bool]_[*@3 e
n], [_^Ctrl^ Ctrl][@(0.0.255) `&]_[*@3 ctrl], [@(0.0.255) int]_[*@3 cx], 
[@(0.0.255) int]_[*@3 cy]_`=_[@3 0])&]
[s2; Добавляет виджет [%-*@3 ctrl] к данному 
брусу.&]
[s2; &]
[s2; Если у [%-*@3 ctrl] определена горизонтальная/верт
икальная логическая позиция посредством 
LeftPos/TopPos, ненулевой размер логической 
позиции используется для определения 
размера виджета в этом Bar (другими 
словами, можно установить размер, 
вызвав SetRect(0, 0, ширина, высота).&]
[s2; &]
[s2; Если либо вертикальный, либо горизонтальный
 размер логической позиции пуст, тогда 
используются ненулевые параметры 
метода [%-*@3 cx] [%-*@3 cy].&]
[s2; &]
[s2; Если хотя бы какой`-то из них равен 
нулю (или отсутствует в декларации 
метода), то GetMinSize() используется для 
[%-*@3 ctrl]. Enable([%-*@3 en]) вызывается для активировани
я или дезактивирования этого виджета.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Bar`:`:Add`(bool`,const char`*`,const Upp`:`:Image`&`,const Upp`:`:Callback`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* Add]([@(0.0.255) bool]_[*@3 enable], [@(0.0.255) const]_[@(0.0.255) ch
ar]_`*[*@3 text], [@(0.0.255) const]_[_^Upp`:`:Image^ Upp`::Image][@(0.0.255) `&]_[*@3 image
], [@(0.0.255) const]_[_^Upp`:`:Callback^ Callback][@(0.0.255) `&]_[*@3 callback])&]
[s5;:Upp`:`:Bar`:`:Add`(bool`,Upp`:`:KeyInfo`&`(`*`)`(`)`,const Upp`:`:Image`&`,const Upp`:`:Callback`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* Add]([@(0.0.255) bool]_[*@3 enable], [_^Upp`:`:KeyInfo^ KeyInfo][@(0.0.255) `&
]_(`*[*@3 key])(), [@(0.0.255) const]_Upp`::Image[@(0.0.255) `&]_[*@3 image], 
[@(0.0.255) const]_Callback[@(0.0.255) `&]_[*@3 callback])&]
[s5;:Upp`:`:Bar`:`:Add`(const char`*`,const Upp`:`:Image`&`,const Upp`:`:Callback`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* Add]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 text], 
[@(0.0.255) const]_[_^Upp`:`:Image^ Upp`::Image][@(0.0.255) `&]_[*@3 image], 
[@(0.0.255) const]_[_^Upp`:`:Callback^ Callback][@(0.0.255) `&]_[*@3 callback])&]
[s5;:Upp`:`:Bar`:`:Add`(Upp`:`:KeyInfo`&`(`*`)`(`)`,const Upp`:`:Image`&`,const Upp`:`:Callback`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* Add]([_^Upp`:`:KeyInfo^ KeyInfo][@(0.0.255) `&]_(`*[*@3 key])(), 
[@(0.0.255) const]_Upp`::Image[@(0.0.255) `&]_[*@3 image], [@(0.0.255) const]_Callback[@(0.0.255) `&
]_[*@3 callback])&]
[s5;:Upp`:`:Bar`:`:Add`(bool`,const char`*`,const Upp`:`:Callback`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* Add]([@(0.0.255) bool]_[*@3 enable], [@(0.0.255) const]_[@(0.0.255) ch
ar]_`*[*@3 text], [@(0.0.255) const]_[_^Upp`:`:Callback^ Callback][@(0.0.255) `&]_[*@3 call
back])&]
[s5;:Upp`:`:Bar`:`:Add`(bool`,Upp`:`:KeyInfo`&`(`*`)`(`)`,const Upp`:`:Callback`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* Add]([@(0.0.255) bool]_[*@3 enable], [_^Upp`:`:KeyInfo^ KeyInfo][@(0.0.255) `&
]_(`*[*@3 key])(), [@(0.0.255) const]_Callback[@(0.0.255) `&]_[*@3 callback])&]
[s5;:Upp`:`:Bar`:`:Add`(const char`*`,const Upp`:`:Callback`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* Add]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 text], 
[@(0.0.255) const]_[_^Upp`:`:Callback^ Callback][@(0.0.255) `&]_[*@3 callback])&]
[s5;:Upp`:`:Bar`:`:Add`(Upp`:`:KeyInfo`&`(`*`)`(`)`,const Upp`:`:Callback`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* Add]([_^Upp`:`:KeyInfo^ KeyInfo][@(0.0.255) `&]_(`*[*@3 key])(), 
[@(0.0.255) const]_Callback[@(0.0.255) `&]_[*@3 callback])&]
[s5;:Upp`:`:Bar`:`:Add`(bool`,const char`*`,const Upp`:`:Image`&`,const Upp`:`:Function`<void`(`)`>`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* Add]([@(0.0.255) bool]_[*@3 enable], [@(0.0.255) const]_[@(0.0.255) ch
ar]_`*[*@3 text], [@(0.0.255) const]_[_^Upp`:`:Image^ Upp`::Image][@(0.0.255) `&]_[*@3 image
], [@(0.0.255) const]_[_^Upp`:`:Function^ Function]<[@(0.0.255) void]_()>`&_[*@3 fn])&]
[s5;:Upp`:`:Bar`:`:Add`(bool`,Upp`:`:KeyInfo`&`(`*`)`(`)`,const Upp`:`:Image`&`,const Upp`:`:Function`<void`(`)`>`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* Add]([@(0.0.255) bool]_[*@3 enable], [_^Upp`:`:KeyInfo^ KeyInfo][@(0.0.255) `&
]_(`*[*@3 key])(), [@(0.0.255) const]_Upp`::Image[@(0.0.255) `&]_[*@3 image], 
[@(0.0.255) const]_Function<[@(0.0.255) void]_()>`&_[*@3 fn])&]
[s5;:Upp`:`:Bar`:`:Add`(const char`*`,const Upp`:`:Image`&`,const Upp`:`:Function`<void`(`)`>`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* Add]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 text], 
[@(0.0.255) const]_[_^Upp`:`:Image^ Upp`::Image][@(0.0.255) `&]_[*@3 image], 
[@(0.0.255) const]_[_^Upp`:`:Function^ Function]<[@(0.0.255) void]_()>`&_[*@3 fn])&]
[s5;:Upp`:`:Bar`:`:Add`(Upp`:`:KeyInfo`&`(`*`)`(`)`,const Upp`:`:Image`&`,const Upp`:`:Function`<void`(`)`>`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* Add]([_^Upp`:`:KeyInfo^ KeyInfo][@(0.0.255) `&]_(`*[*@3 key])(), 
[@(0.0.255) const]_Upp`::Image[@(0.0.255) `&]_[*@3 image], [@(0.0.255) const]_Function<[@(0.0.255) v
oid]_()>`&_[*@3 fn])&]
[s5;:Upp`:`:Bar`:`:Add`(bool`,const char`*`,const Upp`:`:Function`<void`(`)`>`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* Add]([@(0.0.255) bool]_[*@3 enable], [@(0.0.255) const]_[@(0.0.255) ch
ar]_`*[*@3 text], [@(0.0.255) const]_[_^Upp`:`:Function^ Function]<[@(0.0.255) void]_()>`&
_[*@3 fn])&]
[s5;:Upp`:`:Bar`:`:Add`(bool`,Upp`:`:KeyInfo`&`(`*`)`(`)`,const Upp`:`:Function`<void`(`)`>`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* Add]([@(0.0.255) bool]_[*@3 enable], [_^Upp`:`:KeyInfo^ KeyInfo][@(0.0.255) `&
]_(`*[*@3 key])(), [@(0.0.255) const]_Function<[@(0.0.255) void]_()>`&_[*@3 fn])&]
[s5;:Upp`:`:Bar`:`:Add`(const char`*`,const Upp`:`:Function`<void`(`)`>`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* Add]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 text], 
[@(0.0.255) const]_[_^Upp`:`:Function^ Function]<[@(0.0.255) void]_()>`&_[*@3 fn])&]
[s5;:Upp`:`:Bar`:`:Add`(Upp`:`:KeyInfo`&`(`*`)`(`)`,const Upp`:`:Function`<void`(`)`>`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* Add]([_^Upp`:`:KeyInfo^ KeyInfo][@(0.0.255) `&]_(`*[*@3 key])(), 
[@(0.0.255) const]_Function<[@(0.0.255) void]_()>`&_[*@3 fn])&]
[s2; Добавляет элемент к меню или тулбару. 
Если присутствует [%-*@3 text], то элемент 
добавляется к меню; если же присутствует 
[%-*@3 image], то он добавляется к тулбару. 
Если присутствуют оба, то [%-*@3 image] также 
используется как иконка в меню, а 
[%-*@3 text] как подсказка к кнопке тулбара 
(т.е. `"тултип`"). Когда выделен элемент 
меню или нажата кнопка тулбара, вызывается 
[%-*@3 callback] или [%-*@3 fn]. [%-*@3 enable] устанавливает 
статус этого элемента, а [%-*@3 key] `- это 
клавиша акселератора, любо настаиваемое 
инфо акселератора. Все варианты возвращают 
ссылку на элемент, которую можно использоват
ь для последующей его настройки.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Bar`:`:Add`(bool`,const char`*`,const Upp`:`:Event`<Upp`:`:Bar`&`>`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* Add]([@(0.0.255) bool]_[*@3 enable], [@(0.0.255) const]_[@(0.0.255) ch
ar]_`*[*@3 text], [@(0.0.255) const]_[_^Upp`:`:Event^ Event]<Bar[@(0.0.255) `&]>`&_[*@3 pro
c])&]
[s5;:Upp`:`:Bar`:`:Add`(const char`*`,const Upp`:`:Event`<Upp`:`:Bar`&`>`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* Add]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 text], 
[@(0.0.255) const]_[_^Upp`:`:Event^ Event]<Bar[@(0.0.255) `&]>`&_[*@3 proc])&]
[s5;:Upp`:`:Bar`:`:Add`(bool`,const char`*`,const Upp`:`:Image`&`,const Upp`:`:Event`<Upp`:`:Bar`&`>`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* Add]([@(0.0.255) bool]_[*@3 enable], [@(0.0.255) const]_[@(0.0.255) ch
ar]_`*[*@3 text], [@(0.0.255) const]_[_^Upp`:`:Image^ Upp`::Image][@(0.0.255) `&]_[*@3 image
], [@(0.0.255) const]_[_^Upp`:`:Event^ Event]<Bar[@(0.0.255) `&]>`&_[*@3 proc])&]
[s5;:Upp`:`:Bar`:`:Add`(const char`*`,const Upp`:`:Image`&`,const Upp`:`:Event`<Upp`:`:Bar`&`>`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* Add]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 text], 
[@(0.0.255) const]_[_^Upp`:`:Image^ Upp`::Image][@(0.0.255) `&]_[*@3 image], 
[@(0.0.255) const]_[_^Upp`:`:Event^ Event]<Bar[@(0.0.255) `&]>`&_[*@3 proc])&]
[s5;:Upp`:`:Bar`:`:Sub`(bool`,const char`*`,const Upp`:`:Function`<void`(Upp`:`:Bar`&`)`>`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* Sub]([@(0.0.255) bool]_[*@3 enable], [@(0.0.255) const]_[@(0.0.255) ch
ar]_`*[*@3 text], [@(0.0.255) const]_[_^Upp`:`:Function^ Function]<[@(0.0.255) void]_(Bar
[@(0.0.255) `&])>`&_[*@3 submenu])&]
[s5;:Upp`:`:Bar`:`:Sub`(const char`*`,const Upp`:`:Function`<void`(Upp`:`:Bar`&`)`>`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* Sub]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 text], 
[@(0.0.255) const]_[_^Upp`:`:Function^ Function]<[@(0.0.255) void]_(Bar[@(0.0.255) `&])>`&
_[*@3 submenu])&]
[s5;:Upp`:`:Bar`:`:Sub`(bool`,const char`*`,const Upp`:`:Image`&`,const Upp`:`:Function`<void`(Upp`:`:Bar`&`)`>`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* Sub]([@(0.0.255) bool]_[*@3 enable], [@(0.0.255) const]_[@(0.0.255) ch
ar]_`*[*@3 text], [@(0.0.255) const]_[_^Upp`:`:Image^ Upp`::Image][@(0.0.255) `&]_[*@3 image
], [@(0.0.255) const]_[_^Upp`:`:Function^ Function]<[@(0.0.255) void]_(Bar[@(0.0.255) `&])
>`&_[*@3 submenu])&]
[s5;:Upp`:`:Bar`:`:Sub`(const char`*`,const Upp`:`:Image`&`,const Upp`:`:Function`<void`(Upp`:`:Bar`&`)`>`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* Sub]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 text], 
[@(0.0.255) const]_[_^Upp`:`:Image^ Upp`::Image][@(0.0.255) `&]_[*@3 image], 
[@(0.0.255) const]_[_^Upp`:`:Function^ Function]<[@(0.0.255) void]_(Bar[@(0.0.255) `&])>`&
_[*@3 submenu])&]
[s2; Добавляет подменю. Для тулбара просто 
добавляет все элементы [%-*@3 submenu]. [%-*@3 enable] 
устанавливает статус этого элемента. 
Заметьте, что лямбда`-варианты (которые 
с Function) должны быть проименованы по 
разному, по причине перегрузочных 
проблем.&]
[s3; &]
[s4;%- &]
[s5;:Bar`:`:MenuSeparator`(`):%- [@(0.0.255) void]_[* MenuSeparator]()&]
[s5;:Bar`:`:MenuBreak`(`):%- [@(0.0.255) void]_[* MenuBreak]()&]
[s5;:Bar`:`:MenuGap`(int`):%- [@(0.0.255) void]_[* MenuGap]([@(0.0.255) int]_[*@3 size]_`=_[@3 8
])&]
[s5;:Bar`:`:MenuGapRight`(`):%- [@(0.0.255) void]_[* MenuGapRight]()&]
[s5;:Bar`:`:AddMenu`(Ctrl`&`):%- [@(0.0.255) void]_[* AddMenu]([_^Ctrl^ Ctrl][@(0.0.255) `&]_
[*@3 ctrl])&]
[s5;:Bar`:`:AddMenu`(Ctrl`&`,Size`):%- [@(0.0.255) void]_[* AddMenu]([_^Ctrl^ Ctrl][@(0.0.255) `&
]_[*@3 ctrl], [_^Size^ Size]_[*@3 sz])&]
[s5;:Bar`:`:AddMenu`(Ctrl`&`,int`,int`):%- [@(0.0.255) void]_[* AddMenu]([_^Ctrl^ Ctrl][@(0.0.255) `&
]_[*@3 ctrl], [@(0.0.255) int]_[*@3 cx], [@(0.0.255) int]_[*@3 cy]_`=_[@3 0])&]
[s5;:Upp`:`:Bar`:`:AddMenu`(bool`,const char`*`,const Upp`:`:Image`&`,const Upp`:`:Callback`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* AddMenu]([@(0.0.255) bool]_[*@3 enable], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 text], [@(0.0.255) const]_[_^Upp`:`:Image^ Upp`::Image][@(0.0.255) `&]_[*@3 imag
e], [@(0.0.255) const]_[_^Upp`:`:Callback^ Callback][@(0.0.255) `&]_[*@3 callback])&]
[s5;:Upp`:`:Bar`:`:AddMenu`(bool`,Upp`:`:KeyInfo`&`(`*`)`(`)`,const Upp`:`:Image`&`,const Upp`:`:Callback`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* AddMenu]([@(0.0.255) bool]_[*@3 enable], [_^Upp`:`:KeyInfo^ KeyInfo][@(0.0.255) `&
]_(`*[*@3 key])(), [@(0.0.255) const]_Upp`::Image[@(0.0.255) `&]_[*@3 image], 
[@(0.0.255) const]_Callback[@(0.0.255) `&]_[*@3 callback])&]
[s5;:Upp`:`:Bar`:`:AddMenu`(const char`*`,const Upp`:`:Image`&`,const Upp`:`:Callback`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* AddMenu]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 text], 
[@(0.0.255) const]_[_^Upp`:`:Image^ Upp`::Image][@(0.0.255) `&]_[*@3 image], 
[@(0.0.255) const]_[_^Upp`:`:Callback^ Callback][@(0.0.255) `&]_[*@3 callback])&]
[s5;:Upp`:`:Bar`:`:AddMenu`(const Upp`:`:String`&`,const Upp`:`:Image`&`,const Upp`:`:Callback`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* AddMenu]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_
[*@3 t], [@(0.0.255) const]_[_^Upp`:`:Image^ Upp`::Image][@(0.0.255) `&]_[*@3 m], 
[@(0.0.255) const]_[_^Upp`:`:Callback^ Callback][@(0.0.255) `&]_[*@3 c])&]
[s5;:Upp`:`:Bar`:`:AddMenu`(Upp`:`:KeyInfo`&`(`*`)`(`)`,const Upp`:`:Image`&`,const Upp`:`:Callback`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* AddMenu]([_^Upp`:`:KeyInfo^ KeyInfo][@(0.0.255) `&]_(`*[*@3 key])(),
 [@(0.0.255) const]_Upp`::Image[@(0.0.255) `&]_[*@3 m], [@(0.0.255) const]_Callback[@(0.0.255) `&
]_[*@3 c])&]
[s5;:Upp`:`:Bar`:`:AddMenu`(bool`,const char`*`,const Upp`:`:Image`&`,const Upp`:`:Function`<void`(`)`>`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* AddMenu]([@(0.0.255) bool]_[*@3 enable], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 text], [@(0.0.255) const]_[_^Upp`:`:Image^ Upp`::Image][@(0.0.255) `&]_[*@3 imag
e], [@(0.0.255) const]_[_^Upp`:`:Function^ Function]<[@(0.0.255) void]_()>`&_[*@3 fn])&]
[s5;:Upp`:`:Bar`:`:AddMenu`(bool`,Upp`:`:KeyInfo`&`(`*`)`(`)`,const Upp`:`:Image`&`,const Upp`:`:Function`<void`(`)`>`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* AddMenu]([@(0.0.255) bool]_[*@3 enable], [_^Upp`:`:KeyInfo^ KeyInfo][@(0.0.255) `&
]_(`*[*@3 key])(), [@(0.0.255) const]_Upp`::Image[@(0.0.255) `&]_[*@3 image], 
[@(0.0.255) const]_Function<[@(0.0.255) void]_()>`&_[*@3 fn])&]
[s5;:Upp`:`:Bar`:`:AddMenu`(const char`*`,const Upp`:`:Image`&`,const Upp`:`:Function`<void`(`)`>`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* AddMenu]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 text], 
[@(0.0.255) const]_[_^Upp`:`:Image^ Upp`::Image][@(0.0.255) `&]_[*@3 image], 
[@(0.0.255) const]_[_^Upp`:`:Function^ Function]<[@(0.0.255) void]_()>`&_[*@3 fn])&]
[s5;:Upp`:`:Bar`:`:AddMenu`(const Upp`:`:String`&`,const Upp`:`:Image`&`,const Upp`:`:Function`<void`(`)`>`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* AddMenu]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_
[*@3 t], [@(0.0.255) const]_[_^Upp`:`:Image^ Upp`::Image][@(0.0.255) `&]_[*@3 m], 
[@(0.0.255) const]_[_^Upp`:`:Function^ Function]<[@(0.0.255) void]_()>`&_[*@3 fn])&]
[s5;:Upp`:`:Bar`:`:AddMenu`(Upp`:`:KeyInfo`&`(`*`)`(`)`,const Upp`:`:Image`&`,const Upp`:`:Function`<void`(`)`>`&`):%- [_^Upp`:`:Bar`:`:Item^ I
tem][@(0.0.255) `&]_[* AddMenu]([_^Upp`:`:KeyInfo^ KeyInfo][@(0.0.255) `&]_(`*[*@3 key])(),
 [@(0.0.255) const]_Upp`::Image[@(0.0.255) `&]_[*@3 m], [@(0.0.255) const]_Function<[@(0.0.255) v
oid]_()>`&_[*@3 fn])&]
[s2; Эти методы эквивалентны методам 
без [* Menu][*/  ]в названии, но выполняют 
соответствующие операции только 
тогда, когда этот Bar является меню.&]
[s3; &]
[s4;%- &]
[s5;:Bar`:`:ToolSeparator`(`):%- [@(0.0.255) void]_[* ToolSeparator]()&]
[s5;:Bar`:`:ToolBreak`(`):%- [@(0.0.255) void]_[* ToolBreak]()&]
[s5;:Bar`:`:ToolGap`(int`):%- [@(0.0.255) void]_[* ToolGap]([@(0.0.255) int]_[*@3 size]_`=_[@3 8
])&]
[s5;:Bar`:`:ToolGapRight`(`):%- [@(0.0.255) void]_[* ToolGapRight]()&]
[s5;:Bar`:`:AddTool`(Ctrl`&`):%- [@(0.0.255) void]_[* AddTool]([_^Ctrl^ Ctrl][@(0.0.255) `&]_
[*@3 ctrl])&]
[s5;:Bar`:`:AddTool`(Ctrl`&`,Size`):%- [@(0.0.255) void]_[* AddTool]([_^Ctrl^ Ctrl][@(0.0.255) `&
]_[*@3 ctrl], [_^Size^ Size]_[*@3 sz])&]
[s5;:Bar`:`:AddTool`(Ctrl`&`,int`,int`):%- [@(0.0.255) void]_[* AddTool]([_^Ctrl^ Ctrl][@(0.0.255) `&
]_[*@3 ctrl], [@(0.0.255) int]_[*@3 cx], [@(0.0.255) int]_[*@3 cy]_`=_[@3 0])&]
[s2; Эти методы эквивалентны методам 
без [* Tool][*/  ]в названии, но выполняют 
соответствующие операции только 
тогда, когда этот Bar является тулбаром&]
[s0; &]
[s3; &]
[s4;%- &]
[s5;:Bar`:`:AddKey`(dword`,Callback`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* AddKey](
[_^dword^ dword]_[*@3 key], [_^Callback^ Callback]_[*@3 cb])&]
[s5;:Bar`:`:AddKey`(KeyInfo`&`(`*`)`(`)`,Callback`):%- [@(0.0.255) void]_[* AddKey]([_^KeyInfo^ K
eyInfo][@(0.0.255) `&]_(`*[*@3 key])(), Callback_[*@3 cb])&]
[s2; Эти варианты не добавляют элементов 
ни в меню, ни в тулбар, но добавляют 
клавиши`-акселераторы.&]
[s3; &]
[s4;%- &]
[s5;:Bar`:`:IsMenuBar`(`)const:%- [@(0.0.255) virtual] [@(0.0.255) bool]_[* IsMenuBar]()_[@(0.0.255) c
onst]&]
[s2; Возвращает true, если Bar является менюбаром.&]
[s3; &]
[s4;%- &]
[s5;:Bar`:`:IsToolBar`(`)const:%- [@(0.0.255) virtual] [@(0.0.255) bool]_[* IsToolBar]()_[@(0.0.255) c
onst]&]
[s2; Возвращает true, если Bar является тулбаром.&]
[s3; &]
[s4;%- &]
[s5;:Bar`:`:IsScanKeys`(`)const:%- [@(0.0.255) virtual] [@(0.0.255) bool]_[* IsScanKeys]()_
[@(0.0.255) const]&]
[s2; Возвращает true, если Bar сканирует только 
клавиши`-акселераторы.&]
[s3; &]
[s0; &]
[s0; &]
[s0;%- &]
[ {{10000@3 [s0; [*@(229)4 Структура Bar`::Item]]}}&]
[s3; &]
[s1;:Bar`:`:Item`:`:struct:%- [@(0.0.255)3 struct][3 _][*3 Item]&]
[s9; Эта структура используется для описания 
дополнительных свойств индивидуального 
элемента бруса (бара). Методы возвращают 
`*this, что позволяет делать сцепку вызовов.&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Список Публичных Членов]]}}&]
[s3; &]
[s5;:Bar`:`:Item`:`:Image`(const Image`&`):%- [_^Bar`:`:Item^ Bar`::Item][@(0.0.255) `&]_[* I
mage]([@(0.0.255) const]_[_^Image^ UPP`::Image][@(0.0.255) `&]_[*@3 img])&]
[s2; Устанавливает изображение, используемое 
с этим элементом.&]
[s3; &]
[s4;%- &]
[s5;:Bar`:`:Item`:`:Text`(const char`*`):%- [@(0.0.255) virtual] [_^Bar`:`:Item^ Item][@(0.0.255) `&
]_[* Text]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 text])&]
[s5;:Upp`:`:Bar`:`:Item`:`:Text`(const Upp`:`:String`&`):%- [@(0.0.255) virtual] 
[_^Upp`:`:Bar`:`:Item^ Item][@(0.0.255) `&]_[* Text]([@(0.0.255) const]_[_^Upp`:`:String^ S
tring][@(0.0.255) `&]_[*@3 text])&]
[s2; Устанавливат текст [%-*@3 text] элемента.&]
[s3; &]
[s4;%- &]
[s5;:Bar`:`:Item`:`:Key`(dword`):%- [@(0.0.255) virtual] [_^Bar`:`:Item^ Item][@(0.0.255) `&
]_[* Key]([_^dword^ dword]_[*@3 key])&]
[s2; Устанавливает клавишу акселератора 
[%-*@3 key]. У элементов может быть более 
одной клавиши`-акселератора (отображается 
только последняя, но активны все).&]
[s3; &]
[s4;%- &]
[s5;:Bar`:`:Item`:`:Repeat`(bool`):%- [@(0.0.255) virtual] [_^Bar`:`:Item^ Item][@(0.0.255) `&
]_[* Repeat]([@(0.0.255) bool]_[*@3 repeat]_`=_[@(0.0.255) true])&]
[s2; В режиме повтора (repeat mode), действие 
от кнопок ToolBar`'а вызываются повторно, 
когда над ними выжата кнопка мыши.&]
[s3; &]
[s4;%- &]
[s5;:Bar`:`:Item`:`:Check`(bool`):%- [@(0.0.255) virtual] [_^Bar`:`:Item^ Item][@(0.0.255) `&
]_[* Check]([@(0.0.255) bool]_[*@3 check])&]
[s2; Делает элемент `"checked`" (`"отмеченным`"). 
Используется с кнопками опций on/off 
или элементами меню.&]
[s3; &]
[s4;%- &]
[s5;:Bar`:`:Item`:`:Radio`(bool`):%- [@(0.0.255) virtual] [_^Bar`:`:Item^ Item][@(0.0.255) `&
]_[* Radio]([@(0.0.255) bool]_[*@3 check])&]
[s2; Делает элемент `"choosen`" (`"выбранным`"). 
Похоже на check, визуальный стиль напоминает 
Switch.&]
[s3; &]
[s4;%- &]
[s5;:Bar`:`:Item`:`:Enable`(bool`):%- [@(0.0.255) virtual] [_^Bar`:`:Item^ Item][@(0.0.255) `&
]_[* Enable]([@(0.0.255) bool]_[*@3 `_enable]_`=_[@(0.0.255) true])&]
[s2; Делает элемент активным (дефолтно 
он активен).&]
[s3; &]
[s4;%- &]
[s5;:Bar`:`:Item`:`:Bold`(bool`):%- [@(0.0.255) virtual] [_^Bar`:`:Item^ Item][@(0.0.255) `&
]_[* Bold]([@(0.0.255) bool]_[*@3 bold]_`=_[@(0.0.255) true])&]
[s2; Устанавливает полужирным текст элемента 
меню, в соответствии со значением 
[%-*@3 bold].&]
[s3; &]
[s4;%- &]
[s5;:Bar`:`:Item`:`:Tip`(const char`*`):%- [@(0.0.255) virtual] [_^Bar`:`:Item^ Item][@(0.0.255) `&
]_[* Tip]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 tip])&]
[s2; Добавляет подсказку`-тултип к элементу 
ToolBar`'а.&]
[s3; &]
[s4;%- &]
[s5;:Bar`:`:Item`:`:Help`(const char`*`):%- [@(0.0.255) virtual] [_^Bar`:`:Item^ Item][@(0.0.255) `&
]_[* Help]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 help])&]
[s2; Добавляет строку справки [%-*@3 help] к 
данному элементу.&]
[s3; &]
[s4;%- &]
[s5;:Bar`:`:Item`:`:Key`(KeyInfo`&`(`*`)`(`)`):%- [_^Bar`:`:Item^ Item][@(0.0.255) `&]_[* K
ey]([_^KeyInfo^ KeyInfo][@(0.0.255) `&]_(`*[*@3 key])())&]
[s2; Добавляет все клавиши из конфигурабильной 
инфо об акселераторе.&]
[s3; &]
[s0; ]]