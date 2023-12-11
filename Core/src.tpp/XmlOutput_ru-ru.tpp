topic "Вывод XML";
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
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Класс XmlTag]]}}&]
[s3;%RU-RU &]
[s1;:XmlTag`:`:class: [@(0.0.255)3 class][3 _][*3 XmlTag][3 _:_][@(0.0.255)3 private][3 _][*@3;3 Mov
eable][3 <][*3 XmlTag][3 >_]&]
[s9;%RU-RU Этот класс упрощает задачу создания 
выводного текста XML. Он даёт средства 
для форматирования тэгов и атрибутов 
XML и выводит в String.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Публичных Методов]]}}&]
[s4;H0; &]
[s5;:XmlTag`:`:Tag`(const char`*`): [_^XmlTag^ XmlTag][@(0.0.255) `&]_[* Tag]([@(0.0.255) con
st]_[@(0.0.255) char]_`*[*@3 s])&]
[s2;%RU-RU Устанавливает имя тэга.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:XmlTag`:`:operator`(`)`(`): [_^String^ String]_[* operator()]()&]
[s2;%RU-RU Возвращает завершённый тэговый 
блок XML с [/ пустым ]контентом. Напр., 
<mytag></mytag>.&]
[s3;%RU-RU &]
[s4; &]
[s5;:XmlTag`:`:operator`(`)`(const char`*`): [_^String^ String]_[* operator()]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 text])&]
[s5;:XmlTag`:`:operator`(`)`(const String`&`): [_^String^ String]_[* operator()]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[*@3 text])&]
[s2;%RU-RU Возвращает завершённый тэговый 
блок XML с контентом [*@3 text]. Напр., <mytag>mytext</mytag>. 
Заметьте, что [*@3 text] здесь ни коим образом 
не меняется в выводе `- он может содержать 
другие субтэги.&]
[s3;%RU-RU &]
[s4; &]
[s5;:XmlTag`:`:Text`(const char`*`,byte`): [_^String^ String]_[* Text]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 text], [_^byte^ byte]_[*@3 charset]_`=_CHARSET`_DEFAULT)&]
[s5;:XmlTag`:`:Text`(const String`&`,byte`): [_^String^ String]_[* Text]([@(0.0.255) const]_
[_^String^ String][@(0.0.255) `&]_[*@3 s], [_^byte^ byte]_[*@3 charset]_`=_CHARSET`_DEFAULT
)&]
[s2; [%RU-RU Возвращает завершённый тэговый 
блок XML с контентом ][*@3 text][%RU-RU . В этом 
случае, текст преобразовывается из 
][*@3 charset] в кодировку UTF`-8 и эскапируется 
сущностями.&]
[s3;%RU-RU &]
[s4; &]
[s5;:XmlTag`:`:PreservedText`(const char`*`,byte`): [_^String^ String]_[* PreservedText](
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s], [_^byte^ byte]_[*@3 charset]_`=_CHARSET`_DE
FAULT)&]
[s5;:XmlTag`:`:PreservedText`(const String`&`,byte`): [_^String^ String]_[* PreservedText
]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 s], [_^byte^ byte]_[*@3 charset]_
`=_CHARSET`_DEFAULT)&]
[s2; Возвращает завершённый тэговый блок 
XML с контентом [*@3 text]. В этом случае, 
текст преобразовывается из [*@3 charset] 
в кодировку UTF`-8 и эскапируется сущностями. 
В отличие от [* Text], в этом варианте добавляется
 атрибут [C xml:space`=`"preserved`"] и сущностями 
эскапируются также `'`\t`' и `'`\n`'.&]
[s3;%RU-RU &]
[s4; &]
[s5;:XmlTag`:`:operator`(`)`(const char`*`,const char`*`): [_^XmlTag^ XmlTag][@(0.0.255) `&
]_[* operator()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 attr], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 val])&]
[s5;:XmlTag`:`:operator`(`)`(const char`*`,int`): [_^XmlTag^ XmlTag][@(0.0.255) `&]_[* oper
ator()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 attr], [@(0.0.255) int]_[*@3 q])&]
[s5;:XmlTag`:`:operator`(`)`(const char`*`,double`): [_^XmlTag^ XmlTag][@(0.0.255) `&]_[* o
perator()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 attr], [@(0.0.255) double]_[*@3 q])&]
[s2;%RU-RU Эти методы добавляют атрибуты 
тэгу XML.&]
[s3;%RU-RU &]
[s0;%RU-RU &]
[ {{10000t/25b/25@1 [s0; [* Детали Конструктора]]}}&]
[s3;%RU-RU &]
[s5;:XmlTag`:`:XmlTag`(`): [* XmlTag]()&]
[s2; Пустой конструктор. Чтобы установить 
имя тэга, нужно использовать метод 
Tag.&]
[s3; &]
[s4; &]
[s5;:XmlTag`:`:XmlTag`(const char`*`): [* XmlTag]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 t
ag])&]
[s2;%RU-RU Устанавливает имя тэга в [%-*@3 tag].&]
[s3; &]
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Функции вывода XML]]}}&]
[s0; &]
[s5;:DeXml`(const char`*`,byte`,bool`): String_[* DeXml]([@(0.0.255) const]_[@(0.0.255) cha
r]_`*[*@3 s], byte_[*@3 charset]_`=_CHARSET`_DEFAULT, [@(0.0.255) bool]_[*@3 escapelf]_`=
_[@(0.0.255) false])&]
[s2; Эта функция эскапирует строку [*@3 s], 
кодированную в [*@3 charset], сущностями 
XML (типа `'[@(128.0.255) `&lt;]`'), преобразуя её 
в UTF`-8. Если [*@3 escapelf ]равно true, `'`\n`' эскапируется
 как `'[@(128.0.255) `&#x%0a;]`'.&]
