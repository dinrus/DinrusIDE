topic "Класс Vector";
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
[ {{10000@(113.42.0) [s0; [*@7;4 Класс Vector]]}}&]
[s3; &]
[s1;:noref:%- [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@4;3 T][@(0.0.255)3 >]&]
[s1;:Vector`:`:class:%- [@(0.0.255) class]_[* Vector]_:_[@(0.0.255) public]_[*@3 MoveableAndDe
epCopyOption][@(0.0.255) <]_[* Vector][@(0.0.255) <][*@4 T][@(0.0.255) >]_>&]
[s8; [*C@4 T]-|Тип элементов, сохраняемых в 
Vector`'е. Требуется, чтобы T был [*/^topic`:`/`/Core`/srcdoc`/Moveable`_ru`-ru^ д
вижимый][*/  ]и имел в наличии либо [*/^topic`:`/`/Core`/srcdoc`/pick`_`_ru`-ru^ к
онструктор глубокой копии], либо [*/^topic`:`/`/Core`/srcdoc`/pick`_`_ru`-ru^ п
ик`-конструктор], либо [*/ дефолтный 
конструктор].&]
[s9; Это самый эффективный `"флейвор`" 
базового контейнера случайного доступа. 
Его фичи (`"фишки`") происходят от того 
факта, что он реализован как простой 
вектор элементов в духе языка Си.&]
[s9; Неудобство Vector`'а состоит в общем 
требовании к элементам, находящимся 
в нём, `- они должны быть [*/^topic`:`/`/Core`/srcdoc`/Moveable`_ru`-ru^ д
вижимыми]. Другое неудобство в том, 
что многие операции повреждают ссылки 
(то есть ссылки и указатели  C`+`+)  на 
элементы.&]
[s9; Подобно любому другому контейнеру 
NTL, Vector является типом [*/^topic`:`/`/Core`/srcdoc`/Moveable`_ru`-ru^ д
вижимый][*/  ]с семантикой [*/^topic`:`/`/Core`/srcdoc`/pick`_`_ru`-ru^ п
ик и необязательная глубокая копия] 
. Вызывать методы `"пикнутого`" Vector`'а 
является логической ошибкой, за исключением
&]
[s0;%- &]
[s0;%- void_operator`=([@(0.128.128) pick`_]_[_^Vector^ Vector]`&_[@3 v])&]
[s0; void operator<<`=(const Vector`& [@3 v]) [/ (defined using DeepCopyOptionTemplate)]&]
[s0;%- void_Clear()&]
[s0;%- bool_IsPicked()_const&]
[s0; &]
[s0; Необязательная глубокая копия реализована 
через макрос DeepCopyOptionTemplate.&]
[s0; &]
[s0; Vector предоставляет доступ к внутреннему 
вектору элементов в духе Си (либо 
через Begin, либо даже посредством прясого 
каста (переброса в тип из типа)) и может 
использоваться в качестве буфера 
для Си`-подобных функций, хотя в таких 
случаях более удобно использовать 
класс [^topic`:`/`/Core`/src`/Buffer`_ru`-ru^ Buffer].&]
[s3;^topic`:`/`/Core`/src`/Buffer`$en`-us^ &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Детали Конструктора]]}}&]
[s3;%- &]
[s5;:Vector`:`:Vector`(`):%- [* Vector]()&]
[s2; Дефолтный конструктор. Конструирует 
пустой Vector.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Vector`:`:Vector`(int`):%- [@(0.0.255) explicit]_[* Vector]([@(0.0.255) int]_[*@3 n
])&]
[s2; Создаёт Vector из [%-*@3 n] дефолтно сконструирован
ных элементов.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Vector`:`:Vector`(int`,const T`&`):%- [@(0.0.255) explicit]_[* Vector]([@(0.0.255) i
nt]_[*@3 n], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 init])&]
[s2; Создаёт Vector из [%-*@3 n] eэлементов, копи`-сконстр
уированных как [%-*@3 init].&]
[s3; &]
[s4;%- &]
[s5;:Vector`:`:Vector`(pick`_ Vector`&`):%- [* Vector]([@(0.128.128) pick`_]_[* Vector][@(0.0.255) `&
]_[*@3 v])&]
[s2; Пик`-конструктор. Делает трансфер 
исходного Vector`'а за незначительное 
константное время, но при пикинге 
разрушает его.&]
[s7; v-|Исходный Vector.&]
[s3; &]
[s4;%- &]
[s5;:Vector`:`:Vector`(const Vector`&`,int`):%- [* Vector]([@(0.0.255) const]_[* Vector][@(0.0.255) `&
]_[*@3 v], [@(0.0.255) int])&]
[s2; Необязательный конструктор глубокой 
копии.&]
[s6; Требуется, чтобы у T был конструктор 
глубокой копии или необязательный 
конструктор глубокой копии.&]
[s2; [*C@3 v]-|Исходный Vector. [%-*@3 v].&]
[s3; &]
[s4;%- &]
[s5;:Vector`:`:`~Vector`(`):%- [@(0.0.255) `~][* Vector]()&]
[s2; Деструктор. Вызывает деструкторы 
всех элементов в Vector`'е.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Vector`:`:Vector`(std`:`:initializer`_list`<T`>`):%- [* Vector]([_^http`:`/`/en`.cppreference`.com`/w`/cpp`/utility`/initializer`_list^ s
td`::initializer`_list]<[*@4 T]>_[*@3 init])&]
[s2; Инициализация C`+`+ 11.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Список Публичных Членов]]}}&]
[s3;%- &]
[s5;:Vector`:`:Add`(`):%- [*@4 T][@(0.0.255) `&]_[* Add]()&]
[s2; Добавляет новый дефолтно сконструированный
 элемент в Vector.&]
[s6; Требуется, чтобы у T был дефолтный 
конструктор.&]
[s6; Повреждает итераторы и ссылки на 
Vector.&]
[s7; [*/ Возвратное значение]-|Ссылка на вновь 
добавленный, дефолтно сконструированный 
элемент.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Vector`:`:Create`(Args`.`.`.args`):%- [@(0.0.255) template]_<[@(0.0.255) clas
s...]_[*@4 Args]>_[*@4 T][@(0.0.255) `&]_[* Create]([*@4 Args][@(0.0.255) ...]_args)&]
[s2; Добавляет новый элемент в Vector, используя 
args как параметры конструктора.&]
[s3;%- &]
[s4;%- &]
[s5;:Vector`:`:Add`(const T`&`):%- [*@4 T][@(0.0.255) `&]_[* Add]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[*@3 x])&]
[s2; Добавляет новый элемент с указанным 
значением в Vector.&]
[s6; Требуется, чтобы у T имелся конструктор 
глубокой копии.&]
[s6; Повреждает итераторы и сылки на Vector.&]
[s7; [*C@3 x]-|Значение, копируемое во вновь 
создаваемый элемент.&]
[s7; [*/ Возвратное значение]-|Ссылка на новый 
элемент в Vector`'е.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Vector`:`:Add`(T`&`&`):%- [*@4 T][@(0.0.255) `&]_[* Add]([*@4 T][@(0.0.255) `&`&]_[*@3 x
])&]
[s2; Добавляет новый элемент в Vector и пикует 
значение его параметра. &]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Vector`:`:Create`(Args`&`&`.`.`.args`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass...]_[*@4 Args]>_[*@4 T][@(0.0.255) `&]_[* Create]([*@4 Args][@(0.0.255) `&`&...]_args)&]
[s2; Создаёт и добавляет новый элемент 
в Array. [%-*@3 args] передаются в конструктор.&]
[s3; &]
[s4;%- &]
[s5;:Vector`:`:AddN`(int`):%- [@(0.0.255) void]_[* AddN]([@(0.0.255) int]_[*@3 n])&]
[s2; Добавляет указанное число дефолтно 
сконструированных элементов в Vector.&]
[s6; Требуется, чтобы у T имелся конструктор 
глубокой копии.&]
[s6; Повреждает итераторы и сылки на Vector&]
[s7; [*C@3 n]-|Число добавляемых элементов&]
[s3;%- &]
[s4;%- &]
[s5;:Vector`:`:operator`[`]`(int`)const:%- [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[* opera
tor`[`]]([@(0.0.255) int]_[*@3 i])_[@(0.0.255) const]&]
[s2; Возвращает ссылку на элемент по указанной 
позиции.&]
[s7; [*C@3 i]-|Позиция элемента.&]
[s7; [*/ Возвратное значение]-|Константаная 
ссылка на элемент.&]
[s3;%- &]
[s4;%- &]
[s5;:Vector`:`:operator`[`]`(int`):%- [*@4 T][@(0.0.255) `&]_[* operator`[`]]([@(0.0.255) int
]_[*@3 i])&]
[s2; Возвращает ссылку на элемент по указанной 
позиции.&]
[s7; [*C@3 i]-|Позиция элемента.&]
[s7; [*/ Возвратное значение]-|Ссылка на элемент.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Vector`:`:Get`(int`,const T`&`)const:%- [@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[* Get]([@(0.0.255) int]_[*@3 i], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 def])_[@(0.0.255) c
onst]&]
[s5;:Upp`:`:Vector`:`:Get`(int`,T`&`):%- [*@4 T][@(0.0.255) `&]_[* Get]([@(0.0.255) int]_[*@3 i
], [*@4 T][@(0.0.255) `&]_[*@3 def])&]
[s2; If [%-*@3 i] is valid index (it is >`= 0 and < GetCount()), returns 
the reference to the element at [%-*@3 i], otherwiser returns [%-*@3 def].&]
[s3; &]
[s4;%- &]
[s5;:Vector`:`:GetCount`(`)const:%- [@(0.0.255) int]_[* GetCount]()_[@(0.0.255) const]&]
[s2; Return the number of elements in Vector.&]
[s7; [*/ Возвратное значение]-|Actual number of elements.&]
[s3;%- &]
[s4;%- &]
[s5;:Vector`:`:IsEmpty`(`)const:%- [@(0.0.255) bool]_[* IsEmpty]()_[@(0.0.255) const]&]
[s2; Tests whether Vector  is empty. То же, что и GetCount() 
`=`= 0.&]
[s7; [*/ Возвратное значение]-|true if Vector is empty, 
false otherwise.&]
[s3;%- &]
[s4;%- &]
[s5;:Vector`:`:Trim`(int`):%- [@(0.0.255) void]_[* Trim]([@(0.0.255) int]_[*@3 n])&]
[s2; Reduces number of elements in Vector to specified number. Required 
number must be less than or equal to actual number of elements 
in Vector. Capacity of Vector is however unchanged.&]
[s6; Invalidates iterators to Vector.&]
[s7; [*C@3 n]-|Required number of elements.&]
[s3;%- &]
[s4;%- &]
[s5;:Vector`:`:SetCount`(int`):%- [@(0.0.255) void]_[* SetCount]([@(0.0.255) int]_[*@3 n])&]
[s2; Changes count of elements in Vector to specified value. If required 
number of elements is greater than actual number, newly added 
elements are default constructed. If Vector has to increase capacity, 
the new capacity will exactly match required number of elements 
(unlike [* SetCountR]).&]
[s6; Requires T to have default constructor.&]
[s6; Invalidates iterators and references to Vector.&]
[s7; [*C@3 n]-|Required number of elements.&]
[s3;%- &]
[s4;%- &]
[s5;:Vector`:`:SetCount`(int`,const T`&`):%- [@(0.0.255) void]_[* SetCount]([@(0.0.255) int
]_[*@3 n], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 init])&]
[s2; Changes count of elements in Vector to specified value. If required 
number of elements is greater than actual number, newly added 
elements are initialized to specified value using copy constructor. 
If Vector has to increase capacity, the new capacity will exactly 
match required number of elements (unlike [* SetCountR]).&]
[s6; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators and references to Vector.&]
[s7; [*C@3 n]-|Required number of elements.&]
[s7; [*C@3 init]-|Initialization value of newly added elements.&]
[s3;%- &]
[s4;%- &]
[s5;:Vector`:`:SetCountR`(int`):%- [@(0.0.255) void]_[* SetCountR]([@(0.0.255) int]_[*@3 n])&]
[s2; Changes count of elements in Vector to specified value. If required 
number of elements is greater than actual number, newly added 
elements are default constructed. If Vector has to increase capacity, 
the new capacity will be greater than required number of elements 
(unlike [* SetCount]) to allow adding other elements without further 
increasing capacity.&]
[s6; Requires T to have default constructor.&]
[s6; Invalidates iterators and references to Vector.&]
[s7; [*C@3 n]-|Required number of elements.&]
[s3;%- &]
[s4;%- &]
[s5;:Vector`:`:SetCountR`(int`,const T`&`):%- [@(0.0.255) void]_[* SetCountR]([@(0.0.255) i
nt]_[*@3 n], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 init])&]
[s2; Changes count of elements in Vector to specified value. If required 
number of elements is greater than actual number, newly added 
elements are initialized to specified value using copy constructor. 
If Vector has to increase capacity, the new capacity will be 
greater than required number of elements (unlike [* SetCount]) 
to allow adding other elements without further increasing capacity.&]
[s6; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators and references to Vector.&]
[s7; [*C@3 n]-|Required number of elements.&]
[s7; [*C@3 init]-|Initialization value of newly added elements.&]
[s3;%- &]
[s4;%- &]
[s5;:Vector`:`:Clear`(`):%- [@(0.0.255) void]_[* Clear]()&]
[s2; Removes all elements from Vector. Capacity is also cleared to 
zero.&]
[s6; Invalidates iterators and references to Vector.&]
[s3;%- &]
[s4;%- &]
[s5;:Vector`:`:At`(int`):%- [*@4 T][@(0.0.255) `&]_[* At]([@(0.0.255) int]_[*@3 i])&]
[s2; If specified position is lower than number of elements in Vector 
([* i < GetCount()]), returns reference to element at specified 
position. Otherwise increases number of elements in Vector to 
[* i `+ 1]. Newly added elements are default constructed. If Vector 
has to increase capacity, the new capacity will be greater than 
required number of elements to allow adding other elements without 
further increasing capacity.&]
[s6; Requires T to have default constructor.&]
[s6; Invalidates iterators and references to Vector.&]
[s7; [*C@3 i]-|Position of required element.&]
[s7; [*/ Возвратное значение]-|Reference to required 
element.&]
[s3;%- &]
[s4;%- &]
[s5;:Vector`:`:At`(int`,const T`&`):%- [*@4 T][@(0.0.255) `&]_[* At]([@(0.0.255) int]_[*@3 i], 
[@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 x])&]
[s2; If specified position is lower than number of elements in Vector 
([* i < GetCount()]), returns reference to element at specified 
position. Otherwise increases number of elements in Vector to 
[* i `+ 1]. Newly added elements are copy constructed from [* x]. 
If Vector has to increase capacity, the new capacity will be 
greater than required number of elements to allow adding other 
elements without further increasing capacity.&]
[s6; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators and references to Vector.&]
[s7; [*C@3 i]-|Position of required element.&]
[s7; [*C@3 x]-|Initialization value of newly added elements.&]
[s7; [*/ Возвратное значение]-|Reference to required 
element.&]
[s3;%- &]
[s4;%- &]
[s5;:Vector`:`:Shrink`(`):%- [@(0.0.255) void]_[* Shrink]()&]
[s2; Minimizes memory consumption of Vector by decreasing capacity 
to number of elements.&]
[s3;%- &]
[s4;%- &]
[s5;:Vector`:`:Reserve`(int`):%- [@(0.0.255) void]_[* Reserve]([@(0.0.255) int]_[*@3 n])&]
[s2; Reserves capacity. If required capacity is greater than current 
capacity, capacity is increased to the required value.&]
[s7; [*C@3 xtra]-|Required capacity.&]
[s3;%- &]
[s4;%- &]
[s5;:Vector`:`:GetAlloc`(`)const:%- [@(0.0.255) int]_[* GetAlloc]()_[@(0.0.255) const]&]
[s2; Возвращает current capacity of Vector.&]
[s7; [*/ Возвратное значение]-|Capacity of Vector.&]
[s3;%- &]
[s4;%- &]
[s5;:Vector`:`:Set`(int`,const T`&`,int`):%- [@(0.0.255) void]_[* Set]([@(0.0.255) int]_[*@3 i
], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 x], [@(0.0.255) int]_[*@3 count])&]
[s2; Устанавливаетrequested number of elements starting 
at the position [* i] to the specified value. If required number 
of elements exceeds existing elements of Vector then elements 
are added to Vector.&]
[s6; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators and references to Vector.&]
[s7; [*C@3 i]-|Starting position.&]
[s7; [*C@3 x]-|Value.&]
[s7; [*C@3 count]-|Number of elements.&]
[s3;%- &]
[s4;%- &]
[s5;:Vector`:`:Set`(int`,const T`&`):%- [*@4 T][@(0.0.255) `&]_[* Set]([@(0.0.255) int]_[*@3 i],
 [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 x])&]
