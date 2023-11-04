topic "Класс Function";
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
[ {{10000@(113.42.0) [s0; [*@7;4 Класс Function]]}}&]
[s3;%- &]
[s1;:noref:%- [@(0.0.255)3 template][3 _<][@(0.0.255)3 typename][3 _][*@4;3 Res][3 , 
][@(0.0.255)3 typename...][3 _][*@4;3 ArgTypes][3 >]&]
[s1;:Function`<Res`(ArgTypes`.`.`.`)`> `:`:class:%- [@(0.0.255) class]_[* Function<Res(Ar
gTypes...)>_]:_[@(0.0.255) private]_[*@3 Moveable]<Function<[*@4 Res]([*@4 ArgTypes][@(0.0.255) .
..])>>_&]
[s2; Upp`::Function является обёрткой для представления
 `"вызывабильной`" операции. Она подобна 
std`::function м двумя отличиями:&]
[s2;i150;O0; Допускается вызов пустой функции 
Function (и NOP). Возвращает ноль.&]
[s2;i150;O0; Функции могут комбинироваться 
(сцепляться) с помощью оператора<<.&]
[s3;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Список Публичных Методов]]}}&]
[s3; &]
[s5;:Function`<Res`(ArgTypes`.`.`.`)`> `:`:Function`(`):%- [* Function]()&]
[s5;:Upp`:`:Function`<Res`(ArgTypes`.`.`.`)`> `:`:Function`(const Upp`:`:Nuller`&`):%- [* F
unction]([@(0.0.255) const]_[_^Upp`:`:Nuller^ Nuller][@(0.0.255) `&])&]
[s2; Создаёт пустую Function. Вызов пустой 
функции является NOP и заведомое возвратное 
значение равно Res() (которое сводится 
к 0 для фундаментальных типов). Вариант 
с Nuller применим, когда объявляется 
определение функции с опциональными 
параметрами Function, типа [*C@5 void Foo(Function<void 
()> `= Null);]&]
[s3;%- &]
[s4; &]
[s5;:Function`<Res`(ArgTypes`.`.`.`)`> `:`:Function`(F`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 F]>_[* Function]([*@4 F]_[*@3 fn])&]
[s2; Это конструктор `'catch lambda`', позволяющий 
преобразовать генерную лямбду в значение 
Function.&]
[s3; &]
[s4; &]
[s5;:Function`<Res`(ArgTypes`.`.`.`)`> `:`:Function`(const Function`&`):%- [* Function](
[@(0.0.255) const]_[* Function][@(0.0.255) `&]_[*@3 src])&]
[s5;:Function`<Res`(ArgTypes`.`.`.`)`> `:`:operator`=`(const Function`&`):%- Function
[@(0.0.255) `&]_[* operator`=]([@(0.0.255) const]_Function[@(0.0.255) `&]_[*@3 src])&]
[s5;:Function`<Res`(ArgTypes`.`.`.`)`> `:`:Function`(Function`&`&`):%- [* Function]([* Fu
nction][@(0.0.255) `&`&]_[*@3 src])&]
[s5;:Function`<Res`(ArgTypes`.`.`.`)`> `:`:operator`=`(Function`&`&`):%- Function[@(0.0.255) `&
]_[* operator`=](Function[@(0.0.255) `&`&]_[*@3 src])&]
[s2; Обычные конструкторы копировать/переместит
ь и операторы присваивания.&]
[s3; &]
[s4; &]
[s5;:Function`<Res`(ArgTypes`.`.`.`)`> `:`:Proxy`(`)const:%- Function_[* Proxy]()_[@(0.0.255) c
onst]&]
[s2; Возвращает новую Function, которая, при 
вызове, вызывает данную Function. Применимо, 
когда какая`-то Function позднее должна 
измениться, но требуется другая Function 
для представления этой.&]
[s3; &]
[s4; &]
[s5;:Function`<Res`(ArgTypes`.`.`.`)`> `:`:operator`<`<`(F`):%- [@(0.0.255) template]_<
[@(0.0.255) class]_[*@4 F]>_Function[@(0.0.255) `&]_[* operator<<]([*@4 F]_[*@3 fn])&]
[s5;:Function`<Res`(ArgTypes`.`.`.`)`> `:`:operator`<`<`(const Function`&`):%- Functi
on[@(0.0.255) `&]_[* operator<<]([@(0.0.255) const]_Function[@(0.0.255) `&]_[*@3 fn])&]
[s5;:Function`<Res`(ArgTypes`.`.`.`)`> `:`:operator`<`<`(Function`&`&`):%- Function[@(0.0.255) `&
]_[* operator<<](Function[@(0.0.255) `&`&]_[*@3 fn])&]
[s2; Эти операторы комбинируют две Functions 
или функцию с лямбдой. Оригинальная 
функция `'this`' вызывается первой, а 
затем [%-*@3 fn]. Если любая из Functions возвращает 
значение, значение [%-*@3 fn] (аргумент 
справа от <<) представляет собой значение 
всей Function.&]
[s3; &]
[s4; &]
[s5;:Function`<Res`(ArgTypes`.`.`.`)`> `:`:operator`(`)`(ArgTypes`.`.`.args`)const:%- [*@4 R
es]_[* operator()]([*@4 ArgTypes][@(0.0.255) ...]_args)_[@(0.0.255) const]&]
[s2; Выполняет функции.&]
[s3; &]
[s4; &]
[s5;:Function`<Res`(ArgTypes`.`.`.`)`> `:`:operator bool`(`)const:%- [* operator_bool](
)_[@(0.0.255) const]&]
[s2; Возвращает true, если Function не пуст.&]
[s3; &]
[s4; &]
[s5;:Function`<Res`(ArgTypes`.`.`.`)`> `:`:Clear`(`):%- [@(0.0.255) void]_[* Clear]()&]
[s2; Устанавливает пустой Function.&]
[s0; &]
[s0;%- &]
[ {{10000@(113.42.0) [s0; [*@7;4 Event и Gate]]}}&]
[s3;%- &]
[s0;%- Двумя самыми общими типами Function 
в U`+`+ являются те, которые возвращают 
`'void`', и те, которые возвращают `'bool`'. 
В U`+`+ определено два удобных шаблонных 
алиаса:&]
[s0;%- &]
[s5;:Upp`:`:Event`:`:typedef:%- [@(0.0.255) template <typename... ][*@4 ArgTypes][@(0.0.255) >
 using]_[* Event]_`=_Function<[@(0.0.255) void]_(ArgTypes[@(0.0.255) ...])>&]
[s2; Создаёт алиас для Function, возвращающей 
void. Например, [*C@5 Event<int, String>] эквивалентно 
[*C@5 Function<void (int, String)>].&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Gate`:`:typedef:%- [@(0.0.255) template <typename... ][*@4 ArgTypes][@(0.0.255) >
 using]_[* Gate]_`=_Function<[@(0.0.255) bool]_(ArgTypes[@(0.0.255) ...])>&]
[s2; Создаёт алиас для Function, возвращающей 
bool. Например, [*C@5 Gate<int, String>] эквивалентно 
[*C@5 Function<bool (int, String)>]. Заметьте, что пустой 
Gate возвращает false (так как пустая Function 
возвращает bool(0), что равно false).&]
[s3;%- &]
[s0;%- &]
[ {{10000@(113.42.0) [s0; [*@7;4 MemFn и THISFN]]}}&]
[s3;%- &]
[s3;%- &]
[s0;%- Несмотря на мощь C`+`+11, бывают ситуации, 
когда лучше помещать вызываемый код 
в функцию`-член. Лямбда`-синтаксис 
C`+`+11 в таких случаях непригоден. Шаблонные 
функции MemFn и THISFN служат для уменьшения 
избыточности типирования.&]
[s0;%- &]
[s5;:Upp`:`:MemFn`(Upp`:`:Ptr`,Res`(`*`)`(ArgTypes`.`.`.`)`):%- [@(0.0.255) template]_<
[@(0.0.255) class]_[*@4 Ptr], [@(0.0.255) class]_[*@4 Class], [@(0.0.255) class]_[*@4 Res], 
[@(0.0.255) class...]_[*@4 ArgTypes]>_[_^Upp`:`:Event^ Event]<[*@4 ArgTypes][@(0.0.255) ...
]>_[* MemFn]([*@4 Ptr]_[*@3 object], [*@4 Res]_(Class`::[@(0.0.255) `*][*@3 member`_function])
([*@4 ArgTypes][@(0.0.255) ...]))&]
[s2; Эквивалентно std`::mem`_fn, возвращающей 
U`+`+ Function. Создаёт Function, представляющуб 
собой вызов фцнкции`-члена [%-*@3 member`_function] 
экземпляра объекта [%-*@3 object].&]
[s3; &]
[s4;%- &]
[s5;:THISFN`(x`):%- [* THISFN](x)&]
[s2; Этот макрос определён как MemFn(this, 
`&CLASSNAME`::x).&]
[s3;%- &]
[s0;@(0.0.255)3%- ]]