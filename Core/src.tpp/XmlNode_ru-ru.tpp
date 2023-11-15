topic "Класс XmlNode";
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
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Класс XmlNode `- Иерархическое 
представление XML]]}}&]
[s3; &]
[s1;:XmlNode`:`:class: [@(0.0.255)3 class][3 _][*3 XmlNode]&]
[s9;%RU-RU Этот класс представляет весь 
документ XML в форме рекурсивной иерархическо
й структуры `- каждый XmlNode может содержать 
любое число XmlNodes`-отпрысков. XmlNode является 
mutable, и U`+`+ предоставляет функцию для 
разбора XML и сохраняет его в XmlNode, а 
также для того, чтобы брать XmlNode и 
создавать соответствующий документ 
XML.&]
[s9;%RU-RU XmlNode типа [*/^topic`:`/`/Core`/srcdoc`/Moveable`_ru`-ru^ moveable][*/  
]с семантикой трансфера [*/^topic`:`/`/Core`/srcdoc`/pick`_`_ru`-ru^ p
ick and optional deep copy]&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Публичных Членов]]}}&]
[s3; &]
[s5;:XmlNode`:`:Void`(`): [@(0.0.255) static] [@(0.0.255) const]_[_^XmlNode^ XmlNode][@(0.0.255) `&
]_[* Void]()&]
[s2;%RU-RU Возвращает константную ссылку 
на XmlNode, представляющий `"ничто`". Эта 
ссылка используется как возвратное 
значение в тех случаях, когда отсутствуют 
требуемые элементы.&]
[s3; &]
[s4; &]
[s5;:XmlNode`:`:IsVoid`(`)const: [@(0.0.255) bool]_[* IsVoid]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает true, если XmlNode является 
Void.&]
[s3; &]
[s4; &]
[s5;:XmlNode`:`:GetType`(`)const: [@(0.0.255) int]_[* GetType]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает тип узла. Возможные 
типы: XML`_PI, XML`_DECL, XML`_COMMENT, XML`_DOC, XML`_TEXT, 
XML`_TAG.&]
[s3; &]
[s4; &]
[s5;:XmlNode`:`:GetText`(`)const: [_^String^ String]_[* GetText]()_[@(0.0.255) const]&]
[s5;:XmlNode`:`:GetTag`(`)const: [_^String^ String]_[* GetTag]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает текст, ассоциированный 
с узлом. Если узел XML`_TAG, то текст является 
идентификатором тэга. Если узел XML`_TEXT, 
то текст ... текст. В иных случаях, типа 
XML`_PI, текст является копией контента 
элемента.&]
[s3; &]
[s4; &]
[s5;:XmlNode`:`:IsTag`(`)const: [@(0.0.255) bool]_[* IsTag]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает true, если узел XML`_TAG.&]
[s3; &]
[s4; &]
[s5;:XmlNode`:`:IsTag`(const char`*`)const: [@(0.0.255) bool]_[* IsTag]([@(0.0.255) const]_
[@(0.0.255) char]_`*[*@3 tag])_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает true, если узел XML`_TAG 
и равен [%-*@3 tag].&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:XmlNode`:`:IsText`(`)const: [@(0.0.255) bool]_[* IsText]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает true, если узел текст.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:XmlNode`:`:Clear`(`): [@(0.0.255) void]_[* Clear]()&]
[s2;%RU-RU Очищает весь контент.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:XmlNode`:`:CreateTag`(const char`*`): [@(0.0.255) void]_[* CreateTag]([@(0.0.255) cons
t]_[@(0.0.255) char]_`*[*@3 tag])&]
[s2;%RU-RU Устанавливает этот XmlNode на представление
 тэга XML с идентификатором [%-*@3 tag].&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:XmlNode`:`:CreateText`(const String`&`): [@(0.0.255) void]_[* CreateText]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[*@3 txt])&]
[s2;%RU-RU Устанавливает этот XmlNode на представление
 текста с контентом [%-*@3 txt].&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:XmlNode`:`:CreatePI`(const String`&`): [@(0.0.255) void]_[* CreatePI]([@(0.0.255) cons
t]_[_^String^ String][@(0.0.255) `&]_[*@3 pi])&]
[s2;%RU-RU Устанавливает этот XmlNode на представление
 информации об обработке [%-*@3 pi].&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:XmlNode`:`:CreateDecl`(const String`&`): [@(0.0.255) void]_[* CreateDecl]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[*@3 decl])&]
[s2;%RU-RU Устанавливает этот XmlNode на представление
 декларации XML [%-*@3 decl].&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:XmlNode`:`:CreateComment`(const String`&`): [@(0.0.255) void]_[* CreateComment]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[*@3 comment])&]
