topic "Класс WithPolyXML";
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
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Класс WithPolyXML]]}}&]
[s1;@(0.0.255)3 &]
[s1;:noref: [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@4;3 T][3 >]&]
[s1;:WithPolyXML`:`:class: [@(0.0.255) class]_[* WithPolyXML]_:_[@(0.0.255) public]_[*@3 WithF
actory]<[*@4 T]>_&]
[s8;%RU-RU [*C@4 T]-|Тип базового класса полиморфной 
иерархии.&]
[s0;%RU-RU Этот шаблонный класс реализует 
базовое поведение полиморфных классов, 
которые нужно отправлять в поток 
в формате XML.&]
[s3;%RU-RU &]
[s1;@(0.0.255)3 &]
[s1;:noref: [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@4;3 T][3 >]&]
[s1;:PolyXMLArray`:`:class: [@(0.0.255) class]_[* PolyXMLArray]_:_[@(0.0.255) public]_[*@3 Arr
ay]<[*@4 T]>_&]
[s8;%RU-RU [*C@4 T]-|Тип базового класса полиморфной 
иерархии.&]
[s0;%RU-RU Этот шаблонный класс реализует 
базовое поведение полиморфного `"XML`-streamable`" 
контейнера Array.&]
[s2;%RU-RU &]
[s3; &]
[s1;:noref: [@(0.0.255)3 template][3 _<][@(0.0.255)3 class ][*@4;3 K][@(0.0.255)3 , 
class][3 _][*@4;3 T][3 >]&]
[s1;:PolyXMLArrayMap`:`:class: [@(0.0.255) class]_[* PolyXMLArrayMap]_:_[@(0.0.255) public]_
[*@3 ArrayMap]<[*@4 K ][* , ][*@4 T]>_&]
[s8;%RU-RU [*C@4 T]-|Тип базового класса полиморфной 
иерархии.&]
[s0;%RU-RU Этот шаблонный класс реализует 
базовое поведение  полиморфного `"XML`-streamable`" 
контейнера ArrayMap.&]
[s2;%RU-RU &]
[s3; &]
[s1;:noref: [@(0.0.255)3 template][3 _<][@(0.0.255)3 class ][*@4;3 K][@(0.0.255)3 , 
class][3 _][*@4;3 T][3 >]&]
[s1;:PolyXMLArrayMapOne`:`:class: [@(0.0.255) class]_[* PolyXMLArrayMapOne]_:_[@(0.0.255) pu
blic]_[*@3 ArrayMap]<[*@4 K ][* , One<][*@4 T]> >_&]
[s8;%RU-RU [*C@4 T]-|Тип базового класса полиморфной 
иерархии.&]
[s0;%RU-RU Этот шаблонный класс реализует 
базовое поведение полиморфного `"XML`-streamable`" 
контейнера ArrayMap, в котором тип Value 
являтся типом [%-* One<][%-*@4 T][%- >].&]
[s2;%RU-RU &]
[s3; &]
[s0;%RU-RU &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Базовое Применение]]}}&]
[s3; &]
[s9;%RU-RU Чтобы реализовать полиморфное 
XML поведение у класса`-основы, просто 
производят этот класс от шаблона 
[* WithPolyXML] :&]
[s0;%RU-RU &]
[s2;%RU-RU [* class MyBaseClass : public WithPolyXML<MyBaseClass>]&]
[s2;%RU-RU [* `{]&]
[s2;%RU-RU [* -|-|public:]&]
[s2;%RU-RU [* -|-|-|virtual void Xmlize(XmlIO xml);]&]
[s2;%RU-RU [* `};]&]
[s9;%RU-RU А затем, как обычно, производят 
от этого класса всю иерархию классов 
:&]
[s0;%RU-RU &]
[s2;%RU-RU [* class MyDerivedClass : public MyBaseClass]&]
[s2;%RU-RU [* `{]&]
[s2;%RU-RU [* -|-|public:]&]
[s2;%RU-RU [* -|-|-|virtual void Xmlize(XmlIO xml);]&]
[s2;%RU-RU [* `};]&]
[s9;%RU-RU &]
[s9;%RU-RU Виртуальная функция`-член [* Xmlize()] 
должна быть определена у каждого 
класса этой иерархии; её задача `- 
XML`-ить [* ТОЛЬКО ]прямых членов класса. 
По этой причине в каждом производном 
классе [* ВНАЧАЛЕ] нужно вызвать из 
него [* Xmlize()] родительского класса; 
вот пример:&]
[s0;%RU-RU &]
[s2;%RU-RU [* void MyBaseClass`::Xmlize(XmlIO xml)]&]
[s2;%RU-RU [* `{]&]
[s2;%RU-RU [* -|-|xml]&]
[s2;%RU-RU [* -|-|-|(`"FirstBaseMember`", FirstBaseMember)]&]
[s2;%RU-RU [* -|-|-|(`"SecondBaseMember`", SecondBaseMember)]&]
[s2;%RU-RU [* -|-|;]&]
[s2;%RU-RU [* `}]&]
[s2;*%RU-RU &]
[s2;%RU-RU [* void MyDerivedClass`::Xmlize(XmlIO xml)]&]
[s2;%RU-RU [* `{]&]
[s2;%RU-RU [* -|-|MyBaseClass`::Xmlize(XmlIO xml);]&]
[s2;%RU-RU [* -|-|xml]&]
[s2;%RU-RU [* -|-|-|(`"FirstDerivedMember`", FirstDerivedMember)]&]
[s2;%RU-RU [* -|-|-|(`"SecondDerivedMember`", SeconDeriveddMember)]&]
[s2;%RU-RU [* -|-|;]&]
[s2;%RU-RU [* `}]&]
[s9;%RU-RU Не забывайте регистрировать свои 
классы макросом [* REGISTERCLASS], смотрите 
тематику справки о [* WithFactory ]по поволу 
этого.&]
[s9;%RU-RU Далее, нужно декларировать контейнер 
класса :&]
[s0;%RU-RU &]
[s2;%RU-RU [* class MyXMLContainer : public PolyXMLArray<MyXMLContainer>]&]
[s2;%RU-RU [* `{]&]
[s2;%RU-RU [* `}];&]
[s0;%RU-RU &]
[s0;%RU-RU Это всё! Теперь можно строить 
свои классы, добавлять их в контейнер, 
поведение которого идентично поведению 
Array`'я, и `"поточить`" этот контейнер 
в формате XML:&]
[s0;%RU-RU &]
[s0;%RU-RU Создать классы и контейнер :&]
[s2;%RU-RU [* MyBaseClass `* baseClass `= new MyBaseClass;]&]
[s2;%RU-RU [* MyBaseClass `* derivedClass `= MyBaseClass`::CreatePtr(`"MyDerivedClass`")
;]&]
[s2;%RU-RU [* MyXMLContainer myContainer;]&]
[s2;%RU-RU [* myContainer.Add(baseClass);]&]
[s2;%RU-RU [* myContainer.Add(derivedClass);]&]
[s0;%RU-RU &]
[s0;%RU-RU `"Употочить`" контейнер в строку 
:&]
[s2;%RU-RU [* String s `= StoreAsXML(myContainer, `"aTagName`");]&]
[s0;%RU-RU &]
[s0;%RU-RU Перезагрузить контейнер из строки 
:&]
[s2;%RU-RU [* myContainer.Clear();]&]
[s2;%RU-RU [* bool  res `= LoadFromXML(myContainer, s);]&]
[s0;%RU-RU &]
[s0;%RU-RU Сохранить контейнер в файл xml 
:&]
[s2;%RU-RU [* bool res `= StoreAsXMLFile(myContainer, `"aTagName`", 
`"a/file/path.xml`");]&]
[s0;%RU-RU &]
[s0;%RU-RU Перезагрузить контейнер из файла 
xml :&]
[s2;%RU-RU [* myContainer.Clear();]&]
[s2;%RU-RU [* bool res `= LoadFromXMLFile(myContainer, `"a/file/path.xml`")]&]
[s0;%RU-RU ]]