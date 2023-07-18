topic "Класс RWMutex";
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
[ {{10000t/25b/25@(113.42.0) [s0; [*@7;4 Класс RWMutex]]}}&]
[s3; &]
[s1;:RWMutex`:`:class: [@(0.0.255)3 class][3 _][*3 RWMutex][3 _:_][@(0.0.255)3 private][3 _][*@3;3 N
oCopy]&]
[s9;%RU-RU This class mediates reader`-writer sharing of global data. 
Only single thread can EnterWrite, but any number of threads 
can EnterRead. RWMutex is NOT reentrant (same thread can Enter 
the Mutex multiple times).&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Публичных Методов]]}}&]
[s3; &]
[s5;:RWMutex`:`:EnterWrite`(`): [@(0.0.255) void]_[* EnterWrite]()&]
[s2;%RU-RU Blocks until all threads Leave the RWMutex, then enters 
it and blocks any threads to enter the RWMutex until invoking 
LeaveWrite().&]
[s3; &]
[s4; &]
[s5;:RWMutex`:`:LeaveWrite`(`): [@(0.0.255) void]_[* LeaveWrite]()&]
[s2;%RU-RU Leaves the RWMutex.&]
[s3; &]
[s4; &]
[s5;:RWMutex`:`:EnterRead`(`): [@(0.0.255) void]_[* EnterRead]()&]
[s2;%RU-RU Enter in read mode. More than single thread can enter 
the RWMutex in read mode. Blocks any threads attempting EnterWrite, 
until all reader threads leave using LeaveRead.&]
[s3; &]
[s4; &]
[s5;:RWMutex`:`:LeaveRead`(`): [@(0.0.255) void]_[* LeaveRead]()&]
[s2;%RU-RU Leave the read mode.&]
[s3; &]
[s0; &]
[s0; &]
[s0; &]
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Класс StaticRWMutex]]}}&]
[s3; &]
[s1;:StaticRWMutex`:`:class: [@(0.0.255)3 class][3 _][*3 StaticRWMutex]&]
[s9;%RU-RU Variant of RWMutex that can be used as static or global 
variable without the need of initialization  `- it has no constructor 
and correctly performs the first initialization when any of methods 
is called. That avoids problems with initialization order or 
multithreaded initialization issues.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Публичных Методов]]}}&]
[s3; &]
[s5;:StaticRWMutex`:`:Get`(`): [_^RWMutex^ RWMutex][@(0.0.255) `&]_[* Get]()&]
[s5;:StaticRWMutex`:`:operator RWMutex`&`(`): [* operator_RWMutex`&]()&]
[s2;%RU-RU Возвращает the instance of RWMutex.&]
[s3; &]
[s4; &]
[s5;:StaticRWMutex`:`:EnterRead`(`): [@(0.0.255) void]_[* EnterRead]()&]
[s5;:StaticRWMutex`:`:LeaveRead`(`): [@(0.0.255) void]_[* LeaveRead]()&]
[s5;:StaticRWMutex`:`:EnterWrite`(`): [@(0.0.255) void]_[* EnterWrite]()&]
[s5;:StaticRWMutex`:`:LeaveWrite`(`): [@(0.0.255) void]_[* LeaveWrite]()&]
[s2;%RU-RU Вызывает respective RWMutex methods.&]
[s3; &]
[s0; &]
[s0; &]
[s0; &]
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Класс RWMutex`::ReadLock]]}}&]
[s3; &]
[s1;:RWMutex`:`:ReadLock`:`:class: [@(0.0.255)3 class][3 _][*3 ReadLock]&]
[s9;%RU-RU This nested class automates calls to Mutex`::EnterRead 
/ Mutex`::LeaveRead for block of code using C`+`+ constructor 
/ destructor rules. Using [* operator StaticRWMutex`::RWMutex], 
it can be used with StaticRWMutex as well.&]
[s3;%RU-RU &]
[s0;%RU-RU &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Конструктор  / Destructor detail]]}}&]
[s3; &]
[s5;:RWMutex`:`:ReadLock`:`:ReadLock`(RWMutex`&`): [* ReadLock]([_^RWMutex^ RWMutex][@(0.0.255) `&
]_[*@3 s])&]
[s2;%RU-RU Вызывает [%-*@3 s].EnterRead().&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:RWMutex`:`:ReadLock`:`:`~ReadLock`(`): [@(0.0.255) `~][* ReadLock]()&]
[s2; [%RU-RU Вызывает s.LeaveRead()] where [*@3 s] is the constructor parameter.&]
[s3;%RU-RU &]
[s0; &]
[s0; &]
[s0; &]
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Класс RWMutex`::WriteLock]]}}&]
[s3; &]
[s1;:RWMutex`:`:WriteLock`:`:class: [@(0.0.255)3 class][3 _][*3 WriteLock]&]
[s9; This nested class automates calls to Mutex`::EnterWrite / Mutex`::LeaveLeave 
for block of code using C`+`+ constructor / destructor rules. 
Using [* operator StaticRWMutex`::RWMutex], it can be used with 
StaticRWMutex as well.&]
[s3; &]
[s0; &]
[ {{10000t/25b/25@1 [s0; [* Конструктор  / Destructor detail]]}}&]
[s3; &]
[s5;:RWMutex`:`:WriteLock`:`:WriteLock`(RWMutex`&`): [* WriteLock]([_^RWMutex^ RWMutex][@(0.0.255) `&
]_[*@3 s])&]
[s2;%RU-RU Вызывает [%-*@3 s].EnterWrite().&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:RWMutex`:`:WriteLock`:`:`~WriteLock`(`): [@(0.0.255) `~][* WriteLock]()&]
[s2;%RU-RU Вызывает [*@3 s].LeaveWrite() where [*@3 s] is the constructor 
parameter.&]
[s3; &]
[s0; ]]