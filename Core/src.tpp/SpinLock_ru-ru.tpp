topic "Структура SpinLock";
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
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Структура SpinLock]]}}&]
[s3; &]
[s1;:SpinLock`:`:struct: [@(0.0.255)3 struct][3 _][*3 SpinLock][3 _:_][@(0.0.255)3 public][3 _][*@3;3 M
oveable][3 <][*3 SpinLock][3 >_]&]
[s2;%RU-RU Легковесный `"усердно ожидающий`" 
замок синхронизации. В отличие от 
Mutex, SpinLock ждёт в цикле до тоех пор пока 
ресурс не станет доступен, thus avoiding 
costs of contention system context switch at the price of active 
waiting. SpinLock methods are also usually inlined (and trivial). 
SpinLock is [* not] reentrant and also [* not] fair (if more threads 
are waiting on the same SpinLock, the order of acquiring it is 
not specified).&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Публичных Методов]]}}&]
[s3; &]
[s5;:SpinLock`:`:TryEnter`(`): [@(0.0.255) bool]_[* TryEnter]()&]
[s2;%RU-RU Пытается приобрести замок, возвращает 
true по его приобретению.&]
[s3; &]
[s4; &]
[s5;:SpinLock`:`:Enter`(`): [@(0.0.255) void]_[* Enter]()&]
[s2;%RU-RU Приобретает замок.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SpinLock`:`:Wait`(`): [@(0.0.255) void]_[* Wait]()&]
[s2;%RU-RU Ждёт шанса на приобретение замка.&]
[s3; &]
[s4; &]
[s5;:SpinLock`:`:Leave`(`): [@(0.0.255) void]_[* Leave]()&]
[s2;%RU-RU Освобождает замок.&]
[s3; &]
[s3; &]
[s0;*@7;4%RU-RU &]
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Класс SpinLock`::Lock]]}}&]
[s3; &]
[s3; &]
[s1;:SpinLock`:`:Lock`:`:class: [@(0.0.255)3 class][3 _][*3 Lock][3 _:_][@(0.0.255)3 private][3 _][*@3;3 N
oCopy]&]
[s2;%RU-RU This nested class automates calls to Mutex`::Enter / Mutex`::Leave 
for block of code using C`+`+ constructor / destructor rules. 
Using [* operator StaticMutex`::Mutex], it can be used with StaticMutex 
as well.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Конструктор  / Destructor detail]]}}&]
[s3; &]
[s5;:SpinLock`:`:Lock`:`:Lock`(SpinLock`&`): [* Lock]([_^SpinLock^ SpinLock][@(0.0.255) `&]_
[*@3 s])&]
[s2;%RU-RU Выполняет [%-*@3 s].Enter().&]
[s3;%RU-RU &]
[s4; &]
[s5;:SpinLock`:`:Lock`:`:`~Lock`(`): [@(0.0.255) `~][* Lock]()&]
[s2; [%RU-RU Выполняет ][*@3 s].Leave(), где [*@3 s] `- параметр 
конструктора.&]
[s0; ]]