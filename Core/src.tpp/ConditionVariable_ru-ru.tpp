topic "Класс ConditionVariable";
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
[{_} 
[ {{10000t/25b/25@(113.42.0) [s0; [%RU-RU*@7;4 Класс ][*@7;4 ConditionVariable]]}}&]
[s3; &]
[s1;:ConditionVariable`:`:class: [@(0.0.255)3 class][3 _][*3 ConditionVariable]&]
[s9;%RU-RU ConditionVariable позволяет потокам `"подвешиват
ь`" своё выполнение (используя Wait), 
до тех пор пока их не `"разбудит`" другой 
поток (thread, т.е. `"нить`") (используя 
методы Signal или Broadcast). ConditionVariable имеет 
ассоциированный с ней Mutex (`"стопор`"), 
во избежание возможных условий `"гона`" 
(race conditions) при входе в `"подвешенное`" 
состояние (suspended state).&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Публичных Методов]]}}&]
[s3; &]
[s5;:Upp`:`:ConditionVariable`:`:Wait`(Upp`:`:Mutex`&`,int`): [@(0.0.255) void]_[* Wait](
[_^Upp`:`:Mutex^ Mutex][@(0.0.255) `&]_[*@3 m], [@(0.0.255) int]_[*@3 timeout`_ms]_`=_`-[@3 1
])&]
[s2;%RU-RU Атомично разблокирует [%-*@3 m ]и запускает 
процесс ожидания Signal, или Broadcast, или 
до тех пор пока не истекут [%-*@3 timeout`_ms] 
миллисекунд. [%-*@3 m] должно находиться 
во владении вызывающей нити перед 
вызовом. Когда получены Signal или Broadcast, 
выполнение нити возобновляется и 
происходит повторное `"овладение`" 
[%-*@3 m][%- . Отрицательное значение] [%-*@3 timeout`_ms] 
означает, что время ожидания неограничено.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:ConditionVariable`:`:Signal`(`): [@(0.0.255) void]_[* Signal]()&]
[s2;%RU-RU Возобновляет выполнение единичной 
ожидающей нити, если она есть.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:ConditionVariable`:`:Broadcast`(`): [@(0.0.255) void]_[* Broadcast]()&]
[s2;%RU-RU Возобновляет выполнение всех 
в данный момент ожидающих нитей.&]
[s3;%RU-RU &]
[s0;3 &]
[s0;3 &]
[s0;3 &]
[ {{10000t/25b/25@(113.42.0) [s0; [*@7;4 Класс StaticConditionVariable]]}}&]
[s3;%RU-RU &]
[s1;:StaticConditionVariable`:`:class: [@(0.0.255)3 class][3 _][*3 StaticConditionVariable]&]
[s9; Вариант ConditionVariable, который можно 
использовать как статическую или 
глобальную переменную без нужды в 
её инициализации: у него нет конструктора 
и он чётко выполняет перввую инициализацию 
при вызове любого из методов. Это 
позволяет избежать проблем с порядком 
инициализации или проблем многопоточной 
инициализации.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Публичных Методов]]}}&]
[s3; &]
[s5;:StaticConditionVariable`:`:Get`(`): [_^ConditionVariable^ ConditionVariable][@(0.0.255) `&
]_[* Get]()&]
[s5;:StaticConditionVariable`:`:operator ConditionVariable`&`(`): [* operator_Condition
Variable`&]()&]
[s2;%RU-RU Возвращает экземпляр ConditionVariable.&]
[s3; &]
[s4; &]
[s5;:StaticConditionVariable`:`:Wait`(Mutex`&`): [@(0.0.255) void]_[* Wait]([_^Mutex^ Mutex
][@(0.0.255) `&]_[*@3 m])&]
[s5;:StaticConditionVariable`:`:Signal`(`): [@(0.0.255) void]_[* Signal]()&]
[s5;:StaticConditionVariable`:`:Broadcast`(`): [@(0.0.255) void]_[* Broadcast]()&]
[s2;%RU-RU Вызывают соответствующий метод 
ConditionVariable.&]
[s3; &]
[s0; ]]