[s2;%RU-RU Устанавливает этот XmlNode на представление
 комментария XML [%-*@3 comment].&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:XmlNode`:`:CreateDocument`(`): [@(0.0.255) void]_[* CreateDocument]()&]
[s2;%RU-RU Установить этот XmlNode узлом верхнеуровнев
ого документа.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:XmlNode`:`:IsEmpty`(`)const: [@(0.0.255) bool]_[* IsEmpty]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает true, если этот XmlNode представляе
т XML`_DOC и не содержит контента.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:XmlNode`:`:operator bool`(`)const: [* operator_bool]()_[@(0.0.255) const]&]
[s2;%RU-RU То же, что и !IsEmpty().&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:XmlNode`:`:GetCount`(`)const: [@(0.0.255) int]_[* GetCount]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает число содержимых подузлов 
XmlNode.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:XmlNode`:`:At`(int`): [_^XmlNode^ XmlNode][@(0.0.255) `&]_[* At]([@(0.0.255) int]_[*@3 i])
&]
[s2;%RU-RU Возвращает подузел по индексу 
[%-*@3 i]. Если такого узла нет (число подузлов 
меньше i `+ 1), число узлов расширяется 
до i `+ 1 с пустыми узлами.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:XmlNode`:`:Node`(int`)const: [@(0.0.255) const]_[_^XmlNode^ XmlNode][@(0.0.255) `&]_[* N
ode]([@(0.0.255) int]_[*@3 i])_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает узел по индексу [%-*@3 i]. 
Если такого нет, поведение неопределённое. 
Эта функция, возможно, немного быстрее 
At или operator`[`].&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:XmlNode`:`:operator`[`]`(int`)const: [@(0.0.255) const]_[_^XmlNode^ XmlNode][@(0.0.255) `&
]_[* operator`[`]]([@(0.0.255) int]_[*@3 i])_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает [%-*@3 i].&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:XmlNode`:`:operator`[`]`(const char`*`)const: [@(0.0.255) const]_[_^XmlNode^ XmlNode
][@(0.0.255) `&]_[* operator`[`]]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 tag])_[@(0.0.255) c
onst]&]
[s2;%RU-RU Находит подузел XML`_TAG с идентификатором 
[%-*@3 tag] и возвращает его. Если такой 
узел не найден, возвращает Void().&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:XmlNode`:`:Add`(`): [_^XmlNode^ XmlNode][@(0.0.255) `&]_[* Add]()&]
[s2;%RU-RU Добавляет новый подузел.&]
[s3;%RU-RU &]
[s4; &]
[s5;:XmlNode`:`:Remove`(int`): [@(0.0.255) void]_[* Remove]([@(0.0.255) int]_[*@3 i])&]
[s2;%RU-RU Удаляет подузел по [%-*@3 i].&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:XmlNode`:`:AddText`(const String`&`): [@(0.0.255) void]_[* AddText]([@(0.0.255) const]_
[_^String^ String][@(0.0.255) `&]_[*@3 txt])&]
[s2;%RU-RU Добавляет новый текстовый подузел 
(XML`_TEXT) и присваивает ему текст [%-*@3 txt].&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:XmlNode`:`:FindTag`(const char`*`)const: [@(0.0.255) int]_[* FindTag]([@(0.0.255) cons
t]_[@(0.0.255) char]_`*[*@3 tag])_[@(0.0.255) const]&]
[s2;%RU-RU Находит подузел XML`_TAG с идентификатором 
[%-*@3 tag] и возвращает его индекс. Если 
такого подузла нет, возвращает отрицательно
е число.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:XmlNode`:`:Add`(const char`*`): [_^XmlNode^ XmlNode][@(0.0.255) `&]_[* Add]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 tag])&]
[s2;%RU-RU Добавляет новый узел XML`_TAG с идентификато
ром [%-*@3 tag] и возвращает ссылку на него.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:XmlNode`:`:GetAdd`(const char`*`): [_^XmlNode^ XmlNode][@(0.0.255) `&]_[* GetAdd]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 tag])&]
[s5;:XmlNode`:`:operator`(`)`(const char`*`): [_^XmlNode^ XmlNode][@(0.0.255) `&]_[* operat
or()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 tag])&]
[s2;%RU-RU Возвращает ссылку на подузел 
XML`_TAG с идентификатором [%-*@3 tag]. Если 
такого узла нет, он добавляется.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:XmlNode`:`:Remove`(const char`*`): [@(0.0.255) void]_[* Remove]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 tag])&]
[s2;%RU-RU Удаляет подузел XML`_TAG с идентификатором 
[%-*@3 tag]. Емсли такого подузла нет, ничего 
не происходит.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:XmlNode`:`:GatherText`(`)const: [_^String^ String]_[* GatherText]()_[@(0.0.255) const]&]
[s5;:XmlNode`:`:operator`~`(`)const: [_^String^ String]_[* operator`~]()_[@(0.0.255) const]&]
[s2;%RU-RU Конкатенирует все подузлы XML`_TEXT 
в целях увеличения индексов.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:XmlNode`:`:HasTags`(`)const: [@(0.0.255) bool]_[* HasTags]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает true, если у узла есть 
субтэги.&]
[s3; &]
[s4;%RU-RU &]
[s5;:XmlNode`:`:GetAttrCount`(`)const: [@(0.0.255) int]_[* GetAttrCount]()_[@(0.0.255) cons
t]&]
[s2;%RU-RU Возвращает число атрибутов у 
текущего узла (он должен быть XML`_TAG, 
чтобы иметь какие`-либо атрибуты, 
`- возвращается не ноль).&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:XmlNode`:`:AttrId`(int`)const: [_^String^ String]_[* AttrId]([@(0.0.255) int]_[*@3 i])_[@(0.0.255) c
onst]&]
[s2;%RU-RU Возвращает идентификатор атрибута 
[%-*@3 i].&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:XmlNode`:`:Attr`(int`)const: [_^String^ String]_[* Attr]([@(0.0.255) int]_[*@3 i])_[@(0.0.255) c
onst]&]
[s2;%RU-RU Возвращает значение атрибута 
[%-*@3 i].&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:XmlNode`:`:Attr`(const char`*`)const: [_^String^ String]_[* Attr]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 id])_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает значение атрибута 
[%-*@3 id]. Если такого атрибута не существует, 
возвращает пустой String.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:XmlNode`:`:SetAttr`(const char`*`,const String`&`): [_^XmlNode^ XmlNode][@(0.0.255) `&
]_[* SetAttr]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], [@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 val])&]
[s2;%RU-RU Устанавливает атрибут [%-*@3 id], у 
которого значение [%-*@3 val]. Его ещё не 
должно существовать.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:XmlNode`:`:AttrInt`(const char`*`,int`)const: [@(0.0.255) int]_[* AttrInt]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 id], [@(0.0.255) int]_[*@3 def]_`=_Null)_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает значение атрибута 
[%-*@3 id], преобразованное в целое. Если 
такого атрибута не существует, возвращает 
[%-*@3 def].&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:XmlNode`:`:SetAttr`(const char`*`,int`): [_^XmlNode^ XmlNode][@(0.0.255) `&]_[* SetAtt
r]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], [@(0.0.255) int]_[*@3 val])&]
[s2;%RU-RU Устанавливает атрибут [%-*@3 id], у 
которого целочисленное значение 
[%-*@3 val].&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:XmlNode`:`:SetAttrs`(Upp`:`:VectorMap`<Upp`:`:String`,Upp`:`:String`>`&`&`): [@(0.0.255) v
oid]_[* SetAttrs]([_^Upp`:`:VectorMap^ VectorMap]<[_^Upp`:`:String^ String], 
[_^Upp`:`:String^ String]>`&`&_[*@3 a])&]
[s2;%RU-RU Заменяет все атрибуты `"пикнутым`" 
[%-*@3 a].&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:XmlNode`:`:SetAttrsPick`(pick`_ VectorMap`<String`,String`>`&`): [@(0.0.255) void]_
[* SetAttrsPick]([@(0.128.128) pick`_]_[_^VectorMap^ VectorMap]<[_^String^ String], 
[_^String^ String]>`&_[*@3 a])&]
[s2;%RU-RU Заменяет все атрибуты к [%-*@3 a], используя 
операцию pick ([%-*@3 a] при ней разрушается).&]
[s3; &]
[s4; &]
[s5;:XmlNode`:`:Shrink`(`): [@(0.0.255) void]_[* Shrink]()&]
[s2;%RU-RU Пытается уменьшить потребляемую 
память.&]
[s3; &]
[s4; &]
[s5;:XmlNode`:`:IsPicked`(`)const: [@(0.0.255) bool]_[* IsPicked]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает true, если `"пикнут`".&]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Конструктор  detail]]}}&]
[s3;%RU-RU &]
[s0; [* XmlNode]()&]
[s2;%RU-RU Конструирует пустой XmlNode.&]
[s3; &]
[s4; &]
[s5;:XmlNode`:`:XmlNode`(const XmlNode`&`,int`): [* XmlNode]([@(0.0.255) const]_[* XmlNode][@(0.0.255) `&
]_[*@3 n], [@(0.0.255) int])&]
[s2;%RU-RU Конструктор глубокой копии.&]
[s0; &]
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 XmlNode `- функции разбора и вывода]]}}&]
[s3;%RU-RU &]
[s4;H0; &]
[s5;:ParseXML`(XmlParser`&`,dword`): [_^XmlNode^ XmlNode]_[* ParseXML]([_^XmlParser^ XmlPar
ser][@(0.0.255) `&]_[*@3 p], [_^dword^ dword]_[*@3 style]_`=_XML`_IGNORE`_DECLS[@(0.0.255) `|
]XML`_IGNORE`_PIS[@(0.0.255) `|]XML`_IGNORE`_COMMENTS)&]
[s5;:ParseXML`(const char`*`,dword`): [_^XmlNode^ XmlNode]_[* ParseXML]([@(0.0.255) const]_
[@(0.0.255) char]_`*[*@3 s], [_^dword^ dword]_[*@3 style]_`=_XML`_IGNORE`_DECLS[@(0.0.255) `|
]XML`_IGNORE`_PIS[@(0.0.255) `|]XML`_IGNORE`_COMMENTS)&]
[s5;:ParseXML`(XmlParser`&`,ParseXmlFilter`&`,dword`): [_^XmlNode^ XmlNode]_[* ParseXML](
[_^XmlParser^ XmlParser][@(0.0.255) `&]_[*@3 p], [_^ParseXmlFilter^ ParseXmlFilter][@(0.0.255) `&
]_[*@3 filter], [_^dword^ dword]_[*@3 style]_`=_XML`_IGNORE`_DECLS[@(0.0.255) `|]XML`_IGN
ORE`_PIS[@(0.0.255) `|]XML`_IGNORE`_COMMENTS)&]
[s5;:ParseXML`(const char`*`,ParseXmlFilter`&`,dword`): [_^XmlNode^ XmlNode]_[* ParseXML](
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s], [_^ParseXmlFilter^ ParseXmlFilter][@(0.0.255) `&
]_[*@3 filter], [_^dword^ dword]_[*@3 style]_`=_XML`_IGNORE`_DECLS[@(0.0.255) `|]XML`_IGN
ORE`_PIS[@(0.0.255) `|]XML`_IGNORE`_COMMENTS)&]
[s5;:ParseXML`(Stream`&`,dword`): [_^XmlNode^ XmlNode]_[* ParseXML]([_^Stream^ Stream][@(0.0.255) `&
]_[*@3 in], [_^dword^ dword]_[*@3 style]_`=_XML`_IGNORE`_DECLS[@(0.0.255) `|]XML`_IGNORE`_
PIS[@(0.0.255) `|]XML`_IGNORE`_COMMENTS)&]
[s5;:ParseXMLFile`(const char`*`,dword`): [_^XmlNode^ XmlNode]_[* ParseXMLFile]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 path], [_^dword^ dword]_[*@3 style]_`=_XML`_IGNORE`_DECLS[@(0.0.255) `|
]XML`_IGNORE`_PIS[@(0.0.255) `|]XML`_IGNORE`_COMMENTS)&]
[s5;:ParseXML`(Stream`&`,ParseXmlFilter`&`,dword`): [_^XmlNode^ XmlNode]_[* ParseXML]([_^Stream^ S
tream][@(0.0.255) `&]_[*@3 in], [_^ParseXmlFilter^ ParseXmlFilter][@(0.0.255) `&]_[*@3 filt
er], [_^dword^ dword]_[*@3 style]_`=_XML`_IGNORE`_DECLS[@(0.0.255) `|]XML`_IGNORE`_PIS[@(0.0.255) `|
]XML`_IGNORE`_COMMENTS)&]
[s5;:ParseXMLFile`(const char`*`,ParseXmlFilter`&`,dword`): [_^XmlNode^ XmlNode]_[* Parse
XMLFile]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 path], [_^ParseXmlFilter^ ParseXmlFi
lter][@(0.0.255) `&]_[*@3 filter], [_^dword^ dword]_[*@3 style]_`=_XML`_IGNORE`_DECLS[@(0.0.255) `|
]XML`_IGNORE`_PIS[@(0.0.255) `|]XML`_IGNORE`_COMMENTS)&]
[s0; &]
[s2;%RU-RU Создаёт XmlNode, разбирая документ 
XML, предоставленный как XmlParser, строку, 
поток ввода или путь к файлу. [%-*@3 style] 
может быть комбинацией из&]
[s2;%RU-RU &]
[ {{3581:6419<288;^ [s2;l32;%RU-RU XML`_IGNORE`_DECLS]
::= [s2;l64;%RU-RU Элементы декларации игнорируются.]
::^ [s2;l32;%RU-RU XML`_IGNORE`_PIS]
::= [s2;l64;%RU-RU Элементы информации об обработке 
игнорируются.]
::^ [s2;l32;%RU-RU XML`_IGNORE`_COMMENTS]
::= [s2;l64;%RU-RU Комментарии игнорируются.]}}&]
[s3;%RU-RU &]
[s2;%RU-RU Варианты с [%-*@3 filter] позволяют указать 
[^topic`:`/`/Core`/src`/ParseXmlFilter`_ru`-ru`#ParseXmlFilter`:`:struct^ класс
`-фильтр], чтобы исключить некоторые 
части XML, обычно, для сохранения памяти. 
Может выводить XmlError.&]
[s3;%RU-RU &]
[s4; &]
[s5;:AsXML`(Stream`&`,const XmlNode`&`,dword`): [@(0.0.255) void]_[* AsXML]([_^Stream^ Stre
am][@(0.0.255) `&]_[*@3 out], [@(0.0.255) const]_[_^XmlNode^ XmlNode][@(0.0.255) `&]_[*@3 n],
 [_^dword^ dword]_[*@3 style]_`=_XML`_HEADER[@(0.0.255) `|]XML`_DOCTYPE[@(0.0.255) `|]XML
`_PRETTY)&]
[s5;:AsXML`(const XmlNode`&`,dword`): [_^String^ String]_[* AsXML]([@(0.0.255) const]_[_^XmlNode^ X
mlNode][@(0.0.255) `&]_[*@3 n], [_^dword^ dword]_[*@3 style]_`=_XML`_HEADER[@(0.0.255) `|]X
ML`_DOCTYPE[@(0.0.255) `|]XML`_PRETTY)&]
[s5;:AsXMLFile`(const char`*`,const XmlNode`&`,dword`): [@(0.0.255) bool]_[* AsXMLFile]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 path], [@(0.0.255) const]_[_^XmlNode^ XmlNode][@(0.0.255) `&
]_[*@3 n], [_^dword^ dword]_[*@3 style]_`=_XML`_HEADER[@(0.0.255) `|]XML`_DOCTYPE[@(0.0.255) `|
]XML`_PRETTY)&]
[s0; &]
[s2;%RU-RU Создаёт документ XML из XmlNode [%-*@3 n]. 
[%-*@3 style] может быть комбинацией из&]
[s2;%RU-RU &]
[ {{3581:6419<288;^ [s2;l32; XML`_HEADER]
::= [s2;l64;%RU-RU Добавляет стандаотный `"заголовочник`"
 XML в ачале документа.]
::^ [s2;l32; XML`_DOCTYPE]
::= [s2;l64;%RU-RU Добавляет декларацию DOCTYPE в 
начале документа,  с типом, взятым 
как идентификатор первого подузла 
XML`_TAG (представляющего корневой элемент).]
::^ [s2;l32; XML`_PRETTY]
::= [s2;l64;%RU-RU Документ XML форматирован как 
многострочный документ с соблюдением 
отступов. Если не активен, всё в одну 
строку, без отступов.]}}&]
[s2; &]
[s2; Вывод сохраняется в поток Stream, возвращается
 как String или (через AsXMLFile) сохраняется 
в файле. AsXMLFile возвращает true, если 
файл вывода создан успешно.&]
[s2; ]]