[s3; &]
[s4; &]
[s5;:XmlHeader`(const char`*`,const char`*`,const char`*`): [_^String^ String]_[* XmlHead
er]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 encoding]_`=_`"UTF[@(0.0.255) `-][@3 8]`", 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 version]_`=_`"[@3 1][@(0.0.255) .][@3 0]`", 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 standalone]_`=_NULL)&]
[s2; [%RU-RU Создаёт заглавие файла XML. 
Если ][*@3 version]_ равно NULL, он не включается, 
[*@3 standalone] может быть либо `"yes`", либо 
`"no`", либо NULL (тогда отдельно стоящий 
атрибут не включается).&]
[s3;%RU-RU &]
[s4; &]
[s5;:XmlDecl`(const char`*`): [_^String^ String]_[* XmlDecl]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 text])&]
[s2;%RU-RU Создаёт элемент декларации XML.&]
[s3;%RU-RU &]
[s4; &]
[s5;:XmlDocType`(const char`*`): [_^String^ String]_[* XmlDocType]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 text])&]
[s2;%RU-RU Создаёт элемент декларации DOCTYPE 
XML.&]
[s3;%RU-RU &]
[s4; &]
[s5;:XmlDoc`(const char`*`,const char`*`): [_^String^ String]_[* XmlDoc]([@(0.0.255) const]_
[@(0.0.255) char]_`*[*@3 name], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 xmlbody])&]
[s2;%RU-RU Возвращает [*C XmlHeader() `+ XmlDocType(name) 
`+ XmlTag(name)(xmlbody)] `- в основном добавляет 
стандартное заглавие XML к телу 
XML.&]
[s3;%RU-RU &]
[s4; &]
[s5;:XmlComment`(const char`*`): [_^String^ String]_[* XmlComment]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 text])&]
[s2;%RU-RU Возвращает элемент комментария 
XML.&]
[s3;%RU-RU &]
[s4; &]
[s5;:XmlPI`(const char`*`): [_^String^ String]_[* XmlPI]([@(0.0.255) const]_[@(0.0.255) char]_
`*[*@3 text])&]
[s2;%RU-RU Создаёт элемент информации об 
обработке XML.&]
[s3;%RU-RU &]
[s0; ]]