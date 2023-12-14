topic "Класс TextCtrl";
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
[ {{10000@3 [s0; [*@(229)4 Класс TextCtrl]]}}&]
[s4; &]
[s1;:TextCtrl`:`:class: [@(0.0.255)3 class][3 _][*3 TextCtrl][3 _:_][@(0.0.255)3 public][3 _][*@3;3 C
trl][3 , ][@(0.0.255)3 protected][3 _][*@3;3 TextArrayOps]&]
[s9;%RU-RU Базовый класс, реализующий общие 
операции для [^topic`:`/`/CtrlLib`/src`/DocEdit`_ru`-ru`#DocEdit`:`:class^ D
ocEdit] (редактора `"сырого`" текста, в 
котором строки `- разделы, разделённые 
символом LF `-показываются как параграфы 
`- и подгоняются под размер окна, если 
не вмещаются в него по ширине) и [^topic`:`/`/CtrlLib`/src`/LineEdit`_ru`-ru`#LineEdit`:`:class^ L
ineEdit].(в котором строки `- просто строки 
и не подгоняются совсем никак). TextCtrl 
работает в кодировке UNICODE, но способен 
выполнять преобразования в любой 
из заранее определённых набсимов. 
Значением TextCtrl`'а является содержащийся 
в нём текст; его установка равносильна 
использованию метода Set, а чтение 
равносильно методу Get.&]
[s3;%RU-RU &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Защищённых Методов]]}}&]
[s4; &]
[s5;:TextCtrl`:`:SelectionChanged`(`): [@(0.0.255) virtual] [@(0.0.255) void]_[* SelectionC
hanged]()&]
[s2;%RU-RU Якорь секции или курсор изменён 
(если вызван при отсутствии выделения, 
то только курсор). При переписании 
следует в теле вызывать версию базового 
класса.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:ClearLines`(`): [@(0.0.255) virtual] [@(0.0.255) void]_[* ClearLines]()&]
[s2;%RU-RU Контент был удалён (напр., операцией 
Clear или Load). При переписании следует 
в теле вызывать версию базового класса.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:InsertLines`(int`,int`): [@(0.0.255) virtual] [@(0.0.255) void]_[* InsertL
ines]([@(0.0.255) int]_[*@3 line], [@(0.0.255) int]_[*@3 count])&]
[s2;%RU-RU Было вставлено [%-*@3 count] строк в 
позиции [%-*@3 line]. При переписании следует 
в теле вызывать версию базового класса.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:RemoveLines`(int`,int`): [@(0.0.255) virtual] [@(0.0.255) void]_[* RemoveL
ines]([@(0.0.255) int]_[*@3 line], [@(0.0.255) int]_[*@3 count])&]
[s2;%RU-RU Было удалено [%-*@3 count] строк в позиции 
[%-*@3 line]. При переписании следует в теле 
вызывать версию базового класса.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:PreInsert`(int`,const WString`&`): [@(0.0.255) virtual] 
[@(0.0.255) void]_[* PreInsert]([@(0.0.255) int]_[*@3 pos], [@(0.0.255) const]_[_^WString^ WS
tring][@(0.0.255) `&]_[*@3 text])&]
[s2;%RU-RU Вызывается перед тем, как любой 
текст [%-*@3 text] вставлен в позиции [%-*@3 pos.] 
При переписании следует в теле вызывать 
версию базового класса.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:PostInsert`(int`,const WString`&`): [@(0.0.255) virtual] 
[@(0.0.255) void]_[* PostInsert]([@(0.0.255) int]_[*@3 pos], [@(0.0.255) const]_[_^WString^ W
String][@(0.0.255) `&]_[*@3 text])&]
[s2;%RU-RU Вызывается после того, как текст 
[%-*@3 text] был вставлен в позиции [%-*@3 pos.]При 
переписании следует в теле вызывать 
версию базового класса.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:PreRemove`(int`,int`): [@(0.0.255) virtual] [@(0.0.255) void]_[* PreRemove
]([@(0.0.255) int]_[*@3 pos], [@(0.0.255) int]_[*@3 size])&]
[s2;%RU-RU Вызывается перед тем, как [%-*@3 size] 
символов удалится из позиции [%-*@3 pos]. 
При переписании следует в теле вызывать 
версию базового класса.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:PostRemove`(int`,int`): [@(0.0.255) virtual] [@(0.0.255) void]_[* PostRemo
ve]([@(0.0.255) int]_[*@3 pos], [@(0.0.255) int]_[*@3 size])&]
[s2;%RU-RU Вызывается после того, как [%-*@3 size] 
символов удалится из позиции [%-*@3 pos]. 
При переписании следует в теле вызывать 
версию базового класса.&]
[s3;%RU-RU &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Публичных Методов]]}}&]
[s3; &]
[s5;:TextCtrl`:`:RefreshLine`(int`): [@(0.0.255) virtual] [@(0.0.255) void]_[* RefreshLine](
[@(0.0.255) int]_[*@3 i])&]
[s2;%RU-RU Этот виртуальный метод вызывается 
TextCtrl`'ом, когда изменён контент строки 
[%-*@3 i]. При переписании следует в теле 
вызывать версию базового класса.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:WhenBar: [_^Callback1^ Callback1][@(0.0.255) <]Bar[@(0.0.255) `&>]_[* WhenBa
r]&]
[s2;%RU-RU Этот обратный вызов происходит, 
когда пользователь делает правый 
клик на виджете и должен задать локальное 
меню. Инициализируется конструктором 
в метод StdBar.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:WhenState: [_^Callback^ Callback]_[* WhenState]&]
[s2;%RU-RU Этот обрвыз происходит, когда 
статус виджета меняется из неизменённого 
в изменённый (`"грязный`"), или наоборот.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:WhenSel: [_^Callback^ Callback]_[* WhenSel]&]
[s2;%RU-RU Вызывается, когда изменены курсор 
или выделение.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:TextCtrl`:`:WhenViewLoading: [_^Upp`:`:Event^ Event]<[_^Upp`:`:int64^ int64]>
_[* WhenViewMapping]&]
[s2;%RU-RU Вызывается при маппинге потока 
видимой области с текущей позиции 
в потоке.&]
[s3; &]
[s4; &]
[s5;:TextCtrl`:`:CachePos`(int`): [@(0.0.255) void]_[* CachePos]([@(0.0.255) int]_[*@3 pos])&]
[s2;%RU-RU Это особая оптимизационная подсказка 
виджету, сообщающая, что следующие 
операции будут выполняться рядом 
с [%-*@3 pos]. Едва ли может использоваться 
в клиентском коде.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:CacheLinePos`(int`): [@(0.0.255) void]_[* CacheLinePos]([@(0.0.255) int]_[*@3 l
inei])&]
[s2;%RU-RU Это особая оптимизационная подсказка 
виджету, сообщающая, что следующие 
операции будут выполняться рядом 
со строкой [%-*@3 linei]. Едва ли может использовать
ся в клиентском коде.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:Load`(Stream`&`,byte`): [@(0.0.255) int]_[* Load]([_^Stream^ Stream][@(0.0.255) `&
]_[*@3 s], [_^byte^ byte]_[*@3 charset]_`=_CHARSET`_DEFAULT)&]
[s2;%RU-RU Загружает текст из потока с определённым
 набсимом [%-*@3 charset]. Функция возвращает 
обнаруженный ею режим окончания строк 
`- LE`_CRLF, LE`_LF или LE`_DEFAUL, если в файле 
окончания строк не обнаружены. Если 
файл превышает ограничение, установленное 
посредством MaxLength,то редактор устанавливает
ся в режим `'обрезанный`' и только`-чтение.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:IsTruncated`(`)const: [@(0.0.255) bool]_[* IsTruncated]()_[@(0.0.255) cons
t]&]
[s2;%RU-RU Указывает на то, что последняя 
загрузка (Load) вынуждена была выполнить 
обрезку, так как превышено ограничение 
максимальной длины файла (MaxLength).&]
[s3; &]
[s4; &]
[s5;:TextCtrl`:`:Save`(Stream`&`,byte`,int`)const: [@(0.0.255) void]_[* Save]([_^Stream^ St
ream][@(0.0.255) `&]_[*@3 s], [_^byte^ byte]_[*@3 charset]_`=_CHARSET`_DEFAULT, 
[@(0.0.255) int]_[*@3 line`_endings]_`=_LE`_DEFAULT)_[@(0.0.255) const]&]
[s2;%RU-RU Сохраняет текст в поток, с определённым 
набором символов [%-*@3 charset]. Символы, 
которые не могут быть представлены 
в предложенном набсиме [%-*@3 charset], сохраняются 
как `'?`'. Параметр [%-*@3 line`_endings] устанавливает 
режим окончания строк. LE`_DEFAULT использует 
специфичные для платформы окончания 
строк (CRLF на Windows, LF на POSIX), LE`_CRLF устанавливает
 окончания строк CRLF, LE`_LF устанавливает 
окончание строки LF). Если IsTruncated равно 
true, Save блокируется.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:GetInvalidCharPos`(byte`)const: [@(0.0.255) int]_[* GetInvalidCharPos]([_^byte^ b
yte]_[*@3 charset]_`=_CHARSET`_DEFAULT)_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает позицию первого символа, 
который не может быть представлен 
в набсиме [%-*@3 charset]. Если таковой отсутствует, 
возвращает отрицательное число.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:CheckCharset`(byte`)const: [@(0.0.255) bool]_[* CheckCharset]([_^byte^ byt
e]_[*@3 charset]_`=_CHARSET`_DEFAULT)_[@(0.0.255) const]&]
[s2; [%RU-RU То же, что и GetInvalidCharPos(]charset) < 0.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:Set`(const WString`&`): [@(0.0.255) void]_[* Set]([@(0.0.255) const]_[_^WString^ W
String][@(0.0.255) `&]_[*@3 s])&]
[s5;:TextCtrl`:`:Set`(const String`&`,byte`): [@(0.0.255) void]_[* Set]([@(0.0.255) const]_
[_^String^ String][@(0.0.255) `&]_[*@3 s], [_^byte^ byte]_[*@3 charset]_`=_CHARSET`_DEFAULT
)&]
[s2;%RU-RU Устанавливает текст виджета.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:Get`(byte`)const: [_^String^ String]_[* Get]([_^byte^ byte]_[*@3 charset]_`=
_CHARSET`_DEFAULT)_[@(0.0.255) const]&]
[s2;%RU-RU Выдаёт текст виджета.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:TextCtrl`:`:Get`(Upp`:`:int64`,int`,Upp`:`:byte`)const: [_^Upp`:`:String^ S
tring]_[* Get]([_^Upp`:`:int64^ int64]_[*@3 pos], [@(0.0.255) int]_[*@3 size], 
[_^Upp`:`:byte^ byte]_[*@3 charset]_`=_CHARSET`_DEFAULT)_[@(0.0.255) const]&]
[s2;%RU-RU Выдаёт часть текста виджета.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:TextCtrl`:`:GetW`(Upp`:`:int64`,int`)const: [_^Upp`:`:WString^ WString]_[* Ge
tW]([_^Upp`:`:int64^ int64]_[*@3 pos], [@(0.0.255) int]_[*@3 size])_[@(0.0.255) const]&]
[s2;%RU-RU Выдаёт часть текста виджета в 
UNICODE.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:GetW`(`)const: [_^WString^ WString]_[* GetW]()_[@(0.0.255) const]&]
[s2;%RU-RU Выдаёт текст виджета в UNICODE.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:ClearDirty`(`): [@(0.0.255) void]_[* ClearDirty]()&]
[s2;%RU-RU Удаляет флаг `"грязный`" (dirty). Помните, 
что разница между флагами `"dirty`" и 
`"modified`" интерфейса Ctrl состоит в том, 
что флаг `"dirty`" может убираться операцией 
undo, а текст восстанавливается в исходное 
состояние (one after setting it or at ClearDirty).&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:IsDirty`(`)const: [@(0.0.255) bool]_[* IsDirty]()_[@(0.0.255) const]&]
[s2;%RU-RU Проверяет, отличается ли текст 
от состояния `"original`" (one after setting it or 
at ClearDirty).&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:Clear`(`): [@(0.0.255) void]_[* Clear]()&]
[s2;%RU-RU Удаляет текст.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:TextCtrl`:`:GetLine`(Upp`:`:int64`)const: [@(0.0.255) int]_[* GetLine]([_^Upp`:`:int64^ i
nt64]_[*@3 pos])_[@(0.0.255) const]&]
[s2;%RU-RU Подобен GetLinePos, но параметр [%-*@3 pos] 
не меняется.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:GetLinePos`(int`&`)const: [@(0.0.255) int]_[* GetLinePos]([@(0.0.255) int`&
]_[*@3 pos])_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает строку, в которой находится 
символ со смещением [%-*@3 pos]; [%-*@3 pos] меняется 
на его позицию в строке.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:GetPos`(int`,int`)const: [@(0.0.255) int]_[* GetPos]([@(0.0.255) int]_[*@3 l
ine], [@(0.0.255) int]_[*@3 column])_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает смещение символа на 
строке [%-*@3 line] и индекс колонки [%-*@3 column] 
в этой строке.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:GetPos`(int`)const: [@(0.0.255) int]_[* GetPos]([@(0.0.255) int]_[*@3 line])
_[@(0.0.255) const]&]
[s2;%RU-RU То же, что и GetPos([%-*@3 line], 0).&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:GetUtf8Line`(int`)const: [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[* GetUtf8Line]([@(0.0.255) int]_[*@3 i])_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает строку [%-*@3 i] в кодировке 
UTF`-8.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:GetWLine`(int`)const: [_^WString^ WString]_[* GetWLine]([@(0.0.255) int]_[*@3 i
])_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает строку [%-*@3 i] в UNICODE.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:GetEncodedLine`(int`,byte`)const: [_^String^ String]_[* GetEncodedLine](
[@(0.0.255) int]_[*@3 i], [_^byte^ byte]_[*@3 charset]_`=_CHARSET`_DEFAULT)_[@(0.0.255) con
st]&]
[s2;%RU-RU Возвращает строку [%-*@3 i] в требуемой 
кодировке.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:GetLineLength`(int`)const: [@(0.0.255) int]_[* GetLineLength]([@(0.0.255) i
nt]_[*@3 i])_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает длину строки [%-*@3 i].&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:GetLineCount`(`)const: [@(0.0.255) int]_[* GetLineCount]()_[@(0.0.255) con
st]&]
[s2;%RU-RU Возвращает число строк.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:TextCtrl`:`:GetChar`(Upp`:`:int64`)const: [@(0.0.255) int]_[* GetChar]([_^Upp`:`:int64^ i
nt64]_[*@3 pos])_[@(0.0.255) const]&]
[s5;:Upp`:`:TextCtrl`:`:operator`[`]`(Upp`:`:int64`)const: [@(0.0.255) int]_[* operator`[
`]]([_^Upp`:`:int64^ int64]_[*@3 pos])_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает символ UNICODE, расположенный 
по смещению [%-*@3 pos].&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:TextCtrl`:`:GetChar`(`)const: [@(0.0.255) int]_[* GetChar]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает символ UNICODE по курсору, 
или 0, если курсор расположен за последним 
символом.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:GetLength`(`)const: [@(0.0.255) int]_[* GetLength]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает общее число символов 
в тексте.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:GetCursor`(`)const: [@(0.0.255) int]_[* GetCursor]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает позицию курсора.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:TextCtrl`:`:GetCursorLine`(`)const: [@(0.0.255) int]_[* GetCursorLine]()_[@(0.0.255) c
onst]&]
[s2;%RU-RU То же, что и GetLine(GetCursor).&]
[s3; &]
[s4; &]
[s5;:Upp`:`:TextCtrl`:`:SetSelection`(Upp`:`:int64`,Upp`:`:int64`): [@(0.0.255) void]_[* S
etSelection]([_^Upp`:`:int64^ int64]_[*@3 anchor]_`=_[@3 0], [_^Upp`:`:int64^ int64]_[*@3 c
ursor]_`=_[@3 2147483647]_)&]
[s2;%RU-RU Устанавливает выделение. Если 
[%-*@3 anchor] или [%-*@3 cursor] вне диапазона, они 
`"фиксируются`". Если они равны, то 
метод изменяет позицию курсора.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:IsSelection`(`)const: [@(0.0.255) bool]_[* IsSelection]()_[@(0.0.255) cons
t]&]
[s2;%RU-RU Проверяет, имеется ли непустое 
нормальное выделение.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:IsRectSelection`(`)const: [@(0.0.255) bool]_[* IsRectSelection]()_[@(0.0.255) c
onst]&]
[s2;%RU-RU Возвращает true, если имеется прямоугольно
е выделение.&]
[s3; &]
[s4; &]
[s5;:TextCtrl`:`:IsAnySelection`(`)const: [@(0.0.255) bool]_[* IsAnySelection]()_[@(0.0.255) c
onst]&]
[s2;%RU-RU Возвращает true, если имеется прямоугольно
е или нормальное выделение.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:GetSelection`(int`&`,int`&`)const: [@(0.0.255) bool]_[* GetSelection]([@(0.0.255) i
nt`&]_[*@3 l], [@(0.0.255) int`&]_[*@3 h])_[@(0.0.255) const]&]
[s5;:Upp`:`:TextCtrl`:`:GetSelection`(Upp`:`:int64`&`,Upp`:`:int64`&`)const: [@(0.0.255) b
ool]_[* GetSelection]([_^Upp`:`:int64^ int64][@(0.0.255) `&]_[*@3 l], 
[_^Upp`:`:int64^ int64][@(0.0.255) `&]_[*@3 h])_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает нижний и верхний предел 
выделения. Версия int64 используется 
только в режиме просмотра. В режиме 
просмотра, 32`-битная версия возвращает 
false, если диапазон превышвет 2 ГБ.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:GetSelection`(byte`)const: [_^String^ String]_[* GetSelection]([_^byte^ by
te]_[*@3 charset]_`=_CHARSET`_DEFAULT)_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает выделенный текст в 
заданном набсиме [%-*@3 charset].&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:GetSelectionW`(`)const: [_^WString^ WString]_[* GetSelectionW]()_[@(0.0.255) c
onst]&]
[s2;%RU-RU Возвращает выделенный текст в 
UNICODE.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:ClearSelection`(`): [@(0.0.255) void]_[* ClearSelection]()&]
[s2;%RU-RU Отменяет выделение.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:RemoveSelection`(`): [@(0.0.255) bool]_[* RemoveSelection]()&]
[s2;%RU-RU Удаляет текст выделения.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:TextCtrl`:`:SetCursor`(Upp`:`:int64`): [@(0.0.255) void]_[* SetCursor]([_^Upp`:`:int64^ i
nt64]_[*@3 cursor])&]
[s2;%RU-RU Помещает курсор в новую позицию.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:Paste`(const WString`&`): [@(0.0.255) int]_[* Paste]([@(0.0.255) const]_[_^WString^ W
String][@(0.0.255) `&]_[*@3 text])&]
[s2;%RU-RU Вставляет текст [%-*@3 text] в текущей 
позиции курсора.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:Insert`(int`,const WString`&`): [@(0.0.255) int]_[* Insert]([@(0.0.255) in
t]_[*@3 pos], [@(0.0.255) const]_[_^WString^ WString][@(0.0.255) `&]_[*@3 txt])&]
[s2;%RU-RU Вставляет текст [%-*@3 txt] в позиции 
[%-*@3 pos].&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:Insert`(int`,const String`&`,byte`): [@(0.0.255) int]_[* Insert]([@(0.0.255) i
nt]_[*@3 pos], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 txt], 
[_^byte^ byte]_[*@3 charset]_`=_CHARSET`_DEFAULT)&]
[s2;%RU-RU Вставляет текст [%-*@3 txt], кодированный 
в набсиме [%-*@3 charset], в позиции [%-*@3 pos].&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:Insert`(int`,const char`*`): [@(0.0.255) int]_[* Insert]([@(0.0.255) int]_
[*@3 pos], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 txt])&]
[s2;%RU-RU Вставляет текст [%-*@3 txt] в позиции 
[%-*@3 pos], используя дефолтный набсим.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:Remove`(int`,int`): [@(0.0.255) void]_[* Remove]([@(0.0.255) int]_[*@3 pos],
 [@(0.0.255) int]_[*@3 size])&]
[s2;%RU-RU Удаляет текст.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:TextCtrl`:`:Append`(const Upp`:`:WString`&`): [@(0.0.255) int]_[* Append]([@(0.0.255) c
onst]_[_^Upp`:`:WString^ WString][@(0.0.255) `&]_[*@3 txt])&]
[s2;%RU-RU То же, что и Insert(GetLength(), [%-*@3 txt]).&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:TextCtrl`:`:Append`(const Upp`:`:String`&`,Upp`:`:byte`): [@(0.0.255) int]_
[* Append]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 txt], 
[_^Upp`:`:byte^ byte]_[*@3 charset]_`=_CHARSET`_DEFAULT)&]
[s2;%RU-RU То же, что и Insert(GetLength(), [%-*@3 txt], [%-*@3 charset]).&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:TextCtrl`:`:Append`(const char`*`): [@(0.0.255) int]_[* Append]([@(0.0.255) con
st]_[@(0.0.255) char]_`*[*@3 txt])&]
[s2;%RU-RU То же, что и Append(WString([%-*@3 txt])).&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:NextUndo`(`): [@(0.0.255) void]_[* NextUndo]()&]
[s2;%RU-RU Метит начало следующего блока 
undo. Все изменяющие текст операции 
(вставки и удаления) записываются 
в буфер undo посекционно, причём каждая 
секция представляет собой единичное 
действие пользователя. NextUndo сообщает 
TextCtrl`'у, что следующая операция изменения 
относится к новой секции. Заметьте, 
что NextUndo не создаёт новую секцию; 
дважды вызвать её без операции изменения, 
равносильно вызову её единожды.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:Undo`(`): [@(0.0.255) void]_[* Undo]()&]
[s2;%RU-RU Выполняет undo (`"Отменить`").&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:Redo`(`): [@(0.0.255) void]_[* Redo]()&]
[s2;%RU-RU Выполняет redo (`"Восстановить`").&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:IsUndo`(`)const: [@(0.0.255) bool]_[* IsUndo]()_[@(0.0.255) const]&]
[s2;%RU-RU Проверяет на наличие каких`-либо 
записей undo.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:IsRedo`(`)const: [@(0.0.255) bool]_[* IsRedo]()_[@(0.0.255) const]&]
[s2;%RU-RU Проверяет на наличие каких`-либо 
записей redo.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:ClearUndo`(`): [@(0.0.255) void]_[* ClearUndo]()&]
[s2;%RU-RU Удаляет все записи undo и redo.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:ClearRedo`(`): [@(0.0.255) void]_[* ClearRedo]()&]
[s2;%RU-RU Удаляет записи redo.&]
[s3; &]
[s4; &]
[s5;:TextCtrl`:`:PickUndoData`(`): [_^TextCtrl`:`:UndoData^ UndoData]_[* PickUndoData]()&]
[s2;%RU-RU Пикует запись undo и redo. Используется, 
когда один виджет применяется для 
редактирования разных текстов,что 
позволяет изменять записи undo/redo records 
вместе с текстом. За ним должны следовать 
либо SetPickUndoData, либо ClearUndo, прежде чем 
выполняется какая`-либо операция 
по изменению текста.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:TextCtrl`:`:SetPickUndoData`(Upp`:`:TextCtrl`:`:UndoData`&`&`): [@(0.0.255) v
oid]_[* SetPickUndoData]([_^Upp`:`:TextCtrl`:`:UndoData^ TextCtrl`::UndoData][@(0.0.255) `&
`&]_[*@3 data])&]
[s2;%RU-RU Устанавливает записи undo/redo. [%-@3 data] 
пикуется (разрушается) при этой операции.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:Cut`(`): [@(0.0.255) void]_[* Cut]()&]
[s2;%RU-RU Выполняет Cut (вырезку выделения 
в буфер обмена).&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:Copy`(`): [@(0.0.255) void]_[* Copy]()&]
[s2;%RU-RU Выполняет Copy (копирование выделения 
в буфер обмена).&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:Paste`(`): [@(0.0.255) void]_[* Paste]()&]
[s2;%RU-RU Выполняет Paste (вставку из буфера 
обмена).&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:SelectAll`(`): [@(0.0.255) void]_[* SelectAll]()&]
[s2;%RU-RU Выделяет весь текст.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:StdBar`(Bar`&`): [@(0.0.255) void]_[* StdBar]([_^Bar^ Bar][@(0.0.255) `&]_[*@3 m
enu])&]
[s2;%RU-RU Предоставляет стандартное локальное 
меню [%-*@3 menu].&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:SetCharset`(byte`): [@(0.0.255) void]_[* SetCharset]([_^byte^ byte]_[*@3 cs])
&]
[s2;%RU-RU Устанавливает вводную символьную 
кодировку виджета. Этот набор символов 
определяет, какие символы будут вводиться 
пользователем в текст.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:GetCharset`(`)const: [_^byte^ byte]_[* GetCharset]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает значение, установленное 
посредством SetCharset.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:SetColor`(int`,Color`): [@(0.0.255) void]_[* SetColor]([@(0.0.255) int]_[*@3 i
], [_^Color^ Color]_[*@3 c])&]
[s2;%RU-RU Устанавливает один из цветов 
для отображения текста.&]
[s3;%RU-RU &]
[ {{4443:5557<288;>864; [s0; [+75 TextCtrl`::][*+75 INK`_NORMAL]]
:: [s0;%RU-RU [+75 Нормальный цвет текста.]]
:: [s0; [+75 TextCtrl`::][*+75 INK`_DISABLED]]
:: [s0;%RU-RU [+75 Цвет текста при отключенном 
виджете. ]]
:: [s0; [+75 TextCtrl`::][*+75 INK`_SELECTED]]
:: [s0;%RU-RU [+75 Цвет выделенного текста.]]
:: [s0; [+75 TextCtrl`::][*+75 PAPER`_NORMAL]]
:: [s0;%RU-RU [+75 Нормальный цвет бумаги.]]
:: [s0; [+75 TextCtrl`::][*+75 PAPER`_READONLY]]
:: [s0;%RU-RU [+75 Цвет бумаги, когда виджет только 
для чтения.]]
:: [s0; [+75 TextCtrl`::][*+75 PAPER`_SELECTED]]
:: [s0;%RU-RU [+75 Цвет бумаги при выделении.]]
:: [s0; [+75 TextCtrl`::][*+75 COLOR`_COUNT]]
:: [s0;%RU-RU [+75 Общее число индексов цвета.]]}}&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:GetColor`(int`)const: [_^Color^ Color]_[* GetColor]([@(0.0.255) int]_[*@3 i])
_[@(0.0.255) const]&]
[s2;%RU-RU Выдаёт цвет, используемый для 
отображения текста.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:TextCtrl`:`:View`(Upp`:`:Stream`&`,Upp`:`:byte`): [@(0.0.255) int]_[* View]([_^Upp`:`:Stream^ S
tream][@(0.0.255) `&]_[*@3 s], [_^Upp`:`:byte^ byte]_[*@3 charset]_`=_CHARSET`_DEFAULT)&]
[s2;%RU-RU Инициирует режим просмотра. В 
режиме просмотра, файл не загружается, 
но отображается по необходимости, 
загружаясь из потока. TextCtrl снова приобретает
 ссылку на поток [%-*@3 s], поэтому она 
должна существовать всё время, пока 
происходит отображение в TextCtrl`'е. 
TextCtrl iнаходится в режиме только`-чтение 
при операциях просмотра. Просмотр 
позволяет просматривать файлы >2 ГБ, 
следовательно у некоторых функций 
есть 64`-битные `"двойники`", работающие 
в режиме только просмотра. При просмотре 
файл нуждается в сканировании и создании 
маппинга; эта операция выполняется 
в фоновом режиме (через PostCallbacks), приложение 
может использовать SerializeViewMap для сохранения 
кэшированного мапа для определённого 
файла.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:TextCtrl`:`:WaitView`(int`,bool`): [@(0.0.255) void]_[* WaitView]([@(0.0.255) i
nt]_[*@3 line], [@(0.0.255) bool]_[*@3 progress])&]
[s2;%RU-RU Пока проводится маппинг файла 
для просмотра, этот метод гарантирует, 
что файл будет маппирован до строки 
[%-*@3 line] или EOF (КФ `- `"конца файла`"). Если 
[%-*@3 progress] равно true, то прогресс отображается.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:TextCtrl`:`:LockViewMapping`(`): [@(0.0.255) void]_[* LockViewMapping]()&]
[s2;%RU-RU Останавливает фоновый маппинг 
файла для просмотра.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:TextCtrl`:`:UnlockViewMapping`(`): [@(0.0.255) void]_[* UnlockViewMapping]()&]
[s2; Продолжает маппирование файла для 
просмотра.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:TextCtrl`:`:SerializeViewMap`(Upp`:`:Stream`&`): [@(0.0.255) void]_[* Seriali
zeViewMap]([_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 s])&]
[s2;%RU-RU Сериализует мап просмотра файла.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:TextCtrl`:`:IsView`(`)const: [@(0.0.255) bool]_[* IsView]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает true, если TextCtrl находится 
в режиме просмотра.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:TextCtrl`:`:GetViewSize`(`)const: [_^Upp`:`:int64^ int64]_[* GetViewSize]()_[@(0.0.255) c
onst]&]
[s2;%RU-RU Возвращает GetSize потока просмотра.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:TextCtrl`:`:GetLinePos64`(Upp`:`:int64`&`)const: [@(0.0.255) int]_[* GetLineP
os64]([_^Upp`:`:int64^ int64][@(0.0.255) `&]_[*@3 pos])_[@(0.0.255) const]&]
[s5;:Upp`:`:TextCtrl`:`:GetPos64`(int`,int`)const: [_^Upp`:`:int64^ int64]_[* GetPos64]([@(0.0.255) i
nt]_[*@3 ln], [@(0.0.255) int]_[*@3 lpos])_[@(0.0.255) const]&]
[s5;:Upp`:`:TextCtrl`:`:GetLength64`(`)const: [_^Upp`:`:int64^ int64]_[* GetLength64]()_[@(0.0.255) c
onst]&]
[s5;:Upp`:`:TextCtrl`:`:GetCursor64`(`)const: [_^Upp`:`:int64^ int64]_[* GetCursor64]()_[@(0.0.255) c
onst]&]
[s2;%RU-RU Это варианты GetLinePos, GetPos, GetLength 
и GetCursor64 для режима просмотра (при 
котором возможны значения > INT`_MAX).&]
[s3; &]
[s4; &]
[s5;:TextCtrl`:`:UndoSteps`(int`): [_^TextCtrl^ TextCtrl][@(0.0.255) `&]_[* UndoSteps]([@(0.0.255) i
nt]_[*@3 n])&]
[s2;%RU-RU Устанавливает максимальное число 
шагов undo.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:ProcessTab`(bool`): [_^TextCtrl^ TextCtrl][@(0.0.255) `&]_[* ProcessTab]([@(0.0.255) b
ool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%RU-RU Если активен (дефолт), виджет принимает 
нажатие клавиши табуляции K`_TAB (вставляет 
`'`\t`').&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:NoProcessTab`(`): [_^TextCtrl^ TextCtrl][@(0.0.255) `&]_[* NoProcessTab]()
&]
[s2;%RU-RU То же, что и ProcessTab(false).&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:ProcessEnter`(bool`): [_^TextCtrl^ TextCtrl][@(0.0.255) `&]_[* ProcessEnte
r]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%RU-RU Если активен (недефолт), виджет 
принимает нажатие клавиши K`_ENTER (вставляет 
новую строку).&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:NoProcessEnter`(`): [_^TextCtrl^ TextCtrl][@(0.0.255) `&]_[* NoProcessEnte
r]()&]
[s2;%RU-RU То же, что и ProcessEnter(false).&]
[s3; &]
[s4; &]
[s5;:TextCtrl`:`:NoBackground`(bool`): [_^TextCtrl^ TextCtrl][@(0.0.255) `&]_[* NoBackgroun
d]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%RU-RU Устанавливает виджет в прозрачный 
режим `- фон не отрисовывается и активируется
 Transparent `- в итоге видно всё, что отрисовано 
за виджетом, что позволяет клиентскому 
коду предоставлять любой необходимый 
фон.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:MaxLength`(int`): [_^TextCtrl^ TextCtrl][@(0.0.255) `&]_[* MaxLength]([@(0.0.255) i
nt]_[*@3 len])&]
[s2;%RU-RU Устанавливает максимальный размер 
текста в символах unicode. Должен быть 
менее 1Gchars. Дефолтно равен 400Mchars.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TextCtrl`:`:IsProcessTab`(`): [@(0.0.255) bool]_[* IsProcessTab]()&]
[s2;%RU-RU Проверяет, активен ли режим ProcessTab.&]
[s3;%RU-RU &]
[ {{10000@3 [s0; [*@(229)4 Структура TextCtrl`::UndoData]]}}&]
[s4; &]
[s1;:TextCtrl`:`:UndoData`:`:struct: [@(0.0.255)3 struct][3 _][*3 UndoData]&]
[s9;%RU-RU Эта структура содержит информацию 
о состоянии undo/redo виджета. Может быть 
с PickUndoData и SetPickUndoDate для сохранения 
и восстановления состояния undo/redo.&]
[s3;%RU-RU &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Публичных Методов]]}}&]
[s3; &]
[s5;:TextCtrl`:`:UndoData`:`:Clear`(`): [@(0.0.255) void]_[* Clear]()&]
[s2;%RU-RU Очищает состояние undo/redo&]
[s3;%RU-RU &]
[s3;%RU-RU .]]