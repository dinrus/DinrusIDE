topic "Класс ImageBuffer";
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
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Класс ImageBuffer]]}}&]
[s3; &]
[s1;:ImageBuffer`:`:class: [*@(0.0.255) class][* _ImageBuffer_:_][*@(0.0.255) private][* _][*@3 N
oCopy]&]
[s9;%RU-RU ImageBuffer представляет собой записываемый 
Image `- массив из пикселей RGBA. ImageBuffer 
может преобразовываться в Image за low`-const 
константное время, при этом теряя 
свой контент, и наоборот, Image может 
преобразовываться в ImageBuffer, опять 
же, теряя свой контент.&]
[s9;%RU-RU Контент у Image / ImageBuffer можно классифициров
ать, чтобы оптимизировать drawing. Возможные 
классификации&]
[s2;%RU-RU &]
[ {{2913:7087<288;>416; [s0;%RU-RU [* IMAGE`_UNKNOWN]]
:: [s0;%RU-RU Род рисунка неизвестен.]
:: [s0;%RU-RU [* IMAGE`_ALPHA]]
:: [s0;%RU-RU У рисунка есть какие`-то значения 
альфа, не равные 255.]
:: [s0;%RU-RU [* IMAGE`_OPAQUE]]
:: [s0;%RU-RU У рисунка все альфа`-значения 
равны 255.]
:: [s0;%RU-RU [* IMAGE`_EMPTY]]
:: [s0;%RU-RU [@5 `[депрекирован`]]]
:: [s0;%RU-RU [* IMAGE`_MASK]]
:: [s0;%RU-RU [@5 `[депрекирован`]]]}}&]
[s2;%RU-RU &]
[s9;%RU-RU Чтобы оптимизировать производительност
ь отрисовки, советуем установить 
вид в IMAGE`_OPAQUE, если это известный факт.&]
[s9;%RU-RU Пиксели изображения организованы 
в простой протяжённый массив POD, первый 
пиксель `- верхний левый.&]
[s9;%RU-RU U`+`+ ожидает пиксели в преумноженном 
альфа формате (premultiplied alpha).&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Публичных Членов]]}}&]
[s3; &]
[s5;:ImageBuffer`:`:SetKind`(int`): [*@(0.0.255) void][* _SetKind(][*@(0.0.255) int][* _][*@3 k
][* )]&]
[s2;%RU-RU Устанавливает род рисунка. Может 
использоваться для избежания автоматическо
й детекции.&]
[s3; &]
[s4; &]
[s5;:ImageBuffer`:`:GetKind`(`)const: [*@(0.0.255) int][* _GetKind()_][*@(0.0.255) const]&]
[s2;%RU-RU Возвращает род рисунка, установленного 
посредством SetKind.&]
[s3; &]
[s4; &]
[s5;:ImageBuffer`:`:ScanKind`(`)const: [*@(0.0.255) int][* _ScanKind()_][*@(0.0.255) const]&]
[s2;%RU-RU Сканирует все пиксели RGBA, определяя 
род рисунка.&]
[s3; &]
[s4; &]
[s5;:ImageBuffer`:`:GetScanKind`(`)const: [*@(0.0.255) int][* _GetScanKind()_][*@(0.0.255) c
onst]&]
[s2;%RU-RU Если вид, установленный SetKind, иной, 
чем IMAGE`_UNKNOWN, возвращает его, иначе 
вызывает ScanKind().&]
[s3; &]
[s4; &]
[s5;:Upp`:`:ImageBuffer`:`:Opaque`(`): [_^Upp`:`:ImageBuffer^ ImageBuffer][@(0.0.255) `&]_
[* Opaque]()&]
[s2;%RU-RU То же, что и SetKind(IMAGE`_OPAQUE).&]
[s3; &]
[s4; &]
[s5;:ImageBuffer`:`:SetHotSpot`(Point`): [*@(0.0.255) void][* _SetHotSpot(][*_^Point^ Point
][* _][*@3 p][* )]&]
[s2;%RU-RU Устанавливает точку ссылки. Эта 
точка, напр., используется как hot`-spot, 
когда рисунок используется как указатель 
мыши. Дижайнер Iml может определять 
эту точку.&]
[s3; &]
[s4; &]
[s5;:ImageBuffer`:`:GetHotSpot`(`)const: [*_^topic`:`/`/Core`/src`/Point`$en`-us^ Point
][* _GetHotSpot()_][*@(0.0.255) const]&]
[s2;%RU-RU Возвращает точку ссылки.&]
[s3; &]
[s4; &]
[s5;:ImageBuffer`:`:Set2ndSpot`(Point`): [*@(0.0.255) void][* _Set2ndSpot]([_^topic`:`/`/Core`/src`/Point`$en`-us^ P
oint]_[*@3 p])&]
[s2;%RU-RU Устанавливает вторичную точку 
ссылки. Дизайнер Iml может эту точку 
определять.&]
[s3;%RU-RU &]
[s4; &]
[s5;:ImageBuffer`:`:Get2ndSpot`(`)const: [*_^topic`:`/`/Core`/src`/Point`$en`-us^ Point
][* _Get2ndSpot]()_[@(0.0.255) const]&]
[s2;%RU-RU Читает вторичную точку ссылки.&]
[s3; &]
[s4; &]
[s5;:ImageBuffer`:`:SetDots`(Size`): [*@(0.0.255) void][* _SetDots(][*_^Size^ Size][* _][*@3 sz
][* )]&]
[s2;%RU-RU Устанавливает опциональный физический 
размер в точках.&]
[s3; &]
[s4; &]
[s5;:ImageBuffer`:`:GetDots`(`)const: [*_^topic`:`/`/Core`/src`/Size`$en`-us^ Size][* _Ge
tDots()_][*@(0.0.255) const]&]
[s2;%RU-RU Возвращает опциональный физический 
размер. Дефолтное значение равно 
Size(0, 0).&]
[s3; &]
[s4; &]
[s5;:ImageBuffer`:`:SetDPI`(Size`): [@(0.0.255) void]_[* SetDPI]([_^topic`:`/`/Core`/src`/Size`$en`-us^ S
ize]_[*@3 sz])&]
[s2;%RU-RU Устанавливает разрешение рисунка 
в dpi (`"точек на дюйм`" (`'dots per inch`')).&]
[s2;%RU-RU Так как этот dpi непосредственно 
не сохраняется, вызывается SetDots() или 
изменяется размер рисунка, the dpi could 
change.&]
[s3;%RU-RU &]
[s4; &]
[s5;:ImageBuffer`:`:GetDPI`(`): [_^topic`:`/`/Core`/src`/Size`$en`-us^ Size]_[* GetDPI]()
&]
[s2;%RU-RU Возвращает разрешение рисунка 
в dpi (dots per inch).&]
[s2;%RU-RU Так как этот dpi непосредственно 
не сохраняется, вызывается SetDots() или 
изменяется размер рисунка, the dpi could 
change.&]
[s3; &]
[s4; &]
[s5;:ImageBuffer`:`:GetSize`(`)const: [*_^topic`:`/`/Core`/src`/Size`$en`-us^ Size][* _Ge
tSize()_][*@(0.0.255) const]&]
[s2;%RU-RU Возвращает размеры рисунка.&]
[s3; &]
[s4; &]
[s5;:ImageBuffer`:`:GetWidth`(`)const: [*@(0.0.255) int][* _GetWidth()_][*@(0.0.255) const]&]
[s2;%RU-RU То же, что и GetSize().cx.&]
[s3; &]
[s4; &]
[s5;:ImageBuffer`:`:GetHeight`(`)const: [*@(0.0.255) int][* _GetHeight()_][*@(0.0.255) cons
t]&]
[s2;%RU-RU То же, что и GetSize().cy.&]
[s3; &]
[s4; &]
[s5;:ImageBuffer`:`:GetLength`(`)const: [*@(0.0.255) int][* _GetLength()_][*@(0.0.255) cons
t]&]
[s2;%RU-RU Возвращает число пикселей в рисунке. 
То же, что и GetSize().cx `* GetSize().cy.&]
[s3; &]
[s4; &]
[s5;:ImageBuffer`:`:operator`[`]`(int`): [*_^topic`:`/`/Core`/src`/RGBA`$en`-us^ RGBA][* _
`*operator`[`](][*@(0.0.255) int][* _][*@3 i][* )]&]
[s5;:ImageBuffer`:`:operator`[`]`(int`)const: [*@(0.0.255) const][* _][*_^topic`:`/`/Core`/src`/RGBA`$en`-us^ R
GBA][* _`*operator`[`](][*@(0.0.255) int][* _][*@3 i][* )_][*@(0.0.255) const]&]
[s2;%RU-RU Возвращает указатель на первый 
пиксель в строке [%-*@3 i].&]
[s3; &]
[s4; &]
[s5;:ImageBuffer`:`:operator`~`(`): [*_^topic`:`/`/Core`/src`/RGBA`$en`-us^ RGBA][* _`*op
erator`~()]&]
[s5;:ImageBuffer`:`:operator RGBA`*`(`): [* operator_RGBA`*()]&]
[s5;:ImageBuffer`:`:operator`~`(`)const: [*@(0.0.255) const][* _][*_^topic`:`/`/Core`/src`/RGBA`$en`-us^ R
GBA][* _`*operator`~()_][*@(0.0.255) const]&]
[s5;:ImageBuffer`:`:operator const RGBA`*`(`)const: [* operator_const_RGBA`*()_][*@(0.0.255) c
onst]&]
[s2;%RU-RU Возвращает указатель на первый 
пиксель в рисунке.&]
[s3; &]
[s4; &]
[s5;:ImageBuffer`:`:Create`(int`,int`): [*@(0.0.255) void][* _Create(][*@(0.0.255) int][* _][*@3 c
x][* , ][*@(0.0.255) int][* _][*@3 cy][* )]&]
[s5;:ImageBuffer`:`:Create`(Size`): [*@(0.0.255) void][* _Create(][*_^topic`:`/`/Core`/src`/Size`$en`-us^ S
ize][* _][*@3 sz][* )]&]
[s2;%RU-RU Создаёт новый рисунок заданного 
размера. Значение пикселей неопределённое. 
Предыдущий контент ImageBuffer`'а утрачивается.&]
[s3; &]
[s4; &]
[s5;:ImageBuffer`:`:IsEmpty`(`)const: [*@(0.0.255) bool][* _IsEmpty()_][*@(0.0.255) const]&]
[s2;%RU-RU То же, что и GetLength() `=`= 0.&]
[s3; &]
[s4; &]
[s5;:ImageBuffer`:`:Clear`(`): [*@(0.0.255) void][* _Clear()]&]
[s2;%RU-RU Очищает контент, удаляя все пиксели 
и устанавливая размер рисунка на 
Size(0, 0).&]
[s3; &]
[s4; &]
[s5;:Upp`:`:ImageBuffer`:`:PaintOnceHint`(bool`): [@(0.0.255) void]_[* PaintOnceHint]([@(0.0.255) b
ool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s5;:Upp`:`:ImageBuffer`:`:IsPaintOnceHint`(`)const: [@(0.0.255) bool]_[* IsPaintOnceHint
]()_[@(0.0.255) const]&]
[s2;%RU-RU Устанавливает/получаетхинт (hint), 
что изображение, вероятно, будет нарисовано 
только раз посредствомh Draw`::DrawImage. 
Этот хинт к определённых контекстах 
может улучшить производительность 
отображения. Дефолт равен false.&]
[s3; &]
[s4; &]
[s5;:ImageBuffer`:`:operator`=`(Image`&`): [*@(0.0.255) void][* _operator`=(][*_^Image^ Ima
ge][*@(0.0.255) `&][* _][*@3 img][* )]&]
[s5;:ImageBuffer`:`:operator`=`(ImageBuffer`&`): [*@(0.0.255) void][* _operator`=(][*_^ImageBuffer^ I
mageBuffer][*@(0.0.255) `&][* _][*@3 img][* )]&]
[s2;%RU-RU Присваивает пиксели [%-*@3 img] ImageBuffer`'у. 
[%-*@3 img] зачищается и пуст после этой 
операции.&]
[s3; &]
[s0; &]
[ {{10000t/25b/25@1 [s0; [* Детали Конструктора]]}}&]
[s3; &]
[s5;:ImageBuffer`:`:ImageBuffer`(`): [* ImageBuffer]()&]
[s2;%RU-RU Конструирует пустой ImageBuffer.&]
[s3; &]
[s4; &]
[s5;:ImageBuffer`:`:ImageBuffer`(int`,int`): [* ImageBuffer(][*@(0.0.255) int][* _][*@3 cx][* ,
 ][*@(0.0.255) int][* _][*@3 cy][* )]&]
[s5;:ImageBuffer`:`:ImageBuffer`(Size`): [* ImageBuffer(][*_^topic`:`/`/Core`/src`/Size`$en`-us^ S
ize][* _][*@3 sz][* )]&]
[s2;%RU-RU Конструирует ImageBuffer заданного 
размера. Значение пикселей неопределённое.&]
[s3; &]
[s4; &]
[s5;:ImageBuffer`:`:ImageBuffer`(Image`&`): [* ImageBuffer]([*_^Image^ Image][@(0.0.255) `&
]_[*@3 img])&]
[s5;:ImageBuffer`:`:ImageBuffer`(ImageBuffer`&`): [* ImageBuffer]([* ImageBuffer][@(0.0.255) `&
]_[*@3 img])&]
[s2;%RU-RU Присваивает пиксели [%-*@3 img] ImageBuffer`'у. 
[%-*@3 img] зачищается и пуст после этой 
операции.&]
[s3; &]
[s0; ]]