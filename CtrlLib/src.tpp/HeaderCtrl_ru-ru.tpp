topic "Класс HeaderCtrl (ЗагКтрл)";
[i448;a25;kKO9;*@(64)2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[a83;*R6 $$3,0#31310162474203024125188417583966:caption]
[l288;i1121;b17;O9;~~~.1408;2 $$4,0#10431211400427159095818037425705:param]
[i448;a25;kKO9;*@(64)2 $$5,0#37138531426314131252341829483370:item]
[*+117 $$6,6#14700283458701402223321329925657:header]
[0 $$7,0#96390100711032703541132217272105:end]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_}%RU-RU 
[ {{10000@3 [s3;~~~64;%- [%RU-RUA@7$3;4 Класс][%RU-RUA$3;4  ][@7$3 HeaderCtrl 
(ЗагКтрл)]]}}&]
[s0;:Upp`:`:HeaderCtrl`:`:class:%- [@(0.0.255)3 class][3 _][*3 HeaderCtrl][3 _:_][@(0.0.255)3 pu
blic][3 _][*@3;3 Ctrl][3 , ][@(0.0.255)3 public][3 _][*@3;3 CtrlFrame]&]
[s2; &]
[s2; 
@@image:1350&150
(A9gAGAAAAOj/AAAAAHic7ZnBDcIwDEWZmD0YgQ16QFxBYgEODMAKSN0BTlTgxG7SOo6TfuuJQxrqfv6Xg9Tx9RwBAAB82R/OAFSB5PCNQpkXl8Pqk9me03CE8FrdkUMndmxWOHLoyo7NCs/N4e6/uNsKlxZA7qZ780Q7EoW3S7rw8FOre+48nO1ewikb9+Wx0GUCZeGcZOSwih2Wz1CFPnJIDizhCIte4laIZKI9bEq+qGhHVHi09aw0n6TnsIlzOXw8zjthc+gvt0hWyB5FO2Qts0IULStEyv9DwRSV7orzUCWH8raipqvnMDokHdLBPMyKRPpmzuW2cqhikwHIoeK5LPx6K+0QTEmZ4YqWFaKDHE52/J7L3GEUvcStkHZhIOUYKNohm5IlzSct5nANzh3Jfa3Q0MTTFV6iO3K4wI5wyjmXpiW8XHe8X3Zix2aFCzl83K8AmMHl8HYZADBDOJdRKMtCDlEeCjlEeagphx8wHNk4)
&]
[s2; &]
[s0; [/ Производный от] Ctrl, [@(0.0.255) CtrlFrame]&]
[s0; &]
[s0; HeaderCtrl используется, главным образом, 
для заголовочников таблиц. Его можно 
настроить под определённую колоночную 
структуру (в терминологии кода, индивидуальн
ые заголовочные колонки называются 
[/ вкладки]), включая визуальные характеристик
и вкладок (имена титулов вкладок, 
иконки и т.д.) и поведенческие констрейнты 
(пределы ширины вкладки, способность 
показывать/скрывать отдельные вкладки).&]
[s0; &]
[s0; Ширины индивидуальной вкладки определены 
тремя базовыми свойствами `- [/ логической 
шириной вкладки], её [/ минимальной 
]и [/ максимальной пиксельной шириной]. 
Минимальная и максимальная ширина 
вкладки `- это конечные пиксельные 
значения, определяющие констрейнты 
для физических (конечных) ширин колонок. 
С другой стороны, логические ширины 
вкладок задаются в произвольных единицах, 
которые умножаются на определённый 
фактор масштабирования всякий раз, 
когда контрол`-заголовочник выкладывается, 
подстраиваясь под размер видимой 
области. Следовательно, их абсолютные 
величины не имеют значения, а только 
их пропорции.&]
[s0; &]
[s0; Контрол`-заголовочник может работать 
в четырёх разных визуальных режимах. 
Каждый режим эффективно определяет 
то, как относительные логические 
ширины вкладок встраиваются в текущий 
размер видимой области:&]
[s0; &]
[s0;i150;O0; [* Пропорциональный]: ширины вкладок 
пропорционально регулируются, соблюдая 
предустановленные ограничения (констрейнты
) ширин, заполняя текущую ширину видимости;&]
[s0;i150;O0; [* УменьшитьПоследний]: при каждой 
установке ширины вкладки, когда общая 
ширина вкладок превосходит размер 
видимой области, ширины следующих 
вкладок (вкладки справа) уменьшаются 
в порядке справа налево (начиная с 
самой правой вкладки), пока не попадут 
в размер видимой области;&]
[s0;i150;O0; [* УменьшитьСледующий]: при каждой 
установке ширины вкладки, когда общая 
ширина вкладок превосходит размер 
видимой области, ширины следующих 
вкладок (вкладки справа) уменьшаются 
в порядке слева направо (начиная с 
вкладки, находящейся сразу за регулируемой),
 пока не попадут в размер видимой 
области;&]
[s0;i150;O0; [* Абсолютный]: ширины вкладок не 
регулируются автоматически, логическая 
ширина заголовочника может быть меньше 
или больше действительного размера 
видимой области. При каждом превышении 
общей ширины вкладок размера видимой 
области, этот заголовочник может 
проматываться влево или вправо, панируя 
свою видимую порцию в пределах области 
видимости родителя.&]
[s0; &]
[s0; С точки зрения программиста, HeaderCtrl 
полностью контролирует распределение 
вкладок и необходимый для их регулирования 
пользовательский интерфейс. Контрол`-хост 
может использовать контрол`-заголовочник 
(как правило во время операций [/ Layout] 
или [/ Paint]), чтобы получить горизонтальную 
пиксельную позицию отдельных разрывов 
вкладок и использовать их для размещения 
и отрисовки данных колонки.&]
[s0; &]
[s0; Если используется модификатор Moving, 
HeaderCtrl позволяет переупорядочивать 
вкладки, используя операцию тяга 
`& броса. Клиентский код может идентифицирова
ть этот порядок, используя метод GetTabIndex, 
который возвращает `"оригинальный`" 
индекс для данной вкладки.&]
[s0; &]
[s0; HeaderCtrl реализует [/ интерфейс фрейма]; 
если он `"приторочен`" к родительскому 
контролу методом [/ AddFrame], он автоматически 
самопозиционируется поверх прямоугольника 
этого контрола (над его видимой областью).&]
[s0; &]
[s0; В дополнение к стандартному функционалу 
титула вкладки, прямоугольные области, 
соответствующие отдельным вкладкам, 
могут действовать как псевдо`-кнопки, 
вызывая некий функционал при клике. 
У каждой вкладки`-заголовочника есть 
обрвыз [/ WhenAction]; если установлен не 
в нулевое значение, контрол заголовочник 
начинает проверять клики мыши и выполнять 
этот обрвыз, когда нажимается соответствующ
ая вкладка. При использовании для 
вызова всплывающих окон, методом 
[/ GetTabRect] можно определять действительное 
положение определённой вкладки заголовочни
ка и размещать ниспадающий контрол 
в соответствии с положением прямоугольника 
вкладки.&]
[s0; &]
[s5;K:Upp`:`:HeaderCtrl`:`:GetTabRect`(int`):%- Rect_[@0 GetTabRect]([@(0.0.255) int]_[@3 i
])&]
[s2; Возвращает ограничивающий бокс определённо
й вкладки (в координатах, относительных 
к видимости HeaderCtrl). Может использоваться 
контролами, использующими заголовочник 
(наподобии ArrayCtrl) для определения 
положений разрыва колонок или хостовыми 
приложениями для позиционирования 
выпадающих контролов (активируемых 
при клике на вкладке) относительно 
бокса вкладки внутри заголовочника.&]
[s4; [*C@3 i]-|Индекс вкладки (с нулевым основанием).&]
[s4; [*/ Возвратное значение]-|прямоугольный 
ограничивающий бокс вкладки внутри 
HeaderCtrl.&]
[s0;* &]
[s0; [* Подсказка:] для размещения выпадающих 
контролов (напр.,  выпадающих меню) 
относительно прямоугольника вкладки, 
нужно трансформировать возвращённый 
прямоугольник в экранные координаты. 
Это можно выполнить, например, используя 
следующий сниппет кода:&]
[s0; &]
[s0; [C -|Rect tab2`_scr`_rect `= tab.GetTabRect(2) `+ tab.GetScreenView().TopLeft();]&]
[s0;3 &]
[s5;:Upp`:`:HeaderCtrl`:`:Tab`(int`)const: [@(0.0.255) const]_[^`:`:Column^ Column]`&_[@0 T
ab]([@(0.0.255) int]_[@3 i][@0 )_const]&]
[s2; Возвращает описательную структуру 
[* HeaderCtrl`::Column] данной вкладки заголовочника. 
Эту константную версию можно использовать 
для получения свойств индивидуальной 
вкладки. Более подробное описание 
структуры [* Column ]можно найти ниже.&]
[s4; [*C@3 i]-|Индекс вкладки внутри заголовочника 
(на основании нуль).&]
[s4; [*/ Возвратное значение]-|(константная) 
ссылка на структуру [* Column], описывающую 
данную вкладку.&]
[s0;3 &]
[s5;:Upp`:`:HeaderCtrl`:`:Tab`(int`): Column[%- `&]_[@0 Tab]([@(0.0.255) int]_[@3 i][@0 )]&]
[s2; Возвращает (неконстантную) ссылку 
на описательную структуру [* HeaderCtrl`::Column] 
данной вкладки заголовочника. Её 
можно использовать для установки 
и получения свойств индивидуальной 
вкладки (более подробное описание 
структуры [* Column ]можно найти ниже).&]
[s4; [*C@3 i]-|Индекс вкладки (с нулевым основанием).&]
[s4; [*/ Возвратное значение]-|неконстантная 
ссылка на структуру [* Column], описывающую 
данную вкладку.&]
[s0;3 &]
[s5;K:Upp`:`:HeaderCtrl`:`:Add`(const char`*`,double`):%- Column`&_[@0 Add]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[@3 text], [@(0.0.255) double]_[@3 ratio]_`=_[@3 0])&]
[s2; Добавляет новую вкладку после (справа 
от) всех существующих вкладок`-заголовочнико
в. Эта функция возвращает (неконстантную) 
ссылку на структуру [* HeaderCtrl`::Column], которую 
затем можно использовать для настройти 
(установки) дополнительных свойств 
вкладки.&]
[s4; [*C@3 text]-|Имя вкладки (титул).&]
[s4; [*C@3 ratio]-|Логическая ширина вкладки.&]
[s4; [*/ Возвратное значение]-|неконстантная 
ссылка на структуру [* Column], описывающую 
вновь добавленную вкладку.&]
[s0;3 &]
[s5;K:Upp`:`:HeaderCtrl`:`:Add`(`):%- Column`&_[@0 Add]()&]
[s2; Добавляет новую вкладку после (справа 
от)  всех существующих вкладок`-заголовочник
ов. Это идентично версии выше, с аргументом 
[%-@3 text ]установленным как пустая строка 
и [%-@3 ratio ]равным 0.&]
[s4; [*/ Возвратное значение]-|неконстантная 
ссылка на структуру [* Column], описывающую 
вновь добавленную вкладку.&]
[s0;3 &]
[s5;K:Upp`:`:HeaderCtrl`:`:operator`[`]`(int`)const:%- [@(0.0.255) const]_[^`:`:HeaderCtrl`:`:Column^ C
olumn]`&_[@0 operator`[`]]([@(0.0.255) int]_[@3 i])_[@(0.0.255) const]&]
[s2; Возвращает (константную) ссылку на 
заданную вкладку`-заголовочник. Идентично 
версии [/ const] метода [* Tab].&]
[s4; [*C@3 i]-|Индекс вкладки (на основании 
ноль).&]
[s4; [*/ Возвратное значение]-|константная 
ссылка на заданную вкладку`-заголовочник.&]
[s0;3 &]
[s5;K:Upp`:`:HeaderCtrl`:`:GetCount`(`)const:%- [@(0.0.255) int]_[@0 GetCount]()_[@(0.0.255) c
onst]&]
[s2; Возвращает текущее число вкладок`-заголовоч
ников. Каждый [* Add] увеличивает это 
значение на единицу, а [* Reset] устанавливает 
его в 0 (начальное значение).&]
[s4; [*/ Возвратное значение]-|Число вкладок.&]
[s0;3 &]
[s5;K:Upp`:`:HeaderCtrl`:`:Reset`(`):%- [@(0.0.255) void]_[@0 Reset]()&]
[s2; Сбрасывает вкладку`-заголовочник 
в его начальное состояние (удаляет 
все вкладки).&]
[s0;3 &]
[s5;K:Upp`:`:HeaderCtrl`:`:ShowTab`(int`,bool`):%- [@(0.0.255) void]_[@0 ShowTab]([@(0.0.255) i
nt]_[@3 i], [@(0.0.255) bool]_[@3 show]_`=_true)&]
[s2; Показывает или скрывает определённую 
вкладку`-заголовочник.&]
[s4; [*C@3 i]-|индекс вкладки (с основанием 
ноль)&]
[s4; [*C@3 show]-|флаг, указывающий, показывать 
ли вкладку ([* true]) или же скрывать её 
([* false]).&]
[s0;3 &]
[s5;K:Upp`:`:HeaderCtrl`:`:HideTab`(int`):%- [@(0.0.255) void]_[@0 HideTab]([@(0.0.255) int
]_[@3 i])&]
[s2; Скрывает определённую вкладку`-заголовочни
к. Идентично [* ShowTab(i, false)].&]
[s4; [*C@3 i]-|индекс вкладки (с основанием 
ноль)&]
[s0;3 &]
[s5;K:Upp`:`:HeaderCtrl`:`:IsTabVisible`(int`):%- [@(0.0.255) bool]_[@0 IsTabVisible]([@(0.0.255) i
nt]_[@3 i])&]
[s2; Проверяет видимость определённой 
вкладки.&]
[s4; [*C@3 i]-|индекс вкладки (с основанием 
ноль).&]
[s4; [*/ Возвратное значение]-|[* false] `= скрыта, 
[* true] `= видна.&]
[s0;3 &]
[s5;K:Upp`:`:HeaderCtrl`:`:SetTabRatio`(int`,double`):%- [@(0.0.255) void]_[@0 SetTabRati
o]([@(0.0.255) int]_[@3 i], [@(0.0.255) double]_[@3 ratio])&]
[s2; Устанавливает логическую ширину 
(пропорцию) указанной вкладки.&]
[s4; [*C@3 i]-|индекс вкладки&]
[s4; [*C@3 ratio]-|логическая ширина вкладки.&]
[s0;3 &]
[s5;K:Upp`:`:HeaderCtrl`:`:GetTabRatio`(int`)const:%- [@(0.0.255) double]_[@0 GetTabRatio
]([@(0.0.255) int]_[@3 i])_[@(0.0.255) const]&]
[s2; Выводит текущую логическую ширину 
определённой вкладки. Логическую 
ширину вкладки можно установить методом 
[* SetTabRatio] (или использую метод [* SetRatio] 
для индивидуального объекта [* HeaderCtrl`::Column]), 
либо перетягиванием разрывов вкладки 
мышью.&]
[s4; [*C@3 i]-|индекс вкладки (с основанием 
ноль)&]
[s4; [*/ Возвратное значение]-|текущая логическая 
ширина вкладки&]
[s0;3 &]
[s5;K:Upp`:`:HeaderCtrl`:`:SetTabWidth`(int`,int`):%- [@(0.0.255) void]_[@0 SetTabWidth](
[@(0.0.255) int]_[@3 i], [@(0.0.255) int]_[@3 cx])&]
[s2; Устанавливает указанную вкладку`-заголовоч
ник в физический (пиксельный) размер. 
Соответственно это изменяет и логическую 
(относительную) ширину вкладки.&]
[s4; [*C@3 i]-|индекс вкладки (с основанием 
ноль)&]
[s4; [*C@3 cx]-|пиксельный размер вкладки&]
[s0;3 &]
[s5;K:Upp`:`:HeaderCtrl`:`:GetTabWidth`(int`):%- [@(0.0.255) int]_[@0 GetTabWidth]([@(0.0.255) i
nt]_[@3 i])&]
[s2; Возвращает физическую (пиксельную) 
ширину указанной вкладки`-заголовочника. 
Помните, что (если только HeaderCtrl не 
в режиме [* Scroll]) физическая ширина вкладки 
может меняться вместе с размером 
видимости родителя. Алгоритмы для 
долгосрочного манипулирования размерами 
вкладок (типично, путём сериализации) 
всегда должны опираться на логические 
ширины вкладок.&]
[s4; [*C@3 i]-|индекс вкладки (с основанием 
ноль)&]
[s4; [*/ Возвратное значение]-|физическая 
(пиксельная) ширина вкладки. Заметьте, 
что эта функция не [/ const], так как контрол`-заго
ловочник использует `"ленивый`" алгоритм 
при своей выкладке, и при вызове [* GetTabWidth] 
может понадобиться перевыкладка 
вкладок.&]
[s0;3 &]
[s5;K:Upp`:`:HeaderCtrl`:`:SwapTabs`(int`,int`):%- [@(0.0.255) void]_[@0 SwapTabs]([@(0.0.255) i
nt]_[@3 first], [@(0.0.255) int]_[@3 second])&]
[s2; Меняет местами вкладки [%-*@3 first] и [%-*@3 second].&]
[s0; &]
[s5;K:Upp`:`:HeaderCtrl`:`:MoveTab`(int`,int`):%- [@(0.0.255) void]_[@0 MoveTab]([@(0.0.255) i
nt]_[@3 from], [@(0.0.255) int]_[@3 to])&]
[s2; Перемещает вкладку в другую позицию.&]
[s0; &]
[s5;K:Upp`:`:HeaderCtrl`:`:GetTabIndex`(int`):%- [@(0.0.255) int]_[@0 GetTabIndex]([@(0.0.255) i
nt]_[@3 i])&]
[s2; Возвращает `"оригинальный`" индекс 
вкладки.&]
[s0; &]
[s5;K:Upp`:`:HeaderCtrl`:`:FindIndexTab`(int`):%- [@(0.0.255) int]_[@0 FindIndexTab]([@(0.0.255) i
nt]_[@3 ndx])&]
[s2; Находит текущую позицию вкладки 
с оригинальным индексом [%-*@3 ndx].&]
[s0; &]
[s5;K:Upp`:`:HeaderCtrl`:`:StartSplitDrag`(int`):%- [@(0.0.255) void]_[@0 StartSplitDrag](
[@(0.0.255) int]_[@3 s])&]
[s2; Начинает тяг `& брос вкладки`-заголовочника 
для указанного разрыва вкладки.&]
[s4; [*C@3 s]-|индекс разрыва (ноль `= левая сторона 
вкладки 0).&]
[s0;3 &]
[s5;K:Upp`:`:HeaderCtrl`:`:GetSplit`(int`):%- [@(0.0.255) int]_[@0 GetSplit]([@(0.0.255) in
t]_[@3 x])&]
[s2; Определяет положение разрыва вкладки`-загол
овочника, соответствующее заданной 
координате [/ x] (относительно контрола`-заголо
вочника). Может использоваться родительским
и контролами для проверки, находится 
ли курсор мывши поверх разрыва вкладки 
(напр.,. [* ArrayCtrl] использует эту функцию 
для проверки того, показывать ли ему 
горизонтальный курсор перемера).&]
[s4; [*C@3 x]-|горизонтальная пиксельная координата 
(относительно органичивающего бокса 
контрола`-заголовочника).&]
[s4; [*/ Возвратное значение]-|индекс разрыва 
вкладки (в диапазоне `[0..GetCount()`]) или 
`-1, когда поблизости от заданной точки 
нет разрыва вкладки.&]
[s0;3 &]
[s5;K:Upp`:`:HeaderCtrl`:`:GetScroll`(`)const:%- [@(0.0.255) int]_[@0 GetScroll]()_[@(0.0.255) c
onst]&]
[s2; Только в режиме [* Absolute]: возвращает 
пиксельное смещение, определяющее 
текущее начало (потенциально промотанного) 
заголовочника. Для безпромоточных 
визуальных режимов, эта функция всегда 
возвращает 0 (в этих режимах левая 
сторона первой вкладки всегда совпадает 
с левой стороной родительской области 
видимости).&]
[s4; [*/ Возвратное значение]-|Число пикселей, 
на которые в данный момент промотан 
контрол`-заголовочник в горизонтальном 
направлении. Физическая координата 
[/ x] края вкладки (относительно родительской 
видимости) может быть вычислена вычитанием 
этого значения от его логической 
координаты (относительно крайней 
левой кромки заголовочника).&]
[s0;3 &]
[s5;K:Upp`:`:HeaderCtrl`:`:IsScroll`(`)const:%- [@(0.0.255) bool]_[@0 IsScroll]()_[@(0.0.255) c
onst]&]
[s2; Проверяет, находится ли в данное 
время контрол`-заголовочник в режиме 
промотки (т.е., находится ли он в визуальном 
режиме [* Absolute] и его общая ширина вкладок 
превышает текущий размер видимой 
области). Когда [* IsScroll] возвращает [* true], 
метод [* GetScroll] можно использовать для 
получения текущей позиции горизонтальной 
полосы промотки.&]
[s4; [*/ Возвратное значение]-|[* true] `= проматываемый 
заголовочник, [* false ]`= непроматываемый 
заголовочник.&]
[s0;3 &]
[s5;K:Upp`:`:HeaderCtrl`:`:GetHeight`(`)const:%- [@(0.0.255) int]_[@0 GetHeight]()_[@(0.0.255) c
onst]&]
[s2; Определяет пиксельную высоту контрола`-заго
ловочника. Она равна максимуму всех 
индивидуальных высот вкладок (зависит 
от шрифтов титула вкладки и размеров 
иконок).&]
[s4; [*/ Возвратное значение]-|пиксельная 
высота HeaderCtrl.&]
[s0;3 &]
[s5;K:Upp`:`:HeaderCtrl`:`:Invisible`(bool`):%- HeaderCtrl`&_[@0 Invisible]([@(0.0.255) b
ool]_[@3 inv])&]
[s2; When called with a [* true] argument, makes the header zero height 
in frame mode (the tab placement routines still work but the 
header itself remains hidden).&]
[s4; [*C@3 inv]-|[* true] `= hide the header, [* false] `= show it.&]
[s4; [*/ Возвратное значение]-|[* `*this]&]
[s0;3 &]
[s5;K:Upp`:`:HeaderCtrl`:`:Track`(bool`):%- HeaderCtrl`&_[@0 Track]([@(0.0.255) bool]_[@3 `_
track]_`=_true)&]
[s2; Устанавливает whether the HeaderCtrl and its parent 
view contents should be `'animated`' during repositioning the 
tab breaks using mouse drag `& drop. When [* `_track] is set to 
[* true], the header and its parents repaint is requested every 
time the mouse moves during the drag `& drop. When set to [* false], 
the header and the parent control get repainted only after the 
drag `& drop is finished. It is wise to set this value depending 
on the complexity of the data shown `'beneath`' the header (usually 
in an ArrayCtrl) and on the expected target hardware because 
the track mode is much more demanding with respect to computational 
time spent during the necessary multiple repaints.&]
[s4; [*C@3 `_track]-|[* true] `= regenerate header and its parent dynamically 
during drag `& drop, [* false] `= only after it`'s finished.&]
[s4; [*/ Возвратное значение]-|[* `*this]&]
[s0;3 &]
[s5;K:Upp`:`:HeaderCtrl`:`:NoTrack`(`):%- HeaderCtrl`&_[@0 NoTrack]()&]
[s2; Turns off track mode. This is identical to [* Track(false)].&]
[s4; [*/ Возвратное значение]-|[* `*this]&]
[s0;3 &]
[s5;K:Upp`:`:HeaderCtrl`:`:Proportional`(`):%- HeaderCtrl`&_[@0 Proportional]()&]
[s2; Switches the HeaderCtrl to the [/ proportional] mode. In proportional 
mode, the individual logical tab widths are always recalculated 
to physical (pixel) widths by scaling them using a common multiplication 
factor in order to fit the whole header into the view width.&]
[s4; [*/ Возвратное значение]-|[* `*this]&]
[s0;3 &]
[s5;K:Upp`:`:HeaderCtrl`:`:ReduceNext`(`):%- HeaderCtrl`&_[@0 ReduceNext]()&]
[s2; Switches the HeaderCtrl to the [/ reduce next] mode. Every time 
a tab width is set in this mode and the total tab width exceeds 
the view size, the following tab widths (tabs to the right) are 
reduced in left`-to`-right order (beginning with the tab just 
after the tab being adjusted) to fit the view size.&]
[s4; [*/ Возвратное значение]-|[* `*this]&]
[s0;3 &]
[s5;K:Upp`:`:HeaderCtrl`:`:ReduceLast`(`):%- HeaderCtrl`&_[@0 ReduceLast]()&]
[s2; Switches the HeaderCtrl to the [/ reduce last] mode. Every time 
a tab width is set in this mode and the total tab width exceeds 
the view size, the following tab widths (tabs to the right) are 
reduced in right`-to`-left order (beginning with the rightmost 
tab) to fit the view size.&]
[s4; [*/ Возвратное значение]-|[* `*this]&]
[s0;3 &]
[s5;K:Upp`:`:HeaderCtrl`:`:Absolute`(`):%- HeaderCtrl`&_[@0 Absolute]()&]
[s2; Switches the HeaderCtrl to the [/ absolute] (scroll) mode. In 
this mode the tab widths are never modified automatically, the 
logical header width can be smaller or greater than the actual 
view size. Whenever the total tab width exceeds the view size, 
the header can be scrolled to the left or right to pan its visible 
portion within the parent view.&]
[s4; [*/ Возвратное значение]-|[* `*this]&]
[s0;3 &]
[s5;K:Upp`:`:HeaderCtrl`:`:Moving`(bool`):%- HeaderCtrl`&_[@0 Moving]([@(0.0.255) bool]_[@3 b
]_`=_true)&]
[s2; Activates mode when tabs can be rearranged by dragging them. 
Default is inactive.&]
[s0;3 &]
[s0;3 &]
[s5;K:Upp`:`:HeaderCtrl`:`:GetStdHeight`(`):%- static static_[@(0.0.255) int]_[@0 GetStdH
eight]()&]
[s2; Возвращает the standard header control height (when 
there are no icons and the standard font is used in all tabs).&]
[s4; [*/ Возвратное значение]-|&]
[s0;3 &]
[s5;K:Upp`:`:HeaderCtrl`:`:WhenLayout:%- Callback_[@0 WhenLayout]&]
[s2; This callback is executed whenever the header control gets laid 
out (whenever the tab positions and/or widths change).&]
[s0;3 &]
[s5;K:Upp`:`:HeaderCtrl`:`:WhenScroll:%- Callback_[@0 WhenScroll]&]
[s2; This callback is executed whenever the header control gets scrolled 
([/ absolute] mode only).&]
[s0;3 &]
[s5;K:Upp`:`:HeaderCtrl`:`:WhenScrollVisibility:%- Callback_[@0 WhenScrollVisibility]&]
[s2; This callback is executes whenever its scrollbar appears or 
disappears (whenever the return value of the [* IsScroll] method 
changes).&]
[s0; &]
[s0; &]
[s0;:Upp`:`:HeaderCtrl`:`:Column`:`:class: [*+117 HeaderCtrl`::Column 
(nested class)]&]
[s2; &]
[s0;%- [%RU-RU/ Производный от][%RU-RU  ]LabelBase&]
[s0; &]
[s0; Структура [* HeaderCtrl`::Column] представляет 
индивидуальную вкладку заголовочника. 
Методы HeaderCtrl для вставки вкладок 
и редактирования, как правило, возвращают 
ссылку на этот класс [/ Column], который 
может использоваться впоследствии 
для программирования различных свойств 
вкладки.&]
[s0; &]
[s0; The set methods return a reference to [* `*this] ; this allows 
multiple tab properties to be set using a single C`+`+ statement 
with repeated use of the dot operator, e.g.:&]
[s0; &]
[s0; [C -|headerctrl.Add().Min(10).Max(20).Margin(3);]&]
[s0;3 &]
[s5;K:Upp`:`:HeaderCtrl`:`:Column`:`:Min`(int`):%- Column`&_[@0 Min]([@(0.0.255) int]_[@3 `_
min])&]
[s2; Устанавливает minimum tab pixel size. The default 
value is 0 (the tab can be shrunk arbitrarily and can disappear 
altogether at a certain moment).&]
[s4; [%-*C@3 `_min]-|Minimum tab width in pixels.&]
[s4; [*/ Возвратное значение]-|[* `*this]&]
[s0;3 &]
[s5;K:Upp`:`:HeaderCtrl`:`:Column`:`:Max`(int`):%- Column`&_[@0 Max]([@(0.0.255) int]_[@3 `_
max])&]
[s2; Устанавливает maximum tab pixel size. The default 
value is [/ INT`_MAX] (unlimited).&]
[s4; [%-*C@3 `_max]-|Maximum tab width in pixels.&]
[s4; [*/ Возвратное значение]-|[* `*this]&]
[s0;3 &]
[s5;K:Upp`:`:HeaderCtrl`:`:Column`:`:MinMax`(int`,int`):%- Column`&_[@0 MinMax]([@(0.0.255) i
nt]_[@3 m], [@(0.0.255) int]_[@3 n])&]
[s2; Устанавливает both minimum and maximum tab pixel 
size at the same time. This has the same effect as [* .Min(m).Max(n)].&]
[s4; [%-*C@3 m]-|Minimum tab width in pixels.&]
[s4; [%-*C@3 n]-|Maximum tab width in pixels.&]
[s4; [*/ Возвратное значение]-|[* `*this]&]
[s0;3 &]
[s5;K:Upp`:`:HeaderCtrl`:`:Column`:`:Fixed`(int`):%- Column`&_[@0 Fixed]([@(0.0.255) int]_
[@3 f])&]
[s2; Устанавливает both minimum and maximum tab pixel 
size to the same value, effectively making the tab width constant. 
Равно [* .MinMax(f, f)].&]
[s4; [%-*C@3 f]-|Fixed tab width in pixels.&]
[s4; [*/ Возвратное значение]-|[* `*this]&]
[s0;3 &]
[s5;K:Upp`:`:HeaderCtrl`:`:Column`:`:SetRatio`(double`):%- Column`&_[@0 SetRatio]([@(0.0.255) d
ouble]_[@3 ratio])&]
[s2; Устанавливает logical (relative) tab width. The 
logical tab width (together with logical widths of other tabs 
and the minimum / maximum width constraints) is used to calculate 
the final pixel size of each tab according to the current header 
visual mode.&]
[s4; [%-*C@3 ratio]-|relative tab width (a generic floating`-point value)&]
[s4; [*/ Возвратное значение]-|[* `*this]&]
[s0;3 &]
[s5;K:Upp`:`:HeaderCtrl`:`:Column`:`:SetMargin`(int`):%- Column`&_[@0 SetMargin]([@(0.0.255) i
nt]_[@3 m])&]
[s2; Устанавливает horizontal tab margin size, the number 
of pixels to deflate the tab rectangle horizontally to obtain 
the final `"column data`" rectangle. In [/ ArrayCtrl], the tab 
margin areas are used to paint the grid lines and the inverted 
selection rectangle outside of the table cells.&]
[s4; [%-*C@3 m]-|Horizontal tab margin size in pixels.&]
[s4; [*/ Возвратное значение]-|[* `*this]&]
[s0;3 &]
[s5;K:Upp`:`:HeaderCtrl`:`:Column`:`:GetMargin`(`)const:%- [@(0.0.255) int]_[@0 GetMargin
]()_[@(0.0.255) const]&]
[s2; Возвращает current tab margin width in pixels.&]
[s4; [*/ Возвратное значение]-|[* `*this]&]
[s0;3 &]
[s5;K:Upp`:`:HeaderCtrl`:`:Column`:`:WhenAction:%- Callback_[@0 WhenAction]&]
[s2; The tab action callback. When set to a non`-null value, the 
header control starts to check for mouse clicks within this tab 
and calls this callback whenever the corresponding tab is clicked.&]
[s0; &]
[s5;K:`:`:HeaderCtrl`:`:Column`:`:WhenBar:%- Callback1<Bar`&>_[@0 WhenBar]&]
[s2; Provides am optional context menu for header tab.  &]
[s0; ]]