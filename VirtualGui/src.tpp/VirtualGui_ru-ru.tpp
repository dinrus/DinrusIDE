topic "VirtualGui";
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
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Структура VirtualGui]]}}&]
[s3; &]
[s1;:Upp`:`:VirtualGui`:`:struct: [@(0.0.255)3 struct][3 _][*3 VirtualGui]&]
[s2;%RU-RU VirtualGui представляет собой интерфейс 
для реализации простого виртуализированног
о рабочего стола ГИП. Реализуя ряд 
методов, клиент получает в итоге рабочий 
стол ГИП, работающий над заданной 
поверхностью.&]
[s2;%RU-RU VirtualGUI представляет собой в основном 
`"событийную`" часть ГИП. Клиент также 
должен реализовать или использовать 
некоторый Draw, чтобы отрисовывать 
на поверхности, а также некоторую 
шрифтовую систему, чтобы измерять 
шрифты (напр., DrawGL и plugin/FT`_fontsys).&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Публичных Методов]]}}&]
[s3; &]
[s5;:Upp`:`:VirtualGui`:`:GetOptions`(`): [@(0.0.255) virtual] [_^Upp`:`:dword^ dword]_[* G
etOptions]()&]
[s2;%RU-RU Возвращает набор флагов, описывающий 
некоторые аспекты поведения VirtualGui. 
Доступные необязательные флаги следующие:-|&]
[s2;%RU-RU &]
[s7;i1120;a17;:Ctrl`:`:CENTER:%RU-RU [%-*C@3 GUI`_SETMOUSECURSOR]-|Ипользовать
 метод SetMouseCursor() вместо отрисовки 
курсора.&]
[s7;i1120;a17;:Ctrl`:`:STDSIZE:%RU-RU [%-*C@3 GUI`_SETCARET]-|Использовать 
метод SetCaret() вместо отрисовуи каретки.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:VirtualGui`:`:GetSize`(`): [@(0.0.255) virtual] [_^Upp`:`:Size^ Size]_[* GetSiz
e]()_`=_[@3 0]&]
[s2;%RU-RU Возвращает размер ГИП`-поверхности.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:VirtualGui`:`:GetMouseButtons`(`): [@(0.0.255) virtual] 
[_^Upp`:`:dword^ dword]_[* GetMouseButtons]()_`=_[@3 0]&]
[s2;%RU-RU Возвращает текущее состояние 
кнопок мыши.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:VirtualGui`:`:GetModKeys`(`): [@(0.0.255) virtual] [_^Upp`:`:dword^ dword]_[* G
etModKeys]()_`=_[@3 0]&]
[s2;%RU-RU Возвращает текущее состояние 
клавиш`-модификаторов.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:VirtualGui`:`:IsMouseIn`(`): [@(0.0.255) virtual] [@(0.0.255) bool]_[* IsMouseI
n]()_`=_[@3 0]&]
[s2;%RU-RU Возвращает true, если указатель 
мыши расположен на ГИП`-поверхности.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:VirtualGui`:`:IsWaitingEvent`(`): [@(0.0.255) virtual] 
[@(0.0.255) bool]_[* IsWaitingEvent]()_`=_[@3 0]&]
[s2;%RU-RU Возвращает true, если есть события 
ввода, нуждающиеся в обработке.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:VirtualGui`:`:ProcessEvent`(bool`*`): [@(0.0.255) virtual] 
[@(0.0.255) bool]_[* ProcessEvent]([@(0.0.255) bool]_`*[*@3 quit])_`=_[@3 0]&]
[s2;%RU-RU Processes input event. This function should use Ctrl`::DoKeyFB 
and Ctrl`::DoMouseFB static functions to pass events to virtual 
GUI. It can also use Ctrl`::PaintAll and Ctrl`::EndSession if approrate. 
It can set [%-*@3 quit] to true indicate the end of session initiated 
by host (quit can be NULL, so it is necessary to test it before 
setting it).&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:VirtualGui`:`:WaitEvent`(int`): [@(0.0.255) virtual] [@(0.0.255) void]_[* WaitE
vent]([@(0.0.255) int]_[*@3 ms])_`=_[@3 0]&]
[s2;%RU-RU Waits up to [%-*@3 ms] milliseconds until next input event 
is available.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:VirtualGui`:`:WakeUpGuiThread`(`): [@(0.0.255) virtual] 
[@(0.0.255) void]_[* WakeUpGuiThread]()_`=_[@3 0]&]
[s2;%RU-RU This should work as if `'empty`' input event comes to 
the queue, so that WithEvent returns immediately. This function 
can be called from different thread.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:VirtualGui`:`:SetMouseCursor`(const Upp`:`:Image`&`): [@(0.0.255) virtual] 
[@(0.0.255) void]_[* SetMouseCursor]([@(0.0.255) const]_[_^Upp`:`:Image^ Image][@(0.0.255) `&
]_[*@3 image])_`=_[@3 0]&]
[s2;%RU-RU Changes the mouse cursor. This is only used if GUI`_SETMOUSECURSOR 
in GetOptions flag is active.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:VirtualGui`:`:SetCaret`(const Upp`:`:Rect`&`): [@(0.0.255) void]_[* SetCaret](
[@(0.0.255) const]_[_^Upp`:`:Rect^ Rect][@(0.0.255) `&]_[*@3 caret])&]
[s2;%RU-RU Places the caret. This is only used if GUI`_SETMOUSECURSOR 
in GetOptions flag is active.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:VirtualGui`:`:Quit`(`): [@(0.0.255) virtual] [@(0.0.255) void]_[* Quit]()_`=_[@3 0
]&]
[s2;%RU-RU Called when the end of GUI operation is required.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:VirtualGui`:`:BeginDraw`(`): [@(0.0.255) virtual] [_^Upp`:`:SystemDraw^ Syste
mDraw][@(0.0.255) `&]_[* BeginDraw]()_`=_[@3 0]&]
[s2;%RU-RU Starts drawing on the surface. VirtualGui only requires 
single instance of SystemDraw to exist at any time, so it is 
ok to have corresponding Draw as an instance variable of the 
VirtualGui implementation.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:VirtualGui`:`:CommitDraw`(`): [@(0.0.255) virtual] [@(0.0.255) void]_[* CommitD
raw]()_`=_[@3 0]&]
[s2;%RU-RU Ends drawing operations, commits the result to be visible.&]
[s3; &]
[s0; &]
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 VirtualGui]]}}&]
[s4; &]
[s5;:Upp`:`:RunVirtualGui`(Upp`:`:VirtualGui`&`,Upp`:`:Event`<`>`): [@(0.0.255) void]_[* R
unVirtualGui]([_^Upp`:`:VirtualGui^ VirtualGui][@(0.0.255) `&]_[*@3 gui], 
[_^Upp`:`:Event^ Event]<>_[*@3 app`_main])&]
[s2;%RU-RU This function executes the virtual GUI session.&]
[s3;%RU-RU &]
[s0;%RU-RU ]]