topic "Класс DropList (ВыпадающийСписок)";
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
[ {{10000@3 [s0;%RU-RU [*@7;4 Класс ][*@(229)4 DropList  (ВыпадающийСписок)]]}}&]
[s3; &]
[s1;:DropList`:`:class: [@(0.0.255)3 class][3 _][*3 DropList][3 _:_][@(0.0.255)3 public][3 _][*@3;3 M
ultiButton][3 , ][@(0.0.255)3 public][3 _][*@3;3 Convert]&]
[s0;%RU-RU &]
[s2;%RU-RU 
@@image:337&143
(AzYAFwAAAOn/AAAAAHic7dfPCcIwGIfh7OcOjuDVGdzAKbx51YqVKlGxqBg8qRWxGMQNNPBBKAniVy3+ghjeQ5IW+tBD/+hc6X9V1Gh1AswR3gMb3xfK7a3dPXAyZ0KEdGlO5kyUcKKunJhCIYSzpPGJcLzRnF4KfcyzeVlhsr5weuMeViUcrXKqVm/aub8DFMbLs8lgKFr6O0DhMD1RluTMKaBwsDjaLIwqHgIKo3lWzPKcfaCwP9s7GZ6/CXwe9qY7TsB3SiQVJ6AwliknlDD8b5uywxcGGPzX42d6AOPWczE=)
&]
[s0;%RU-RU &]
[s9;%RU-RU Этот виджет предоставляет выбор 
значений из ниспадающего списка. 
Список организован по парам ключ`-значение; 
когда элемент выделен, ключ этого 
элемента становится новым значением 
DropList`'а (так как используется интерфесом 
SetData/GetData). Это значение DropList`'а не обязательно
 должно быть в списке в качестве ключа, 
можно, например, присвоить значение 
DropList`'у с пустым списком и заполнить 
его позже.&]
[s9;%RU-RU Как ключ, так и значение имеют 
генерный тип Value. С этим списком можно 
ассоциировать специфичные Display и 
Convert, и таким образом использовать 
DropList для представления любого типа 
значений (цвета, ширина строк, шрифты 
и прочее).&]
[s9;%RU-RU DropList также реализует однонаправленный 
интерфейс Convert, где Format определен 
как трансляция ключ`->значение. Когда 
включен режим DisplayAll(), Format возвращает 
значения ключей для ключей, отсутствующих 
в таблице ключ / значение; когда режим 
DisplayAll() выключен, такие значения рассматрива
ются как неверные и Format возвращает 
Value().&]
[s9;%RU-RU Примечание: DropBox `- реализация вспомогател
ьного класса, используемого для обмена 
каким`-либо кодом между DropList`'ом и 
подобными ему классами. DropBox производный 
от Ctrl`'а.&]
[s3;%RU-RU &]
[s0;%RU-RU &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Публичные члены DropList]]}}&]
[s3; &]
[s5;:DropList`:`:Style`:`:typedef: [@(0.0.255) typedef]_[_^MultiButton`:`:Style^ MultiBut
ton`::Style]_[* Style]&]
[s2;%RU-RU Стиль `"Хамелеон`" у DropList фактически 
является стилем класса`-основы MultiButton.&]
[s3; &]
[s4; &]
[s5;:DropList`:`:WhenDrop: [_^Callback^ Callback]_[* WhenDrop]&]
[s2;%RU-RU Этот обрвыз вызывается [* перед] 
выводом на дисплей этого списка. Клиентский 
код может (вос)создать этот список 
при этом обратном вызове.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DropList`:`:Add`(const Upp`:`:Value`&`,const Upp`:`:Value`&`,bool`): [_^Upp`:`:DropList^ D
ropList][@(0.0.255) `&]_[* Add]([@(0.0.255) const]_[_^Upp`:`:Value^ Value][@(0.0.255) `&]_[*@3 k
ey], [@(0.0.255) const]_[_^Upp`:`:Value^ Value][@(0.0.255) `&]_[*@3 value], 
[@(0.0.255) bool]_[*@3 enable] `= [@(0.0.255) true])&]
[s2;%RU-RU Добавляет новый элемент в список, 
ключ [%-*@3 key] определяет значение элемента, 
значение [%-*@3 value] отображается в области 
значения и в списке. Если [%-*@3 enable] равно 
false, DropList`'у можно присвоить этот ключ 
[%-*@3 key ]и будет показано соответствующее 
значение [%-*@3 value], но оно не включается 
во всплывающее окно выпадающего списка.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DropList`:`:Add`(std`:`:initializer`_list`<std`:`:pair`<Upp`:`:Value`,Upp`:`:Value`>`>`): [_^Upp`:`:DropList^ D
ropList][@(0.0.255) `&]_[* Add]([_^std`:`:initializer`_list^ std`::initializer`_list]<[_^std`:`:pair^ s
td`::pair]<[_^Upp`:`:Value^ Value], [_^Upp`:`:Value^ Value]>>_[*@3 init])&]
[s2;%RU-RU Добавляет в список пару [%-*@3 init].&]
[s3;%RU-RU &]
[s4; &]
[s5;:DropList`:`:Add`(const Value`&`): [_^DropList^ DropList][@(0.0.255) `&]_[* Add]([@(0.0.255) c
onst]_[_^Value^ Value][@(0.0.255) `&]_[*@3 value])&]
[s2;%RU-RU То же, что и Add(value, value).&]
[s3; &]
[s4; &]
[s5;:DropList`:`:Remove`(int`): [@(0.0.255) void]_[* Remove]([@(0.0.255) int]_[*@3 i])&]
[s2;%RU-RU Удаляет элемент выпадающего списка 
[%-*@3 i].&]
[s3;%RU-RU &]
[s4; &]
[s5;:DropList`:`:ClearList`(`): [@(0.0.255) void]_[* ClearList]()&]
[s2;%RU-RU Очищает список.&]
[s3; &]
[s4; &]
[s5;:DropList`:`:Clear`(`): [@(0.0.255) void]_[* Clear]()&]
[s2;%RU-RU Очищает список и устанавливает 
DropList в Null.&]
[s3; &]
[s4; &]
[s5;:DropList`:`:AddSeparator`(`): [_^DropList^ DropList][@(0.0.255) `&]_[* AddSeparator]()
&]
[s2;%RU-RU Добавляет элемент разделителя 
горизонтальной линейки. Этот элемент 
нельзя выделить.&]
[s3; &]
[s4; &]
[s5;:DropList`:`:Drop`(`): [@(0.0.255) void]_[* Drop]()&]
[s2;%RU-RU Заставляет список `"выпадать`".&]
[s3;%RU-RU &]
[s4; &]
[s5;:DropList`:`:SetIndex`(int`): [@(0.0.255) void]_[* SetIndex]([@(0.0.255) int]_[*@3 i])&]
[s2;%RU-RU Устанавливает значение элемента 
списка [%-*@3 i] в текущее значение.&]
[s3; &]
[s4; &]
[s5;:DropList`:`:GetIndex`(`)const: [@(0.0.255) int]_[* GetIndex]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает индекс текущего значения 
DropList в списке. Заметьте, что это текущее 
значение не обязательно должно быть 
в списке, в таком случае возвращается 
`-1.&]
[s3; &]
[s4; &]
[s5;:DropList`:`:GoBegin`(`): [@(0.0.255) void]_[* GoBegin]()&]
[s2;%RU-RU Если есть элементы, выделяет самый 
первый.&]
[s3; &]
[s4; &]
[s5;:DropList`:`:GoEnd`(`): [@(0.0.255) void]_[* GoEnd]()&]
[s2;%RU-RU Если есть элементы, выделяет самый 
последний.&]
[s3; &]
[s4; &]
[s5;:DropList`:`:HasKey`(const Value`&`)const: [@(0.0.255) bool]_[* HasKey]([@(0.0.255) con
st]_[_^Value^ Value][@(0.0.255) `&]_[*@3 k])_[@(0.0.255) const]&]
[s2;%RU-RU Проверяет, находится ли ключ [%-*@3 k] 
в этом списке. Поиск быстр (основан 
на [^topic`:`/`/Core`/src`/Indext`_ru`-ru`#Index`:`:Index`(`)^ Index]).&]
[s3; &]
[s4; &]
[s5;:DropList`:`:FindKey`(const Value`&`)const: [@(0.0.255) int]_[* FindKey]([@(0.0.255) co
nst]_[_^Value^ Value][@(0.0.255) `&]_[*@3 k])_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает индекс ключа [%-*@3 k] в 
списке; или `-1, если ключ не найден. 
Поиск быстр (основан на [^topic`:`/`/Core`/src`/Indext`_ru`-ru`#Index`:`:Index`(`)^ I
ndex]).&]
[s3; &]
[s4; &]
[s5;:DropList`:`:Find`(const Value`&`)const: [@(0.0.255) int]_[* Find]([@(0.0.255) const]_[_^Value^ V
alue][@(0.0.255) `&]_[*@3 k])_[@(0.0.255) const]&]
[s2;%RU-RU То же, что и FindKey(k).&]
[s3; &]
[s4; &]
[s5;:DropList`:`:FindValue`(const Value`&`)const: [@(0.0.255) int]_[* FindValue]([@(0.0.255) c
onst]_[_^Value^ Value][@(0.0.255) `&]_[*@3 v])_[@(0.0.255) const]&]
[s2;%RU-RU Находит индекс элемента со значением 
[%-*@3 v]. Если оно отсутствует, возвращает 
`-1.&]
[s3;%RU-RU &]
[s4; &]
[s5;:DropList`:`:GetCount`(`)const: [@(0.0.255) int]_[* GetCount]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает счёт элементов в списке.&]
[s3; &]
[s4; &]
[s5;:DropList`:`:Trim`(int`): [@(0.0.255) void]_[* Trim]([@(0.0.255) int]_[*@3 n])&]
[s2;%RU-RU Обрезает число элементов в списке 
до [%-*@3 n].&]
[s3; &]
[s4; &]
[s5;:DropList`:`:GetKey`(int`)const: [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[* Ge
tKey]([@(0.0.255) int]_[*@3 i])_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает ключ по индексу [%-*@3 i].&]
[s3; &]
[s4; &]
[s5;:DropList`:`:GetValue`(int`)const: [_^Value^ Value]_[* GetValue]([@(0.0.255) int]_[*@3 i])
_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает значение по индексу 
[%-*@3 i].&]
[s3; &]
[s4; &]
[s5;:DropList`:`:GetValue`(`)const: [_^Value^ Value]_[* GetValue]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает первое значение из 
списка, у которого ключ равен текущему 
значению DropList. (Заметьте, что интерфейс 
GetData/SetData DropList`'а использует этот ключ).&]
[s3; &]
[s4; &]
[s5;:DropList`:`:SetValue`(int`,const Value`&`): [@(0.0.255) void]_[* SetValue]([@(0.0.255) i
nt]_[*@3 i], [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 v])&]
[s2;%RU-RU Заменяет значение по индексу 
[%-*@3 i].&]
[s3; &]
[s4; &]
[s5;:DropList`:`:SetValue`(const Value`&`): [@(0.0.255) void]_[* SetValue]([@(0.0.255) cons
t]_[_^Value^ Value][@(0.0.255) `&]_[*@3 v])&]
[s2;%RU-RU Устанавливает первое значение 
в списке, ключ которого равен текущему 
значению DropList.&]
[s3; &]
[s4; &]
[s5;:DropList`:`:operator`[`]`(int`)const: [_^Value^ Value]_[* operator`[`]]([@(0.0.255) in
t]_[*@3 i])_[@(0.0.255) const]&]
[s2;%RU-RU То же, что и GetValue(i).&]
[s3; &]
[s4; &]
[s5;:DropList`:`:Adjust`(`): [@(0.0.255) void]_[* Adjust]()&]
[s2;%RU-RU Проверяет, присутствует ли текущее 
значение DropList`'а в качестве ключа 
в этом списке. Если нет и список не 
пуст, выделяется первый элемент списка.&]
[s3; &]
[s4; &]
[s5;:DropList`:`:Adjust`(const Value`&`): [@(0.0.255) void]_[* Adjust]([@(0.0.255) const]_[_^Value^ V
alue][@(0.0.255) `&]_[*@3 k])&]
[s2;%RU-RU Проверяет, присутствует ли в списке 
текущее значение DropList в качестве 
ключа. Если нет и список не пуст, DropList`'у 
присваивается [%-*@3 k].&]
[s3; &]
[s4; &]
[s5;:DropList`:`:GetList`(`)const: [@(0.0.255) const]_[_^PopUpTable^ PopUpTable][@(0.0.255) `&
]_[* GetList]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает производный от ArrayCtrl 
объект, представляющий выпадающий 
список.&]
[s3; &]
[s4; &]
[s5;:DropList`:`:ListObject`(`): [_^PopUpTable^ PopUpTable][@(0.0.255) `&]_[* ListObject]()
&]
[s2;%RU-RU Возвращает ссылку на внутреннюю 
таблицу PopUpTable, используемую для показа 
элементов.&]
[s3; &]
[s4; &]
[s5;:DropList`:`:SetDropLines`(int`): [_^DropList^ DropList][@(0.0.255) `&]_[* SetDropLines
]([@(0.0.255) int]_[*@3 d])&]
[s2;%RU-RU Устанавливает число строк в выпадающем 
списке.&]
[s3; &]
[s4; &]
[s5;:DropList`:`:SetValueConvert`(const Convert`&`): [_^DropList^ DropList][@(0.0.255) `&
]_[* SetValueConvert]([@(0.0.255) const]_[_^Convert^ Convert][@(0.0.255) `&]_[*@3 cv])&]
[s2;%RU-RU Устанавливает конвертирующий 
объект для значения в области видимости.&]
[s3; &]
[s4; &]
[s5;:DropList`:`:SetConvert`(const Convert`&`): [_^DropList^ DropList][@(0.0.255) `&]_[* Se
tConvert]([@(0.0.255) const]_[_^Convert^ Convert][@(0.0.255) `&]_[*@3 cv])&]
[s2;%RU-RU Устанавливает конвертирующий 
объект для значений списка: значения 
списка вначале преобразуются, а затем 
отображаются либо в области видимости 
DropList`'а, либо в списке. Возвращает 
`*this.&]
[s3; &]
[s4; &]
[s5;:DropList`:`:SetDisplay`(int`,const Display`&`): [_^DropList^ DropList][@(0.0.255) `&
]_[* SetDisplay]([@(0.0.255) int]_[*@3 i], [@(0.0.255) const]_[_^Display^ Display][@(0.0.255) `&
]_[*@3 d])&]
[s2;%RU-RU Устанавливает дисплей для элемента 
[%-*@3 i] из этого списка. Возвращает `*this.&]
[s3; &]
[s4; &]
[s5;:DropList`:`:SetDisplay`(const Display`&`): [_^DropList^ DropList][@(0.0.255) `&]_[* Se
tDisplay]([@(0.0.255) const]_[_^Display^ Display][@(0.0.255) `&]_[*@3 d])&]
[s2; [%RU-RU Устанавливает общий дисплей ][*@3 d][%RU-RU  
для элементов в этом списке и область 
видимости. Возвращает `*this. Если определены 
специфичные дисплеи для элементов 
списка (]SetDisplay[@(64) (][@(0.0.255) int][@(64) _][@3 i][@(64) , 
][@(0.0.255) const][@(64) _][^`:`:Display^@(64) Display][@(64) `&_][@3 d][@(64) )]) 
или области видимости (ValueDisplay[@(64) (][@(0.0.255) const][@(64) _
][^`:`:Display^@(64) Display][@(64) `&_][@3 d][@(64) )]), то они 
имеют преимущество.&]
[s3; &]
[s4; &]
[s5;:DropList`:`:SetLineCy`(int`): [_^DropList^ DropList][@(0.0.255) `&]_[* SetLineCy]([@(0.0.255) i
nt]_[*@3 lcy])&]
[s2;%RU-RU Устанавливает высоту строки элемента 
списка в пикселях. Возвращает `*this.&]
[s3; &]
[s4; &]
[s5;:DropList`:`:SetDisplay`(const Display`&`,int`): [_^DropList^ DropList][@(0.0.255) `&
]_[* SetDisplay]([@(0.0.255) const]_[_^Display^ Display][@(0.0.255) `&]_[*@3 d], 
[@(0.0.255) int]_[*@3 lcy])&]
[s2;%RU-RU То же, что и SetDisplay(d), SetLineCy(lcy). Возвращает 
`*this.&]
[s3; &]
[s4; &]
[s5;:DropList`:`:ValueDisplay`(const Display`&`): [_^DropList^ DropList][@(0.0.255) `&]_[* V
alueDisplay]([@(0.0.255) const]_[_^Display^ Display][@(0.0.255) `&]_[*@3 d])&]
[s2;%RU-RU Устанавливает объект дисплея, 
используемый в области видимости 
DropList`'а. Возвращает `*this.&]
[s3; &]
[s4; &]
[s5;:DropList`:`:DisplayAll`(bool`): [_^DropList^ DropList][@(0.0.255) `&]_[* DisplayAll]([@(0.0.255) b
ool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%RU-RU В этом режиме, если текущее значение 
отсутствует в списке в качестве ключа, 
в области видимости будет отображено 
само значение. Дефолтно отключено. 
Возвращает `*this. Когда режим DisplayAll() 
включен, Convert`::Format возвращает значение 
ключа, если данный ключ отсутствует 
в таблице ключей / значений; когда 
режим DisplayAll() выключен, Convert`::Format возвращает 
Value() для неизвестных ключевых значений.&]
[s0;%RU-RU &]
[s4; &]
[s5;:DropList`:`:DropFocus`(bool`): [_^DropList^ DropList][@(0.0.255) `&]_[* DropFocus]([@(0.0.255) b
ool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%RU-RU В этом режиме, `"выпадание`" и закрытие 
списка (либо при выделении элемента, 
либо при прекращении выделения) переносит 
клавиатурный фокус в этот виджет. 
По дефолту включено. Возвращает `*this.&]
[s3; &]
[s4; &]
[s5;:DropList`:`:NoDropFocus`(`): [_^DropList^ DropList][@(0.0.255) `&]_[* NoDropFocus]()&]
[s2;%RU-RU То же, что и DropFocus(false).&]
[s3; &]
[s4; &]
[s5;:DropList`:`:AlwaysDrop`(bool`): [_^DropList^ DropList][@(0.0.255) `&]_[* AlwaysDrop]([@(0.0.255) b
ool]_[*@3 e]_`=_[@(0.0.255) true])&]
[s2;%RU-RU Если этот режим активен, кнопка 
`"выброса`" активна, даже если список 
пуст, позволяя клиентскому коду создавать 
список, пока не произойдёт обрвыз 
WhenDrop.&]
[s3; &]
[s4; &]
[s5;:DropList`:`:SetStyle`(const DropList`:`:Style`&`): [_^DropList^ DropList][@(0.0.255) `&
]_[* SetStyle]([@(0.0.255) const]_[_^DropList`:`:Style^ Style][@(0.0.255) `&]_[*@3 s])&]
[s2;%RU-RU Устанавливает хамелеон`-стиль 
DropList`'а.&]
[s3;%RU-RU &]
[s4; &]
[s5;:DropList`:`:NotNull`(bool`): [_^DropList^ DropList][@(0.0.255) `&]_[* NotNull]([@(0.0.255) b
ool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%RU-RU Назначает значение Null, как допустимое 
для DropList. Если активен, значение Null 
не [^Ctrl`:`:Accept^ принимается].&]
[s3;%RU-RU &]
[s4; &]
[s5;:DropList`:`:DropWidth`(int`): [_^DropList^ DropList][@(0.0.255) `&]_[* DropWidth]([@(0.0.255) i
nt]_[*@3 w])&]
[s2;%RU-RU Переписывает ширину у `"выпавшего`" 
выпадающего списка PopUpTable (которая 
дефолтно равна ширине DropList`'а).&]
[s3;%RU-RU &]
[s4; &]
[s5;:DropList`:`:DropWidthZ`(int`): [_^DropList^ DropList][@(0.0.255) `&]_[* DropWidthZ]([@(0.0.255) i
nt]_[*@3 w])&]
[s2;%RU-RU То же, что и DropWidth(HorzLayoutZoom([%-*@3 w])) 
`- настраивает ширину в соответствии 
с текущим масштабированием стандартного 
шрифта ГИП.&]
[s3;%RU-RU &]
[s4; &]
[s5;:DropList`:`:Wheel`(bool`): [_^DropList^ DropList][@(0.0.255) `&]_[* Wheel]([@(0.0.255) b
ool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%RU-RU Если активен, значение DropList`'а 
можно менять колёсиком мыши. По дефолту 
активно.&]
[s3;%RU-RU &]
[s4; &]
[s5;:DropList`:`:NoWheel`(`): [_^DropList^ DropList][@(0.0.255) `&]_[* NoWheel]()&]
[s2;%RU-RU То же, что и Wheel(false).&]
[s3; &]
[s4; &]
[s5;:DropList`:`:SetScrollBarStyle`(const ScrollBar`:`:Style`&`): [_^DropList^ DropList
][@(0.0.255) `&]_[* SetScrollBarStyle]([@(0.0.255) const]_[_^ScrollBar`:`:Style^ ScrollBa
r`::Style][@(0.0.255) `&]_[*@3 s])&]
[s2;%RU-RU Устанавливает стиль `"Хамелеон`" 
Scrollbar`'а выпадающего списка PopUpTable.&]
[s3;%RU-RU &]
[s4; &]
[s5;:DropList`:`:operator`=`(const Value`&`): [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&
]_[* operator`=]([@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 v])&]
[s5;:DropList`:`:operator Value`(`)const: [@(0.0.255) operator]_Value()_[@(0.0.255) const
]&]
[s2;%RU-RU Эти методы деприкированы в пользу 
operator`~ и operator<<`=.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Относящиеся к DropList Глобальные 
Функции]]}}&]
[s9;%RU-RU Это простые вспомогательные функции 
для заполнения списка DropList`'а и MapConvert.&]
[s3;%RU-RU &]
[s3;%RU-RU &]
[s5;:Append`(DropList`&`,const VectorMap`<Value`,Value`>`&`): [@(0.0.255) void]_[* Append
]([_^DropList^ DropList][@(0.0.255) `&]_[*@3 list], [@(0.0.255) const]_[_^VectorMap^ Vector
Map]<[_^Value^ Value], [_^Value^ Value]>`&_[*@3 values])&]
[s5;:Append`(DropList`&`,const VectorMap`<int`,String`>`&`): [@(0.0.255) void]_[* Append](
[_^DropList^ DropList][@(0.0.255) `&]_[*@3 list], [@(0.0.255) const]_[_^VectorMap^ VectorMa
p]<[@(0.0.255) int], [_^String^ String]>`&_[*@3 values])&]
[s2;%RU-RU Заполняет DropList из VectorMap.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Append`(MapConvert`&`,const VectorMap`<Value`,Value`>`&`): [@(0.0.255) void]_[* Appe
nd]([_^MapConvert^ MapConvert][@(0.0.255) `&]_[*@3 convert], [@(0.0.255) const]_[_^VectorMap^ V
ectorMap]<[_^Value^ Value], [_^Value^ Value]>`&_[*@3 values])&]
[s5;:Append`(MapConvert`&`,const VectorMap`<int`,String`>`&`): [@(0.0.255) void]_[* Appen
d]([_^MapConvert^ MapConvert][@(0.0.255) `&]_[*@3 convert], [@(0.0.255) const]_[_^VectorMap^ V
ectorMap]<[@(0.0.255) int], [_^String^ String]>`&_[*@3 values])&]
[s2;%RU-RU Заполняет MapConvert из VectorMap.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Append`(DropList`&`,const MapConvert`&`): [@(0.0.255) void]_[* Append]([_^DropList^ Dr
opList][@(0.0.255) `&]_[*@3 list], [@(0.0.255) const]_[_^MapConvert^ MapConvert][@(0.0.255) `&
]_[*@3 convert])&]
[s2;%RU-RU Заполняет DropList из MapConvert.&]
[s3;%RU-RU &]
[s4; &]
[s5;:operator`*`=`(DropList`&`,const VectorMap`<Value`,Value`>`&`): [@(0.0.255) void]_[* o
perator`*`=]([_^DropList^ DropList][@(0.0.255) `&]_[*@3 list], [@(0.0.255) const]_[_^VectorMap^ V
ectorMap]<[_^Value^ Value], [_^Value^ Value]>`&_[*@3 values])&]
[s5;:operator`*`=`(DropList`&`,const VectorMap`<int`,String`>`&`): [@(0.0.255) void]_[* o
perator`*`=]([_^DropList^ DropList][@(0.0.255) `&]_[*@3 list], [@(0.0.255) const]_[_^VectorMap^ V
ectorMap]<[@(0.0.255) int], [_^String^ String]>`&_[*@3 values])&]
[s2;%RU-RU Очищает и заполняет DropList из VectorMap.&]
[s3;%RU-RU &]
[s4; &]
[s5;:operator`*`=`(MapConvert`&`,const VectorMap`<Value`,Value`>`&`): [@(0.0.255) void]_
[* operator`*`=]([_^MapConvert^ MapConvert][@(0.0.255) `&]_[*@3 convert], 
[@(0.0.255) const]_[_^VectorMap^ VectorMap]<[_^Value^ Value], [_^Value^ Value]>`&_[*@3 valu
es])&]
[s5;:operator`*`=`(MapConvert`&`,const VectorMap`<int`,String`>`&`): [@(0.0.255) void]_
[* operator`*`=]([_^MapConvert^ MapConvert][@(0.0.255) `&]_[*@3 convert], 
[@(0.0.255) const]_[_^VectorMap^ VectorMap]<[@(0.0.255) int], [_^String^ String]>`&_[*@3 va
lues])&]
[s2;%RU-RU Очищает и заполняет MapConvert из VectorMap.&]
[s3;%RU-RU &]
[s4; &]
[s5;:operator`*`=`(DropList`&`,const MapConvert`&`): [@(0.0.255) void]_[* operator`*`=]([_^DropList^ D
ropList][@(0.0.255) `&]_[*@3 list], [@(0.0.255) const]_[_^MapConvert^ MapConvert][@(0.0.255) `&
]_[*@3 convert])&]
[s2;%RU-RU Очищает и заполняет DropList из MapConvert.&]
[s3;%RU-RU &]
[s0;%RU-RU ]]