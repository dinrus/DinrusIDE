topic "Программирование ГИП и многопоточность";
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[a83;*R6 $$10,0#31310162474203024125188417583966:caption]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_}%RU-RU 
[s10; [@5 Программирование ГИП и многопоточность]&]
[s0; Многопоточность при модели программировани
я ГИП (GUI) имеет ограничения целевой 
платформы, так как большинство целевых 
платформ имеют понятие `"главного 
потока (main thread)`" и действия ГИП во 
второстепенных потоках ограничиваются.&]
[s0; &]
[s0; Всё, что манипулирует состоянием 
верхнеуровневых окон (TopWindow или popups), 
и всё, что связано с событийным циклом, 
должно выполняться в главном потоке. 
Единственная неконстантная операция, 
дозволенная неглавным потокам над 
верхнеуровневым окном,`- это Refresh.&]
[s0; &]
[s0; Неглавным потокам, в целом, разрешается 
изменять состояние виджетов, если 
они при этом сериализуют доступ через 
единый глобальный ГИП`-мютекс, либо 
через EnterGuiMutex/LeaveGuiMutex или же через 
вспомогательный гвард`-класс GuiLock 
(он вызывает в конструкторе EnterGuiMutex, 
а в деструкторе LeaveGuiMutex). GuiLock допускает 
повторный вход и оптимизирован под 
 great reentrant performance.Но НЕ нужно использовать 
GuiLock в рядовых методах обработки (типа 
MouseLeft или Key), та как U`+`+ выполняет блокировку 
самостоятельно.&]
[s0; &]
[s0; Другим средством коммуникации неглавных 
потоков с главным является PostCallback, 
который `"постирует`" Event в очередь 
таймера для немедленной обработки, 
но не дожидатся её окончания. А также 
Ctrl`::Call, который передаёт событие, нуждающееся
 в обработки, в главный поток и возвращает, 
когда это событие выполнится в главном 
потоке. Оба эти метода имеют довольно 
высокую латентность, GuiLock и прямой 
доступ следует предпочесть (по возможности).
&]
[s0; ]]