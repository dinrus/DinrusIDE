topic "Процедуры для Шрифта и Текста";
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
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Процедуры для Шрифта и Текста]]}}&]
[s3; &]
[s5;:Upp`:`:SetStdFont`(Upp`:`:Font`): [@(0.0.255) void]_[* SetStdFont]([_^Upp`:`:Font^ Fon
t]_[*@3 font])&]
[s2;%RU-RU Устанавливает стандартный шрифт. 
Это дефолтный шрифт, используемый 
для отрисовки большинства текстов 
ГИП. Стандартный шрифт, как правило, 
устанавливается при стартапе ГИП 
и приложению практически не приходится 
вызывать эту функцию.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:GetStdFont`(`): [_^Upp`:`:Font^ Font]_[* GetStdFont]()&]
[s2;%RU-RU Возвращает стандартный шрифт.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:GetStdFontCy`(`): [@(0.0.255) int]_[* GetStdFontCy]()&]
[s2;%RU-RU Возвращает высоту стандартного 
шрифта (в пикселях или точках).&]
[s3; &]
[s4; &]
[s5;:Upp`:`:StdFont`(`): [_^Upp`:`:Font^ Font]_[* StdFont]()&]
[s2;%RU-RU То же, что и GetStdFont().&]
[s3; &]
[s4; &]
[s5;:Upp`:`:StdFont`(int`): [_^Upp`:`:Font^ Font]_[* StdFont]([@(0.0.255) int]_[*@3 h])&]
[s2;%RU-RU Возвращает стандарный шрифт с 
высотой, изменённой на [%-*@3 h].&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:Serif`(int`): [_^Upp`:`:Font^ Font]_[* Serif]([@(0.0.255) int]_[*@3 n])&]
[s2;%RU-RU Возвращает дефолтный шрифт serif 
с высотой [%-*@3 n].&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:SansSerif`(int`): [_^Upp`:`:Font^ Font]_[* SansSerif]([@(0.0.255) int]_[*@3 n]_`=
_`-[@3 32000])&]
[s2;%RU-RU Возвращает дефолтный шрифт sans`-serif 
с высотой [%-*@3 n].&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:Monospace`(int`): [_^Upp`:`:Font^ Font]_[* Monospace]([@(0.0.255) int]_[*@3 n]_`=
_`-[@3 32000])&]
[s2;%RU-RU Возвращает дефолтный моноширинный 
шрифт с высотой [%-*@3 n].&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:Compose`(Upp`:`:Font`,int`,Upp`:`:ComposedGlyph`&`): [@(0.0.255) bool]_[* Com
pose]([_^Upp`:`:Font^ Font]_[*@3 font], [@(0.0.255) int]_[*@3 chr], [_^Upp`:`:ComposedGlyph^ C
omposedGlyph][@(0.0.255) `&]_[*@3 cg])&]
[s2;%RU-RU Эта функция используется реализацией 
DrawText, когда точка Юникода [%-*@3 chr] в шрифте 
[%-*@3 font ]отсутствует. Функция пытается 
найти два глифа, один из исходного 
шрифта, а второй, возможно, из другого 
шрифта, который, при компонировании, 
даёт символ, соответствующий требуемой 
кодточке (напр.,. [*@(255.0.255) C] `+ [*@(255.0.255) ˇ] 
`= [*@(255.0.255) Č]). Если такие глифы существуют, 
они передаются в [%-*@3 cg] и функция возвращает 
true.&]
[s3;%RU-RU &]
[s4; &]
[s5;:GetTextSize`(const wchar`*`,Font`,int`): [_^Size^ Size]_[* GetTextSize]([@(0.0.255) co
nst]_[_^wchar^ wchar]_`*[*@3 text], [_^Font^ Font]_[*@3 font], [@(0.0.255) int]_[*@3 n]_`=_`-
[@3 1])&]
[s5;:GetTextSize`(const WString`&`,Font`): [_^Size^ Size]_[* GetTextSize]([@(0.0.255) const
]_[_^WString^ WString][@(0.0.255) `&]_[*@3 text], [_^Font^ Font]_[*@3 font])&]
[s5;:GetTextSize`(const char`*`,byte`,Font`,int`): [_^Size^ Size]_[* GetTextSize]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 text], [_^byte^ byte]_[*@3 charset], [_^Font^ Font]_[*@3 font],
 [@(0.0.255) int]_[*@3 n]_`=_`-[@3 1])&]
