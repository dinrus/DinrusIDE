topic "Класс IdCtrls";
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
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Класс IdCtrls]]}}&]
[s3; &]
[s1;:IdCtrls`:`:class: [@(0.0.255)3 class][3 _][*3 IdCtrls]&]
[s2;%RU-RU Вспомогательный класс IdCtrls полезен 
в ситуациях, когда есть связь между 
виджетами и текстовыми идентификаторами. 
IdCtrls, как правило, простой маппинг 
между идентификаторами и виджетами.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Публичных Методов]]}}&]
[s3; &]
[s5;:IdCtrls`:`:Reset`(`): [@(0.0.255) void]_[* Reset]()&]
[s2;%RU-RU Удаляет все записи из IdCtrls.&]
[s3; &]
[s4; &]
[s5;:IdCtrls`:`:Add`(Id`,Ctrl`&`): [@(0.0.255) void]_[* Add]([_^Id^ Id]_[*@3 id], 
[_^Ctrl^ Ctrl][@(0.0.255) `&]_[*@3 ctrl])&]
[s5;:IdCtrls`:`:operator`(`)`(Id`,Ctrl`&`): [_^IdCtrls^ IdCtrls][@(0.0.255) `&]_[* operator
()]([_^Id^ Id]_[*@3 id], [_^Ctrl^ Ctrl][@(0.0.255) `&]_[*@3 ctrl])&]
[s2;%RU-RU Добавляет новую запись из ссылки 
на виджет и идентификатора.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:IdCtrls`:`:GetCount`(`)const: [@(0.0.255) int]_[* GetCount]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает число записей.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:IdCtrls`:`:operator`[`]`(int`): [_^Ctrl^ Ctrl][@(0.0.255) `&]_[* operator`[`]]([@(0.0.255) i
nt]_[*@3 i])&]
[s5;:IdCtrls`:`:operator`[`]`(int`)const: [@(0.0.255) const]_[_^Ctrl^ Ctrl][@(0.0.255) `&]_
[* operator`[`]]([@(0.0.255) int]_[*@3 i])_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает виджет записи [%-*@3 i]. 
.&]
[s3;%RU-RU &]
[s4; &]
[s5;:IdCtrls`:`:GetKey`(int`)const: [_^Id^ Id]_[* GetKey]([@(0.0.255) int]_[*@3 i])_[@(0.0.255) c
onst]&]
[s5;:IdCtrls`:`:operator`(`)`(int`)const: [_^Id^ Id]_[* operator()]([@(0.0.255) int]_[*@3 i])
_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает идентификатор записи 
[%-*@3 i].&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:IdCtrls`:`:Accept`(`): [@(0.0.255) bool]_[* Accept]()&]
[s2;%RU-RU Вызывает Accept у всех виджетов. 
Если любой из виджетов возвращает 
false, возвращает false. Иначе возвращает 
true.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:IdCtrls`:`:ClearModify`(`): [@(0.0.255) void]_[* ClearModify]()&]
[s2;%RU-RU Очищает флаг `'изменён`' у всех 
виджетов.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:IdCtrls`:`:IsModified`(`): [@(0.0.255) bool]_[* IsModified]()&]
[s2;%RU-RU Возвращает true, если у любого из 
виджетов установлен флаг `'изменён`'.&]
[s2;%RU-RU &]
[s4; &]
[s5;:Upp`:`:IdCtrls`:`:Enable`(bool`): [@(0.0.255) void]_[* Enable]([@(0.0.255) bool]_[*@3 b 
][@(0.0.255) `= true])&]
[s2;%RU-RU Включает/отключает все виджеты.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:IdCtrls`:`:Disable`(`): [@(0.0.255) void]_[* Disable]()&]
[s2;%RU-RU То же, что и Enable(false).&]
[s3; &]
[s4; &]
[s5;:Upp`:`:IdCtrls`:`:SetNull`(`): [@(0.0.255) void]_[* SetNull]()&]
[s2;%RU-RU Устанавливает Null (используя SetData) 
всем виджетам.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:IdCtrls`:`:operator`<`<`(Upp`:`:Event`<`>`): [_^Upp`:`:Event^ Event]<>_[* ope
rator<<]([_^Upp`:`:Event^ Event]<>_[*@3 action])&]
[s2;%RU-RU Добавляет [%-*@3 action] к событиям всех 
виджетов WhenAction.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:IdCtrls`:`:operator`^`=`(Upp`:`:Event`<`>`): [_^Upp`:`:Event^ Event]<>_[* ope
rator`^`=]([_^Upp`:`:Event^ Event]<>_[*@3 action])&]
[s2;%RU-RU Устанавливает [%-*@3 action] как событие 
всех виджетов WhenAction.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:IdCtrls`:`:Get`(`)const: [_^ValueMap^ ValueMap]_[* Get]()_[@(0.0.255) const]&]
[s2;%RU-RU Создаёт мап, в котором ключи являются 
идентификаторами записей, а значения 
получаются от соответствующих виджетов 
методом GetData.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:IdCtrls`:`:Set`(const ValueMap`&`): [@(0.0.255) void]_[* Set]([@(0.0.255) const]_[_^ValueMap^ V
alueMap][@(0.0.255) `&]_[*@3 m])&]
[s2;%RU-RU Устанавливает значение виджетов 
через SetData в значения из мапа, используя 
ключи мапа, для идентификации целевых 
виджетов.&]
[s4; &]
[s5;:Upp`:`:IdCtrls`:`:operator`~`(`)const: [_^Upp`:`:ValueMap^ ValueMap]_[* operator`~](
)_[@(0.0.255) const]&]
[s2;%RU-RU То же, что и Get().&]
[s3; &]
[s4; &]
[s5;:Upp`:`:IdCtrls`:`:operator`<`<`=`(const Upp`:`:ValueMap`&`): [@(0.0.255) const]_[_^Upp`:`:ValueMap^ V
alueMap][@(0.0.255) `&]_[* operator<<`=]([@(0.0.255) const]_[_^Upp`:`:ValueMap^ ValueMap][@(0.0.255) `&
]_[*@3 m])&]
[s2;%RU-RU То же, что и Set([%-*@3 m]).&]
[s0;%RU-RU ]]