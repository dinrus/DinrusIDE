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
простую опреацию копирования и является 
Value, преобразовываемым со способностями 
rich`-Value.&]
[s9;%RU-RU Он хранит несколько атриутов, 
описывающих текст. Соответствующий 
системный шрифт хранится в форме 
индекса фаса (face index) `- у каждого смстемного 
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
unless SetStdFont was used. This variant is used by Chameleon 
skin to set font matching platform setting, while SetStdFont 
can be used by client code to override this setting (and thus 
has higher priority).&]
[s3;%RU-RU &]
[s4; &]
[s5;:Font`:`:SetStdFont`(Font`): [@(0.0.255) static] [@(0.0.255) void]_[* SetStdFont]([_^Font^ F
ont]_[@3 font])&]
[s2;%RU-RU Устанавливает стандартный шрифт. 
U`+`+ sets the standard font to match host platform standard. 
This method can be used to change this default setting.&]
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
[s2;%RU-RU Similar to GetFaceName, but returns generic names `"serif`", 
`"sansserif`", `"monospace`", `"STDFONT`" for corresponding faces, 
instead of real names.&]
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
[s2;%RU-RU Replaces Null or STDFONT with actual standard GUI font 
definition. If font is Null, it is set to standard font with 
all attributes. If face is STDFONT, it is replaced with actual 
face of standard font. If font height is `-32000 (which is value 
StdFont function normally returns), it is replaced with standard 
font height.&]
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
[s2; Устанавливает or unsets bold/italic/undeline/strikeout 
modes.&]
[s3; &]
[s4; &]
[s5;:Font`:`:NonAntiAliased`(`): [_^Font^ Font][@(0.0.255) `&]_[* NonAntiAliased]()&]
[s5;:Font`:`:NoNonAntiAliased`(`): [_^Font^ Font][@(0.0.255) `&]_[* NoNonAntiAliased]()&]
[s5;:Font`:`:NonAntiAliased`(bool`): [_^Font^ Font][@(0.0.255) `&]_[* NonAntiAliased]([@(0.0.255) b
ool]_[@3 b])&]
[s6; `[deprecated`]&]
[s2;%RU-RU Sets/unsets non`-anti`-aliased flag. This indicates that 
anti`-aliasing should not be used when painting the font.&]
[s3; &]
[s4; &]
[s5;:Font`:`:IsBold`(`)const: [@(0.0.255) bool]_[* IsBold]()_[@(0.0.255) const]&]
[s5;:Font`:`:IsItalic`(`)const: [@(0.0.255) bool]_[* IsItalic]()_[@(0.0.255) const]&]
[s5;:Font`:`:IsUnderline`(`)const: [@(0.0.255) bool]_[* IsUnderline]()_[@(0.0.255) const]&]
[s5;:Font`:`:IsStrikeout`(`)const: [@(0.0.255) bool]_[* IsStrikeout]()_[@(0.0.255) const]&]
[s2; Tests whether bold/italic/underline/strikeout is active.&]
[s3; &]
[s4; &]
[s5;:Font`:`:IsNonAntiAliased`(`)const: [@(0.0.255) bool]_[* IsNonAntiAliased]()_[@(0.0.255) c
onst]&]
[s2;%RU-RU True if NonAntiAliased is active.&]
[s3; &]
[s4; &]
[s5;:Font`:`:FaceName`(const String`&`): [_^Font^ Font][@(0.0.255) `&]_[* FaceName]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[@3 name])&]
[s2;%RU-RU Устанавливает фас в [%-*@3 name]. If name 
is not valid face`-name, Font is set to Null. Method is able 
to recognize generic names `"serif`", `"sansserif`", `"monospace`" 
and `"STDFONT`".&]
[s3; &]
[s4; &]
[s5;:Font`:`:operator`(`)`(`)const: [_^Font^ Font][@(64) _][@(0.0.255) operator]()()[@(64) _][@(0.0.255) c
onst]&]
[s2;%RU-RU Возвращает копию Font. Useful to simplify 
notation when altering existing Font values.&]
[s3; &]
[s4; &]
[s5;:Font`:`:operator`(`)`(int`)const: [_^Font^ Font][@(64) _][@(0.0.255) operator]()([@(0.0.255) i
nt]_[@3 n])_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает a copy of Font, with height changed 
to [%-*@3 n].&]
[s3; &]
[s4; &]
[s5;:Font`:`:Serialize`(Stream`&`): [@(0.0.255) void]_[* Serialize]([_^Stream^ Stream][@(0.0.255) `&
]_[@3 s])&]
[s2;%RU-RU Serializes the font value (face index is stored as face`-name 
text).&]
[s3; &]
[s4; &]
[s5;:Font`:`:Jsonize`(JsonIO`&`): [@(0.0.255) void]_[* Jsonize]([_^JsonIO^ JsonIO][@(0.0.255) `&
]_[*@3 jio])&]
[s2;%RU-RU Stores/load font to/from JSON.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Font`:`:Xmlize`(XmlIO`&`): [@(0.0.255) void]_[* Xmlize]([_^XmlIO^ XmlIO][@(0.0.255) `&]_
[*@3 xio])&]
[s2;%RU-RU Stores/load font to/from JSON..&]
[s3;%RU-RU &]
[s4; &]
[s5;:Font`:`:operator`=`=`(Font`)const: [@(0.0.255) bool]_[@(0.0.255) operator`=`=]([_^Font^ F
ont]_[@3 f])_[@(0.0.255) const]&]
[s5;:Font`:`:operator`!`=`(Font`)const: [@(0.0.255) bool]_[@(0.0.255) operator!`=]([_^Font^ F
ont]_[@3 f])_[@(0.0.255) const]&]
[s2;%RU-RU Compares two Font instances.&]
[s3; &]
[s4; &]
[s5;:Font`:`:GetHashValue`(`)const: [_^dword^ dword]_[* GetHashValue]()[@(64) _][@(0.0.255) c
onst]&]
[s2;%RU-RU Возвращает the font hash value.&]
[s3; &]
[s4; &]
[s5;:Font`:`:IsNullInstance`(`)const: [@(0.0.255) bool]_[* IsNullInstance]()_[@(0.0.255) co
nst]&]
[s2;%RU-RU True if Font is Null. (This method is used by default 
IsNull template).&]
[s3; &]
[s4; &]
[s5;:Font`:`:SetNull`(`): [@(0.0.255) void]_[* SetNull]()&]
[s2;%RU-RU Устанавливает Font в Null.&]
[s3; &]
[s4; &]
[s5;:Font`:`:GetAscent`(`)const: [@(0.0.255) int]_[* GetAscent]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает the character cell extend above the 
baseline.&]
[s3; &]
[s4; &]
[s5;:Font`:`:GetDescent`(`)const: [@(0.0.255) int]_[* GetDescent]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает the character cell extend below the 
baseline.&]
[s3; &]
[s4; &]
[s5;:Font`:`:GetCy`(`)const: [@(0.0.255) int]_[* GetCy]()_[@(0.0.255) const]&]
[s2;%RU-RU То же, что и GetDescent() `+ GetAscent().&]
[s3; &]
[s4; &]
[s5;:Font`:`:GetExternal`(`)const: [@(0.0.255) int]_[* GetExternal]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает the additional space that font description 
recommends to insert between two lines of text. Rarely used.&]
[s3; &]
[s4; &]
[s5;:Font`:`:GetInternal`(`)const: [@(0.0.255) int]_[* GetInternal]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает the mostly free space inside GetAscent 
value. Rarely used.&]
[s3; &]
[s4; &]
[s5;:Font`:`:GetHeight`(`)const: [@(0.0.255) int]_[* GetHeight]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает GetAscent() `+ GetDescent() `- the 
height of the line of text.&]
[s3; &]
[s4; &]
[s5;:Font`:`:GetLineHeight`(`)const: [@(0.0.255) int]_[* GetLineHeight]()_[@(0.0.255) const
]&]
[s2;%RU-RU Возвращает suggested total line height.&]
[s3; &]
[s4; &]
[s5;:Font`:`:GetOverhang`(`)const: [@(0.0.255) int]_[* GetOverhang]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает overhang of font.&]
[s3; &]
[s4; &]
[s5;:Font`:`:GetAveWidth`(`)const: [@(0.0.255) int]_[* GetAveWidth]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает the average width of character. Depends 
on information from the font provider, which is not always reliable 
`- it is better the obtain the spacing info from the width of 
individual glyphs.&]
[s3; &]
[s4; &]
[s5;:Font`:`:GetMaxWidth`(`)const: [@(0.0.255) int]_[* GetMaxWidth]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает the maximal width of character. Depends 
on information from the font provider, which is not always reliable 
`- it is better the obtain the spacing info from the width of 
individual glyphs.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Font`:`:GetMonoWidth`(`)const: [@(0.0.255) int]_[* GetMonoWidth]()_[@(0.0.255) c
onst]&]
[s2;%RU-RU Возвращает supposed width of character cell 
if the font is (or is considered) monospace. This is the method 
used in LineEdit to determine cell width. Currently, max(GetWidth(`'M`'), 
GetWidth(`'W`')) value is used as GetAveWidth and GetMaxWidth 
using the information from the font metadata is unreliable.&]
[s3; &]
[s4; &]
[s5;:Font`:`:IsNormal`(int`)const: [@(0.0.255) bool]_[* IsNormal]([@(0.0.255) int]_[@3 ch])_[@(0.0.255) c
onst]&]
[s2;%RU-RU Возвращает true, если [%-*@3 ch] exists as 
regular glyph in the font.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Font`:`:IsComposed`(int`)const: [@(0.0.255) bool]_[* IsComposed]([@(0.0.255) int]_[@3 ch
])_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает true, если [%-*@3 ch].is to be 
rendered as composition of 2 other glyphs (ASCII letter and diacritical 
mark).&]
[s3;%RU-RU &]
[s4; &]
[s5;:Font`:`:IsReplaced`(int`)const: [@(0.0.255) bool]_[* IsReplaced]([@(0.0.255) int]_[@3 ch
])_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает true [%-*@3 ch] is to be rendered using 
character from some other font.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Font`:`:IsMissing`(int`)const: [@(0.0.255) bool]_[* IsMissing]([@(0.0.255) int]_[@3 ch])
_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает true, если [%-*@3 ch] cannot be 
rendered.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Font`:`:HasChar`(int`)const: [@(0.0.255) int]_HasChar([@(0.0.255) int]_[@3 ch])_[@(0.0.255) c
onst]&]
[s2;%RU-RU Возвращает true, если [%-*@3 ch] can be rendered 
(in any way).&]
[s3;%RU-RU &]
[s4; &]
[s5;:Font`:`:GetWidth`(int`)const: [@(0.0.255) int]_[* GetWidth]([@(0.0.255) int]_[@3 c])_[@(0.0.255) c
onst]&]
[s2;%RU-RU Возвращает the total advance with of character 
[%-*@3 c] (encoded in unicode).&]
[s3; &]
[s4; &]
[s5;:Font`:`:operator`[`]`(int`)const: [@(0.0.255) int]_[@(0.0.255) operator`[`]]([@(0.0.255) i
nt]_[@3 c])_[@(0.0.255) const]&]
[s2;%RU-RU То же, что и GetWidth([%-*@3 c]).&]
[s3; &]
[s4; &]
[s5;:Font`:`:GetLeftSpace`(int`)const: [@(0.0.255) int]_[* GetLeftSpace]([@(0.0.255) int]_[@3 c
])_[@(0.0.255) const]&]
[s2;%RU-RU Describes the relation of left side of character cell 
and leftmost area painted for character [%-*@3 c]. Negative value 
means that character extends character cell, positive value means 
that there is a space inside the cell not used for glyph.&]
[s3; &]
[s4; &]
[s5;:Font`:`:GetRightSpace`(int`)const: [@(0.0.255) int]_[* GetRightSpace]([@(0.0.255) int]_
[@3 c])_[@(0.0.255) const]&]
[s2;%RU-RU Similar to GetLeftSpace for the right edge of character 
cell.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Font`:`:IsFixedPitch`(`)const: [@(0.0.255) bool]_[* IsFixedPitch]()_[@(0.0.255) const]&]
[s2;%RU-RU True if font is mono`-spaced.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Font`:`:IsSerif`(`)const: [@(0.0.255) bool]_[* IsSerif]()_[@(0.0.255) const]&]
[s2;%RU-RU True if font has serif style.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Font`:`:IsScript`(`)const: [@(0.0.255) bool]_[* IsScript]()_[@(0.0.255) const]&]
[s2;%RU-RU True if font has script style.&]
[s3; &]
[s4; &]
[s5;:Font`:`:IsScaleable`(`)const: [@(0.0.255) bool]_[* IsScaleable]()_[@(0.0.255) const]&]
[s2;%RU-RU True if font is freely scaleable.&]
[s3; &]
[s4; &]
[s5;:Font`:`:IsSpecial`(`)const: [@(0.0.255) bool]_[* IsSpecial]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает true, если font does not use 
unicode placement of glyphs, for example some symbol fonts have 
this issue.&]
[s3; &]
[s4; &]
[s5;:Font`:`:IsTrueType`(`)const: [@(0.0.255) bool]_[* IsTrueType]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает true, если font is TrueType font. 
(Note that PdfDraw and Linux printing are only able to handle 
TrueType fonts).&]
[s3; &]
[s4; &]
[s5;:Font`:`:GetPath`(`)const: [_^String^ String]_[* GetPath]()[@(64) _][@(0.0.255) const]&]
[s2;%RU-RU [/ POSIX specific:] Возвращает the path of font 
file.&]
[s3; &]
[s4; &]
[s5;:Font`:`:GetTextFlags`(`)const: [_^String^ String]_[* GetTextFlags]()_[@(0.0.255) const
]&]
[s2;%RU-RU Возвращает font flags (like Bold) in verbose 
string form (e.g. `"bold underline`").&]
[s3; &]
[s4; &]
[s5;:Font`:`:ParseTextFlags`(const char`*`): [@(0.0.255) void]_[* ParseTextFlags]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 s])&]
[s2;%RU-RU Устанавливаетfont flags based on text in 
format created by GetTextFlags.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Font`:`:GetData`(`)const: [_^String^ String]_[* GetData]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает the raw content of font file.&]
[s3; &]
[s3; &]
[s4; &]
[s5;:Upp`:`:Font`:`:GetData`(const char`*`,int`,int`)const: [_^Upp`:`:String^ String]_[* G
etData]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 table]_`=_NULL, 
[@(0.0.255) int]_[*@3 offset]_`=_`-[@3 1], [@(0.0.255) int]_[*@3 size]_`=_`-[@3 1])_[@(0.0.255) c
onst]&]
[s2;%RU-RU Возвращает the raw content of font file. [%-*@3 table] 
[%-*@3 offset] [%-*@3 size] can specify which part of TTF or OTF 
font data to retrieve.&]
[s0;%RU-RU &]
[s4; &]
[s5;:Font`:`:Render`(FontGlyphConsumer`&`,double`,double`,int`)const: [@(0.0.255) void]_
[* Render]([_^FontGlyphConsumer^ FontGlyphConsumer][@(0.0.255) `&]_[*@3 sw], 
[@(0.0.255) double]_[*@3 x], [@(0.0.255) double]_[*@3 y], [@(0.0.255) int]_[*@3 ch])_[@(0.0.255) c
onst]&]
[s2;%RU-RU For true`-type fonts, renders the character glyph lines 
and curves.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Font`:`:Font`(`): [* Font]()&]
[s2;%RU-RU Initializes Font to STDFONT, default height, all attributes 
not active.&]
[s3; &]
[s4; &]
[s5;:Font`:`:Font`(int`,int`): [* Font]([@(0.0.255) int]_[*@3 face], [@(0.0.255) int]_[*@3 heig
ht])&]
[s2;%RU-RU Initializes font to [%-*@3 face] index and [%-*@3 height].&]
[s3; &]
[s4; &]
[s5;:Font`:`:Font`(const Nuller`&`): [* Font]([@(0.0.255) const]_[_^Nuller^ Nuller][@(0.0.255) `&
])&]
[s2;%RU-RU Устанавливает шрифт Null.&]
[s3; &]
[s4; &]
[s5;:Font`:`:operator Value`(`)const: [* operator_Value]()[@(64) _][@(0.0.255) const]&]
[s2;%RU-RU Converts the Font to the Value (font is rich Value).&]
[s3; &]
[s4; &]
[s5;:Font`:`:Font`(const Value`&`): [* Font]([@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&
]_[@3 q])&]
[s2;%RU-RU Converts the Value to the Font.&]
[s3; &]
[s0; ]]