topic "Логирование U++";
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
[{_}%RU-RU 
[ {{10000@(113.42.0) [s0; [*@7;4 Логирование U`+`+]]}}&]
[s0;i448;a25;kKO9;@(0.0.255)%- &]
[s0; Инфраструктура логирования U`+`+ основана 
на выводном потоке, способном выводить 
строки лога (`"журнала`") в выбранные 
средства вывода (файлы, консоль, системный 
лог).&]
[s0; &]
[s0; Само логирование затем опирается 
на набор макросов, которые выводят 
значения в этот поток, обычно используя 
AsString или оператор<<(Stream`&, ...), чтобы 
преобразовывать значения.&]
[s0; &]
[s0; Этот поток доступен через функцию 
UppLog() и её дефолтная версия предложена 
U`+`+, однако, её можно заменить (посредством 
SetUppLog).&]
[s0; &]
[s0; Заметка о многопоточности: Стандартный 
поток логирования обрабатывает сериализаци
ю вывода в своей реализации (и любая 
вероятная замена тоже должна это 
делать), поэтому можно использовать 
единый поток вывода из всех потоков.&]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Список Сущностей]]}}&]
[s4;%- &]
[s5;:StdLogSetup`(dword`,const char`*`,int`):%- [@(0.0.255) void]_[* StdLogSetup]([_^dword^ d
word]_[*@3 options], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 filepath]_`=_NULL, 
[@(0.0.255) int]_[*@3 filesize`_limit]_`=_[@3 10]_`*_[@3 1024]_`*_[@3 1024])&]
[s2; Эта функция настраивает стандартное 
логирование U`+`+. [%-*@3 filepath] `- путь к файлу 
.log (если активно логирование в файл), 
[%-*@3 filesize`_limit] `- максимальный размер 
лога (если активны опции LOG`_FILE), если 
настройка имела место, создаётся 
новый файл лога. [%-*@3 options] `- это комбинация 
бит`-флагов:&]
[s3; &]
[ {{2939:7061<288;^ [s0; LOG`_FILE ]
:: [s0; Выводить лог в файл (это дефолт). Дефолтный 
путь файла ConfigFile(`"имя`-программы.log`").]
:: [s0; LOG`_COUT ]
:: [s0; Выводить лог в стандартный вывод, 
используя простой putchar.]
:: [s0; LOG`_CERR ]
:: [s0; Выводить лог в вывлд для ошибок, используя 
простой putc.]
:: [s0; LOG`_DBG ]
:: [s0; Выводить лог в отладчик (Специфично 
для Win32).]
:: [s0; LOG`_SYS ]
:: [s0; Выводить лог в syslog (Специфично для 
POSIX).]
:: [s0; LOG`_ELAPSED]
:: [s0; Предпоставлять число миллисекунд 
к каждой строке лога, прошедшее от 
появления предыдущей строки.]
:: [s0; LOG`_TIMESTAMP ]
:: [s0; Предпоставлять штамп локального 
времени в каждой строке лога (не относится 
к LOG`_SYS, так как syslog это делает сам).]
:: [s0; LOG`_TIMESTAMP`_UTC]
:: [s0; Предпоставлять штамп универсального 
времени в каждой строке лога (не относится 
к LOG`_SYS).]
:: [s0; LOG`_APPEND ]
:: [s0; При старте программы, добавлять лог 
в существующий файл, вместо его замены.]
:: [s0; LOG`_ROTATE([*@3 x])]
:: [s0; При старте нового файла лога, сохранять 
до [*@3 x] более старых логов (переименованных 
с расширением `'.1`', `'.2`' и т.д.).]
:: [s0; LOG`_ROTATE`_GZIP]
:: [s0; Старые сохранённые файлы сжимать 
с помощью gzip (кроме самого нового 
лога `'.1`'.]
:: [s0; LOG`_COUTW]
:: [s0; Выводить лог на стандартный вывод, 
используя Cout. Это предоставляет вероятное 
преобразование символов UTF8, за счёт 
использования кучи (поэтому не может 
использоваться, напр., внутри процедур 
кучи).]
:: [s0; LOG`_CERRW]
:: [s0; Выводить лог на стандартный вывод, 
используя Cerr.Это предоставляет вероятное 
преобразование символов UTF8, за счёт 
использования кучи (поэтому не может 
использоваться, напр., внутри процедур 
кучи).]}}&]
[s0; &]
[s4;%- &]
[s5;:StdLog`(`):%- [_^Stream^ Stream][@(0.0.255) `&]_[* StdLog]()&]
[s2; Возвращает ссылку на стандартный 
поток логирования.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:GetStdLogPath`(`):%- [_^Upp`:`:String^ String]_[* GetStdLogPath]()&]
[s2; Возвращает путь к текущему логу, 
если он имеется.&]
[s3;%- &]
[s4;%- &]
[s5;:LOG`_BEGIN:%- [@(0.0.255) const]_[@(0.0.255) char]_[* LOG`_BEGIN `= 
`'`\x1e`';]&]
[s2; Помещение этого символа в стандартный 
поток логирования добавляет один 
табулятор отступа во все последующие 
строки (сдвигает текст `"вправо`").&]
[s3;%- &]
[s4;%- &]
[s5;:LOG`_END:%- [@(0.0.255) const]_[@(0.0.255) char]_[* LOG`_END `= `'`\x1f`';]&]
[s2; Помещение этого символа в стандартный 
поток логирования удаляет один табулятор 
отступа во всех последующих строках 
(сдвигает текст `"влево`").&]
[s3;%- &]
[s4;%- &]
[s5;:UppLog`(`):%- [_^Stream^ Stream][@(0.0.255) `&]_[* UppLog]()&]
[s2; Возвращает ссылку на [/ текущий] поток 
логирования.&]
[s3; &]
[s4;%- &]
[s5;:SetUppLog`(Stream`&`):%- [@(0.0.255) void]_[* SetUppLog]([_^Stream^ Stream][@(0.0.255) `&
]_[*@3 log])&]
[s2; Устанавливает текущий поток логирования 
в [%-*@3 log] (должен быть объектом с глобальным 
сроком существования).&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:SetUppLog`(Upp`:`:LogLineFn`):%- [_^Upp`:`:LogLineFn^ LogLineFn]_[* SetUppLog
]([_^Upp`:`:LogLineFn^ LogLineFn]_[*@3 log`_line])&]
[s0;l288; Подобно SetUppLog с параметром Stream, 
но вместо замены всего потока, заменяется 
только последняя процедура, чтобы 
записать единственную строку, возвращая 
её предыдущее определение (как указатель 
на функцию). LogLineFn определена как&]
[s0;l288; &]
[s0;l288; [C typedef void (`*LogLineFn)(const char `*buffer, int len, 
int depth);]&]
[s2; &]
[s2; где dept инкрементирован символом 
LOG`_BEGIN и декрементирован LOG`_END `- стандартный 
поток логирования вставляет эквивалентное 
число табуляций в начале строки.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Макросы логгинга]]}}&]
[s0; &]
[s0; Стандартные макросы логирования 
идут в 3`-х основных категориях. `"Нормальные`"
 макросы (LOG, DUMP, ...) эмитируют код только 
в отладочном режиме, `"Отладочные`" 
макросы (DLOG, DDUMP, ..., первая буква `'D`') 
[*/ компилируются] только в отладочном 
режиме и в их присутствии невозможна 
компиляция в режиме выпуска (что предотвраща
ет в коде наличие забытых отладочных 
макросов), и, наконец, `"Релизные`" макросы 
(RLOG, RDUMP, ..., первая буква `'R`') производят 
вывод даже в режиме выпуска.&]
[s0; &]
[ {{5000:5000^ [s0; LOG(x), DLOG(x), RLOG(x)]
:: [s0; Выводит в лог одну строку.]
:: [s0; DUMP(x), DDUMP(x), RDUMP(x)]
:: [s0; Выводит переменную `- добавляет имя 
переменной перед значением `-  определён 
как LOG(#x << `"`=`" << x)]
:: [s0; DUMPC(x), DDUMPC(x), RDUMPC(x)]
:: [s0; Выводит Vector, или Array, или Index значений.]
:: [s0; DUMPM(x), DDUMPM(x), RDUMPM(x)]
:: [s0; Выводит VectorMap или ArrayMap.]
:: [s0; TIMING(x), DTIMING(x), RTIMING(x)]
:: [s0; Устанавливает инспектор тайминга 
профилирования, профилирующий от 
определения до конца блока, профилируемые 
значения выводятся в лог по выходу 
из программы.]
:: [s0; LOGHEX(x), DLOGHEX(x), RLOGHEX(x)]
:: [s0; Выводит значение как 16`-ричный дамп, 
сейчас уже работает со String.]
:: [s0; DUMPHEX(x), DDUMPHEX(x), RDUMPHEX(x)]
:: [s0; Выводит переменную как 16`-ричный 
дамп, наподобии LOGHEX, но помещает в 
лог имя переменной.]}}&]
[s0; &]
[s0; В дополнение к этому общеприменимо 
использование `"локальных`" макросов, 
которые могут включаться/отключаться 
для указанных файлов модулей; стандартно 
добавляется&]
[s0; &]
[s0; #define LLOG(x)    // DLOG(x)&]
[s0; &]
[s0; в начале файлов с локальным логированием, 
а затем логирование активируется, 
раскомментированием DLOG.&]
[s0; &]
[s0; U`+`+ имеет также понятие `"модульное`" 
логгирование, когда создаётся особый 
макрос логиирования, привязанны к 
некой глобалльной булевой сущности 
(обычно INI`_BOOL). Создание таких модульных 
логов упрощено макросом:&]
[s0; &]
[s0; #define [* LOG`_](flag, x)     do `{ if(flag) RLOG(x); `} while(false)&]
[s0; &]
[s0; Существует один такой предопределённый 
тип, USRLOG, предназначенный для логирования 
действий пользователя (напр., открытие 
окон, нажатие клавиш клавиатуры). 
Его можно активировать посредством 
IniBool Ini`::user`_log (а также установив user`_log`=true 
в файле .ini):&]
[s0; &]
[s0; #define [* USRLOG](x)         LOG`_(Ini`::user`_log, x)&]
[s0; ]]