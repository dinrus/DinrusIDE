topic "Утилиты файлового пути";
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
[s0;%RU-RU [*@3;4 Утилиты файлового пути]&]
[s3; &]
[s0; &]
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Структура FileTime]]}}&]
[s3; &]
[s1;:FileTime`:`:struct: [@(0.0.255)3 struct][3 _][*3 FileTime][3 _:_][@(0.0.255)3 public][3 _][*@3;3 C
ompareRelOps][@(0.0.255)3 <const][3 _][*3 FileTime][@(0.0.255)3 `&][3 , 
`&Compare`_FileTime][@(0.0.255)3 >][3 _]&]
[s9;%RU-RU FileTime `- структура, инкапсулирующая 
специфичный для хоста тип, используемый 
под штампы времени (timestamps) файлов 
и директорий.&]
[s9;%RU-RU У неё определённый порядок. Её 
можно сравнивать с Time. Time имеет конверсионны
й конструктор, состоящий из методов 
FileTime и AsFileTime, для обратной совместимости.&]
[s3;%RU-RU &]
[s0;%RU-RU &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Функций]]}}&]
[s3; &]
[s5;:PatternMatch`(const char`*`,const char`*`): [@(0.0.255) bool]_[* PatternMatch]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 p], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s])&]
[s2; [%RU-RU Сверяет образец имени файла ][*@3 p][%RU-RU  
с именем файла ][*@3 s][%RU-RU . Образец может 
использовать символы`-уайлдкарды: 
`'][%RU-RU@(0.0.255) ?][%RU-RU `', проверяющий на совпадение 
любого единичного символа или `'][%RU-RU@(0.0.255) `*][%RU-RU `'
 проверяющий на совпадение любое 
число любых символов (tэти уайлдкарды 
консистентны под unix/windows). Если образец 
оканчивается на `'][%RU-RU@(0.0.255) .][%RU-RU `', 
то не должно быть `'.`' в ][*@3 s] (у имени 
файла не может быть расширения).&]
[s3;%RU-RU &]
[s4; &]
[s5;:PatternMatchMulti`(const char`*`,const char`*`): [@(0.0.255) bool]_[* PatternMatchMu
lti]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 p], [@(0.0.255) const]_[@(0.0.255) char]_`*
[*@3 s])&]
[s2;%RU-RU Сверяет несколько образцов, разделённых
 точкой с запятой (`';`') в [%-*@3 p] с именем 
файла [%-*@3 s], используя [^topic`:`/`/Core`/src`/Path`_ru`-ru`#PatternMatch`(const char`*`,const char`*`)^ P
atternMatch].&]
[s3;%RU-RU &]
[s4; &]
[s5;:GetFileNamePos`(const char`*`): [@(0.0.255) const]_[@(0.0.255) char]_`*[* GetFileNameP
os]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 path])&]
[s2;%RU-RU Возвращает указатель на первый 
символ имени файла в пути [%-*@3 path] `- 
это символ после последнего разделителя 
пути (`'/`' на POSIX, `'`\`\`' или `'/`' на Win32). 
Если такой символ отсутствует, возвращается
 [%-*@3 path].&]
