topic "Класс RectTracker";
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
[ {{10000t/25b/25@3 [s0; [*@(229)4 Класс RectTracker]]}}&]
[s3; &]
[s1;:RectTracker`:`:class: [@(0.0.255)3 class][3 _][*3 RectTracker][3 _:_][@(0.0.255)3 public][3 _
][*@3;3 LocalLoop]&]
[s9;%RU-RU RectTracker `- класс, представляющий 
репозиционирующий модальный цикл. 
Он может репозиционировать и/или 
`"перемерять`" прямоугольник или линию 
в единой модальной операции.&]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Публичных Членов]]}}&]
[s3; &]
[s5;:RectTracker`:`:sync: [_^Callback1^ Callback1]<[_^Rect^ Rect]>_[* sync]&]
[s2;%RU-RU Этот обрвыз вызывается всякий 
раз при изменений размеров.&]
[s3;%RU-RU &]
[s4; &]
[s5;:RectTracker`:`:round: [_^Callback1^ Callback1]<Rect[@(0.0.255) `&]>_[* round]&]
[s2;%RU-RU Если присутствует, этот обрвыз 
используется для изменения координат 
прямоугольника, после того как они 
определены по позиции мыши, до иных 
обработок. Может использоваться, 
например,  для реализации операций 
`"snap to grid`". Это свежая альтернатива 
определению округления объекта и 
метода Round.&]
[s3; &]
[s4; &]
[s5;:RectTracker`:`:SetCursorImage`(const Image`&`): [_^RectTracker^ RectTracker][@(0.0.255) `&
]_[* SetCursorImage]([@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 m])&]
[s2;%RU-RU Устанавливает внешний вид курсора 
мыши во время цикла.&]
[s3;%RU-RU &]
[s4; &]
[s5;:RectTracker`:`:MinSize`(Size`): [_^RectTracker^ RectTracker][@(0.0.255) `&]_[* MinSize
]([_^Size^ Size]_[*@3 sz])&]
[s2;%RU-RU Устанавливает минимальный размер 
прямоугольника.&]
[s3;%RU-RU &]
[s4; &]
[s5;:RectTracker`:`:MaxSize`(Size`): [_^RectTracker^ RectTracker][@(0.0.255) `&]_[* MaxSize
]([_^Size^ Size]_[*@3 sz])&]
[s2;%RU-RU Устанавливает максимальный размер 
прямоугольника.&]
[s3;%RU-RU &]
[s4; &]
[s5;:RectTracker`:`:MaxRect`(const Rect`&`): [_^RectTracker^ RectTracker][@(0.0.255) `&]_
[* MaxRect]([@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 mr])&]
[s2;%RU-RU Устанавливает ограничивающий 
прямоугольник, который не может превышать 
итоговый прямоугольник.&]
[s3;%RU-RU &]
[s4; &]
[s5;:RectTracker`:`:Clip`(const Rect`&`): [_^RectTracker^ RectTracker][@(0.0.255) `&]_[* Cl
ip]([@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 c])&]
[s2;%RU-RU Устанавливает прямоугольник 
обрезки.&]
[s3;%RU-RU &]
[s4; &]
[s5;:RectTracker`:`:Width`(int`): [_^RectTracker^ RectTracker][@(0.0.255) `&]_[* Width]([@(0.0.255) i
nt]_[*@3 n])&]
[s2;%RU-RU Устанавливает ширину линии формы 
трэкингового прямоугольника.&]
[s3;%RU-RU &]
[s4; &]
[s5;:RectTracker`:`:SetColor`(Color`): [_^RectTracker^ RectTracker][@(0.0.255) `&]_[* SetCo
lor]([_^Color^ Color]_[*@3 c])&]
[s2;%RU-RU Устанавливает цвет трэкингового 
прямоугольника (если он на белом фоне).&]
[s3;%RU-RU &]
[s4; &]
[s5;:RectTracker`:`:Образец`(int`): [_^RectTracker^ RectTracker][@(0.0.255) `&]_[* О
бразец]([@(0.0.255) int]_[*@3 p])&]
[s2;%RU-RU Устанавливает образец, используемый 
для черчения линий. [%-*@3 p] может быть 
одним из&]
[s2;%RU-RU &]
[ {{5000:5000<288; [s2;l0;%RU-RU DRAWDRAGRECT`_SOLID]
:: [s2;l0;%RU-RU плотная линия]
:: [s2;l0;%RU-RU DRAWDRAGRECT`_NORMAL]
:: [s2;l0;%RU-RU линия из чередующихся единичных 
точек `- дефолт]
:: [s2;l0;%RU-RU DRAWDRAGRECT`_DASHED ]
:: [s2;l0;%RU-RU линия из чередующихся сегментов 
в 4 пикселя]}}&]
[s3;%RU-RU &]
[s4; &]
[s5;:RectTracker`:`:Dashed`(`): [_^RectTracker^ RectTracker][@(0.0.255) `&]_[* Dashed]()&]
[s2;%RU-RU То же, что и Образец(DRAWDRAGRECT`_DASH).&]
[s3;%RU-RU &]
[s4; &]
[s5;:RectTracker`:`:Solid`(`): [_^RectTracker^ RectTracker][@(0.0.255) `&]_[* Solid]()&]
[s2;%RU-RU То же, что и Образец(DRAWDRAGRECT`_SOLID).&]
[s3;%RU-RU &]
[s4; &]
[s5;:RectTracker`:`:Normal`(`): [_^RectTracker^ RectTracker][@(0.0.255) `&]_[* Normal]()&]
[s2;%RU-RU То же, что и Образец(DRAWDRAGRECT`_NORMAL). 
Это дефолт.&]
[s3; &]
[s4; &]
[s5;:RectTracker`:`:Animation`(int`): [_^RectTracker^ RectTracker][@(0.0.255) `&]_[* Animat
ion]([@(0.0.255) int]_[*@3 step`_ms]_`=_[@3 40])&]
[s2;%RU-RU Активирует анимацию образца.&]
[s3;%RU-RU &]
[s4; &]
[s5;:RectTracker`:`:KeepRatio`(bool`): [_^RectTracker^ RectTracker][@(0.0.255) `&]_[* KeepR
atio]([@(0.0.255) bool]_[*@3 b])&]
[s2;%RU-RU Заставляет прямоугольник сохранять 
исходные пропорции.&]
[s3;%RU-RU &]
[s4; &]
[s5;:RectTracker`:`:Round`(RectTracker`:`:Rounder`&`): [_^RectTracker^ RectTracker][@(0.0.255) `&
]_[* Round]([_^RectTracker`:`:Rounder^ Rounder][@(0.0.255) `&]_[*@3 r])&]
[s2;%RU-RU Устанавливает округляемый объект. 
Устарело при появлении обрвыза round.&]
[s3;%RU-RU &]
[s4; &]
[s5;:RectTracker`:`:Get`(`): [_^Rect^ Rect]_[* Get]()&]
[s2;%RU-RU Возвращает текущий трэкинговый 
прямоугольник.&]
[s3;%RU-RU &]
[s4; &]
[s5;:RectTracker`:`:Track`(const Rect`&`,int`,int`): [_^Rect^ Rect]_[* Track]([@(0.0.255) c
onst]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 r], [@(0.0.255) int]_[*@3 tx]_`=_ALIGN`_RIGHT, 
[@(0.0.255) int]_[*@3 ty]_`=_ALIGN`_BOTTOM)&]
[s2;%RU-RU Выполняет трэкинговый цикл модального 
прямоугольника. [%-*@3 tx] и [%-*@3 ty] задают, 
какой край или угол прямоугольника 
будет изменён:&]
[s2;%RU-RU &]
[ {{2807:2850:4343<288;>1088;h1;@(204) [s0; [*@3 tx]]
:: [s0; [*@3 ty]]
:: [s0;%RU-RU Изменяется]
::@2 [s0;%RU-RU ALIGN`_LEFT]
:: [s0;%RU-RU ALIGN`_TOP]
:: [s0;%RU-RU Верхний`-левый угол.]
:: [s0;%RU-RU ALIGN`_LEFT]
:: [s0;%RU-RU ALIGN`_CENTER]
:: [s0;%RU-RU Левый край.]
:: [s0;%RU-RU ALIGN`_LEFT]
:: [s0;%RU-RU ALIGN`_BOTTOM]
:: [s0;%RU-RU Нижний`-левый угол.]
:: [s0;%RU-RU ALIGN`_CENTER]
:: [s0;%RU-RU ALIGN`_TOP]
:: [s0;%RU-RU Верхний край.]
::@(255.255.192) [s0;%RU-RU ALIGN`_CENTER]
:: [s0;%RU-RU ALIGN`_CENTER]
:: [s0;%RU-RU Прямоугольник перемещён.]
::@2 [s0;%RU-RU ALIGN`_CENTER]
:: [s0;%RU-RU ALIGN`_BOTTOM]
:: [s0;%RU-RU Нижний край.]
:: [s0;%RU-RU ALIGN`_RIGHT]
:: [s0;%RU-RU ALIGN`_TOP]
:: [s0;%RU-RU Верхний`-правый угол.]
:: [s0;%RU-RU ALIGN`_RIGHT]
:: [s0;%RU-RU ALIGN`_CENTER]
:: [s0;%RU-RU Правый край.]
:: [s0;%RU-RU ALIGN`_RIGHT]
:: [s0;%RU-RU ALIGN`_BOTTOM]
:: [s0;%RU-RU Нижний`-правый угол.]}}&]
[s2;%RU-RU &]
[s2;%RU-RU Возвращает новый прямоугольник.&]
[s3;%RU-RU &]
[s4; &]
[s5;:RectTracker`:`:TrackHorzLine`(int`,int`,int`,int`): [@(0.0.255) int]_[* TrackHorzLin
e]([@(0.0.255) int]_[*@3 x0], [@(0.0.255) int]_[*@3 y0], [@(0.0.255) int]_[*@3 cx], 
[@(0.0.255) int]_[*@3 line])&]
[s2;%RU-RU Выполняет трэкинговый цикл горизонтальн
ой линии. [%-*@3 x0], [%-*@3 y0] является базовой 
точкой, [%-*@3 cx] `- ширина линии, [%-*@3 line] 
`- текущая позиция. Возвращает новую 
позицию.&]
[s3;%RU-RU &]
[s4; &]
[s5;:RectTracker`:`:TrackVertLine`(int`,int`,int`,int`): [@(0.0.255) int]_[* TrackVertLin
e]([@(0.0.255) int]_[*@3 x0], [@(0.0.255) int]_[*@3 y0], [@(0.0.255) int]_[*@3 cy], 
[@(0.0.255) int]_[*@3 line])&]
[s2;%RU-RU Выполняет трэкинговый цикл вертикальной
 линии. [%-*@3 x0], [%-*@3 y0] является базовой 
точкой, [%-*@3 cy] `- ширина линии, [%-*@3 line 
]`- текущая позиция. Возвращает новую 
позицию.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:RectTracker`:`:TrackLine`(int`,int`): [_^Upp`:`:Point^ Point]_[* TrackLine]([@(0.0.255) i
nt]_[*@3 x0], [@(0.0.255) int]_[*@3 y0])&]
[s2;%RU-RU Возвращает трэкинговый цикл линии 
из точки [%-*@3 x0] [%-*@3 y0]. Возвращает новую 
конечную точку.&]
[s3;%RU-RU &]
[s4; &]
[s5;:RectTracker`:`:RectTracker`(Ctrl`&`): [* RectTracker]([_^Ctrl^ Ctrl][@(0.0.255) `&]_[*@3 m
aster])&]
[s2;%RU-RU Конструирует RectTracker, где используется 
область видимости мастера [%-*@3 master] 
для черчения и системы координат.&]
[s3;%RU-RU &]
[s0;%RU-RU ]]