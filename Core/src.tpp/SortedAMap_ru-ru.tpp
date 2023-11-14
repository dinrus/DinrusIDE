topic "Класс SortedAMap";
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
[ {{10000@(113.42.0) [s0; [*@7;4 Класс SortedAMap]]}}&]
[s0;3%- &]
[s1;:noref:%- [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@4;3 K][3 , 
][@(0.0.255)3 class][3 _][*@4;3 T][3 , ][@(0.0.255)3 class][3 _][*@4;3 Less][3 , 
][@(0.0.255)3 class][3 _][*@4;3 Data][3 >]&]
[s1;:SortedAMap`:`:class:%- [@(0.0.255) class]_[* SortedAMap]_:_[@(0.0.255) private]_[*@3 Move
ableAndDeepCopyOption]<_[* SortedAMap]<[*@4 K], [*@4 T], [*@4 Less], 
[*@4 Data]>_>_&]
[s2; SortedAMap реализует методы, общие как 
для SortedVectorMap, так и для SortedArrayMap. Сортированны
й мап `- это композиция из [^topic`:`/`/Core`/src`/SortedIndex`_ru`-ru^ S
ortedIndex] и [^topic`:`/`/Core`/src`/InVector`_ru`-ru^ InVector], 
или [^topic`:`/`/Core`/src`/InArray`_ru`-ru^ InArray]. Здесь 
используется строгое упорядочивание 
ключей предикатом [%-*@4 Less], чтобы выполнялся 
бинарный поиск элементов. Требуется, 
чтобы тип ключей [%-*@4 K] был [*/^topic`:`/`/Core`/srcdoc`/Moveable`_ru`-ru^ m
oveable ]типом. Как иной другой контейнер 
NTL, AMap [*/^topic`:`/`/Core`/srcdoc`/Moveable`_ru`-ru^ moveable][*/  
]типа с семантикой трансфера [*/^topic`:`/`/Core`/srcdoc`/Moveable`_ru`-ru^ p
ick and optional deep copy], хотя эти фичи важнее 
в производных `- конкретных `- видах 
AMap.&]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Список Публичных Методов]]}}&]
[s3;%- &]
[s5;:SortedAMap`:`:FindLowerBound`(const K`&`)const:%- [@(0.0.255) int]_[* FindLowerBound
]([@(0.0.255) const]_[*@4 K][@(0.0.255) `&]_[*@3 k])_[@(0.0.255) const]&]
[s2; Возвращает нижний индекс элемента 
с ключом [%-*@3 k].&]
[s3; &]
[s4; &]
[s5;:SortedAMap`:`:FindUpperBound`(const K`&`)const:%- [@(0.0.255) int]_[* FindUpperBound
]([@(0.0.255) const]_[*@4 K][@(0.0.255) `&]_[*@3 k])_[@(0.0.255) const]&]
[s2; Возвращает верхний индекс элемента 
с ключом [%-*@3 k].&]
[s3; &]
[s4; &]
[s5;:SortedAMap`:`:Find`(const K`&`)const:%- [@(0.0.255) int]_[* Find]([@(0.0.255) const]_[*@4 K
][@(0.0.255) `&]_[*@3 k])_[@(0.0.255) const]&]
[s2; Возвращает минимальный индекс элемента 
с ключом, равным [%-*@3 k], или отрицательное 
число, если он не найден.&]
[s3; &]
[s4; &]
[s5;:SortedAMap`:`:FindNext`(int`)const:%- [@(0.0.255) int]_[* FindNext]([@(0.0.255) int]_[*@3 i
])_[@(0.0.255) const]&]
[s2; Если ключ по [%-*@3 i] `+ 1 равен ключу по 
[%-*@3 i] , возвращает [%-*@3 i] `+ 1, иначе `- отрицательн
ое число.&]
[s3; &]
[s4; &]
[s5;:SortedAMap`:`:FindLast`(const K`&`)const:%- [@(0.0.255) int]_[* FindLast]([@(0.0.255) c
onst]_[*@4 K][@(0.0.255) `&]_[*@3 k])_[@(0.0.255) const]&]
[s2; Возвращает максимальный индекс элемента 
с ключом [%-*@3 k] или отрицательное число, 
если он не найден.&]
[s3; &]
[s4; &]
[s5;:SortedAMap`:`:FindPrev`(int`)const:%- [@(0.0.255) int]_[* FindPrev]([@(0.0.255) int]_[*@3 i
])_[@(0.0.255) const]&]
[s2; Если ключ по [%-*@3 i] `- 1 равен элементу 
по [%-*@3 i] , возвращает [%-*@3 i] `- 1, иначе 
`- отрицательное число&]
[s3; &]
[s4; &]
[s5;:SortedAMap`:`:Get`(const K`&`):%- [*@4 T][@(0.0.255) `&]_[* Get]([@(0.0.255) const]_[*@4 K
][@(0.0.255) `&]_[*@3 k])&]
[s5;:SortedAMap`:`:Get`(const K`&`)const:%- [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[* Get](
[@(0.0.255) const]_[*@4 K][@(0.0.255) `&]_[*@3 k])_[@(0.0.255) const]&]
[s2; Возвращает значение первого элемента 
с ключом [%-*@3 k], если он не найден, поведение 
неясное.&]
[s3; &]
[s4; &]
[s5;:SortedAMap`:`:Get`(const K`&`,const T`&`)const:%- [@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[* Get]([@(0.0.255) const]_[*@4 K][@(0.0.255) `&]_[*@3 k], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[*@3 d])_[@(0.0.255) const]&]
[s2; Возвращает значение первого элемента 
с ключом [%-*@3 k]. Если он не найден, возвращает 
[%-*@3 d].&]
[s3; &]
[s4; &]
[s5;:SortedAMap`:`:FindPtr`(const K`&`):%- [*@4 T]_`*[* FindPtr]([@(0.0.255) const]_[*@4 K][@(0.0.255) `&
]_[*@3 k])&]
[s5;:SortedAMap`:`:FindPtr`(const K`&`)const:%- [@(0.0.255) const]_[*@4 T]_`*[* FindPtr]([@(0.0.255) c
onst]_[*@4 K][@(0.0.255) `&]_[*@3 k])_[@(0.0.255) const]&]
[s2; Возвращает указатель на значение 
первого элемента с ключом [%-*@3 k]. Если 
он не найден, возвращает NULL.&]
[s3; &]
[s4; &]
[s5;:SortedAMap`:`:GetKey`(int`)const:%- [@(0.0.255) const]_[*@4 K][@(0.0.255) `&]_[* GetKey](
[@(0.0.255) int]_[*@3 i])_[@(0.0.255) const]&]
[s2; Возвращает ключ элемента по индексу 
[%-*@3 i].&]
[s3;@(0.0.255)%- &]
[s4;@(0.0.255)%- &]
[s5;:SortedAMap`:`:operator`[`]`(int`)const:%- [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[* o
perator`[`]]([@(0.0.255) int]_[*@3 i])_[@(0.0.255) const]&]
[s5;:SortedAMap`:`:operator`[`]`(int`):%- [*@4 T][@(0.0.255) `&]_[* operator`[`]]([@(0.0.255) i
nt]_[*@3 i])&]
[s2; Возвращает значение элемента по 
индексу [%-*@3 i].&]
[s3; &]
[s4; &]
[s5;:SortedAMap`:`:GetCount`(`)const:%- [@(0.0.255) int]_[* GetCount]()_[@(0.0.255) const]&]
[s2; Возвращает число элементов.&]
[s3; &]
[s4; &]
[s5;:SortedAMap`:`:IsEmpty`(`)const:%- [@(0.0.255) bool]_[* IsEmpty]()_[@(0.0.255) const]&]
[s2; То же, что и GetCount() `=`= 0.&]
[s3; &]
[s4; &]
[s5;:SortedAMap`:`:Clear`(`):%- [@(0.0.255) void]_[* Clear]()&]
[s2; Удаляет все элементы.&]
[s3; &]
[s4; &]
[s5;:SortedAMap`:`:Shrink`(`):%- [@(0.0.255) void]_[* Shrink]()&]
[s2; Минимизирует использование памяти, 
роняя зарезервированные аллокации.&]
[s3; &]
[s4; &]
[s5;:SortedAMap`:`:Remove`(int`):%- [@(0.0.255) void]_[* Remove]([@(0.0.255) int]_[*@3 i])&]
[s2; Удаляет элемент по индексу [%-*@3 i].&]
[s3; &]
[s4; &]
[s5;:SortedAMap`:`:Remove`(int`,int`):%- [@(0.0.255) void]_[* Remove]([@(0.0.255) int]_[*@3 i
], [@(0.0.255) int]_[*@3 count])&]
[s2; Удаляет [%-*@3 count] элементов по индексу 
[%-*@3 i].&]
[s3; &]
[s4; &]
[s5;:SortedAMap`:`:RemoveKey`(const K`&`):%- [@(0.0.255) int]_[* RemoveKey]([@(0.0.255) con
st]_[*@4 K][@(0.0.255) `&]_[*@3 k])&]
[s2; Удаляет все элементы с ключом [%-*@3 k].&]
[s3; &]
[s4; &]
[s5;:SortedAMap`:`:Swap`(SortedAMap`&`):%- [@(0.0.255) void]_[* Swap]([_^SortedAMap^ Sorted
AMap][@(0.0.255) `&]_[*@3 x])&]
[s2; Меняет местами этот SortedAMap с другим.&]
[s3; &]
[s4; &]
[s5;:SortedAMap`:`:IsPicked`(`)const:%- [@(0.0.255) bool]_[* IsPicked]()_[@(0.0.255) const]&]
[s2; Возвращает true, если SortedAMap `"пикнут`".&]
[s3; &]
[s4; &]
[s5;:SortedAMap`:`:GetIndex`(`)const:%- [@(0.0.255) const]_[_^SortedIndex^ SortedIndex]<[*@4 K
]>`&_[* GetIndex]()_[@(0.0.255) const]&]
[s2; Возвращает ссылку на внутренний 
индекс ключей.&]
[s3; &]
[s4; &]
[s5;:SortedAMap`:`:GetKeys`(`)const:%- [@(0.0.255) const]_[_^InVector^ InVector]<[*@4 K]>`&
_[* GetKeys]()_[@(0.0.255) const]&]
[s2; Возвращает ссылку на внутренний 
InVector ключей.&]
[s3; &]
[s4; &]
[s5;:SortedAMap`:`:SortedAMap`(const SortedAMap`&`,int`):%- [* SortedAMap]([@(0.0.255) co
nst]_[* SortedAMap][@(0.0.255) `&]_[*@3 s], [@(0.0.255) int])&]
[s2; Конструктор глубокой копии.&]
[s3; &]
[s4; &]
[s5;:SortedAMap`:`:KeyBegin`(`)const:%- [_^SortedAMap`:`:KeyConstIterator^ KeyConstIter
ator]_[* KeyBegin]()_[@(0.0.255) const]&]
[s5;:SortedAMap`:`:KeyEnd`(`)const:%- [_^SortedAMap`:`:KeyConstIterator^ KeyConstIterat
or]_[* KeyEnd]()_[@(0.0.255) const]&]
[s5;:SortedAMap`:`:KeyGetIter`(int`)const:%- [_^SortedAMap`:`:KeyConstIterator^ KeyCons
tIterator]_[* KeyGetIter]([@(0.0.255) int]_[*@3 pos])_[@(0.0.255) const]&]
[s2; Возвращает итератор на ключ по begin/end/[%-*@3 pos].&]
[s3; &]
[s4; &]
[s5;:SortedAMap`:`:Begin`(`):%- [_^SortedAMap`:`:Iterator^ Iterator]_[* Begin]()&]
[s5;:SortedAMap`:`:End`(`):%- [_^SortedAMap`:`:Iterator^ Iterator]_[* End]()&]
[s5;:SortedAMap`:`:GetIter`(int`):%- [_^SortedAMap`:`:Iterator^ Iterator]_[* GetIter]([@(0.0.255) i
nt]_[*@3 pos])&]
[s5;:SortedAMap`:`:Begin`(`)const:%- [_^SortedAMap`:`:ConstIterator^ ConstIterator]_[* Be
gin]()_[@(0.0.255) const]&]
[s5;:SortedAMap`:`:End`(`)const:%- [_^SortedAMap`:`:ConstIterator^ ConstIterator]_[* End](
)_[@(0.0.255) const]&]
[s5;:SortedAMap`:`:GetIter`(int`)const:%- [_^SortedAMap`:`:ConstIterator^ ConstIterator
]_[* GetIter]([@(0.0.255) int]_[*@3 pos])_[@(0.0.255) const]&]
[s2; Возвращает итератор на значение 
по begin/end/[%-*@3 pos].&]
[s2; ]]