topic "Класс BarCtrl (БрусКтрл)";
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
[ {{10000t/25b/25@3 [s0; [*@(229)4 Класс BarCtrl (БрусКтрл)]]}}&]
[s3; &]
[s1;:BarCtrl`:`:class: [@(0.0.255)3 class][3 _][*3 BarCtrl][3 _:_][@(0.0.255)3 public][3 _][*@3;3 Ba
r][3 , ][@(0.0.255)3 public][3 _][*@3;3 CtrlFrame]&]
[s9;%RU-RU В данном классе находятся общие 
методы для контролов MenuBar и ToolBar. Производный
 от Bar, он добавляет поддержку размещения 
виджетов, таких как  [^topic`:`/`/CtrlCore`/srcdoc`/AboutFrames`_ru`-ru^ ф
рейм] и строка справки о состоянии.&]
[s3;%RU-RU &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Публичных Членов]]}}&]
[s3; &]
[s5;:BarCtrl`:`:WhenHelp: [_^Callback1^ Callback1][@(0.0.255) <const]_String[@(0.0.255) `&>
]_[* WhenHelp]&]
[s2;%RU-RU Этот обратный вызов делается, 
когда статические методы SendHelpLine или 
ClearHelpLine запрашивают отображение строки 
справки о состоянии (status help line). Благодаря 
перегрузке операторов, этот обратный 
вызов можно непосредственно присваивать 
экземпляру StatusBar.&]
[s3;%RU-RU &]
[s4; &]
[s5;:BarCtrl`:`:WhenLeftClick: [_^Callback^ Callback]_[* WhenLeftClick]&]
[s2;%RU-RU Вызывается при пользовательском 
клике в области бара (бруса).&]
[s3;%RU-RU &]
[s4; &]
[s5;:BarCtrl`:`:GetBarCtrlParent`(Ctrl`*`): [@(0.0.255) static] [_^BarCtrl^ BarCtrl]_`*[* G
etBarCtrlParent]([_^Ctrl^ Ctrl]_`*[*@3 child])&]
[s2;%RU-RU Возвращает  первого родителя 
типа BarCtrl для отпрыска [%-*@3 child].&]
[s3;%RU-RU &]
[s4; &]
[s5;:BarCtrl`:`:SendHelpLine`(Ctrl`*`): [@(0.0.255) static] [@(0.0.255) void]_[* SendHelpLi
ne]([_^Ctrl^ Ctrl]_`*[*@3 q])&]
[s2;%RU-RU Если у [%-*@3 q] определена строка 
справки (методом Ctrl`::HelpLine), она отправляется 
этим методом к CtrlBar`'у, полученному 
в итоге вызова GetBarCtrlParent (если он есть) 
обратного вызова WhenHelp.&]
[s3;%RU-RU &]
[s4; &]
[s5;:BarCtrl`:`:ClearHelpLine`(Ctrl`*`): [@(0.0.255) static] [@(0.0.255) void]_[* ClearHelp
Line]([_^Ctrl^ Ctrl]_`*[*@3 q])&]
[s2;%RU-RU Отправляет пустую String к CtrlBar`'у, 
полученную при вызове GetBarCtrlParent (если 
он есть) обратного вызова WhenHelp.&]
[s3;%RU-RU &]
[s4; &]
[s5;:BarCtrl`:`:Align`(int`): [_^BarCtrl^ BarCtrl][@(0.0.255) `&]_[* Align]([@(0.0.255) int]_
[*@3 align])&]
[s2; [%RU-RU Устанавливает размещение бруса 
при его использование как фрейма, 
][*@3 align][%RU-RU  может быть одним из следующих 
вариантов `- BarCtrl`::][* BAR`_BOTTOM], [%RU-RU BarCtrl`::][* BAR`_TOP], 
[%RU-RU BarCtrl`::][* BAR`_RIGHT], [%RU-RU BarCtrl`::][* BAR`_LEFT]. Дефолтное 
значение равно BAR`_TOP.&]
[s3;%RU-RU &]
[s4; &]
[s5;:BarCtrl`:`:Top`(`): [_^BarCtrl^ BarCtrl][@(0.0.255) `&]_[* Top]()&]
[s2;%RU-RU То же, что и Align(BAR`_TOP).&]
[s3;%RU-RU &]
[s4; &]
[s5;:BarCtrl`:`:Bottom`(`): [_^BarCtrl^ BarCtrl][@(0.0.255) `&]_[* Bottom]()&]
[s2;%RU-RU То же, что и Align(BAR`_BOTTOM).&]
[s3;%RU-RU &]
[s4; &]
[s5;:BarCtrl`:`:Left`(`): [_^BarCtrl^ BarCtrl][@(0.0.255) `&]_[* Left]()&]
[s2;%RU-RU То же, что и Align(BAR`_LEFT).&]
[s3;%RU-RU &]
[s4; &]
[s5;:BarCtrl`:`:Right`(`): [_^BarCtrl^ BarCtrl][@(0.0.255) `&]_[* Right]()&]
[s2;%RU-RU То же, что и Align(BAR`_RIGHT).&]
[s3;%RU-RU &]
[s4; &]
[s5;:BarCtrl`:`:GetAlign`(`)const: [@(0.0.255) int]_[* GetAlign]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает текущую настройку 
Align.&]
[s3;%RU-RU &]
[s4; &]
[s5;:BarCtrl`:`:Wrap`(int`): [_^BarCtrl^ BarCtrl][@(0.0.255) `&]_[* Wrap]([@(0.0.255) int]_[*@3 q
]_`=_[@3 1])&]
[s2;%RU-RU Устанавливает режим `"обёртки`" 
строки (или колонки для вертикального 
бруса). 1 `- режим `"обёртки`" (wrap mode) активен, 
0 `- он активен для фрейма BarCtrl, 1 `- режим 
обёртки дезактивирован. Дефолт равен 
0 (активен в фрейме).&]
[s3;%RU-RU &]
[s4; &]
[s5;:BarCtrl`:`:NoWrap`(`): [_^BarCtrl^ BarCtrl][@(0.0.255) `&]_[* NoWrap]()&]
[s2;%RU-RU То же, что и Wrap(`-1).&]
[s3;%RU-RU &]
[s0; ]]