topic "Класс LineEdit";
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_} 
[ {{10000@3 [s0;%RU-RU [*@(229)4 Класс LineEdit]]}}&]
[s3; &]
[s1;:LineEdit`:`:class: [@(0.0.255)3 class][3 _][*3 LineEdit][3 _:_][@(0.0.255)3 public][3 _][*@3;3 T
extCtrl]&]
[s9;%RU-RU Виджет, используемый для построчного 
редактирования документа.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Защищённых Методов]]}}&]
[s3; &]
[s5;:LineEdit`:`:HighlightLine`(int`,Vector`<LineEdit`:`:Highlight`>`&`,int`): [@(0.0.255) v
irtual] [@(0.0.255) void]_[* HighlightLine]([@(0.0.255) int]_[*@3 line], 
[_^Vector^ Vector][@(0.0.255) <][_^LineEdit`:`:Highlight^ Highlight][@(0.0.255) >`&]_[*@3 h
], [@(0.0.255) int]_[*@3 pos])&]
[s2;%RU-RU Метод можно переписывать, добавляя 
подсветку синтаксиса в LineEdit. Он вызывается 
один раз для каждой отрисовываемой 
на экране строки. [%-*@3 line] это индекс 
строки (первая строка равна 0), [%-*@3 h] 
представляет ячейки символов в этой 
строке; клиентский код должен изменить 
этот массив, чтобы добиться подсветки, 
[%-*@3 pos] `- это индекс символа от начала 
текста. [%-*@3 h] изначально содержит ещё 
один пробел в добавок к числу символов 
в строке; этот дополнительный символ 
можно использовать для установки 
внешнего вида остальной строки, после 
последнего символа. Также нужно добавить 
символов в [%-*@3 h]. Дефолтная реализация 
пустая.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:NewScrollPos`(`): [@(0.0.255) virtual] [@(0.0.255) void]_[* NewScrollPos](
)&]
[s2;%RU-RU Вызывается при получение полосой 
промотки новой позиции. Дефолтная 
реализация пустая.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Публичных Членов]]}}&]
[s3;%RU-RU &]
[s5;:Upp`:`:LineEdit`:`:WhenScroll: [_^Upp`:`:Event^ Event]<>_[* WhenScroll]&]
[s2;%RU-RU Вызывается при новой позиции 
полосы промотки.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:GetFontSize`(`)const: [_^Size^ Size]_[* GetFontSize]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает размер ячейки символа 
шрифта (LineEdit ожидает моноширинные 
глифы `-  ширина символа константна).&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:GetGPos`(int`,int`)const: [@(0.0.255) int]_[* GetGPos]([@(0.0.255) int]_[*@3 l
n], [@(0.0.255) int]_[*@3 cl])_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает `"графическую`" позицию 
строки [%-*@3 ln] и колонки [%-*@3 cl]. Принимаются 
во внимание любые символы табуляторы 
в строке.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:GetMousePos`(Point`)const: [@(0.0.255) int]_[* GetMousePos]([_^Point^ Poin
t]_[*@3 p])_[@(0.0.255) const]&]
[s2;%RU-RU Получить смещение символа, размещённого
 в точке [%-*@3 p].&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:GetColumnLine`(int`)const: [_^Point^ Point]_[* GetColumnLine]([@(0.0.255) i
nt]_[*@3 pos])_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает строку и колонку для 
символа в [%-*@3 pos], принимая во внимание 
все табуляторы. Колонка является 
членом x итоговой точки Point, строка 
`- y.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:GetColumnLinePos`(Point`)const: [@(0.0.255) int]_[* GetColumnLinePos]([_^Point^ P
oint]_[*@3 pos])_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает позицию для данной 
колонки и строки [%-*@3 pos]. Принимает 
во внимание табуляторы.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:GetIndexLine`(int`)const: [_^Point^ Point]_[* GetIndexLine]([@(0.0.255) in
t]_[*@3 pos])_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает строку и индекс символа 
в строке для заданной позиции [%-*@3 pos]. 
Не принимает во внимание табуляторы.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:GetIndexLinePos`(Point`)const: [@(0.0.255) int]_[* GetIndexLinePos]([_^Point^ P
oint]_[*@3 pos])_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает позицию для данной 
колонки и строки [%-*@3 pos]. Не принимает 
во внимание табуляторы..&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:SetRectSelection`(int`,int`): [@(0.0.255) void]_[* SetRectSelection]([@(0.0.255) i
nt]_[*@3 l], [@(0.0.255) int]_[*@3 h])&]
[s2;%RU-RU Устанавливает прямоугольное 
выделение.&]
[s3;%RU-RU &]
[s4; &]
[s5;:LineEdit`:`:SetRectSelection`(const Rect`&`): [@(0.0.255) void]_[* SetRectSelection](
[@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 rect])&]
[s2;%RU-RU То же, что и SetRectSelection(GetGPos(rect.top, 
rect.left), GetGPos(rect.bottom, rect.right));&]
[s3;%RU-RU &]
[s4; &]
[s5;:LineEdit`:`:GetRectSelection`(`)const: [_^Rect^ Rect]_[* GetRectSelection]()_[@(0.0.255) c
onst]&]
[s2;%RU-RU Возвращает прямоугольное выделение 
(как `"graphical`").&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:GetRectSelection`(const Rect`&`,int`,int`&`,int`&`): [@(0.0.255) bool]_
[* GetRectSelection]([@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 rect], 
[@(0.0.255) int]_[*@3 line], [@(0.0.255) int`&]_[*@3 l], [@(0.0.255) int]_`&[*@3 h])&]
[s2;%RU-RU Возвращает нижний и верхний пределы 
[%-*@3 l] [%-*@3 h] символов строки [%-*@3 line], которые 
находятся в прямоугольном выделении 
[%-*@3 rect]. Возвращает false, когда строка 
не в выделении.&]
[s3;%RU-RU &]
[s4; &]
[s5;:LineEdit`:`:ScrollUp`(`): [@(0.0.255) void]_[* ScrollUp]()&]
[s2;%RU-RU Проматывает текст на строку вверх.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:ScrollDown`(`): [@(0.0.255) void]_[* ScrollDown]()&]
[s2;%RU-RU Проматывает текст на строку вниз.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:ScrollPageUp`(`): [@(0.0.255) void]_[* ScrollPageUp]()&]
[s2;%RU-RU Проматывает текст на страницу 
вверх.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:ScrollPageDown`(`): [@(0.0.255) void]_[* ScrollPageDown]()&]
[s2;%RU-RU Проматывает текст на страницу 
вниз.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:ScrollBegin`(`): [@(0.0.255) void]_[* ScrollBegin]()&]
[s2;%RU-RU Проматывает текст в начало.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:ScrollEnd`(`): [@(0.0.255) void]_[* ScrollEnd]()&]
[s2;%RU-RU Проматывает текст в конец.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:GetLineScreenRect`(int`)const: [_^Rect^ Rect]_[* GetLineScreenRect]([@(0.0.255) i
nt]_[*@3 line])_[@(0.0.255) const]&]
[s2;%RU-RU Получает абсолютную позицию прямоугольн
ика экрана строки [%-*@3 line].&]
[s5;K:`:`:LineEdit`:`:TopCursor`(`): [@(0.0.255) void][@(64) _]TopCursor[@(64) ()]&]
[s2;%RU-RU Проматывает текст, помещая строку 
с курсором первой в поле видимости.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:TopCursor`(int`): [@(0.0.255) void]_[* TopCursor]([@(0.0.255) int]_[*@3 line
s]_`=_[@3 0])&]
[s2;%RU-RU Проматывает текст, помещая строку 
с курсором на расстоянии в [%-*@3 lines] 
от верха.&]
[s3;%RU-RU &]
[s4; &]
[s5;:LineEdit`:`:CenterCursor`(`): [@(0.0.255) void]_[* CenterCursor]()&]
[s2;%RU-RU Проматывает текст, помещая строку 
с курсором в центре видимости.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:MoveUpDown`(int`,bool`): [@(0.0.255) void]_[* MoveUpDown]([@(0.0.255) int]_
[*@3 n], [@(0.0.255) bool]_[*@3 sel]_`=_[@(0.0.255) false])&]
[s2;%RU-RU Перемещает курсор на [%-*@3 n] строк 
вверх ([%-*@3 n] отрицательное) или вниз. 
Если [%-*@3 sel] равно true, выделяет символы 
между начальной и конечной позицией 
курсора.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:MoveLeft`(bool`): [@(0.0.255) void]_[* MoveLeft]([@(0.0.255) bool]_[*@3 sel]_
`=_[@(0.0.255) false])&]
[s5;:LineEdit`:`:MoveRight`(bool`): [@(0.0.255) void]_[* MoveRight]([@(0.0.255) bool]_[*@3 se
l]_`=_[@(0.0.255) false])&]
[s5;:LineEdit`:`:MoveUp`(bool`): [@(0.0.255) void]_[* MoveUp]([@(0.0.255) bool]_[*@3 sel]_`=_
[@(0.0.255) false])&]
[s5;:LineEdit`:`:MoveDown`(bool`): [@(0.0.255) void]_[* MoveDown]([@(0.0.255) bool]_[*@3 sel]_
`=_[@(0.0.255) false])&]
[s5;:LineEdit`:`:MovePageUp`(bool`): [@(0.0.255) void]_[* MovePageUp]([@(0.0.255) bool]_[*@3 s
el]_`=_[@(0.0.255) false])&]
[s5;:LineEdit`:`:MovePageDown`(bool`): [@(0.0.255) void]_[* MovePageDown]([@(0.0.255) bool]_
[*@3 sel]_`=_[@(0.0.255) false])&]
[s5;:LineEdit`:`:MoveHome`(bool`): [@(0.0.255) void]_[* MoveHome]([@(0.0.255) bool]_[*@3 sel]_
`=_[@(0.0.255) false])&]
[s5;:LineEdit`:`:MoveEnd`(bool`): [@(0.0.255) void]_[* MoveEnd]([@(0.0.255) bool]_[*@3 sel]_`=
_[@(0.0.255) false])&]
[s5;:LineEdit`:`:MoveTextBegin`(bool`): [@(0.0.255) void]_[* MoveTextBegin]([@(0.0.255) boo
l]_[*@3 sel]_`=_[@(0.0.255) false])&]
[s5;:LineEdit`:`:MoveTextEnd`(bool`): [@(0.0.255) void]_[* MoveTextEnd]([@(0.0.255) bool]_[*@3 s
el]_`=_[@(0.0.255) false])&]
[s2;%RU-RU [%- Перемещает курсор в определённом 
направлении (Home/End это начало/конец 
строки). Если] [%-*@3 sel] равно true, выделяет 
символы между начальной и конечной 
позицией курсора.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:InsertChar`(dword`,int`,bool`): [@(0.0.255) bool]_[* InsertChar]([_^dword^ d
word]_[*@3 key], [@(0.0.255) int]_[*@3 count]_`=_[@3 1], [@(0.0.255) bool]_[*@3 canoverwrite]_
`=_[@(0.0.255) false])&]
[s2;%RU-RU Вставляет [%-*@3 count ]символов клавиши 
[%-*@3 key] в позиции курсора. Если [%-*@3 canoverwrite] 
равно true, может использоваться режем 
переписания (установленный по нажатию 
пользователем клавиши Insert).&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:DeleteChar`(`): [@(0.0.255) void]_[* DeleteChar]()&]
[s2;%RU-RU Удаляет символ в позиции курсора.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:Backspace`(`): [@(0.0.255) void]_[* Backspace]()&]
[s2;%RU-RU Если символ не первый в тексте, 
удаляет его и перемещает курсор на 
позицию назад.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:DeleteLine`(`): [@(0.0.255) void]_[* DeleteLine]()&]
[s2;%RU-RU Удаляет строку с курсором (если 
это не единственная строка текста).&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:CutLine`(`): [@(0.0.255) void]_[* CutLine]()&]
[s2;%RU-RU Сохраняет строку в буфер обмена 
и вызывает DeleteLine.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:PasteColumn`(const WString`&`): [@(0.0.255) void]_[* PasteColumn]([@(0.0.255) c
onst]_[_^WString^ WString][@(0.0.255) `&]_[*@3 text])&]
[s2;%RU-RU Вставляет строки текста [%-*@3 text] 
в актуальную графическую колонку 
текста.&]
[s3;%RU-RU &]
[s4; &]
[s5;:LineEdit`:`:PasteColumn`(`): [@(0.0.255) void]_[* PasteColumn]()&]
[s2;%RU-RU Вставляет колонку с текстом буфера 
обмена.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:Sort`(`): [@(0.0.255) void]_[* Sort]()&]
[s2;%RU-RU Сортирует строки, основываясь 
на прямоугольном выделении.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:GetScrollPos`(`)const: [_^Point^ Point]_[* GetScrollPos]()_[@(0.0.255) con
st]&]
[s2;%RU-RU Возвращает актуальную позицию 
промотки.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:GetPageSize`(`): [_^Size^ Size]_[* GetPageSize]()&]
[s2;%RU-RU Возвращает текущий размер видимости 
в ячейках символов.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:SetScrollPos`(Point`): [@(0.0.255) void]_[* SetScrollPos]([_^Point^ Point]_
[*@3 p])&]
[s2;%RU-RU Устанавливает позицию промотки 
видимости, напр., в значение, полученное 
от GetScrollPos.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:GetEditPos`(`)const: [_^LineEdit`:`:EditPos^ EditPos]_[* GetEditPos]()_[@(0.0.255) c
onst]&]
[s2;%RU-RU Возвращает позицию редактирования 
в тексте, для восстановления позднее. 
Позиция редактирования состоит из 
позиций курсора и полосы промотки.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:LineEdit`:`:SetEditPos`(const Upp`:`:TextCtrl`:`:EditPos`&`): [@(0.0.255) v
oid]_[* SetEditPos]([@(0.0.255) const]_[_^Upp`:`:TextCtrl`:`:EditPos^ EditPos][@(0.0.255) `&
]_[*@3 pos])&]
[s2;%RU-RU Устанавливает позицию редактирования, 
полученую при вызове GetEditPos. Если курсор 
после восстановление не в поле видимости, 
вид проматывается так, как это происходит 
при вызове ScrollIntoCursor.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:SetEditPosSb`(const LineEdit`:`:EditPos`&`): [@(0.0.255) void]_[* SetEdi
tPosSb]([@(0.0.255) const]_[_^LineEdit`:`:EditPos^ LineEdit`::EditPos][@(0.0.255) `&]_[*@3 p
os])&]
[s2;%RU-RU Устанавливает позицию редактирования, 
полученную вызовом GetEditPos. В отличие 
от SetEditPos, для получения курсора в 
области видимости не выполняется 
никаких промоток.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:LineEdit`:`:SetEditPosSbOnly`(const Upp`:`:LineEdit`:`:EditPos`&`): [@(0.0.255) v
oid]_[* SetEditPosSbOnly]([@(0.0.255) const]_[_^Upp`:`:LineEdit`:`:EditPos^ LineEdit`::E
ditPos][@(0.0.255) `&]_[*@3 pos])&]
[s2;%RU-RU Устанавливает только позицию 
полосы промотки [%-*@3 pos].&]
[s3;%RU-RU &]
[s4; &]
[s5;:LineEdit`:`:ScrollIntoCursor`(`): [@(0.0.255) void]_[* ScrollIntoCursor]()&]
[s2;%RU-RU Выполняет минимальную промотку, 
чтобы получить курсор в видимости. 
`"Минимальная`" означает, что если 
курсор находится `"перед`" видимостью, 
то вид `"проматывается`", перемещая 
курсор на первую строку, если же он 
`"после`", то он перемещается на последнюю 
строку.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:GetCaretPoint`(`)const: [_^Point^ Point]_[* GetCaretPoint]()_[@(0.0.255) c
onst]&]
[s2;%RU-RU Возвращает текущую позицию каретки.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:Clear`(`): [@(0.0.255) void]_[* Clear]()&]
[s2;%RU-RU Устанавливает пустой текст.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:OverWriteMode`(bool`): [@(0.0.255) void]_[* OverWriteMode]([@(0.0.255) boo
l]_[*@3 o]_`=_[@(0.0.255) true])&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:IsOverWriteMode`(`)const: [@(0.0.255) bool]_[* IsOverWriteMode]()_[@(0.0.255) c
onst]&]
[s2;%RU-RU В режиме OverWriteMode, вводимые символы 
заменяют контент, а не вставляются 
в него.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:RefreshChars`(bool`(`*`)`(int c`)`): [@(0.0.255) void]_[* RefreshChars](
[@(0.0.255) bool]_(`*[*@3 predicate])([@(0.0.255) int]_c))&]
[s2;%RU-RU Вызывает предикат [%-*@3 predicate] для 
всех символов в текущей видимости, 
и, когда он возвращает true, освежает 
(планирует на перерисовку) ячейку 
символа. Применяется в продвинутых 
редакторах кода (напр., освежение 
цвета фигурных скобок в редакторе 
DinrusIDE).&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:TabSize`(int`): [_^LineEdit^ LineEdit][@(0.0.255) `&]_[* TabSize]([@(0.0.255) i
nt]_[*@3 n])&]
[s2;%RU-RU Устанавливает размер табулятора.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:GetTabSize`(`)const: [@(0.0.255) int]_[* GetTabSize]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает текущий размер табулятора.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:BorderColumn`(int`,Color`): [_^LineEdit^ LineEdit][@(0.0.255) `&]_[* Borde
rColumn]([@(0.0.255) int]_[*@3 col], [_^Color^ Color]_[*@3 c]_`=_SColorFace())&]
[s2;%RU-RU Отображает колонку бордюра с 
шириной [%-*@3 col] и с цветом [%-*@3 c].&]
[s3; &]
[s4; &]
[s5;:Upp`:`:LineEdit`:`:GetBorderColumn`(`)const: [@(0.0.255) int]_[* GetBorderColumn]()_
[@(0.0.255) const]&]
[s2;%RU-RU Возвращает позицию колонки бордюра.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:SetFont`(Font`): [_^LineEdit^ LineEdit][@(0.0.255) `&]_[* SetFont]([_^Font^ F
ont]_[*@3 f])&]
[s2;%RU-RU Устанавливает шрифт. HighlightLine может 
заменять этот шрифт, однако, размер 
ячейки символа определяется этим 
шрифтом (заметьте, что идеограммы 
CJK имеют ширину в две ячейки). Шрифт 
должен быть моноширинным. Возвращает 
`*this. Дефолтный шрифт Courier(16).&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:GetFont`(`)const: [_^Font^ Font]_[* GetFont]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает шрифт.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:NoHorzScrollbar`(bool`): [_^LineEdit^ LineEdit][@(0.0.255) `&]_[* NoHorzSc
rollbar]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%RU-RU В этом режиме горизонтальная 
полоса промотки никогда не показывается. 
Дефолтно выключен.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:IsNoHorzScrollbar`(`)const: [@(0.0.255) bool]_[* IsNoHorzScrollbar]()_[@(0.0.255) c
onst]&]
[s2;%RU-RU Возвращает статус NoHorzScrollbar.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:ShowTabs`(bool`): [_^LineEdit^ LineEdit][@(0.0.255) `&]_[* ShowTabs]([@(0.0.255) b
ool]_[*@3 st]_`=_[@(0.0.255) true])&]
[s2;%RU-RU В этом режиме виджет отображает 
табуляторы с ложной графикой. Дефолтно 
выключен.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:IsShowTabs`(`)const: [@(0.0.255) bool]_[* IsShowTabs]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает статус ShowTabs..&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:ShowSpaces`(bool`): [_^LineEdit^ LineEdit][@(0.0.255) `&]_[* ShowSpaces]([@(0.0.255) b
ool]_[*@3 ss]_`=_[@(0.0.255) true])&]
[s2;%RU-RU В этом режиме виджет отображает 
пробелы с ложными точками. Дефолтно 
выключен.&]
[s3;%RU-RU &]
[s4; &]
[s5;:LineEdit`:`:IsShowSpaces`(`)const: [@(0.0.255) bool]_[* IsShowSpaces]()_[@(0.0.255) co
nst]&]
[s2;%RU-RU Возвращает статус ShowSpaces.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:LineEdit`:`:ShowLineEndings`(bool`): [_^Upp`:`:LineEdit^ LineEdit][@(0.0.255) `&
]_[* ShowLineEndings]([@(0.0.255) bool]_[*@3 sl]_`=_[@(0.0.255) true])&]
[s2;%RU-RU В этом режиме виджет отображает 
окончания строк с ложными точками. 
Дефолтно выключен.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:LineEdit`:`:IsShowLineEndings`(`)const: [@(0.0.255) bool]_[* IsShowLineEnding
s]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает статус ShowLineEndings.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:LineEdit`:`:WarnWhiteSpace`(bool`): [_^Upp`:`:LineEdit^ LineEdit][@(0.0.255) `&
]_[* WarnWhiteSpace]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%RU-RU В этом режиме виджет показывает 
пробелы, которые кажутся ошибочными, 
как табуляторы после пробелов, или 
пробелы в окончании строк. Дефолтно 
выключен.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:LineEdit`:`:IsWantWhiteSpace`(`)const: [@(0.0.255) bool]_[* IsWantWhiteSpace](
)_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает статус WarnWhiteSpace.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:WithCutLine`(bool`): [_^LineEdit^ LineEdit][@(0.0.255) `&]_[* WithCutLine](
[@(0.0.255) bool]_[*@3 b])&]
[s2;%RU-RU В этом режиме виджет вызывает 
CutLine, когда пользователь нажимает 
Ctrl`+Y или Ctrl`+L. Дефолтно включен.&]
[s0; &]
[s4; &]
[s5;:LineEdit`:`:NoCutLine`(`): [_^LineEdit^ LineEdit][@(0.0.255) `&]_[* NoCutLine]()&]
[s2;%RU-RU Отключает WithCutLine.&]
[s3;%RU-RU &]
[s4; &]
[s5;:LineEdit`:`:IsWithCutLine`(`)const: [@(0.0.255) bool]_[* IsWithCutLine]()_[@(0.0.255) c
onst]&]
[s2;%RU-RU Возвращает статус WithCutLine.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:SetFilter`(int`(`*`)`(int c`)`): [_^LineEdit^ LineEdit][@(0.0.255) `&]_[* S
etFilter]([@(0.0.255) int]_(`*[*@3 f])([@(0.0.255) int]_c))&]
[s2;%RU-RU Устанавливает фильтр обработанных 
символов. Функция Filter может менять 
символы до их вставки в текст. Если 
фильтрующая функция возвращает ноль, 
символ отвергается.&]
[s0; &]
[s0; &]
[ {{10000@3 [s0;%RU-RU [*@(229)4 Структура LineEdit`::Highlight]]}}&]
[s3; &]
[s1;:LineEdit`:`:Highlight`:`:struct: [@(0.0.255)3 struct][3 _][*3 Highlight][3 _:_][@(0.0.255)3 p
ublic][3 _][*@3;3 Moveable][@(0.0.255)3 <][*3 Highlight][@(0.0.255)3 >][3 _]&]
[s9; Эта структура определяет свойства 
подсвечивания синтаксиса единичной 
символьной ячейки. &]
[s3;%RU-RU &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Публичных Членов]]}}&]
[s3; &]
[s5;:LineEdit`:`:Highlight`:`:paper: [_^Color^ Color]_[* paper]&]
[s3;%RU-RU &]
[s4; &]
[s5;:LineEdit`:`:Highlight`:`:ink: [_^Color^ Color]_[* ink]&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:Highlight`:`:font: [_^Font^ Font]_[* font]&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:Highlight`:`:chr: [_^wchar^ wchar]_[* chr]&]
[s3;%RU-RU &]
[s4; &]
[s5;:LineEdit`:`:Highlight`:`:operator`=`=`(const LineEdit`:`:Highlight`&`)const: [@(0.0.255) b
ool]_[* operator`=`=]([@(0.0.255) const]_[_^LineEdit`:`:Highlight^ Highlight][@(0.0.255) `&
]_[*@3 h])_[@(0.0.255) const]&]
[s2;%RU-RU Сравнение на равенство.&]
[s3; &]
[s0; ]]