[s3;%RU-RU &]
[s4; &]
[s5;:GetFileExtPos`(const char`*`): [@(0.0.255) const]_[@(0.0.255) char]_`*[* GetFileExtPos
]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 path])&]
[s2;%RU-RU Если имя файлй в пути [%-*@3 path ]имеет 
какое`-либо расширение (фактически, 
содержит один или более символов 
`'.`'), то возвращается указатель на 
последнюю `'.`', иначе возвращается 
указатель на завершающий `'`\0`'.&]
[s3;%RU-RU &]
[s4; &]
[s5;:HasFileExt`(const char`*`): [@(0.0.255) bool]_[* HasFileExt]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 path])&]
[s2;%RU-RU Возвращает true, если путь [%-*@3 path] 
заканчивается именем файла с расширением 
(другими словами, если имя файла содержит 
`'.`').&]
[s3;%RU-RU &]
[s4; &]
[s5;:HasWildcards`(const char`*`): [@(0.0.255) bool]_[* HasWildcards]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 path])&]
[s2;%RU-RU Возвращает true, если путь [%-*@3 path] 
содержит символы `'`*`' или `'?`' (уалдкарды).&]
[s3;%RU-RU &]
[s4; &]
[s5;:IsFullPath`(const char`*`): [@(0.0.255) bool]_[* IsFullPath]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 path])&]
[s2;%RU-RU Возвращает true, если путь [%-*@3 path] 
абсолютен `- начинается с `'/`' на системах 
POSIX или с буквы диска в Win32.&]
[s3;%RU-RU &]
[s4; &]
[s5;:GetFileDirectory`(const char`*`): [_^String^ String]_[* GetFileDirectory]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 path])&]
[s2;%RU-RU Извлекает путь к директории из 
пути [%-*@3 path] `- подстроку, оканчивающуюся 
на последний разделитель пути (`'/`' 
на POSIX, `'/`' или `'`\`\`' на Win32). Разделитель 
[*/ включается] в конец результата. Если 
он отсутствует, то возвращается пустая 
строка.&]
[s2;%RU-RU Пример (POSIX): GetFileDirectory([@3 `"/home/user/test.ext`"]) 
возвращает [@3 `"/home/user/`"].&]
[s0;%RU-RU &]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:GetFileFolder`(const char`*`): [_^String^ String]_[* GetFileFolder]([@(0.0.255) const]_
[@(0.0.255) char]_`*[*@3 path])&]
[s2;%RU-RU Извлекает путь к директории из 
пути [%-*@3 path] `- подстроку, оканчивающуюся 
последним сепаратором пути (`'/`' на 
POSIX, `'/`' или `'`\`\`' на Win32). Сепаратор 
[*/ не включается] в конец результата. 
Если он отсутствует, то возвращается 
пустая строка.&]
[s2;%RU-RU Пример (POSIX): GetFileFolder([@3 `"/home/user/test.ext`"]) 
возвращает [@3 `"/home/user`"].&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:GetFileTitle`(const char`*`): [_^String^ String]_[* GetFileTitle]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 path])&]
[s2;%RU-RU Получает имя файла из пути [%-*@3 path] 
без расширения (если оно есть).&]
[s2;%RU-RU Пример (POSIX): GetFileTitle([@3 `"/home/user/test.ext`"]) 
возвращает [@3 `"test`"].&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:GetFileExt`(const char`*`): [_^String^ String]_[* GetFileExt]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 path])&]
[s2;%RU-RU Получает расширение файла, включая 
символ `'.`'. Если расширения нет, то 
возващает пустую строку.&]
[s2;%RU-RU Пример (POSIX): GetFileExt([@3 `"/home/user/test.ext`"]) 
возвращает[@3 `".ext`"].&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:GetFileName`(const char`*`): [_^String^ String]_[* GetFileName]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 path])&]
[s2;%RU-RU Возвращает имя файла (без директории).&]
[s2;%RU-RU Пример(POSIX): GetFileName([@3 `"/home/user/test.ext`"]) 
возвращает [@3 `"test.ext`"].&]
[s3;%RU-RU &]
[s4; &]
[s5;:AppendFileName`(const String`&`,const char`*`): [_^String^ String]_[* AppendFileName
]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 path], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 filename])&]
[s2;%RU-RU Приставляет имя файла [%-*@3 filename] 
к директории [%-*@3 path]. Главная цель этой 
операции `- поддерживать корректный 
разделитель пути. [%-*@3 path] допускается 
что, но не обязательно, оканчивается 
на этом сепараторе.&]
[s3;%RU-RU &]
[s4; &]
[s5;:WinPath`(const char`*`): [_^String^ String]_[* WinPath]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 path])&]
[s2;%RU-RU Преобразует `'/`' в [%-*@3 path] в `'`\`\`'.&]
[s3;%RU-RU &]
[s4; &]
[s5;:UnixPath`(const char`*`): [_^String^ String]_[* UnixPath]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 path])&]
[s2;%RU-RU Преобразует `'`\`\`' в [%-*@3 path] в `'/`'.&]
[s3;%RU-RU &]
[s4; &]
[s5;:NativePath`(const char`*`): [_^String^ String]_[* NativePath]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 path])&]
[s2;%RU-RU То же, что и [^topic`:`/`/Core`/src`/Path`$en`-us`#WinPath`(const char`*`)^ W
inPath] в Win32 или [^topic`:`/`/Core`/src`/Path`$en`-us`#UnixPath`(const char`*`)^ U
nixPath] в POSIX.&]
[s3;%RU-RU &]
[s4; &]
[s5;:AppendExt`(const char`*`,const char`*`): [_^String^ String]_[* AppendExt]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 path], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 ext])&]
[s2;%RU-RU Если путь [%-*@3 path] не имеет расширения, 
то приставляется [%-*@3 ext]. Возвращает 
итог.&]
[s3;%RU-RU &]
[s4; &]
[s5;:ForceExt`(const char`*`,const char`*`): [_^String^ String]_[* ForceExt]([@(0.0.255) co
nst]_[@(0.0.255) char]_`*[*@3 path], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 ext])&]
[s2;%RU-RU Изменяет расширение файла в пути 
[%-*@3 path] на [%-*@3 ext]. Если расширение отсутствует, 
оно приставляется. Возвращает итог.&]
[s3;%RU-RU &]
[s4; &]
[s5;:GetFileOnPath`(const char`*`,const char`*`,bool`,const char`*`): [_^String^ String
]_[* GetFileOnPath]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 file], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 paths], [@(0.0.255) bool]_[*@3 current]_`=_[@(0.0.255) t
rue], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 curdir]_`=_NULL)&]
[s2;%RU-RU Пытается найти файл [%-*@3 file] в разделённом 
точками с запятыми списке путей директории 
[%-*@3 paths]. Если [%-*@3 current] равно true, [%-*@3 curdir].добавляе
тся к этому списку. Если [%-*@3 curdir] равно 
NULL, то будет использована реальная 
текущая директория (полученная с 
помощью GetCurrentDirectory). Возвращает полный 
путь файла, если он найден, в противном 
случае `- Null.&]
[s3;%RU-RU &]
[s4; &]
[s5;:GetCurrentDirectory`(`): [_^String^ String]_[* GetCurrentDirectory]()&]
[s2;%RU-RU Возвращает текущую директорию.&]
[s3; &]
[s4; &]
[s5;:GetFileLength`(const char`*`): [_^int64^ int64]_[* GetFileLength]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 path])&]
[s2;%RU-RU Возвращает длину файла по пути 
[%-*@3 path ][%- или отрицательное число] , 
если файла не существует.&]
[s3;%RU-RU &]
[s4; &]
[s5;:FileExists`(const char`*`): [@(0.0.255) bool]_[* FileExists]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 path])&]
[s2;%RU-RU Возвращает true, если путь [%-*@3 path] 
представляет файл.&]
[s3;%RU-RU &]
[s4; &]
[s5;:DirectoryExists`(const char`*`): [@(0.0.255) bool]_[* DirectoryExists]([@(0.0.255) con
st]_[@(0.0.255) char]_`*[*@3 path])&]
[s2;%RU-RU Возвращает true, если путь [%-*@3 path] 
представляет директорию.&]
[s3;%RU-RU &]
[s4; &]
[s5;:GetFileTime`(const char`*`): [_^FileTime^ FileTime]_[* GetFileTime]([@(0.0.255) const]_
[@(0.0.255) char]_`*[*@3 path])&]
[s2;%RU-RU Возвращает последнее время записи 
в файл по пути [%-*@3 path]. Если элемента 
не существует, итог устанавливается 
в ноль (испольуется memset, так как FileTime 
является в Win32 структурой).&]
[s3;%RU-RU &]
[s4; &]
[s5;:FileGetTime`(const char`*`): [_^Time^ Time]_[* FileGetTime]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 path])&]
[s2;%RU-RU Возвращает последнее время записи 
в файл по пути [%-*@3 path] или Null, если файла 
не существует.&]
[s3;%RU-RU &]
[s4; &]
[s5;:SetFileTime`(const char`*`,FileTime`): [@(0.0.255) bool]_[* SetFileTime]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 path], [_^FileTime^ FileTime]_[*@3 ft])&]
[s2;%RU-RU Устанавливает последнее время 
записи в файл по пути [%-*@3 path ]в [%-*@3 ft].&]
[s3;%RU-RU &]
[s4; &]
[s5;:FileSetTime`(const char`*`,Time`): [@(0.0.255) bool]_[* FileSetTime]([@(0.0.255) const
]_[@(0.0.255) char]_`*[*@3 path], [_^Time^ Time]_[*@3 time])&]
[s2;%RU-RU Устанавливает последнее время 
записи в файл по пути [%-*@3 path ]в [%-*@3 time].&]
[s3;%RU-RU &]
[s4; &]
[s5;:TimeToFileTime`(Time`): [_^FileTime^ FileTime]_[* TimeToFileTime]([_^Time^ Time]_[*@3 ti
me])&]
[s2;%RU-RU Преобразует Time в FileTime.&]
[s3;%RU-RU &]
[s4; &]
[s5;:FileCopy`(const char`*`,const char`*`): [@(0.0.255) bool]_[* FileCopy]([@(0.0.255) con
st]_[@(0.0.255) char]_`*[*@3 oldpath], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 newpath])
&]
[s2;%RU-RU Копирует файл по пути [%-*@3 oldpath] 
в новый файл по пути [%-*@3 newpath] (если 
он уже существует, то переписывается). 
Возвращает true при успехе.&]
[s3;%RU-RU &]
[s4; &]
[s5;:FileMove`(const char`*`,const char`*`): [@(0.0.255) bool]_[* FileMove]([@(0.0.255) con
st]_[@(0.0.255) char]_`*[*@3 oldpath], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 newpath])
&]
[s2;%RU-RU Перемещает файл по пути [%-*@3 oldpath] 
в новый файл по пути [%-*@3 newpath] (если 
он уже существует, то переписывается). 
Возвращает true при успехе. Работает 
также и над директориями.&]
[s3;%RU-RU &]
[s4; &]
[s5;:FileDelete`(const char`*`): [@(0.0.255) bool]_[* FileDelete]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 path])&]
[s2;%RU-RU Удаляет файл по пути [%-*@3 path]. Возвращает 
true при успехе.&]
[s3;%RU-RU &]
[s4; &]
[s5;:DirectoryCreate`(const char`*`,int`): [@(0.0.255) bool]_[* DirectoryCreate]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 path], [@(0.0.255) int]_[*@3 mode]_`=_[@3 0755])&]
[s6; `[POSIX`]&]
[s2;%RU-RU Создаёт новую директорию по пути 
[%-*@3 path] с режимом доступа [%-*@3 mode].&]
[s3;%RU-RU &]
[s4; &]
[s5;:RealizeDirectory`(const String`&`,int`): [@(0.0.255) bool]_[* RealizeDirectory]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[*@3 path], [@(0.0.255) int]_[*@3 mode]_`=_[@3 0755])
&]
[s6; `[POSIX`]&]
[s2;%RU-RU Создаёт все недостающие директории 
по пути [%-*@3 path] с режимом доступа [%-*@3 mode]. 
Помните, что путь [%-*@3 path] `- это путь 
к файлу. Для реализации пути к папке, 
он должен оканчиваться на `'`\`\`'.&]
[s3;%RU-RU &]
[s4; &]
[s5;:RealizePath`(const String`&`,int`): [@(0.0.255) bool]_[* RealizePath]([@(0.0.255) cons
t]_[_^String^ String][@(0.0.255) `&]_[*@3 path], [@(0.0.255) int]_[*@3 mode]_`=_[@3 0755])&]
[s6; `[POSIX`]&]
[s2;%RU-RU То же, что и RealizeDirectory(GetFileFolder([%-*@3 path]), 
[%-*@3 mode]) `- создаёт все требуемые директории, 
прежде чем записывать файл в [%-*@3 path. 
]Помните, что [%-*@3 path] `- это путь к файлу. 
Чтобы реализовать путь в папке, у 
него должно быть окончание `'`\`\`'.&]
[s3;%RU-RU &]
[s4; &]
[s5;:DirectoryCreate`(const char`*`): [@(0.0.255) bool]_[* DirectoryCreate]([@(0.0.255) con
st]_[@(0.0.255) char]_`*[*@3 path])&]
[s6; `[Win32`]&]
[s2;%RU-RU Создаёт новую директорию.&]
[s3;%RU-RU &]
[s4; &]
[s5;:RealizeDirectory`(const String`&`): [@(0.0.255) bool]_[* RealizeDirectory]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[*@3 path])&]
[s6; `[Win32`]&]
[s2;%RU-RU Создаёт все отсутствующие директории 
на пути [%-*@3 path].Помните, что [%-*@3 path] `- 
это путь к файлу. Чтобы реализовать 
путь в папке, у него должно быть окончание 
`'`\`\`'.&]
[s3;%RU-RU &]
[s4; &]
[s5;:RealizePath`(const String`&`): [@(0.0.255) bool]_[* RealizePath]([@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 path])&]
[s6; `[Win32`]&]
[s2;%RU-RU То же, что и RealizeDirectory(GetFileFolder([%-*@3 path])) 
`-  создаёт все требуемые директории, 
прежде чем записывать файл в [%-*@3 path]. 
Помните, что [%-*@3 path] `- это путь к файлу. 
Чтобы реализовать путь в папке, у 
него должно быть окончание `'`\`\`'.&]
[s3;%RU-RU &]
[s4; &]
[s5;:DirectoryDelete`(const char`*`): [@(0.0.255) bool]_[* DirectoryDelete]([@(0.0.255) con
st]_[@(0.0.255) char]_`*[*@3 path])&]
[s2;%RU-RU Удаляет директорию по пути [%-*@3 path]. 
Возвращает true при успехе.&]
[s3;%RU-RU &]
[s4; &]
[s5;:NormalizePath`(const char`*`,const char`*`): [_^String^ String]_[* NormalizePath]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 path], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 currdir])
&]
[s2;%RU-RU Преобразует путь [%-*@3 path] в нормализованны
й формат. Если путь [%-*@3 path] относительный, 
он преобразуется в абсолютный, используя 
[%-*@3 currdir] как точку ссылки. Все сегменты 
пути `"..`" и `".`" разрешаются и удаляются 
из итога.&]
[s3;%RU-RU &]
[s4; &]
[s5;:NormalizePath`(const char`*`): [_^String^ String]_[* NormalizePath]([@(0.0.255) const]_
[@(0.0.255) char]_`*[*@3 path])&]
[s2;%RU-RU То же, что и NormalizePath([%-*@3 path]., GetCurrentDirectory()).&]
[s3;%RU-RU &]
[s4; &]
[s5;:PathIsEqual`(const char`*`,const char`*`): [@(0.0.255) bool]_[* PathIsEqual]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 p1], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 p2])&]
[s2;%RU-RU Возвращает true, если [%-*@3 p1] и [%-*@3 p2] 
идентифицируют одтн и тот же файл 
или директорию. Оба пути могут быть 
относительными (за отправную точку 
принимается текущая директория) и 
могут содержать сегменты `".`" или `"..`".&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:GetCurrentDirectoryA`(`): [_^Upp`:`:String^ String]_[* GetCurrentDirectory]()
&]
[s2;%RU-RU Возвращает текущую рабочую директорию.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:ChangeCurrentDirectory`(const char`*`): [@(0.0.255) bool]_[* ChangeCurrentDir
ectory]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 path])&]
[s2;%RU-RU Изменяет текущую рабочую директорию 
на [%-*@3 path].&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:DeleteFolderDeep`(const char`*`,bool`): [@(0.0.255) bool]_[* DeleteFolderDeep
]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 dir], [@(0.0.255) bool]_[*@3 rdonly]_`=_[@(0.0.255) f
alse])&]
[s2;%RU-RU Удаляет директорию [%-*@3 dir] и всё, 
что в ней находится. Возвращает true 
при успехе. Если [%-*@3 rdonly] равно true, папка 
удаляется, несмотря ни на какие настройки 
read`-only (покуда процессу позволено 
их удалять).&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:FindAllPaths`(const Upp`:`:String`&`,const char`*`,Upp`:`:dword`): [_^Upp`:`:Vector^ V
ector]<[_^Upp`:`:String^ String]>_[* FindAllPaths]([@(0.0.255) const]_[_^Upp`:`:String^ S
tring][@(0.0.255) `&]_[*@3 dir], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 patterns]_`=_`"
`*`", [_^Upp`:`:dword^ dword]_[*@3 opt]_`=_FINDALLFILES)&]
[s2;%RU-RU Возвращает Vector всех путей в директории 
[%-*@3 dir] и её поддиректориях, совпадающих 
с одним из образцов [%-*@3 patterns] (см. PatternMatchMulti) 
и являющихся либо файлом, либо папкой 
(`".`" и `"..`" исключены), основываясь 
на флагах [%-*@3 opt] (FINDALLFILES, FINDALLFOLDERS).&]
[s3;%RU-RU &]
[s4; &]
[s5;:GetTempPath`(`): [_^String^ String]_[* GetTempPath]()&]
[s2;%RU-RU Возвращает путь к директории, 
пригодной для хранения временных 
файлов.&]
[s3; &]
[s4; &]
[s5;:GetTempFileName`(const char`*`): [_^String^ String]_[* GetTempFileName]([@(0.0.255) co
nst]_[@(0.0.255) char]_`*[*@3 prefix]_`=_NULL)&]
[s2;%RU-RU Возвращает уникальный путь временного 
файла. Если [%-*@3 prefix ]не равен NULL, он 
приставляется к имени файла.&]
[s3;%RU-RU &]
[s0; ]]