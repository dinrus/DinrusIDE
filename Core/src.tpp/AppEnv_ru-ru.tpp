topic "Приложение & Среда (утилиты)";
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
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Приложение `& Среда (утилиты)]]}}&]
[s9;%RU-RU Ряд утилитных функций для получения 
информации о вашем приложении и системной 
среде.&]
[s3; &]
[s0;%RU-RU &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Функций]]}}&]
[s3; &]
[s5;:GetExeFilePath`(`): [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ Stri
ng]_[* GetExeFilePath]()&]
[s2;%RU-RU Возвращает абсолютный путь к 
бинарнику приложения.&]
[s3; &]
[s4; &]
[s5;:GetEnv`(const char`*`): [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ S
tring]_[* GetEnv]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id])&]
[s2;%RU-RU Возвращает значение переменной 
среды [*@3 id].&]
[s3;%RU-RU &]
[s4; &]
[s5;:GetExeDirFile`(const char`*`): [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ S
tring]_[* GetExeDirFile]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 fp])&]
[s2;%RU-RU Возвращает абсолютный путь файла, 
находящегося в папке бинарников приложения.
&]
[s2;%RU-RU [*@(129.0.0) fp][@(129.0.0) ..................................................
]имя файла&]
[s3; &]
[s4; &]
[s5;:GetHomeDirectory`(`): [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ St
ring]_[* GetHomeDirectory]()&]
[s2;%RU-RU Возвращает абсолютный путь домашней 
папки текущего пользователя.&]
[s3; &]
[s4; &]
[s5;:GetHomeDirFile`(const char`*`): [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ S
tring]_[* GetHomeDirFile]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 fp])&]
[s2;%RU-RU Возвращает абсолютный путь файла, 
расположенного в домашней папке текущего 
пользователя.&]
[s7;%RU-RU [*@3 fp]..................................................имя 
файла&]
[s0;%RU-RU &]
[s4; &]
[s5;:GetExeTitle`(`): [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ String]_
[* GetExeTitle]()&]
[s2;%RU-RU Возвращает титул этого приложения.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:GetAppFolder`(`): [_^Upp`:`:String^ String]_[* GetAppFolder]()&]
[s6; `[MacOS only`]&]
[s2;%RU-RU Фукнция возвращает путь к папке 
приложения, если исполнимый находится 
в какой`-то папке.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:GetAppName`(`): [_^Upp`:`:String^ String]_[* GetAppName]()&]
[s2;%RU-RU Возвращает имя приложения. Дефолтное 
значение `= GetExeTitle, но его можно изменить 
посредством SetAppName. Это значение используетс
я, например, как имя папки конфигурации.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SetAppName`(const Upp`:`:String`&`): [@(0.0.255) void]_[* SetAppName]([@(0.0.255) c
onst]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 name])&]
[s2;%RU-RU Переписывает имя приложения.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:SetConfigGroup`(const char`*`): [@(0.0.255) void]_[* SetConfigGroup]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 group])&]
[s2;%RU-RU На системах POSIX, функция ConfigFile 
даёт дефолтный путь внутри паки `".cache`". 
Эта функция позволяет установить 
имя подпапки, в другой подпапке которой 
с именем GetAppName() будет сохранена конфигураци
я. Установка group пустой просто удаляет 
весь уровень. Дефолтное значение 
`= `"u`+`+`".&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:GetConfigGroup`(`): [_^Upp`:`:String^ String]_[* GetConfigGroup]()&]
[s2;%RU-RU Возвращает текущую группу config.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:GetTempDirectory`(`): [_^Upp`:`:String^ String]_[* GetTempDirectory]()&]
[s2;%RU-RU Вовзращает временную папку.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:TempFile`(const char`*`): [_^Upp`:`:String^ String]_[* TempFile]([@(0.0.255) co
nst]_[@(0.0.255) char]_`*[*@3 filename])&]
[s2;%RU-RU Возвращает абсолютный путь файла 
во временной папке.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:GetExeFolder`(`): [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ S
tring]_[* GetExeFolder]()&]
[s2;%RU-RU Возвращает директорию исполнимого 
приложения.&]
[s3; &]
[s4; &]
[s5;:UseHomeDirectoryConfig`(bool`): [@(0.0.255) void]_[* UseHomeDirectoryConfig]([@(0.0.255) b
ool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%RU-RU Устанавливает путь поиска конфигурации 
либо на домашнюю паку текущего пользователя
, либо на папку, где находится бинарник 
приложения.&]
[s2;%RU-RU [*@(129.0.0) b]...................................................испол
ьзовать домашнюю папку (true) или папку 
приложения (false)&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SetConfigDirectory`(const Upp`:`:String`&`): [@(0.0.255) void]_[* SetConfigDi
rectory]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 s])&]
[s2;%RU-RU Переписывает директорию, в которой 
хранятся файлы конфигурации.&]
[s3;%RU-RU &]
[s4; &]
[s5;:ConfigFile`(const char`*`): [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ S
tring]_[* ConfigFile]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 file])&]
[s2;%RU-RU Возвращает абсолютный путь файла 
конфигурации. Эта директория зависит 
от хостовой платформы и других настроек. 
Если уже установлена какая`-то, посредством 
SetConfigDirectory, используется она. Иначе, 
на POSIX, функция ищет папку .config, начиная 
с директории бинарника, затем выше, 
до корня. Если не найдено, используется 
переменная среды XDG`_CONFIG`_HOME. Если её 
не существует, используется `~/.config. 
Затем функция приставляет GetAppName() 
к этой директории и проверяет её наличие 
(посредством RealizeDirectory). В Win32, это либо 
директория файла .exe, либо, если UseHomeDirectoryConfig 
установлен, домашняя директория пользовател
я.&]
[s3; &]
[s4; &]
[s5;:ConfigFile`(`): [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ String]_
[* ConfigFile]()&]
[s2;%RU-RU Возвращает дефолтный файл конфигурации 
из пути поиска конфигурационных файлов. 
Дефолтный файл конфигурации называется 
[*/ `[GetAppName()`]][* .cfg].&]
[s3; &]
[s4; &]
[s5;:CommandLine`(`): [@(0.0.255) const]_[_^topic`:`/`/Core`/src`/Vector`$en`-us`#Vector`:`:class^ V
ector][@(0.0.255) <][_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ String][@(0.0.255) >
`&]_[* CommandLine]()&]
[s2;%RU-RU Возвращает  вектор стрингов, содержащий 
параметры командной строки. В этом 
массиве отсутствует main`'ов argv`[0`],поэтому 
число элементов равно argc `- 1.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:GetArgv0`(`): [_^Upp`:`:String^ String]_[* GetArgv0]()&]
[s2;%RU-RU Возвращает аргумент argv`[0`] main (содержит 
имя исполнимого).&]
[s3; &]
[s4; &]
[s5;:Environment`(`): [@(0.0.255) const]_[_^topic`:`/`/Core`/src`/VectorMap`$en`-us`#VectorMap`:`:class^ V
ectorMap][@(0.0.255) <][_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ Stri
ng], [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ String][@(0.0.255) >`&
]_[* Environment]()&]
[s2;%RU-RU Возвращает вектор строк с системными 
переменными среды.&]
[s3; &]
[s4; &]
[s5;:SetExitCode`(int`): [@(0.0.255) void]_[* SetExitCode]([@(0.0.255) int]_[*@3 code])&]
[s2;%RU-RU Устанавливает код выхода, который 
будет возвращён на выходе приложения.&]
[s2;%RU-RU [*@(129.0.0) code].............................................код 
выхода&]
[s3; &]
[s4; &]
[s5;:GetExitCode`(`): [@(0.0.255) int]_[* GetExitCode]()&]
[s2;%RU-RU Получает код выхода, который будет 
возвращён на выходе приложения.&]
[s3; &]
[s4; &]
[s5;:IsMainRunning`(`): [@(0.0.255) bool]_[* IsMainRunning]()&]
[s2;%RU-RU Возвращает true, если выполнение 
приложения достигло, но ещё не вышло 
из главной функции main.&]
[s3; &]
[s4; &]
[s5;:GetDataFile`(const char`*`): [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ S
tring]_[* GetDataFile]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 filename])&]
[s2;%RU-RU Эта функция разработана для получения 
пути файла, со специфичными данными 
приложения. Поведение зависит от 
того, было ли приложение запущено 
из `'DinrusIDE`': в таком случае DinrusIDE передаёт 
ссылку на директорию главного пакета 
через переменную среды `"UPP`_MAIN`_`_, а 
итоговый файл находится в этой директории. 
При одиночном старте, эта функция 
эквивалентна GetExeDirFile. Альтернативно, 
приложение может вызвать SetDataPath, чтобы 
определить эту директорию.&]
[s3; &]
[s4; &]
[s5;:LoadDataFile`(const char`*`): [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ S
tring]_[* LoadDataFile]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 filename])&]
[s2;%RU-RU То же, что и LoadFile(GetDataFile([%-*@3 filename])).&]
[s3;%RU-RU &]
[s4; &]
[s5;:SetDataPath`(const char`*`): [@(0.0.255) void]_[* SetDataPath]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 path])&]
[s2;%RU-RU Явно определяет директорию, которая 
будет использоваться GetDataFile.&]
[s3;%RU-RU &]
[s4; &]
[s5;:GetComputerName`(`): [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ Str
ing]_[* GetComputerName]()&]
[s2;%RU-RU Возвращает имя компьютера.&]
[s3; &]
[s4; &]
[s5;:GetUserName`(`): [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ String]_
[* GetUserName]()&]
[s2;%RU-RU Возвращает текущего пользователя.&]
[s3; &]
[s4; &]
[s5;:GetDesktopManager`(`): [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ S
tring]_[* GetDesktopManager]()&]
[s2;%RU-RU Возвращает текущий идентификатор 
ГИП (GUI). Предопределены значения `"windows`", 
`"gnome`", `"kde`". Если в этом списке отсутствует 
менеджер рабочего стола X11, то возвращается 
значение переменной среды `"DESKTOP`_SESSION`".&]
[s3; &]
[s4; &]
[s5;:LaunchWebBrowser`(const String`&`): [@(0.0.255) void]_[* LaunchWebBrowser]([@(0.0.255) c
onst]_[_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ String][@(0.0.255) `&
]_[*@3 url])&]
[s2;%RU-RU Открывает URL в дефолтном Web Browser.&]
[s2;%RU-RU [*@(129.0.0) url]................................................URL, 
который будет открыт в браузере&]
[s3; &]
[s4; &]
[s5;:SetHomeDirectory`(const char`*`): [@(0.0.255) void]_[* SetHomeDirectory]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 dir])&]
[s2;%RU-RU Устанавливает домашнюю папку 
текущего пользователя.&]
[s2;%RU-RU [*@(129.0.0) dir]................................................URL, 
который будет открыт в браузере&]
[s3;%RU-RU &]
[s4; &]
[s5;:CpuMMX`(`): [@(0.0.255) bool]_[* CpuMMX]()&]
[s2;%RU-RU Возвращает true, если ЦПБ поддерживает 
MMX.&]
[s3;%RU-RU &]
[s3;%RU-RU &]
[s4; &]
[s5;:CpuSSE`(`): [@(0.0.255) bool]_[* CpuSSE]()&]
[s2;%RU-RU Возвращает true, если ЦПБ поддерживает 
SSE.&]
[s3; &]
[s4; &]
[s5;:CpuSSE2`(`): [@(0.0.255) bool]_[* CpuSSE2]()&]
[s2;%RU-RU Возвращает true, если ЦПБ поддерживает 
SSE2.&]
[s3; &]
[s4; &]
[s5;:CpuSSE3`(`): [@(0.0.255) bool]_[* CpuSSE3]()&]
[s2;%RU-RU Возвращает true, если ЦПБ поддерживает 
SSE3.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:CpuHypervisor`(`): [@(0.0.255) bool]_[* CpuHypervisor]()&]
[s2;%RU-RU Проверяет, установлен ли флаг 
ЦПБ `"hypervisor`". Если да, то программа 
выполняется на виртуальной машине. 
К сожалению, противоположное неверно: 
некоторые VM`'ы работают без установки 
этого флага.&]
[s3; &]
[s4; &]
[s5;:CPU`_Cores`(`): [@(0.0.255) int]_[* CPU`_Cores]()&]
[s2;%RU-RU Возвращает число ядер ЦПБ.&]
[s3; &]
[s0; ]]