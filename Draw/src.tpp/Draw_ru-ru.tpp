topic "Класс Draw";
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
[ {{10000t/25b/25@3 [s0; [*@(229)4 Класс Draw]]}}&]
[s3; &]
[s1;:Draw`:`:class: [@(0.0.255)3 class][3 _][*3 Draw][3 _:_][@(0.0.255)3 private][3 _][*@3;3 NoCopy]&]
[s9;%RU-RU Класс предоставляет абстрактный 
интерфейс к базовому графическому 
выводу, возможно, с ускорением HW. Когда 
нужно что`-либо нарисовать в окне, 
для этого требуется объект Draw. Стандартный 
метод Paint класса Ctrl также принимает 
параметр Draw`&. В этом случае `"низлежащий`" 
(глубиный) механизм передаёт производный 
SystemDraw, который уже инициализирован 
на вывод в соответствующий контрол, 
имеет данные о текущем клиппинге, 
разрешении и проч. Таким же образом 
можно создавать собственные объекты 
Draw, чтобы рисовать на Image`'ах, Drawing`'ах 
(векторных метафайлах, используемых 
для сериализации графических операций) 
или же выполнять вывод на принтер. 
Можно также использовать Draw для запроса 
о свойствах, связанных с графикой, 
напр., о размере области вывода или 
о нативном разрешении.&]
[s9;%RU-RU Когда Draw представляет собой вывод 
на экран, все метрики всегда в пикселях. 
Если Draw представляет какой`-либо физическое 
устройство (типа принтера), то он использует 
искусственную единицу, `"dot`" (`"точку`"), 
по умолчанию, которая эквивалента 
1/600 дюйма. Другими словами, то же самое, 
что и пиксельный размер принтера 
`- 600 dpi (точек на дюйм) (стандартное 
разрешение принтера).&]
[s9;%RU-RU Однако, в некоторых случаях можно 
просто переключиться в нативный режим 
`"Native`". Тогда Draw работает в `"нативных`" 
пикселях, например, 1/720 дюйма для некоторых 
принтеров. Draw предоставляет необходимую 
информацию о метрике, что позволяет 
настроить графический вывод.&]
[s9;%RU-RU У некоторых методов имеется несколько 
перегруженных вариантов, например, 
DrawRect имеет варианты, которые определяют 
прямоугольник посредством типа Rect 
или координат x, y, cx, cy. В этом случае, 
реализация предоставляет абстрактный 
виртуальный бэкэндный метод, с окончанием 
на Op (DrawRectOp), а невиртуальные фронтэндные 
методы затем используются, чтобы 
обеспечить перегрузку (посредством 
трансляции параметров в параметры 
бэкэндного метода).&]
[s9;%RU-RU Рисование внутри размера вывода 
всегда `"легально`" `- в этом случае 
выполняется необходимая обрезка 
(clipping).&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Публичных Членов]]}}&]
[s3; &]
[s5;:Draw`:`:GetInfo`(`)const: [@(0.0.255) virtual] [_^dword^ dword]_[* GetInfo]()_[@(0.0.255) c
onst]_`=_[@3 0]&]
[s2;%RU-RU Возвращает некоторую базовую 
информацию о Draw в форме комбинации 
флагов:&]
[s2;%RU-RU &]
[ {{3735:6265<256;^ [s2;l64;%RU-RU [* Draw`::DOTS_`=_0x001]]
:: [s2;l96;%RU-RU Draw, возможно, представляет собой 
физическое устройство (принтер, вывод 
в PDF и проч.).]
:: [s2;l64;%RU-RU [* Draw`::GUI_`=_0x002]]
:: [s2;l96;%RU-RU Draw инкапсулирует графический 
вывод ГИП главной хостовой платформы 
(напр., GDI на Win32).]
:: [s2;l64;%RU-RU [* Draw`::PRINTER_`=_0x004]]
:: [s2;l96;%RU-RU Draw представляет принтер.]
:: [s2;l64;%RU-RU [* Draw`::NATIVE_`=_0x008]]
:: [s2;l96;%RU-RU Draw в нативном режиме.]
:: [s2;l64; [* Draw`::DATABANDS_`=_0x010]]
:: [s2;l96;%RU-RU Draw может использовать banding для 
ремасштабирования изображений или 
для операции DrawData, чтобы понизить 
потребление памяти.]}}&]
[s3; &]
[s4; &]
[s5;:Draw`:`:GetPageSize`(`)const: [@(0.0.255) virtual] [_^Size^ Size]_[* GetPageSize]()_[@(0.0.255) c
onst]&]
[s2;%RU-RU Возвращает размер действительного 
вывода (размер окна в пикселях, размер 
печатаемой области). Если активен 
нативный режим и имеется иной dpi, значение 
соответственно регулируется.&]
[s3; &]
[s4; &]
[s5;:Draw`:`:StartPage`(`): [@(0.0.255) virtual] [@(0.0.255) void]_[* StartPage]()&]
[s2;%RU-RU При выводе на принтер, начинает 
новую страницу.&]
[s3; &]
[s4; &]
[s5;:Draw`:`:EndPage`(`): [@(0.0.255) virtual] [@(0.0.255) void]_[* EndPage]()&]
[s2;%RU-RU При выводе на принтер, завершает 
страницу.&]
[s3; &]
[s4; &]
[s5;:Draw`:`:BeginOp`(`): [@(0.0.255) virtual] [@(0.0.255) void]_[* BeginOp]()_`=_[@3 0]&]
[s2;%RU-RU Реализует операцию Begin: Вталкивает 
текущие настройки обрезки и смещения 
на внутренний стэк Draw.&]
[s3; &]
[s4; &]
[s5;:Draw`:`:EndOp`(`): [@(0.0.255) virtual] [@(0.0.255) void]_[* EndOp]()_`=_[@3 0]&]
[s2;%RU-RU Реализует операцию End: Вынимает 
со стэка настройки смещения и обрезки, 
восстанавливая статус перед спариванием 
BeginOp, OffsetOp, ClipOp, ExcludeClipOp или IntersectClipOp.&]
[s3; &]
[s4; &]
[s5;:Draw`:`:OffsetOp`(Point`): [@(0.0.255) virtual] [@(0.0.255) void]_[* OffsetOp]([_^Point^ P
oint]_[*@3 p])_`=_[@3 0]&]
[s2;%RU-RU Реализует операцию Offset: Вызывает 
Begin и затем смещает координаты так, 
что [%-*@3 p] становится Point(0, 0).&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:ClipOp`(const Rect`&`): [@(0.0.255) virtual] [@(0.0.255) bool]_[* ClipOp]([@(0.0.255) c
onst]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 r])_`=_[@3 0]&]
[s2;%RU-RU Реализует операцию Clip: Вызывает 
Begin и затем ограничивает все операции 
рисования до [%-*@3 r] `- ничего вне этого 
прямоугольника не отрисовывается.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:ClipoffOp`(const Rect`&`): [@(0.0.255) virtual] [@(0.0.255) bool]_[* ClipoffOp
]([@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 r])_`=_[@3 0]&]
[s2;%RU-RU Реализует операцию ClipOff: Вызывает 
begin, затем ограничивает все операции 
рисования до[%-  ][%-*@3 r] и смещает координаты 
так, что левая точка [%-*@3 r] становится 
Point(0, 0).&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:ExcludeClipOp`(const Rect`&`): [@(0.0.255) virtual] [@(0.0.255) bool]_[* Exclu
deClipOp]([@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 r])_`=_[@3 0]&]
[s2;%RU-RU Реализует операцию ExcludeClip: Исключает 
[%-*@3 r] из вывода `- в этой области ничего 
не отрисовывается. Не влияет на стэк 
смещение/обрезка. Эта операция гарантирован
но работает только в SystemDraw.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:IntersectClipOp`(const Rect`&`): [@(0.0.255) virtual] 
[@(0.0.255) bool]_[* IntersectClipOp]([@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 r
])_`=_[@3 0]&]
[s2;%RU-RU Реализует операцию IntersectClip: Пересекает 
текущую область обрезки с [%-*@3 r]. Не 
влияет на стэк смещение/обрезка. Эта 
операция гарантированно работает 
только в SystemDraw.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:IsPaintingOp`(const Rect`&`)const: [@(0.0.255) virtual] 
[@(0.0.255) bool]_[* IsPaintingOp]([@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 r])_
[@(0.0.255) const]_`=_[@3 0]&]
[s2;%RU-RU Реализует операцию IsPainting: Тестирует, 
должно ли происходить какое`-либо 
рисование в [%-*@3 r]. Например, если Draw 
представляет собой графический вывод 
в окно в Ctrl`::Paint, то не все области нужно 
постоянно перерисовывать.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:GetPaintRect`(`)const: [@(0.0.255) virtual] [_^Rect^ Rect]_[* GetPaintRect]()_
[@(0.0.255) const]&]
[s2;%RU-RU Предоставляет ограничивающий 
прямоугольник для отрисовываемых 
областей, где IsPaintingOp возвращает true. 
Дефолтная реализация возвращает 
тот самый прямоугольник.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:DrawRectOp`(int`,int`,int`,int`,Color`): [@(0.0.255) virtual] 
[@(0.0.255) void]_[* DrawRectOp]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], 
[@(0.0.255) int]_[*@3 cx], [@(0.0.255) int]_[*@3 cy], [_^Color^ Color]_[*@3 color])_`=_[@3 0]&]
[s2;%RU-RU Реализует операцию DrawRect: Заполняет 
прямоугольник [%-*@3 x],[%-*@3 y],[%-*@3 cx],[%-*@3 cy], 
используя цвет [%-*@3 color]. Как специальное 
дополнение, если Draw представляет 
собой вывод на экран в Ctrl`::Paint, цвету 
[%-*@3 color] можно присваивать особое значение 
InvertColor(), вызывающее инверсию всех 
пикселей в целевой области. Заметка: 
На некоторых платформах использование 
InvertColor может приводить к неудовлетворительн
ой производительности.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Draw`:`:SysDrawImageOp`(int`,int`,const Image`&`,Color`): [@(0.0.255) virtual] 
[@(0.0.255) void]_[* SysDrawImageOp]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], 
[@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 img], [_^Color^ Color]_[*@3 color])&]
[s2;%RU-RU Низкоуровневая отрисовка изображения 
класса Image. Зависит от второго варианта 
SysDrawImageOp, чтобы поддерживать параметр 
[%-*@3 src], используя Offset и Clip.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Draw`:`:SysDrawImageOp`(int`,int`,const Image`&`,const Rect`&`,Color`): [@(0.0.255) v
irtual] [@(0.0.255) void]_[* SysDrawImageOp]([@(0.0.255) int]_[*@3 x], 
[@(0.0.255) int]_[*@3 y], [@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 img], 
[@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 src], [_^Color^ Color]_[*@3 color])&]
[s2;%RU-RU (Ещё более) низкоуровневая операция 
отрисовки изо класса Image, вызываемая 
DrawImageOp. В производном от Draw классе 
можно либо переписать (override) этот 
метод и зависить от DrawImageOp в плане 
выполнения ремасштабирования и banding`'а 
принтера, либо прямо переписать DrawImageOp. 
В дефолтной реализации  Offset и Clip используютс
я для поддержки параметра [%-*@3 src].&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:DrawImageOp`(int`,int`,int`,int`,const Image`&`,const Rect`&`,Color`): [@(0.0.255) v
irtual] [@(0.0.255) void]_[* DrawImageOp]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y],
 [@(0.0.255) int]_[*@3 cx], [@(0.0.255) int]_[*@3 cy], [@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&
]_[*@3 img], [@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 src], 
[_^Color^ Color]_[*@3 color])_`=_[@3 0]&]
[s2;%RU-RU Реализует DrawImageOp: Рисует часть 
[%-*@3 src] изо Image [%-*@3 img] по [%-*@3 x], [%-*@3 y], ремасштабируя
 его до [%-*@3 cx], [%-*@3 cy] (если они равны исходному 
размеру Image, то никакого ремасштабирования 
не выполняется). Если [%-*@3 color] не равен 
нулю, тспллдьзуется только информация 
об альфе изображения, а цветные пиксели 
отрисовываются цветом [%-*@3 color]. Дефолтная 
реализация выполняет операцию ремасштабиро
вания на ПО (software), включая поддержку 
banding`'а для принтера, а затем вызывает 
SysDrawImageOp.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:DrawDataOp`(int`,int`,int`,int`,const String`&`,const char`*`): [@(0.0.255) v
irtual] [@(0.0.255) void]_[* DrawDataOp]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], 
[@(0.0.255) int]_[*@3 cx], [@(0.0.255) int]_[*@3 cy], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[*@3 data], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id])&]
[s2;%RU-RU Реализует операцию DrawData. Рисует 
произвольные данные [%-*@3 data] типа [%-*@3 id] 
в прямоугольную область [%-*@3 x],[%-*@3 y],[%-*@3 cx],[%-*@3 cy]. 
Может использоваться, например, для 
прямой печати сжатых битмапов (напр., 
в формате png), без необходимости их 
загрузки в Image, возможно, это сохраняет 
память, т.к. banding может исползоваться 
для отправки изображения на принтер 
по частям. Действительный процесс 
отрисовки реализован в [^DataDrawer^ DataDrawer], 
а [%-*@3 id] нужно зарегистрировать посредством 
DataDrawer`::Register.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:DrawLineOp`(int`,int`,int`,int`,int`,Color`): [@(0.0.255) virtual] 
[@(0.0.255) void]_[* DrawLineOp]([@(0.0.255) int]_[*@3 x1], [@(0.0.255) int]_[*@3 y1], 
[@(0.0.255) int]_[*@3 x2], [@(0.0.255) int]_[*@3 y2], [@(0.0.255) int]_[*@3 width], 
[_^Color^ Color]_[*@3 color])_`=_[@3 0]&]
[s2;%RU-RU Реализация операции DrawLine. Рисует 
линию от [%-*@3 x1],[%-*@3 y1] до [%-*@3 x2],[%-*@3 y2] (включительно
), с пиксельной шириной [%-*@3 width], с цветом 
[%-*@3 color]. Ширина может содержать особые 
значения, в случае чего рисуется линия 
с шириной 1 и применяется специальный 
пунктирный паттерн:&]
[s2;%RU-RU &]
[ {{3035:6965<288; [s2;l64;%RU-RU [* PEN`_NULL]]
:: [s2;l64;%RU-RU пустое перо (ничего не отрисовывается)
]
:: [s2;l64;%RU-RU [* PEN`_SOLID]]
:: [s2;l64;%RU-RU плотное перо]
:: [s2;l64;%RU-RU [* PEN`_DASH]]
:: [s2;l64;%RU-RU пунктирное перо  `_ `_ `_]
:: [s2;l64;%RU-RU [* PEN`_DOT]]
:: [s2;l64;%RU-RU точечное перо (пунктиры короче) 
[* . . .]]
:: [s2;l64;%RU-RU [* PEN`_DASHDOT]]
:: [s2;l64;%RU-RU паттерн тире `- точка `- тире 
`- точка `_.`_.`_.]
:: [s2;l64;%RU-RU [* PEN`_DASHDOTDOT]]
:: [s2;l64;%RU-RU паттерн тире `- точка `- точка 
`- тире `- точка `- точка `_..`_..`_..]}}&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:DrawPolyPolylineOp`(const Point`*`,int`,const int`*`,int`,int`,Color`,Color`): [@(0.0.255) v
irtual] [@(0.0.255) void]_[* DrawPolyPolylineOp]([@(0.0.255) const]_[_^Point^ Point]_`*[*@3 v
ertices], [@(0.0.255) int]_[*@3 vertex`_count], [@(0.0.255) const]_[@(0.0.255) int]_`*[*@3 c
ounts], [@(0.0.255) int]_[*@3 count`_count], [@(0.0.255) int]_[*@3 width], 
[_^Color^ Color]_[*@3 color], [_^Color^ Color]_[*@3 doxor])_`=_[@3 0]&]
[s2;%RU-RU Реализует операцию DrawPolyPolyline: Рисует 
ряд полилиний. Вершины полилиний 
хранятся в массиве [%-*@3 vertices]. Параметр 
[%-*@3 vertex`_count] даёт общее число вершин 
всех полилиний в этом массиве. Массив 
[%-*@3 counts ]даёт номера точек, определяющих 
отдельные полилинии, а [%-*@3 count`_count ]даёт 
число элементов в этом массиве (т.е. 
число соединённых полилиний). Первая 
полилиния состоит из вершин [%-*@3 vertices]`[0`], 
[%-*@3 vertices]`[1`] ... [%-*@3 vertices]`[[%-*@3 counts]`[0`] `- 
1`], вторая полилиния [%-*@3 vertices]`[[%-*@3 counts]`[0`]`], 
[%-*@3 vertices]`[[%-*@3 counts]`[0`] `+ 1`] ... [%-*@3 vertices]`[[%-*@3 counts]`[0`] 
`+ [%-*@3 counts]`[1`] `- 1`], и т.д. Параметр width 
может быть равен одинаковым особым 
значениям [* PEN`_]`*, таким как width в DrawLineOp. 
Линия рисуется цветом [%-*@3 color], если 
[%-*@3 doxor] не равен нулю, то такой цвет 
в фоне инвертируется.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:DrawPolyPolyPolygonOp`(const Point`*`,int`,const int`*`,int`,const int`*`,int`,Color`,int`,Color`,uint64`,Color`): [@(0.0.255) v
irtual] [@(0.0.255) void]_[* DrawPolyPolyPolygonOp]([@(0.0.255) const]_[_^Point^ Point]_`*
[*@3 vertices], [@(0.0.255) int]_[*@3 vertex`_count], [@(0.0.255) const]_[@(0.0.255) int]_`*
[*@3 subpolygon`_counts], [@(0.0.255) int]_[*@3 scc], [@(0.0.255) const]_[@(0.0.255) int]_`*
[*@3 disjunct`_polygon`_counts], [@(0.0.255) int]_[*@3 dpcc], [_^Color^ Color]_[*@3 color],
 [@(0.0.255) int]_[*@3 width], [_^Color^ Color]_[*@3 outline], [_^uint64^ uint64]_[*@3 patter
n], [_^Color^ Color]_[*@3 doxor])_`=_[@3 0]&]
[s2;%RU-RU Реализует операцию DrawPolyPolyPolygon: 
Рисует серию сложных многоугольников. 
Массив вершин [%-*@3 vertices] содержит все 
вершины, определяющие многоугольник. 
Этот массив разделён на секции, соответствую
щие всем сложным многоугольникам 
(параметры [%-*@3 disjunct`_polygon`_counts]), и далее 
эти секции делятся на отдельные многоугольн
ики, определяя один сложный многоугольник 
(т.е. внешнюю границу и впадины). Число 
вершин в отдельных многоугольниках 
содержится в массиве [%-*@3 subpolygon`_counts 
](общее число простых многоугольников 
`= subpolygon`_count`_count).&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:DrawArcOp`(const Rect`&`,Point`,Point`,int`,Color`): [@(0.0.255) virtual] 
[@(0.0.255) void]_[* DrawArcOp]([@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 rc], 
[_^Point^ Point]_[*@3 start], [_^Point^ Point]_[*@3 end], [@(0.0.255) int]_[*@3 width], 
[_^Color^ Color]_[*@3 color])_`=_[@3 0]&]
[s2;%RU-RU Реализует операцию DrawArc: Рисует 
эллиптическую арку, соответствующую 
наибольшему эллипсу, входящему в 
прямоугольник [%-*@3 rc], и направленному 
против часовой стрелки от направления, 
соотвествующего линии, соединяющей 
центр эллипса ([%-*@3 rc].CenterPoint()) с точкой 
[%-*@3 start], и заканчивающемуся в направлении 
точки [%-*@3 end ]от центра этого эллипса. 
Когда start `=`= end, рисуется весь эллипс. 
[%-*@3 width] `- это ширина линии, нарисованной 
цветом [%-*@3 color].&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:DrawEllipseOp`(const Rect`&`,Color`,int`,Color`): [@(0.0.255) virtual] 
[@(0.0.255) void]_[* DrawEllipseOp]([@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 r],
 [_^Color^ Color]_[*@3 color], [@(0.0.255) int]_[*@3 pen], [_^Color^ Color]_[*@3 pencolor])_`=
_[@3 0]&]
[s2;%RU-RU Реализует операцию DrawEllipse: Чертит 
наибольший эллипс, у которого обе 
оси параллельны осям координат, вписанный 
в прямоугольник [%-*@3 r], т.е. с центральной 
точкой в r.CenterPoint(), semi major ось и semi minor 
ось равны r.Width() / 2 и r.Height() / 2. Если [%-*@3 pen] 
не равен 0, то у эллипса будет бордюрная 
линия шириной [%-*@3 pen] и цветом [%-*@3 pencolor].&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:DrawTextOp`(int`,int`,int`,const wchar`*`,Font`,Color`,int`,const int`*`): [@(0.0.255) v
irtual] [@(0.0.255) void]_[* DrawTextOp]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], 
[@(0.0.255) int]_[*@3 angle], [@(0.0.255) const]_[_^wchar^ wchar]_`*[*@3 text], 
[_^Font^ Font]_[*@3 font], [_^Color^ Color]_[*@3 ink], [@(0.0.255) int]_[*@3 n], 
[@(0.0.255) const]_[@(0.0.255) int]_`*[*@3 dx])_`=_[@3 0]&]
[s2;%RU-RU Реализует операцию DrawText:Чертит 
[%-*@3 text ][%- в позиции, начинающейся с] 
[%-*@3 x], [%-*@3 y] (представляющей верхний 
левый угол ячейки текста: для горизонтальног
о текста, x идентифицирует точку ссылки, 
а y поднимается над основанием), идущий 
под углом [%-*@3 angle], испльзующий шрифт 
[%-*@3 font] и цвет [%-*@3 ink]. [%-*@3 n] `- число отрисовываемы
х символов. Если [%-*@3 dx ]не равен NULL, 
то определяет advancements отдельных символов 
(в этом массиве должно быть [%-*@3 n] элементов), 
иначе используются advancements из метрики 
шрифта.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:DrawDrawingOp`(const Rect`&`,const Drawing`&`): [@(0.0.255) virtual] 
[@(0.0.255) void]_[* DrawDrawingOp]([@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 tar
get], [@(0.0.255) const]_[_^Drawing^ Drawing][@(0.0.255) `&]_[*@3 w])&]
[s2;%RU-RU Реализует операцию DrawDrawing: Чертит 
[^Drawing^ Drawing] [%-*@3 w], масштабированный до 
[%-*@3 target]. Базовый Draw предоставляет 
дефолтную реализацию этой функции 
(фактически, вероятнее всего, она 
будет невиртуальной).&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:DrawPaintingOp`(const Rect`&`,const Painting`&`): [@(0.0.255) virtual] 
[@(0.0.255) void]_[* DrawPaintingOp]([@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 ta
rget], [@(0.0.255) const]_[_^Painting^ Painting][@(0.0.255) `&]_[*@3 w])&]
[s2;%RU-RU Реализует операцию DrawPainting: Чертит 
[^Drawing^ Drawing] [%-*@3 w], масштабированный до 
[%-*@3 target]. Базовый Draw предоставляет 
дефолтную реализацию, которая рисует 
Painting, используя DrawImage, возможно, используя 
технику banding`'а для уменьшения потребляемой 
памяти. Пакет Painter должен быть включен 
в проект для этой работы.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:GetNativeDpi`(`)const: [@(0.0.255) virtual] [_^Size^ Size]_[* GetNativeDpi]()_
[@(0.0.255) const]&]
[s2;%RU-RU Возвращает нативное разрешение 
целевого устройства. Дефолтная реализация 
возвращает Size(600, 600) для устройства 
Dots и Size(96, 96) для вывода на экран.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:BeginNative`(`): [@(0.0.255) virtual] [@(0.0.255) void]_[* BeginNative]()&]
[s2;%RU-RU Запускает режим нативного разрешения, 
если он доступен. Может вызываться 
несколько раз (срабатывает только 
первый вызов). Если у устройства нет 
нативного режима, не срабатывает 
вообще.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:EndNative`(`): [@(0.0.255) virtual] [@(0.0.255) void]_[* EndNative]()&]
[s2;%RU-RU Завершает режим нативного разрешения. 
Если BeginNative вызывался несколько раз, 
должен быть вызван такое же количество 
раз, чтобы завершить нативный режим.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:GetCloffLevel`(`)const: [@(0.0.255) virtual] [@(0.0.255) int]_[* GetCloffLevel
]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает число элементов в 
стэке clip`&offset (обрезки и смещения). 
Используется, главным образом, для 
диагностических целей.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:Draw`:`:Escape`(const Upp`:`:String`&`): [@(0.0.255) virtual 
void]_[* Escape]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 data])&]
[s2;%RU-RU Передаёт дополнительную информацию 
целевому экземпляру Draw. Например, 
PdfDraw понимает эскапады `"url:http://link.html`", 
чтобы определять текстовые гиперссылки. 
Фронтэнд к EscapeOp.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:GetPixelsPerInch`(`)const: [_^Size^ Size]_[* GetPixelsPerInch]()_[@(0.0.255) c
onst]&]
[s2;%RU-RU Возвращает активное разрешение. 
Возвращает нативное разрешение, если 
активен нативный режим, или Size(600, 
600) для физического вывода, или Size(96, 
96) для вывода на экран.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:GetPageMMs`(`)const: [_^Size^ Size]_[* GetPageMMs]()_[@(0.0.255) const]&]
[s2;%RU-RU Пересчитывает GetPageSize в миллиметры.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:Dots`(`)const: [@(0.0.255) bool]_[* Dots]()_[@(0.0.255) const]&]
[s2;%RU-RU То же, что и GetInfo() `& DOTS. True для физического
 устройства.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:Pixels`(`)const: [@(0.0.255) bool]_[* Pixels]()_[@(0.0.255) const]&]
[s2;%RU-RU То же, что и !Dots(). True для вывода 
на экран.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:IsPrinter`(`)const: [@(0.0.255) bool]_[* IsPrinter]()_[@(0.0.255) const]&]
[s2;%RU-RU То же, что и GetInfo() `& PRINTER. True при 
выводе на принтер.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:IsNative`(`)const: [@(0.0.255) bool]_[* IsNative]()_[@(0.0.255) const]&]
[s2;%RU-RU То же, что и GetInfo() `& NATIVE. True, если 
активен режим нативного разрешения.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:GetNativeX`(int`)const: [@(0.0.255) int]_[* GetNativeX]([@(0.0.255) int]_[*@3 x])
_[@(0.0.255) const]&]
[s2;%RU-RU Пересчитывает [%-*@3 x] из dots 600 dpi в 
нативное горизонтальное разрешение.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:GetNativeY`(int`)const: [@(0.0.255) int]_[* GetNativeY]([@(0.0.255) int]_[*@3 y])
_[@(0.0.255) const]&]
[s2;%RU-RU Пересчитывает [%-*@3 y] из dots 600 в нативное 
вертикальное разрешение.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:Native`(int`&`,int`&`)const: [@(0.0.255) void]_[* Native]([@(0.0.255) int`&]_[*@3 x
], [@(0.0.255) int`&]_[*@3 y])_[@(0.0.255) const]&]
[s2;%RU-RU Пересчитывает координаты dots из 
точек 600 dpi в нативное разрешение.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:Native`(Point`&`)const: [@(0.0.255) void]_[* Native]([_^Point^ Point][@(0.0.255) `&
]_[*@3 p])_[@(0.0.255) const]&]
[s2;%RU-RU Пересчитывает точку из dots  600 dpi 
в нативное разрешение.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:Native`(Size`&`)const: [@(0.0.255) void]_[* Native]([_^Size^ Size][@(0.0.255) `&
]_[*@3 sz])_[@(0.0.255) const]&]
[s2;%RU-RU Пересчитывает Size из dots 600 dpi в нативное 
разрешение.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:Native`(Rect`&`)const: [@(0.0.255) void]_[* Native]([_^Rect^ Rect][@(0.0.255) `&
]_[*@3 r])_[@(0.0.255) const]&]
[s2;%RU-RU Пересчитывает Rect из 600 dpi dots в нативное 
разрешение..&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:Begin`(`): [@(0.0.255) void]_[* Begin]()&]
[s2;%RU-RU Вталкивает настройки смещения 
и обрезки на внутренний стэк Draw. Фронтэнд 
к BeginOp.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:End`(`): [@(0.0.255) void]_[* End]()&]
[s2;%RU-RU Вынимает настройки смещения и 
обрезки, восстанавливая статус перед 
спариванием BeginOp, OffsetOp, ClipOp, ExcludeClipOp 
или IntersectClipOp. Фронтэнд к EndOp.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:Offset`(Point`): [@(0.0.255) void]_[* Offset]([_^Point^ Point]_[*@3 p])&]
[s5;:Draw`:`:Offset`(int`,int`): [@(0.0.255) void]_[* Offset]([@(0.0.255) int]_[*@3 x], 
[@(0.0.255) int]_[*@3 y])&]
[s2;%RU-RU Вызывает Begin и затем координаты 
смещения так, что [%-*@3 p] или [%-*@3 x][%- ,][%-*@3 y][%-  
становится] Point(0, 0). Использует OffsetOp 
для реализации. Фронтэнд к OffsetOp.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:Clip`(const Rect`&`): [@(0.0.255) bool]_[* Clip]([@(0.0.255) const]_[_^Rect^ Rec
t][@(0.0.255) `&]_[*@3 r])&]
[s5;:Draw`:`:Clip`(int`,int`,int`,int`): [@(0.0.255) bool]_[* Clip]([@(0.0.255) int]_[*@3 x],
 [@(0.0.255) int]_[*@3 y], [@(0.0.255) int]_[*@3 cx], [@(0.0.255) int]_[*@3 cy])&]
[s2;%RU-RU Вызывает Begin и затем ограничивает 
все операции рисования до [%-*@3 r] или 
прямоугольника [%-*@3 x][%- ,][%-*@3 y][%- ,][%-*@3 cx][%- ,][%-*@3 cy] 
`- ничто не рисуется вне этого прямоугольника
. Фронтэнд к ClipOp.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:Clipoff`(const Rect`&`): [@(0.0.255) bool]_[* Clipoff]([@(0.0.255) const]_[_^Rect^ R
ect][@(0.0.255) `&]_[*@3 r])&]
[s5;:Draw`:`:Clipoff`(int`,int`,int`,int`): [@(0.0.255) bool]_[* Clipoff]([@(0.0.255) int]_
[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) int]_[*@3 cx], [@(0.0.255) int]_[*@3 cy])&]
[s2;%RU-RU Реализует операцию ClipOff: Вызывает 
begin, затем ограничивает все операции 
рисования до[%-  ][%-*@3 r] или прямоугольника 
[%-*@3 x][%- ,][%-*@3 y][%- ,][%-*@3 cx][%- ,][%-*@3 cy], а координаты 
смешения так, что левая точка [%-*@3 r] 
или точка [%-*@3 x][%- ,][%-*@3 y] становится Point(0, 
0). Фронтэнд к ClipoffOp.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:ExcludeClip`(const Rect`&`): [@(0.0.255) bool]_[* ExcludeClip]([@(0.0.255) con
st]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 r])&]
[s5;:Draw`:`:ExcludeClip`(int`,int`,int`,int`): [@(0.0.255) bool]_[* ExcludeClip]([@(0.0.255) i
nt]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) int]_[*@3 cx], [@(0.0.255) int]_[*@3 cy])&]
[s2;%RU-RU Исключает [%-*@3 r] или прямоугольник 
[%-*@3 x],[%-*@3 y],[%-*@3 cx],[%-*@3 cy] из вывода `- ничто 
не рисуется в этой области. Не влияет 
на стэк смещение/обрезка. Эта операция 
гарантированно работает только в 
SystemDraw. Фронтэнд к ExcludeClipOp.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:IntersectClip`(const Rect`&`): [@(0.0.255) bool]_[* IntersectClip]([@(0.0.255) c
onst]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 r])&]
[s5;:Draw`:`:IntersectClip`(int`,int`,int`,int`): [@(0.0.255) bool]_[* IntersectClip]([@(0.0.255) i
nt]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) int]_[*@3 cx], [@(0.0.255) int]_[*@3 cy])&]
[s2;%RU-RU Пересекает область обрезки с 
[%-*@3 r] или с прямоугольником [%-*@3 x],[%-*@3 y],[%-*@3 cx],[%-*@3 cy
]. Не влияет на стэк смещение/обрезка. 
Эта операция гарантированно работает 
только в SystemDraw. Фронтэнд к IntersectClipOp.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:IsPainting`(const Rect`&`)const: [@(0.0.255) bool]_[* IsPainting]([@(0.0.255) c
onst]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 r])_[@(0.0.255) const]&]
[s5;:Draw`:`:IsPainting`(int`,int`,int`,int`)const:%RU-RU [%-@(0.0.255) bool][%- _][%-* IsP
ainting][%- (][%-@(0.0.255) int][%- _][%-*@3 x][%- , ][%-@(0.0.255) int][%- _][%-*@3 y][%- , 
][%-@(0.0.255) int][%- _][%-*@3 cx][%- , ][%-@(0.0.255) int][%- _][%-*@3 cy][%- )_][%-@(0.0.255) co
nst] .&]
[s2;%RU-RU Тестирует, должно ли произойти 
какое`-либо рисование в [%-*@3 r] или прямоугольни
ке [%-*@3 x],[%-*@3 y],[%-*@3 cx],[%-*@3 cy]. Например, если 
Draw представляет собой графический 
вывод в окно в Ctrl`::Paint, не все области 
требуется перерисовывать. Фронтэнд 
к IsPaintingOp.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:DrawRect`(int`,int`,int`,int`,Color`): [@(0.0.255) void]_[* DrawRect]([@(0.0.255) i
nt]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) int]_[*@3 cx], [@(0.0.255) int]_[*@3 cy], 
[_^Color^ Color]_[*@3 color])&]
[s5;:Draw`:`:DrawRect`(const Rect`&`,Color`): [@(0.0.255) void]_[* DrawRect]([@(0.0.255) co
nst]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 rect], [_^Color^ Color]_[*@3 color])&]
[s2;%RU-RU Заполняет [%-*@3 rect] или прямоугольник 
[%-*@3 x], [%-*@3 y], [%-*@3 cx], [%-*@3 cy], используя цвет 
[%-*@3 color]. Как специальное дополнение, 
если Draw представляет вывод на экран 
в Ctrl`::Paint, то цвету [%-*@3 color] может быть 
присвоено специальное значение InvertColor(), 
вызывающее инверсию всех пикселей 
в целевой области. Фронтэнд к DrawRectOp. 
[/ Заметка:] [/ На некоторых платформах, 
применение InvertColor может вызывать 
неудовлетворительную производительность.]&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:DrawImage`(int`,int`,int`,int`,const Image`&`,const Rect`&`): [@(0.0.255) v
oid]_[* DrawImage]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], 
[@(0.0.255) int]_[*@3 cx], [@(0.0.255) int]_[*@3 cy], [@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&
]_[*@3 img], [@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 src])&]
[s5;:Draw`:`:DrawImage`(int`,int`,int`,int`,const Image`&`): [@(0.0.255) void]_[* DrawIma
ge]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) int]_[*@3 cx], 
[@(0.0.255) int]_[*@3 cy], [@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 img])&]
[s5;:Draw`:`:DrawImage`(int`,int`,int`,int`,const Image`&`,const Rect`&`,Color`): [@(0.0.255) v
oid]_[* DrawImage]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], 
[@(0.0.255) int]_[*@3 cx], [@(0.0.255) int]_[*@3 cy], [@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&
]_[*@3 img], [@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 src], 
[_^Color^ Color]_[*@3 color])&]
[s5;:Draw`:`:DrawImage`(int`,int`,int`,int`,const Image`&`,Color`): [@(0.0.255) void]_[* D
rawImage]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) int]_[*@3 cx], 
[@(0.0.255) int]_[*@3 cy], [@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 img], 
[_^Color^ Color]_[*@3 color])&]
[s5;:Draw`:`:DrawImage`(const Rect`&`,const Image`&`,const Rect`&`): [@(0.0.255) void]_
[* DrawImage]([@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 r], 
[@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 img], [@(0.0.255) const]_[_^Rect^ Rec
t][@(0.0.255) `&]_[*@3 src])&]
[s5;:Draw`:`:DrawImage`(const Rect`&`,const Image`&`): [@(0.0.255) void]_[* DrawImage]([@(0.0.255) c
onst]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 r], [@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&
]_[*@3 img])&]
[s5;:Draw`:`:DrawImage`(const Rect`&`,const Image`&`,const Rect`&`,Color`): [@(0.0.255) v
oid]_[* DrawImage]([@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 r], 
[@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 img], [@(0.0.255) const]_[_^Rect^ Rec
t][@(0.0.255) `&]_[*@3 src], [_^Color^ Color]_[*@3 color])&]
[s5;:Draw`:`:DrawImage`(const Rect`&`,const Image`&`,Color`): [@(0.0.255) void]_[* DrawIm
age]([@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 r], [@(0.0.255) const]_[_^Image^ I
mage][@(0.0.255) `&]_[*@3 img], [_^Color^ Color]_[*@3 color])&]
[s5;:Draw`:`:DrawImage`(int`,int`,const Image`&`,const Rect`&`): [@(0.0.255) void]_[* Dra
wImage]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) const]_[_^Image^ Imag
e][@(0.0.255) `&]_[*@3 img], [@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 src])&]
[s5;:Draw`:`:DrawImage`(int`,int`,const Image`&`): [@(0.0.255) void]_[* DrawImage]([@(0.0.255) i
nt]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 i
mg])&]
[s5;:Draw`:`:DrawImage`(int`,int`,const Image`&`,const Rect`&`,Color`): [@(0.0.255) voi
d]_[* DrawImage]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) const]_[_^Image^ I
mage][@(0.0.255) `&]_[*@3 img], [@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 src], 
[_^Color^ Color]_[*@3 color])&]
[s5;:Draw`:`:DrawImage`(int`,int`,const Image`&`,Color`): [@(0.0.255) void]_[* DrawImage](
[@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&
]_[*@3 img], [_^Color^ Color]_[*@3 color])&]
[s2;%RU-RU Рисует [^Image^ Image] [%-*@3 img]. Если присутствует 
целевой прямоугольник ([%-*@3 r] или [%-*@3 x,y,cx,cy]), 
то Image ремасштабируется (uс использованием 
высококачественного фильтра) , чтобы 
уместить его в область (в жтом процессе 
модет использоваться banding, чтобы понизить 
потребление памяти).Если предоставлена 
только целевая точка [%-*@3 x],[%-*@3 y], то 
Image не ремасштабируется. Если предоставлен 
прямоугольник [%-*@3 src], то используется 
только эта порция Image`'а. Если предоставлен 
цвет [%-*@3 color], используется только информация 
об альфе и информация о цвете заменяется 
ею. Фронтэнд к DrawImageOp.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:DrawData`(int`,int`,int`,int`,const String`&`,const char`*`): [@(0.0.255) v
oid]_[* DrawData]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) int]_[*@3 cx],
 [@(0.0.255) int]_[*@3 cy], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 data], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 type])&]
[s5;:Draw`:`:DrawData`(const Rect`&`,const String`&`,const char`*`): [@(0.0.255) void]_
[* DrawData]([@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 r], 
[@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 data], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 type])&]
[s2;%RU-RU Чертит произвольные данные [%-*@3 data] 
типа [%-*@3 id] в прямоугольной области 
[%-*@3 r] или [%-*@3 x],[%-*@3 y],[%-*@3 cx],[%-*@3 cy]. Может 
использоваться, например, для прямого 
вывода сжатых битмапов (напр., в формате 
png), без нужды загружать их в Image, что, 
вероятно, экономит память, т.к. banding 
можно использовать для отправки изображения
 на принтер по частям. Действительный 
процесс отрисовки реализован в [^DataDrawer^ DataDrawer],
 а [%-*@3 id] нужно зарегистрировать с помощью 
DataDrawer`::Register. Фронтэнд к DrawDataOp.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:DrawLine`(int`,int`,int`,int`,int`,Color`): [@(0.0.255) void]_[* DrawLine]([@(0.0.255) i
nt]_[*@3 x1], [@(0.0.255) int]_[*@3 y1], [@(0.0.255) int]_[*@3 x2], [@(0.0.255) int]_[*@3 y2], 
[@(0.0.255) int]_[*@3 width]_`=_[@3 0], [_^Color^ Color]_[*@3 color]_`=_DefaultInk)&]
[s5;:Draw`:`:DrawLine`(Point`,Point`,int`,Color`): [@(0.0.255) void]_[* DrawLine]([_^Point^ P
oint]_[*@3 p1], [_^Point^ Point]_[*@3 p2], [@(0.0.255) int]_[*@3 width]_`=_[@3 0], 
[_^Color^ Color]_[*@3 color]_`=_DefaultInk)&]
[s2;%RU-RU Чертит линию от [%-*@3 x1],[%-*@3 y1] или 
[%-*@3 p1] до [%-*@3 x2],[%-*@3 y2] или [%-*@3 p2] (включительно) 
Ширина [%-*@3 width] в пикселях, с цветом 
[%-*@3 color]. Ширина может содержать специальные 
значения, в случае чего чертится линия 
шириной 1 и применяется специальный 
пунктирный паттерн `- см. детали в 
DrawLineOp. Фронтэнд к DrawLineOp.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:DrawEllipse`(const Rect`&`,Color`,int`,Color`): [@(0.0.255) void]_[* DrawEll
ipse]([@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 r], [_^Color^ Color]_[*@3 color]_
`=_DefaultInk, [@(0.0.255) int]_[*@3 pen]_`=_Null, [_^Color^ Color]_[*@3 pencolor]_`=_Def
aultInk)&]
[s5;:Draw`:`:DrawEllipse`(int`,int`,int`,int`,Color`,int`,Color`): [@(0.0.255) void]_[* D
rawEllipse]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) int]_[*@3 cx], 
[@(0.0.255) int]_[*@3 cy], [_^Color^ Color]_[*@3 color]_`=_DefaultInk, 
[@(0.0.255) int]_[*@3 pen]_`=_Null, [_^Color^ Color]_[*@3 pencolor]_`=_DefaultInk)&]
[s2;%RU-RU Чертит наибольший эллипс, у которого 
обе оси параллельны осям координат, 
вписанный в прямоугольник [%-*@3 r] ([%-*@3 x],[%-*@3 y],[%-*@3 cx
],[%-*@3 cy]), т.е. с центральной точкой в 
r.CenterPoint(), semi major ось и semi minor ось равны 
r.Width() / 2 и r.Height() / 2. Если [%-*@3 pen] не равен 
0, Ellipse will be have border line of width [%-*@3 pen] and color 
[%-*@3 pencolor]. Фронтэнд к DrawEllipseOp.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:DrawArc`(const Rect`&`,Point`,Point`,int`,Color`): [@(0.0.255) void]_[* Draw
Arc]([@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 rc], [_^Point^ Point]_[*@3 start],
 [_^Point^ Point]_[*@3 end], [@(0.0.255) int]_[*@3 width]_`=_[@3 0], [_^Color^ Color]_[*@3 colo
r]_`=_DefaultInk)&]
[s2;%RU-RU Чертит elliptic arc corresponding to the largest 
ellipse fully within the rectangle [%-*@3 rc] and running counterclockwise 
from the direction corresponding to the line connecting the centre 
of the ellipse ([%-*@3 rc].CenterPoint()) with the point [%-*@3 start 
]and ending at direction of the point [%-*@3 end ]from the ellipse 
centre. When start `=`= end, the full ellipse is drawn. [%-*@3 width] 
is the width of line, painted in [%-*@3 color]. Фронтэнд 
к DrawArcOp.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:DrawPolyPolyline`(const Point`*`,int`,const int`*`,int`,int`,Color`,Color`): [@(0.0.255) v
oid]_[* DrawPolyPolyline]([@(0.0.255) const]_[_^Point^ Point]_`*[*@3 vertices], 
[@(0.0.255) int]_[*@3 vertex`_count], [@(0.0.255) const]_[@(0.0.255) int]_`*[*@3 counts], 
[@(0.0.255) int]_[*@3 count`_count], [@(0.0.255) int]_[*@3 width]_`=_[@3 0], 
[_^Color^ Color]_[*@3 color]_`=_DefaultInk, [_^Color^ Color]_[*@3 doxor]_`=_Null)&]
[s5;:Draw`:`:DrawPolyPolyline`(const Vector`<Point`>`&`,const Vector`<int`>`&`,int`,Color`,Color`): [@(0.0.255) v
oid]_[* DrawPolyPolyline]([@(0.0.255) const]_[_^Vector^ Vector]<[_^Point^ Point]>`&_[*@3 ve
rtices], [@(0.0.255) const]_[_^Vector^ Vector]<[@(0.0.255) int]>`&_[*@3 counts], 
[@(0.0.255) int]_[*@3 width]_`=_[@3 0], [_^Color^ Color]_[*@3 color]_`=_DefaultInk, 
[_^Color^ Color]_[*@3 doxor]_`=_Null)&]
[s2;%RU-RU Чертит несколько полилайнов (многоуголь
ник без заполнения).&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:DrawPolyline`(const Point`*`,int`,int`,Color`,Color`): [@(0.0.255) void]_[* D
rawPolyline]([@(0.0.255) const]_[_^Point^ Point]_`*[*@3 vertices], 
[@(0.0.255) int]_[*@3 count], [@(0.0.255) int]_[*@3 width]_`=_[@3 0], [_^Color^ Color]_[*@3 col
or]_`=_DefaultInk, [_^Color^ Color]_[*@3 doxor]_`=_Null)&]
[s5;:Draw`:`:DrawPolyline`(const Vector`<Point`>`&`,int`,Color`,Color`): [@(0.0.255) vo
id]_[* DrawPolyline]([@(0.0.255) const]_[_^Vector^ Vector]<[_^Point^ Point]>`&_[*@3 vertice
s], [@(0.0.255) int]_[*@3 width]_`=_[@3 0], [_^Color^ Color]_[*@3 color]_`=_DefaultInk, 
[_^Color^ Color]_[*@3 doxor]_`=_Null)&]
[s2;%RU-RU Чертит полилайн (многоугольник 
без заполнения).&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:DrawPolyPolyPolygon`(const Point`*`,int`,const int`*`,int`,const int`*`,int`,Color`,int`,Color`,uint64`,Color`): [@(0.0.255) v
oid]_[* DrawPolyPolyPolygon]([@(0.0.255) const]_[_^Point^ Point]_`*[*@3 vertices], 
[@(0.0.255) int]_[*@3 vertex`_count], [@(0.0.255) const]_[@(0.0.255) int]_`*[*@3 subpolygon
`_counts], [@(0.0.255) int]_[*@3 subpolygon`_count`_count], [@(0.0.255) const]_[@(0.0.255) i
nt]_`*[*@3 disjunct`_polygon`_counts], [@(0.0.255) int]_[*@3 disjunct`_polygon`_count`_
count], [_^Color^ Color]_[*@3 color]_`=_DefaultInk, [@(0.0.255) int]_[*@3 width]_`=_[@3 0],
 [_^Color^ Color]_[*@3 outline]_`=_Null, [_^uint64^ uint64]_[*@3 pattern]_`=_[@3 0], 
[_^Color^ Color]_[*@3 doxor]_`=_Null)&]
[s5;:Draw`:`:DrawPolyPolyPolygon`(const Vector`<Point`>`&`,const Vector`<int`>`&`,const Vector`<int`>`&`,Color`,int`,Color`,uint64`,Color`): [@(0.0.255) v
oid]_[* DrawPolyPolyPolygon]([@(0.0.255) const]_[_^Vector^ Vector]<[_^Point^ Point]>`&_[*@3 v
ertices], [@(0.0.255) const]_[_^Vector^ Vector]<[@(0.0.255) int]>`&_[*@3 subpolygon`_coun
ts], [@(0.0.255) const]_[_^Vector^ Vector]<[@(0.0.255) int]>`&_[*@3 disjunct`_polygon`_co
unts], [_^Color^ Color]_[*@3 color]_`=_DefaultInk, [@(0.0.255) int]_[*@3 width]_`=_[@3 0], 
[_^Color^ Color]_[*@3 outline]_`=_Null, [_^uint64^ uint64]_[*@3 pattern]_`=_[@3 0], 
[_^Color^ Color]_[*@3 doxor]_`=_Null)&]
[s2; Чертит poly`-poly`-polygon.&]
[s3; &]
[s4; &]
[s5;:Draw`:`:DrawPolyPolygon`(const Point`*`,int`,const int`*`,int`,Color`,int`,Color`,uint64`,Color`): [@(0.0.255) v
oid]_[* DrawPolyPolygon]([@(0.0.255) const]_[_^Point^ Point]_`*[*@3 vertices], 
[@(0.0.255) int]_[*@3 vertex`_count], [@(0.0.255) const]_[@(0.0.255) int]_`*[*@3 subpolygon
`_counts], [@(0.0.255) int]_[*@3 subpolygon`_count`_count], [_^Color^ Color]_[*@3 color]_
`=_DefaultInk, [@(0.0.255) int]_[*@3 width]_`=_[@3 0], [_^Color^ Color]_[*@3 outline]_`=_Nu
ll, [_^uint64^ uint64]_[*@3 pattern]_`=_[@3 0], [_^Color^ Color]_[*@3 doxor]_`=_Null)&]
[s5;:Draw`:`:DrawPolyPolygon`(const Vector`<Point`>`&`,const Vector`<int`>`&`,Color`,int`,Color`,uint64`,Color`): [@(0.0.255) v
oid]_[* DrawPolyPolygon]([@(0.0.255) const]_[_^Vector^ Vector]<[_^Point^ Point]>`&_[*@3 ver
tices], [@(0.0.255) const]_[_^Vector^ Vector]<[@(0.0.255) int]>`&_[*@3 subpolygon`_counts
], [_^Color^ Color]_[*@3 color]_`=_DefaultInk, [@(0.0.255) int]_[*@3 width]_`=_[@3 0], 
[_^Color^ Color]_[*@3 outline]_`=_Null, [_^uint64^ uint64]_[*@3 pattern]_`=_[@3 0], 
[_^Color^ Color]_[*@3 doxor]_`=_Null)&]
[s2;%RU-RU Чертит poly`-polygon.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:DrawPolygons`(const Point`*`,int`,const int`*`,int`,Color`,int`,Color`,uint64`,Color`): [@(0.0.255) v
oid]_[* DrawPolygons]([@(0.0.255) const]_[_^Point^ Point]_`*[*@3 vertices], 
[@(0.0.255) int]_[*@3 vertex`_count], [@(0.0.255) const]_[@(0.0.255) int]_`*[*@3 polygon`_c
ounts], [@(0.0.255) int]_[*@3 polygon`_count`_count], [_^Color^ Color]_[*@3 color]_`=_Def
aultInk, [@(0.0.255) int]_[*@3 width]_`=_[@3 0], [_^Color^ Color]_[*@3 outline]_`=_Null, 
[_^uint64^ uint64]_[*@3 pattern]_`=_[@3 0], [_^Color^ Color]_[*@3 doxor]_`=_Null)&]
[s5;:Draw`:`:DrawPolygons`(const Vector`<Point`>`&`,const Vector`<int`>`&`,Color`,int`,Color`,uint64`,Color`): [@(0.0.255) v
oid]_[* DrawPolygons]([@(0.0.255) const]_[_^Vector^ Vector]<[_^Point^ Point]>`&_[*@3 vertic
es], [@(0.0.255) const]_[_^Vector^ Vector]<[@(0.0.255) int]>`&_[*@3 polygon`_counts], 
[_^Color^ Color]_[*@3 color]_`=_DefaultInk, [@(0.0.255) int]_[*@3 width]_`=_[@3 0], 
[_^Color^ Color]_[*@3 outline]_`=_Null, [_^uint64^ uint64]_[*@3 pattern]_`=_[@3 0], 
[_^Color^ Color]_[*@3 doxor]_`=_Null)&]
[s2;%RU-RU Чертит несколько многоугольников.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:DrawPolygon`(const Point`*`,int`,Color`,int`,Color`,uint64`,Color`): [@(0.0.255) v
oid]_[* DrawPolygon]([@(0.0.255) const]_[_^Point^ Point]_`*[*@3 vertices], 
[@(0.0.255) int]_[*@3 vertex`_count], [_^Color^ Color]_[*@3 color]_`=_DefaultInk, 
[@(0.0.255) int]_[*@3 width]_`=_[@3 0], [_^Color^ Color]_[*@3 outline]_`=_Null, 
[_^uint64^ uint64]_[*@3 pattern]_`=_[@3 0], [_^Color^ Color]_[*@3 doxor]_`=_Null)&]
[s5;:Draw`:`:DrawPolygon`(const Vector`<Point`>`&`,Color`,int`,Color`,uint64`,Color`): [@(0.0.255) v
oid]_[* DrawPolygon]([@(0.0.255) const]_[_^Vector^ Vector]<[_^Point^ Point]>`&_[*@3 vertice
s], [_^Color^ Color]_[*@3 color]_`=_DefaultInk, [@(0.0.255) int]_[*@3 width]_`=_[@3 0], 
[_^Color^ Color]_[*@3 outline]_`=_Null, [_^uint64^ uint64]_[*@3 pattern]_`=_[@3 0], 
[_^Color^ Color]_[*@3 doxor]_`=_Null)&]
[s2;%RU-RU Чертит многоугольник.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:DrawDrawing`(const Rect`&`,const Drawing`&`): [@(0.0.255) void]_[* DrawDrawi
ng]([@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 r], [@(0.0.255) const]_[_^Drawing^ D
rawing][@(0.0.255) `&]_[*@3 iw])&]
[s5;:Draw`:`:DrawDrawing`(int`,int`,int`,int`,const Drawing`&`): [@(0.0.255) void]_[* Dra
wDrawing]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) int]_[*@3 cx], 
[@(0.0.255) int]_[*@3 cy], [@(0.0.255) const]_[_^Drawing^ Drawing][@(0.0.255) `&]_[*@3 iw])&]
[s2;%RU-RU Чертит [^Drawing^ Dr][^topic`:`/`/Draw`/src`/Drawing`_ru`-ru`#Drawing`:`:class^ a
][^Drawing^ wing] [%-*@3 iw], масштабированный до 
прямоугольника [%-*@3 r], соответственно 
с [%-*@3 x],[%-*@3 y],[%-*@3 cx],[%-*@3 cy]. Фронтэнд к DrawDrawing 
op.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Draw`:`:DrawDrawing`(int`,int`,const Drawing`&`): [@(0.0.255) void]_[* DrawDrawing](
[@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) const]_[_^Drawing^ Drawing][@(0.0.255) `&
]_[*@3 iw])&]
[s2;%RU-RU Чертит [^topic`:`/`/Draw`/src`/Drawing`_ru`-ru`#Drawing`:`:class^ Draw
ing] [%-*@3 iw] at [%-*@3 x],[%-*@3 y], используя его оригинальны
й размер. Фронтэнд к DrawDrawing op.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:DrawPainting`(int`,int`,const Painting`&`): [@(0.0.255) void]_[* DrawPaintin
g]([@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 r], [@(0.0.255) const]_[_^Painting^ P
ainting][@(0.0.255) `&]_[*@3 iw])&]
[s5;:Draw`:`:DrawPainting`(int`,int`,int`,int`,const Painting`&`): [@(0.0.255) void]_[* D
rawPainting]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) int]_[*@3 cx], 
[@(0.0.255) int]_[*@3 cy], [@(0.0.255) const]_[_^Painting^ Painting][@(0.0.255) `&]_[*@3 iw])
&]
[s5;:Draw`:`:DrawPainting`(int`,int`,const Painting`&`): [@(0.0.255) void]_[* DrawPaintin
g]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) const]_[_^Painting^ Painti
ng][@(0.0.255) `&]_[*@3 iw])&]
[s2;%RU-RU Чертит [^Painting^ Painting] [%-*@3 iw], масштабированный
 до прямоугольник [%-*@3 r], в соответствии 
с [%-*@3 x],[%-*@3 y],[%-*@3 cx],[%-*@3 cy]. Должен быть включен 
в проект пакет Painter. Вариант без [%-*@3 cx],[%-*@3 cy] 
рисует Painting в его оригинальном размере. 
Фронтэнд к DrawPaintingOp.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:DrawText`(int`,int`,int`,const wchar`*`,Font`,Color`,int`,const int`*`): [@(0.0.255) v
oid]_[* DrawText]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) int]_[*@3 ang
le], [@(0.0.255) const]_[_^wchar^ wchar]_`*[*@3 text], [_^Font^ Font]_[*@3 font]_`=_StdFont
(), [_^Color^ Color]_[*@3 ink]_`=_DefaultInk, [@(0.0.255) int]_[*@3 n]_`=_`-[@3 1], 
[@(0.0.255) const]_[@(0.0.255) int]_`*[*@3 dx]_`=_NULL)&]
[s5;:Draw`:`:DrawText`(int`,int`,const wchar`*`,Font`,Color`,int`,const int`*`): [@(0.0.255) v
oid]_[* DrawText]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) const]_[_^wchar^ w
char]_`*[*@3 text], [_^Font^ Font]_[*@3 font]_`=_StdFont(), [_^Color^ Color]_[*@3 ink]_`=_D
efaultInk, [@(0.0.255) int]_[*@3 n]_`=_`-[@3 1], [@(0.0.255) const]_[@(0.0.255) int]_`*[*@3 d
x]_`=_NULL)&]
[s5;:Draw`:`:DrawText`(int`,int`,const WString`&`,Font`,Color`,const int`*`): [@(0.0.255) v
oid]_[* DrawText]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) const]_[_^WString^ W
String][@(0.0.255) `&]_[*@3 text], [_^Font^ Font]_[*@3 font]_`=_StdFont(), 
[_^Color^ Color]_[*@3 ink]_`=_DefaultInk, [@(0.0.255) const]_[@(0.0.255) int]_`*[*@3 dx]_`=
_NULL)&]
[s5;:Draw`:`:DrawText`(int`,int`,int`,const WString`&`,Font`,Color`,const int`*`): [@(0.0.255) v
oid]_[* DrawText]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) int]_[*@3 ang
le], [@(0.0.255) const]_[_^WString^ WString][@(0.0.255) `&]_[*@3 text], 
[_^Font^ Font]_[*@3 font]_`=_StdFont(), [_^Color^ Color]_[*@3 ink]_`=_DefaultInk, 
[@(0.0.255) const]_[@(0.0.255) int]_`*[*@3 dx]_`=_NULL)&]
[s5;:Draw`:`:DrawText`(int`,int`,int`,const char`*`,byte`,Font`,Color`,int`,const int`*`): [@(0.0.255) v
oid]_[* DrawText]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) int]_[*@3 ang
le], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 text], [_^byte^ byte]_[*@3 charset], 
[_^Font^ Font]_[*@3 font]_`=_StdFont(), [_^Color^ Color]_[*@3 ink]_`=_DefaultInk, 
[@(0.0.255) int]_[*@3 n]_`=_`-[@3 1], [@(0.0.255) const]_[@(0.0.255) int]_`*[*@3 dx]_`=_NULL)
&]
[s5;:Draw`:`:DrawText`(int`,int`,const char`*`,byte`,Font`,Color`,int`,const int`*`): [@(0.0.255) v
oid]_[* DrawText]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 text], [_^byte^ byte]_[*@3 charset], [_^Font^ Font]_[*@3 font]_`=_StdFont(), 
[_^Color^ Color]_[*@3 ink]_`=_DefaultInk, [@(0.0.255) int]_[*@3 n]_`=_`-[@3 1], 
[@(0.0.255) const]_[@(0.0.255) int]_`*[*@3 dx]_`=_NULL)&]
[s5;:Draw`:`:DrawText`(int`,int`,int`,const char`*`,Font`,Color`,int`,const int`*`): [@(0.0.255) v
oid]_[* DrawText]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) int]_[*@3 ang
le], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 text], [_^Font^ Font]_[*@3 font]_`=_StdFon
t(), [_^Color^ Color]_[*@3 ink]_`=_DefaultInk, [@(0.0.255) int]_[*@3 n]_`=_`-[@3 1], 
[@(0.0.255) const]_[@(0.0.255) int]_`*[*@3 dx]_`=_NULL)&]
[s5;:Draw`:`:DrawText`(int`,int`,const char`*`,Font`,Color`,int`,const int`*`): [@(0.0.255) v
oid]_[* DrawText]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 text], [_^Font^ Font]_[*@3 font]_`=_StdFont(), [_^Color^ Color]_[*@3 ink]_`=_De
faultInk, [@(0.0.255) int]_[*@3 n]_`=_`-[@3 1], [@(0.0.255) const]_[@(0.0.255) int]_`*[*@3 dx
]_`=_NULL)&]
[s5;:Draw`:`:DrawText`(int`,int`,const String`&`,Font`,Color`,const int`*`): [@(0.0.255) v
oid]_[* DrawText]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 text], [_^Font^ Font]_[*@3 font]_`=_StdFont(), 
[_^Color^ Color]_[*@3 ink]_`=_DefaultInk, [@(0.0.255) const]_[@(0.0.255) int]_`*[*@3 dx]_`=
_NULL)&]
[s5;:Draw`:`:DrawText`(int`,int`,int`,const String`&`,Font`,Color`,const int`*`): [@(0.0.255) v
oid]_[* DrawText]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) int]_[*@3 ang
le], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 text], 
[_^Font^ Font]_[*@3 font]_`=_StdFont(), [_^Color^ Color]_[*@3 ink]_`=_DefaultInk, 
[@(0.0.255) const]_[@(0.0.255) int]_`*[*@3 dx]_`=_NULL)&]
[s2;%RU-RU Чертит текст [%-*@3 text ]в позиции, 
начинающейся с [%-*@3 x],[%-*@3 y] (представляет 
собой верхний левый угол ячейки текста: 
для горизонтального текста, x идентифицирует
 точку ссылки, а y `- подъём над основанием 
(baseline)) под углом [%-*@3 angle], со шрифтом 
[%-*@3 font] и цветом [%-*@3 ink]. [%-*@3 n] `- число отрисовывае
мых символов, если отрицательное, 
оно получается посредством `[w`]strlen. 
Если [%-*@3 dx ]не равно NULL, то определяет 
advancements отдельных символов (в этом 
массиве должно быть [%-*@3 n] элементов), 
иначе используются advancements из метрик 
шрифта. Для 8`-битных текстов ([%-*@3 text] 
`- [^topic`:`/`/Core`/src`/String`_ru`-ru`#String`:`:class^ String] 
или [@(0.0.255) const char `*]), если присутствет 
[%-*@3 charset], текст преобраовывается из 
этого набсима перед печатью, если 
[%-*@3 charset] не указан, используется дефолтный 
набор символов. DrawText aвыполняет такде 
замену отсутствующих глифов и компонировани
е. Если Draw`::GetInfo() возвращает DRAWTEXTLINES, 
DrawText использует DrawLine, чтобы обеспечить 
поддержку стилей шрифта Undeline и Strikeout. 
DrawText вызывает DrawTextOp для низкоуровнего 
отображения глифов.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Draw`:`:SinCos`(int`,double`&`,double`&`): [@(0.0.255) static] 
[@(0.0.255) void]_[* SinCos]([@(0.0.255) int]_[*@3 angle], [@(0.0.255) double`&]_[*@3 sina], 
[@(0.0.255) double`&]_[*@3 cosa])&]
[s2;%RU-RU Эта простая утилитная функция 
вычисляет sin(angle) и cos(angle), где полный 
угол angle (2[%- π) равен] 3600.&]
[s3; &]
[s0; ]]