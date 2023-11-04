topic "SVG support";
[H6;0 $$1,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$2,0#37138531426314131252341829483370:codeitem]
[l288;2 $$3,0#27521748481378242620020725143825:desc]
[0 $$4,0#96390100711032703541132217272105:end]
[ $$0,0#00000000000000000000000000000000:Default]
[{_}%RU-RU 
[ {{10000@(113.42.0) [s0; [*@(229)4 SVG support]]}}&]
[s4; &]
[s2;:RenderSVG`(Painter`&`,const char`*`,Callback2`<String`,String`&`>`&`): [@(0.0.255) b
ool]_[* RenderSVG]([_^Painter^ Painter][@(0.0.255) `&]_[*@3 p], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 svg], [_^Callback2^ Callback2]<[_^String^ String], [_^String^ String][@(0.0.255) `&
]>`&_[*@3 resloader])&]
[s2;:RenderSVG`(Painter`&`,const char`*`): [@(0.0.255) bool]_[* RenderSVG]([_^Painter^ Pain
ter][@(0.0.255) `&]_[*@3 p], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 svg])&]
[s3; Отображает [*@3 svg ]на Painter [*@3 p]. Используйте 
[*@3 resloader], чтобы предоставить ресурсы 
типа рисунков, первая String обрвыза 
является именем ресурса (имя файла), 
а данные будут возвращены во второй 
параметр. Если ресурс не найден, возвратит 
пустую строку.&]
[s4; &]
[s1; &]
[s2;:GetSVGDimensions`(const char`*`,Sizef`&`,Rectf`&`): [@(0.0.255) void]_[* GetSVGDimen
sions]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 svg], [_^Sizef^ Sizef][@(0.0.255) `&]_[*@3 s
z], [_^Rectf^ Rectf][@(0.0.255) `&]_[*@3 viewbox])&]
[s3; Возвращает атрибуты size и viewbox для 
SVG.&]
[s4; &]
[s1; &]
[s2;:GetSVGBoundingBox`(const char`*`): [_^Rectf^ Rectf]_[* GetSVGBoundingBox]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 svg])&]
[s3; Вычисляет полностью ограничивающий 
периметр SVG.&]
[s4; &]
[s1;%- &]
[s2;:Upp`:`:GetSVGPathBoundingBox`(const char`*`):%- [_^Upp`:`:Rectf^ Rectf]_[* GetSVGPat
hBoundingBox]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 path])&]
[s3; Возвращает путь ограничивающего 
периметра [%-*@3 SVG], сохранённый в формате 
элемента пути.&]
[s4; &]
[s1; &]
[s2;:RenderSVGImage`(Size`,const char`*`,Callback2`<String`,String`&`>`&`): [_^Image^ I
mage]_[* RenderSVGImage]([_^Size^ Size]_[*@3 sz], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s
vg], [_^Callback2^ Callback2]<[_^String^ String], [_^String^ String][@(0.0.255) `&]>`&_[*@3 r
esloader])&]
[s2;:RenderSVGImage`(Size`,const char`*`): [_^Image^ Image]_[* RenderSVGImage]([_^Size^ Siz
e]_[*@3 sz], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 svg])&]
[s3; Отображает SVG в Image. Как правило, эта 
функция игнорирует размерности, определённы
е в SVG, и использует вычисленный органичивающ
ий периметр. Затем он масштабируется 
в [*@3 sz] при максимальном размере, сохраняющем 
пропорции. Используйте [*@3 resloader], чтобы 
предоставить ресурсы типа рисунков.&]
[s4; &]
[s1;%- &]
[s2;:Upp`:`:IsSVG`(const char`*`):%- [@(0.0.255) bool]_[* IsSVG]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 svg])&]
[s3; Возвращает  true, если есть вероятность, 
что [%-*@3 svg] содержит рисунок SVG.&]
[s4; &]
[s0; ]]