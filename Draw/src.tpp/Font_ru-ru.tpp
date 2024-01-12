topic "Класс Font (Шрифт)";
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
[ {{10000t/25b/25@3 [s0; [*@(229)4 Класс Font (Шрифт)]]}}&]
[s3; &]
[s1;:Font`:`:class: [@(0.0.255) class]_[* Font]_:_[@(0.0.255) private]_[@3 AssignValueTypeNo]<
Font, FONT`_V, [_^Moveable^ Moveable]<Font>_>&]
[s9;%RU-RU Font `- это конкретный класс, описывающий 
внешний вид текста. Он Moveable, имеет 
простую операцию копирования и является 
Value, преобразовываемым со способностями 
rich`-Value.&]
[s9;%RU-RU Он хранит несколько атрибутов, 
описывающих текст. Соответствующий 
системный шрифт хранится в форме 
индекса фаса (face index) `- у каждого системного 
шрифта есть специфичное индексное 
значение. Для общих шрифтов существует 
несколько предопределённых индексов:&]
[s0;%RU-RU &]
[ {{2595:7405<256;>256;^ [s2;l0; [* Font`::STDFONT]]
::= [s2;l0;%RU-RU Стандартный шрифт ГИП, определённый 
платформой. Может быть изменён посредством 
Draw`::SetStdFont.]
::^ [s2;l0; [* Font`::SERIF]]
::= [s2;l0;%RU-RU Стандартный шрифт serif.]
::^ [s2;l0; [* Font`::SANSSERIF]]
::= [s2;l0;%RU-RU Стандартный шрифт sans`-serif.]
::^ [s2;l0; [* Font`::MONOSPACE]]
::= [s2;l0;%RU-RU Стандартный фиксированный pitch 
шрифт.]}}&]
[s2;%RU-RU &]
[s9;%RU-RU Font также предоставляет службы 
текстовой метрики.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Публичных Членов]]}}&]
[s3; &]
[s5;:Font`:`:GetFaceCount`(`): [@(0.0.255) static] [@(0.0.255) int]_[* GetFaceCount]()&]
[s2;%RU-RU Возвращает доступное число имён 
фасов.&]
[s3; &]
[s4; &]
[s5;:Font`:`:GetFaceName`(int`): [@(0.0.255) static] [_^String^ String]_[* GetFaceName]([@(0.0.255) i
nt]_[@3 index])&]
[s2;%RU-RU Возвращает имя фаса [%-*@3 index].&]
[s3; &]
[s4; &]
[s5;:Font`:`:FindFaceNameIndex`(const String`&`): [@(0.0.255) static] 
[@(0.0.255) int]_[* FindFaceNameIndex]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_
[@3 name])&]
[s2;%RU-RU Находит индекс фаса по его имени 
[%-*@3 name].&]
[s3;%RU-RU &]
[s4; &]
[s5;:Font`:`:GetFaceInfo`(int`): [@(0.0.255) static] [_^dword^ dword]_[* GetFaceInfo]([@(0.0.255) i
nt]_[@3 index])&]
[s2;%RU-RU Возвращает информацию о фасе 
[%-*@3 index] как комбинацию двоичных флагов:&]
[ {{2587:7413<256;^ [s0; [* Font`::FIXEDPITCH]]
::= [s0;%RU-RU Фас шрифта моноширинный.]
::^ [s0; [* Font`::SCALEABLE]]
::= [s0;%RU-RU Фас шрифта свободно масштабируемый 
(от в векторном формате, напр., Truetype).]
::^ [s0; [* Font`::SPECIAL]]
::= [s0;%RU-RU Шрифт не в кодировке Unicode (типично 
для некоторых символьных шрифтов).]}}&]
[s3; &]
[s4; &]
[s5;:Font`:`:SetDefaultFont`(Font`): [@(0.0.255) static] [@(0.0.255) void]_[* SetDefaultFon
t]([_^Font^ Font]_[*@3 font])&]
[s2;%RU-RU Устанавливает стандартный шрифт, 
если не используется SetStdFont. Этот вариант 
используется `"скином`" Chameleon для установки 
шрифта, соответствующего настройкам 
платформы, в то время как SetStdFont может 
использоваться кодом`-клиентом, чтобы 
переписать эту настройку (и, поэтому, 
имеет приоритет выше).&]
[s3;%RU-RU &]
[s4; &]
[s5;:Font`:`:SetStdFont`(Font`): [@(0.0.255) static] [@(0.0.255) void]_[* SetStdFont]([_^Font^ F
ont]_[@3 font])&]
[s2;%RU-RU Устанавливает стандартный шрифт. 
U`+`+ устанавливает стандартный шрифт, 
соответствующий стандарту платформы. 
Этот метод может быть использован 
для изменения дефолтной настройки.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Font`:`:GetStdFont`(`): [@(0.0.255) static] [_^Font^ Font]_[* GetStdFont]()&]
[s2;%RU-RU Возвращает стандартный шрифт.&]
[s3; &]
[s4; &]
[s5;:Font`:`:GetStdFontSize`(`): [@(0.0.255) static] [_^Size^ Size]_[* GetStdFontSize]()&]
[s2;%RU-RU Возвращает метрику стандартного 
шрифта `- высоту и среднюю ширину глифов.&]
[s3; &]
[s4; &]
[s5;:Font`:`:GetFace`(`)const: [@(0.0.255) int]_[* GetFace]()_[@(0.0.255) const]&]
[s2;%RU-RU Индекс фаса шрифта Font.&]
[s3; &]
[s4; &]
[s5;:Font`:`:GetHeight`(`)const: [@(0.0.255) int]_[* GetHeight]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает требуемую высоту шрифта.&]
[s3; &]
[s4; &]
[s5;:Font`:`:GetWidth`(`)const: [@(0.0.255) int]_[* GetWidth]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает ширину шрифта или 
0 для дефолтной ширины.&]
[s3; &]
[s4; &]
[s5;:Font`:`:GetFaceName`(`)const: [_^String^ String]_[* GetFaceName]()[@(64) _][@(0.0.255) c
onst]&]
[s2;%RU-RU Возвращает текстовое имя фаса 
для текущего экземпляра.&]
[s3; &]
[s4; &]
[s5;:Font`:`:GetFaceNameStd`(`)const: [_^String^ String]_[* GetFaceNameStd]()_[@(0.0.255) c
onst]&]
[s2;%RU-RU Подобно GetFaceName, но возвращает 
генерные имена `"serif`", `"sansserif`", `"monospace`", 
`"STDFONT`" для соответствующих фасов, 
вместо реальных имён.&]
[s3; &]
[s4; &]
[s5;:Font`:`:GetFaceInfo`(`)const: [_^dword^ dword]_[* GetFaceInfo]()[@(64) _][@(0.0.255) con
st]&]
[s2;%RU-RU То же, что и GetFaceInfo(GetFace()).&]
[s3; &]
[s4; &]
[s5;:Font`:`:AsInt64`(`)const: [_^int64^ int64]_[* AsInt64]()[@(64) _][@(0.0.255) const]&]
[s2;%RU-RU Возвращает 64`-битное число, представляющ
ее шрифт. Все атрибуты шрифта выражаются 
как бит`-поля этого числа.&]
[s3; &]
[s4; &]
[s5;:Font`:`:RealizeStd`(`): [@(0.0.255) void]_[* RealizeStd]()&]
[s2;%RU-RU Заменяет Null или STDFONT на действующее 
стандартное определение шрифта ГИП. 
Если шрифт Null, он устанавливается 
в стандартный, со всеми его атрибутами. 
Если фас `= STDFONT, то он заменяется действитель
ным фасом стандартного шрифта. Если 
высота шрифта `= `-32000 (значение, которое 
обычно возвращает функция StdFont), то 
она заменяется высотой стандартного 
шрифта.&]
[s3; &]
[s4; &]
[s5;:Font`:`:Face`(int`): [_^Font^ Font][@(0.0.255) `&]_[* Face]([@(0.0.255) int]_[@3 n])&]
[s2;%RU-RU Устанавливает индекс фаса.&]
[s3; &]
[s4; &]
[s5;:Font`:`:Height`(int`): [_^Font^ Font][@(0.0.255) `&]_[* Height]([@(0.0.255) int]_[@3 n])&]
[s2;%RU-RU Устанавливает высоту шрифта (в 
пиксели/точки).&]
[s3; &]
[s4; &]
[s5;:Font`:`:Width`(int`): [_^Font^ Font][@(0.0.255) `&]_[* Width]([@(0.0.255) int]_[@3 n])&]
[s2;%RU-RU Устанавливает ширину шрифта. 
Для дефолтной ширины используется 
0 (в пиксели/точки).&]
[s3; &]
[s4; &]
[s5;:Font`:`:Bold`(`): [_^Font^ Font][@(0.0.255) `&]_[* Bold]()&]
[s5;:Font`:`:NoBold`(`): [_^Font^ Font][@(0.0.255) `&]_[* NoBold]()&]
[s5;:Font`:`:Bold`(bool`): [_^Font^ Font][@(0.0.255) `&]_[* Bold]([@(0.0.255) bool]_[@3 b])&]
[s5;:Font`:`:Italic`(`): [_^Font^ Font][@(0.0.255) `&]_[* Italic]()&]
[s5;:Font`:`:NoItalic`(`): [_^Font^ Font][@(0.0.255) `&]_[* NoItalic]()&]
[s5;:Font`:`:Italic`(bool`): [_^Font^ Font][@(0.0.255) `&]_[* Italic]([@(0.0.255) bool]_[@3 b])
&]
[s5;:Font`:`:Underline`(`): [_^Font^ Font][@(0.0.255) `&]_[* Underline]()&]
[s5;:Font`:`:NoUnderline`(`): [_^Font^ Font][@(0.0.255) `&]_[* NoUnderline]()&]
[s5;:Font`:`:Underline`(bool`): [_^Font^ Font][@(0.0.255) `&]_[* Underline]([@(0.0.255) bool]_
[@3 b])&]
[s5;:Font`:`:Strikeout`(`): [_^Font^ Font][@(0.0.255) `&]_[* Strikeout]()&]
[s5;:Font`:`:NoStrikeout`(`): [_^Font^ Font][@(0.0.255) `&]_[* NoStrikeout]()&]
[s5;:Font`:`:Strikeout`(bool`): [_^Font^ Font][@(0.0.255) `&]_[* Strikeout]([@(0.0.255) bool]_
[@3 b])&]
[s2; Устанавливает и отменяет режимы 
bold/italic/undeline/strikeout.&]
[s3; &]
[s4; &]
[s5;:Font`:`:NonAntiAliased`(`): [_^Font^ Font][@(0.0.255) `&]_[* NonAntiAliased]()&]
[s5;:Font`:`:NoNonAntiAliased`(`): [_^Font^ Font][@(0.0.255) `&]_[* NoNonAntiAliased]()&]
[s5;:Font`:`:NonAntiAliased`(bool`): [_^Font^ Font][@(0.0.255) `&]_[* NonAntiAliased]([@(0.0.255) b
ool]_[@3 b])&]
[s6; `[deprecated`]&]
[s2;%RU-RU Устанавливает/отменяет флаг non`-anti`-aliased. 
Этим указывается, что при отрисовке 
шрифта не будет использоваться anti`-aliasing 
(сглаживание).&]
[s3; &]
[s4; &]
[s5;:Font`:`:IsBold`(`)const: [@(0.0.255) bool]_[* IsBold]()_[@(0.0.255) const]&]
[s5;:Font`:`:IsItalic`(`)const: [@(0.0.255) bool]_[* IsItalic]()_[@(0.0.255) const]&]
[s5;:Font`:`:IsUnderline`(`)const: [@(0.0.255) bool]_[* IsUnderline]()_[@(0.0.255) const]&]
[s5;:Font`:`:IsStrikeout`(`)const: [@(0.0.255) bool]_[* IsStrikeout]()_[@(0.0.255) const]&]
[s2; Проверяет, активен ли режим bold/italic/underline/strikeo
ut.&]
[s3; &]
[s4; &]
[s5;:Font`:`:IsNonAntiAliased`(`)const: [@(0.0.255) bool]_[* IsNonAntiAliased]()_[@(0.0.255) c
onst]&]
[s2;%RU-RU True, если активен NonAntiAliased.&]
[s3; &]
[s4; &]
[s5;:Font`:`:FaceName`(const String`&`): [_^Font^ Font][@(0.0.255) `&]_[* FaceName]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[@3 name])&]
[s2;%RU-RU Устанавливает фас в [%-*@3 name]. Если 
name является неполноценным именем 
фаса, то Font устанавливается в Null. Метод 
способен распознать генерные имена 
`"serif`", `"sansserif`", `"monospace`" и `"STDFONT`".&]
[s3; &]
[s4; &]
[s5;:Font`:`:operator`(`)`(`)const: [_^Font^ Font][@(64) _][@(0.0.255) operator]()()[@(64) _][@(0.0.255) c
onst]&]
[s2;%RU-RU Возвращает копию Font. Применим 
для упрощения нотации, при изменении 
существующих значений Font`'а.&]
[s3; &]
[s4; &]
[s5;:Font`:`:operator`(`)`(int`)const: [_^Font^ Font][@(64) _][@(0.0.255) operator]()([@(0.0.255) i
nt]_[@3 n])_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает копию Font, с изменением 
высоты до [%-*@3 n].&]
[s3; &]
[s4; &]
[s5;:Font`:`:Serialize`(Stream`&`): [@(0.0.255) void]_[* Serialize]([_^Stream^ Stream][@(0.0.255) `&
]_[@3 s])&]
[s2;%RU-RU Сериализует значение шрифта (индекс 
фаса сохраняется как текст имени 
фаса).&]
[s3; &]
[s4; &]
[s5;:Font`:`:Jsonize`(JsonIO`&`): [@(0.0.255) void]_[* Jsonize]([_^JsonIO^ JsonIO][@(0.0.255) `&
]_[*@3 jio])&]
[s2;%RU-RU Сохранить/загрузить шрифт в/из 
JSON.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Font`:`:Xmlize`(XmlIO`&`): [@(0.0.255) void]_[* Xmlize]([_^XmlIO^ XmlIO][@(0.0.255) `&]_
[*@3 xio])&]
[s2;%RU-RU Сохранить/загрузить шрифт в/из 
XML.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Font`:`:operator`=`=`(Font`)const: [@(0.0.255) bool]_[@(0.0.255) operator`=`=]([_^Font^ F
ont]_[@3 f])_[@(0.0.255) const]&]
[s5;:Font`:`:operator`!`=`(Font`)const: [@(0.0.255) bool]_[@(0.0.255) operator!`=]([_^Font^ F
ont]_[@3 f])_[@(0.0.255) const]&]
[s2;%RU-RU Сравнивает два экземпляра Font.&]
[s3; &]
[s4; &]
[s5;:Font`:`:GetHashValue`(`)const: [_^dword^ dword]_[* GetHashValue]()[@(64) _][@(0.0.255) c
onst]&]
[s2;%RU-RU Возвращает хэш`-значение шрифта.&]
[s3; &]
[s4; &]
[s5;:Font`:`:IsNullInstance`(`)const: [@(0.0.255) bool]_[* IsNullInstance]()_[@(0.0.255) co
nst]&]
[s2;%RU-RU True, если Font равен Null. (Метод используется 
дефолтным шаблоном IsNull).&]
[s3; &]
[s4; &]
[s5;:Font`:`:SetNull`(`): [@(0.0.255) void]_[* SetNull]()&]
[s2;%RU-RU Устанавливает Font в Null.&]
[s3; &]
[s4; &]
[s5;:Font`:`:GetAscent`(`)const: [@(0.0.255) int]_[* GetAscent]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает протяженность ячейки 
символа над основанием (baseline).&]
[s3; &]
[s4; &]
[s5;:Font`:`:GetDescent`(`)const: [@(0.0.255) int]_[* GetDescent]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает протяженность ячейки 
символа под основанием (baseline).&]
[s3; &]
[s4; &]
[s5;:Font`:`:GetCy`(`)const: [@(0.0.255) int]_[* GetCy]()_[@(0.0.255) const]&]
[s2;%RU-RU То же, что и GetDescent() `+ GetAscent().&]
[s3; &]
[s4; &]
[s5;:Font`:`:GetExternal`(`)const: [@(0.0.255) int]_[* GetExternal]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает дополнительное пространство
, которое по описанию шрифта рекомендуется 
вставлять между двух строк текста. 
Используется редко.&]
[s3; &]
[s4; &]
[s5;:Font`:`:GetInternal`(`)const: [@(0.0.255) int]_[* GetInternal]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает пустующее пространство 
внутри значения GetAscent. Используется 
редко.&]
[s3; &]
[s4; &]
[s5;:Font`:`:GetHeight`(`)const: [@(0.0.255) int]_[* GetHeight]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает GetAscent() `+ GetDescent() `- высоту 
строки текста.&]
[s3; &]
[s4; &]
[s5;:Font`:`:GetLineHeight`(`)const: [@(0.0.255) int]_[* GetLineHeight]()_[@(0.0.255) const
]&]
[s2;%RU-RU Возвращает рекомендуемую общую 
высоту строк.&]
[s3; &]
[s4; &]
[s5;:Font`:`:GetOverhang`(`)const: [@(0.0.255) int]_[* GetOverhang]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает overhang шрифта.&]
[s3; &]
[s4; &]
[s5;:Font`:`:GetAveWidth`(`)const: [@(0.0.255) int]_[* GetAveWidth]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает среднюю ширину символа. 
Зависит от информации от провайдера 
шрифта, которая не всегда надёжна, 
`- лучше всего получить информацию 
о spacing`'е из ширины отдельных глифов.&]
[s3; &]
[s4; &]
[s5;:Font`:`:GetMaxWidth`(`)const: [@(0.0.255) int]_[* GetMaxWidth]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает максимальную ширину 
символа. Зависит от информации от 
провайдера шрифта, которая не всегда 
надёжна, `- лучше всего получить информацию 
о spacing`'е из ширины отдельных глифов.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Font`:`:GetMonoWidth`(`)const: [@(0.0.255) int]_[* GetMonoWidth]()_[@(0.0.255) c
onst]&]
[s2;%RU-RU Возвращает предполагаемую ширину 
ячейки символа, если шрифт моноширинный. 
Этот метод используется в LineEdit для 
определения ширины ячейки. На данный 
момент, значение max(GetWidth(`'M`'), GetWidth(`'W`')) 
используется как GetAveWidth и GetMaxWidth, так 
как применять информацию из метаданных 
шрифта ненадёжно.&]
[s3; &]
[s4; &]
[s5;:Font`:`:IsNormal`(int`)const: [@(0.0.255) bool]_[* IsNormal]([@(0.0.255) int]_[@3 ch])_[@(0.0.255) c
onst]&]
[s2;%RU-RU Возвращает true, если [%-*@3 ch] существует 
в шрифте как регулярный глиф.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Font`:`:IsComposed`(int`)const: [@(0.0.255) bool]_[* IsComposed]([@(0.0.255) int]_[@3 ch
])_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает true, если [%-*@3 ch].нужно 
отображать как сочетание 2`-х других 
глифов (буква ASCII и диакритическая 
метка).&]
[s3;%RU-RU &]
[s4; &]
[s5;:Font`:`:IsReplaced`(int`)const: [@(0.0.255) bool]_[* IsReplaced]([@(0.0.255) int]_[@3 ch
])_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает true, если [%-*@3 ch] нужно 
отображать символом из какого`-либо 
иного шрифта.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Font`:`:IsMissing`(int`)const: [@(0.0.255) bool]_[* IsMissing]([@(0.0.255) int]_[@3 ch])
_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает true, если [%-*@3 ch] нельзя 
отобразить.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Font`:`:HasChar`(int`)const: [@(0.0.255) int]_HasChar([@(0.0.255) int]_[@3 ch])_[@(0.0.255) c
onst]&]
[s2;%RU-RU Возвращает true, если [%-*@3 ch] можно 
отобразить (любым образом).&]
[s3;%RU-RU &]
[s4; &]
[s5;:Font`:`:GetWidth`(int`)const: [@(0.0.255) int]_[* GetWidth]([@(0.0.255) int]_[@3 c])_[@(0.0.255) c
onst]&]
[s2;%RU-RU Возвращает общий аванс ширины 
символа [%-*@3 c] (кодированного в unicode).&]
[s3; &]
[s4; &]
[s5;:Font`:`:operator`[`]`(int`)const: [@(0.0.255) int]_[@(0.0.255) operator`[`]]([@(0.0.255) i
nt]_[@3 c])_[@(0.0.255) const]&]
[s2;%RU-RU То же, что и GetWidth([%-*@3 c]).&]
[s3; &]
[s4; &]
[s5;:Font`:`:GetLeftSpace`(int`)const: [@(0.0.255) int]_[* GetLeftSpace]([@(0.0.255) int]_[@3 c
])_[@(0.0.255) const]&]
[s2;%RU-RU Описывает отношение левой стороны 
ячейки символа и левой отрисовываемой 
области символа [%-*@3 c]. Отрицательное 
значение означает, что символ превышает 
ячейку, положительное `- что есть внутри 
ячейки пространство, неиспользуемое 
глифом.&]
[s3; &]
[s4; &]
[s5;:Font`:`:GetRightSpace`(int`)const: [@(0.0.255) int]_[* GetRightSpace]([@(0.0.255) int]_
[@3 c])_[@(0.0.255) const]&]
[s2;%RU-RU Подобно GetLeftSpace для правого края 
ячейки символа.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Font`:`:IsFixedPitch`(`)const: [@(0.0.255) bool]_[* IsFixedPitch]()_[@(0.0.255) const]&]
[s2;%RU-RU True, если шрифт моноширинный.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Font`:`:IsSerif`(`)const: [@(0.0.255) bool]_[* IsSerif]()_[@(0.0.255) const]&]
[s2;%RU-RU True, если у шрифта стиль serif.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Font`:`:IsScript`(`)const: [@(0.0.255) bool]_[* IsScript]()_[@(0.0.255) const]&]
[s2;%RU-RU True, если у шрифта стиль сценария.&]
[s3; &]
[s4; &]
[s5;:Font`:`:IsScaleable`(`)const: [@(0.0.255) bool]_[* IsScaleable]()_[@(0.0.255) const]&]
[s2;%RU-RU True, если шрифт свободно масштабируемый.&]
[s3; &]
[s4; &]
[s5;:Font`:`:IsSpecial`(`)const: [@(0.0.255) bool]_[* IsSpecial]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает true, если шрифт не использует 
unicode placement of glyphs, напр., у некоторых 
символьных шрифтов есть такая проблема.&]
[s3; &]
[s4; &]
[s5;:Font`:`:IsTrueType`(`)const: [@(0.0.255) bool]_[* IsTrueType]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает true, если шрифт TrueType. 
(Заметьте, что только печать в PdfDraw 
и на Linux могут обрабатывать шрифты 
TrueType).&]
[s3; &]
[s4; &]
[s5;:Font`:`:GetPath`(`)const: [_^String^ String]_[* GetPath]()[@(64) _][@(0.0.255) const]&]
[s2;%RU-RU [/ Специфика POSIX:] Возвращает путь 
к файлу шрифта.&]
[s3; &]
[s4; &]
[s5;:Font`:`:GetTextFlags`(`)const: [_^String^ String]_[* GetTextFlags]()_[@(0.0.255) const
]&]
[s2;%RU-RU Возвращает флаги шрифта (типа 
Bold) в подробной строковой форме (напр., 
`"bold underline`").&]
[s3; &]
[s4; &]
[s5;:Font`:`:ParseTextFlags`(const char`*`): [@(0.0.255) void]_[* ParseTextFlags]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 s])&]
[s2;%RU-RU Устанавливает флаги шрифта, на 
основе текста в формате, созданном 
GetTextFlags.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Font`:`:GetData`(`)const: [_^String^ String]_[* GetData]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает сырой контент файла 
шрифта.&]
[s3; &]
[s3; &]
[s4; &]
[s5;:Upp`:`:Font`:`:GetData`(const char`*`,int`,int`)const: [_^Upp`:`:String^ String]_[* G
etData]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 table]_`=_NULL, 
[@(0.0.255) int]_[*@3 offset]_`=_`-[@3 1], [@(0.0.255) int]_[*@3 size]_`=_`-[@3 1])_[@(0.0.255) c
onst]&]
[s2;%RU-RU Возвращает сырой контент файла 
шрифта. [%-*@3 table] [%-*@3 offset] [%-*@3 size] может 
задавать, какую часть данных шрифта 
TTF или OTF нужно получить.&]
[s0;%RU-RU &]
[s4; &]
[s5;:Font`:`:Render`(FontGlyphConsumer`&`,double`,double`,int`)const: [@(0.0.255) void]_
[* Render]([_^FontGlyphConsumer^ FontGlyphConsumer][@(0.0.255) `&]_[*@3 sw], 
[@(0.0.255) double]_[*@3 x], [@(0.0.255) double]_[*@3 y], [@(0.0.255) int]_[*@3 ch])_[@(0.0.255) c
onst]&]
[s2;%RU-RU Для шрифтов true`-type, отображает 
линии и кривые глифа символа.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Font`:`:Font`(`): [* Font]()&]
[s2;%RU-RU Инициализует Font в STDFONT, дефолтной 
высоты, все атрибуты неактивны.&]
[s3; &]
[s4; &]
[s5;:Font`:`:Font`(int`,int`): [* Font]([@(0.0.255) int]_[*@3 face], [@(0.0.255) int]_[*@3 heig
ht])&]
[s2;%RU-RU Инициализует шрифт в индекс фаса 
[%-*@3 face] и высоту [%-*@3 height].&]
[s3; &]
[s4; &]
[s5;:Font`:`:Font`(const Nuller`&`): [* Font]([@(0.0.255) const]_[_^Nuller^ Nuller][@(0.0.255) `&
])&]
[s2;%RU-RU Устанавливает шрифт Null.&]
[s3; &]
[s4; &]
[s5;:Font`:`:operator Value`(`)const: [* operator_Value]()[@(64) _][@(0.0.255) const]&]
[s2;%RU-RU Преобразует Font в Value (шрифт является 
rich Value).&]
[s3; &]
[s4; &]
[s5;:Font`:`:Font`(const Value`&`): [* Font]([@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&
]_[@3 q])&]
[s2;%RU-RU Преобразует Value в Font.&]
[s3; &]
[s0; ]]