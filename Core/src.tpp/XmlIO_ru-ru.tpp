topic "Класс XmlIO и фреймворк Xmlize";
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
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Класс XmlIO и фреймворк Xmlize]]}}&]
[s3; &]
[s1;:XmlIO`:`:class: [@(0.0.255)3 class][3 _][*3 XmlIO]&]
[s2;%RU-RU XmlIO представляет единичный XmlNode 
в фреймворке Jsonize. Он используется 
как параметр для глобальных функций 
Xmlize и XmlAttrLoad/XmlAttrStore. Эти функции имеют 
дефолтные шаблонные определения, 
которые, в свою очередь, вызывают 
одноимённые методы объекта. Однако, 
когда нельзя использовать метод в 
реализации (напр., для поддерживаемых 
примитивных типов), могут использоваться 
глобальные шаблонные специализации.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Публичных Методов]]}}&]
[s3; &]
[s5;:XmlIO`:`:IsLoading`(`)const: [@(0.0.255) bool]_[* IsLoading]()_[@(0.0.255) const]&]
[s2;%RU-RU True, если протекающая операция 
получает данные из XML в переменные 
C`+`+.&]
[s3; &]
[s4; &]
[s5;:XmlIO`:`:IsStoring`(`)const: [@(0.0.255) bool]_[* IsStoring]()_[@(0.0.255) const]&]
[s2;%RU-RU True, если протекающая операция 
сохраняет данные из переменных C`+`+ 
в XML.&]
[s3; &]
[s4; &]
[s5;:XmlIO`:`:Node`(`): [_^XmlNode^ XmlNode][@(0.0.255) `&]_[* Node]()&]
[s5;:XmlIO`:`:Node`(`)const: [@(0.0.255) const]_[_^XmlNode^ XmlNode][@(0.0.255) `&]_[* Node](
)_[@(0.0.255) const]&]
[s5;:XmlIO`:`:operator`-`>`(`): [_^XmlNode^ XmlNode]_`*[* operator`->]()&]
[s2;%RU-RU Возвращает текущий XmlNode, представленный 
этим XmlIO.&]
[s3; &]
[s4; &]
[s5;:XmlIO`:`:GetAttr`(const char`*`): [_^String^ String]_[* GetAttr]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 id])&]
[s2;%RU-RU То же, что и Node().GetAttr([%-*@3 id]) `- возвращает 
значение атрибута текущего XmlNode (используемо
е при получение данных из XML).&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:XmlIO`:`:SetAttr`(const char`*`,const String`&`): [@(0.0.255) void]_[* SetAttr]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 id], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 v
al])&]
[s2;%RU-RU То же, что и Node().SetAttr([%-*@3 id], [%-*@3 val]) 
`- устанавливает значение атрибута 
текущего XmlNode (используется при сохранении 
данных в XML).&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:XmlIO`:`:operator`(`)`(const char`*`,T`&`): [@(0.0.255) template]_<[@(0.0.255) class
]_[*@4 T]>_[_^XmlIO^ XmlIO]_[* operator()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 tag], 
[*@4 T][@(0.0.255) `&]_[*@3 var])&]
[s5;:Upp`:`:XmlIO`:`:operator`(`)`(const char`*`,T`&`,const D`&`): [@(0.0.255) template
]_<[@(0.0.255) class]_[*@4 T], [@(0.0.255) class]_[*@4 D]>_[_^Upp`:`:XmlIO^ XmlIO]_[* operato
r()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 tag], [*@4 T][@(0.0.255) `&]_[*@3 var], 
[@(0.0.255) const]_[*@4 D][@(0.0.255) `&]_[*@3 def])&]
[s2;%RU-RU Создаёт субтэг текущего тэга 
XmlNode [%-*@3 tag], формирует новый XmlIO для 
него и вызывает глобальную функцию 
[* Xmlize ]с этим новым XmlIO и [%-*@3 var] в качестве 
параметров. Общая глобальная [* Xmlize 
`- ]это шаблонная функция вызывающая 
метод [* Xmlize ]для [%-*@3 var] с новым XmlIO в качестве 
параметра `- таким образом [* Xmlize ]может 
быть определён либо как глобальная 
шаблонная функция для [%-*@4 T], либо как 
метод у [%-*@4 T] (это обычно легче, но не 
всегда возможно). Параметр [%-*@3 def] может 
использоваться для предоставления 
дефолтного значения, когда при загрузке 
нет соответствующего тэга. Вариант 
без [%-*@3 def] оставляет [%-*@3 var] неизменённой 
в этом случае.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:XmlIO`:`:List`(const char`*`,const char`*`,T`&`,const D`&`): [@(0.0.255) te
mplate]_<[@(0.0.255) class]_[*@4 T], [@(0.0.255) class]_[*@4 D]>_[_^Upp`:`:XmlIO^ XmlIO]_[* L
ist]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 tag], [@(0.0.255) const]_[@(0.0.255) char]_
`*[*@3 itemtag], [*@4 T][@(0.0.255) `&]_[*@3 var], [@(0.0.255) const]_[*@4 D][@(0.0.255) `&]_[*@3 d
ef])&]
[s5;:Upp`:`:XmlIO`:`:List`(const char`*`,const char`*`,T`&`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 T]>_[_^Upp`:`:XmlIO^ XmlIO]_[* List]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 t
ag], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 itemtag], [*@4 T][@(0.0.255) `&]_[*@3 var])&]
[s2;%RU-RU Создаёт субтэг текущего тэга 
XmlNode [%-*@3 tag], формирует новый XmlIO для 
него и вызывает глобальную функцию 
[* Xmlize ]с этим новым XmlIO, [%-*@3 itemtag] и [%-*@3 var] 
в качестве параметров. Общая глобальная 
[* Xmlize `- ]это шаблонная функция вызывающая 
метод [* Xmlize ]для [%-*@3 var] с новым XmlIO в качестве 
параметра  (но не [%-*@3 itemtag]) `- таким образом 
[* Xmlize ]может быть определён либо как 
глобальная шаблонная функция для 
[%-*@4 T], либо как метод у [%-*@4 T] (это обычно 
легче, но не всегда возможно). [%-*@3 itemtag] 
`- дополнительный параметр, который 
можно использовать как имя внедрённых 
тэгов, когда коллекции элементов 
Xml`'изируются. Параметр [%-*@3 def] может 
использоваться для предоставления 
дефолтного значения, когда при загрузке 
нет соответствующего тэга. Вариант 
без [%-*@3 def] оставляет [%-*@3 var] неизменённой 
в этом случае.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:XmlIO`:`:Var`(const char`*`,T`&`,X`): [@(0.0.255) template]_<[@(0.0.255) clas
s]_[*@4 T], [@(0.0.255) class]_[*@4 X]>_[_^Upp`:`:XmlIO^ XmlIO]_[* Var]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 tag], [*@4 T][@(0.0.255) `&]_[*@3 var], [*@4 X]_[*@3 var`_xmlize])&]
[s2;%RU-RU Создаёт субтэг текущего тэга 
XmlNode [%-*@3 tag], формирует новый XmlIO для 
него и вызывает глобальный [%-*@3 item`_xmlize]c, 
с этим новым XmlIO и [%-*@3 var] в качестве 
параметров. Это позволяет на месте 
определять структуру XML, используемую 
для представления [%-*@3 var].&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:XmlIO`:`:Array`(const char`*`,T`&`,X`,const char`*`): [@(0.0.255) template]_
<[@(0.0.255) class]_[*@4 T], [@(0.0.255) class]_[*@4 X]>_[_^Upp`:`:XmlIO^ XmlIO]_[* Array]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 tag], [*@4 T][@(0.0.255) `&]_[*@3 var], 
[*@4 X]_[*@3 item`_xmlize], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 itemtag] 
`=[*@3  `"item`"])&]
[s2;%RU-RU Xml`'изирует контейнер массива 
(должен иметь методы GetCount, Add и оператор 
`[`]), используя [%-*@3 item`_xmlize] для определения 
структуры элементов. [%-*@3 itemtag] предоставляет 
тэги индивидуальных элементов.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:XmlIO`:`:Attr`(const char`*`,T`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>
_[_^XmlIO^ XmlIO]_[* Attr]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], 
[*@4 T][@(0.0.255) `&]_[*@3 var])&]
[s5;:XmlIO`:`:Attr`(const char`*`,T`&`,T`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T
], [@(0.0.255) class]_[*@4 D]>_[_^Upp`:`:XmlIO^ XmlIO]_[* Attr]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 id], [*@4 T][@(0.0.255) `&]_[*@3 var], [@(0.0.255) const]_[*@4 D][@(0.0.255) `&]_[*@3 d
ef])&]
[s2;%RU-RU При подучении данных, вызывает 
глобальную функцию [@(0.0.255) void ][* XmlAttrLoad]([%-*@4 T][%-@(0.0.255) `&
][%- _][%-*@3 var], const String`& [* text]), где [* text] устанавливаетс
я в значение атрибута [%-*@3 id] текущего 
XmlNode. Общий глобальный Xmlize `- это шаблонная 
функция, которая вызывает метод [@(0.0.255) void 
][* XmlAttrLoad](const String`& [* text]) для [%-*@3 var]. При 
сохранении данных, устанавливает 
атрибут [%-*@3 id] текущего XmlNode в итог 
вызова глобальной функции String [* XmlAttrStore]([@(0.0.255) c
onst ][%-*@4 T]`& var).Параметр  [%-*@3 def] может использоват
ься для предоставления дефолтного 
значения при получении данных из 
XML. Возвращает `*this.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:XmlIO`:`:At`(int`): [_^XmlIO^ XmlIO]_[* At]([@(0.0.255) int]_[*@3 i])&]
[s2;%RU-RU Создаёт новый подузел с индексом 
[%-*@3 i] текущего XmlNode и возвращает новый 
XmlIO для него. Тип узла и свойства устанавлива
ются через созданный XmlIO.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:XmlIO`:`:Add`(`): [_^XmlIO^ XmlIO]_[* Add]()&]
[s2;%RU-RU Добавляет новый подузел к текущему 
XmlNode и возвращает новый XmlIO для него. 
Тип узла и свойства устанавливаются 
через созданный XmlIO.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:XmlIO`:`:Add`(const char`*`): [_^XmlIO^ XmlIO]_[* Add]([@(0.0.255) const]_[@(0.0.255) ch
ar]_`*[*@3 id])&]
[s2;%RU-RU Добавляет новый субтэг [%-*@3 id] к 
текущему XmlNode и возвращает XmlIO для 
него. Свойства узла устанавливаются 
через созданный XmlIO.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:XmlIO`:`:GetAdd`(const char`*`): [_^XmlIO^ XmlIO]_[* GetAdd]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 id])&]
[s2;%RU-RU Если уже есть субтэг [%-*@3 id] в текущем 
XmlNode, возвращает XmlIO для него, Если 
нет, он создаётся и возвращается XmlIO 
для этого нового тэга. Свойства узла 
устанавливаются через возвращённый 
XmlIO.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:XmlIO`:`:SetUserData`(const Value`&`): [@(0.0.255) void]_[* SetUserData]([@(0.0.255) c
onst]_[_^Value^ Value][@(0.0.255) `&]_[*@3 v])&]
[s2;%RU-RU Ассоциирует некоторое значение 
кода`-клиента с текущим XmlIO. Это значение 
прередаётся любому новому XmlIO, созданному 
текущим XmlIO.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:XmlIO`:`:GetUserData`(`)const: [_^Value^ Value]_[* GetUserData]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает значение, установленное 
SetUserData.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:XmlIO`:`:XmlIO`(XmlNode`&`,bool`,const Value`&`): [* XmlIO]([_^XmlNode^ XmlNode][@(0.0.255) `&
]_[*@3 xml], [@(0.0.255) bool]_[*@3 loading], [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&
]_[*@3 userdata])&]
[s5;:XmlIO`:`:XmlIO`(XmlNode`&`,bool`): [* XmlIO]([_^XmlNode^ XmlNode][@(0.0.255) `&]_[*@3 xm
l], [@(0.0.255) bool]_[*@3 loading])&]
[s2;%RU-RU Создаёт XmlIO для данного XmlNode и 
устанавливает режим направления, 
на основе [%-*@3 loading]. Если присутствует 
[%-*@3 userdata], вызывает SetUserData([%-*@3 userdata]).&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:XmlIO`:`:XmlIO`(XmlIO`,const char`*`): [* XmlIO]([* XmlIO]_[*@3 xml], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id])&]
[s2;%RU-RU Если уже существует субтэг [%-*@3 id] 
в текущем XmlNode, то конструирует XmlIO 
для него, если нет, то тэг создаётся 
и конструируется XmlIO для этого нового 
тэга.&]
[s0; &]
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Функции Xmlize Store/Load]]}}&]
[s3; &]
[s5;:StoreAsXML`(const T`&`,const char`*`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T
]>_[_^String^ String]_[* StoreAsXML]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 data], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 name]_`=_NULL)&]
[s2;%RU-RU Сохраняет данные [%-*@3 data] в строке 
XML. Высокоуровневый тэг определяется 
[%-*@3 name], если NULL, устанавливается как 
`"app`" по умолчанию.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:LoadFromXML`(T`&`,const String`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>
_[@(0.0.255) bool]_[* LoadFromXML]([*@4 T][@(0.0.255) `&]_[*@3 data], [@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 xml])&]
[s2;%RU-RU Загружает данные [%-*@3 data] из строки 
XML [%-*@3 xml]. В случае ошибки или пустого 
[%-*@3 xml] возвращает false.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TryLoadFromXML`(T`&`,const String`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T
]>_[@(0.0.255) bool]_[* TryLoadFromXML]([*@4 T][@(0.0.255) `&]_[*@3 data], 
[@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 xml])&]
[s2;%RU-RU Загружает данные [%-*@3 data] из строки 
XML [%-*@3 xml]. Если [%-*@3 xml] пустая, возвращает 
false. В случае ошибки, выводит XmlError.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:StoreAsXMLFile`(T`&`,const char`*`,const char`*`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 T]>_[@(0.0.255) bool]_[* StoreAsXMLFile]([*@4 T][@(0.0.255) `&]_[*@3 data], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 name]_`=_NULL, [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 file]_`=_NULL)&]
[s2;%RU-RU Сохраняет данные [%-*@3 data] в файл 
XML, используя [%-*@3 name] (`"app`", если NULL) как 
верхнеуровневый тэг и [%-*@3 file] в качестве 
пути к файлу. Если [%-*@3 file] равен NULL, 
в качестве пути к файлу используется 
ConfigFile(GetExeTitle() `+ `".xml`").&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:LoadFromXMLFile`(T`&`,const char`*`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T
]>_[@(0.0.255) bool]_[* LoadFromXMLFile]([*@4 T][@(0.0.255) `&]_[*@3 data], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 file]_`=_NULL)&]
[s2;%RU-RU Загружает данные [%-*@3 data] из файла 
XML, используя [%-*@3 file] в качестве пути 
к файлу. Если [%-*@3 file] равен NULL, в качестве 
пути к файлу используется ConfigFile(GetExeTitle() 
`+ `".xml`"). В случае ошибки или пустого 
[%-*@3 xml] возвращает false.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TryLoadFromXMLFile`(T`&`,const char`*`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T
]>_[@(0.0.255) bool]_[* TryLoadFromXMLFile]([*@4 T][@(0.0.255) `&]_[*@3 data], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 file]_`=_NULL)&]
[s2;%RU-RU Загружает данные [%-*@3 data] из файла 
XML, используя [%-*@3 file] в качестве пути 
к файлу. Если [%-*@3 file] равен NULL, в качестве 
пути к файлу используется ConfigFile(GetExeTitle() 
`+ `".xml`"). В случае пустого [%-*@3 xml] возвращает 
false. В случае ошибки, выводит XmlError.&]
[s3;%RU-RU &]
[s0; &]
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Стандартные шаблонные специализации 
Xmlize]]}}&]
[s3; &]
[s5;:XmlAttrLoad`(String`&`,const String`&`): [@(0.0.255) template]_<>_[@(0.0.255) void]_
[* XmlAttrLoad]([_^String^ String][@(0.0.255) `&]_[*@3 var], [@(0.0.255) const]_[_^String^ St
ring][@(0.0.255) `&]_[*@3 text])&]
[s5;:XmlAttrStore`(const String`&`): [@(0.0.255) template]_<>_[_^String^ String]_[* XmlAttr
Store]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 var])&]
[s5;:XmlAttrLoad`(WString`&`,const String`&`): [@(0.0.255) template]_<>_[@(0.0.255) void]_
[* XmlAttrLoad]([_^WString^ WString][@(0.0.255) `&]_[*@3 var], [@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 text])&]
[s5;:XmlAttrStore`(const WString`&`): [@(0.0.255) template]_<>_[_^String^ String]_[* XmlAtt
rStore]([@(0.0.255) const]_[_^WString^ WString][@(0.0.255) `&]_[*@3 var])&]
[s5;:XmlAttrLoad`(int`&`,const String`&`): [@(0.0.255) template]_<>_[@(0.0.255) void]_[* Xm
lAttrLoad]([@(0.0.255) int`&]_[*@3 var], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[*@3 text])&]
[s5;:XmlAttrStore`(const int`&`): [@(0.0.255) template]_<>_[_^String^ String]_[* XmlAttrSto
re]([@(0.0.255) const]_[@(0.0.255) int`&]_[*@3 var])&]
[s5;:XmlAttrLoad`(dword`&`,const String`&`): [@(0.0.255) template]_<>_[@(0.0.255) void]_[* X
mlAttrLoad]([_^dword^ dword][@(0.0.255) `&]_[*@3 var], [@(0.0.255) const]_[_^String^ String
][@(0.0.255) `&]_[*@3 text])&]
[s5;:XmlAttrStore`(const dword`&`): [@(0.0.255) template]_<>_[_^String^ String]_[* XmlAttrS
tore]([@(0.0.255) const]_[_^dword^ dword][@(0.0.255) `&]_[*@3 var])&]
[s5;:XmlAttrLoad`(double`&`,const String`&`): [@(0.0.255) template]_<>_[@(0.0.255) void]_
[* XmlAttrLoad]([@(0.0.255) double`&]_[*@3 var], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[*@3 text])&]
[s5;:XmlAttrStore`(const double`&`): [@(0.0.255) template]_<>_[_^String^ String]_[* XmlAttr
Store]([@(0.0.255) const]_[@(0.0.255) double`&]_[*@3 var])&]
[s5;:XmlAttrLoad`(bool`&`,const String`&`): [@(0.0.255) template]_<>_[@(0.0.255) void]_[* X
mlAttrLoad]([@(0.0.255) bool`&]_[*@3 var], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[*@3 text])&]
[s5;:XmlAttrStore`(const bool`&`): [@(0.0.255) template]_<>_[_^String^ String]_[* XmlAttrSt
ore]([@(0.0.255) const]_[@(0.0.255) bool`&]_[*@3 var])&]
[s5;:XmlAttrStore`(const bool`&`): [@(0.0.255) template]_<>_[@(0.0.255) void]_[* XmlAttrLoa
d]([_^int16^ int16][@(0.0.255) `&]_[*@3 var], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[*@3 text])&]
[s5;:XmlAttrStore`(const int16`&`): [@(0.0.255) template]_<>_[_^String^ String]_[* XmlAttrS
tore]([@(0.0.255) const]_[_^int16^ int16][@(0.0.255) `&]_[*@3 var])&]
[s5;:XmlAttrLoad`(int64`&`,const String`&`): [@(0.0.255) template]_<>_[@(0.0.255) void]_[* X
mlAttrLoad]([_^int64^ int64][@(0.0.255) `&]_[*@3 var], [@(0.0.255) const]_[_^String^ String
][@(0.0.255) `&]_[*@3 text])&]
[s5;:XmlAttrStore`(const int64`&`): [@(0.0.255) template]_<>_[_^String^ String]_[* XmlAttrS
tore]([@(0.0.255) const]_[_^int64^ int64][@(0.0.255) `&]_[*@3 var])&]
[s5;:XmlAttrLoad`(byte`&`,const String`&`): [@(0.0.255) template]_<>_[@(0.0.255) void]_[* X
mlAttrLoad]([_^byte^ byte][@(0.0.255) `&]_[*@3 var], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[*@3 text])&]
[s5;:XmlAttrStore`(const byte`&`): [@(0.0.255) template]_<>_[_^String^ String]_[* XmlAttrSt
ore]([@(0.0.255) const]_[_^byte^ byte][@(0.0.255) `&]_[*@3 var])&]
[s5;:XmlAttrLoad`(Date`&`,const String`&`): [@(0.0.255) template]_<>_[@(0.0.255) void]_[* X
mlAttrLoad]([_^Date^ Date][@(0.0.255) `&]_[*@3 var], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[*@3 text])&]
[s5;:XmlAttrStore`(const Date`&`): [@(0.0.255) template]_<>_[_^String^ String]_[* XmlAttrSt
ore]([@(0.0.255) const]_[_^Date^ Date][@(0.0.255) `&]_[*@3 var])&]
[s5;:XmlAttrLoad`(Time`&`,const String`&`): [@(0.0.255) template]_<>_[@(0.0.255) void]_[* X
mlAttrLoad]([_^Time^ Time][@(0.0.255) `&]_[*@3 var], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[*@3 text])&]
[s5;:XmlAttrStore`(const Time`&`): [@(0.0.255) template]_<>_[_^String^ String]_[* XmlAttrSt
ore]([@(0.0.255) const]_[_^Time^ Time][@(0.0.255) `&]_[*@3 var])&]
[s2; Предоставляет шаблонную специализацию 
для поддержки типов данных [*@3 var] в 
качестве атрибутов XML.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Xmlize`(XmlIO`&`,String`&`): [@(0.0.255) template]_<>_[@(0.0.255) void]_[* Xmlize]([_^XmlIO^ X
mlIO][@(0.0.255) `&]_[*@3 xml], [_^String^ String][@(0.0.255) `&]_[*@3 var])&]
[s5;:Xmlize`(XmlIO`&`,WString`&`): [@(0.0.255) template]_<>_[@(0.0.255) void]_[* Xmlize]([_^XmlIO^ X
mlIO][@(0.0.255) `&]_[*@3 xml], [_^WString^ WString][@(0.0.255) `&]_[*@3 var])&]
[s5;:Xmlize`(XmlIO`&`,int`&`): [@(0.0.255) template]_<>_[@(0.0.255) void]_[* Xmlize]([_^XmlIO^ X
mlIO][@(0.0.255) `&]_[*@3 xml], [@(0.0.255) int`&]_[*@3 var])&]
[s5;:Xmlize`(XmlIO`&`,dword`&`): [@(0.0.255) template]_<>_[@(0.0.255) void]_[* Xmlize]([_^XmlIO^ X
mlIO][@(0.0.255) `&]_[*@3 xml], [_^dword^ dword][@(0.0.255) `&]_[*@3 var])&]
[s5;:Xmlize`(XmlIO`&`,double`&`): [@(0.0.255) template]_<>_[@(0.0.255) void]_[* Xmlize]([_^XmlIO^ X
mlIO][@(0.0.255) `&]_[*@3 xml], [@(0.0.255) double`&]_[*@3 var])&]
[s5;:Xmlize`(XmlIO`&`,bool`&`): [@(0.0.255) template]_<>_[@(0.0.255) void]_[* Xmlize]([_^XmlIO^ X
mlIO][@(0.0.255) `&]_[*@3 xml], [@(0.0.255) bool`&]_[*@3 var])&]
[s5;:Xmlize`(XmlIO`&`,Date`&`): [@(0.0.255) template]_<>_[@(0.0.255) void]_[* Xmlize]([_^XmlIO^ X
mlIO][@(0.0.255) `&]_[*@3 xml], [_^Date^ Date][@(0.0.255) `&]_[*@3 var])&]
[s5;:Xmlize`(XmlIO`&`,Time`&`): [@(0.0.255) template]_<>_[@(0.0.255) void]_[* Xmlize]([_^XmlIO^ X
mlIO][@(0.0.255) `&]_[*@3 xml], [_^Time^ Time][@(0.0.255) `&]_[*@3 var])&]
[s5;:Xmlize`(XmlIO`&`,int16`&`): [@(0.0.255) template]_<>_[@(0.0.255) void]_[* Xmlize]([_^XmlIO^ X
mlIO][@(0.0.255) `&]_[*@3 xml], [_^int16^ int16][@(0.0.255) `&]_[*@3 var])&]
[s5;:Xmlize`(XmlIO`&`,int64`&`): [@(0.0.255) template]_<>_[@(0.0.255) void]_[* Xmlize]([_^XmlIO^ X
mlIO][@(0.0.255) `&]_[*@3 xml], [_^int64^ int64][@(0.0.255) `&]_[*@3 var])&]
[s5;:Xmlize`(XmlIO`&`,byte`&`): [@(0.0.255) template]_<>_[@(0.0.255) void]_[* Xmlize]([_^XmlIO^ X
mlIO][@(0.0.255) `&]_[*@3 xml], [_^byte^ byte][@(0.0.255) `&]_[*@3 var])&]
[s2; Предоставляет шаблонную спецификацию 
для поддержки Xml`'изации типов данных 
[*@3 var].&]
[s3;%RU-RU &]
[s0;%RU-RU &]
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Особые варианты Xmlize]]}}&]
[s3; &]
[s5;:XmlizeLangAttr`(XmlIO`&`,int`&`,const char`*`): [@(0.0.255) void]_[* XmlizeLangAttr](
[_^XmlIO^ XmlIO][@(0.0.255) `&]_[*@3 xml], [@(0.0.255) int`&]_[*@3 lang], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id]_`=_`"[*@3 lang]`")&]
[s2;%RU-RU Xml`'изует номер идентификатора 
языка [%-*@3 lang] (который кодирует строки 
i18n типа `"CS`-CZ`", см. Lang.h) как атрибут 
[%-*@3 id], используя более читаемый формат 
(`"CS`-CZ`") вместо простого целого числа.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:XmlizeLang`(XmlIO`&`,const char`*`,int`&`,const char`*`): [@(0.0.255) void]_[* Xmliz
eLang]([_^XmlIO^ XmlIO][@(0.0.255) `&]_[*@3 xml], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 t
ag], [@(0.0.255) int`&]_[*@3 lang], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id]_`=_`"[*@3 i
d]`")&]
[s2;%RU-RU Xml`'изует номер идентификатора 
языка [%-*@3 lang] (который кодирует строки 
i18n типа `"CS`-CZ`", см. Lang.h) как тэг XML [%-*@3 tag], 
используя более читаемый формат (`"CS`-CZ`") 
вместо простого целого числа.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:XmlizeBySerialize`(XmlIO`&`,T`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_
[@(0.0.255) void]_[* XmlizeBySerialize]([_^XmlIO^ XmlIO][@(0.0.255) `&]_[*@3 xio], 
[*@4 T][@(0.0.255) `&]_[*@3 x])&]
[s2;%RU-RU Эта функция кодирует/получает 
XML двоичной сериализацией объекта 
и Json`'изацией 16`-ричной строки данных.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:XmlizeByJsonize`(XmlIO`&`,T`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_[@(0.0.255) v
oid]_[* XmlizeByJsonize]([_^XmlIO^ XmlIO][@(0.0.255) `&]_[*@3 xio], [*@4 T][@(0.0.255) `&]_[*@3 x
])&]
[s2;%RU-RU Эта функция кодирует/получает 
XML преобразованием в JSON и дальнейшей 
интерпретацией полей объекта JSON как 
тэгов XML.&]
[s3;%RU-RU ]]