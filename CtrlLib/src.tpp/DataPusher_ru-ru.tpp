topic "Класс DataPusher";
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
[{_}%RU-RU 
[ {{10000@3 [s0; [*@7;4 Класс ][*@(229)4 DataPusher]]}}&]
[s3; &]
[s1;:DataPusher`:`:class:%- [@(0.0.255)3 class][3 _][*3 DataPusher][3 _:_][@(0.0.255)3 public][3 _
][*@3;3 Pusher]&]
[s0; &]
[s2; 
@@image:1906&143
(AzEBFwAAAOn/AAAAAHic7ZzRrcIwDEU7MXu8ZZiHFZ7EDuUPRbF947aUmvYc3Q+SJrYbcmkkEM//xxMhhBBCsW5/d4TQgbKWnAHgILAkQCmwJEApsCRAKT5ryWma1o18N6cGPdcOiPpXlBdFzpS3PenqOuEEDC250/ZwLZnpFP1u5OQljSgvOXdRrkWWh5NR2ZJRASe2ZJsIV16TjCXtqbJrtj3RyC6m24ws2Z4b53jH2uNllNTehVu2Ls+WEcXsemwifWvuYrpP0mExUJ/8U9K+3VGPuzHcmCKFuDq0pJtoGCcz0fpdLIv1jl63NouoJMqeKQbqs7clLcNdFI3fYslM0uGnR35AxlOttd3HYvs6GtyZNH9fUJZrWlKEFdUuGpBcNzeyXkxR0rAYqM+XD64rmtstGWWJ4g8nRtHcgPl1m71lzEzPFwP1yXwvaQ9RXdN1TdTvRm57urlzvG+7wbZfZ7FX9cRoV7v7XzhdJ3JvfxgwWQzUZ8tPBXijd4KFvTJYEqAU/MYVoBRYEqAUriURQgfq+/8lghBCCP2cXgKYpno=)
&]
[s0; &]
[s9; DataPusher `- класс, который способен преобразовы
вать (Convert) и отображать (Display) некоторое 
значение (Value), а также позволяет пользовател
ю кликать на этом значении (обычно 
в целях его изменения). Заметьте, что 
`"время жизни`" как Convert, так и Display, 
должно превышать `"лайфтайм`" DataPusher`'а 
(DataPusher хранит только ссылки на них).&]
[s2; &]
[s9; Обычный пример использования: DataPusher 
отображает некоторые данные, которые 
могут меняться посредством некоторого 
сложного диалога. Этот паттерн можно 
реализовать либо произведя новый 
класс от DataPusher, переписав метод DoAction 
и использовав SetDataAction в конце всего, 
чтобы сменить значение DataPusher`'а и 
одновременно вызвать перерисовку 
и событие WhenAction, либо использовав 
дефолтную реализацию DoAction и обратный 
вызов WhenAction (в этом случае не вызывается 
SetDataAction).&]
[s2; &]
[s0;%- [%RU-RU/ Производный от][%RU-RU  ][^topic`:`/`/CtrlLib`/src`/Pusher`_ru`-ru^ P
usher]&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Детали Конструктора]]}}&]
[s3;%- &]
[s5;:DataPusher`:`:DataPusher`(`):%- [* DataPusher]()&]
[s2; Инициализует `"поместитель`" данных.&]
[s3; &]
[s4; &]
[s5;:DataPusher`:`:DataPusher`(const Convert`&`,const Display`&`):%- [* DataPusher]([@(0.0.255) c
onst]_[_^Convert^ Convert][@(0.0.255) `&]_[*@3 convert], [@(0.0.255) const]_[_^Display^ Dis
play][@(0.0.255) `&]_[*@3 display]_`=_StdDisplay())&]
[s2; Создаёт DataPusher с заданными Convert и Display, 
используемыми для преобразования 
и отображения значения DataPusher`'а. Заметьте, 
что и тот и другой метод должны превышать 
время жизни DataPusher`'а (DataPusher сохраняет 
только ссылки на них).&]
[s7; [%-*C@3 convert]-|Convert.&]
[s7; [%-*C@3 display]-|Display.&]
[s3; &]
[s4;%- &]
[s5;:DataPusher`:`:DataPusher`(const Display`&`):%- [* DataPusher]([@(0.0.255) const]_[_^Display^ D
isplay][@(0.0.255) `&]_[*@3 display])&]
[s2; Создаёт DataPusher с указанным Display.&]
[s7; [%-*C@3 display]-|Display.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Список Защищённых Методов]]}}&]
[s3;%- &]
[s5;:DataPusher`:`:DoAction`(`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* DoAction]()&]
[s2; Этот виртуальный метод вызывается, 
когда пользователь кликает DataPusher. 
При дефолтном действии вызывается 
обратный вызов (обрвыз) WhenAction.&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Список Публичных Членов]]}}&]
[s3;%- &]
[s5;:DataPusher`:`:SetConvert`(const Convert`&`):%- [_^DataPusher^ DataPusher][@(0.0.255) `&
]_[* SetConvert]([@(0.0.255) const]_[_^Convert^ Convert][@(0.0.255) `&]_[*@3 `_convert])&]
[s2; Устанавливает новый Convert.&]
[s7; [%-*C@3 `_convert]-|Новый Convert.&]
[s7; [*/ Возвратное значение]-|`*this для сцепления.&]
[s3; &]
[s4;%- &]
[s5;:DataPusher`:`:GetConvert`(`)const:%- [@(0.0.255) const]_[_^Convert^ Convert][@(0.0.255) `&
]_[* GetConvert]()_[@(0.0.255) const]&]
[s2; Возвращает текущий Convert.&]
[s7; [*/ Возвратное значение]-|Текущий Convert.&]
[s3; &]
[s4;%- &]
[s5;:DataPusher`:`:SetDisplay`(const Display`&`):%- [_^DataPusher^ DataPusher][@(0.0.255) `&
]_[* SetDisplay]([@(0.0.255) const]_[_^Display^ Display][@(0.0.255) `&]_[*@3 `_display])&]
[s2; Устанавливает новый Display.&]
[s7; [%-*C@3 `_display]-|Новый Display.&]
[s7; [*/ Возвратное значение]-|`*this для сцепления.&]
[s3; &]
[s4;%- &]
[s5;:DataPusher`:`:GetDisplay`(`)const:%- [@(0.0.255) const]_[_^Display^ Display][@(0.0.255) `&
]_[* GetDisplay]()_[@(0.0.255) const]&]
[s2; Возвращает текущий Display.&]
[s7; [*/ Возвратное значение]-|Текущий Display.&]
[s3; &]
[s4;%- &]
[s5;:DataPusher`:`:SetDataAction`(const Value`&`):%- [@(0.0.255) void]_[* SetDataAction](
[@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 value])&]
[s2; Вызывает SetData([%-*C@3 value]) и, далее, UpdateActionRefresh(). 
Применятся при построении производного 
класса от DataPusher`'а.&]
[s3; &]
[s4;%- &]
[s5;:DataPusher`:`:Set`(const Value`&`):%- [@(0.0.255) void]_[* Set]([@(0.0.255) const]_[_^Value^ V
alue][@(0.0.255) `&]_[*@3 value])&]
[s2; Альтернативный способ установки 
данных, декупирован из интерфейса 
SetData. Устанавливает отображаемые 
данные. Предназначен для использования 
в производных классах, реализующих 
собственные методы SetData/GetData.&]
[s3; &]
[s4;%- &]
[s5;:DataPusher`:`:SetData`(const Value`&`):%- [@(0.0.255) virtual] 
[@(0.0.255) void]_[* SetData]([@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 value])
&]
[s2; Устанавливает данные этого контрола 
в значение [%-*@3 value].&]
[s3; &]
[s4;%- &]
[s5;:DataPusher`:`:GetData`(`)const:%- [@(0.0.255) virtual] [_^Value^ Value]_[* GetData]()_
[@(0.0.255) const]&]
[s2; Возвращает данные этого контрола.&]
[s3;%- &]
[s4;%- &]
[s5;:DataPusher`:`:NullText`(const char`*`,Color`):%- [_^DataPusher^ DataPusher][@(0.0.255) `&
]_[* NullText]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 text]_`=_t`_(`"([@(0.0.255) defa
ult])`"), [_^Color^ Color]_[*@3 ink]_`=_Brown)&]
[s5;:DataPusher`:`:NullText`(const char`*`,Font`,Color`):%- [_^DataPusher^ DataPusher][@(0.0.255) `&
]_[* NullText]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 text], [_^Font^ Font]_[*@3 fnt], 
[_^Color^ Color]_[*@3 ink])&]
[s2; Устанавливает дефолтный текст, которые 
отображается, когда нет никаких представлен
ных данных.&]
[s7;%- [*@3 text]-|дефолтный текст&]
[s7;%- [*@3 ink]-|цвет текста&]
[s7;%- [*@3 fnt]-|шрифт текста&]
[s3; &]
[s4;%- &]
[s5;:DataPusher`:`:WhenPreAction:%- [_^Callback^ Callback]_[* WhenPreAction]&]
[s2; Этот обрвыз вызывается перед DoAction.&]
[s3; &]
[s0; ]]