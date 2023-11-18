topic "Класс ValueMap";
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
[ {{10000@(113.42.0) [s0; [*@7;4 Класс ValueMap]]}}&]
[s3;%- &]
[s1;:ValueMap`:`:class:%- [@(0.0.255)3 class][3 _][*3 ValueMap]&]
[s2; ValueMap `- это мап с парами ключ`-значение, 
где как ключ, так и значение имеют 
тип Value. Ключ должен быть типа rich Value 
м оператором `=`= и определением GetHashValue 
(в действительности, GetHashValue не нужно 
определять, если производительность 
некритична). Он полностью совместим 
с Rich Value.&]
[s2; &]
[s2; Всем парам ключ`-значение, добавленным 
в ValueMap, присваиваются индексы, в порядке 
их добавления, а элементы доступны 
через эти индексы.&]
[s2; &]
[s2; ValueMap совместим с ValueArray (также как 
Time совместимо с Date). При присваивании 
Value, содержащего ValueMap, к ValueArray`'ю ему 
присваивается ValueArray, который представляет 
значения из ValueMap. При присваивании 
ValueArray к ValueMap`'у создаётся мап, где 
ключами являются числа 0, 1, ... GetCount() 
`- 1, а значения содержат элементы с 
соответствующими индексами.&]
[s2; &]
[s2; Заметьте, что Value предоставляет методы 
operator`[`] для прямого доступа к элементам 
ValueArray`'я, содержащегося в Value, по текстовым 
ключам (если Value не содержит ValueMap или 
требуемый ключ, возвращается Void Value).&]
[s2; &]
[ {{10000F(128)G(128)@1 [s0; [* Список Публичных Методов]]}}&]
[s3;%- &]
[s5;:ValueMap`:`:ValueMap`(`):%- [* ValueMap]()&]
[s2; Дефолтный конструктор.&]
[s3;%- &]
[s4;%- &]
[s5;:ValueMap`:`:ValueMap`(const ValueMap`&`):%- [* ValueMap]([@(0.0.255) const]_[* ValueMa
p][@(0.0.255) `&]_[*@3 v])&]
[s2; Копи`-конструктор.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:ValueMap`:`:ValueMap`(const Upp`:`:ValueArray`&`):%- [* ValueMap]([@(0.0.255) c
onst]_[_^Upp`:`:ValueArray^ ValueArray][@(0.0.255) `&]_[*@3 va])&]
[s2; Создаёт ValueMap из ValueArray`'я. Элементы 
ValueArray`'я становятся значениями с ключами, 
равными индексам.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:ValueMap`:`:ValueMap`(Upp`:`:Index`<Upp`:`:Value`>`&`&`,Upp`:`:Vector`<Upp`:`:Value`>`&`&`):%- [* V
alueMap]([_^Upp`:`:Index^ Index]<[_^Upp`:`:Value^ Value]>`&`&_[*@3 k], 
[_^Upp`:`:Vector^ Vector]<[_^Upp`:`:Value^ Value]>`&`&_[*@3 v])&]
[s2; Создаёт ValueMap, пикуя Index и Vector Value`'ев 
(эффективно, но источник разрушается).&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:ValueMap`:`:ValueMap`(Upp`:`:VectorMap`<Upp`:`:Value`,Upp`:`:Value`>`&`&`):%- [* V
alueMap]([_^Upp`:`:VectorMap^ VectorMap]<[_^Upp`:`:Value^ Value], 
[_^Upp`:`:Value^ Value]>`&`&_[*@3 m])&]
[s2; Создаёт ValueMap, пикуя VectorMap (эффективно, 
но источник разрушается).&]
[s3; &]
[s4;%- &]
[s5;:ValueMap`:`:ValueMap`(const Index`<Value`>`&`,const Vector`<Value`>`&`,int`):%- [* V
alueMap]([@(0.0.255) const]_[_^Index^ Index]<[_^Value^ Value]>`&_[*@3 k], 
[@(0.0.255) const]_[_^Vector^ Vector]<[_^Value^ Value]>`&_[*@3 v], [@(0.0.255) int]_[*@3 deep
])&]
[s2; Создаёт ValueMap, глубоко копируя Index 
и Vector Value`'в.&]
[s3; &]
[s4;%- &]
[s5;:ValueMap`:`:ValueMap`(const VectorMap`<Value`,Value`>`&`,int`):%- [* ValueMap]([@(0.0.255) c
onst]_[_^VectorMap^ VectorMap]<[_^Value^ Value], [_^Value^ Value]>`&_[*@3 m], 
[@(0.0.255) int]_[*@3 deep])&]
[s2; Создаёт ValueMap путём глубокого копирования 
VectorMap`'а.&]
[s3; &]
[s4;%- &]
[s5;:ValueMap`:`:`~ValueMap`(`):%- [@(0.0.255) `~][* ValueMap]()&]
[s2; Деструктор.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:ValueMap`:`:ValueMap`(std`:`:initializer`_list`<std`:`:pair`<Upp`:`:Value`,Upp`:`:Value`>`>`):%- [* V
alueMap]([_^http`:`/`/en`.cppreference`.com`/w`/cpp`/utility`/initializer`_list^ st
d`::initializer`_list]<[_^http`:`/`/en`.cppreference`.com`/w`/cpp`/utility`/pair^ st
d`::pair]<[_^Upp`:`:Value^ Value], [_^Upp`:`:Value^ Value]>>_[*@3 init])&]
[s2; Инициализация C`+`+11.&]
[s3; &]
[s4;%- &]
[s5;:ValueMap`:`:operator`=`(const ValueMap`&`):%- [_^ValueMap^ ValueMap][@(0.0.255) `&]_
[* operator`=]([@(0.0.255) const]_[_^ValueMap^ ValueMap][@(0.0.255) `&]_[*@3 v])&]
[s2; Присваивание.&]
[s3; &]
[s4;%- &]
[s5;:ValueMap`:`:operator`=`(VectorMap`<Value`,Value`>rval`_`):%- [_^ValueMap^ ValueMap
][@(0.0.255) `&]_[* operator`=]([_^VectorMap^ VectorMap]<[_^Value^ Value], 
[_^Value^ Value]>_[@(0.0.255) rval`_]_[*@3 m])&]
[s2; Пик`-присваивание `- эффективно, источник 
разрушается.&]
[s3; &]
[s4;%- &]
[s5;:ValueMap`:`:operator Value`(`)const:%- [* operator_Value]()_[@(0.0.255) const]&]
[s2; Преобразование в Value.&]
[s3;%- &]
[s4;%- &]
[s5;:ValueMap`:`:ValueMap`(const Value`&`):%- [* ValueMap]([@(0.0.255) const]_[_^Value^ Val
ue][@(0.0.255) `&]_[*@3 src])&]
[s2; Преобразование из Value.&]
[s3; &]
[s4;%- &]
[s5;:ValueMap`:`:ValueMap`(const Nuller`&`):%- [* ValueMap]([@(0.0.255) const]_[_^Nuller^ N
uller][@(0.0.255) `&])&]
[s2; Присваивание Null. ValueMap считается Null, 
если в нём нет элементов.&]
[s3;%- &]
[s4;%- &]
[s5;:ValueMap`:`:IsNullInstance`(`)const:%- [@(0.0.255) bool]_[* IsNullInstance]()_[@(0.0.255) c
onst]&]
[s2; То же, что и GetCount() `=`= 0.&]
[s3;%- &]
[s4;%- &]
[s5;:ValueMap`:`:Clear`(`):%- [@(0.0.255) void]_[* Clear]()&]
[s2; Удаляет все элементы из ValueMap`'а.&]
[s3;%- &]
[s4;%- &]
[s5;:ValueMap`:`:GetCount`(`)const:%- [@(0.0.255) int]_[* GetCount]()_[@(0.0.255) const]&]
[s2; Возвращает число элементов в ValueMap`'е.&]
[s3;%- &]
[s4;%- &]
[s5;:ValueMap`:`:IsEmpty`(`)const:%- [@(0.0.255) bool]_[* IsEmpty]()_[@(0.0.255) const]&]
[s2; То же, что и GetCount() `=`= 0.&]
[s3;%- &]
[s4;%- &]
[s5;:ValueMap`:`:GetKey`(int`)const:%- [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[* G
etKey]([@(0.0.255) int]_[*@3 i])_[@(0.0.255) const]&]
[s2; Возвращает ключ по индексу [%-*@3 i].&]
[s3; &]
[s4;%- &]
[s5;:ValueMap`:`:GetValue`(int`)const:%- [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_
[* GetValue]([@(0.0.255) int]_[*@3 i])_[@(0.0.255) const]&]
[s2; Возвращает значение по индексу [%-*@3 i].&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:ValueMap`:`:Find`(const Upp`:`:Value`&`)const:%- [@(0.0.255) int]_[* Find]([@(0.0.255) c
onst]_[_^Upp`:`:Value^ Value][@(0.0.255) `&]_[*@3 key])_[@(0.0.255) const]&]
[s2; Возвращает индекс первого ключа 
[%-*@3 key], или `-1, если он отсутствует.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:ValueMap`:`:FindNext`(int`)const:%- [@(0.0.255) int]_[* FindNext]([@(0.0.255) i
nt]_[*@3 ii])_[@(0.0.255) const]&]
[s2; Возвращает индекс следующего ключа 
с таким же значением, как у ключа по 
индексу [%-*@3 ii], или `-1, если он отсутствует.&]
[s3; &]
[s4;%- &]
[s5;:ValueMap`:`:Add`(const Value`&`,const Value`&`):%- [@(0.0.255) void]_[* Add]([@(0.0.255) c
onst]_[_^Value^ Value][@(0.0.255) `&]_[*@3 key], [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&
]_[*@3 value])&]
[s5;:ValueMap`:`:Add`(const String`&`,const Value`&`):%- [@(0.0.255) void]_[* Add]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[*@3 key], [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&
]_[*@3 value])&]
[s5;:ValueMap`:`:Add`(const char`*`,const Value`&`):%- [@(0.0.255) void]_[* Add]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 key], [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 v
alue])&]
[s5;:ValueMap`:`:Add`(int`,const Value`&`):%- [@(0.0.255) void]_[* Add]([@(0.0.255) int]_[*@3 k
ey], [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 value])&]
[s5;:ValueMap`:`:Add`(Id`,const Value`&`):%- [@(0.0.255) void]_[* Add]([_^Id^ Id]_[*@3 key], 
[@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 value])&]
[s2;%- Добавляет новую пару ключ`-значение 
в ValueMap. ValueMap может содержать несколько 
одинаковых ключей. Примечание: требуется 
несколько перегрузок, чтобы Id и SqlId 
смогли работать с ValueMap`'ом.&]
[s3; &]
[s4;%- &]
[s5;:ValueMap`:`:operator`(`)`(const Value`&`,const Value`&`):%- [_^ValueMap^ ValueMap][@(0.0.255) `&
]_[* operator()]([@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 key], 
[@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 value])&]
[s5;:ValueMap`:`:operator`(`)`(const String`&`,const Value`&`):%- [_^ValueMap^ ValueMap
][@(0.0.255) `&]_[* operator()]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 key
], [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 value])&]
[s5;:ValueMap`:`:operator`(`)`(const char`*`,const Value`&`):%- [_^ValueMap^ ValueMap][@(0.0.255) `&
]_[* operator()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 key], [@(0.0.255) const]_[_^Value^ V
alue][@(0.0.255) `&]_[*@3 value])&]
[s5;:ValueMap`:`:operator`(`)`(int`,const Value`&`):%- [_^ValueMap^ ValueMap][@(0.0.255) `&
]_[* operator()]([@(0.0.255) int]_[*@3 key], [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&
]_[*@3 value])&]
[s5;:ValueMap`:`:operator`(`)`(Id`,const Value`&`):%- [_^ValueMap^ ValueMap][@(0.0.255) `&
]_[* operator()]([_^Id^ Id]_[*@3 key], [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 v
alue])&]
[s2;%- [%RU-RU То же, что и Add(][*@3 key], [*@3 value]).&]
[s3; &]
[s4;%- &]
[s5;:ValueMap`:`:Set`(const Value`&`,const Value`&`):%- [@(0.0.255) void]_[* Set]([@(0.0.255) c
onst]_[_^Value^ Value][@(0.0.255) `&]_[*@3 key], [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&
]_[*@3 value])&]
[s5;:ValueMap`:`:Set`(const String`&`,const Value`&`):%- [@(0.0.255) void]_[* Set]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[*@3 s], [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&
]_[*@3 value])&]
[s5;:ValueMap`:`:Set`(const char`*`,const Value`&`):%- [@(0.0.255) void]_[* Set]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 s], [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 va
lue])&]
[s5;:ValueMap`:`:Set`(int`,const Value`&`):%- [@(0.0.255) void]_[* Set]([@(0.0.255) int]_[*@3 q
], [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 value])&]
[s5;:ValueMap`:`:Set`(Id`,const Value`&`):%- [@(0.0.255) void]_[* Set]([_^Id^ Id]_[*@3 id], 
[@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 value])&]
[s2;%- [%RU-RU Если ключ ][*@3 key][%RU-RU  имеется в ValueMap`'е, 
ассоциированное с ним Valueзаменяется 
на ][*@3 value][%RU-RU . Если такой ключ отсутствует, 
то добавляется пара ключ`-значение. 
Если таких ключей несколько, заменяется 
только первый (у которого наименьший 
индекс). ]Примечание: требуется несколько 
перегрузок, чтобы Id и SqlId смогли работать 
с ValueMap`'ом.&]
[s3; &]
[s4;%- &]
[s5;:ValueMap`:`:SetAt`(int`,const Value`&`):%- [@(0.0.255) void]_[* SetAt]([@(0.0.255) int
]_[*@3 i], [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 v])&]
[s2; Заменяет значение по указанному 
индексу.&]
[s3; &]
[s4;%- &]
[s5;:ValueMap`:`:SetKey`(int`,const Value`&`):%- [@(0.0.255) void]_[* SetKey]([@(0.0.255) i
nt]_[*@3 i], [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 key])&]
[s5;:ValueMap`:`:SetKey`(int`,const String`&`):%- [@(0.0.255) void]_[* SetKey]([@(0.0.255) i
nt]_[*@3 i], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 key])&]
[s5;:ValueMap`:`:SetKey`(int`,const char`*`):%- [@(0.0.255) void]_[* SetKey]([@(0.0.255) in
t]_[*@3 i], [@(0.0.255) const]_[@(0.0.255) char`*]_[*@3 key])&]
[s5;:ValueMap`:`:SetKey`(int`,int`):%- [@(0.0.255) void]_[* SetKey]([@(0.0.255) int]_[*@3 i],
 [@(0.0.255) int]_[*@3 key])&]
[s5;:ValueMap`:`:SetKey`(int`,Id`):%- [@(0.0.255) void]_[* SetKey]([@(0.0.255) int]_[*@3 i], 
[_^Id^ Id]_[*@3 key])&]
[s2;%- [%RU-RU Заменяет ключ по указанному 
индексу. ]Примечание: требуется несколько 
перегрузок, чтобы Id и SqlId смогли работать 
с ValueMap`'ом.&]
[s3; &]
[s4;%- &]
[s5;:ValueMap`:`:RemoveKey`(const Value`&`):%- [@(0.0.255) int]_[* RemoveKey]([@(0.0.255) c
onst]_[_^Value^ Value][@(0.0.255) `&]_[*@3 key])&]
[s5;:ValueMap`:`:RemoveKey`(const String`&`):%- [@(0.0.255) int]_[* RemoveKey]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[*@3 key])&]
[s5;:ValueMap`:`:RemoveKey`(const char`*`):%- [@(0.0.255) int]_[* RemoveKey]([@(0.0.255) co
nst]_[@(0.0.255) char`*]_[*@3 key])&]
[s5;:ValueMap`:`:RemoveKey`(int`):%- [@(0.0.255) int]_[* RemoveKey]([@(0.0.255) int]_[*@3 key
])&]
[s5;:ValueMap`:`:RemoveKey`(Id`):%- [@(0.0.255) int]_[* RemoveKey]([_^Id^ Id]_[*@3 key])&]
[s2; Удаляет все пары ключ`-значение с 
заданным ключом. Медленная операция 
O(n).&]
[s3; &]
[s4;%- &]
[s5;:ValueMap`:`:Remove`(int`):%- [@(0.0.255) void]_[* Remove]([@(0.0.255) int]_[*@3 i])&]
[s2; Удаляет пару ключ`-значение по индексу 
[%-*@3 i].&]
[s3; &]
[s4;%- &]
[s5;:ValueMap`:`:GetKeys`(`)const:%- [@(0.0.255) const]_[_^Index^ Index]<[_^Value^ Value]>`&
_[* GetKeys]()_[@(0.0.255) const]&]
[s2; Возвращает ключи.&]
[s3;%- &]
[s4;%- &]
[s5;:ValueMap`:`:GetValues`(`)const:%- [_^ValueArray^ ValueArray]_[* GetValues]()_[@(0.0.255) c
onst]&]
[s2; Возвращает значения.&]
[s3;%- &]
[s4;%- &]
[s5;:ValueMap`:`:operator ValueArray`(`)const:%- [* operator_ValueArray]()_[@(0.0.255) co
nst]&]
[s2; То же, что и GetValues.&]
[s3;%- &]
[s4;%- &]
[s5;:ValueMap`:`:Pick`(`):%- [_^VectorMap^ VectorMap]<[_^Value^ Value], 
[_^Value^ Value]>_[* Pick]()&]
[s2; Пикует контент `- быстро, ValueMap очищается.&]
[s3;%- &]
[s4;%- &]
[s5;:ValueMap`:`:operator`[`]`(const Value`&`)const:%- [@(0.0.255) const]_[_^Value^ Value
][@(0.0.255) `&]_[* operator`[`]]([@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 key
])_[@(0.0.255) const]&]
[s5;:ValueMap`:`:operator`[`]`(const String`&`)const:%- [@(0.0.255) const]_[_^Value^ Valu
e][@(0.0.255) `&]_[* operator`[`]]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 s
])_[@(0.0.255) const]&]
[s5;:ValueMap`:`:operator`[`]`(const char`*`)const:%- [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&
]_[* operator`[`]]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 key])_[@(0.0.255) const]&]
[s5;:ValueMap`:`:operator`[`]`(const Id`&`)const:%- [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&
]_[* operator`[`]]([@(0.0.255) const]_[_^Id^ Id][@(0.0.255) `&]_[*@3 key])_[@(0.0.255) const]&]
[s5;:ValueMap`:`:operator`[`]`(const int`)const:%- [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&
]_[* operator`[`]]([@(0.0.255) const]_[@(0.0.255) int]_[*@3 key])_[@(0.0.255) const]&]
[s2;%- [%RU-RU Возвращает значение ключа ][*@3 key][%RU-RU . 
Если ][*@3 key][%RU-RU  отсутствует, возвращает 
ErrorValue (которое также void и Null Value). ]Примечание: 
требуется несколько перегрузок, чтобы 
Id и SqlId смогли работать с ValueMap`'ом.&]
[s3; &]
[s4;%- &]
[s5;:ValueMap`:`:GetAdd`(const Value`&`):%- [_^Value^ Value][@(0.0.255) `&]_[* GetAdd]([@(0.0.255) c
onst]_[_^Value^ Value][@(0.0.255) `&]_[*@3 key])&]
[s2; Возвращает ссылку на элемент по ключу 
[%-*@3 key], а если он отсутствует, создаёт 
Void Value.  [^topic`:`/`/Core`/srcdoc`/ValueReference`_ru`-ru^ Возвращённ
ая ссылка повреждается при любом 
дальнейшем использовании исходного 
Value].&]
[s3; &]
[s4;%- &]
[s5;:ValueMap`:`:operator`(`)`(const Value`&`):%- [_^Value^ Value][@(0.0.255) `&]_[* operat
or()]([@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 key])&]
[s5;:ValueMap`:`:operator`(`)`(const String`&`):%- [_^Value^ Value][@(0.0.255) `&]_[* opera
tor()]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 key])&]
[s5;:ValueMap`:`:operator`(`)`(const char`*`):%- [_^Value^ Value][@(0.0.255) `&]_[* operato
r()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 key])&]
[s5;:ValueMap`:`:operator`(`)`(const int`):%- [_^Value^ Value][@(0.0.255) `&]_[* operator()
]([@(0.0.255) const]_[@(0.0.255) int]_[*@3 key])&]
[s5;:ValueMap`:`:operator`(`)`(const Id`&`):%- [_^Value^ Value][@(0.0.255) `&]_[* operator(
)]([@(0.0.255) const]_[_^Id^ Id][@(0.0.255) `&]_[*@3 key])&]
[s2; То же, что и GetAdd([%-*@3 key]).&]
[s3; &]
[s4;%- &]
[s5;:ValueMap`:`:At`(int`):%- [_^Value^ Value][@(0.0.255) `&]_[* At]([@(0.0.255) int]_[*@3 i])&]
[s2; Возвращает ссылку на значение по 
индексу [%-*@3 i].  [^topic`:`/`/Core`/srcdoc`/ValueReference`_ru`-ru^ Возв
ращённая ссылка повреждается при 
любом дальнейшем использовании исходного 
Value].&]
[s3; &]
[s4;%- &]
[s5;:ValueMap`:`:GetAndClear`(const Value`&`):%- [_^Value^ Value]_[* GetAndClear]([@(0.0.255) c
onst]_[_^Value^ Value][@(0.0.255) `&]_[*@3 key])&]
[s2; Если [%-*@3 key] отсутствует, возвращает 
ErrorValue (которое Void и Null), иначе возвращает 
значение для ключа [%-*@3 key] и устанавливает 
его в void Value. Цель `- оптимизировать 
копирование в ситациях, когда комплексное 
значение (напр., ValueArray) предстоит изменить 
после его получения, не очищая его 
вValueMap`'е, и при его изменении нужно 
сделать глубокую копию.&]
[s3; &]
[s4;%- &]
[s5;:ValueMap`:`:GetHashValue`(`)const:%- [@(0.0.255) unsigned]_[* GetHashValue]()_[@(0.0.255) c
onst]&]
[s2; Возвращает хэшированное значение.&]
[s3;%- &]
[s4;%- &]
[s5;:ValueMap`:`:Serialize`(Stream`&`):%- [@(0.0.255) void]_[* Serialize]([_^Stream^ Stream
][@(0.0.255) `&]_[*@3 s])&]
[s2; Двоичная сериализация.&]
[s3; &]
[s4;%- &]
[s5;:ValueMap`:`:Jsonize`(JsonIO`&`):%- [@(0.0.255) void]_[* Jsonize]([_^JsonIO^ JsonIO][@(0.0.255) `&
]_[*@3 jio])&]
[s2; Поддержка JSON`-сериализации.&]
[s3; &]
[s4;%- &]
[s5;:ValueMap`:`:Xmlize`(XmlIO`&`):%- [@(0.0.255) void]_[* Xmlize]([_^XmlIO^ XmlIO][@(0.0.255) `&
]_[*@3 xio])&]
[s2; Поддержка XML`-сериализации.&]
[s3; &]
[s4;%- &]
[s5;:ValueMap`:`:ToString`(`)const:%- [_^String^ String]_[* ToString]()_[@(0.0.255) const]&]
[s2; Преобразование в текст.&]
[s3;%- &]
[s4;%- &]
[s5;:ValueMap`:`:operator`=`=`(const ValueMap`&`)const:%- [@(0.0.255) bool]_[* operator`=
`=]([@(0.0.255) const]_[_^ValueMap^ ValueMap][@(0.0.255) `&]_[*@3 v])_[@(0.0.255) const]&]
[s2; Сравнение на равенство, дваValueMap равны, 
если содержат одинаковые пары ключ`-значение
, расположенные [*/ в одинаковом порядке].&]
[s3; &]
[s4;%- &]
[s5;:ValueMap`:`:operator`!`=`(const ValueMap`&`)const:%- [@(0.0.255) bool]_[* operator!`=
]([@(0.0.255) const]_[_^ValueMap^ ValueMap][@(0.0.255) `&]_[*@3 v])_[@(0.0.255) const]&]
[s2; То же, что и !operator`=`=(v).&]
[s3; &]
[s4;%- &]
[s5;:ValueMap`:`:Compare`(const ValueMap`&`)const:%- [@(0.0.255) int]_[* Compare]([@(0.0.255) c
onst]_[_^ValueMap^ ValueMap][@(0.0.255) `&]_[*@3 b])_[@(0.0.255) const]&]
[s5;:ValueMap`:`:operator`<`=`(const ValueMap`&`)const:%- [@(0.0.255) bool]_[* operator<`=
]([@(0.0.255) const]_[_^ValueMap^ ValueMap][@(0.0.255) `&]_[*@3 x])_[@(0.0.255) const]&]
[s5;:ValueMap`:`:operator`>`=`(const ValueMap`&`)const:%- [@(0.0.255) bool]_[* operator>`=
]([@(0.0.255) const]_[_^ValueMap^ ValueMap][@(0.0.255) `&]_[*@3 x])_[@(0.0.255) const]&]
[s5;:ValueMap`:`:operator`<`(const ValueMap`&`)const:%- [@(0.0.255) bool]_[* operator<]([@(0.0.255) c
onst]_[_^ValueMap^ ValueMap][@(0.0.255) `&]_[*@3 x])_[@(0.0.255) const]&]
[s5;:ValueMap`:`:operator`>`(const ValueMap`&`)const:%- [@(0.0.255) bool]_[* operator>]([@(0.0.255) c
onst]_[_^ValueMap^ ValueMap][@(0.0.255) `&]_[*@3 x])_[@(0.0.255) const]&]
[s2; Сравнивает два ValueMap`'а. Выполняет 
упорядоченное (т.к. у ValueMap порядок 
пар ключ`-значение) сравнение пар 
ключ`-значение.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:ValueMap`:`:IsSame`(const Upp`:`:ValueMap`&`)const:%- [@(0.0.255) bool]_[* Is
Same]([@(0.0.255) const]_[_^Upp`:`:ValueMap^ ValueMap][@(0.0.255) `&]_[*@3 b])_[@(0.0.255) c
onst]&]
[s2; ValueMap упорядочивается и сравнивается, 
т.е., даже если значения ключей одинаковы, 
а порядок отличается, то ValueMap`'ы или 
Value`'и, содержащие их, считаются неравными 
(как с оператором `=`=). Этот метод предоставля
ет альтернативное сравнение Value`'в, 
которое предполагает, что содержимые 
ValueMap`'ы неупорядочены. Он рекурсивно 
обходит любые содержимые ValueMap`'ы, 
а также ValueArray`'и. Для иных типов Value, 
не являющихся ValueMap`'ом, используется 
обычный оператор сравнения `=`=.&]
[s3; &]
[s0; ]]