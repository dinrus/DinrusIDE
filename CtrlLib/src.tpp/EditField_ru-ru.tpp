topic "Класс EditField (ПолеРедактирования)";
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
[ {{10000@3 [s0;%RU-RU [*@7;4 Класс ][*@(229)4 EditField (ПолеРедактирования)]]}}&]
[s3; &]
[s1;:EditField`:`:class: [@(0.0.255)3 class][3 _][*3 EditField][3 _:_][@(0.0.255)3 public][3 _][*@3;3 C
trl][3 , ][@(0.0.255)3 private][3 _][*@3;3 TextArrayOps]&]
[s0;%RU-RU &]
[s2;%RU-RU 
@@image:406&143
(A0EAFwAAAAAAAAAAAHic7ZfBCYAwEASvcyuxEDuxBSE96MtH7uJhgoZd2GEfIeSxwwUx5diLgpFl3egSFU4qpOAwsw+7vUQKDkwF18pumjuACrGtW0eXKa0reqeQ7HMpNG/X0+G/GVBI5kKhkJSkUMgngqngPqFWE88AKvQihTH0p4pAU4Eu89+2SpILMkWx/w==)
&]
[s0; &]
[s9;%RU-RU Этот виджет используется в тех 
случаях, когда есть значение, редактируемое 
в своём текстовом представлении. 
Конечно, самым очевидным таким типом 
значения являются (W)String, но EditField можно 
использовать для редактирования 
любых иных значений, типа Date, int, double. 
Адаптация EditField достигается применением 
объекта на основе [^`:`:Convert^ Convert], преобразующег
о значение в текст, и наоборот.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Публичных Членов]]}}&]
[s3; &]
[s5;:EditField`:`:GetViewHeight`(Font`): [@(0.0.255) static] [@(0.0.255) int]_[* GetViewHei
ght]([_^Font^ Font]_[*@3 font]_`=_StdFont())&]
[s2; Возвращает точную высоту для видимой 
части EditField со шрифтом [*@3 font][%RU-RU , используемым
 для редактирования текста].&]
[s3; &]
[s4; &]
[s5;:EditField`:`:GetStdHeight`(Font`): [@(0.0.255) static] [@(0.0.255) int]_[* GetStdHeigh
t]([_^Font^ Font]_[*@3 font]_`=_StdFont())&]
[s2;%RU-RU [%- Возвращает точную высоту для 
видимой части EditField со шрифтом ][%-*@3 font], 
используемым для редактирования 
текста, включая стандартный статический 
фрейм.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:HighlightText`(Vector`<EditField`:`:Highlight`>`&`): [@(0.0.255) virt
ual] [@(0.0.255) void]_[* HighlightText]([_^Vector^ Vector]<[_^EditField`:`:Highlight^ Hi
ghlight]>`&_[*@3 hl])&]
[s2;%RU-RU Даёт возможность менять цвет 
текста и фона для отдельных символов.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:WhenBar: [_^Callback1^ Callback1]<Bar[@(0.0.255) `&]>_[* WhenBar]&]
[s2;%RU-RU Этот обрвыз представляет контекстное 
меню EditField. Дефолт равен StdBar.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:WhenEnter: [_^Callback^ Callback]_[* WhenEnter]&]
[s2;%RU-RU Этот обрвыз вызывается, если пользовател
ь нажимает клавишу Enter в EditField. Если 
не пустой, EditField также `"потребляет`" 
клавишу Enter (так что она не передаётся 
вверх по иерархии Ctrl). Дефолт равен 
пустой.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:WhenPasteFilter: [_^Callback1^ Callback1]<WString[@(0.0.255) `&]>_[* When
PasteFilter]&]
[s2;%RU-RU Этот обрвыз вызывается при выполнении 
операции вставки Paste. И может использоваться
 для альтерации вставляемого текста. 
Дефолтно текст не меняется.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:WhenHighlight: [_^Callback1^ Callback1]<[_^Vector^ Vector]<[_^EditField`:`:Highlight^ H
ighlight]>`&>_[* WhenHighlight]&]
[s2;%RU-RU Вызывается дефолтной реализацией 
HighlightText. Даёт возможность менять цвет 
текста и фона для отдельных символов.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:Insert`(int`,const WString`&`): [@(0.0.255) int]_[* Insert]([@(0.0.255) i
nt]_[*@3 pos], [@(0.0.255) const]_[_^WString^ WString][@(0.0.255) `&]_[*@3 text])&]
[s2;%RU-RU [%- Вставляет текст ][%-*@3 text] в позиции 
[%-*@3 pos].&]
[s3; &]
[s4; &]
[s5;:EditField`:`:Insert`(int`,const String`&`): [@(0.0.255) int]_[* Insert]([@(0.0.255) in
t]_[*@3 pos], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 text])&]
[s5;:EditField`:`:Insert`(int`,const char`*`): [@(0.0.255) int]_[* Insert]([@(0.0.255) int]_
[*@3 pos], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 text])&]
[s2;%RU-RU [%- Вставляет текст ][%-*@3 text] в позиции 
[%-*@3 pos]. Текст преобразуется в Юникод 
с использованием дефолтного набсима 
(гарнитуры).&]
[s3; &]
[s4; &]
[s5;:EditField`:`:Remove`(int`,int`): [@(0.0.255) void]_[* Remove]([@(0.0.255) int]_[*@3 pos],
 [@(0.0.255) int]_[*@3 n])&]
[s2;%RU-RU [%- Удаляет ][%-*@3 n] символов в позиции 
[%-*@3 pos].&]
[s3; &]
[s4; &]
[s5;:EditField`:`:Insert`(const WString`&`): [@(0.0.255) void]_[* Insert]([@(0.0.255) const
]_[_^WString^ WString][@(0.0.255) `&]_[*@3 text])&]
[s2;%RU-RU [%- Вставляет текст ][%-*@3 text] в позицию 
курсора.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:Insert`(const String`&`): [@(0.0.255) void]_[* Insert]([@(0.0.255) const]_
[_^String^ String][@(0.0.255) `&]_[*@3 text])&]
[s5;:EditField`:`:Insert`(const char`*`): [@(0.0.255) void]_[* Insert]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 text])&]
[s2;%RU-RU [%- Вставляет текст ][%-*@3 text] в позицию 
курсора. Текст преобразуется в Юникод 
с использованием дефолтного набсима 
(гарнитуры).&]
[s3; &]
[s4; &]
[s5;:EditField`:`:Insert`(int`): [@(0.0.255) void]_[* Insert]([@(0.0.255) int]_[*@3 chr])&]
[s2;%RU-RU [%- Вставляет единичный символ Юникода 
][%-*@3 chr] в позицию курсора.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:Move`(int`,bool`): [@(0.0.255) void]_[* Move]([@(0.0.255) int]_[*@3 newpos],
 [@(0.0.255) bool]_[*@3 select]_`=_[@(0.0.255) false])&]
[s2;%RU-RU [%- Перемещает курсор к новой позиции] 
[%-*@3 newpos]. Если [%-*@3 select] равно true, устанавливаетс
я выделение между преждней позицией 
курсора и новой [%-*@3 newpos].&]
[s3; &]
[s4; &]
[s5;:EditField`:`:SetSelection`(int`,int`): [@(0.0.255) void]_[* SetSelection]([@(0.0.255) i
nt]_[*@3 l]_`=_[@3 0], [@(0.0.255) int]_[*@3 h]_`=_INT`_MAX)&]
[s2;%RU-RU [%- Устанавливает выделение, если 
][%-*@3 l] < [%-*@3 h][%- . Если ][%-*@3 l] или [%-*@3 h ]превышает 
имеющийся диапазон (то есть от ноля 
до длины текста), они привязываются 
к нему. Если[%-  ][%-*@3 l] >`= [%-*@3 h], выделение 
отменяется и [%-*@3 l] становится новой 
позицией курсора.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:GetSelection`(int`&`,int`&`)const: [@(0.0.255) bool]_[* GetSelection]([@(0.0.255) i
nt`&]_[*@3 l], [@(0.0.255) int`&]_[*@3 h])_[@(0.0.255) const]&]
[s2;%RU-RU [%- Возвращает текущее выделение. 
Если выделения нет, оба] [%-*@3 l] [%-*@3 h] возвращают 
текущую позицию курсора.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:IsSelection`(`)const: [@(0.0.255) bool]_[* IsSelection]()_[@(0.0.255) con
st]&]
[s2; Возвращает true, если есть выделение.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:RemoveSelection`(`): [@(0.0.255) bool]_[* RemoveSelection]()&]
[s2; Если выделения нет, возвращает false, 
иначе удаляет выделенный текст и 
возвращает true.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:CancelSelection`(`): [@(0.0.255) void]_[* CancelSelection]()&]
[s2;%RU-RU Любое выделение отменяется (после 
этой операции нет выделенных символов).&]
[s3; &]
[s4; &]
[s5;:EditField`:`:Copy`(`): [@(0.0.255) void]_[* Copy]()&]
[s2; Копирует выделенный текст в буфер 
обмена&]
[s3; &]
[s4; &]
[s5;:EditField`:`:Undo`(`): [@(0.0.255) void]_[* Undo]()&]
[s2;%RU-RU Выполняет операцию undo.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:Cut`(`): [@(0.0.255) void]_[* Cut]()&]
[s2;%RU-RU Вырезает выделенное.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:Paste`(`): [@(0.0.255) void]_[* Paste]()&]
[s2;%RU-RU Вставляет содержимое буфера обмена, 
если оно совместимо, в EditField.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:Erase`(`): [@(0.0.255) void]_[* Erase]()&]
[s2;%RU-RU Удаляет содержимое EditField.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:SelectAll`(`): [@(0.0.255) void]_[* SelectAll]()&]
[s2;%RU-RU Выделяет весь текст в EditField.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:StdBar`(Bar`&`): [@(0.0.255) void]_[* StdBar]([_^Bar^ Bar][@(0.0.255) `&]_[*@3 m
enu])&]
[s2;%RU-RU Стандартное меню EditField.&]
[s3;%RU-RU &]
[s4; &]
[s5;:EditField`:`:SetText`(const WString`&`): [@(0.0.255) void]_[* SetText]([@(0.0.255) con
st]_[_^WString^ WString][@(0.0.255) `&]_[*@3 text])&]
[s2;%RU-RU [%- Устанавливает] [%-*@3 text].&]
[s3; &]
[s4; &]
[s5;:EditField`:`:SetText`(const String`&`): [@(0.0.255) void]_[* SetText]([@(0.0.255) cons
t]_[_^String^ String][@(0.0.255) `&]_[*@3 t])&]
[s5;:EditField`:`:SetText`(const char`*`): [@(0.0.255) void]_[* SetText]([@(0.0.255) const]_
[@(0.0.255) char]_`*[*@3 t])&]
[s2;%RU-RU [%- Устанавливает текст. ]Текст преобразует
ся в Юникод с использованием дефолтного 
набсима (гарнитуры).&]
[s3; &]
[s4; &]
[s5;:EditField`:`:GetText`(`)const: [@(0.0.255) const]_[_^WString^ WString][@(0.0.255) `&]_
[* GetText]()_[@(0.0.255) const]&]
[s2; Возвращает текст.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:GetLength`(`)const: [@(0.0.255) virtual] [@(0.0.255) int]_[* GetLength]()
_[@(0.0.255) const]&]
[s2; Возвращает длину текста.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:Clear`(`): [@(0.0.255) void]_[* Clear]()&]
[s2; Устанавливает пустой текст.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:Reset`(`): [@(0.0.255) void]_[* Reset]()&]
[s2; Сбрасывает EditField в дефолтное состояние.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:Error`(bool`): [@(0.0.255) void]_[* Error]([@(0.0.255) bool]_[*@3 error])&]
[s2;%RU-RU Если [%-*@3 error] равно true, отрисовывает 
контент как повреждённый (точно так 
же, когда присвоенный Convert возвращает 
ErrorValue).&]
[s3;%RU-RU &]
[s4; &]
[s5;:EditField`:`:Password`(bool`): [_^EditField^ EditField][@(0.0.255) `&]_[* Password]([@(0.0.255) b
ool]_[*@3 pwd]_`=_[@(0.0.255) true])&]
[s2;%RU-RU Устанавливает режим ввода пароля. 
В режиме пароля все символы обображаются 
как звёздочки (asterisk). Возвращает `*this.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:IsPassword`(`)const: [@(0.0.255) bool]_[* IsPassword]()_[@(0.0.255) const
]&]
[s2;%RU-RU Возвращает true, если режим ввода 
пароля активен.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:SetFilter`(int`(`*`)`(int`)`): [_^EditField^ EditField][@(0.0.255) `&]_
[* SetFilter]([@(0.0.255) int]_(`*[*@3 f])([@(0.0.255) int]))&]
[s2;%RU-RU [%- Устанавливает символьный фильтр] 
[%-*@3 f]. Все нажатые клавиши вначале 
изменяются этой функцией и используются 
только тогда, когда результат не равен 
нулю. Возвращает `*this.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:SetConvert`(const Convert`&`): [_^EditField^ EditField][@(0.0.255) `&]_
[* SetConvert]([@(0.0.255) const]_[_^Convert^ Convert][@(0.0.255) `&]_[*@3 c])&]
[s2;%RU-RU Устанавливает преобразователь 
значения. Применяя специальные преобразоват
ели, можно адаптировать EditField под 
редактирование значений любого типа 
с текстовым представлением. Заметьте, 
что EditField сохраняет только ссылку 
на [%-@3 c], поэтому его время жизни должно 
превышать `"лайфтайм`" EditField. Возвращает 
`*this.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:SetInactiveConvert`(const Convert`&`): [_^EditField^ EditField][@(0.0.255) `&
]_[* SetInactiveConvert]([@(0.0.255) const]_[_^Convert^ Convert][@(0.0.255) `&]_[*@3 c])&]
[s2;%RU-RU Устанавливает нективный преобразовател
ь значения. Такой преобразователь 
используется для форматирования 
отображаемого значения, когда у контрола 
редактирования нет владения фокусом. 
Возвращает `*this.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:SetCharset`(byte`): [_^EditField^ EditField][@(0.0.255) `&]_[* SetCharset
]([_^byte^ byte]_[*@3 cs])&]
[s2;%RU-RU Устанавливает кодировку в [%-*@3 cs: 
]в EditField допускаются только символы, 
которые можно представить в данной 
кодировке. Заметьте, что это не влияет 
на кодировку строковых значений, 
возвращаемых из EditField, а работает 
только как ещё один фильтр.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:AutoFormat`(bool`): [_^EditField^ EditField][@(0.0.255) `&]_[* AutoFormat
]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%RU-RU [%- В режиме AutoFormat, когда фокус уходит 
из поля редактирования, текстовое 
представление значения реформатируется 
текущим Convert: текст конвертируется 
в Value  и обратно. Этот режим активен 
дефолтно.] Возвращает `*this.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:NoAutoFormat`(`): [_^EditField^ EditField][@(0.0.255) `&]_[* NoAutoFormat
]()&]
[s2; То же, что и AutoFormat(false).&]
[s3; &]
[s4; &]
[s5;:EditField`:`:IsAutoFormat`(`)const: [@(0.0.255) bool]_[* IsAutoFormat]()_[@(0.0.255) c
onst]&]
[s2;%RU-RU Возвращает true, если AutoFormat активен.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:SetFont`(Font`): [_^EditField^ EditField][@(0.0.255) `&]_[* SetFont]([_^Font^ F
ont]_[*@3 `_font])&]
[s2;%RU-RU [%- Устанавливает шрифт ][%-*@3 font] показываемо
го текста. Возвращает `*this.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:SetColor`(Color`): [_^EditField^ EditField][@(0.0.255) `&]_[* SetColor]([_^Color^ C
olor]_[*@3 c])&]
[s2;%RU-RU [%- Устанавливает цвет показываемого 
текста]. Возвращает `*this.&]
[s3;%RU-RU &]
[s4; &]
[s5;:EditField`:`:ClickSelect`(bool`): [_^EditField^ EditField][@(0.0.255) `&]_[* ClickSele
ct]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; В режиме ClickSelect, при клике мышью выделяется 
весь текст в EditField. Возвращает `*this.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:IsClickSelect`(`)const: [@(0.0.255) bool]_[* IsClickSelect]()_[@(0.0.255) c
onst]&]
[s2;%RU-RU Возвращает true, если ClickSelect активен.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:InitCaps`(bool`): [_^EditField^ EditField][@(0.0.255) `&]_[* InitCaps]([@(0.0.255) b
ool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; В режиме InitCaps, начальные буквы введённого 
текста становятся заглавными. Возвращает 
`*this.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:IsInitCaps`(`)const: [@(0.0.255) bool]_[* IsInitCaps]()_[@(0.0.255) const
]&]
[s2;%RU-RU Возвращает true, если InitCaps активен.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:NullText`(const Image`&`,const char`*`,Color`): [_^EditField^ EditFie
ld][@(0.0.255) `&]_[* NullText]([@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 icon],
 [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 text]_`=_t`_(`"([@(0.0.255) default])`"), 
[_^Color^ Color]_[*@3 ink]_`=_SColorDisabled)&]
[s5;:EditField`:`:NullText`(const Image`&`,const char`*`,Font`,Color`): [_^EditField^ E
ditField][@(0.0.255) `&]_[* NullText]([@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 i
con], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 text], [_^Font^ Font]_[*@3 fnt], 
[_^Color^ Color]_[*@3 ink])&]
[s5;:EditField`:`:NullText`(const char`*`,Color`): [_^EditField^ EditField][@(0.0.255) `&
]_[* NullText]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 text]_`=_t`_(`"([@(0.0.255) defa
ult])`"), [_^Color^ Color]_[*@3 ink]_`=_SColorDisabled)&]
[s5;:EditField`:`:NullText`(const char`*`,Font`,Color`): [_^EditField^ EditField][@(0.0.255) `&
]_[* NullText]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 text], [_^Font^ Font]_[*@3 fnt], 
[_^Color^ Color]_[*@3 ink])&]
[s2; В режиме NullText, пустой EditField отображает 
текст[%RU-RU  ][*@3 text][%RU-RU , используя цвет 
][*@3 ink][%RU-RU  и шрифт ][%RU-RU*@3 fnt][%RU-RU , с иконкой 
][*@3 icon][%RU-RU .] Возвращает `*this.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:MaxChars`(int`): [_^EditField^ EditField][@(0.0.255) `&]_[* MaxChars]([@(0.0.255) i
nt]_[*@3 mc])&]
[s2; Ограничивает максимальное число 
символов до[%RU-RU  ][*@3 mc][%RU-RU .] Возвращает 
`*this.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:GetMaxChars`(`)const: [@(0.0.255) int]_[* GetMaxChars]()_[@(0.0.255) cons
t]&]
[s2;%RU-RU Возвращает лимит максимального 
числа символов, установленный через 
MaxChars.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:AutoSize`(int`): [_^EditField^ EditField][@(0.0.255) `&]_[* AutoSize]([@(0.0.255) i
nt]_[*@3 maxcx]_`=_INT`_MAX)&]
[s2;%RU-RU [%- Изменяет ширину EditField, согласно 
длине текста, вплоть до ширины] [%-*@3 maxcx].&]
[s3; &]
[s4; &]
[s5;:EditField`:`:NoBackground`(bool`): [_^EditField^ EditField][@(0.0.255) `&]_[* NoBackgr
ound]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%RU-RU Устанавливает виджет в прозрачный 
режим: фон не отрисовывается, активируется 
Transparent `- в итоге видно всё, что позади 
виджета, позволяя клиентскому коду 
обеспечивать любой, необходимый ему, 
фон.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:GetFilter`(`)const: [_^CharFilter^ CharFilter]_[* GetFilter]()_[@(0.0.255) c
onst]&]
[s2;%RU-RU Возвращает текущий символьный 
фильтр.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:GetConvert`(`)const: [@(0.0.255) const]_[_^Convert^ Convert][@(0.0.255) `&
]_[* GetConvert]()_[@(0.0.255) const]&]
[s2; Возвращает текущий конвертер.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:GetFont`(`)const: [_^Font^ Font]_[* GetFont]()_[@(0.0.255) const]&]
[s2; Возвращает текущий шрифт.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:AlignRight`(bool`): [_^EditField^ EditField][@(0.0.255) `&]_[* AlignRight
]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%RU-RU Если [%-*@3 b].равно true, текст отображается 
размещённым справа. Левое размещение 
дефолтное.&]
[s3;%RU-RU &]
[s4; &]
[s5;:EditField`:`:IsNoBackground`(`)const: [@(0.0.255) bool]_[* IsNoBackground]()_[@(0.0.255) c
onst]&]
[s2;%RU-RU Возвращает true, если NoBackground активен.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:IsAlignRight`(`)const: [@(0.0.255) bool]_[* IsAlignRight]()_[@(0.0.255) c
onst]&]
[s2;%RU-RU Возвращает true, если AlignRight активен.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:ShowSpaces`(bool`): [@(0.0.255) bool]_[* ShowSpaces]([@(0.0.255) bool]_[*@3 b
]_`=_[@(0.0.255) true])&]
[s2;%RU-RU Когда активен, EditField рисует синие 
точки, показывающие пробелы.&]
[s3;%RU-RU &]
[s4; &]
[s5;:EditField`:`:NoInternalMargin`(bool`): [_^EditField^ EditField][@(0.0.255) `&]_[* NoIn
ternalMargin]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%RU-RU Как правило, у EditField есть небольшой 
внутренний margin (промежуток) между 
фреймом (рамкой) и редактируемым текстом. 
Вызов этого метода с [%-@(0.0.255) true] удаляет 
его.&]
[s3;%RU-RU &]
[s4; &]
[s5;:EditField`:`:GetChar`(int`)const: [@(0.0.255) virtual] [@(0.0.255) int]_[* GetChar]([@(0.0.255) i
nt]_[*@3 i])_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает символ, расположенный 
по индексу [%-*@3 i].&]
[s3;%RU-RU &]
[s4; &]
[s5;:EditField`:`:GetCaretRect`(int`)const: [_^Rect^ Rect]_[* GetCaretRect]([@(0.0.255) int
]_[*@3 pos])_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает размещение каретки 
для символа [%-*@3 pos] в координатах видимости 
EditField.&]
[s3;%RU-RU &]
[s4; &]
[s5;:EditField`:`:GetCaretRect`(`)const: [_^Rect^ Rect]_[* GetCaretRect]()_[@(0.0.255) cons
t]&]
[s2;%RU-RU То же, что и GetCaretRect(cursor).&]
[s3; &]
[s4; &]
[s5;:EditField`:`:StyleDefault`(`): [@(0.0.255) static] [@(0.0.255) const]_[_^EditField`:`:Style^ S
tyle][@(0.0.255) `&]_[* StyleDefault]()&]
[s2;%RU-RU Возвращает дефолтный стиль, используемы
й для отрисовки полей редактирования.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:SetStyle`(const EditField`:`:Style`&`): [_^EditField^ EditField][@(0.0.255) `&
]_[* SetStyle]([@(0.0.255) const]_[_^EditField`:`:Style^ Style][@(0.0.255) `&]_[*@3 s])&]
[s2;%RU-RU Устанавливает стиль в EditField`::Style 
[%-*@3 s].&]
[s3;%RU-RU &]
[s0; &]
[ {{10000@3 [s0;%RU-RU [*@(229)4 Структура EditField`::Style]]}}&]
[s3; &]
[s1;:EditField`:`:Style`:`:struct: [@(0.0.255)3 struct][3 _][*3 Style][3 _:_][@(0.0.255)3 public
][3 _][*@3;3 ChStyle][@(0.0.255)3 <][*3 Style][@(0.0.255)3 >][3 _]&]
[s9;%RU-RU Эта структура определяет визуальный 
стиль EditField.&]
[s0; ]]