[s5;:Upp`:`:Vector`:`:Set`(int`,T`&`&`):%- [*@4 T][@(0.0.255) `&]_[* Set]([@(0.0.255) int]_[*@3 i
], [*@4 T][@(0.0.255) `&`&]_[*@3 x])&]
[s2; Устанавливаетthe element at [%-*@3 i] to [%-*@3 x] 
(adds default constructed elements as necessary) and returns 
a reference to this element.&]
[s3; &]
[s4;%- &]
[s5;:Vector`:`:Remove`(int`,int`):%- [@(0.0.255) void]_[* Remove]([@(0.0.255) int]_[*@3 i], 
[@(0.0.255) int]_[*@3 count]_`=_[@3 1])&]
[s2; Removes requested number of elements.&]
[s6; Invalidates iterators and references to Vector.&]
[s7; [*C@3 i]-|Position.&]
[s7; [*C@3 count]-|Number of elements to remove.&]
[s3;%- &]
[s4;%- &]
[s5;:Vector`:`:Remove`(const int`*`,int`):%- [@(0.0.255) void]_[* Remove]([@(0.0.255) const
]_[@(0.0.255) int]_`*[*@3 sorted`_list], [@(0.0.255) int]_[*@3 n])&]
[s2; Removes multiple elements from Vector. Time of operation almost 
does not depend on number of elements.&]
[s6; Invalidates iterators and references to Vector.&]
[s7; [*C@3 sorted`_list]-|Pointer to array of positions to remove. It 
must be sorted from lowest to greatest value.&]
[s7; [*C@3 n]-|Number of elements to remove.&]
[s3;%- &]
[s4;%- &]
[s5;:Vector`:`:Remove`(const Vector`<int`>`&`):%- [@(0.0.255) void]_[* Remove]([@(0.0.255) c
onst]_[_^Vector^ Vector][@(0.0.255) <int>`&]_[*@3 sorted`_list])&]
[s2; Removes multiple elements from Vector. То же, что и 
[* Remove(sorted`_list, sorted`_list.GetCount())].&]
[s6; Invalidates iterators and references to Vector.&]
[s7; [*C@3 sorted`_list]-|Sorted Vector of positions to remove.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Vector`:`:RemoveIf`(Condition`):%- [@(0.0.255) template]_<[@(0.0.255) class]_
[*@4 Condition]>_[@(0.0.255) void]_[* RemoveIf]([*@4 Condition]_[*@3 c])&]
[s2; Removes all elements that satisfy predicate [%-*@3 c]. [%-*@3 c] 
is a function object (usually a lambda) that has a single int 
parameter representing the index of an element and returns true 
when the element at the  index is supposed to be removed.&]
[s3; &]
[s4;%- &]
[s5;:Vector`:`:InsertN`(int`,int`):%- [@(0.0.255) void]_[* InsertN]([@(0.0.255) int]_[*@3 i],
 [@(0.0.255) int]_[*@3 count]_`=_[@3 1])&]
[s2; Inserts a specified number of default constructed elements at 
a specified position.&]
[s6; Requires T to have default constructor.&]
[s6; Invalidates iterators and references to Vector.&]
[s7; [*C@3 i]-|Position.&]
[s7; [*C@3 count]-|Number of elements to insert.&]
[s3;%- &]
[s4;%- &]
[s5;:Vector`:`:Insert`(int`):%- [*@4 T][@(0.0.255) `&]_[* Insert]([@(0.0.255) int]_[*@3 i])&]
[s2; Inserts one default constructed element at the specified position.&]
[s7; [*1 Requires T to have default constructor.]&]
[s6; Invalidates iterators and references to Vector.&]
[s7; [*C@3 i]-|Position.&]
[s3;%- &]
[s4;%- &]
[s5;:Vector`:`:Insert`(int`,const T`&`,int`):%- [@(0.0.255) void]_[* Insert]([@(0.0.255) in
t]_[*@3 i], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 x], [@(0.0.255) int]_[*@3 count])&]
[s2; Inserts a specified number of elements, setting them to a specified 
value.&]
[s6;~~~.992; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators and references to Vector.&]
[s7; [*C@3 i]-|Position.&]
[s7; [*C@3 x]-|Value of inserted elements.&]
[s7; [*C@3 count]-|Number of elements to insert.&]
[s3;%- &]
[s4;%- &]
[s5;:Vector`:`:Insert`(int`,const T`&`):%- [*@4 T][@(0.0.255) `&]_[* Insert]([@(0.0.255) int]_
[*@3 i], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 x])&]
[s2; Inserts element at position [%-*@3 i] setting its value to [%-*@3 x] 
and returns a reference to it.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Vector`:`:Insert`(int`,T`&`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T
]>_[*@4 T][@(0.0.255) `&]_[* Insert]([@(0.0.255) int]_[*@3 q], [*@4 T][@(0.0.255) `&`&]_[*@3 x])&]
[s2; Inserts an element at [%-*@3 i] picking the content of [%-*@3 x] 
and returns a reference to this new element.&]
[s3; &]
[s4;%- &]
[s5;:Vector`:`:Insert`(int`,const Vector`&`):%- [@(0.0.255) void]_[* Insert]([@(0.0.255) in
t]_[*@3 i], [@(0.0.255) const]_[_^Vector^ Vector][@(0.0.255) `&]_[*@3 x])&]
[s2; Inserts all elements from another Vector.&]
[s6;~~~.992; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators and references to Vector.&]
[s7; [*C@3 i]-|Position.&]
[s7; [*C@3 x]-|Source Vector.&]
[s3;%- &]
[s4;%- &]
[s5;:Vector`:`:Insert`(int`,const Vector`&`,int`,int`):%- [@(0.0.255) void]_[* Insert]([@(0.0.255) i
nt]_[*@3 i], [@(0.0.255) const]_[_^Vector^ Vector][@(0.0.255) `&]_[*@3 x], 
[@(0.0.255) int]_[*@3 offset], [@(0.0.255) int]_[*@3 count])&]
[s2; Inserts a range of elements from another Vector.&]
[s6;~~~.992; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators and references to Vector.&]
[s7; [*C@3 i]-|Insertion position.&]
[s7; [*C@3 x]-|Source Vector.&]
[s7; [*C@3 offset]-|Position of first element in source Vector to be 
inserted.&]
[s7; [*C@3 count]-|Number of elements to insert.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Vector`:`:Insert`(int`,Upp`:`:Vector`&`&`):%- [@(0.0.255) void]_[* Insert]([@(0.0.255) i
nt]_[*@3 i], [_^Upp`:`:Vector^ Vector][@(0.0.255) `&`&]_[*@3 x])&]
[s2; Inserts source Vector at specified position using pick transfer 
semantics. It is faster than deep copy insert, does not use deep 
copy constructor for T, but destroys source Vector.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Vector`:`:Insert`(int`,std`:`:initializer`_list`<T`>`):%- [@(0.0.255) void]_
[* Insert]([@(0.0.255) int]_[*@3 i], [_^std`:`:initializer`_list^ std`::initializer`_list]<
[*@4 T]>_[*@3 init])&]
[s2; Inserts C`+`+11 style initialization list.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Vector`:`:Append`(std`:`:initializer`_list`<T`>`):%- [@(0.0.255) void]_[* App
end](std`::initializer`_list<[*@4 T]>_[*@3 init])&]
[s2; Appends C`+`+11 style initialization list.&]
[s3;%- &]
[s4;%- &]
[s5;:Vector`:`:InsertSplit`(int`,Vector`<T`>`&`,int`):%- [@(0.0.255) void]_[* InsertSplit
]([@(0.0.255) int]_[*@3 i], [_^Vector^ Vector]<[*@4 T]>`&_[*@3 v], [@(0.0.255) int]_[*@3 from])
&]
[s2; Insert the part of source vector [%-*@3 v] starting at element 
[%-*@3 from] till the end at position [%-*@3 i] and trims [%-*@3 v] 
at [%-*@3 from], effectively splitting [%-*@3 v] into two parts.&]
[s3; &]
[s4;%- &]
[s5;:Vector`:`:Append`(const Vector`&`):%- [@(0.0.255) void]_[* Append]([@(0.0.255) const]_
[_^Vector^ Vector][@(0.0.255) `&]_[*@3 x])&]
[s2; Appends all elements of source Vector.&]
[s6;~~~.992; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators and references to Vector.&]
[s7; [*C@3 x]-|Source Vector.&]
[s3;%- &]
[s4;%- &]
[s5;:Vector`:`:Append`(const Vector`&`,int`,int`):%- [@(0.0.255) void]_[* Append]([@(0.0.255) c
onst]_[_^Vector^ Vector][@(0.0.255) `&]_[*@3 x], [@(0.0.255) int]_[*@3 o], 
[@(0.0.255) int]_[*@3 c])&]
[s2; Appends a range of elements from source Vector.&]
[s6;~~~.992; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators and references to Vector.&]
[s7; [*C@3 x]-|Source Vector.&]
[s7; [*C@3 o]-|Position of first element in source Vector to be inserted.&]
[s7; [*C@3 c]-|Number of elements to insert.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Vector`:`:Append`(Upp`:`:Vector`&`&`):%- [@(0.0.255) void]_[* Append]([_^Upp`:`:Vector^ V
ector][@(0.0.255) `&`&]_[*@3 x])&]
[s2; Appends source Vector using pick transfer semantics. It is faster 
than deep copy insert, does not use deep copy constructor for 
T, but destroys source Vector by picking.&]
[s3;%- &]
[s4;%- &]
[s5;:Vector`:`:Swap`(int`,int`):%- [@(0.0.255) void]_[* Swap]([@(0.0.255) int]_[*@3 i1], 
[@(0.0.255) int]_[*@3 i2])&]
[s2; Swaps elements at [%-*@3 i1] and [%-*@3 i2].&]
[s3; &]
[s4;%- &]
[s5;:Vector`:`:Drop`(int`):%- [@(0.0.255) void]_[* Drop]([@(0.0.255) int]_[*@3 n]_`=_[@3 1])&]
[s2; Drops specified number of last elements in the Vector (same 
as Trim(GetCount() `- n)).&]
[s7; [*C@3 n]-|Number of elements.&]
[s3;%- &]
[s4;%- &]
[s5;:Vector`:`:Top`(`):%- [*@4 T][@(0.0.255) `&]_[* Top]()&]
[s2; Возвращает reference to the last element in the Vector.&]
[s7; [*/ Возвратное значение]-|Reference of last element 
in the Vector.&]
[s3;%- &]
[s4;%- &]
[s5;:Vector`:`:Top`(`)const:%- [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[* Top]()_[@(0.0.255) c
onst]&]
[s2; Возвращает constant reference to the last element 
in the Vector.&]
[s7; [*/ Возвратное значение]-|Reference of last element 
in the Vector.&]
[s3;%- &]
[s4;%- &]
[s5;:Vector`:`:Pop`(`):%- [*@4 T]_[* Pop]()&]
[s2; Drops last element of Vector and returns its value.&]
[s6; Requires T to have deep copy constructor.&]
[s7; [*/ Возвратное значение]-|Value of dropped element.&]
[s3;%- &]
[s4;%- &]
[s5;:Vector`:`:operator T`*`(`):%- [@(0.0.255) operator]_[*@4 T][@(0.0.255) `*]()&]
[s2; Возвращает non`-constant pointer to elements.&]
[s7; [*/ Возвратное значение]-|Pointer to elements.&]
[s3;%- &]
[s4;%- &]
[s5;:Vector`:`:operator const T`*`(`)const:%- [@(0.0.255) operator]_[@(0.0.255) const]_[*@4 T
][@(0.0.255) `*]()_[@(0.0.255) const]&]
[s2; Возвращает constant pointer to elements.&]
[s7; [*/ Возвратное значение]-|Pointer to elements.&]
[s3;%- &]
[s4;%- &]
[s5;:Vector`:`:operator`<`<`(const T`&`):%- [_^Vector^ Vector][@(0.0.255) `&]_[* operator<<
]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 x])&]
[s2; Operator equivalent of [* void Add(const T`&x)]. By returning 
reference to to Vector allows adding more elements in single 
expression, thus e.g. allowing to construct temporary Vector 
as part of expression like Foo((Vector<int>() << 1 << 2 << 4)).&]
[s6; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators and references to Vector.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Vector`:`:operator`<`<`(T`&`&`):%- [_^Upp`:`:Vector^ Vector][@(0.0.255) `&]_[* o
perator<<]([*@4 T][@(0.0.255) `&`&]_[*@3 x])&]
[s2; Operator equivalent of [* void Add(TT`&x)]. By returning reference 
to to Vector allows adding more elements in single expression, 
thus e.g. allowing to construct temporary Vector as part of expression 
like.&]
[s6; Requires T to have deep copy constructor.&]
[s6; Invalidates iterators and references to Vector.&]
[s3; &]
[s4;%- &]
[s5;:Vector`:`:ToString`(`)const:%- [_^String^ String]_[* ToString]()_[@(0.0.255) const]&]
[s2; Converts container into String, mostly for debugging purposes.&]
[s3;%- &]
[s4;%- &]
[s5;:Vector`:`:operator`=`=`(const Vector`<T`>`&`)const:%- [@(0.0.255) bool]_[* operator`=
`=]([@(0.0.255) const]_[_^Vector^ Vector]<[*@4 T]>`&_[*@3 b])_[@(0.0.255) const]&]
[s5;:Vector`:`:operator`!`=`(const Vector`<T`>`&`)const:%- [@(0.0.255) bool]_[* operator!
`=]([@(0.0.255) const]_[_^Vector^ Vector]<[*@4 T]>`&_[*@3 b])_[@(0.0.255) const]&]
[s2; Compares two containers for (in)equality, using T`::operator`=`=.&]
[s3; &]
[s4;%- &]
[s5;:Vector`:`:Compare`(const Vector`<T`>`&`)const:%- [@(0.0.255) int]_[* Compare]([@(0.0.255) c
onst]_[_^Vector^ Vector]<[*@4 T]>`&_[*@3 b])_[@(0.0.255) const]&]
[s5;:Vector`:`:operator`<`=`(const Vector`<T`>`&`)const:%- [@(0.0.255) bool]_[* operator<
`=]([@(0.0.255) const]_[_^Vector^ Vector]<[*@4 T]>`&_[*@3 b])_[@(0.0.255) const]&]
[s5;:Vector`:`:operator`>`=`(const Vector`<T`>`&`)const:%- [@(0.0.255) bool]_[* operator>
`=]([@(0.0.255) const]_[_^Vector^ Vector]<[*@4 T]>`&_[*@3 b])_[@(0.0.255) const]&]
[s5;:Vector`:`:operator`<`(const Vector`<T`>`&`)const:%- [@(0.0.255) bool]_[* operator<](
[@(0.0.255) const]_[_^Vector^ Vector]<[*@4 T]>`&_[*@3 b])_[@(0.0.255) const]&]
[s5;:Vector`:`:operator`>`(const Vector`<T`>`&`)const:%- [@(0.0.255) bool]_[* operator>](
[@(0.0.255) const]_[_^Vector^ Vector]<[*@4 T]>`&_[*@3 b])_[@(0.0.255) const]&]
[s2; Lexicographically compares two containers, using SgnCompare 
for elements.&]
[s3; &]
[s4;%- &]
[s5;:Vector`:`:operator`|`(pick`_ T`&`):%- [_^Vector^ Vector][@(0.0.255) `&]_[* operator`|](
[@(0.128.128) pick`_]_[*@4 T][@(0.0.255) `&]_[*@3 x])&]
[s2; Operator replacement of [* void AddPick(pick`_ T`&x)]. By returning 
reference to to Vector allows adding more elements in single 
expression, thus e.g. allowing to construct temporary Vector 
as part of expression.&]
[s6; Requires T to have pick constructor.&]
[s6; Invalidates iterators and references to Vector.&]
[s7; [*C@3 x]-|Source instance of T that is to be picked.&]
[s7; [*/ Возвратное значение]-|Reference to Vector 
(`*this).&]
[s3;%- &]
[s4;%- &]
[s5;:Vector`:`:Serialize`(Stream`&`):%- [@(0.0.255) void]_[* Serialize]([_^Stream^ Stream][@(0.0.255) `&
]_[*@3 s])&]
[s2; Serializes content of Vector to/from Stream. Works only if NTL 
is used as part of UPP.&]
[s6; Requires T to have serialization operator defined.&]
[s7; [*C@3 s]-|Target/source stream.&]
[s3;%- &]
[s4;%- &]
[s5;:Vector`:`:operator`=`(pick`_ Vector`&`):%- [@(0.0.255) void]_[* operator`=]([@(0.128.128) p
ick`_]_[_^Vector^ Vector][@(0.0.255) `&]_[*@3 v])&]
[s2; Pick operator. Transfers source Vector in low constant time, 
but destroys it by picking.&]
[s7; [*C@3 v]-|Source Vector.&]
[s3;%- &]
[s4;%- &]
[s5;:Vector`:`:IsPicked`(`)const:%- [@(0.0.255) bool]_[* IsPicked]()_[@(0.0.255) const]&]
[s2; Возвращает [* true] if Vector is in picked state.&]
[s7; [*/ Возвратное значение]-|[* true] if Vector is 
in picked state, [* false] otherwise.&]
[s3; &]
[s4; &]
[s5;%- friend_T`&_operator<<`=[@(64) (T`&_][@3 dest][@(64) , ][@(0.0.255) const][@(64) _T`&_][@3 s
rc][@(64) )]&]
[s2; Optional deep copy operator. Defined using DeepCopyOptionTemplate.&]
[s6; Requires T to have deep copy operator or optional deep copy 
operator.&]
[s7; [*C@3 v]-|Source Vector.&]
[s3;%- &]
[s4;%- &]
[s5;:Vector`:`:ValueType`:`:typedef:%- [@(0.0.255) typedef]_T_[* ValueType]&]
[s2; Typedef of T for use in templated algorithms.&]
[s3;%- &]
[s4;%- &]
[s5;:Vector`:`:Iterator`:`:typedef:%- [@(0.0.255) typedef]_T_`*[* Iterator]&]
[s2; Iterator type. Iterator is guaranteed to be of T`* type.&]
[s3;%- &]
[s4;%- &]
[s5;:Vector`:`:ConstIterator`:`:typedef:%- [@(0.0.255) typedef]_[@(0.0.255) const]_T_`*[* C
onstIterator]&]
[s2; Constant iterator type. Iterator is guaranteed to be of const 
T`* type.&]
[s3;%- &]
[s4;%- &]
[s5;:Vector`:`:Begin`(`)const:%- [_^Vector`:`:ConstIterator^ ConstIterator]_[* Begin]()_[@(0.0.255) c
onst]&]
[s2; Возвращает constant iterator to the first element 
in Vector.&]
[s7; [*/ Возвратное значение]-|Iterator.&]
[s3;%- &]
[s4;%- &]
[s5;:Vector`:`:End`(`)const:%- [_^Vector`:`:ConstIterator^ ConstIterator]_[* End]()_[@(0.0.255) c
onst]&]
[s2; Возвращает constant iterator to the position just 
beyond the last element in Vector.&]
[s7; [*/ Возвратное значение]-|Iterator.&]
[s3;%- &]
[s4;%- &]
[s5;:Vector`:`:GetIter`(int`)const:%- [_^Vector`:`:ConstIterator^ ConstIterator]_[* GetIt
er]([@(0.0.255) int]_[*@3 i])_[@(0.0.255) const]&]
[s2; Возвращает constant iterator to the element at specified 
position. То же, что и [* Begin() `+ i]. Benefit of this 
methods is that in debug mode [* pos] is range checked.&]
[s7; [*C@3 i]-|Required position.&]
[s7; [*/ Возвратное значение]-|Iterator.&]
[s3;%- &]
[s4;%- &]
[s5;:Vector`:`:Begin`(`):%- [_^Vector`:`:Iterator^ Iterator]_[* Begin]()&]
[s2; Возвращает non`-constant iterator to the first element 
in Vector.&]
[s7; [*/ Возвратное значение]-|Iterator.&]
[s3;%- &]
[s4;%- &]
[s5;:Vector`:`:End`(`):%- [_^Vector`:`:Iterator^ Iterator]_[* End]()&]
[s2; Возвращает non`-constant iterator to the position 
just beyond the last element in Vector.&]
[s7; [*/ Возвратное значение]-|Iterator.&]
[s3;%- &]
[s4;%- &]
[s5;:Vector`:`:GetIter`(int`):%- [_^Vector`:`:Iterator^ Iterator]_[* GetIter]([@(0.0.255) i
nt]_[*@3 i])&]
[s2; Возвращает non`-constant iterator to the element at 
specified position. То же, что и [* Begin() `+ pos]. Benefit 
of this methods is that in debug mode [* pos] is range checked.&]
[s7; [*C@3 i]-|Required position.&]
[s7; [*/ Возвратное значение]-|Iterator.&]
[s3;%- &]
[s4;%- &]
[s5;:`:`:Vector`:`:Swap`(`:`:Vector`&`,`:`:Vector`&`):%- [@(0.0.255) friend]_[@(0.0.255) v
oid]_[* Swap]([_^`:`:Vector^ Vector][@(0.0.255) `&]_[*@3 a], [_^`:`:Vector^ Vector][@(0.0.255) `&
]_[*@3 b])&]
[s2; Specialization of generic [* Swap] for Vector. Swaps Vector in 
simple constant time operation.&]
[s7; [*C@3 a]-|First Vector to swap.&]
[s7; [*C@3 b]-|Second Vector to swap.&]
[s3; &]
[s0; ]]