topic "class RealTimeStop";
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
[{_}%EN-US 
[ {{10000@(113.42.0) [s0; [*@7;4 Класс RealTimeStop]]}}&]
[s4;2 &]
[s1;:Upp`:`:RealTimeStop`:`:class: [@(0.0.255)3 class][3 _][*3 RealTimeStop]&]
[s2; &]
[s0; Класс RealTimeStop представляет таймер, 
который можно поставить на паузу 
и установить обратно.&]
[s0; Кроме того, он мониторирует то, поставлена 
ли главная программа га паузу другим 
процессом или из`-за зависания компьютера, 
он саморегулируется и фиксирует только 
прошедшее время, а не время часов.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0; [* Список Публичных Методов]]}}&]
[s4; &]
[s5;:Upp`:`:RealTimeStop`:`:Start`(`): [@(0.0.255) void]_[* Start]()&]
[s2; Запускает таймер.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:RealTimeStop`:`:Pause`(bool`): [@(0.0.255) void]_[* Pause]([@(0.0.255) bool]_[*@3 p
ause])&]
[s2; Если [*@3 pause] равно true, таймер становится 
на паузу, если false, продолжает выполняться.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:RealTimeStop`:`:Pause`(`): [@(0.0.255) void]_[* Pause]()&]
[s2; Ставит таймер на паузу&]
[s3; &]
[s4; &]
[s5;:Upp`:`:RealTimeStop`:`:Continue`(`): [@(0.0.255) void]_[* Continue]()&]
[s2; Продолжает выполнение таймера&]
[s3; &]
[s4; &]
[s5;:Upp`:`:RealTimeStop`:`:Seconds`(`): [@(0.0.255) double]_[* Seconds]()&]
[s2; Возвращает количество прошедших 
секунд&]
[s3; &]
[s4; &]
[s5;:Upp`:`:RealTimeStop`:`:SetBack`(double`): [@(0.0.255) void]_[* SetBack]([@(0.0.255) do
uble]_[*@3 secs])&]
[s2; Устанавливает обратно таймер на 
[*@3 secs] секунд.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:RealTimeStop`:`:IsPaused`(`): [@(0.0.255) bool]_[* IsPaused]()&]
[s2; Возвращает true, если на паузе.&]
[s3;%- &]
[s0; ]]