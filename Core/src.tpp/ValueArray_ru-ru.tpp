topic "Класс ValueArray";
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
[{_} 
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Класс ValueArray]]}}&]
[s1;@(0.0.255)3 &]
[s1;:ValueArray`:`:class: [@(0.0.255)3 class][3 _][*3 ValueArray][3 _]&]
[s2;%RU-RU ValueArray `- массив из элементов Value 
(Значение). Он полностью совместим 
с Rich Value (Обогащённым Значением).&]
[s2;%RU-RU &]
[s2;%RU-RU ValueArray совместим также с ValueMap (так 
же, как Date (Дата) совместима с Time (Временем)). 
При присваивании Value, содержащего 
ValueMap, к ValueArray, ему присваивается ValueArray, 
представляющий значения из ValueMap. 
При присваивании ValueArray`'я к ValueMap`'у, 
создаётся мап, с числовыми ключами 
0, 1, ... GetCount() `- 1 и значениями, содержащими 
элементы с соответствующими индексами.&]
[s2;%RU-RU &]
[s2;%RU-RU Заметьте, что Value предоставляет 
также методы (GetCount() и оператор `[`]) 
для прямого доступа к элементам ValueArray`'я, 
содержимого в Value (если Value не содержит 
ValueArray, GetCount() возвращает 0).&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Публичных Методов]]}}&]
[s3; &]
[s5;:ValueArray`:`:ValueArray`(`): [* ValueArray]()&]
[s2;%RU-RU Создаёт пустой ValueArray.&]
[s3; &]
[s4; &]
[s5;:ValueArray`:`:ValueArray`(const ValueArray`&`): [* ValueArray]([@(0.0.255) const]_[* V
alueArray][@(0.0.255) `&]_[*@3 v])&]
[s2;%RU-RU Копи`-конструктор.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:ValueArray`:`:ValueArray`(Upp`:`:Vector`<Upp`:`:Value`>`&`&`): [* ValueArra
y]([_^Upp`:`:Vector^ Vector]<[_^Upp`:`:Value^ Value]>`&`&_[*@3 values])&]
[s2;%RU-RU Создаёт ValueArray, пикуя значения 
[%-*@3 values],`- источник разрушается.&]
[s3;%RU-RU &]
[s4; &]
[s5;:ValueArray`:`:ValueArray`(const Vector`<Value`>`&`,int`): [@(0.0.255) explicit]_[* V
alueArray]([@(0.0.255) const]_[_^Vector^ Vector]<[_^Value^ Value]>`&_[*@3 values], 
[@(0.0.255) int]_[*@3 deep])&]
[s2;%RU-RU Создаёт ValueArray как глубокую копию 
значений [%-*@3 values] ([%-*@3 deep] `- это `"огульный`" 
параметр, который делает сигнатуру 
отличной от версии с пикингом).&]
[s3;%RU-RU &]
[s4; &]
[s5;:ValueArray`:`:`~ValueArray`(`): [@(0.0.255) `~][* ValueArray]()&]
[s2;%RU-RU Деструктор.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:ValueArray`:`:ValueArray`(std`:`:initializer`_list`<Upp`:`:Value`>`): [* Va
lueArray]([_^http`:`/`/en`.cppreference`.com`/w`/cpp`/utility`/initializer`_list^ s
td`::initializer`_list]<[_^Upp`:`:Value^ Value]>_[*@3 init])&]
[s2;%RU-RU Инициализация C`+`+11.&]
[s3;%RU-RU &]
[s4; &]
[s5;:ValueArray`:`:operator`=`(const ValueArray`&`): [_^ValueArray^ ValueArray][@(0.0.255) `&
]_[* operator`=]([@(0.0.255) const]_[_^ValueArray^ ValueArray][@(0.0.255) `&]_[*@3 v])&]
[s2;%RU-RU Оператор присваивания.&]
[s3;%RU-RU &]
[s4; &]
[s5;:ValueArray`:`:operator`=`(Vector`<Value`>rval`_`): [_^ValueArray^ ValueArray][@(0.0.255) `&
]_[* operator`=]([_^Vector^ Vector]<[_^Value^ Value]>_[@(0.0.255) rval`_]_[*@3 values])&]
[s2;%RU-RU Пик`-присваивание `- источник разрушается
.&]
[s3;%RU-RU &]
[s4; &]
[s5;:ValueArray`:`:operator Value`(`)const: [* operator_Value]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает ValueArray как Value.&]
[s3; &]
[s4; &]
[s5;:ValueArray`:`:ValueArray`(const Value`&`): [* ValueArray]([@(0.0.255) const]_[_^Value^ V
alue][@(0.0.255) `&]_[*@3 src])&]
[s2;%RU-RU Получает ValueArray из Value [%-*@3 src]. Заметьте, 
что это работает, даже если Value содержит 
ValueMap (возвращается вектор значений, 
состоящий из пар ключ`-значение).&]
[s3;%RU-RU &]
[s4; &]
[s5;:ValueArray`:`:ValueArray`(const Nuller`&`): [* ValueArray]([@(0.0.255) const]_[_^Nuller^ N
uller][@(0.0.255) `&])&]
[s2;%RU-RU Создаёт пустой ValueArray (присваиванием 
Null).&]
[s3; &]
[s4; &]
[s5;:ValueArray`:`:IsNull`(`)const: [@(0.0.255) bool]_[* IsNull]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает true, если элементов 
нет.&]
[s3; &]
[s4; &]
[s5;:ValueArray`:`:Clear`(`): [@(0.0.255) void]_[* Clear]()&]
[s2;%RU-RU Удаляет все элементы.&]
[s3; &]
[s4; &]
[s5;:ValueArray`:`:SetCount`(int`): [@(0.0.255) void]_[* SetCount]([@(0.0.255) int]_[*@3 n])&]
[s2;%RU-RU Устанавливает счёт элементов 
в [%-*@3 n]. Если число элементов возрастает, 
то новые элементы представляют собой 
значения типа void.&]
[s3;%RU-RU &]
[s4; &]
[s5;:ValueArray`:`:SetCount`(int`,const Value`&`): [@(0.0.255) void]_[* SetCount]([@(0.0.255) i
nt]_[*@3 n], [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 v])&]
[s2;%RU-RU Устанавливает счёт элементов 
в [%-*@3 n]. Если число элементов возрастает, 
то новые элементы устанавливаются 
в [%-*@3 v] .&]
[s3;%RU-RU &]
[s4; &]
[s5;:ValueArray`:`:GetCount`(`)const: [@(0.0.255) int]_[* GetCount]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает число элементов.&]
[s3; &]
[s4; &]
[s5;:ValueArray`:`:IsEmpty`(`)const: [@(0.0.255) bool]_[* IsEmpty]()_[@(0.0.255) const]&]
[s2;%RU-RU То же, что и GetCount() `=`= 0 (или как 
IsNull()).&]
[s3; &]
[s4; &]
[s5;:ValueArray`:`:Add`(const Value`&`): [@(0.0.255) void]_[* Add]([@(0.0.255) const]_[_^Value^ V
alue][@(0.0.255) `&]_[*@3 v])&]
[s2;%RU-RU Добавляет новый элемент [%-*@3 v].&]
[s3;%RU-RU &]
[s4; &]
[s5;:ValueArray`:`:operator`<`<`(const Value`&`): [_^ValueArray^ ValueArray][@(0.0.255) `&
]_[* operator<<]([@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 v])&]
[s2;%RU-RU То же, что и Add([%-*@3 v]).&]
[s3;%RU-RU &]
[s4; &]
[s5;:ValueArray`:`:Set`(int`,const Value`&`): [@(0.0.255) void]_[* Set]([@(0.0.255) int]_[*@3 i
], [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 v])&]
[s2;%RU-RU Устанавливает элемент по индексу 
[%-*@3 i] в значение [%-*@3 v]. Если [%-*@3 i] >`= GetCount(), 
то добавляются значения типа void, чем 
создаётся элемент [%-*@3 i].&]
[s3;%RU-RU &]
[s4; &]
[s5;:ValueArray`:`:Get`(int`)const: [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[* Get
]([@(0.0.255) int]_[*@3 i])_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает элемент по индексу 
[%-*@3 i].&]
[s3;%RU-RU &]
[s4; &]
[s5;:ValueArray`:`:GetAndClear`(int`): [_^Value^ Value]_[* GetAndClear]([@(0.0.255) int]_[*@3 i
])&]
[s2;%RU-RU Присваивает пустое значение Value 
по [%-*@3 i] и возвращает Value, которое находилось 
в этом элементе ранее. Этот особый 
метод предназначен для оптимизации 
в некоторых ситуациях, когда он не 
делает излишнего клонирования Value, 
так как не требуется сохранять исходное 
Value в ValueArray`'е.&]
[s3;%RU-RU &]
[s4; &]
[s5;:ValueArray`:`:Get`(`)const: [@(0.0.255) const]_[_^Vector^ Vector]<[_^Value^ Value]>`&_
[* Get]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает константную ссылку 
на (внутренний) Vector<Value>.&]
[s3; &]
[s4; &]
[s5;:ValueArray`:`:Pick`(`): [_^Vector^ Vector]<[_^Value^ Value]>_[* Pick]()&]
[s2;%RU-RU Пикует контент, ValueArray очищается.&]
[s3; &]
[s4; &]
[s5;:ValueArray`:`:Remove`(int`,int`): [@(0.0.255) void]_[* Remove]([@(0.0.255) int]_[*@3 i],
 [@(0.0.255) int]_[*@3 count]_`=_[@3 1])&]
[s2;%RU-RU Удаляет [%-*@3 count] элементов по индексу 
[%-*@3 i].&]
[s3;%RU-RU &]
[s4; &]
[s5;:ValueArray`:`:Remove`(const Vector`<int`>`&`): [@(0.0.255) void]_[* Remove]([@(0.0.255) c
onst]_[_^Vector^ Vector]<[@(0.0.255) int]>`&_[*@3 ii])&]
[s2;%RU-RU Удаляет отсортированный набор 
индексов.&]
[s3;%RU-RU &]
[s4; &]
[s5;:ValueArray`:`:Insert`(int`,const ValueArray`&`): [@(0.0.255) void]_[* Insert]([@(0.0.255) i
nt]_[*@3 i], [@(0.0.255) const]_[_^ValueArray^ ValueArray][@(0.0.255) `&]_[*@3 va])&]
[s2;%RU-RU Вставляет другой ValueArraу [%-*@3 va] по 
индексу [%-*@3 i].&]
[s3;%RU-RU &]
[s4; &]
[s5;:ValueArray`:`:Append`(const ValueArray`&`): [@(0.0.255) void]_[* Append]([@(0.0.255) c
onst]_[_^ValueArray^ ValueArray][@(0.0.255) `&]_[*@3 va])&]
[s2;%RU-RU Приставляет [%-*@3 va] в конце ValueArray.&]
[s3;%RU-RU &]
[s4; &]
[s5;:ValueArray`:`:operator`[`]`(int`)const: [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&
]_[* operator`[`]]([@(0.0.255) int]_[*@3 i])_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает элементо по индексу 
[%-*@3 i].&]
[s3;%RU-RU &]
[s4; &]
[s5;:ValueArray`:`:At`(int`): [_^Value^ Value][@(0.0.255) `&]_[* At]([@(0.0.255) int]_[*@3 i])&]
[s2;%RU-RU Возвращает ссылку на элемент 
по индексу [%-*@3 i] , если его нет, добавляет 
в массив значения Void, так чтобы этот 
элемент появился.  [^topic`:`/`/Core`/srcdoc`/ValueReference`_ru`-ru^ В
озвращённая ссылка повреждается 
при любом дальнейшем использовании 
исходного Value].&]
[s3;%RU-RU &]
[s4; &]
[s5;:ValueArray`:`:GetHashValue`(`)const: [@(0.0.255) unsigned]_[* GetHashValue]()_[@(0.0.255) c
onst]&]
[s2;%RU-RU Возвращает хэширующее значение.&]
[s3; &]
[s4; &]
[s5;:ValueArray`:`:Serialize`(Stream`&`): [@(0.0.255) void]_[* Serialize]([_^Stream^ Stream
][@(0.0.255) `&]_[*@3 s])&]
[s2;%RU-RU Сериализует ValueArray. У всех элементов 
должна быть реализована сериализация.&]
[s3;%RU-RU &]
[s4; &]
[s5;:ValueArray`:`:Jsonize`(JsonIO`&`): [@(0.0.255) void]_[* Jsonize]([_^JsonIO^ JsonIO][@(0.0.255) `&
]_[*@3 jio])&]
[s2;%RU-RU Преобразует ValueArray в/из JSON. У всех 
элементов должно быть реализовано 
преобразование в JSON.&]
[s3;%RU-RU &]
[s4; &]
[s5;:ValueArray`:`:Xmlize`(XmlIO`&`): [@(0.0.255) void]_[* Xmlize]([_^XmlIO^ XmlIO][@(0.0.255) `&
]_[*@3 xio])&]
[s2;%RU-RU Поддержка сериализации в XML.&]
[s3;%RU-RU &]
[s4; &]
[s5;:ValueArray`:`:ToString`(`)const: [_^String^ String]_[* ToString]()_[@(0.0.255) const]&]
[s2;%RU-RU Преобразует ValueArray в текст.&]
[s3; &]
[s4; &]
[s5;:ValueArray`:`:operator`=`=`(const ValueArray`&`)const: [@(0.0.255) bool]_[* operator
`=`=]([@(0.0.255) const]_[_^ValueArray^ ValueArray][@(0.0.255) `&]_[*@3 v])_[@(0.0.255) con
st]&]
[s5;:ValueArray`:`:operator`!`=`(const ValueArray`&`)const: [@(0.0.255) bool]_[* operator
!`=]([@(0.0.255) const]_[_^ValueArray^ ValueArray][@(0.0.255) `&]_[*@3 v])_[@(0.0.255) cons
t]&]
[s2;%RU-RU Сравнение на равенство. Два ValueArray`'я 
равны, если у них одинаковое число 
элементов и все элементы, расположенные 
в одинаковых позициях, равны.&]
[s3;%RU-RU &]
[s4; &]
[s5;:ValueArray`:`:Compare`(const ValueArray`&`)const: [@(0.0.255) int]_[* Compare]([@(0.0.255) c
onst]_[_^ValueArray^ ValueArray][@(0.0.255) `&]_[*@3 b])_[@(0.0.255) const]&]
[s5;:ValueArray`:`:operator`<`=`(const ValueArray`&`)const: [@(0.0.255) bool]_[* operator
<`=]([@(0.0.255) const]_[_^ValueArray^ ValueArray][@(0.0.255) `&]_[*@3 x])_[@(0.0.255) cons
t]&]
[s5;:ValueArray`:`:operator`>`=`(const ValueArray`&`)const: [@(0.0.255) bool]_[* operator
>`=]([@(0.0.255) const]_[_^ValueArray^ ValueArray][@(0.0.255) `&]_[*@3 x])_[@(0.0.255) cons
t]&]
[s5;:ValueArray`:`:operator`<`(const ValueArray`&`)const: [@(0.0.255) bool]_[* operator<](
[@(0.0.255) const]_[_^ValueArray^ ValueArray][@(0.0.255) `&]_[*@3 x])_[@(0.0.255) const]&]
[s5;:ValueArray`:`:operator`>`(const ValueArray`&`)const: [@(0.0.255) bool]_[* operator>](
[@(0.0.255) const]_[_^ValueArray^ ValueArray][@(0.0.255) `&]_[*@3 x])_[@(0.0.255) const]&]
[s2;%RU-RU Сравнивает два ValueArray`'я, используя 
лексикографическое сравнение.&]
[s3;%RU-RU ]]