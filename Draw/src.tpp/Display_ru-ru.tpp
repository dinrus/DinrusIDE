topic "Класс Display";
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
[{_}%RU-RU 
[ {{10000@(113.42.0) [s0; [*@7;4 Класс Display]]}}&]
[s3;%- &]
[s1;:Display`:`:class:%- [@(0.0.255) class]_Display&]
[s9; Display и производные от Display классы 
 отображают значение Value на заданной 
прямоугольной области. Ссылки на 
Display`'и используются во многих виджетах 
в качестве атрибутов, влияющих на 
отображение значений Values виджетом. 
Дефолтная реализация применяет StdDisplay 
для выполнения всех действий (смотрите 
ниже описание к StdDisplay).&]
[s0; &]
[s0; [* Константы визуального стиля ]используются 
как бит`-флаги параметра `"style`" методоб 
отображения и несут дополнительную 
информацию о требуемом зрительном 
образе:&]
[s0;l288;:Display`:`:CURSOR:~~~.1408;%- [* CURSOR-|]ГИП`-элемент 
является текущим (`"имеет фокус`").&]
[s0;l288;:Display`:`:FOCUS:~~~.1408; [%-* FOCUS][* -|][%- ГИП`-элемент] 
имеет фокус.&]
[s0;l288;:Display`:`:SELECT:~~~.1408;%- [* SELECT]-|ГИП`-элемент 
выделен.&]
[s0;l288;:Display`:`:READONLY:~~~.1408;%- [* READONLY]-|ГИП`-элемент 
только для чтения.&]
[s3;%- &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Список Публичных Членов]]}}&]
[s3;%- &]
[s5;:Display`:`:Paint`(Draw`&`,const Rect`&`,const Value`&`,Color`,Color`,dword`)const:%- [@(0.0.255) v
irtual] [@(0.0.255) void]_Paint([_^Draw^ Draw][@(0.0.255) `&]_[@3 w], 
[@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[@3 r], [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&
]_[@3 q], [_^Color^ Color]_[@3 ink], [_^Color^ Color]_[@3 paper], [_^dword^ dword]_[@3 style])_
[@(0.0.255) const]&]
[s2; Этот виртуальный метод используется 
для отрисовки прямоугольного контента, 
в соответствии с заданным значением 
Value. Заметьте, что для производных 
классов свойственно понимать только 
типы значения Value, для которых они 
предназначены (и `"падать`" в противном 
случае) `- клиентский код ответственен 
за использование корректного дисплея 
Display.&]
[s7; [%-*C@3 w]-|Draw.&]
[s7; [%-*C@3 r]-|Целевой прямоугольник.&]
[s7; [%-*C@3 q]-|Отрисовываемое значение.&]
[s7; [%-*C@3 ink]-|Рекомендуемый цвет переднего 
плана.&]
[s7; [%-*C@3 paper]-|Рекомендуемый цвет фона.&]
[s7; [%-*C@3 style]-|Визуальный стиль.&]
[s3;%- &]
[s4;%- &]
[s5;:Display`:`:PaintBackground`(Draw`&`,const Rect`&`,const Value`&`,Color`,Color`,dword`)const:%- [@(0.0.255) v
irtual] [@(0.0.255) void]_PaintBackground([_^Draw^ Draw][@(0.0.255) `&]_[@3 w], 
[@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[@3 r], [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&
]_[@3 q], [_^Color^ Color]_[@3 ink], [_^Color^ Color]_[@3 paper], [_^dword^ dword]_[@3 style])_
[@(0.0.255) const]&]
[s2; Этот виртуальный метод используется 
для отрисовки областей ГИП`-элемента, 
которые расположены вне прямоугольника 
(периметра), указанного в методе Paint, 
но у него должен иметься цвет, как`-то 
связанный с текущим классом Display `- 
обычно это тот же цвет, что  и фон, 
отрисовываемый в Paint. (Заметьте, что 
метод Paint должен очищать фон отдельно, 
хотя базовый класс Display определяет 
использование для этой задачи метода 
PaintBackground).&]
[s7; [%-*C@3 w]-|Draw.&]
[s7; [%-*C@3 r]-|Целевой прямоугольник.&]
[s7; [%-*C@3 q]-|Отрисовываемое значение.&]
[s7; [%-*C@3 ink]-|Рекомендуемый цвет переднего 
плана.&]
[s7; [%-*C@3 paper]-|Рекомендуемый цвет фона.&]
[s7; [%-*C@3 style]-|Визуальный стиль.&]
[s3;%- &]
[s4;%- &]
[s5;:Display`:`:GetStdSize`(const Value`&`)const:%- [@(0.0.255) virtual] 
[_^Size^ Size]_GetStdSize([@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[@3 q])_[@(0.0.255) c
onst]&]
[s2; Должен возвращать стандартный размер 
для данного значения и дисплея Display. 
Например, если Display отображает рисунки 
Images, то он должен вернуть размер Size 
этого рисунка Image в пикселях. Базовый 
Display возвращает размер текстового 
представления этого значения Value.&]
[s7; [%-*C@3 q]-|Значение.&]
[s7; [*/ Возвратное значение]-|Размер Value 
для Display`'я.&]
[s3;%- &]
[s4;%- &]
[s5;:Display`:`:RatioSize`(const Value`&`,int`,int`)const:%- [@(0.0.255) virtual] 
[_^Size^ Size]_RatioSize([@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[@3 q], 
[@(0.0.255) int]_[@3 cx], [@(0.0.255) int]_[@3 cy])_[@(0.0.255) const]&]
[s2; Возвращает размер для заданного 
значения, соблюдая пропорции.&]
[s7; [%-*C@3 q]-|Значение.&]
[s7; [%-*C@3 cx]-|Требуемая ширина. Если 0, то 
она должна быть вычислена с сохранением 
пропорции с cy.&]
[s7; [%-*C@3 cy]-|Требуемая высота. Если 0, должна 
быть вычислета пропорционально cx.&]
[s7; [*/ Возвратное значение]-|Размер Значения 
для Дисплея.&]
[s3; &]
[s0; &]
[s0; &]
[s0;%- &]
[ {{10000t/25b/25@(113.42.0) [s0;%- [*@(229)4 Структура AttrText]]}}&]
[s3; &]
[s1;:AttrText`:`:struct:%- [@(0.0.255) struct]_AttrText&]
[s9; Вспомогательный класс, конвертируемый 
в это Значение. StdDisplay, StdRightDisplay и StdCenterDisplay 
обнаруживают,  является ли переданное 
Значнеие типа AttrText и по разному обрабатывают
 его, адаптируя не`-нулевые атрибуты 
к вырисовываемому тексту. AttrText является 
`'богатым`' типом Value, поддерживающим 
сравнение и сериализацию. Его можно 
сравнивать с другими типами значений. 
Можно также присваивать текст, отличный 
от значения AttrText.&]
[s3;%- &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Список Публичных Членов]]}}&]
[s3;%- &]
[s5;:AttrText`:`:text:%- [_^WString^ WString]_text&]
[s2; Отображаемый текст.&]
[s3;%- &]
[s4;%- &]
[s5;:AttrText`:`:value:%- [_^Value^ Value]_[* value]&]
[s2; Представляет Значение AttrText`'а, обычно 
для сравнения/сортировки.&]
[s3;%- &]
[s4;%- &]
[s5;:AttrText`:`:font:%- [_^Font^ Font]_[* font]&]
[s2; Шрифт текста. Дефолтно инициализуется 
конструктором в StdFont.&]
[s3;%- &]
[s4;%- &]
[s5;:AttrText`:`:ink:%- [_^Color^ Color]_[* ink]&]
[s2; Цвет текста.&]
[s3;%- &]
[s4;%- &]
[s5;:AttrText`:`:normalink:%- [_^Color^ Color]_[* normalink]&]
[s2; Цвет текста, который будет использован, 
если этот элемент находится ни в выделенном,
 ни в фокусированном, ни в `"только`-чтение`" 
состоянии.&]
[s3;%- &]
[s4;%- &]
[s5;:AttrText`:`:paper:%- [_^Color^ Color]_[* paper]&]
[s2; Цвет фона&]
[s3;%- &]
[s4;%- &]
[s5;:AttrText`:`:normalpaper:%- [_^Color^ Color]_[* normalpaper]&]
[s2; Цвет фона, используемый, если этот 
элемент находится ни в выделенном, 
ни в фокусированном, ни в `"только`-чтение`" 
состоянии.&]
[s3;%- &]
[s4;%- &]
[s5;:AttrText`:`:align:%- [@(0.0.255) int]_[* align]&]
[s2; Текущее размещение. Может быть одним 
из ALIGN`_LEFT, ALIGN`_RIGHT, ALIGN`_CENTER.&]
[s3;%- &]
[s4;%- &]
[s5;:AttrText`:`:img:%- [_^Image^ Image]_[* img]&]
[s2; Иконка, размещённая слева.&]
[s3;%- &]
[s4;%- &]
[s5;:AttrText`:`:imgspc:%- [@(0.0.255) int]_[* imgspc]&]
[s2; Пространство между иконкой и текстом.&]
[s3;%- &]
[s4;%- &]
[s5;:AttrText`:`:Set`(const Value`&`):%- [_^AttrText^ AttrText][@(0.0.255) `&]_[* Set]([@(0.0.255) c
onst]_[_^Value^ Value][@(0.0.255) `&]_[*@3 v])&]
[s5;:AttrText`:`:operator`=`(const Value`&`):%- [_^AttrText^ AttrText][@(0.0.255) `&]_[* op
erator`=]([@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 v])&]
[s2; Устанавливает значение AttrText`'а и 
его текст (используя AsString).&]
[s3; &]
[s4;%- &]
[s5;:AttrText`:`:Text`(const String`&`):%- [_^AttrText^ AttrText][@(0.0.255) `&]_[* Text]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[*@3 txt])&]
[s5;:AttrText`:`:Text`(const WString`&`):%- [_^AttrText^ AttrText][@(0.0.255) `&]_[* Text](
[@(0.0.255) const]_[_^WString^ WString][@(0.0.255) `&]_[*@3 txt])&]
[s5;:AttrText`:`:Text`(const char`*`):%- [_^AttrText^ AttrText][@(0.0.255) `&]_[* Text]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 txt])&]
[s2; Устанавливает текст, независимо 
от Value.&]
[s3; &]
[s4;%- &]
[s5;:AttrText`:`:Ink`(Color`):%- [_^AttrText^ AttrText][@(0.0.255) `&]_[* Ink]([_^Color^ Colo
r]_[@3 c])&]
[s2; Устанавливает цвет текста.&]
[s7; [%-*C@3 c]-|Цвет.&]
[s7; [*/ Возвратное значение]-|`*this.&]
[s3;%- &]
[s4;%- &]
[s5;:AttrText`:`:NormalInk`(Color`):%- [_^AttrText^ AttrText][@(0.0.255) `&]_[* NormalInk](
[_^Color^ Color]_[*@3 c])&]
[s2; Устанавливает цвет текста, используемый, 
 если этот элемент находится ни в 
выделенном, ни в фокусированном, ни 
в `"только`-чтение`" состоянии.&]
[s3; &]
[s4;%- &]
[s5;:AttrText`:`:Paper`(Color`):%- [_^AttrText^ AttrText][@(0.0.255) `&]_[* Paper]([_^Color^ C
olor]_[@3 c])&]
[s2; Устанавливает цвет `"бумаги`" (фона).&]
[s7; [%-*C@3 c]-|Цвет.&]
[s7; [*/ Возвратное значение]-|`*this.&]
[s3;%- &]
[s4;%- &]
[s5;:AttrText`:`:NormalPaper`(Color`):%- [_^AttrText^ AttrText][@(0.0.255) `&]_[* NormalPap
er]([_^Color^ Color]_[*@3 c])&]
[s2; Устанавливает цвет фона, используемый, 
если этот элемент находится ни в выделенном,
 ни в фокусированном, ни в `"только`-чтение`" 
состоянии.&]
[s3; &]
[s4;%- &]
[s5;:AttrText`:`:SetFont`(Font`):%- [_^AttrText^ AttrText][@(0.0.255) `&]_[* SetFont]([_^Font^ F
ont]_[@3 f])&]
[s2; Устанавливает шрифт.&]
[s7; [%-*C@3 f]-|Шрифт.&]
[s7; [*/ Возвратное значение]-|`*this.&]
[s3;%- &]
[s4;%- &]
[s5;:AttrText`:`:Bold`(bool`):%- [_^AttrText^ AttrText][@(0.0.255) `&]_[* Bold]([@(0.0.255) b
ool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; Вызывает полужирный шрифт.Bold([%-*@3 b]). 
Возвращает `*this.&]
[s3; &]
[s4;%- &]
[s5;:AttrText`:`:Italic`(bool`):%- [_^AttrText^ AttrText][@(0.0.255) `&]_[* Italic]([@(0.0.255) b
ool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; Вызывает курсивный шрифт Italic([%-*@3 b]). 
Возвращает `*this.&]
[s3; &]
[s4;%- &]
[s5;:AttrText`:`:Underline`(bool`):%- [_^AttrText^ AttrText][@(0.0.255) `&]_[* Underline]([@(0.0.255) b
ool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; Вызывает подчёркнутый шрифт.Underline([%-*@3 b]). 
Возвращает `*this.&]
[s3; &]
[s4;%- &]
[s5;:AttrText`:`:Strikeout`(bool`):%- [_^AttrText^ AttrText][@(0.0.255) `&]_[* Strikeout]([@(0.0.255) b
ool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; Вызывает зачёркнутый шрифт.Strikeout([%-*@3 b]). 
Возвращает `*this.&]
[s3; &]
[s4;%- &]
[s5;:AttrText`:`:Align`(int`):%- [_^AttrText^ AttrText][@(0.0.255) `&]_[* Align]([@(0.0.255) i
nt]_[@3 a])&]
[s2; Устанавливает горизонтальное размещение 
текста. Одобряются значения ALIGN`_LEFT, 
ALIGN`_CENTER и ALIGN`_RIGHT.&]
[s3;%- &]
[s4;%- &]
[s5;:AttrText`:`:Left`(`):%- [_^AttrText^ AttrText][@(0.0.255) `&]_[* Left]()&]
[s2; Размещает текст слева.&]
[s3;%- &]
[s4;%- &]
[s5;:AttrText`:`:Center`(`):%- [_^AttrText^ AttrText][@(0.0.255) `&]_[* Center]()&]
[s2; Размещает текст по центру.&]
[s3;%- &]
[s4;%- &]
[s5;:AttrText`:`:Right`(`):%- [_^AttrText^ AttrText][@(0.0.255) `&]_[* Right]()&]
[s2; Размещает текст справа.&]
[s3;%- &]
[s4;%- &]
[s5;:AttrText`:`:SetImage`(const Image`&`,int`):%- [_^AttrText^ AttrText][@(0.0.255) `&]_
[* SetImage]([@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[@3 m], 
[@(0.0.255) int]_[@3 spc]_`=_[@3 4])&]
[s2; Устанавливает иконку и пространство 
между иконкой и текстом.&]
[s3; &]
[s4;%- &]
[s5;:AttrText`:`:operator Value`(`)const:%- operator_[* Value]()_[@(0.0.255) const]&]
[s7; Преобразует AttrText в Value.&]
[s3;%- &]
[s4;%- &]
[s5;:AttrText`:`:AttrText`(const Value`&`):%- [* AttrText]([@(0.0.255) const]_[_^Value^ Val
ue][@(0.0.255) `&]_[*@3 v])&]
[s2; Преобразует Value в AttrText. Если [%-*@3 v] не 
является AttrText`'ом, то оно устанавливается 
как значение конструируемого дефолтно 
AttrText`'а.&]
[s3; &]
[s4;%- &]
[s5;:AttrText`:`:AttrText`(`):%- [* AttrText]()&]
[s2; Дефолтный конструктор. Присваивает 
Null всем членам, кроме шрифта, инициализуемог
о в StdFont.&]
[s3; &]
[s0; &]
[s0; &]
[s0;%- &]
[ {{10000t/25b/25@(85.42.0) [s0;%- [*@(229)4 Стандартные дисплеи]]}}&]
[s9; Стандартные Дисплеи реализованы 
как `"functional globals`" (`"функглобы`") `- функции, 
возвращающие константную ссылку 
на единый глобальный экземпляр Display`'я.&]
[s3; &]
[s0; &]
[ {{3530:6470h1;@(204) [s0; Имя дисплея]
:: [s0; Описание]
::^@2 [s0;:StdDisplay: [* StdDisplay]]
::= [s0; Standard Display. Displays Value as text, unless it is AttrText 
(see above).]
::^ [s0;:StdRightDisplay: [* StdRightDisplay]]
::= [s0; Standard Display. Displays Value as right`-aligned text, unless 
it is AttrText (see above).]
::^ [s0;:StdCenterDisplay: [* StdCenterDisplay]]
::= [s0; Standard Display. Displays Value as centered text, unless it 
is AttrText (see above).]
::^ [s0;:ColorDisplay: [* ColorDisplay]]
::= [s0; Displays Color (required) `- simply paints background using 
the Value passed in.]
::^ [s0;:SizeTextDisplay: [* SizeTextDisplay]]
::= [s0; Similar to StdDisplay, but stretches the text size to fill whole 
display area.]
::^ [s0;:ImageDisplay: [* ImageDisplay]]
::= [s0; Displays Image passed in as Value, aligns it to the top`-left 
corner.]
::^ [s0;:FittedImageDisplay: [* FittedImageDisplay]]
::= [s0; Displays Image scaled to fit the rectangle.]
::^ [s0;:CenteredImageDisplay: [* CenteredImageDisplay]]
::= [s0; Displays Image centered in the rectangle.]
::^ [s0;:CenteredHighlightImageDisplay: [* CenteredHighlightImageDisplay]]
::= [s0; Displays Image centered in the rectangle with 1 pixel wide white 
border.]
::^ [s0;:DrawingDisplay: [* DrawingDisplay]]
::= [s0; Displays Drawing scaled to fit the rectangle.]}}&]
[s3; &]
[s0; ]]