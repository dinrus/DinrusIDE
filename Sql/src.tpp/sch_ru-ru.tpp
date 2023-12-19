topic ".sch - Файлы схемы SQL";
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
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 .sch `- Файлы схемы SQL]]}}&]
[s0;%RU-RU &]
[s0; [3 Файлы .sch представляют собой схему 
базы данных SQL. Эти файлы входят в 
целевое приложение с различными определения
ми предлагаемых (поставляемых) .sch 
`"ключевых слов`"]&]
[s0;3 &]
[s0;i150;O0; [3 Константы SqlId]&]
[s0;i150;O0; [^topic`:`/`/Sql`/src`/S`_type`_ru`-ru^3 S`_][/^topic`:`/`/Sql`/src`/S`_type`_ru`-ru^3 t
ype][3  структуры для мапирования рядов 
таблицы в структуру C`+`+]&]
[s0;i150;O0; [3 .sql скрипты для создания/апгрейда/ 
`"урона`" схемы]&]
[s0;3 &]
[s0; [3 Пример содержимого .sch:]&]
[s0;*C@5;3 &]
[s0; [*C@5;3 TABLE`_(DEMO)]&]
[s0; [*C@5;3 -|INT`_(ID) PRIMARY`_KEY AUTOINCREMENT]&]
[s0; [*C@5;3 -|STRING`_(UN, 200) UNIQUE]&]
[s0; [*C@5;3 -|STRING`_(W, 200) INDEX]&]
[s0; [*C@5;3 -|INT`_ (F`_ID) REFERENCES(TABLE2)]&]
[s0; [*C@5;3 END`_TABLE]&]
[s0;3 &]
[s0; [3 Все ключевые слова .sch, содержащие 
имя таблицы, тип или колонку, имеют 
два варианта `- простой и с постфиксом 
`'`_`' (напр., TABLE и TABLE`_). Вариант с постфиксом 
`'`_`' также генерирует глобальную переменную 
SqlId, у которой такое же имя и значение 
как у таблицы/типа/колонки (контент 
может меняться посредством SQL`_NAME). 
Этим имена колонок и таблиц подготавливаютс
я к использованию с выражениями SQL.]&]
[s0;3 &]
[s0; [3 Большинство ключевых слов для определения 
колонок идёт с вариантом ARRAY, который 
не использует никаких возможностей 
массивов движка SQL, а скорее вставляет 
размер массива элементов с особыми 
именами, эммулируя фиксированный 
массив.]&]
[s0;3 &]
[s0; [3 Список ключевых слов файла .sch:]&]
[s0;3 &]
[ {{5007:4993^ [s0; [* TABLE(][*/ таблица][* )]]
:: [s0; Определяет новую таблицу. Создаёт 
структуры S`_ , чтобы создать мап таблицы 
(`"мапировать таблицу`") SQL в структуру 
C`+`+, записи интроспекции, а также может 
создавать сценарии создания/апгрейда 
схемы SQL.]
:: [s0; [* TABLE`_I(][*/ таблица][* , ][*/ base1][* )]]
:: [s0; Определяет новую таблицу, в которой 
имеются все колонки из [/ base1, ]который 
может быть либо таблицкй, лиюо типом.]
:: [s0; [* TABLE`_II(][*/ таблица][* , ][*/ base1][* , ][*/ base2][* )]]
:: [s0; Определяет новую таблицу, у которой 
все колонки от [/ base1 и] [/ base2, ]которые 
могут быть либо таблицами, либо типами.]
:: [s0; [* TABLE`_III(][*/ таблица][* , ][*/ base1][* , ][*/ base2][* , ][*/ base3][* )]]
:: [s0; Определяет новую таблицу, у которой 
к тому же все колонки от [/ base1 , base2 ]и 
[/ base3], которые могут быть либо таблицами, 
либо типами.]
:: [s0; [* END`_TABLE]]
:: [s0; Завершает определение таблицы.]
:: [s0; [* TYPE(][*/ тип][* )]]
:: [s0; Определяет новый тип. В отличие от 
TABLE, TYPE не производит код создания 
таблиц в сценариях схем sql. Списки 
колонок и атрибутов следуют ниже 
этого заголовка, завершаясь END`_TYPE.]
:: [s0; [* TYPE`_I(][*/ тип][* , ][*/ base][* )]]
:: [s0; Определяет новый тип, у которого 
все колонки от [/ base.]]
:: [s0; [* TYPE`_II(][*/ тип][* , ][*/ base1][* , ][*/ base2][* )]]
:: [s0; Определяет новый тип, у которого 
все колонки от  [/ base1 ]и [/ base2].]
:: [s0; [* TYPE`_III(][*/ тип][* , ][*/ base1][* , ][*/ base2][* , ][*/ base3][* )]]
:: [s0; Определяет новый тип, у которого 
все колонки от [/ base1 , base2 ]и [/ base3].]
:: [s0; [* END`_TYPE]]
:: [s0; Завершает определение типа.]
:: [s0; [* SQL`_NAME(][*/ id][* )]]
:: [s0; Предоставляет альтернативное имя 
SQL для предыдущего элемента. При использован
ии имя колонки будет использоваться 
в контексе C`+`+, а это альтернативное 
имя `- в контексте SQL. [*/ id]  `- литерал 
Си.]
:: [s0; ]
:: [s0; ]
:: [s0; [* INT(][*/ имя`_колонки][* )]]
:: [s0; Колонка, способная хранить 32`-битное 
целое значение, тип int в C`+`+.]
:: [s0; [* INT`_ARRAY(][*/ имя`_колонки][* , ][*/ элементы][* )]]
:: [s0; Массив из 32`-битных целых чисел.]
:: [s0; [* INT64(][*/ имя`_колонки][* )]]
:: [s0; Колонка, способная хранить 64`-битное 
целое значение, тип int64 в C`+`+. `[sqlite3`]]
:: [s0; [* INT64`_ARRAY(][*/ имя`_колонки][* , ][*/ элементы][* )]]
:: [s0; Массив из 64`-битных целых значений. 
`[sqlite3`]]
:: [s0; [* DOUBLE(][*/ имя`_колонки][* )]]
:: [s0; Колонка, способная хранить значение 
с плавающей точкой двойной точности, 
тип double в C`+`+.]
:: [s0; [* DOUBLE`_ARRAY(][*/ имя`_колонки][* , ][*/ элементы][* )]]
:: [s0; Массив из значений типа double.]
:: [s0; [* STRING(][*/ имя`_колонки][* , ][*/ maxlen][* )]]
:: [s0; Колонка, способная хранить строку 
с предельным числом символов [/ maxlen], 
тип String в C`+`+.]
:: [s0; [* STRING`_ARRAY(][*/ имя`_колонки][* , n, ][*/ элементы][* )]]
:: [s0; Массив из строк (текст).]
:: [s0; [* DATE(][*/ имя`_колонки][* )]]
:: [s0; Колонка, способная хранить календарную 
дату (без времени), тип Date в C`+`+.]
:: [s0; [* DATE`_ARRAY(][*/ имя`_колонки][* , ][*/ элементы][* )]]
:: [s0; Массив дат.]
:: [s0; [* TIME(][*/ column`_name][* )]]
:: [s0; Колонка, способная хранить календарную 
дату со временем, тип Time в C`+`+.]
:: [s0; [* TIME`_ARRAY(][*/ имя`_колонки][* , ][*/ элементы][* )]]
:: [s0; Массив из дат`-времён.]
:: [s0; [* BOOL(][*/ имя`_колонки][* )]]
:: [s0; Колонка, способная хранить булево 
значение, тип bool в C`+`+. Важно: в целях 
совместимости между разными движками 
SQL, BOOL всегда эмулируется текстовым 
значением в один символ (`"1`" для true). 
Преобразование предоставляется для 
типов S`_, но команды SQL должны это учитывать. 
]
:: [s0; [* BOOL`_ARRAY(][*/ имя`_колонки][* , ][*/ элементы][* )]]
:: [s0; Массив из булевых значений.]
:: [s0; [* BIT]]
:: [s0; 0 или 1. В C`+`+ это тип int. `[mssql`]]
:: [s0; [* BIT`_ARRAY]]
:: [s0; Массив из 0 или 1. `[mssql`]]
:: [s0; [* BLOB(][*/ имя`_колонки][* )]]
:: [s0; Двоичные данные неограниченного 
размера. В C`+`+ это тип String.]
:: [s0; [* CLOB(][*/ имя`_колонки][* )]]
:: [s0; Текст неограниченного размера. В 
C`+`+ это тип String. `[mysql`] `[postgresql`] `[oracle`]]
:: [s0; [* SERIAL(][*/ имя`_колонки][* )]]
:: [s0; Тип PostgreSQL [* serial] (эквивалентен автоинкременту
 в некоторых других движках SQL). Тип 
в базе данных `- 32`-битное беззначное 
целое, в C`+`+ это тип int64.]
:: [s0; [* ISERIAL(][*/ имя`_колонки][* )]]
:: [s0; Тип PostgreSQL [* serial], с  int в типе C`+`+.]
:: [s0; [* BIGSERIAL(][*/ имя`_колонки][* )]]
:: [s0; Тип PostgreSQL 64`-битный [* bigserial], с int64 в 
типе C`+`+.]
:: [s0; [* NOT`_NULL]]
:: [s0; Констрейнт Not null.]
:: [s0; [* PRIMARY`_KEY]]
:: [s0; Колонка с первичным ключом.]
:: [s0; [* AUTO`_INCREMENT]]
:: [s0; Колонка автоинкремента. `[sqlite3`] `[mysql`]]
:: [s0; [* REFERENCES(][*/ table][* )]]
:: [s0; Определение стороннего ключа для 
колонки.]
:: [s0; [* REFERENCES`_CASCADE(][*/ table][* )]]
:: [s0; Сторонний ключ с опцией `"ON DELETE CASCADE`". 
`[sqlite3`]]
:: [s0; [* DUAL`_PRIMARY`_KEY(][*/ column1][* , ][*/ column2][* )]]
:: [s0; Дуальный первичный ключ таблицы.]
:: [s0; [* SQLDEFAULT(v)]]
:: [s0; Дефолтное значение колонки. `[sqlite3`] 
`[mysql`] `[postgresql`]]
:: [s0; [* INDEX]]
:: [s0; У колонки есть индекс.]
:: [s0; [* PARTIAL`_INDEX(][*/ условие][* )]]
:: [s0; У колонки частичный индекс. `[postgresql`]]
:: [s0; [* PARTIAL`_INDEX`_(][*/ имя`_констрейнта][* , ][*/ условие][* )]]
:: [s0; У колонки именованый частичный индекс. 
`[postgresql`]]
:: [s0; [* UNIQUE]]
:: [s0; У колонки констрейнт UNIQUE.]
:: [s0; [* UNIQUE`_LIST(][*/ имя`_констрейнта][* , ][*/ list][* )]]
:: [s0; Создаёт констрейнт UNIQUE для набора 
колонок. [*/ list]  `- это литерал Си с именами 
колонок, разделёнными запятой.]
:: [s0; [* DUAL`_UNIQUE(][*/ column1][* , ][*/ column1][* )]]
:: [s0; Констрейнт UNIQUE для двух колонок.]
:: [s0; [* INDEX`_LIST(][*/ имя`_констрейнта][* , ][*/ list][* )]]
:: [s0; [*/ list] i`- это литерал Си с именами колонок, 
разделёнными запятыми. `[sqlite3`]]
:: [s0; [* CHECK`_IN(][*/ list][* )]]
:: [s0; Значение колонки констрейнтов к 
списку [*/ list], литералу Си, разделённому 
запятыми.`[sqlite3`]]
:: [s0; [* CHECK`_COND(][*/ условие][* )]]
:: [s0; Значение колонки констрейнтов, удовлетворя
ющее условию [*/ condition], являющемуся 
литералом Си с выражением. `[sqlite3`]]
:: [s0; [* SQLCHECK(][*/ имя`_констрейнта][* , ][*/ check][* )]]
:: [s0; Добавляет именованый констрейнт 
CHECK, [*/ check] `- литерал Си с выражением 
проверки. `[sqlite3`] `[postgresql`] `[mssql`]]
:: [s0; [* INNODB]]
:: [s0; У таблицы тип INNODB. `[mysql`]]
:: [s0; [* SEQUENCE(][*/ имя`_последовательности][* )]]
:: [s0; Последовательность. `[postgresql`] `[mssql`] 
`[oracle`]]
:: [s0; [* COMMEN(][*/ комментарий][* )]]
:: [s0; Вставляет комментарий в данный сценарий.]}}&]
[s0;%RU-RU &]
[s0;3 &]
[s0;3 &]
[s0;%RU-RU ]]