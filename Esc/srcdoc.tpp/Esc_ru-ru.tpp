topic "Язык сценариев Esc";
[b133;a83;*+117 $$1,0#31310162474203024125188417583966:subtitle]
[H4;b83;*+150 $$2,0#07864147445237544204411237157677:title]
[l321;C@5;1 $$3,3#20902679421464641399138805415013:code]
[b42;a42;ph2 $$4,4#45413000475342174754091244180557:text]
[a83;*6 $$5,5#B3A4428F1E67450ED340E28F681B00E8:caption]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_}%RU-RU 
[s5; Язык сценариев Esc&]
[s2; Содержание&]
[s0; &]
[s0; [^topic`:`/`/Esc`/srcdoc`/Esc`_ru`-ru`#1^ 1. Введение]&]
[s0; [^topic`:`/`/Esc`/srcdoc`/Esc`_ru`-ru`#2^ 2. Типы значений]&]
[s0;     [^topic`:`/`/Esc`/srcdoc`/Esc`_ru`-ru`#2`_1^ 2.1 Voids (пустые 
значения)]&]
[s0;     [^topic`:`/`/Esc`/srcdoc`/Esc`_ru`-ru`#2`_2^ 2.2 Числа]&]
[s0;     [^topic`:`/`/Esc`/srcdoc`/Esc`_ru`-ru`#2`_3^ 2.3 Массивы]&]
[s0;     [^topic`:`/`/Esc`/srcdoc`/Esc`_ru`-ru`#2`_4^ 2.4 Мапы]&]
[s0;     [^topic`:`/`/Esc`/srcdoc`/Esc`_ru`-ru`#2`_5^ 2.5 Лямбды]&]
[s0; [^topic`:`/`/Esc`/srcdoc`/Esc`_ru`-ru`#3^ 3. Переменные 
и контексты]&]
[s0; [^topic`:`/`/Esc`/srcdoc`/Esc`_ru`-ru`#4^ 4. Выражения]&]
[s0; [^topic`:`/`/Esc`/srcdoc`/Esc`_ru`-ru`#5^ 5. Инструкции]&]
[s0; [^topic`:`/`/Esc`/srcdoc`/Esc`_ru`-ru`#6^ 6. Стандартная 
библиотека]&]
[s0; &]
[s2;:1: 1. Введение&]
[s4; Esc является простым языком сценариев, 
предназначенным для внедрения в приложения 
Ultimate`+`+.&]
[s4; Он безтипный, ориентированный на 
значения, язык с простым кодом, имеющий 
интерфейс к C`+`+/U`+`+. Очень простой, 
но достаточно полноценный, язык.&]
[s2;:2: 2. Типы значений&]
[s1;:2`_1: 2.1 Voids(пустые значения)&]
[s4; Все переменные имеют значение void 
до присвоения им любого другого значения. 
У типа Void одно значение&]
[s0; &]
[s3; void&]
[s0; &]
[s4; и можно проверить выражение на void`-ность 
с помощью функции [@(128.0.255) is`_void].&]
[s1;:2`_2: 2.2 Числа&]
[s4; Число `- это число с плавающей запятой 
(соответствующее типу в Си double). Примеры 
числовых литералов:&]
[s0; &]
[s3; 1&]
[s3; 1.123&]
[s3; 1.123e`-96&]
[s3; 0x123       [@4 // hexadecimal (16`-ричное)]&]
[s3; 0123        [@4 // octal (8`-ричное)]&]
[s3; 0b10100100  [@4 // binary (двоичное)]&]
[s3; `'x`'         [@4 // character code (символьный код)]&]
[s0; &]
[s4; Esc поддерживает общие операции, наподобии 
как в Си, для числовых значений.&]
[s1;:2`_3: 2.3 Массивы&]
[s4; Массивами являются упорядоченные 
последовательности (с возможностью) 
случайного доступа, содержащие прочие 
значения Esc (включая `"гнездовые`" (`'nested`') 
массивы). Значения массива определяются 
с помощью [@(128.0.255) `[`]] (квадратных) скобок:&]
[s0; &]
[s3; `[`]                 [@4 // пустой массив]&]
[s3; `[ 1, 2, 3, 4, 5 `]&]
[s3; `[ 1, `[2, 3`], 4 `]   [@4 // в массиве имеется 
гнездовой массив]&]
[s3; `"Hello`"            [@4 // эквивалентно `[`'H`', 
`'e`', `'l`', `'l`', `'o`'`]]&]
[s3; `[ 1, 2, 3 `* alfa `] [@4 // также могут использоваться 
выражения]&]
[s0; &]
[s4; Число элементов, расположенных в 
массиве, можно получить стандартной 
функцией [*C count].&]
[s4; Доступ к элементам массива предоставляется
 оператором subscript [@(128.0.255) `[ `]]:&]
[s0; &]
[s3; a`[0`]&]
[s3; a`[1`] `= 3&]
[s0; &]
[s4; Первый элемент массива соответствует 
индексу [@(128.0.255) `[0`]].&]
[s4; Допускаются отрицательные индексы 
и определения элементов с конца массива: 
[@(128.0.255) `[`-1`]] последний элемент массива, 
[@(128.0.255) `[`-2`]] предпоследний и т.д.&]
[s0; &]
[s3; a`[`-1`]&]
[s0; &]
[s4; эквивалентно&]
[s0; &]
[s3; a`[count(a) `- 1`]&]
[s0; &]
[s4; Когда индекс используется как r`-value 
(значение, стоящее справа от [@(128.0.255) `=]), 
у него проверяется диапазон. Если 
как l`-value (значение, слева от `=), и какой`-либо 
положительный индекс определяет 
элемент после массива, то массив расширяется
 на значения void, создавая этот элемент.&]
[s4; `"Пустой`" индекс [@(128.0.255) `[`]] обозначает 
элемент, находящийся после последнего, 
и может использоваться для добавления 
в массив элементов:&]
[s0; &]
[s3; x`[`] `= item&]
[s0; &]
[s4; равнозначно&]
[s0; &]
[s3; x`[count(x)`] `= item&]
[s0; &]
[s4; Диапазон элементов массива можно 
получать или изменять, используя 
срезы:&]
[s0; &]
[s3; array`[pos, count`]&]
[s0; &]
[s4; определяет срез массива с [/ count] элементов, 
начиная с элемента [/ pos].&]
[s0; &]
[s3; x`[start:end`]&]
[s0; &]
[s4; обозначает срез массива из [/ end `- start] 
элементов, начиная со [/ start] .&]
[s4; Части срезов могут пропускаться&]
[s0; &]
[s3; array`[pos,`]&]
[s4; или&]
[s3; array`[pos:`]&]
[s0; &]
[s4; эквивалентно&]
[s0; &]
[s3; array`[pos : count(array)`]&]
[s0; &]
[s4; а&]
[s0; &]
[s3; array`[,count`]&]
[s4; или&]
[s3; array`[:count`]&]
[s0; &]
[s4; эквивалентно&]
[s0; &]
[s3; array`[0, count`]&]
[s0; &]
[s4; Когда срезы используются как l`-value, 
заменяется соответствующая часть 
массива:&]
[s0; &]
[s3; s `= `"1234`";&]
[s3; s`[1: `-1`] `= `"xXx`"; [@4 // s is now `"1xXx4`"]&]
[s0; &]
[s4; К массивам могут применяться несколько 
операторов&]
[s0; &]
[s3; array1 `+ array2&]
[s0; &]
[s4; объединяет два массива (`"конкатенирует`"),&]
[s0; &]
[s3; array `* number&]
[s3; number `* array&]
[s0; &]
[s4; повторяет массив number раз.&]
[s0; &]
[s3; array `+`= array1&]
[s0; &]
[s4; эквивалентно&]
[s0; &]
[s3; array `= array `+ array1&]
[s0; &]
[s4; также&]
[s0; &]
[s3; array << array1 << array2 << array3&]
[s0; &]
[s4; эквивалентно&]
[s0; &]
[s3; array `= array `+ array1 `+ array2 `+ array3&]
[s0; &]
[s4; В Esc допустимо наличие [/ void] значения, 
расположенного справа от выражения 
конкатенации массива, `- в таких случаях 
оператор не имеет никакого действия.&]
[s1;:2`_4: 2.4 Мапы (`=`"соответствия`", `"карты`")&]
[s4; В мапах в качестве записей содержатся 
пары из ключей и значений. Как правило 
для доступа к записям мапа используются 
обычные субскрипты [@(128.0.255) `[`]]:&]
[s0; &]
[s3; map`[key`] `= value&]
[s0; &]
[s4; Все записи мапа, которым не присвоено 
не`-void значение, имеют значение void. 
Подобное присваивание void ключу мапа 
можно считать удалением из него записи&]
[s0; &]
[s3; map`[key`] `= void&]
[s0; &]
[s4; а проверку записи на void`-ность можно 
считать проверкой на наличия ключа 
в мапе:&]
[s0;C &]
[s3; if(is`_void(map`[key`])) `{ ... `}&]
[s0; &]
[s4; В качестве особого исключения, когда 
значение void находится перед l`-value 
[@(128.0.255) `[`]], оно меняется на пустой 
мап:&]
[s0; &]
[s3; x `= void;       [@4 // x является void]&]
[s3; x`[key`] `= value; [@4 // x теперь мап]&]
[s0; &]
[s4; Мапы также используются для создания 
составных (неоднородных) структур 
данных. Для облегчения этой задачи 
используется специальный синтаксис 
[/ .field]:&]
[s0; &]
[s3; x.field `= value;&]
[s0; &]
[s4; эквивалентно&]
[s0; &]
[s3; x`[`"field`"`] `= value;&]
[s0; &]
[s0; Значения мапов также можно определять 
с помощью `{`} фигурных скобок:&]
[s0; &]
[s3; `{`}                                [@4 // пустой мап]&]
[s3; `{ `"alfa`":10, `"beta`":20, 20:`"40`" `}&]
[s3; `{ 2 `* a : `"100`" `}                 [@4 // значениями 
мапа могут быть выражения]&]
[s0; &]
[s1;:2`_5: 2.5 Лямбды&]
[s4; Лямбды представляют собой исполнимый 
код на Esc. Лямбда`-значения вводят 
с помощью символа [@(128.0.255) `@]:&]
[s0; &]
[s3; `@(x) `{ return 2 `* x; `}&]
[s0; &]
[s4; Присвоение лямбда`-значения переменной 
равносильно определению функции:&]
[s0; &]
[s3; :foo `= `@(x) `{ return 2 `* x; `};-|[@4 // определяет 
глобальную функцию foo]&]
[s0; &]
[s4; Если перед аругментом лямбды указан 
символ [@(128.0.255) `&], он обозначает аргумент 
ввода`-вывода (другие аргументы только 
вводные).&]
[s4; [@(128.0.255) ...] в конце списка аргументов 
разрешается предоставлять переменное 
число аргументов. В таком случае, 
дополнительные параметры передаются 
в переменной [/ argv] из типового массива.&]
[s0; &]
[s3; :sum `= `@(...) `{ s `= 0; for(i in argv) s `+`= argv`[i`]; `}&]
[s0; &]
[s4; У параметров могут иметься дефолтные 
значения, разделённые [@(128.0.255) `=]:&]
[s0; &]
[s3; :bar `= `@(x `= 0) `{ ... `}&]
[s0; &]
[s4; Для простоты доступна альтернативная 
форма определения функции:&]
[s0; &]
[s3; #:foo(x) `{ return 2 `* x; `}&]
[s0; &]
[s2;i150;O9;:3: 3. Переменные и контексты&]
[s4; Имена переменных в Esc следуют принципам 
языка Си (регистрочувствительны, 
начинаются с алфавитного символа 
или [@(128.0.255) `_] и могут содержать алфавитные 
символы, [@(128.0.255) `_] и числа).&]
[s4; Esc различает три типа контекстов 
переменных, использующих символы 
[*@(128.0.255) .] и [*@(128.0.255) :] :&]
[s0; &]
[s3; var-|-|[@4 // локальная переменная]&]
[s3; .var-|-|[@4 // переменная экземпляра]&]
[s3; :var-|-|[@4 // глобальная переменная]&]
[s0; &]
[s4; Локальные переменные специфичны 
для функции, глобальные переменные 
разделяются всеми функциями. Переменные 
экземпляра &]
[s4; представляют значения мапа (ключи 
определяют имена переменных), лямбда`-значен
ие которого вызывается, или может 
формироваться, с помощью оператора 
двоичного связывания ( binary) [@(128.0.255) !] 
(bind operator):&]
[s0; &]
[s3; var.x `= 0;&]
[s3; var.Next `= `@() `{ .x`+`+; `};&]
[s3; var.Next();-|-|-|[@4 // var.x теперь равно 1 ]&]
[s3; var1.x `= 0;&]
[s3; var.Next()! var1;-|-|[@4 // var1.x теперь равно 1]&]
[s0; &]
[s4; Для облегчения жизни программиста 
относительно контекста применено 
одно исключение `- при вызове лямбды 
из локальной переменной и её отсутствии, 
также проверяется её наличие в контексте 
экземпляра и глобальном (именно в 
таком порядке):&]
[s0; &]
[s3; beta(x)&]
[s0; &]
[s4; Вначале пытается получить лямбду 
из локальной переменной [/ beta], затем 
(если ничего не обнаружено) из переменной 
экземпляра [/ beta,]и, наконец (если лямбды 
всё ещё нет) `- из глобальной [/ beta]. Конечно 
же, [*@(128.0.255) .] и [*@(128.0.255) :] всё ещё могут 
использоваться для разметки контекста:&]
[s0; &]
[s3; :beta(x) [@4 // вызывает глобальную beta, даже 
если имеется локальная или экземпляра]&]
[s0; &]
[s2;:4: 4. Выражения&]
[s4; Когда выражения используются как 
логические значения,`-void, число ноль 
и мап или массив с нулём элементов 
`- представляют [/ false], а прочие значения 
равны [/ true.]&]
[s4; В следующей таблице толстые линии 
разделяют операторов с одинаковым 
приоритетом, где самые верхние элементы 
имеют наивысший приоритет:&]
[s0; &]
[ {{4397:5603h1;@(216) [s0; Оператор]
:: [s0; Комментарии]
::^@2 [s0; [/C map][*C `[][/C key][*C `]]]
:: [s0; Map value at [/ key].]
:: [s0; [/C map][*C .][/C field]]
:: [s0; То же, что и [/ map]`[`"[/ field]`"`].]
:: [s0; [/C array][*C `[][/C index][*C `]]]
:: [s0; Элемент массива в позиции [/ index].]
:: [s0; [/C array][*C `[][/C start][*C , ][/C count][*C `]]]
:: [s0; Срез массива, начиная со [/ start], из [/ count] 
элементов.]
:: [s0; [/C array][*C `[][/C start][*C :][/C end][*C `]]]
:: [s0; Срез массива из элементов [/ start .. end] 
`- 1]
:: [s0; [/C array][*C `[,][/C count][*C `]]&]
[s0; [/C array][*C `[:][/C count][*C `]]]
:: [s0; То же, что и [/ array]`[0, [/ count]`].&]
[s0; ]
:: [s0; [/C array][*C `[][/C start][*C :`]]&]
[s0; [/C array][*C `[][/C start][*C ,`]]]
:: [s0; То же, что и [/ array]`[[/ start] : count(array)`]. ]
:: [s0; [/C lambda][*C (][/C args][*C )]]
:: [s0; Вызывает [/ lambda] с набором аргументов 
[/ args]. If lambda is subscript of the map, map becomes instance 
for [/ lambda] execution (accessible via [* .][/ name] or [* self]).]
:: [s0; [/C lambda][*C (][/C args][*C ) ! ][/C instance]]
:: [s0; Invokes [/ lambda], using[/  instance] as instance for lambda execution 
(accessible via [* .][/ name] or [* self]).]
:: [s0; [*C self]]
:: [s0; Instance.]
::t33/15 [s0; [*C `+`+][/C l`-number]&]
[s0; [*C `-`-][/C l`-number]]
:: [s0; Increments/decrements number l`-value.]
::t0/15 [s0; [*C `-][/C number]]
:: [s0; Unary minus.]
:: [s0; [*C `~][/C number]]
:: [s0; Bit`-wise complement.]
:: [s0; [*C !][/C value]]
:: [s0; Logical not. 1 when value represents false, 0 otherwise.]
:: [s0; [/C l`-number][*C `-`-]&]
[s0; [/C l`-number][*C `+`+]]
:: [s0; Post`-increment / post`-decrement of number l`-value.]
::t33/15 [s0; [/C array][*C  `* ][/C number]&]
[s0; [/C number][*C  `* ][/C array]]
:: [s0; Возвращает [/ array][*/  ]repeated [/ number] times.]
::t0/15 [s0; [/C number][*C  `* ][/C number]]
:: [s0; Multiplication.]
:: [s0; [/C number][*C  / ][/C number]]
:: [s0; Division.]
:: [s0; [/C number][*C  % ][/C number]]
:: [s0; Modulo.]
::t33/15 [s0; [/C array][*C  `+ ][/C array]]
:: [s0; Concatenates [/ array]s.]
::t0/15 [s0; [/C number][*C  `+ ][/C number]]
:: [s0; Addition.]
:: [s0; [/C number][*C  `- ][/C number]]
:: [s0; Subtraction.]
::t33/15 [s0; [/C l`-array][*C  << ][/C array]]
:: [s0; Concatenates [/ array ]to [/ l`-array], yielding [/ l`-array ]as l`-value 
result.]
::t0/15 [s0; [/C number][*C  << ][/C number]]
:: [s0; Shift left.]
:: [s0; [/C number][*C  >> ][/C number]]
:: [s0; Shift right.]
::t33/15 [s0; [/C number][*C  < ][/C number]&]
[s0; [/C number][*C  > ][/C number]&]
[s0; [/C number][*C  <`= ][/C number]&]
[s0; [/C number][*C  >`= ][/C number]]
:: [s0; Comparison of [/ number]s.]
::t0/15 [s0; [/C array][*C  < ][/C array]&]
[s0; [/C array][*C  > ][/C array]&]
[s0; [/C array][*C  <`= ][/C array]&]
[s0; [/C array][*C  >`= ][/C array]]
:: [s0; Comparison of [/ array]s.]
::t33/15 [s0; [/C value][*C  `=`= ][/C value]]
:: [s0; Equality.]
::t0/15 [s0; [/C value][*C  !`= ][/C value]]
:: [s0; Inequality.]
::t33/15 [s0; [/C number][*C  `& ][/C number]]
:: [s0; Binary and.]
:: [s0; [/C number][*C  `^ ][/C number]]
:: [s0; Binary xor.]
:: [s0; [/C number][*C  `| ][/C number]]
:: [s0; Binary or.]
:: [s0; [/C value][*C  `&`& ][/C value]]
:: [s0; Logical and. If first value is false, second value is not evaluated, 
just skipped.]
:: [s0; [/C value][*C  `|`| ][/C value]]
:: [s0; Logical or. If first value is true, second value is not evaluated, 
just skipped.]
:: [s0; [/C value ][*C ?][/C  value ][*C :][/C  value]]
:: [s0; Conditional expression. Only necessary expressions are evaluated.]
:: [s0; [/C l`-value][*C  `= ][/C value]]
:: [s0; Assignment.]
::t0/15 [s0; [/C l`-array][*C  `+`= ][/C array]]
:: [s0; То же, что и [C l`-array `= l`-array `+ array].]
:: [s0; [/C l`-number][*C  `+`= ][/C number]]
:: [s0; То же, что и [C l`-number `= l`-number `+ number].]
:: [s0; [/C l`-number][*C  `-`= ][/C number]]
:: [s0; То же, что и [C l`-number `= l`-number `- number].]
:: [s0; [/C l`-number][*C  `*`= ][/C number]]
:: [s0; То же, что и [C l`-number `= l`-number `* number].]
:: [s0; [/C l`-number][*C  /`= ][/C number]]
:: [s0; То же, что и [C l`-number `= l`-number / number].]
:: [s0; [/C l`-number][*C  %`= ][/C number]]
:: [s0; То же, что и [C l`-number `= l`-number % number].]}}&]
[s0; &]
[s0; &]
[s2;:5: 5. Инструкции&]
[s0; &]
[ {{4508:5492h1;@(216) [s0; Инструкция]
:: [s0; Комментарии]
::^@2 [s0; [*C ;]]
:: [s0; Пустая инструкция.]
:: [s0; [/C expression][*C ;]]
:: [s0; Оцениваемое выражение.]
:: [s0; [*C `{ ][/C statement][*C ; ... `}]]
:: [s0; Составная инструкция.]
:: [s0; [*C if(][/C condition][*C ) ][/C statement]]
:: [s0; Условная инструкция.]
:: [s0; [*C if(][/C condition][*C ) ][/C statement]&]
[s0; [*C else ][/C statement]]
:: [s0; Условная инструкция with ]
:: [s0; [*C while(][/C condition][*C ) ][/C statement]]
:: [s0; Цикл. Инструкция выполняется, покуда 
[/ condition] равно true.]
:: [s0; [*C do ][/C statement]&]
[s0; [*C while(][/C condition][*C )]]
:: [s0; Loop. Statement is performed once and then is repeated as long 
as the [/ condition] is true.]
:: [s0; [*C for(][/C init][*C , ][/C condition][*C , ][/C increment][*C )]&]
[s0; [*C -|][/C statement]]
:: [s0; То же, что и&]
[s0; [/C init][C ;]&]
[s0; [C while(][/C condition][C ) `{]&]
[s0; [C -|][/C statement][C ;]&]
[s0; [C -|][/C increment][C ;]&]
[s0; [C `}]]
:: [s0; [*C for(][/C l`-value][*C  in ][/C array][*C )]&]
[s0; [*C -|][/C statement]]
:: [s0; То же, что и&]
[s0; [C for(][/C l`-value][C  `= 0; ][/C l`-value][C  < count(][/C array][C );]&]
[s0; [C     ][/C l`-value][C `+`+)]&]
[s0; [C -|][/C statement]&]
[s0; ]
:: [s0; [*C for(][/C l`-value][*C  in ][/C map][*C )]&]
[s0; [*C -|][/C statement]]
:: [s0; Iterates through [/ map], assigning keys of elements with non`-void 
value to the [/ l`-value].]
:: [s0; [*C continue]]
:: [s0; Skips the rest of loop statement.]
:: [s0; [*C break]]
:: [s0; Causes exit from innermost loop or switch statement.]
:: [s0; [*C switch(][/C value][*C ) ][/C statement]]
:: [s0; Matches [/ value] to all case statements inside the [/ statement], 
continues at the case with the same value, or with default if 
no such case exists and default is present. If neither matching 
case and default exists, does nothing.]
:: [s0; [*C case ][/C value][*C :]]
:: [s0; Switch case.]
:: [s0; [*C default:]]
:: [s0; Default switch case.]
:: [s0; [*C #][/C name][*C (][/C args][*C ) `{ ... `}]]
:: [s0; То же, что и [/C name][C  `= `@(][/C args][C ) `{ ... `};]]
:: [s0; [*C #.][/C name][*C (][/C args][*C ) `{ ... `}]]
:: [s0; То же, что и [C .][/C name][C  `= `@(][/C args][C ) `{ ... `};]]
:: [s0; [*C #:][/C name][*C (][/C args][*C ) `{ ... `}]]
:: [s0; То же, что и [C :][/C name][C  `= `@(][/C args][C ) `{ ... `};]]}}&]
[s0; &]
[s2;:6: 6. Стандартная библиотека&]
[s0;3 &]
[ {{2479:7521h1;@(216) [s0; Функция]
:: [s0; Комментарии]
::^@2 [s0; [*C is`_number(][/C x][*C )]]
:: [s0; Возвращает 1 if [/ x] is a number, 0 otherwise.]
:: [s0; [*C is`_array(][/C x][*C )]]
:: [s0; Возвращает 1 if [/ x] is a array, 0 otherwise.]
:: [s0; [*C is`_map(][/C x][*C )]]
:: [s0; Возвращает 1 if [/ x] is a map, 0 otherwise.]
:: [s0; [*C is`_void(][/C x][*C )]]
:: [s0; Возвращает 1 if [/ x] is void, 0 otherwise.]
:: [s0; [*C int(][/C number][*C )]]
:: [s0; Возвращает the [/ number] rounded towards zero.]
:: [s0; [*C to`_string(][/C x][*C )]]
:: [s0; Return [/ x] as array representing string representation.]
:: [s0; [*C to`_number(][/C x][*C )]]
:: [s0; Converts an array (string) to the number.]
:: [s0; [*C count(][/C x][*C )]]
:: [s0; If [/ x ]is void, returns 0.&]
[s0; If x is a number, returns 1.&]
[s0; If x is an array, returns number of elements of this array.&]
[s0; If x is an map, returns number of entries in this map with non`-void 
values.]
:: [s0; [*C keys(][/C map][*C )]]
:: [s0; Возвращает array of keys of map (ordering is the same 
as for [* values] function, otherwise is unspecified).]
:: [s0; [*C values(][/C map][*C )]]
:: [s0; Возвращает array of values of map (ordering is the 
same as for [* keys] function, otherwise is unspecified).]
:: [s0; [*C rand()]]
:: [s0; Возвращает random number in range 0..32767.]
:: [s0; [*C reverse(][/C array][*C )]]
:: [s0; Возвращает reversed [/ array].]
:: [s0; [*C sort(][/C array][*C )]]
:: [s0; Возвращает sorted [/ array].]
:: [s0; [*C order(][/C array][*C )]]
:: [s0; Возвращает sort order of elements in the [/ array].]
:: [s0; [*C IsDate(][/C map][*C )]]
:: [s0; Возвращает 1 if map contains `"day`", `"month`" and 
`"year`" keys, 0 otherwise.]
:: [s0; [*C IsTime(][/C map][*C )]]
:: [s0; Возвращает 1 if map contains `"second`", `"minute`", 
`"hour`", `"day`", `"month`" and `"year`" members, 0 otherwise.]
:: [s0; [*C GetSysTime()]]
:: [s0; Возвращает current time as map with `"second`", `"minute`", 
`"hour`", `"day`", `"month`" and `"year`" members.]
:: [s0; [*C sin(][/C x][*C )]&]
[s0; [*C cos(][/C x][*C )]&]
[s0; [*C tan(][/C x][*C )]&]
[s0; [*C asin(][/C x][*C )]&]
[s0; [*C acos(][/C x][*C )]&]
[s0; [*C atan(][/C x][*C )]&]
[s0; [*C atan2(][/C a][*C , ][/C b][*C )]&]
[s0; [*C sinh(][/C x][*C )]&]
[s0; [*C cosh(][/C x][*C )]&]
[s0; [*C tanh(][/C x][*C )]&]
[s0; [*C asinh(][/C x][*C )]&]
[s0; [*C acosh(][/C x][*C )]&]
[s0; [*C atanh(][/C x][*C )]&]
[s0; [*C exp(][/C x][*C )]&]
[s0; [*C log(][/C x][*C )]&]
[s0; [*C log10(][/C x][*C )]&]
[s0; [*C exp2(][/C x][*C )]&]
[s0; [*C log2(][/C x][*C )]&]
[s0; [*C sqrt(][/C x][*C )]&]
[s0; [*C cbrt(][/C x][*C )]&]
[s0; [*C pow(][/C base][*C , ][/C exp][*C )]]
:: [s0; Well know mathematical functions.]}}&]
[s0; ]]