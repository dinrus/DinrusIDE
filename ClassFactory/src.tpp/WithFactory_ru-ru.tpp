topic "Класс WithFactory";
[i448;a25;kKO9; $$1,0#37138531426314131252341829483380:structitem]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[i448;b42;O9;2 $$4,4#61672508125594000341940100500538:tparam]
[b42;2 $$5,5#13035079074754324216151401829390:normal]
[H6;0 $$6,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$7,0#37138531426314131252341829483370:codeitem]
[ $$0,0#00000000000000000000000000000000:Default]
[{_}%EN-US 
[ {{10000@(113.42.0) [s0; [*@7;4 Класс WithFactory]]}}&]
[s0;*@3;4 &]
[s1;:noref:%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>&]
[s1;:WithFactory`:`:class:%- [@(0.0.255) class]_[* WithFactory]&]
[s4; [*C@4 T]-|Тип класса`-основы полиморфной 
иерархии.&]
[s5; Этот шаблонный класс добавляет в 
производные классы некоторую RTTI (Run 
Time Type Identification) `- идентификацию типов 
во время выполнения (ИТВВ),`- и реализует 
так называемую `'фабрику классов`', 
внося способность регистрировать 
иерархии классов, конструировать 
их по имени, проверять их тип во время 
выполнения (рантайм), создавать список 
всех классов в иерархии и т.д. &]
[s5; Использование полиморфных классов 
базируется именно на таком поведении.&]
[s2; &]
[s3;%- &]
[s0;2 &]
[ {{10000F(128)G(128)@1 [s0; [* Основное применение]]}}&]
[s3;%- &]
[s5; Чтобы реализовать поведение фабрики 
в своём классе`-основе, нужно сделать 
его производным от WithFactory (наследником) 
:&]
[s0; &]
[s2; [* class MyBaseClass : public WithFactory<MyBaseClass>]&]
[s2; [* `{]&]
[s2; [* -|-|.........]&]
[s2; [* `};]&]
[s5; Затем можно создавать иерархию от 
класса`-основы, как обычно :&]
[s0; &]
[s2; [* class MyDerivedClass : public MyBaseClass]&]
[s2; [* `{]&]
[s2; [* -|-|.........]&]
[s2; [* `};]&]
[s5; Когда класс определён, его нужно 
зарегистрировать; что выполняется 
вставкой в файле .cpp (или в отдельных, 
если вам это больше нравится, но только 
НЕ в файлах включения) следующей инструкции 
:&]
[s5; &]
[s2; [* REGISTERCLASS(MyBaseClass `[, `"описание класса`" 
`[, anIndex `[, `"имя русунка Iml`"`]`]`])]&]
[s2; [* REGISTERCLASS(MyDerivedClass `[, `"описание класса`" 
`[, anIndex `[, `"имя русунка Iml`"`]`]`])]&]
[s5; &]
[s5; где можно вставить необязательное 
описание класса, индекс и иконку в 
формате Iml; их использование будет 
пояснено далее.&]
[s5; Создание класса можно выполнить 
следующими способами :&]
[s5; &]
[s2; Как указатель от классического оператора 
new:&]
[s2; -|-|[* MyBaseClass `*ptr `= new MyBaseClass;]&]
[s2; [* -|-|MyBaseClass `*ptr `= new MyDerivedClass;]&]
[s0; &]
[s2; Как указатель от имени класса в ascii 
:&]
[s2; -|-|[* MyBaseClass `*ptr `= MyBaseClass`::CreatePtr(`"MyBaseClass`");]&]
[s2; [* -|-|MyBaseClass `*ptr `= MyBaseClass`::CreatePtr(`"MyDerivedClass`");]&]
[s0; &]
[s2; Как One<MyBaseClass> :&]
[s2; -|-|[* One<MyBaseClass> ptr `= MyBaseClass`::CreateInstance(`"MyBaseClass`");]&]
[s2; [* -|-|One<MyBaseClass> ptr `= MyBaseClass`::CreateInstance(`"MyDerivedClass`");]&]
[s0; &]
[s5; Во время выполнения тип объекта можно 
запрашивать с помощью функции`-члена 
[* IsA() ]:&]
[s5; &]
[s2; [* MyBaseClass `*ptr `= new MyDerivedClass;]&]
[s2; [* String classType `= ptr`->IsA();   ]возвращает строку 
`"MyDerivedClass`"&]
[s0; &]
[s5; Можно,`- также в рантайм,`- получить 
список иерархии класса, с помощью 
статической функции`-члена [* Classes()]:&]
[s5; &]
[s2; [* Vector<String> const `&classList `= MyBaseClass`::Classes();]&]
[s0; &]
[s5; или получать описания по имени класса, 
статической функцией`-членом [* GetClassDescription()] 
:&]
[s5; &]
[s2; [* String classDesc `= MyBaseClass`::GetClassDescription(`"MyBaseClass`");]&]
[s2; [* String classDesc `= MyBaseClass`::GetClassDescription(`"MyDerivedClass`");]&]
[s0; &]
[s5; либо получать целочисленный `'индекс`', 
статической функцией`-членом [* GetClassIndex()] 
:&]
[s5; &]
[s2; [* int classIndex `= MyBaseClass`::GetClassIndex(`"MyBaseClass`");]&]
[s2; [* int classIndex `= MyBaseClass`::GetClassIndex(`"MyDerivedClass`");]&]
[s0;2 &]
[ {{10000F(128)G(128)@1 [s0; [* Регистрация класса]]}}&]
[s3;%- &]
[s7;:REGISTERCLASS`(type`, `.`.`.`):%- [* REGISTERCLASS]([*@3 type])&]
[s2; Регистрирует класс по его типу [%-*@3 type].&]
[s3; &]
[s6; &]
[s7;:REGISTERCLASS`(type`, `.`.`.`):%- [* REGISTERCLASS]([*@3 type], 
[*@3 description])&]
[s2; Регистрирует класс по его типу [%-*@3 type], 
приводя необязательное (опционное) 
описание [%-*@3 description], которое можно 
запрашивать позднее. Главное назначение 
у описания `- представлять списки 
классов в меню или в диалоговые окна 
при создании их во время выполнения.&]
[s3; &]
[s7;:REGISTERCLASS`(type`, `.`.`.`):%- [* REGISTERCLASS]([*@3 type], 
[*@3 description, index])&]
[s2; Регистрирует класс по его типу [%-*@3 type], 
приводя необязательное описание 
[%-*@3 description ]и целочисленный индекс 
[%-*@3 index], которые можно запрашивать 
позже. Главное предназначение индекса 
`- дать возможность сортировать список 
классов в рантайм по важности. [%-*@3 Index] 
может быть любым целым числом &]
[s6; &]
[s7;:REGISTERCLASS`(type`, `.`.`.`):%- [* REGISTERCLASS]([*@3 type], 
[*@3 description, index, icon])&]
[s2; Регистрирует класс по его типу [%-*@3 type][%RU-RU , 
приводя необязательное описание 
][%-*@3 description][%RU-RU , целочисленный индекс 
][%-*@3 index ]и иконку [%-*@3 icon ]which can be queried 
later on. Главное предназначение индекса 
`- дать возможность сортировать список 
классов в рантайм по важности или 
группам. [%-*@3 Index] может быть любым целым 
числом  [%-*@3 icon ]должна быть String с полным 
именем иконки Iml, наподобие `"MyIml`::MyImage`" 
&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0; [* Подробный список членов]]}}&]
[s3;%- &]
[s6;%- &]
[s7;:WithFactory`:`:Create`(const String`&`):%- [@(0.0.255) static] 
[_^One^ One]<[*@4 T]>_[* CreateInstance]([@(0.0.255) const]_[_^String^ String]_`&[*@3 classNa
me])&]
[s2; Создаёт класс, производный от основы 
иерархии T по его имени [%-*@3 className].&]
[s2; Возвращает смарт`-указатель на класс`-основу
 T&]
[s3; &]
[s6;%- &]
[s7;:WithFactory`:`:CreatePtr`(String const`&`):%- [@(0.0.255) static] 
[*@4 T]_`*[* CreatePtr]([_^String^ String]_[@(0.0.255) const]_`&[*@3 className])&]
[s2; оздаёт класс, производный от основы 
иерархии T по его имени [%-*@3 className].&]
[s2; Возвращает традиционный указатель 
на класс`-основу T&]
[s3; &]
[s6;%- &]
[s7;:WithFactory`:`:Classes`(void`):%- [@(0.0.255) static] [_^Vector^ Vector]<[_^String^ St
ring]>_[@(0.0.255) const]_`&[* Classes]([@(0.0.255) void])&]
[s2; Возвращает Vector из строк, содержащий 
все имена зарегистрированных классов 
иерархии.&]
[s3;%- &]
[s6;%- &]
[s7;:WithFactory`:`:GetClassDescription`(const String`&`):%- [@(0.0.255) static] 
[_^String^ String]_[@(0.0.255) const]_`&[* GetClassDescription]([@(0.0.255) const]_[_^String^ S
tring]_`&[*@3 className])&]
[s2; Возвращает описание класса в ascii, 
который идентифицируется по имени 
[%-*@3 className].&]
[s2; Если при регистрации класса не приводилось 
никакого описания, то возвратит пустую 
строку.&]
[s3; &]
[s6;%- &]
[s7;:WithFactory`:`:GetClassIndex`(const String`&`):%- [@(0.0.255) static] 
[@(0.0.255) int]_[@(0.0.255) const]_`&[* GetClassIndex]([@(0.0.255) const]_[_^String^ Strin
g]_`&[*@3 className])&]
[s2; Возвращает целочисленный индекс, 
присвоенный типу классу, идентифицируемого 
по имени [%-*@3 className].&]
[s2; Если при регистрации класса не приводилось 
никакого индекса, то возвратит 0.&]
[s3; &]
[s6;%- &]
[s7;:WithFactory`:`:GetClassIndex`(const String`&`):%- [@(0.0.255) static] 
Image_[* GetClassImage]([@(0.0.255) const]_[_^String^ String]_`&[*@3 className])&]
[s2; Возвращает объект Image, присовенный 
типу класса, идентифицируемого по 
[%-*@3 className].&]
[s2; Если при регистрации класса не приводилось 
никакого изо, то возвратит Null.&]
[s3; &]
[s6;%- &]
[s7;:WithFactory`:`:IsA`(void`):%- [_^String^ String]_[@(0.0.255) const]_`&[* IsA]([@(0.0.255) v
oid])&]
[s2; Возвращает строку, содержащую имя 
класса.&]
[s3;%- &]
[s6;%- &]
[s0; ]]