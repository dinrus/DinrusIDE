topic "Классы Image и ImageBuffer";
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
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Класс Image]]}}&]
[s3; &]
[s1;:Image`:`:class: [*@(0.0.255) class][* _Image_:_][*@(0.0.255) public][* _][*@3 AssignValueTy
peNo][* <_Image, ][*@3 150][* , ][*_^Moveable^ Moveable][* <Image>_>_]&]
[s9;%RU-RU Image rпредставляет неизменяемое 
значение рисунка. Image может прямо 
отрисовываться в Draw. Для создания 
или изменения Image, используется [^topic`:`/`/Draw`/src`/ImageBuffer`_ru`-ru`#ImageBuffer`:`:class^ I
mageBuffer].&]
[s9;%RU-RU У Image константное незначительное 
время глубокого копирования. Он типа 
moveable  и совместим с Rich`-Value.&]
[s9;%RU-RU U`+`+ ожидает пиксели в преумноженном 
альфа формате (premultiplied alpha).&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Публичных Членов]]}}&]
[s3; &]
[s5;:Image`:`:operator`~`(`)const: [@(0.0.255) const]_[_^RGBA^ RGBA][@(0.0.255) `*]_operato
r`~()_[@(0.0.255) const]&]
[s5;:Image`:`:operator const RGBA`*`(`)const: operator_const_RGBA`*()_[@(0.0.255) const
]&]
[s2;%RU-RU Возвращает указатель на пиксели 
Image`'а.&]
[s3; &]
[s4; &]
[s5;:Image`:`:operator`[`]`(int`)const: [@(0.0.255) const]_[_^RGBA^ RGBA][@(0.0.255) `*]_op
erator`[`]([@(0.0.255) int]_[@3 i])_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает указатель на первый 
пиксель в строке [%-*@3 i].&]
[s3; &]
[s4; &]
[s5;:Image`:`:GetSize`(`)const: [_^topic`:`/`/Core`/src`/Size`$en`-us^ Size]_GetSize()_
[@(0.0.255) const]&]
[s2;%RU-RU Возвращает размер Image`'а.&]
[s3; &]
[s4; &]
[s5;:Image`:`:GetWidth`(`)const: [@(0.0.255) int]_GetWidth()_[@(0.0.255) const]&]
[s2;%RU-RU То же, что и GetSize().cx.&]
[s3; &]
[s4; &]
[s5;:Image`:`:GetHeight`(`)const: [@(0.0.255) int]_GetHeight()_[@(0.0.255) const]&]
[s2;%RU-RU То же, что и GetSize().cy.&]
[s3; &]
[s4; &]
[s5;:Image`:`:GetLength`(`)const: [@(0.0.255) int]_GetLength()_[@(0.0.255) const]&]
[s2;%RU-RU Число пикселей в Image`'е `- GetWidth() 
`* GetHeight().&]
[s3; &]
[s4; &]
[s5;:Image`:`:GetHotSpot`(`)const: [_^topic`:`/`/Core`/src`/Point`$en`-us^ Point]_GetHo
tSpot()[@(64) _][@(0.0.255) const]&]
[s2;%RU-RU Возвращает точку ссылки.&]
[s3; &]
[s4; &]
[s5;:Image`:`:Get2ndSpot`(`)const: [_^topic`:`/`/Core`/src`/Point`$en`-us^ Point]_Get2n
dSpot()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает вторичную точку ссылки.&]
[s3; &]
[s4; &]
[s5;:Image`:`:GetDots`(`)const: [_^topic`:`/`/Core`/src`/Size`$en`-us^ Size]_GetDots()[@(64) _
][@(0.0.255) const]&]
[s2;%RU-RU Выдаёт физический размер Image`'а. 
Если физический размер не установлен, 
возвращает Size(0, 0).&]
[s3; &]
[s4; &]
[s5;:Image`:`:GetDPI`(`): [_^Upp`:`:Size^ Size]_[* GetDPI]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает разрешение изображения 
в dpi (dots per inch, точки на дюйм).&]
[s3; &]
[s4; &]
[s5;:Image`:`:GetKindNoScan`(`)const: [@(0.0.255) int]_GetKindNoScan()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает вид рисунка. В отличие 
от GetKind(), не выполняет сканирования 
рисунка, если вид неизвестен, следовательно,
 может вернуть IMAGE`_UNKNOWN.&]
[s3; &]
[s4; &]
[s5;:Image`:`:GetKind`(`)const: [@(0.0.255) int]_GetKind()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает вид рисунка, либо установленн
ый ImageBuffer`::SetKind, либо (если это IMAGE`_UNKNOWN) 
разрешённый путём сканирования пикселей. 
Если вид IMAGE`_UNKNOWN, сканируются пиксели 
и определяется тип, затем он сохраняется 
(в приватной переменной экземпляра 
класса Image) для дальнейшего использования, 
и возвращается.&]
[s3; &]
[s4; &]
[s5;:Image`:`:Begin`(`)const: [@(0.0.255) const]_[_^RGBA^ RGBA]_`*[* Begin]()_[@(0.0.255) con
st]&]
[s5;:Upp`:`:Image`:`:begin`(`)const: [@(0.0.255) const]_[_^Upp`:`:RGBA^ RGBA]_`*[* begin]()
_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает указатель на первый 
пиксель.&]
[s3; &]
[s4; &]
[s5;:Image`:`:End`(`)const: [@(0.0.255) const]_[_^RGBA^ RGBA]_`*[* End]()_[@(0.0.255) const]&]
[s5;:Upp`:`:Image`:`:end`(`)const: [@(0.0.255) const]_[_^Upp`:`:RGBA^ RGBA]_`*[* end]()_[@(0.0.255) c
onst]&]
[s2;%RU-RU Возвращает begin() `+ GetLength().&]
[s3; &]
[s4; &]
[s5;:Image`:`:GetSerialId`(`)const: [_^int64^ int64]_GetSerialId()[@(64) _][@(0.0.255) cons
t]&]
[s2;%RU-RU Возвращает уникальный, на каждый 
запуск приложения, идентификатор 
Image`'а. Все Image`'и с одинаковым серийным 
идентификатором могут считаться 
одинаковыми (это используется для 
их кэширования).&]
[s3; &]
[s4; &]
[s5;:Image`:`:IsSame`(const Image`&`)const: [@(0.0.255) bool]_IsSame([@(0.0.255) const]_[_^Image^ I
mage][@(0.0.255) `&]_[@3 img])_[@(0.0.255) const]&]
[s2;%RU-RU То же, что и GetSerialId() `=`= img.GetSerialId().&]
[s3; &]
[s4; &]
[s5;:Image`:`:operator`=`=`(const Image`&`)const: [@(0.0.255) bool]_operator`=`=([@(0.0.255) c
onst]_[_^Image^ Image][@(0.0.255) `&]_[@3 img])_[@(0.0.255) const]&]
[s5;:Image`:`:operator`!`=`(const Image`&`)const: [@(0.0.255) bool]_operator!`=([@(0.0.255) c
onst]_[_^Image^ Image][@(0.0.255) `&]_[@3 img])_[@(0.0.255) const]&]
[s2;%RU-RU Тестирует два Image`'а на равенство 
(или неравенство). Сравниваются все 
размеры, горячие точки и пиксели. 
Заметьте, что сравнение может быть 
медлительным.&]
[s3; &]
[s4; &]
[s5;:Image`:`:GetHashValue`(`)const: [_^dword^ dword]_GetHashValue()[@(64) _][@(0.0.255) co
nst]&]
[s2;%RU-RU Возвращает хэш`-значение для изображения
. Все пиксели комбинируются в хэш`-значение 
(потенциально медленно).&]
[s3; &]
[s4; &]
[s5;:Image`:`:ToString`(`)const: [_^topic`:`/`/Core`/src`/String`$en`-us^ String]_ToStr
ing()[@(64) _][@(0.0.255) const]&]
[s2;%RU-RU Возвращает базовую информацию 
об Image как String.&]
[s3; &]
[s4; &]
[s5;:Image`:`:Serialize`(Stream`&`): [@(0.0.255) void]_Serialize([_^Stream^ Stream][@(0.0.255) `&
]_[@3 s])&]
[s2;%RU-RU Сериализует Image.&]
[s3; &]
[s4; &]
[s5;:Image`:`:Clear`(`): [@(0.0.255) void]_Clear()&]
[s2;%RU-RU Присваивает пустой Image.&]
[s3; &]
[s4; &]
[s5;:Image`:`:operator`=`(const Image`&`): [_^Image^ Image][@(0.0.255) `&]_operator`=([@(0.0.255) c
onst]_[_^Image^ Image][@(0.0.255) `&]_[@3 img])&]
[s2;%RU-RU Присваивает другой Image.&]
[s3; &]
[s4; &]
[s5;:Image`:`:operator`=`(ImageBuffer`&`): [_^Image^ Image][@(0.0.255) `&]_operator`=([_^ImageBuffer^ I
mageBuffer][@(0.0.255) `&]_[@3 img])&]
[s2;%RU-RU Присваивает Image, созданный в ImageBuffer`'е. 
ImageBuffer опустошается при этой операции.&]
[s3; &]
[s4; &]
[s5;:Image`:`:IsNullInstance`(`)const: [@(0.0.255) bool]_IsNullInstance()_[@(0.0.255) con
st]&]
[s2;%RU-RU Проверяет, имеется ли у Image`'а ненулевое 
измерение.&]
[s3; &]
[s4; &]
[s5;:Image`:`:IsEmpty`(`)const: [@(0.0.255) bool]_IsEmpty()_[@(0.0.255) const]&]
[s2;%RU-RU То же, что и IsNullInstance.&]
[s3; &]
[s4; &]
[s5;:Image`:`:operator Value`(`)const: operator_Value()[@(64) _][@(0.0.255) const]&]
[s2;%RU-RU Преобразует Image в Value.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Image`:`:IsPaintOnceHint`(`)const: [@(0.0.255) bool]_[* IsPaintOnceHint]()_[@(0.0.255) c
onst]&]
[s2;%RU-RU Сигнализирует о том, что Image предположите
льно отрисовывается только раз посредством 
Draw`::DrawImage.,&]
[s3; &]
[s0; &]
[ {{10000t/25b/25@1 [s0; [* Детали Конструктора]]}}&]
[s3; &]
[s5;:Image`:`:Image`(`): Image()&]
[s3; &]
[s4; &]
[s5;:Image`:`:Image`(const Nuller`&`): Image([@(0.0.255) const]_[_^Nuller^ Nuller][@(0.0.255) `&
])&]
[s2;%RU-RU Конструирует пустой Image.&]
[s3; &]
[s4; &]
[s5;:Image`:`:Image`(const Value`&`): Image([@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&
]_[@3 src])&]
[s2;%RU-RU Преобразует Image из Value.&]
[s3; &]
[s4; &]
[s5;:Image`:`:Image`(const Image`&`): Image([@(0.0.255) const]_Image[@(0.0.255) `&]_[@3 img
])&]
[s2;%RU-RU Копи`-конструктор.&]
[s3; &]
[s4; &]
[s5;:Image`:`:Image`(Image`(`*`)`(`)`): Image(Image_(`*fn)())&]
[s2;%RU-RU Эта функция позволяет прямо конструиров
ать Image из указателя на функцию, возвращающую
 этот Image. Это позволяет не использовать 
фигурные скобки при передаче констант 
изображений Iml в качестве аргументов.&]
[s3; &]
[s4; &]
[s5;:Image`:`:Image`(ImageBuffer`&`): Image([_^ImageBuffer^ ImageBuffer][@(0.0.255) `&]_[@3 b
])&]
[s2;%RU-RU Использует Image, созданный в ImageBuffer`'е. 
ImageBuffer при этой операции опустошается.&]
[s3;%RU-RU &]
[s0;%RU-RU &]
[ {{10000t/25b/25@1 [s0;%RU-RU [* Стандартные курсоры]]}}&]
[s9;%RU-RU Класс Image содержит несколько стандартых 
изображений курсора мыши как статические 
методы`-члены:&]
[s0;%RU-RU &]
[ {{3333:3333:3334f0;g0; [ {{10000>274; [s0; Arrow]
:: [s0; Wait]
:: [s0; IBeam]
:: [s0; No]
:: [s0; SizeAll]}}]
:: [ {{10000>274; [s0; SizeHorz]
:: [s0; SizeVert]
:: [s0; SizeTopLeft]
:: [s0; SizeTop]
:: [s0; SizeTopRight]}}]
:: [ {{10000>274; [s0; SizeLeft]
:: [s0; SizeRight]
:: [s0; SizeBottomLeft]
:: [s0; SizeBottom]
:: [s0; SizeBottomRight]}}]}}&]
[s3; ]]