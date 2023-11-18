topic "Класс WithDropChoice";
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
[ {{10000@3 [s0; [*@(229)4 Класс WithDropChoice]]}}&]
[s3; &]
[s1;:noref: [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@3;3 T][@(0.0.255)3 >]&]
[s1;:WithDropChoice`:`:class: [@(0.0.255) class]_[* WithDropChoice]_:_[@(0.0.255) public]_[*@3 T
]&]
[s0;%RU-RU &]
[s2;%RU-RU 
@@image:1350&331
(A9gANQAAAMv/AAAAAHic7ZyxTttAGIDzSjxH36EP0KFrK8YGhi6gMmUoZUFCLJ0oyogKQUItIkIIVVUpDjE1hEilhbaj+0MU1707J2fHds7o+/QNtrGTIH3c5SCmf/mpj4iIQ5++fItYssYOQ4ASoUMogg+frxvvzmyUM0M6hGKQwM77f2yUM0M6hGKQuvyr3zbad1gbMqXvCaqH1NXt/bLRssN4fvmmaP9o/AhUDqnr7PLWxgwd5gsdPmDuO7wZ+Ojxs2hbPzJhh8pkHd9VJvGkaT3p8viTGh8Q3Efq6lzciJLcwMGufmSS94f69oiEjFcZL0l6NCKsHFKXF/wcGIWnbA/MsF62Lyf8fygzDon6YKiPkPrTQSW47/BHZJTfMMJ/Xyqtw6SXOjozOqw0Utfpt+u4UYTK8UnWy8zLMBqp6+v5d0WJUD844e8PR68slI2khYblOiUkxaph7NAof0+B4rjrsHthIx1Ccdx16Hk20iEUB5+3gSqS1CFiyeod1gBKx9jhzJMWYmnSIbrg2A6V95NTf8H4ILUcD9MWSLGYSjpEF5TkvJN9v9MO/KNecGzfoTJTKxtM4phKSW5vt3l4sHX65aOkaNlhfDcKz7iLaKMkt7baaG6uS4pdr23fob54MQ6PiDZKcosLdUlRRkWZoDOMh/GDdIjZlOTm52ZXlpda2xsyNTMv41SU5Oovnr95/WrnfYoOZ8atU4yXICZp2SFiodIhumBSh+k/QQaQHToEF6BDcAE6BBegQ3CBsR3WuJMdiofxEFyADsEF6BBcgA7BBVingAswHoIL0CG4QFKHACWjd6jonezv7TbXVhuLC/X5uVk5HzF39ftTFP1O+/Bgq7m5LqetLC9JtIi5q9+vpxj4RzJUygnSamt7Q0ZOxNzV719W7AXH8iWpVAZMOQ2xCOP/z+EvcWfmOg==)
&]
[s0;%RU-RU &]
[s9;%RU-RU Этот шаблонный класс добавляет 
список выборов, с правой стороны фрейма 
которого помещена кнопка, обеспечивающая 
`"выпадение`", любому виджету с ассоциированн
ым значением, доступным через интерфейс 
SetData/GetData.&]
[s3;%RU-RU &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Публичных Членов]]}}&]
[s3; &]
[s5;:WithDropChoice`:`:WhenDrop: [_^Callback^ Callback]_[* WhenDrop]&]
[s2;%RU-RU Этот обрвыз вызывается при нажатии 
кнопки `"выпадения`" (перед появлением 
выпадающего списка). Клиентский код 
может использовать его для создания 
или изменения списка.&]
[s3;%RU-RU &]
[s4; &]
[s5;:WithDropChoice`:`:WhenSelect: [_^Callback^ Callback]_[* WhenSelect]&]
[s2;%RU-RU Этот обрвыз вызывается после 
выбора пользователем элемента в выпадающем 
списке и присваивает выбранное значение 
базовому виджету.&]
[s3;%RU-RU &]
[s4; &]
[s5;:WithDropChoice`:`:ClearList`(`): [@(0.0.255) void]_[* ClearList]()&]
[s2;%RU-RU Очищает выпадающий список.&]
[s3;%RU-RU &]
[s4; &]
[s5;:WithDropChoice`:`:AddList`(const Value`&`): [@(0.0.255) void]_[* AddList]([@(0.0.255) c
onst]_[_^Value^ Value][@(0.0.255) `&]_[*@3 data])&]
[s2;%RU-RU Добавляет данные [%-*@3 data] в выпадающий 
список.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:WithDropChoice`:`:FindAddList`(const Upp`:`:Value`&`): [@(0.0.255) void]_[* F
indAddList]([@(0.0.255) const]_[_^Upp`:`:Value^ Value][@(0.0.255) `&]_[*@3 data])&]
[s2;%RU-RU Добавляет данные [%-*@3 data] в выпадающий 
список, но только, если их там ещё 
нет.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:WithDropChoice`:`:Find`(const Upp`:`:Value`&`)const: [@(0.0.255) int]_[* Find
]([@(0.0.255) const]_[_^Upp`:`:Value^ Value][@(0.0.255) `&]_[*@3 data])_[@(0.0.255) const]&]
[s2;%RU-RU Находит индекс первой записи 
списка, равной [%-*@3 data].&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:WithDropChoice`:`:Set`(int`,const Upp`:`:Value`&`): [@(0.0.255) void]_[* Set](
[@(0.0.255) int]_[*@3 i], [@(0.0.255) const]_[_^Upp`:`:Value^ Value][@(0.0.255) `&]_[*@3 data
])&]
[s2;%RU-RU Устанавливает запись [%-*@3 i] выпадающего 
списка в значение [%-*@3 data] .&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:WithDropChoice`:`:Remove`(int`): [@(0.0.255) void]_[* Remove]([@(0.0.255) int]_
[*@3 i])&]
[s2;%RU-RU Удаляет запись [%-*@3 i] в выпадающем 
списке.&]
[s3;%RU-RU &]
[s4; &]
[s5;:WithDropChoice`:`:SerializeList`(Stream`&`): [@(0.0.255) void]_[* SerializeList]([_^Stream^ S
tream][@(0.0.255) `&]_[*@3 s])&]
[s2;%RU-RU Сериализует контент выпадающего 
списка.&]
[s3;%RU-RU &]
[s4; &]
[s5;:WithDropChoice`:`:AddHistory`(int`): [@(0.0.255) void]_[* AddHistory]([@(0.0.255) int]_
[*@3 max]_`=_[@3 12])&]
[s2;%RU-RU Помещает значение в начало выпадающего 
списка. Если оно уже в списке, оно 
вначале удаляется (значения в списке 
уникальны). Если в списке более чем 
[%-*@3 max] элементов,список укорачивается. 
Метод эффективен, когда нужно сохранить 
историю предыдущих значений.&]
[s3;%RU-RU &]
[s4; &]
[s5;:WithDropChoice`:`:Dropping`(bool`): [_^WithDropChoice^ WithDropChoice][@(0.0.255) `&
]_[* Dropping]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%RU-RU В этом режиме, кнопка `"выпадания`" 
видна. Может использоваться для временного 
отключения функции `"выпадения`" списка. 
Дефолтно включен.&]
[s3;%RU-RU &]
[s4; &]
[s5;:WithDropChoice`:`:NoDropping`(`): [_^WithDropChoice^ WithDropChoice][@(0.0.255) `&]_
[* NoDropping]()&]
[s2;%RU-RU То же, что и Dropping(false).&]
[s3;%RU-RU &]
[s4; &]
[s5;:WithDropChoice`:`:NoDropFocus`(`): [_^WithDropChoice^ WithDropChoice][@(0.0.255) `&]_
[* NoDropFocus]()&]
[s2;%RU-RU Если NoDropFocus активен, то при нажатии 
кнопки `"выпадения`" фокус не перемещается 
в базовый виджет.Дефолтно фокус перемещаетс
я.&]
[s3;%RU-RU &]
[s4; &]
[s5;:WithDropChoice`:`:Appending`(const String`&`,`"`): [_^WithDropChoice^ WithDropChoi
ce][@(0.0.255) `&]_[* Appending]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 s]_
`=_`", `")&]
[s2; [*@3 s]&]
[s3;%RU-RU &]
[s4; &]
[s5;:WithDropChoice`:`:SetDropLines`(int`): [_^WithDropChoice^ WithDropChoice][@(0.0.255) `&
]_[* SetDropLines]([@(0.0.255) int]_[*@3 n])&]
[s2;%RU-RU Устанавливает максимальную высоту 
выпадающего списка на вмещение в 
него [%-*@3 n] строк (если их больше, появляется 
полоса промотки).&]
[s3;%RU-RU &]
[s4; &]
[s5;:WithDropChoice`:`:SetDisplay`(int`,const Display`&`): [_^WithDropChoice^ WithDropC
hoice][@(0.0.255) `&]_[* SetDisplay]([@(0.0.255) int]_[*@3 i], [@(0.0.255) const]_[_^Display^ D
isplay][@(0.0.255) `&]_[*@3 d])&]
[s2;%RU-RU Устанавливает дисплей, который 
будет использоваться с элементом 
выпадающего списка [%-*@3 i].&]
[s3;%RU-RU &]
[s4; &]
[s5;:WithDropChoice`:`:SetDisplay`(const Display`&`): [_^WithDropChoice^ WithDropChoice
][@(0.0.255) `&]_[* SetDisplay]([@(0.0.255) const]_[_^Display^ Display][@(0.0.255) `&]_[*@3 d
])&]
[s2;%RU-RU Устанавливает дисплей, используемый 
с выпадающим списком, если, конечно, 
для элемента не установлен специфичный 
дисплей.&]
[s3;%RU-RU &]
[s4; &]
[s5;:WithDropChoice`:`:SetLineCy`(int`): [_^WithDropChoice^ WithDropChoice][@(0.0.255) `&
]_[* SetLineCy]([@(0.0.255) int]_[*@3 lcy])&]
[s2;%RU-RU Устанавливает высоту строки дисплей 
выпадения.&]
[s3;%RU-RU &]
[s4; &]
[s5;:WithDropChoice`:`:SetDisplay`(const Display`&`,int`): [_^WithDropChoice^ WithDropC
hoice][@(0.0.255) `&]_[* SetDisplay]([@(0.0.255) const]_[_^Display^ Display][@(0.0.255) `&]_
[*@3 d], [@(0.0.255) int]_[*@3 lcy])&]
[s2;%RU-RU То же, что и SetDisplay(d).SetLineCy(lcy).&]
[s3;%RU-RU &]
[s4; &]
[s5;:WithDropChoice`:`:SetConvert`(const Convert`&`): [_^WithDropChoice^ WithDropChoice
][@(0.0.255) `&]_[* SetConvert]([@(0.0.255) const]_[_^Convert^ Convert][@(0.0.255) `&]_[*@3 d
])&]
[s2;%RU-RU Устанавливает Convert, который будет 
использоваться с элементом выпадающего 
списка [%-*@3 i].&]
[s3;%RU-RU &]
[s4; &]
[s5;:WithDropChoice`:`:AlwaysDrop`(bool`): [_^WithDropChoice^ WithDropChoice][@(0.0.255) `&
]_[* AlwaysDrop]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%RU-RU Если активен, кнопка выпадания 
видна, даже если список пуст (клиентский 
код может заполнить его, используя 
обрвыз WhenDrop).&]
[s3; &]
[s4; &]
[s5;:Upp`:`:WithDropChoice`:`:HideDrop`(bool`): [_^Upp`:`:WithDropChoice^ WithDropChoic
e][@(0.0.255) `&]_[* HideDrop]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%RU-RU Вместо отключения стрелки выпадения 
при пустом списке и AlwaysDrop равном false, 
сконпа выпадания скрывается.&]
[s3;%RU-RU &]
[s4; &]
[s5;:WithDropChoice`:`:RdOnlyDrop`(bool`): [_^WithDropChoice^ WithDropChoice][@(0.0.255) `&
]_[* RdOnlyDrop]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%RU-RU Если активен, позволяет изменять 
значение, даже если оно только для 
чтения.&]
[s3;%RU-RU &]
[s4; &]
[s5;:WithDropChoice`:`:WithWheel`(bool`): [_^WithDropChoice^ WithDropChoice][@(0.0.255) `&
]_[* WithWheel]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%RU-RU Виджет реагирует на колёсико 
мыши. Активно дефолтно.&]
[s3;%RU-RU &]
[s4; &]
[s5;:WithDropChoice`:`:NoWithWheel`(`): [_^WithDropChoice^ WithDropChoice][@(0.0.255) `&]_
[* NoWithWheel]()&]
[s2;%RU-RU То же, что и WithWheel(false).&]
[s3; &]
[s4; &]
[s5;:WithDropChoice`:`:DropWidth`(int`): [_^WithDropChoice^ WithDropChoice][@(0.0.255) `&
]_[* DropWidth]([@(0.0.255) int]_[*@3 w])&]
[s2;%RU-RU Устанавливает ширину выпадающего 
списка в [%-*@3 w].&]
[s3;%RU-RU &]
[s4; &]
[s5;:WithDropChoice`:`:DropWidthZ`(int`): [_^WithDropChoice^ WithDropChoice][@(0.0.255) `&
]_[* DropWidthZ]([@(0.0.255) int]_[*@3 w])&]
[s2;%RU-RU Устанавливает ширину выпадающего 
списка в значение, полученное масштабирован
ием (зумом) [%-*@3 w] в соответствии с масштабиров
анием шрифта.&]
[s3;%RU-RU &]
[s4; &]
[s5;:WithDropChoice`:`:UpDownKeys`(bool`): [_^WithDropChoice^ WithDropChoice][@(0.0.255) `&
]_[* UpDownKeys]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%RU-RU Если активен, клавиши Вверх и 
Вниз проматывают историю циклически. 
Дефолтно активен.&]
[s3;%RU-RU &]
[s4; &]
[s5;:WithDropChoice`:`:NoUpDownKeys`(`): [_^WithDropChoice^ WithDropChoice][@(0.0.255) `&
]_[* NoUpDownKeys]()&]
[s2;%RU-RU То же, что и UpDownKeys(false).&]
[s3;%RU-RU &]
[s4; &]
[s5;:WithDropChoice`:`:GetCount`(`)const: [@(0.0.255) int]_[* GetCount]()_[@(0.0.255) const
]&]
[s2;%RU-RU Возвращает число элементов в 
выпадающем списке.&]
[s3;%RU-RU &]
[s4; &]
[s5;:WithDropChoice`:`:Get`(int`)const: [_^Value^ Value]_[* Get]([@(0.0.255) int]_[*@3 i])_[@(0.0.255) c
onst]&]
[s2;%RU-RU Возвращает элемент с индексом 
[%-*@3 i ]из выпадающего списка.&]
[s3;%RU-RU &]
[s4; &]
[s5;:WithDropChoice`:`:StyleDefault`(`): [@(0.0.255) const]_[_^MultiButton`:`:Style^ Mult
iButton`::Style][@(0.0.255) `&]_[* StyleDefault]()&]
[s2;%RU-RU Возвращает дефолтный стиль фрейма 
выпадающего списка, добовленный этому 
виджету.&]
[s3; &]
[s4; &]
[s5;:WithDropChoice`:`:SetStyle`(const MultiButton`:`:Style`&`): [_^WithDropChoice^ Wit
hDropChoice][@(0.0.255) `&]_[* SetStyle]([@(0.0.255) const]_[_^MultiButton`:`:Style^ Mult
iButton`::Style][@(0.0.255) `&]_[*@3 s])&]
[s2;%RU-RU Устанавливает стиль фрейма [%-_^MultiButton`:`:Style^ M
ultiButton`::Style] [%-*@3 s].&]
[s3;%RU-RU &]
[s0; ]]