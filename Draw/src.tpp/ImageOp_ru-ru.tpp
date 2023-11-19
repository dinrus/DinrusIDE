topic "Утилитные Функции Image";
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
[ {{10000@(113.42.0) [s0;%RU-RU [*@(229)4 Утилитные Функции Image]]}}&]
[s3; &]
[s5;:Upp`:`:SetHotSpots`(Upp`:`:Image`&`,Upp`:`:Point`,Upp`:`:Point`): [@(0.0.255) void
]_[* SetHotSpots]([_^Upp`:`:Image^ Image][@(0.0.255) `&]_[*@3 m], [_^Upp`:`:Point^ Point]_[*@3 h
otspot], [_^Upp`:`:Point^ Point]_[*@3 hotspot2])&]
[s2;%RU-RU Устанавливает горячие точки в 
[%-*@3 m] в [%-*@3 hotspot], [%-*@3 hotspot2].&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:WithHotSpots`(const Upp`:`:Image`&`,Upp`:`:Point`,Upp`:`:Point`): [_^Upp`:`:Image^ I
mage]_[* WithHotSpots]([@(0.0.255) const]_[_^Upp`:`:Image^ Image][@(0.0.255) `&]_[*@3 m], 
[_^Upp`:`:Point^ Point]_[*@3 hotspot], [_^Upp`:`:Point^ Point]_[*@3 hotspot2])&]
[s5;:WithHotSpots`(const Image`&`,int`,int`,int`,int`): [_^Image^ Image]_[* WithHotSpots](
[@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 m], [@(0.0.255) int]_[*@3 x1], 
[@(0.0.255) int]_[*@3 y1], [@(0.0.255) int]_[*@3 x2], [@(0.0.255) int]_[*@3 y2])&]
[s2;%RU-RU Возвращает [%-*@3 m] с новыми горячими 
точками.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:WithHotSpot`(const Image`&`,int`,int`): [_^Image^ Image]_[* WithHotSpot]([@(0.0.255) c
onst]_[_^Image^ Image][@(0.0.255) `&]_[*@3 m], [@(0.0.255) int]_[*@3 x1], 
[@(0.0.255) int]_[*@3 y1])&]
[s2; [%RU-RU Возвращает ][*@3 m][%RU-RU  с горячей точкой 
][*@3 x1,][%RU-RU  ][*@3 y1.]&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:SetResolution`(Upp`:`:Image`&`,int`): [@(0.0.255) void]_[* SetResolution]([_^Upp`:`:Image^ I
mage][@(0.0.255) `&]_[*@3 m], [@(0.0.255) int]_[*@3 res])&]
[s2;%RU-RU Устанавливает разрешение у [%-*@3 m] 
в [%-*@3 res].&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:WithResolution`(const Upp`:`:Image`&`,int`): [_^Upp`:`:Image^ Image]_[* WithR
esolution]([@(0.0.255) const]_[_^Upp`:`:Image^ Image][@(0.0.255) `&]_[*@3 m], 
[@(0.0.255) int]_[*@3 res])&]
[s2;%RU-RU Возвращает [%-*@3 m] с разрешением 
[%-*@3 res].&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:WithResolution`(const Upp`:`:Image`&`,const Upp`:`:Image`&`): [_^Upp`:`:Image^ I
mage]_[* WithResolution]([@(0.0.255) const]_[_^Upp`:`:Image^ Image][@(0.0.255) `&]_[*@3 m],
 [@(0.0.255) const]_[_^Upp`:`:Image^ Image][@(0.0.255) `&]_[*@3 res])&]
[s2;%RU-RU Возвращает [%-*@3 m] с разрешением, 
равным [%-*@3 res].GetResolution().&]
[s3;%RU-RU &]
[s4; &]
[s5;:CreateImage`(Size`,const RGBA`&`): [_^Image^ Image]_[* CreateImage]([_^Size^ Size]_[*@3 s
z], [@(0.0.255) const]_[_^RGBA^ RGBA][@(0.0.255) `&]_[*@3 rgba])&]
[s2;%RU-RU Создаёт новый Image с Size [%-*@3 sz], залитым 
цветом [%-*@3 rgba].&]
[s3;%RU-RU &]
[s4; &]
[s5;:CreateImage`(Size`,Color`): [_^Image^ Image]_[* CreateImage]([_^Size^ Size]_[*@3 sz], 
[_^Color^ Color]_[*@3 color])&]
[s2;%RU-RU Создаёт новый Image с Size [%-*@3 sz], залитым 
цветом [%-*@3 color].&]
[s3;%RU-RU &]
[s4; &]
[s5;:DstSrc`(ImageBuffer`&`,Point`&`,const Image`&`,Rect`&`): [_^Size^ Size]_[* DstSrc]([_^ImageBuffer^ I
mageBuffer][@(0.0.255) `&]_[*@3 dest], [_^Point^ Point][@(0.0.255) `&]_[*@3 p], 
[@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 src], [_^Rect^ Rect][@(0.0.255) `&]_[*@3 s
r])&]
[s2;%RU-RU Вычисляет минимальный Size Rect`'а 
[%-*@3 sr] Image`'а [%-*@3 src], пытаясь скопировать 
его в Point [%-*@3 p] ImageBuffer`'а [%-*@3 dest].&]
[s3;%RU-RU &]
[s4; &]
[s5;:DstSrcOp`(ImageBuffer`&`,Point`,const Image`&`,const Rect`&`,void`(`*`)`(RGBA`*t`,const RGBA`*s`,int n`)`): [@(0.0.255) v
oid]_[* DstSrcOp]([_^ImageBuffer^ ImageBuffer][@(0.0.255) `&]_[*@3 dest], 
[_^Point^ Point]_[*@3 p], [@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 src], 
[@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 srect], [@(0.0.255) void]_(`*[*@3 op])(
RGBA_`*t, [@(0.0.255) const]_RGBA_`*s, [@(0.0.255) int]_n))&]
[s2;%RU-RU Копирует Rect [%-*@3 srect] Image`'а [%-*@3 src] 
в Point [%-*@3 p] ImageBuffer`'а [%-*@3 dest], используя 
функцию копирования [%-*@3 op].&]
[s3;%RU-RU &]
[s4; &]
[s5;:Copy`(ImageBuffer`&`,Point`,const Image`&`,const Rect`&`): [@(0.0.255) void]_[* Copy
]([_^ImageBuffer^ ImageBuffer][@(0.0.255) `&]_[*@3 dest], [_^Point^ Point]_[*@3 p], 
[@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 src], [@(0.0.255) const]_[_^Rect^ Rec
t][@(0.0.255) `&]_[*@3 srect])&]
[s2;%RU-RU Копирует Rect [%-*@3 srect] Image`'а [%-*@3 src] 
в Point [%-*@3 p] ImageBuffer`'а [%-*@3 dest].&]
[s3;%RU-RU &]
[s4; &]
[s5;:Fill`(ImageBuffer`&`,const Rect`&`,RGBA`): [@(0.0.255) void]_[* Fill]([_^ImageBuffer^ I
mageBuffer][@(0.0.255) `&]_[*@3 dest], [@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 r
ect], [_^RGBA^ RGBA]_[*@3 color])&]
[s2;%RU-RU Заполняет прямоугольную область 
[%-*@3 rect] цветом [%-*@3 color] .&]
[s3;%RU-RU &]
[s4; &]
[s5;:Copy`(Image`&`,Point`,const Image`&`,const Rect`&`): [@(0.0.255) void]_[* Copy]([_^Image^ I
mage][@(0.0.255) `&]_[*@3 dest], [_^Point^ Point]_[*@3 p], [@(0.0.255) const]_[_^Image^ Image
][@(0.0.255) `&]_[*@3 src], [@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 srect])&]
[s2;%RU-RU Копирует Rect [%-*@3 srect] Image`'а [%-*@3 src] 
в Point [%-*@3 p] Image`'а [%-*@3 dest].&]
[s3;%RU-RU &]
[s4; &]
[s5;:Over`(ImageBuffer`&`,Point`,const Image`&`,const Rect`&`): [@(0.0.255) void]_[* Over
]([_^ImageBuffer^ ImageBuffer][@(0.0.255) `&]_[*@3 dest], [_^Point^ Point]_[*@3 p], 
[@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 src], [@(0.0.255) const]_[_^Rect^ Rec
t][@(0.0.255) `&]_[*@3 srect])&]
[s2;%RU-RU Смешивает, используя информацию 
об альфе Rect [%-*@3 srect] Image`'а [%-*@3 src] , в Point 
[%-*@3 p] ImageBuffer`'а [%-*@3 dest].&]
[s3;%RU-RU &]
[s4; &]
[s5;:Over`(Image`&`,Point`,const Image`&`,const Rect`&`): [@(0.0.255) void]_[* Over]([_^Image^ I
mage][@(0.0.255) `&]_[*@3 dest], [_^Point^ Point]_[*@3 p], [@(0.0.255) const]_[_^Image^ Image
][@(0.0.255) `&]_[*@3 src], [@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 srect])&]
[s2;%RU-RU Смешивает, используя информацию 
об альфе Rect [%-*@3 srect] Image`'а [%-*@3 src] , в Point 
[%-*@3 p] Image`'а [%-*@3 dest].&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:Over`(Upp`:`:Image`&`,const Upp`:`:Image`&`): [@(0.0.255) void]_[* Over]([_^Upp`:`:Image^ I
mage][@(0.0.255) `&]_[*@3 dest], [@(0.0.255) const]_[_^Upp`:`:Image^ Image][@(0.0.255) `&]_
[*@3 src])&]
[s2;%RU-RU То же, что и Over([%-*@3 dest], Point(0, 0), [%-*@3 src], 
[%-*@3 src].GetSize()).&]
[s3;%RU-RU &]
[s4; &]
[s5;:Fill`(Image`&`,const Rect`&`,RGBA`): [@(0.0.255) void]_[* Fill]([_^Image^ Image][@(0.0.255) `&
]_[*@3 dest], [@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 rect], 
[_^RGBA^ RGBA]_[*@3 color])&]
[s2;%RU-RU Заполняет прямоугольную область 
[%-*@3 rect] цветом [%-*@3 color].&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:Copy`(const Upp`:`:Image`&`,const Upp`:`:Rect`&`): [_^Upp`:`:Image^ Image]_
[* Copy]([@(0.0.255) const]_[_^Upp`:`:Image^ Image][@(0.0.255) `&]_[*@3 src], 
[@(0.0.255) const]_[_^Upp`:`:Rect^ Rect][@(0.0.255) `&]_[*@3 srect])&]
[s2;%RU-RU Возвращает область [%-*@3 srect] [%-*@3 src]`'а 
.&]
[s3;%RU-RU &]
[s4; &]
[s5;:OverStraightOpaque`(ImageBuffer`&`,Point`,const Image`&`,const Rect`&`): [@(0.0.255) v
oid]_[* OverStraightOpaque]([_^ImageBuffer^ ImageBuffer][@(0.0.255) `&]_[*@3 dest], 
[_^Point^ Point]_[*@3 p], [@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 src], 
[@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 srect])&]
[s2;%RU-RU Смешивает, используя информацию 
об альфе Rect [%-*@3 srect] Image`'а [%-*@3 src] , в Point 
[%-*@3 p] ImageBuffer`'а [%-*@3 dest].&]
[s2;%RU-RU Альфа`-канал у [%-*@3 dest] `= 255 (непрозрачный).&]
[s3;%RU-RU &]
[s4; &]
[s5;:OverStraightOpaque`(Image`&`,Point`,const Image`&`,const Rect`&`): [@(0.0.255) voi
d]_[* OverStraightOpaque]([_^Image^ Image][@(0.0.255) `&]_[*@3 dest], 
[_^Point^ Point]_[*@3 p], [@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 `_src], 
[@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 srect])&]
[s2;%RU-RU Смешивает, используя информацию 
об альфе Rect [%-*@3 srect] Image`'а [%-*@3 src] , в Point 
[%-*@3 p] Image`'а [%-*@3 dest].&]
[s2;%RU-RU Альфа`-канал у [%-*@3 dest] `= 255 (непрозрачный).&]
[s3;%RU-RU &]
[s4; &]
[s5;:Crop`(const Image`&`,const Rect`&`): [_^Image^ Image]_[* Crop]([@(0.0.255) const]_[_^Image^ I
mage][@(0.0.255) `&]_[*@3 img], [@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 rc])&]
[s2;%RU-RU Возвращает субимидж из [%-*@3 img], 
определённый Rect`'ом [%-*@3 rc].&]
[s3;%RU-RU &]
[s4; &]
[s5;:Crop`(const Image`&`,int`,int`,int`,int`): [_^Image^ Image]_[* Crop]([@(0.0.255) const
]_[_^Image^ Image][@(0.0.255) `&]_[*@3 img], [@(0.0.255) int]_[*@3 x], 
[@(0.0.255) int]_[*@3 y], [@(0.0.255) int]_[*@3 cx], [@(0.0.255) int]_[*@3 cy])&]
[s2;%RU-RU Возвращает субимидж из [%-*@3 img], 
определённый исходной [%-*@3 x,] [%-*@3 y], шириной 
[%-*@3 cx] и высотой [%-*@3 cy].&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:FindBounds`(const Upp`:`:Image`&`,Upp`:`:RGBA`): [_^Upp`:`:Rect^ Rect]_[* Fin
dBounds]([@(0.0.255) const]_[_^Upp`:`:Image^ Image][@(0.0.255) `&]_[*@3 m], 
[_^Upp`:`:RGBA^ RGBA]_[*@3 bg]_`=_RGBAZero())&]
[s2;%RU-RU Детектирует прямоугольную кромку 
с униформным цветом [%-*@3 bg] и возвращает 
этот прямоугольник с рисунком, пиксели 
которого находятся внутри этой кромки.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:AutoCrop`(const Upp`:`:Image`&`,Upp`:`:RGBA`): [_^Upp`:`:Image^ Image]_[* Aut
oCrop]([@(0.0.255) const]_[_^Upp`:`:Image^ Image][@(0.0.255) `&]_[*@3 m], 
[_^Upp`:`:RGBA^ RGBA]_[*@3 bg]_`=_RGBAZero())&]
[s2;%RU-RU Детектирует прямоугольную кромку 
с униформным цветом [%-*@3 bg] and then crops 
this margin out.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:AutoCrop`(Upp`:`:Image`*`,int`,Upp`:`:RGBA`): [@(0.0.255) void]_[* AutoCrop](
[_^Upp`:`:Image^ Image]_`*[*@3 m], [@(0.0.255) int]_[*@3 count], [_^Upp`:`:RGBA^ RGBA]_[*@3 b
g])&]
[s2;%RU-RU Находит максимальную прямоугольную 
кромку с униформным цветом [%-*@3 bg] у 
всех рисунков и затем crops this margin out. 
У всех рисунков должен быть одинаковый 
размер до и после операции.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:ClampHotSpots`(Upp`:`:Image`&`): [@(0.0.255) void]_[* ClampHotSpots]([_^Upp`:`:Image^ I
mage][@(0.0.255) `&]_[*@3 m])&]
[s2;%RU-RU Clamps позиции горячих точек, так 
что они получаются внутри [%-*@3 m].&]
[s3;%RU-RU &]
[s4; &]
[s5;:ColorMask`(const Image`&`,Color`): [_^Image^ Image]_[* ColorMask]([@(0.0.255) const]_[_^Image^ I
mage][@(0.0.255) `&]_[*@3 src], [_^Color^ Color]_[*@3 transparent])&]
[s2;%RU-RU Возвращает новый Image, на основе 
[%-*@3 src], заменённый Color [%-*@3 transparent] с RGBAZero().&]
[s3;%RU-RU &]
[s4; &]
[s5;:CanvasSize`(const Image`&`,int`,int`): [_^Image^ Image]_[* CanvasSize]([@(0.0.255) con
st]_[_^Image^ Image][@(0.0.255) `&]_[*@3 img], [@(0.0.255) int]_[*@3 cx], 
[@(0.0.255) int]_[*@3 cy])&]
[s2;%RU-RU  [%-*@3 img] [%-*@3 cx] [%-*@3 cy].&]
[s3;%RU-RU &]
[s4; &]
[s5;:AssignAlpha`(const Image`&`,const Image`&`): [_^Image^ Image]_[* AssignAlpha]([@(0.0.255) c
onst]_[_^Image^ Image][@(0.0.255) `&]_[*@3 img], [@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&
]_[*@3 new`_alpha])&]
[s2;%RU-RU Возвращает новый Image, на основе 
цветов [%-*@3 img] и альфе [%-*@3 new`_alpha].&]
[s2;%RU-RU Размер Image равен минимуму [%-*@3 img] 
и [%-*@3 new`_alpha].&]
[s3;%RU-RU &]
[s4; &]
[s5;:Equalight`(const Image`&`,int`): [_^Image^ Image]_[* Equalight]([@(0.0.255) const]_[_^Image^ I
mage][@(0.0.255) `&]_[*@3 img], [@(0.0.255) int]_[*@3 thold]_`=_[@3 10])&]
[s2;%RU-RU  [%-*@3 img] [%-*@3 thold].&]
[s3;%RU-RU &]
[s4; &]
[s5;:Grayscale`(const Image`&`): [_^Image^ Image]_[* Grayscale]([@(0.0.255) const]_[_^Image^ I
mage][@(0.0.255) `&]_[*@3 img])&]
[s2;%RU-RU Возвращает новый Image, включающий 
в себя [%-*@3 img],  преобразованный в grayscale.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Grayscale`(const Image`&`,int`): [_^Image^ Image]_[* Grayscale]([@(0.0.255) const]_[_^Image^ I
mage][@(0.0.255) `&]_[*@3 img], [@(0.0.255) int]_[*@3 amount])&]
[s2;%RU-RU  [%-*@3 img] [%-*@3 amount].&]
[s3;%RU-RU &]
[s4; &]
[s5;:Colorize`(const Image`&`,Color`,int`): [_^Image^ Image]_[* Colorize]([@(0.0.255) const
]_[_^Image^ Image][@(0.0.255) `&]_[*@3 img], [_^Color^ Color]_[*@3 color], 
[@(0.0.255) int]_[*@3 alpha]_`=_[@3 100])&]
[s2;%RU-RU  [%-*@3 img] [%-*@3 color] [%-*@3 alpha].&]
[s3;%RU-RU &]
[s4; &]
[s5;:Contrast`(const Image`&`,int`): [_^Image^ Image]_[* Contrast]([@(0.0.255) const]_[_^Image^ I
mage][@(0.0.255) `&]_[*@3 img], [@(0.0.255) int]_[*@3 amount]_`=_[@3 256])&]
[s2;%RU-RU Возвращает новый Image, у которого 
констраст увеличен до [%-*@3 amount], на основе 
[%-*@3 img].&]
[s3;%RU-RU &]
[s4; &]
[s5;:Rescale`(const Image`&`,Size`,const Rect`&`,Gate2`<int`,int`>`): [_^Image^ Image]_
[* Rescale]([@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 src], 
[_^Size^ Size]_[*@3 sz], [@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 src`_rc], 
[_^Gate2^ Gate2]<[@(0.0.255) int], [@(0.0.255) int]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s2; [%RU-RU Возвращает новый Image, ремасштабированный
 до Size`'а ][*@3 sz][%RU-RU , основанный на Rect`'е 
][*@3 src`_rc ][%RU-RU Image`'а ][*@3 src]&]
[s3;%RU-RU &]
[s4; &]
[s5;:Sharpen`(const Image`&`,int`): [_^Image^ Image]_[* Sharpen]([@(0.0.255) const]_[_^Image^ I
mage][@(0.0.255) `&]_[*@3 img], [@(0.0.255) int]_[*@3 amount]_`=_[@3 100])&]
[s2;%RU-RU Делает image резче, используя примитивный 
фильтр конволюции.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Dither`(const Image`&`,int`): [_^Image^ Image]_[* Dither]([@(0.0.255) const]_[_^Image^ I
mage][@(0.0.255) `&]_[*@3 m], [@(0.0.255) int]_[*@3 dival]_`=_[@3 394])&]
[s2;%RU-RU Предоставляет примитивный dithering 
с матрицей 8x8  в катинку black/white. Dival 
`- своего рода контроль гаммы вывода, 
понижающий её от дефолтого значения 
(до, напр., 350). Иожет даваьб лучший 
результат, когда dithering используется 
для печати на чисто монохромных (юнз 
поддкржки полутонов) принтерах.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:GaussianBlur`(const Upp`:`:Image`&`,int`,bool`): [_^Upp`:`:Image^ Image]_[* G
aussianBlur]([@(0.0.255) const]_[_^Upp`:`:Image^ Image][@(0.0.255) `&]_[*@3 img], 
[@(0.0.255) int]_[*@3 radius], [@(0.0.255) bool]_[*@3 co]_`=_[@(0.0.255) false])&]
[s2;%RU-RU [%-@N An implementation of well known fast box and gaussian 
blur approximation algorithms by Wojciech Jarosz and Peter Kovesi.] 
[%-*@3 co] активирует параллельную обработку.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Etched`(const Image`&`): [_^Image^ Image]_[* Etched]([@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&
]_[*@3 img])&]
[s2;%RU-RU `"Зудящий`" (`"Etching`") эффект.&]
[s3;%RU-RU &]
[s4; &]
[s5;:SetColorKeepAlpha`(const Image`&`,Color`): [_^Image^ Image]_[* SetColorKeepAlpha]([@(0.0.255) c
onst]_[_^Image^ Image][@(0.0.255) `&]_[*@3 img], [_^Color^ Color]_[*@3 c])&]
[s2;%RU-RU Возвращает новый Image, на основе 
альфы [%-*@3 img] и цвета [%-*@3 c] Color.&]
[s3;%RU-RU &]
[s4; &]
[s5;:RotateClockwise`(const Image`&`): [_^Image^ Image]_[* RotateClockwise]([@(0.0.255) con
st]_[_^Image^ Image][@(0.0.255) `&]_[*@3 img])&]
[s2;%RU-RU Возвращает новый Image, повёрнутый 
на 90º (по часовой стрелке), основанный 
на [%-*@3 img].&]
[s3;%RU-RU &]
[s4; &]
[s5;:RotateAntiClockwise`(const Image`&`): [_^Image^ Image]_[* RotateAntiClockwise]([@(0.0.255) c
onst]_[_^Image^ Image][@(0.0.255) `&]_[*@3 img])&]
[s2;%RU-RU Возвращает новый Image, повёрнутый 
на `-90º (против часовой стрелки), основанный 
на [%-*@3 img].&]
[s3;%RU-RU &]
[s4; &]
[s5;:Rotate180`(const Image`&`): [_^Image^ Image]_[* Rotate180]([@(0.0.255) const]_[_^Image^ I
mage][@(0.0.255) `&]_[*@3 orig])&]
[s2;%RU-RU Возвращает новый Image, повёрнутый 
на 180º, основанный на [%-*@3 img].&]
[s3;%RU-RU &]
[s4; &]
[s5;:MirrorHorz`(const Image`&`): [_^Image^ Image]_[* MirrorHorz]([@(0.0.255) const]_[_^Image^ I
mage][@(0.0.255) `&]_[*@3 img])&]
[s2;%RU-RU Возвращает новый Image, зеркалящий 
[%-*@3 img, ][%- следующий по горизонтальной 
оси].&]
[s3;%RU-RU &]
[s4; &]
[s5;:MirrorVert`(const Image`&`): [_^Image^ Image]_[* MirrorVert]([@(0.0.255) const]_[_^Image^ I
mage][@(0.0.255) `&]_[*@3 img])&]
[s2;%RU-RU Возвращает новый Image, зеркалящий 
[%-*@3 img, ][%- следующий по вертикальной 
оси].&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Rotate`(const Image`&`,int`): [_^Image^ Image]_[* Rotate]([@(0.0.255) const]_[_^Image^ I
mage][@(0.0.255) `&]_[*@3 m], [@(0.0.255) int]_[*@3 angle])&]
[s2;%RU-RU Возвращает новый Image, на основе 
Image`'а [%-*@3 m], повёрнутый на угол [%-*@3 angle] 
от своего центра, где полное вращение 
(2[%- π) равно] 3600.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:Transpose`(const Upp`:`:Image`&`): [_^Upp`:`:Image^ Image]_[* Transpose]([@(0.0.255) c
onst]_[_^Upp`:`:Image^ Image][@(0.0.255) `&]_[*@3 img])&]
[s2;%RU-RU Итог такой же, как у MirrorHorz(RotateClockwise([%-*@3 img]))
, но реализуется эффективно за один 
шаг.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:Transverse`(const Upp`:`:Image`&`): [_^Upp`:`:Image^ Image]_[* Transverse]([@(0.0.255) c
onst]_[_^Upp`:`:Image^ Image][@(0.0.255) `&]_[*@3 img])&]
[s2;%RU-RU Итог такой же, как у MirrorHorz(RotateAntiClockwise([%-*@3 im
g])), но реализуется эффективно за один 
шаг.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:FlipImage`(const Upp`:`:Image`&`,int`): [_^Upp`:`:Image^ Image]_[* FlipImage](
[@(0.0.255) const]_[_^Upp`:`:Image^ Image][@(0.0.255) `&]_[*@3 m], [@(0.0.255) int]_[*@3 mode
])&]
[s2;%RU-RU Трансформирует Image, основываясь 
на режиме flip [%-*@3 mode], который может 
быть одним из: FLIP`_NONE, FLIP`_MIRROR`_HORZ, FLIP`_ROTATE`_180, 
FLIP`_MIRROR`_VERT, FLIP`_TRANSPOSE, FLIP`_ROTATE`_CLOCKWISE, 
FLIP`_TRANSVERSE, FLIP`_ROTATE`_ANTICLOCKWISE. Значения 
режима Flip совместимы с Raster`::GetOrientation 
и равны EXIF orientation `- 1. Эта функция предназначе
на для поворота Image в корректном направлении
 (обычно JPEG с цифровой камеры).&]
[s2;%RU-RU &]
[s0;%RU-RU ]]