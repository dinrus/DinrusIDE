topic "Класс Semaphore";
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
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Класс Semaphore]]}}&]
[s3; &]
[s1;:Semaphore`:`:class: [@(0.0.255)3 class][3 _][*3 Semaphore]&]
[s9;%RU-RU Хорошо знакомый инструмент многопоточно
й синхронизации. В U`+`+, он впервую 
очередь используется для блокирования/разбл
окирования выполнения потоков. У 
семафора есть внутренний счётчик, 
изначально инициализированный в 
ноль. Операция ожидания [%-* Wait] блокирует 
выполнение потока, пока счётчик находится 
на нуле, а затем уменьшает его на единицу. 
Операция [%-* Release ]увеличивает счётчик 
на 1.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Публичных Методов]]}}&]
[s3; &]
[s5;:Upp`:`:Semaphore`:`:Wait`(int`): [@(0.0.255) bool]_[* Wait]([@(0.0.255) int]_[*@3 timeou
t`_ms]_`=_`-[@3 1])&]
[s2;%RU-RU Если внутренний счётчик семафора 
на нуле, ждёт (блокирует вызывающие 
потоки) пока какой`-то другой поток 
увеличит этот счётчик на 1, вызывая 
метод Release, или пока не пройдёт [%-*@3 timeout`_ms] 
миллисекунд. Перед возвратом, уменьшает 
счётчик на 1. Отрицательное значение 
для [%-*@3 timeout`_ms] означает, что время 
ожидания не ограничено.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Semaphore`:`:Release`(`): [@(0.0.255) void]_[* Release]()&]
[s2;%RU-RU Увеличивает внутренний счётчик 
на 1.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Конструктор  detail]]}}&]
[s3; &]
[s5;:Semaphore`:`:Semaphore`(`): [* Semaphore]()&]
[s2;%RU-RU Инициализует внутренний счётчик 
в 0.&]
[s0; &]
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Класс StaticSemaphore]]}}&]
[s3; &]
[s1;:StaticSemaphore`:`:class: [@(0.0.255)3 class][3 _][*3 StaticSemaphore]&]
[s9;%RU-RU Вариант Semaphore, который может использоват
ься ка статическая или глобальная 
переменная без необходимости в инициализаци
и  `- у него нет конструктора и он корректно 
выполняет первую инициализацию, когда 
вызывается любой из методов. Это решает 
проблемы с порядком инициализации 
и многопоточной инициализацией.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Публичных Методов]]}}&]
[s3; &]
[s5;:StaticSemaphore`:`:Get`(`): [_^Semaphore^ Semaphore][@(0.0.255) `&]_[* Get]()&]
[s0; [* operator_Semaphore`&]()&]
[s2;%RU-RU Возвращает экземпляр Semaphore.&]
[s3;%RU-RU &]
[s4; &]
[s5;:StaticSemaphore`:`:Wait`(`): [@(0.0.255) void]_[* Wait]()&]
[s5;:StaticSemaphore`:`:Release`(`): [@(0.0.255) void]_[* Release]()&]
[s2;%RU-RU Вызывает соответствующий метод 
экземпляра класса Semaphore.&]
[s3; &]
[s0; ]]