[s5;:GetTextSize`(const char`*`,Font`,int`): [_^Size^ Size]_[* GetTextSize]([@(0.0.255) con
st]_[@(0.0.255) char]_`*[*@3 text], [_^Font^ Font]_[*@3 font], [@(0.0.255) int]_[*@3 n]_`=_`-
[@3 1])&]
[s5;:GetTextSize`(const String`&`,Font`): [_^Size^ Size]_[* GetTextSize]([@(0.0.255) const]_
[_^String^ String][@(0.0.255) `&]_[*@3 text], [_^Font^ Font]_[*@3 font])&]
[s2;%RU-RU Вычисляет графический размер 
текста [%-*@3 text], отрисованного шрифтом 
[%-*@3 font], используя метрику шрифта. Возвращённа
я высота является высотой шрифта 
(подъём`+спуск), returned with is the total advance 
width of text. No overhangs are considered. [%-*@3 n] `-это 
число символов; если отрицательное 
(дефолтное значение), то его получают 
посредством `[w`]strlen. Текст может быть 
либо Юникодом, либо в 8`-битной кодировке; 
в таком случае либо предоставляется 
набсим [%-*@3 charset], или используется дефолтный 
набор символов (гарнитура).&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:DrawTextEllipsis`(Upp`:`:Draw`&`,int`,int`,int`,const char`*`,const char`*`,Upp`:`:Font`,Upp`:`:Color`,int`): [@(0.0.255) v
oid]_[* DrawTextEllipsis]([_^Upp`:`:Draw^ Draw][@(0.0.255) `&]_[*@3 w], 
[@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) int]_[*@3 cx], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 text], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 e
llipsis], [_^Upp`:`:Font^ Font]_[*@3 font]_`=_StdFont(), [_^Upp`:`:Color^ Color]_[*@3 ink
]_`=_SColorText(), [@(0.0.255) int]_[*@3 n]_`=_`-[@3 1])&]
[s5;:Upp`:`:DrawTextEllipsis`(Upp`:`:Draw`&`,int`,int`,int`,const Upp`:`:wchar`*`,const char`*`,Upp`:`:Font`,Upp`:`:Color`,int`): [@(0.0.255) v
oid]_[* DrawTextEllipsis]([_^Upp`:`:Draw^ Draw][@(0.0.255) `&]_[*@3 w], 
[@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) int]_[*@3 cx], 
[@(0.0.255) const]_[_^Upp`:`:wchar^ wchar]_`*[*@3 text], [@(0.0.255) const]_[@(0.0.255) cha
r]_`*[*@3 ellipsis], [_^Upp`:`:Font^ Font]_[*@3 font]_`=_StdFont(), 
[_^Upp`:`:Color^ Color]_[*@3 ink]_`=_SColorText(), [@(0.0.255) int]_[*@3 n]_`=_`-[@3 1])&]
[s2;%RU-RU Чертит строку текста [%-*@3 text], ограниченную
 по ширине до [%-*@3 cx]. Если текст длинный 
и не умещается в [%-*@3 cx] , он обрезается 
так, чтобы умещался вместе с текстом 
[%-*@3 ellipsis], приставленным к [%-*@3 cx], а затем 
чертится с приставкой [%-*@3 ellipsis]. [%-*@3 n] 
`- число рисуемых символов, если `-1, 
то вместо него используется `[w`]strlen(text).&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:DrawTLText`(Upp`:`:Draw`&`,int`,int`,int`,const Upp`:`:wchar`*`,Upp`:`:Font`,Upp`:`:Color`,int`): [@(0.0.255) v
oid]_[* DrawTLText]([_^Upp`:`:Draw^ Draw][@(0.0.255) `&]_[*@3 draw], 
[@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) int]_[*@3 cx], 
[@(0.0.255) const]_[_^Upp`:`:wchar^ wchar]_`*[*@3 text], [_^Upp`:`:Font^ Font]_[*@3 font]_`=
_StdFont(), [_^Upp`:`:Color^ Color]_[*@3 ink]_`=_SColorText(), [@(0.0.255) int]_[*@3 acce
sskey]_`=_[@3 0])&]
[s2;%RU-RU TЭта функция чертить многострочный 
текст с табуляторами (содержит `'`\n`' 
и `'`\t`'), возможно, подчёркивается в 
тексте символ [%-*@3 accesskey]. Ширина ограничена 
[%-*@3 cx], текст переносится (word`-wrapped), 
при слишком длинной строке.&]
[s0;%RU-RU &]
[s4; &]
[s5;:Upp`:`:GetTLTextSize`(const Upp`:`:wchar`*`,Upp`:`:Font`): [_^Upp`:`:Size^ Size]_[* G
etTLTextSize]([@(0.0.255) const]_[_^Upp`:`:wchar^ wchar]_`*[*@3 text], 
[_^Upp`:`:Font^ Font]_[*@3 font]_`=_StdFont())&]
[s2;%RU-RU Возвращает размер многострочного 
текста с табуляторами.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:GetTLTextHeight`(const Upp`:`:wchar`*`,Upp`:`:Font`): [@(0.0.255) int]_[* Get
TLTextHeight]([@(0.0.255) const]_[_^Upp`:`:wchar^ wchar]_`*[*@3 s], 
[_^Upp`:`:Font^ Font]_[*@3 font])&]
[s2;%RU-RU То же, что и GetTLTextSize(s, font).cy.&]
[s3;%RU-RU &]
[s0; ]]