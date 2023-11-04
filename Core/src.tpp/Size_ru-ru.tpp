topic "Размер";
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
[ {{10000@(113.42.0) [s0; [*@7;4 Структура Size`_]]}}&]
[s3; &]
[s1;:noref:%- [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@4;3 T][@(0.0.255)3 >]&]
[s1;:Size`_`:`:struct:%- [@(0.0.255) struct]_[* Size`_]_:_[@(0.0.255) public]_[*@3 Moveable][@(0.0.255) <
]_[* Size`_][@(0.0.255) <][*@4 T][@(0.0.255) >]_>_&]
[s0;%- &]
[s0; [* Size`_] `- это генерная структура, описывающая 
объект двумерного размера. Горизонтальная 
мера представлена [*@(154.0.0) cx], вертикальная 
`- [*@(154.0.0) cy]. Обе меры типа [*@4 T]. &]
[s3; &]
[s0; &]
[s0; Чтобы получить специализированную 
версию [* Size`_], используйте что`-л. из 
ниже следующего:&]
[s0; &]
[s1;%- [* Size]&]
[s5;:Size`:`:typedef:%- [@(0.0.255) typedef]_[_^Size`_^ Size`_][@(0.0.255) <int>]_[* Size]&]
[s2; Size`_ с размерами [* int].&]
[s3; &]
[s4; &]
[s1;%- [* Size16]&]
[s5;:Size16`:`:typedef:%- [@(0.0.255) typedef]_[_^Size`_^ Size`_][@(0.0.255) <][_^int16^ int1
6][@(0.0.255) >]_[* Size16]&]
[s2; Size`_ с размерами [* int16].&]
[s3; &]
[s4;*@(154.0.0) &]
[s1;%- [* Size64]&]
[s5;:Size64`:`:typedef:%- [@(0.0.255) typedef]_[_^Size`_^ Size`_][@(0.0.255) <][_^int64^ int6
4][@(0.0.255) >]_[* Size64]&]
[s2; Size`_ с размерами [* int64].&]
[s3; &]
[s4;*@(154.0.0) &]
[s1;%- [* Sizef]&]
[s5;:Sizef`:`:typedef:%- [@(0.0.255) typedef]_[_^Size`_^ Size`_][@(0.0.255) <double>]_[* Size
f]&]
[s2; Size`_ с размерами[* double].&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Детали Конструктора]]}}&]
[s3;%- &]
[s5;:Size`_`:`:Size`_`(`):%- [* Size`_]()&]
[s2; Создаёт неинициализованный [* Size`_].&]
[s3; &]
[s4; &]
[s5;:Size`_`:`:Size`_`(const Value`&`):%- [* Size`_]([@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&
]_[*@3 src])&]
[s2; Дефолтный копи`-конструктор.&]
[s3; &]
[s4;%- &]
[s5;:Size`_`:`:Size`_`(T`,T`):%- [* Size`_]([*@4 T]_[*@3 cx], [*@4 T]_[*@3 cy])&]
[s2; Создаёт объект [* Size`_ ]и инициализует 
его горизонтальный размер в [%-*@3 cx][%- , 
а вертикальный в ][*@3 cy].&]
[s3; &]
[s4;%- &]
[s5;:Size`_`:`:Size`_`(const Size`_`<int`>`&`):%- [* Size`_]([@(0.0.255) const]_[* Size`_][@(0.0.255) <
int>`&]_[*@3 sz])&]
[s2; Создаёт объект [* Size`_ ]и инициализует 
его в [* Size] [*@3 sz].&]
[s3; &]
[s4;%- &]
[s5;:Size`_`:`:Size`_`(const Size`_`<short`>`&`):%- [* Size`_]([@(0.0.255) const]_[* Size`_
][@(0.0.255) <short>`&]_[*@3 sz])&]
[s2; Создаёт объект [* Size`_ ]и инициализует 
его в [* Size16] [*@3 sz].&]
[s3; &]
[s4;%- &]
[s5;:Size`_`:`:Size`_`(const Size`_`<double`>`&`):%- [* Size`_]([@(0.0.255) const]_[* Size`_
][@(0.0.255) <double>`&]_[*@3 sz])&]
[s2; Создаёт объект [* Size`_ ]и инициализует 
его в [* Sizef] [*@3 sz].&]
[s3; &]
[s4;%- &]
[s5;:Size`_`:`:Size`_`(const Size`_`<int64`>`&`):%- [* Size`_]([@(0.0.255) const]_[* Size`_
][@(0.0.255) <][_^int64^ int64][@(0.0.255) >`&]_[*@3 sz])&]
[s2; Создаёт объект [* Size`_ ]и инициализует 
его в [* Size64] [%-*@3 sz].&]
[s3; &]
[s4;%- &]
[s5;:Size`_`:`:Size`_`(const Point`_`<T`>`&`):%- [* Size`_]([@(0.0.255) const]_[_^Point`_^ P
oint`_][@(0.0.255) <][*@4 T][@(0.0.255) >`&]_[*@3 pt])&]
[s2; Создаёт объект [* Size`_ ]и инициализует 
его в [* Point] [%-*@3 sz].&]
[s3; &]
[s4;%- &]
[s5;:Size`_`:`:Size`_`(const Nuller`&`):%- [* Size`_]([@(0.0.255) const]_[_^Nuller^ Nuller][@(0.0.255) `&
])&]
[s2; Создаёт объект [* Size`_ ]и инициализует 
его в [* Null].&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Список Публичных Полей]]}}&]
[s3;%- &]
[s5;:Size`_`:`:cx:%- [*@4 T]_[* cx]&]
[s2; горизонтальный размер&]
[s3; &]
[s4;%- &]
[s5;:Size`_`:`:cy:%- [*@4 T]_[* cy]&]
[s2; вертикальный размер&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Список Методов]]}}&]
[s3;%- &]
[s5;:Size`_`:`:Clear`(`):%- [@(0.0.255) void]_[* Clear]()&]
[s2; Устанавливает оба размера в 0.&]
[s3; &]
[s4; &]
[s5;:Size`_`:`:IsEmpty`(`)const:%- [@(0.0.255) bool]_[* IsEmpty]()_[@(0.0.255) const]&]
[s2; Возвращает [* true], если хотя бы один 
размер равен 0, [* false] `- в противном случае.&]
[s3; &]
[s4;%- &]
[s5;:Size`_`:`:SetNull`(`):%- [@(0.0.255) void]_[* SetNull]()&]
[s2; Реинициализует объект в [* Null].&]
[s3; &]
[s4;%- &]
[s5;:Size`_`:`:IsNullInstance`(`)const:%- [@(0.0.255) bool]_[* IsNullInstance]()_[@(0.0.255) c
onst]&]
[s2; Проверяет, равен ли размер [* Null].&]
[s3; &]
[s4; &]
[s5;:Size`_`:`:GetHashValue`(`)const:%- [@(0.0.255) unsigned]_[* GetHashValue]()_[@(0.0.255) c
onst]&]
[s2; Возвращает хэш`-значение размера.&]
[s3; &]
[s4;%- &]
[s5;:Size`_`:`:ToString`(`)const:%- [_^String^ String]_[* ToString]()_[@(0.0.255) const]&]
[s2; Возвращает текстовое представление 
размера как [* String].&]
[s3; &]
[s4;%- &]
[s5;:Size`_`:`:operator`+`=`(Size`_`):%- [_^Size`_^ Size`_][@(0.0.255) `&]_[* operator`+`=](
[_^Size`_^ Size`_]_[*@3 p])&]
[s2; Увеличивает [* cx] на горизонтальную 
меру [*@3 p], а [* cy] на вертикальную меру 
[*@3 p].&]
[s3; &]
[s4;%- &]
[s5;:Size`_`:`:operator`+`=`(T`):%- [_^Size`_^ Size`_][@(0.0.255) `&]_[* operator`+`=]([*@4 T
]_[*@3 t])&]
[s2; Увеличивает обе меры на одинаковое 
значение [*@3 t].&]
[s3; &]
[s4;%- &]
[s5;:Size`_`:`:operator`-`=`(Size`_`):%- [_^Size`_^ Size`_][@(0.0.255) `&]_[* operator`-`=](
[_^Size`_^ Size`_]_[*@3 p])&]
[s2; Уменьшает [* cx ]на горизонтальную меру 
[*@3 p], а [* cy] на вертикальную меру [*@3 p].&]
[s3; &]
[s4;%- &]
[s5;:Size`_`:`:operator`-`=`(T`):%- [_^Size`_^ Size`_][@(0.0.255) `&]_[* operator`-`=]([*@4 T
]_[*@3 t])&]
[s2; Уменьшает оба размера на одинаковое 
значение [*@3 t].&]
[s3; &]
[s4;%- &]
[s5;:Size`_`:`:operator`*`=`(Size`_`):%- [_^Size`_^ Size`_][@(0.0.255) `&]_[* operator`*`=](
[_^Size`_^ Size`_]_[*@3 p])&]
[s2; Умножает [* cx ]на горизонтальную меру 
[*@3 p], а [* cy `- ]на вертикальную меру [*@3 p].&]
[s3;* &]
[s4;%- &]
[s5;:Size`_`:`:operator`*`=`(T`):%- [_^Size`_^ Size`_][@(0.0.255) `&]_[* operator`*`=]([*@4 T
]_[*@3 t])&]
[s2; Умножает обе меры на одинаковое значение 
[*@3 t].&]
[s3; &]
[s4;%- &]
[s5;:Size`_`:`:operator`/`=`(Size`_`):%- [_^Size`_^ Size`_][@(0.0.255) `&]_[* operator/`=](
[_^Size`_^ Size`_]_[*@3 p])&]
[s2; Делит [* cx ]на горизонтальную меру [*@3 p], 
а [* cy `- ]на вертикальную меру [*@3 p].&]
[s3; &]
[s4;%- &]
[s5;:Size`_`:`:operator`/`=`(T`):%- [_^Size`_^ Size`_][@(0.0.255) `&]_[* operator/`=]([*@4 T]_
[*@3 t])&]
[s2; Делит обе меры на одинаковое значение 
[*@3 t].&]
[s3; &]
[s4;%- &]
[s5;:Size`_`:`:operator`<`<`=`(int`):%- [_^Size`_^ Size`_][@(0.0.255) `&]_[* operator<<`=](
[@(0.0.255) int]_[*@3 sh])&]
[s2; Сдвигает обе меры влево на [*@3 sh].&]
[s3; &]
[s4;%- &]
[s5;:Size`_`:`:operator`>`>`=`(int`):%- [_^Size`_^ Size`_][@(0.0.255) `&]_[* operator>>`=](
[@(0.0.255) int]_[*@3 sh])&]
[s2; Сдвигает обе меры вправо на [*@3 sh].&]
[s3; &]
[s4;%- &]
[s5;:Size`_`:`:operator`+`+`(`):%- [_^Size`_^ Size`_][@(0.0.255) `&]_[* operator`+`+]()&]
[s2; Инкрементирует обе меры.&]
[s3; &]
[s4;%- &]
[s5;:Size`_`:`:operator`-`-`(`):%- [_^Size`_^ Size`_][@(0.0.255) `&]_[* operator`-`-]()&]
[s2; Декрементирует обе меры.&]
[s3; &]
[s4;%- &]
[s5;:Size`_`:`:operator Value`(`)const:%- [* operator_Value]()_[@(0.0.255) const]&]
[s2; Возвращает преобразование размера 
в [* Value].&]
[s3; &]
[s4;%- &]
[s5;:Size`_`:`:Serialize`(Stream`&`):%- [@(0.0.255) void]_[* Serialize]([_^Stream^ Stream][@(0.0.255) `&
]_[*@3 s])&]
[s2; Сериализует размер в поток [*@(141.42.0) s].&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Френд`-список]]}}&]
[s3; &]
[s5;:`:`:Size`_`:`:operator`+`(`:`:Size`_`):%- [@(0.0.255) friend][@(64) _]Size`_[@(64) _]o
perator`+[@(64) (]Size`_[@(64) _][@3 s][@(64) )]&]
[s2; Возвращает неизменённый размер [%-*@3 s].&]
[s3; &]
[s4; &]
[s5;:`:`:Size`_`:`:operator`-`(`:`:Size`_`):%- [@(0.0.255) friend][@(64) _]Size`_[@(64) _]o
perator`-[@(64) (]Size`_[@(64) _][@3 s][@(64) )]&]
[s2; Возвращает отрицательный размер 
[%-*@3 s].&]
[s3; &]
[s4; &]
[s5;:`:`:Size`_`:`:operator`+`(`:`:Size`_`,`:`:Size`_`):%- [@(0.0.255) friend][@(64) _]Si
ze`_[@(64) _]operator`+[@(64) (]Size`_[@(64) _][@3 a][@(64) , ]Size`_[@(64) _][@3 b][@(64) )]&]
[s5;:`:`:Size`_`:`:operator`+`(`:`:Size`_`,T`):%- [@(0.0.255) friend][@(64) _]Size`_[@(64) _
]operator`+[@(64) (]Size`_[@(64) _][@3 a][@(64) , ][@4 T][@(64) _][@3 t][@(64) )]&]
[s5;:`:`:Size`_`:`:operator`+`(T`,`:`:Size`_`):%- [@(0.0.255) friend][@(64) _]Size`_[@(64) _
]operator`+[@(64) (][@4 T][@(64) _][@3 t][@(64) , ]Size`_[@(64) _][@3 b][@(64) )]&]
[s2; Возвращает итог сложения двух значений 
Size`_ или Size`_ с единым значением.&]
[s3; &]
[s4; &]
[s5;:`:`:Size`_`:`:operator`-`(`:`:Size`_`,`:`:Size`_`):%- [@(0.0.255) friend][@(64) _]Si
ze`_[@(64) _]operator`-[@(64) (]Size`_[@(64) _][@3 a][@(64) , ]Size`_[@(64) _][@3 b][@(64) )]&]
[s5;:`:`:Size`_`:`:operator`-`(`:`:Size`_`,T`):%- [@(0.0.255) friend][@(64) _]Size`_[@(64) _
]operator`-[@(64) (]Size`_[@(64) _][@3 a][@(64) , ][@4 T][@(64) _][@3 t][@(64) )]&]
[s5;:`:`:Size`_`:`:operator`-`(T`,`:`:Size`_`):%- [@(0.0.255) friend][@(64) _]Size`_[@(64) _
]operator`-[@(64) (][@4 T][@(64) _][@3 t][@(64) , ]Size`_[@(64) _][@3 b][@(64) )]&]
[s2; Возвращает итог вычитания двух значений 
Size`_ или Size`_ с единым значением.&]
[s3; &]
[s4; &]
[s5;:`:`:Size`_`:`:operator`*`(`:`:Size`_`,`:`:Size`_`):%- [@(0.0.255) friend][@(64) _]Si
ze`_[@(64) _]operator`*[@(64) (]Size`_[@(64) _][@3 a][@(64) , ]Size`_[@(64) _][@3 b][@(64) )]&]
[s5;:`:`:Size`_`:`:operator`*`(`:`:Size`_`,T`):%- [@(0.0.255) friend][@(64) _]Size`_[@(64) _
]operator`*[@(64) (]Size`_[@(64) _][@3 a][@(64) , ][@4 T][@(64) _][@3 b][@(64) )]&]
[s5;:`:`:Size`_`:`:operator`*`(T`,`:`:Size`_`):%- [@(0.0.255) friend][@(64) _]Size`_[@(64) _
]operator`*[@(64) (][@4 T][@(64) _][@3 a][@(64) , ]Size`_[@(64) _][@3 b][@(64) )]&]
[s2; Возвращает итог умножения двух значений 
Size`_ или  Size`_ с единым значением.&]
[s3; &]
[s4; &]
[s5;:`:`:Size`_`:`:operator`/`(`:`:Size`_`,`:`:Size`_`):%- [@(0.0.255) friend][@(64) _]Si
ze`_[@(64) _]operator/[@(64) (]Size`_[@(64) _][@3 a][@(64) , ]Size`_[@(64) _][@3 b][@(64) )]&]
[s5;:`:`:Size`_`:`:operator`/`(`:`:Size`_`,T`):%- [@(0.0.255) friend][@(64) _]Size`_[@(64) _
]operator/[@(64) (]Size`_[@(64) _][@3 a][@(64) , ][@4 T][@(64) _][@3 b][@(64) )]&]
[s2; Возвращает итог деления двух значений 
Size`_ или Size`_ with single value.&]
[s3; &]
[s4; &]
[s5;:`:`:Size`_`:`:operator`<`<`(`:`:Size`_`,int`):%- [@(0.0.255) friend][@(64) _]Size`_[@(64) _
]operator<<[@(64) (]Size`_[@(64) _][@3 a][@(64) , ][@(0.0.255) int][@(64) _][@3 sh][@(64) )]&]
[s2; Возвращает итог левого сдвига [*@3 a] 
на [*@3 sh].&]
[s3; &]
[s4; &]
[s5;:`:`:Size`_`:`:operator`>`>`(`:`:Size`_`,int`):%- [@(0.0.255) friend][@(64) _]Size`_[@(64) _
]operator>>[@(64) (]Size`_[@(64) _][@3 a][@(64) , ][@(0.0.255) int][@(64) _][@3 sh][@(64) )]&]
[s2; Возвращает итог правого сдвига [*@3 a] 
на [*@3 sh].&]
[s3; &]
[s4; &]
[s5;:`:`:Size`_`:`:operator`=`=`(`:`:Size`_`,`:`:Size`_`):%- [@(0.0.255) friend][@(64) _][@(0.0.255) b
ool][@(64) _]operator`=`=[@(64) (]Size`_[@(64) _][@3 a][@(64) , ]Size`_[@(64) _][@3 b][@(64) )]&]
[s2; Возвращает [* true], если у [*@3 a ]и [*@3 b ]соответствен
ные размеры равны, [* false ]иначе.&]
[s3; &]
[s4; &]
[s5;:`:`:Size`_`:`:operator`!`=`(`:`:Size`_`,`:`:Size`_`):%- [@(0.0.255) friend][@(64) _][@(0.0.255) b
ool][@(64) _]operator!`=[@(64) (]Size`_[@(64) _][@3 a][@(64) , ]Size`_[@(64) _][@3 b][@(64) )]&]
[s2; Возвращает [* true], если у [*@3 a ]и [*@3 b ]aкак 
мнимум одна совпадающая мера, [* false 
]иначе.&]
[s3; &]
[s4; &]
[s5;:`:`:Size`_`:`:min`(`:`:Size`_`,`:`:Size`_`):%- [@(0.0.255) friend][@(64) _]Size`_[@(64) _
]min[@(64) (]Size`_[@(64) _][@3 a][@(64) , ]Size`_[@(64) _][@3 b][@(64) )]&]
[s2;%- [%RU-RU Возвращает ][%RU-RU* Size`_][%RU-RU  с размером, 
равным минимуму соответствующего 
размера между ][*@3 a][@3  ]и[%RU-RU  ][*@3 b].&]
[s3;%- &]
[s4;%- &]
[s5;:`:`:Size`_`:`:max`(`:`:Size`_`,`:`:Size`_`):%- [@(0.0.255) friend][@(64) _]Size`_[@(64) _
]max[@(64) (]Size`_[@(64) _][@3 a][@(64) , ]Size`_[@(64) _][@3 b][@(64) )]&]
[s2;%- [%RU-RU Возвращает ][%RU-RU* Size`_][%RU-RU   с размером, 
равным максимуму соответствующего 
размера между ][*@3 a][@3  ]и[%RU-RU  ][*@3 b].&]
[s3;%- &]
[s4; &]
[s5;:`:`:Size`_`:`:Nvl`(`:`:Size`_`,`:`:Size`_`):%- [@(0.0.255) friend][@(64) _]Size`_[@(64) _
]Nvl[@(64) (]Size`_[@(64) _][@3 a][@(64) , ]Size`_[@(64) _][@3 b][@(64) )]&]
[s2; Возвращает [%-*@3 b],если [%-*@3 a] равно Null, 
[%-*@3 a] иначе.&]
[s3; &]
[s4; &]
[s5;:`:`:Size`_`:`:ScalarProduct`(`:`:Size`_`,`:`:Size`_`):%- [@(0.0.255) friend][@(64) _
][@4 T][@(64) _]ScalarProduct[@(64) (]Size`_[@(64) _][@3 a][@(64) , ]Size`_[@(64) _][@3 b][@(64) )]&]
[s2; Возвращает скалярную производную 
(scalar product) между [%-*@3 a] и [%-*@3 b].&]
[s3; &]
[s4; &]
[s5;:`:`:Size`_`:`:VectorProduct`(`:`:Size`_`,`:`:Size`_`):%- [@(0.0.255) friend][@(64) _
][@4 T][@(64) _]VectorProduct[@(64) (]Size`_[@(64) _][@3 a][@(64) , ]Size`_[@(64) _][@3 b][@(64) )]&]
[s2; Возвращает векторную производную 
(vectorial product) между [%-*@3 a] и [%-*@3 b].&]
[s3; &]
[s4; &]
[s5;:`:`:Size`_`:`:Squared`(`:`:Size`_`):%- [@(0.0.255) friend][@(64) _][@4 T][@(64) _]Square
d[@(64) (]Size`_[@(64) _][@3 a][@(64) )]&]
[s2; Возвращает [* Size`_] с [* cx,] равным квадрату 
[%-*@3 a][%-* .cx,][%-  и ][%-* cy,][%-  равным квадрату 
][%-*@3 a][%-* .cy].&]
[s3; &]
[s4; &]
[s5;:`:`:Size`_`:`:Length`(`:`:Size`_`):%- [@(0.0.255) friend][@(64) _][@(0.0.255) double][@(64) _
]Length[@(64) (]Size`_[@(64) _][@3 a][@(64) )]&]
[s2; Возвращает гипотенузу треугольника, 
описанного [%-*@3 a].&]
[s3; &]
[s0; ]]