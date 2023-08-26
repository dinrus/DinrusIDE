topic "Класс TopWindow";
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
[ {{10000t/25b/25@3 [s0; [*@(229)4 Класс TopWindow]]}}&]
[s3; &]
[s1;:TopWindow`:`:class: [@(0.0.255)3 class][3 _][*3 TopWindow][3 _:_][@(0.0.255)3 public][3 _][*@3;3 C
trl]&]
[s9;%RU-RU TopWindow представляет собой верхнеуровнево
е независимое окно хостовой ОС, обычно 
декорированное заголовком, кнопками 
закрыть/свернуть/развернуть/восстановить 
и проч.&]
[s9;%RU-RU Важно запомнить, что в прямоугольник 
окна (определённый методами Ctrl`::SetRect 
и Ctrl`::GetRect) не входят никакие специфичные 
для хостового ГИП декорации (заголовок, 
толстый бордюр для перемера), а только 
область, `"контролируемая`" U`+`+.&]
[s9;%RU-RU Когда TopWindow открыто, оно дефолтно 
центрировано относительно его владельца 
(если он есть). Это поведение можно 
изменить методами NoCenter или CenterScreen.&]
[s3;%RU-RU &]
[s9; [%RU-RU/ Производный от][%RU-RU  ][^topic`:`/`/CtrlCore`/src`/Ctrl`_ru`-ru`#Ctrl`:`:class^ C
trl]&]
[s3;%RU-RU &]
[s0;%RU-RU &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Публичных Членов]]}}&]
[s3; &]
[s5;:TopWindow`:`:Reject`(`): [@(0.0.255) virtual] [@(0.0.255) void]_[* Reject]()&]
[s2;%RU-RU Этот виртуальный метод перегружен 
на выполнение операции Restore, если 
был какой`-либо предшествующий вызов 
метода Backup.&]
[s3; &]
[s4; &]
[s5;:TopWindow`:`:Close`(`): [@(0.0.255) virtual] [@(0.0.255) void]_[* Close]()&]
[s2;%RU-RU Этот виртуальный метод реализован 
в TopWindow так, что он прерывает модальный 
цикл, если TopWindow выполняет его. Если 
имеется Breaker с определением IDCANCEL, 
выполняется RejectBreak(IDCANCEL). Иначе, если 
имеется Breaker с IDYES (as assigned to exit Ctrl by 
CtrlLayoutExit), выполняется AcceptBreak(IDYES). Если 
ничего из этого не верно, то выполняется 
AcceptBreak(IDOK).&]
[s2;%RU-RU Если TopWindow не выполняет модального 
цикла, оно закрывается, `- в этом случае, 
любой бэкап состояния, созданный 
методом Backup, is scratched. Перепись в классе`-потом
ке может вызывать `'delete this`' в самом 
крайнем случае (покуда экземпляр 
размещается на куче).&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:WhenClose: [_^Callback^ Event<>]_[* WhenClose]&]
[s2;%RU-RU Этот обратный вызов дефолтно 
вызывается при нажатии кнопки Закрыть 
хостового окна ГИП. Дефолтно присваивается 
действие, выполняющее виртуальный 
метод Close. Присвоенное событие может 
вызывать `'delete this`' в самом крайнем 
случае (покуда экземпляр размещается 
на куче).&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:Backup`(`): [@(0.0.255) void]_[* Backup]()&]
[s2;%RU-RU Сериализует содержимое всех входящих 
контролов (виртуальным методом [^topic`:`/`/CtrlCore`/src`/Ctrl`_ru`-ru`#Ctrl`:`:Serialize`(Stream`&`)^ S
erialize]) во внутреннем хранилище TopWindow. 
Используется при реализации кнопок 
Отмена.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:Restore`(`): [@(0.0.255) void]_[* Restore]()&]
[s2;%RU-RU Восстанавливает содержимое всех 
входящих контролов из внутреннего 
хранилища. Если внутреннее хранилище 
пустое, ничего не делает.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:Break`(int`): [@(0.0.255) void]_[* Break]([@(0.0.255) int]_[*@3 ID]_`=_IDEX
IT)&]
[s2;%RU-RU Если выполняется цикл TopWindow (смотрите 
методы [^topic`:`/`/CtrlCore`/src`/TopWindow`_ru`-ru`#TopWindow`:`:Run`(bool`)^ R
un] или [^topic`:`/`/CtrlCore`/src`/TopWindow`_ru`-ru`#TopWindow`:`:RunAppModal`(`)^ R
unAppModal]), прерывает этот цикл с обозначенным 
кодом выхода.&]
[s7;%RU-RU [%-*C@3 ID]-|Код выхода.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:AcceptBreak`(int`): [@(0.0.255) bool]_[* AcceptBreak]([@(0.0.255) int]_[*@3 I
D])&]
[s2;%RU-RU Пытается принять все включаемые 
контролы, вызывая Accept для всех отпрысков, 
пока один из них не вернёт false, `- тогда 
и этот метод возвращает false. Если все 
отпрыски возвратят true, вызовет Break 
с указанным ID и вернёт true.&]
[s7;%RU-RU [%-*C@3 ID]-|Код выхода.&]
[s7;%RU-RU [*/ Возвратное значение]-|True, если 
все отпрыски были приняты и вызван 
Break(ID).&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:RejectBreak`(int`): [@(0.0.255) void]_[* RejectBreak]([@(0.0.255) int]_[*@3 I
D])&]
[s2;%RU-RU Отвергает все включаемые виджеты, 
вызывая Reject для всех отпрысков, затем 
вызывает Break с указанным ID.&]
[s7;%RU-RU [%-*C@3 ID]-|Код выхода.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:WorkAreaTrim`(`): [@(0.0.255) void]_[* WorkAreaTrim]()&]
[s2;%RU-RU Настраивает позицию окна так, 
чтобы оно не превышало пространство 
рабочего стола. Лишние части обрезаются 
(размер окна уменьшается).&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:Breaker`(int`): [_^Callback^ Callback]_[* Breaker]([@(0.0.255) int]_[*@3 ID
]_`=_IDEXIT)&]
[s2;%RU-RU Возвращает Callback, который при вызове 
вызывает [* Break] с заданным кодом выхода. 
Заметьте, что Callback как возвратное 
значение используется для обратной 
совместимости с pre`-C`+`+11 U`+`+ и для практически
х целей, но можно считать, что возвращается 
Event<>.&]
[s7;%RU-RU [%-*C@3 ID]-|Код выхода из цикла сообщений.&]
[s7;%RU-RU [*/ Возвратное значение]-|Callback.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:Acceptor`(int`): [_^Callback^ Callback]_[* Acceptor]([@(0.0.255) int]_[*@3 I
D])&]
[s2;%RU-RU Возвращает Callback, который при вызове 
вызывает [* AcceptBreak] с заданным кодом 
выхода. Заметьте, что Callback как возвратное 
значение используется для обратной 
совместимости с pre`-C`+`+11 U`+`+ и для практически
х целей, но можно считать, что возвращается 
Event<>.&]
[s7;%RU-RU [%-*C@3 ID]-|Код выхода из цикла сообщений.&]
[s7;%RU-RU [*/ Возвратное значение]-|Callback.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:Rejector`(int`): [_^Callback^ Callback]_[* Rejector]([@(0.0.255) int]_[*@3 I
D])&]
[s2;%RU-RU Возвращает Callback, который при вызове 
вызывает [* RejectBreak] с заданным кодом 
выхода. Заметьте, что Callback как возвратное 
значение используется для обратной 
совместимости с pre`-C`+`+11 U`+`+ и для практически
х целей, но можно считать, что возвращается 
Event<>.&]
[s7;%RU-RU [%-*C@3 ID]-|Код выхода из цикла сообщений.&]
[s7;%RU-RU [*/ Возвратное значение]-|Callback.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:Breaker`(Ctrl`&`,int`): [_^TopWindow^ TopWindow][@(0.0.255) `&]_[* Breake
r]([_^Ctrl^ Ctrl][@(0.0.255) `&]_[*@3 m], [@(0.0.255) int]_[*@3 ID]_`=_`-[@3 1])&]
[s2;%RU-RU То же, что и m <<`= [* Breaker](ID) `- присваивает 
обрвыз [* Breaker] методу WhenAction указанного 
контрола.&]
[s7;%RU-RU [%-*C@3 m]-|Ctrl.&]
[s7;%RU-RU [%-*C@3 ID]-|Код выхода из цикла сообщений.&]
[s7;%RU-RU [*/ Возвратное значение]-|`*this.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:Acceptor`(Ctrl`&`,int`): [_^TopWindow^ TopWindow][@(0.0.255) `&]_[* Accep
tor]([_^Ctrl^ Ctrl][@(0.0.255) `&]_[*@3 m], [@(0.0.255) int]_[*@3 ID])&]
[s2;%RU-RU То же, что и m <<`= [* Acceptor](ID) `- присваивает 
обрвыз [* Acceptor] методу WhenAction указанного 
контрола.&]
[s7;%RU-RU [%-*C@3 m]-|Ctrl.&]
[s7;%RU-RU [%-*C@3 ID]-|Код выхода из цикла сообщений.&]
[s7;%RU-RU [*/ Возвратное значение]-|`*this.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:Rejector`(Ctrl`&`,int`): [_^TopWindow^ TopWindow][@(0.0.255) `&]_[* Rejec
tor]([_^Ctrl^ Ctrl][@(0.0.255) `&]_[*@3 m], [@(0.0.255) int]_[*@3 ID])&]
[s2;%RU-RU То же, что и m <<`= [* Rejector](ID) `- присваивает 
обрвыз [* Rejector] методу WhenAction указанного 
контрола.&]
[s7;%RU-RU [%-*C@3 m]-|Ctrl.&]
[s7;%RU-RU [%-*C@3 ID]-|Код выхода из цикла сообщений.&]
[s7;%RU-RU [*/ Возвратное значение]-|`*this.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:NoCenter`(`): [_^TopWindow^ TopWindow][@(0.0.255) `&]_[* NoCenter]()&]
[s2;%RU-RU Устанавливает TopWindow нецентрированным 
по экрану при создании. Возвращает 
`*this.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:CenterOwner`(`): [_^TopWindow^ TopWindow][@(0.0.255) `&]_[* CenterOwner](
)&]
[s2;%RU-RU Устанавливает TopWindow центрированным 
относительно его владельца при создании 
на экране. Это дефолт. Возвращает 
`*this.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:CenterScreen`(`): [_^TopWindow^ TopWindow][@(0.0.255) `&]_[* CenterScreen
]()&]
[s2;%RU-RU Устанавливает TopWindow при создании 
центрированным на экране относительно 
рабочего стола. Возвращает `*this.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:SetMinSize`(Size`): [@(0.0.255) virtual] [@(0.0.255) void]_[* SetMinSize](
[_^Size^ Size]_[*@3 sz])&]
[s2;%RU-RU Устанавливает минимальный размер 
окна. Затем этот размер возвращается 
виртуальной функцией GetMinSize. Если 
у окна можно изменять размер, оно 
не может быть меньше этого размера.&]
[s7;%RU-RU [%-*C@3 sz]-|Минимальный размер окна.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:Open`(HWND`): [@(0.0.255) void]_[* Open](HWND_[*@3 ownerhwnd])&]
[s2;%RU-RU Открывает окно с владельцем, указанным 
как Win32 handle.&]
[s6;%RU-RU Специфично для Win32.&]
[s7;%RU-RU [%-*C@3 ownerhwnd]-|Хэндл окна владельца.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:Style`(dword`): [_^TopWindow^ TopWindow][@(0.0.255) `&]_[* Style]([_^dword^ d
word]_[*@3 `_style])&]
[s2;%RU-RU Устанавливает стиль окна Win32.&]
[s6;%RU-RU Специфично для Win32.&]
[s7;%RU-RU [%-*C@3 `_style]-|Стиль.&]
[s7;%RU-RU [*/ Возвратное значение]-|`*this.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:GetStyle`(`)const: [_^dword^ dword]_[* GetStyle]()_[@(0.0.255) const]&]
[s2;%RU-RU Устанавливает расширенный стиль 
окна Win32.&]
[s6;%RU-RU Специфично для Win32.&]
[s7;%RU-RU [*/ Возвратное значение]-|`*this.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:ExStyle`(dword`): [_^TopWindow^ TopWindow][@(0.0.255) `&]_[* ExStyle]([_^dword^ d
word]_[*@3 `_exstyle])&]
[s6;%RU-RU Специфично для Win32.&]
[s7;%RU-RU [%-*C@3 `_exstyle]-|Стиль&]
[s7;%RU-RU [*/ Возвратное значение]-|`*this&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:GetExStyle`(`)const: [_^dword^ dword]_[* GetExStyle]()_[@(0.0.255) const]&]
[s6;%RU-RU Специфично для Win32.&]
[s7;%RU-RU [*/ Возвратное значение]-|Стиль&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:Open`(Ctrl`*`): [@(0.0.255) void]_[* Open]([_^Ctrl^ Ctrl]_`*[*@3 owner])&]
[s2;%RU-RU ОткрываетTopWindow как окно в ГИП 
хоста с заданным владельцем. Окно 
может быть открыто в скрытой форме, 
если флаг видимости установлен в 
false (Hide()), однако, при закрытии окно 
устанавливается в видимое.&]
[s7;%RU-RU [%-*C@3 owner]-|Владелец окна. Если NULL, 
у окна нет хозяина.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:Open`(`): [@(0.0.255) void]_[* Open]()&]
[s2;%RU-RU Открывает TopWindow, владелец которого 
текущее активное окно, если им владеет 
данный процесс.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:OpenMain`(`): [@(0.0.255) void]_[* OpenMain]()&]
[s2;%RU-RU Открывает окно без владельца.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:Run`(bool`): [@(0.0.255) int]_[* Run]([@(0.0.255) bool]_[*@3 appmodal]_`=_[@(0.0.255) f
alse])&]
[s2;%RU-RU Выполняет модальный цикл для 
TopWindow. Если TopWindow не открыт, вызывает 
перед запуском цикла Open, но не закрывает 
его.&]
[s7;%RU-RU [%-*C@3 appmodal]-|Если true, при выполнении 
цикла все окна приложения дезактивируются. 
Если false, дезактивируются только относящиеся
 к TopWindow окна (с одинаковым `"супер`"`-владельце
м).&]
[s7;%RU-RU [*/ Возвратное значение]-|Итоговый 
код модального цикла.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:RunAppModal`(`): [@(0.0.255) int]_[* RunAppModal]()&]
[s2;%RU-RU То же, что и Run(true).&]
[s7;%RU-RU [*/ Возвратное значение]-|Итоговый 
код модального цикла.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:Execute`(`): [@(0.0.255) int]_[* Execute]()&]
[s2;%RU-RU Подобно Run, но по окончании цикла 
закрывает TopWindow.&]
[s7;%RU-RU [*/ Возвратное значение]-|Итоговый 
код модального цикла..&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:ExecuteOK`(`): [@(0.0.255) bool]_[* ExecuteOK]()&]
[s2;%RU-RU То же, что и Execute, но возвращает 
true, если кнопка OK нажата.&]
[s7;%RU-RU [*/ Возвратное значение]-|Итог true, 
если OK, иначе false.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:ExecuteCancel`(`): [@(0.0.255) bool]_[* ExecuteCancel]()&]
[s2;%RU-RU То же, что и Execute, но возвращает 
true, если нажата кнопка Отмена.&]
[s7;%RU-RU [*/ Возвратное значение]-|Итог true, 
если Отмена, иначе false.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:Minimize`(bool`): [@(0.0.255) void]_[* Minimize]([@(0.0.255) bool]_[*@3 eff
ect]_`=_[@(0.0.255) false])&]
[s2;%RU-RU Минимизировать окно.&]
[s7;%RU-RU [%-*C@3 effect]-|Выполняет платформо`-специфичные
 ГИП`-эффекты.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:Maximize`(bool`): [@(0.0.255) void]_[* Maximize]([@(0.0.255) bool]_[*@3 eff
ect]_`=_[@(0.0.255) false])&]
[s2;%RU-RU Максимировать окно.&]
[s7;%RU-RU [%-*C@3 effect]-|Выполняет платформо`-специфичные
 ГИП`-эффекты.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:Overlap`(bool`): [@(0.0.255) void]_[* Overlap]([@(0.0.255) bool]_[*@3 effec
t]_`=_[@(0.0.255) false])&]
[s2;%RU-RU Привести окно в нормальное overlapped 
состояние.&]
[s7;%RU-RU [%-*C@3 effect]-|Выполняет платформо`-специфичные
 ГИП`-эффекты.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:IsMaximized`(`)const: [@(0.0.255) bool]_[* IsMaximized]()_[@(0.0.255) con
st]&]
[s7;%RU-RU [*/ Возвратное значение]-|True, если 
TopWindow максимировано.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:IsMinimized`(`)const: [@(0.0.255) bool]_[* IsMinimized]()_[@(0.0.255) con
st]&]
[s7;%RU-RU [*/ Возвратное значение]-|True, если 
TopWindow минимировано.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:IsOverlapped`(`)const: [@(0.0.255) bool]_[* IsOverlapped]()_[@(0.0.255) c
onst]&]
[s7;%RU-RU [*/ Возвратное значение]-|True, если 
TopWindow в нормальном (overlapped) состоянии.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:ActiveFocus`(Ctrl`&`): [_^TopWindow^ TopWindow][@(0.0.255) `&]_[* ActiveF
ocus]([_^Ctrl^ Ctrl][@(0.0.255) `&]_[*@3 ctrl])&]
[s2;%RU-RU Устанавливает контрол с активным 
фокусом ActiveFocus. Этот Ctrl получает фокус 
при активировании TopWindow. Если контрола 
с активным фокусом нет, активируется 
первый контрол. Когда TopWindow дезактивируется,
 Ctrl, у которого фокус, сохраняется 
как новый контрол с ActiveFocus, и получает 
фокус, как только TopWindow снова активируется 
(если за это время данный метод не 
изменит ActiveFocus). Когда в классе this, 
метод вызывается при активном TopWindow, 
это эквивалентно ctrl.SetFocus().&]
[s7;%RU-RU [%-*C@3 ctrl]-|Ctrl, которому назначить 
&]
[s7;%RU-RU [*/ Возвратное значение]-|`*this.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:Title`(const WString`&`): [_^TopWindow^ TopWindow][@(0.0.255) `&]_[* Titl
e]([@(0.0.255) const]_[_^WString^ WString][@(0.0.255) `&]_[*@3 `_title])&]
[s5;:TopWindow`:`:Title`(const char`*`): [_^TopWindow^ TopWindow][@(0.0.255) `&]_[* Title](
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 `_title])&]
[s2;%RU-RU Устанавливает титул окна.&]
[s7;%RU-RU [%-*C@3 `_title]-|Титул.&]
[s7;%RU-RU [*/ Возвратное значение]-|`*this.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:GetTitle`(`)const: [@(0.0.255) const]_[_^WString^ WString][@(0.0.255) `&]_
[* GetTitle]()_[@(0.0.255) const]&]
[s7;%RU-RU [*/ Возвратное значение]-|Текущий 
титул окна.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:Sizeable`(bool`): [_^TopWindow^ TopWindow][@(0.0.255) `&]_[* Sizeable]([@(0.0.255) b
ool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%RU-RU Указывает, сможет ли пользователь 
менять размеры окна TopWindow.&]
[s7;%RU-RU [%-*C@3 b]-|True, чтобы установить.&]
[s7;%RU-RU [*/ Возвратное значение]-|`*this.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:NoSizeable`(`): [_^TopWindow^ TopWindow][@(0.0.255) `&]_[* NoSizeable]()&]
[s2;%RU-RU То же, что и Sizeable(false).&]
[s7;%RU-RU [*/ Возвратное значение]-|`*this.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:IsSizeable`(`)const: [@(0.0.255) bool]_[* IsSizeable]()_[@(0.0.255) const
]&]
[s7;%RU-RU [*/ Возвратное значение]-|Значение&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:MinimizeBox`(bool`): [_^TopWindow^ TopWindow][@(0.0.255) `&]_[* MinimizeB
ox]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%RU-RU Устанавливает TopWindow с наличием 
бокса `"minimize`" (`"свернуть`").&]
[s7;%RU-RU [%-*C@3 b]-|true, чтобы установить.&]
[s7;%RU-RU [*/ Возвратное значение]-|`*this.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:MaximizeBox`(bool`): [_^TopWindow^ TopWindow][@(0.0.255) `&]_[* MaximizeB
ox]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%RU-RU Устанавливает TopWindow с наличием 
бокса `"maximize`" (`"развернуть`").&]
[s7;%RU-RU [%-*C@3 b]-|true, чтобы установить.&]
[s7;%RU-RU [*/ Возвратное значение]-|`*this.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:Zoomable`(bool`): [_^TopWindow^ TopWindow][@(0.0.255) `&]_[* Zoomable]([@(0.0.255) b
ool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%RU-RU Устанавливает TopWindow как Zoomable `- 
с флагами maximize/minimize, если их поддерживает 
хостовая платформа. То же, что и MinimizeBox(b).Maximize
Box(b).&]
[s7;%RU-RU [%-*C@3 b]-|True, чтобы установить.&]
[s7;%RU-RU [*/ Возвратное значение]-|`*this.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:NoZoomable`(`): [_^TopWindow^ TopWindow][@(0.0.255) `&]_[* NoZoomable]()&]
[s2;%RU-RU То же, что и Zoomable(false).&]
[s7;%RU-RU [*/ Возвратное значение]-|`*this.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:IsZoomable`(`)const: [@(0.0.255) bool]_[* IsZoomable]()_[@(0.0.255) const
]&]
[s7;%RU-RU [*/ Возвратное значение]-|Значение 
флага Zoomable.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:Background`(const PaintRect`&`): [_^TopWindow^ TopWindow][@(0.0.255) `&
]_[* Background]([@(0.0.255) const]_[_^PaintRect^ PaintRect][@(0.0.255) `&]_[*@3 prect])&]
[s2;%RU-RU Устанавливает PaintRect, который будет 
использоваться для отрисовки фона 
TopWindow. Дефолтный PaintRect отрисовывает 
фон посредством SColorFace.&]
[s7;%RU-RU [%-*C@3 prect]-|PaintRect.&]
[s7;%RU-RU [*/ Возвратное значение]-|`*this.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:GetBackground`(`)const: [@(0.0.255) const]_[_^PaintRect^ PaintRect][@(0.0.255) `&
]_[* GetBackground]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает активный PaintRect, используемый 
для отрисовки фона TopWindow.&]
[s7;%RU-RU [*/ Возвратное значение]-|PaintRect.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:ToolWindow`(bool`): [_^TopWindow^ TopWindow][@(0.0.255) `&]_[* ToolWindow
]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%RU-RU TopWindow появляется с тонким caption, 
если его поддерживает хостовая платформа. 
Устанавливается перед открытием.&]
[s7;%RU-RU [%-*C@3 b]-|True, чтобы установить флаг.&]
[s7;%RU-RU [*/ Возвратное значение]-|`*this.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:NoToolWindow`(`): [_^TopWindow^ TopWindow][@(0.0.255) `&]_[* NoToolWindow
]()&]
[s2;%RU-RU То же, что и ToolWindow(false).&]
[s7;%RU-RU [*/ Возвратное значение]-|`*this.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:IsToolWindow`(`)const: [@(0.0.255) bool]_[* IsToolWindow]()_[@(0.0.255) c
onst]&]
[s7;%RU-RU [*/ Возвратное значение]-|Значение 
флага ToolWindow.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:TopMost`(bool`,bool`): [_^TopWindow^ TopWindow][@(0.0.255) `&]_[* TopMost
]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true], [@(0.0.255) bool]_[*@3 stay`_top]_`=_[@(0.0.255) t
rue])&]
[s2;%RU-RU Заставляет окно оставаться над 
любым другим окном. Возвращает `*this.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:NoTopMost`(`): [_^TopWindow^ TopWindow][@(0.0.255) `&]_[* NoTopMost]()&]
[s2;%RU-RU То же, что и TopMost(false).&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:IsTopMost`(`)const: [@(0.0.255) bool]_[* IsTopMost]()_[@(0.0.255) const]&]
[s2;%RU-RU Значение флага TopMost.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:FullScreen`(bool`): [_^TopWindow^ TopWindow][@(0.0.255) `&]_[* FullScreen
]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%RU-RU Устанавливает окно в полный экран, 
если [%-*@3 b] равно true, иначе устанавливает 
окно в обычное состояние. Возвращает 
`*this.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:IsFullScreen`(`)const: [@(0.0.255) bool]_[* IsFullScreen]()_[@(0.0.255) c
onst]&]
[s2;%RU-RU [*/ Возвратное значение]-|True, если 
окно в полноэкранном режиме.&]
[s3; &]
[s4; &]
[s5;:TopWindow`:`:FrameLess`(bool`): [_^TopWindow^ TopWindow][@(0.0.255) `&]_[* FrameLess](
[@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%RU-RU Создаёт окно без бордюрных декораций 
(типа бруса заголовка окна или бордюров 
для изменения размера).&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:IsFrameLess`(`)const: [@(0.0.255) bool]_[* IsFrameLess]()_[@(0.0.255) con
st]&]
[s2;%RU-RU Возвращает статус FrameLess.&]
[s3; &]
[s4; &]
[s5;:TopWindow`:`:Urgent`(bool`): [_^TopWindow^ TopWindow][@(0.0.255) `&]_[* Urgent]([@(0.0.255) b
ool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%RU-RU Вызов Urgent(true) заставляет мерцать 
титул неактивноо окна и его элемент 
в панели задач, привлекая внимание 
пользователя. Этот эффект длится 
до вызова Urgent(false), либо до того, как 
окно выводится на передний план. Если 
Urgent(true) вызывается над активный в 
данный момент окном, тогда никакого 
эффекта не будет.&]
[s2;%RU-RU &]
[s2;%RU-RU Заметка: Большинство оконных менеджеров 
в X11 поддерживает эту фишку, но могут 
быть всё ещё и те, которые игнорируют 
эту установку.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:IsUrgent`(`)const: [@(0.0.255) bool]_[* IsUrgent]()_[@(0.0.255) const]&]
[s2;%RU-RU [*/ Возвратное значение]-|True, если 
окно пытается привлечь внимание пользовател
я мерцанием.&]
[s3; &]
[s4; &]
[s5;:TopWindow`:`:NoAccessKeysDistribution`(`): [_^TopWindow^ TopWindow][@(0.0.255) `&]_[* N
oAccessKeysDistribution]()&]
[s2;%RU-RU Дезактивирует назначение клавиш 
доступа. Как правило, TopWindow пытается 
присвоить ключи доступа (так же известные 
как `"hot`-keys`" `- `"горячие клавиши`") всем 
своим окнам`-отпрыскам перед открытием. 
Этот метод отключает такое поведение.&]
[s7;%RU-RU [*/ Возвратное значение]-|`*this.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:CloseBoxRejects`(bool`): [_^TopWindow^ TopWindow][@(0.0.255) `&]_[* Close
BoxRejects]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%RU-RU Когда активно, нажатие кнопки 
закрытия в титуле окна всегда отвергает 
диалог. Когда не активно, это действие 
зависит от наличия Acceptors/Rejectors, выполняющих 
первую из IDCANCEL `-> reject, IDNO `-> reject, IDEXIT 
`-> accept, IDYES `-> Accept и IDOK `-> Accept как дефолтное 
действие, если нет ничего иного.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:Icon`(const Image`&`): [_^TopWindow^ TopWindow][@(0.0.255) `&]_[* Icon]([@(0.0.255) c
onst]_[_^Image^ Image][@(0.0.255) `&]_[*@3 m])&]
[s2;%RU-RU Назначает иконку для TopWindow, которая 
обычно видна в заголовке окна (платформно`-сп
ецифично).&]
[s7;%RU-RU [%-*C@3 m]-|Иконка.&]
[s7;%RU-RU [*/ Возвратное значение]-|`*this.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:LargeIcon`(const Image`&`): [_^TopWindow^ TopWindow][@(0.0.255) `&]_[* La
rgeIcon]([@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 m])&]
[s2;%RU-RU Определяет `"большую`" иконку для 
TopWindow. Она может быть видна, напр., 
в переключателе задач и другоих местах 
(платформо`-специфично).&]
[s7;%RU-RU [%-*C@3 m]-|Иконка.&]
[s7;%RU-RU [*/ Возвратное значение]-|`*this.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:Icon`(const Image`&`,const Image`&`): [_^TopWindow^ TopWindow][@(0.0.255) `&
]_[* Icon]([@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 smallicon], 
[@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 largeicon])&]
[s2;%RU-RU определяет как `"маленькую`", так 
и `"большую`" иконку для TopWindow.&]
[s7;%RU-RU [%-*C@3 smallicon]-|Маленькая иконка.&]
[s7;%RU-RU [%-*C@3 largeicon]-|Большая иконка.&]
[s7;%RU-RU [*/ Возвратное значение]-|`*this.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:SerializePlacement`(Stream`&`,bool`): [@(0.0.255) void]_[* SerializePla
cement]([_^Stream^ Stream][@(0.0.255) `&]_[*@3 s], [@(0.0.255) bool]_[*@3 reminimize]_`=_[@(0.0.255) f
alse])&]
[s2;%RU-RU Сериализует текущее расположение 
TopWindow. Это платформо`-специфично, однако, 
в информацию может входить позиция 
на экране в `"нормальном`" состоянии 
и текущий статус (maximized, minimized, normal).&]
[s7;%RU-RU [%-*C@3 s]-|Stream.&]
[s7;%RU-RU [%-*C@3 reminimize]-|Если false и информация 
грузится из потока, минимированное 
состояние не восстанавливается.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:TopWindow`(`): [* TopWindow]()&]
[s2;%RU-RU Конструктор .&]
[s3;%RU-RU &]
[s4; &]
[s5;:TopWindow`:`:`~TopWindow`(`): [@(0.0.255) `~][* TopWindow]()&]
[s2;%RU-RU Деструктор.&]
[s3;%RU-RU &]
[s0;%RU-RU ]]