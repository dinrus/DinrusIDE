topic "Класс FixedAMap";
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
[ {{10000@(113.42.0) [s0; [*@7;4 Класс FixedAMap]]}}&]
[s3;%- &]
[s1;:noref:%- [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@4;3 K][3 , 
][@(0.0.255)3 class][3 _][*@4;3 T][3 , ][@(0.0.255)3 class][3 _][*@4;3 V][3 , 
][@(0.0.255)3 class][3 _][*@4;3 Less][3 >]&]
[s1;:FixedAMap`:`:class:%- [@(0.0.255) class]_[* FixedAMap]&]
[s2; FixedAMap `- простой класс`-адаптер для 
вектора ключей и контейнера значений 
(Vector или Array), являющийся пространственно`-эфф
ективным вариантом мапа, используемый 
в тех ситуациях,когда содержимое 
мапа постоянно и заранее известно 
(до его создания). Фундаментальное 
различие в том, что поиск в этого рода 
мапе (карте) невозможен, без вызова 
метода Finish. В текущей реализации используетс
я отсортированный вектор ключей, 
потому потребление памяти у FixedAMap 
такое же, как у двух исходных контейнеров. 
Поиск имеет `"комплектность`" log(N) , 
то есть FixedAMap работает медленнее стандартных
 мапов, основанных на хэшировании, 
но `"отпечаток ноги`" на памяти гораздо 
лучший.&]
[s3;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Список Публичных Методов]]}}&]
[s3;%- &]
[s5;:FixedAMap`:`:Add`(const K`&`,const T`&`):%- [*@4 T][@(0.0.255) `&]_[* Add]([@(0.0.255) c
onst]_[*@4 K][@(0.0.255) `&]_[*@3 k], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 x])&]
[s5;:Upp`:`:FixedAMap`:`:AddPick`(const K`&`,T`&`&`):%- [*@4 T][@(0.0.255) `&]_[* AddPick](
[@(0.0.255) const]_[*@4 K][@(0.0.255) `&]_[*@3 k], [*@4 T][@(0.0.255) `&`&]_[*@3 x])&]
[s5;:FixedAMap`:`:Add`(const K`&`):%- [*@4 T][@(0.0.255) `&]_[* Add]([@(0.0.255) const]_[*@4 K][@(0.0.255) `&
]_[*@3 k])&]
[s2; Добавляет в мап новую пару ключ`-значение, 
возвращая ссылку на значение. После 
вызова одного или более раз метода 
добавки Add, нужно вызвать Finish, чтобы 
мап был готов к поиску.&]
[s3; &]
[s4; &]
[s5;:FixedAMap`:`:Finish`(`):%- [@(0.0.255) void]_[* Finish]()&]
[s2; Подготавливает мап к поиску (текущая 
реализация сортирует индексы ключей 
и значений).&]
[s3; &]
[s4; &]
[s5;:FixedAMap`:`:Find`(const K`&`)const:%- [@(0.0.255) int]_[* Find]([@(0.0.255) const]_[*@4 K
][@(0.0.255) `&]_[*@3 k])_[@(0.0.255) const]&]
[s2; Возвращает минимальный индекс элемента 
с ключом, равным [%-*@3 k], либо отрицательное 
число, если этот индекс не найден.&]
[s3; &]
[s4; &]
[s5;:FixedAMap`:`:FindNext`(int`)const:%- [@(0.0.255) int]_[* FindNext]([@(0.0.255) int]_[*@3 i
])_[@(0.0.255) const]&]
[s2; Если ключ по [%-*@3 i] `+ 1 равен ключу по 
[%-*@3 i] , возвращает [%-*@3 i] `+ 1, иначе возвращается 
отрицательное число.&]
[s3; &]
[s4; &]
[s5;:FixedAMap`:`:Get`(const K`&`):%- [*@4 T][@(0.0.255) `&]_[* Get]([@(0.0.255) const]_[*@4 K][@(0.0.255) `&
]_[*@3 k])&]
[s5;:FixedAMap`:`:Get`(const K`&`)const:%- [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[* Get](
[@(0.0.255) const]_[*@4 K][@(0.0.255) `&]_[*@3 k])_[@(0.0.255) const]&]
[s2; Возвращает значение первого элемента 
с ключом [%-*@3 k]. Если элемент не найден, 
поведение неопределённое&]
[s3; &]
[s4; &]
[s5;:FixedAMap`:`:Get`(const K`&`,const T`&`)const:%- [@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[* Get]([@(0.0.255) const]_[*@4 K][@(0.0.255) `&]_[*@3 k], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[*@3 d])_[@(0.0.255) const]&]
[s2; Возвращает значение первого элемента 
с ключом [%-*@3 k]. Если элемент не найден, 
возвращается [%-*@3 d].&]
[s3; &]
[s4; &]
[s5;:FixedAMap`:`:FindPtr`(const K`&`):%- [*@4 T]_`*[* FindPtr]([@(0.0.255) const]_[*@4 K][@(0.0.255) `&
]_[*@3 k])&]
[s5;:FixedAMap`:`:FindPtr`(const K`&`)const:%- [@(0.0.255) const]_[*@4 T]_`*[* FindPtr]([@(0.0.255) c
onst]_[*@4 K][@(0.0.255) `&]_[*@3 k])_[@(0.0.255) const]&]
[s2; Возвращает указатель на значение 
первого элемента с ключом [%-*@3 k]. Если 
он не найден, вернёт NULL.&]
[s3; &]
[s4; &]
[s5;:FixedAMap`:`:operator`[`]`(int`)const:%- [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[* op
erator`[`]]([@(0.0.255) int]_[*@3 i])_[@(0.0.255) const]&]
[s5;:FixedAMap`:`:operator`[`]`(int`):%- [*@4 T][@(0.0.255) `&]_[* operator`[`]]([@(0.0.255) i
nt]_[*@3 i])&]
[s2; Возвращает значение элемента по 
[%-*@3 i].&]
[s3; &]
[s4; &]
[s5;:FixedAMap`:`:GetCount`(`)const:%- [@(0.0.255) int]_[* GetCount]()_[@(0.0.255) const]&]
[s2; Возвращает число элементов.&]
[s3; &]
[s4; &]
[s5;:FixedAMap`:`:IsEmpty`(`)const:%- [@(0.0.255) bool]_[* IsEmpty]()_[@(0.0.255) const]&]
[s2; То же, что и GetCount() `=`= 0.&]
[s3; &]
[s4; &]
[s5;:FixedAMap`:`:Clear`(`):%- [@(0.0.255) void]_[* Clear]()&]
[s2; Удаляет все элементы.&]
[s3; &]
[s4; &]
[s5;:FixedAMap`:`:Shrink`(`):%- [@(0.0.255) void]_[* Shrink]()&]
[s2; Понижает использование памяти, `"роняя`" 
резервы размещения.&]
[s3; &]
[s4; &]
[s5;:FixedAMap`:`:Reserve`(int`):%- [@(0.0.255) void]_[* Reserve]([@(0.0.255) int]_[*@3 xtra])
&]
[s2; Резервирует ёмкость. Если требуемая 
ёмкость больше текущей, то ёмкость 
увеличивается до требуемого значения.&]
[s3; &]
[s4; &]
[s5;:FixedAMap`:`:GetAlloc`(`)const:%- [@(0.0.255) int]_[* GetAlloc]()_[@(0.0.255) const]&]
[s2; Возвращает текущую ёмкость Array`'я.&]
[s3; &]
[s4; &]
[s5;:FixedAMap`:`:GetKey`(int`)const:%- [@(0.0.255) const]_[*@4 K][@(0.0.255) `&]_[* GetKey](
[@(0.0.255) int]_[*@3 i])_[@(0.0.255) const]&]
[s2; Возвращает ключ элемента по [%-*@3 i].&]
[s3; &]
[s4; &]
[s5;:FixedAMap`:`:Serialize`(Stream`&`):%- [@(0.0.255) void]_[* Serialize]([_^Stream^ Strea
m][@(0.0.255) `&]_[*@3 s])&]
[s2; Сериализует содержимое AMap в/из Stream`'а.&]
[s3; &]
[s4; &]
[s5;:FixedAMap`:`:Xmlize`(XmlIO`&`):%- [@(0.0.255) void]_[* Xmlize]([_^XmlIO^ XmlIO][@(0.0.255) `&
]_[*@3 xio])&]
[s2; Сериализует содержимое AMap`'а в/из 
XML.&]
[s3; &]
[s4; &]
[s5;:FixedAMap`:`:Jsonize`(JsonIO`&`):%- [@(0.0.255) void]_[* Jsonize]([_^JsonIO^ JsonIO][@(0.0.255) `&
]_[*@3 jio])&]
[s2; Сериализует содержимое AMap`'а в/из 
JSON.&]
[s3; &]
[s4; &]
[s5;:FixedAMap`:`:Swap`(FixedAMap`&`):%- [@(0.0.255) void]_[* Swap]([_^FixedAMap^ FixedAMap
][@(0.0.255) `&]_[*@3 x])&]
[s2; Меняет местами контент FixedAMap с другим.&]
[s3; &]
[s4; &]
[s5;:FixedAMap`:`:GetKeys`(`)const:%- [@(0.0.255) const]_[_^Vector^ Vector]<[*@4 K]>`&_[* Get
Keys]()_[@(0.0.255) const]&]
[s2; Возвращает ключи. Ключи, надо заметить, 
отсортированы.&]
[s3; &]
[s4; &]
[s5;:FixedAMap`:`:PickKeys`(`)pick`_:%- [_^Vector^ Vector]<[*@4 K]>_[* PickKeys]()_[@(0.128.128) p
ick`_]&]
[s2; Пикует ключи.&]
[s3; &]
[s4; &]
[s5;:FixedAMap`:`:GetValues`(`)const:%- [@(0.0.255) const]_[*@4 V][@(0.0.255) `&]_[* GetValue
s]()_[@(0.0.255) const]&]
[s5;:FixedAMap`:`:GetValues`(`):%- [*@4 V][@(0.0.255) `&]_[* GetValues]()&]
[s2; Возвращает  значения.&]
[s3; &]
[s4; &]
[s5;:FixedAMap`:`:PickValues`(`)pick`_:%- [*@4 V]_[* PickValues]()_[@(0.128.128) pick`_]&]
[s2; Пикует значения.&]
[s3; &]
[s4; &]
[s5;:FixedAMap`:`:IsPicked`(`)const:%- [@(0.0.255) bool]_[* IsPicked]()_[@(0.0.255) const]&]
[s2; Возвращает true, если мап пикован.&]
[s3; &]
[s4; &]
[s5;:FixedAMap`:`:operator`(`)`(const K`&`,const T`&`):%- [_^FixedAMap^ FixedAMap][@(0.0.255) `&
]_[* operator()]([@(0.0.255) const]_[*@4 K][@(0.0.255) `&]_[*@3 k], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[*@3 v])&]
[s2; То же, что и Add([%-*@3 k], [%-*@3 v]), возвращает 
`*this. Удобный вариант для создания 
мапов.&]
[s3; &]
[s4; &]
[s5;:FixedAMap`:`:FixedAMap`(const FixedAMap`&`,int`):%- [* FixedAMap]([@(0.0.255) const]_
[* FixedAMap][@(0.0.255) `&]_[*@3 s], [@(0.0.255) int])&]
[s2; Конструктор глубокой копии.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:FixedAMap`:`:FixedAMap`(Upp`:`:Vector`<K`>`&`&`,V`&`&`):%- [* FixedAMap]([_^Upp`:`:Vector^ V
ector]<[*@4 K]>`&`&_[*@3 key], [*@4 V][@(0.0.255) `&`&]_[*@3 val])&]
[s2; Конструктор из контейнеров (которые 
пикуемы).&]
[s3; &]
[s4; &]
[s5;:FixedAMap`:`:KeyBegin`(`)const:%- [_^FixedAMap`:`:KeyConstIterator^ KeyConstIterat
or]_[* KeyBegin]()_[@(0.0.255) const]&]
[s5;:FixedAMap`:`:KeyEnd`(`)const:%- [_^FixedAMap`:`:KeyConstIterator^ KeyConstIterator
]_[* KeyEnd]()_[@(0.0.255) const]&]
[s5;:FixedAMap`:`:KeyGetIter`(int`)const:%- [_^FixedAMap`:`:KeyConstIterator^ KeyConstI
terator]_[* KeyGetIter]([@(0.0.255) int]_[*@3 pos])_[@(0.0.255) const]&]
[s2; Возвращает обходчик к ключу в begin/end/[%-*@3 pos].&]
[s3; &]
[s4; &]
[s5;:FixedAMap`:`:Begin`(`):%- [_^FixedAMap`:`:Iterator^ Iterator]_[* Begin]()&]
[s5;:FixedAMap`:`:End`(`):%- [_^FixedAMap`:`:Iterator^ Iterator]_[* End]()&]
[s5;:FixedAMap`:`:GetIter`(int`):%- [_^FixedAMap`:`:Iterator^ Iterator]_[* GetIter]([@(0.0.255) i
nt]_[*@3 pos])&]
[s5;:FixedAMap`:`:Begin`(`)const:%- [_^FixedAMap`:`:ConstIterator^ ConstIterator]_[* Begi
n]()_[@(0.0.255) const]&]
[s5;:FixedAMap`:`:End`(`)const:%- [_^FixedAMap`:`:ConstIterator^ ConstIterator]_[* End]()
_[@(0.0.255) const]&]
[s5;:FixedAMap`:`:GetIter`(int`)const:%- [_^FixedAMap`:`:ConstIterator^ ConstIterator]_
[* GetIter]([@(0.0.255) int]_[*@3 pos])_[@(0.0.255) const]&]
[s2; Возвращает обходчик к значению в 
begin/end/[%-*@3 pos].&]
[s1;@(0.0.255)3%- &]
[ {{10000@(113.42.0) [s0; [*@7;4 Класс FixedVectorMap]]}}&]
[s3;%- &]
[s1;:noref:%- [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@4;3 K][3 , 
][@(0.0.255)3 class][3 _][*@4;3 T][3 , ][@(0.0.255)3 class][3 _][*@4;3 Less][3 _`=_StdLess<][*@4;3 K
][3 >_>]&]
[s1;:FixedVectorMap`:`:class:%- [@(0.0.255) class]_[* FixedVectorMap]_:_[@(0.0.255) public]_
[*@3 MoveableAndDeepCopyOption]<[* FixedVectorMap]<[*@4 K], [*@4 T], 
[*@4 Less]>_>, [@(0.0.255) public]_[*@3 FixedAMap]<_[*@4 K], [*@4 T], [_^Vector^ Vector]<[*@4 T
]>, [*@4 Less]_>_&]
[s2; `"Векторный флейвор`" фиксированного 
мапа.&]
[s3;%- &]
[ {{10000@(113.42.0) [s0; [*@7;4 Класс FixedArrayMap]]}}&]
[s3;%- &]
[s1;:noref:%- [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@4;3 K][3 , 
][@(0.0.255)3 class][3 _][*@4;3 T][3 , ][@(0.0.255)3 class][3 _][*@4;3 Less][3 _`=_StdLess<][*@4;3 K
][3 >_>]&]
[s1;:FixedArrayMap`:`:class:%- [@(0.0.255) class]_[* FixedArrayMap]_:_[@(0.0.255) public]_[*@3 M
oveableAndDeepCopyOption]<_[* FixedArrayMap]<[*@4 K], [*@4 T], [*@4 Less]>_>, 
[@(0.0.255) public]_[*@3 FixedAMap]<_[*@4 K], [*@4 T], [_^Array^ Array]<[*@4 T]>, 
[*@4 Less]_>_&]
[s2; `"Массивный флейвор`" фиксированного 
мапа.&]
[s3; &]
[s0; ]]