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
[s9;%RU-RU FileTime is a structure that encapsulates host specific 
type used for file or directory timestamps.&]
[s9;%RU-RU It has defined ordering and can be compared to Time. Time 
has conversion constructor from FileTime and AsFileTime method 
for backwards conversion.&]
[s3;%RU-RU &]
[s0;%RU-RU &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Функций]]}}&]
[s3; &]
[s5;:ОбразецMatch`(const char`*`,const char`*`): [@(0.0.255) bool]_[* ОбразецMatch]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 p], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s])&]
[s2; [%RU-RU Matches filename pattern ][*@3 p][%RU-RU  with filename ][*@3 s][%RU-RU . 
Образец can use wildcard characters `'][%RU-RU@(0.0.255) ?][%RU-RU `', 
matching any single character or `'][%RU-RU@(0.0.255) `*][%RU-RU `' 
matching any number of any characters (these wildcards are consistent 
with unix/windows). If pattern ends with `'][%RU-RU@(0.0.255) .][%RU-RU `', 
there must be no `'.`' in ][*@3 s] (filename cannot have extension).&]
[s3;%RU-RU &]
[s4; &]
[s5;:ОбразецMatchMulti`(const char`*`,const char`*`): [@(0.0.255) bool]_[* ОбразецMatchMu
lti]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 p], [@(0.0.255) const]_[@(0.0.255) char]_`*
[*@3 s])&]
[s2;%RU-RU Matches multiple patterns separated by semicolon (`';`') 
in [%-*@3 p] with filename [%-*@3 s], using [^topic`:`/`/Core`/src`/Path`$en`-us`#ОбразецMatch`(const char`*`,const char`*`)^ P
atternMatch].&]
[s3;%RU-RU &]
[s4; &]
[s5;:GetFileNamePos`(const char`*`): [@(0.0.255) const]_[@(0.0.255) char]_`*[* GetFileNameP
os]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 path])&]
[s2;%RU-RU Возвращает the pointer to the first character of filename 
in [%-*@3 path] `- the character after the last path separator 
(`'/`' in POSIX, `'`\`\`' or `'/`' in Win32). If there is no 
such character, returns [%-*@3 path].&]
[s3;%RU-RU &]
[s4; &]
[s5;:GetFileExtPos`(const char`*`): [@(0.0.255) const]_[@(0.0.255) char]_`*[* GetFileExtPos
]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 path])&]
[s2;%RU-RU If filename in [%-*@3 path ]has any extension (in fact, 
contains one or more `'.`' charactera), returns the pointer to 
the last `'.`', otherwise returns pointer to terminating `'`\0`'.&]
[s3;%RU-RU &]
[s4; &]
[s5;:HasFileExt`(const char`*`): [@(0.0.255) bool]_[* HasFileExt]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 path])&]
[s2;%RU-RU Возвращает  true, если [%-*@3 path] ends with 
filename with extension (in other words, if filename contains 
`'.`').&]
[s3;%RU-RU &]
[s4; &]
[s5;:HasWildcards`(const char`*`): [@(0.0.255) bool]_[* HasWildcards]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 path])&]
[s2;%RU-RU Возвращает  true, если [%-*@3 path] содержит 
символы `'`*`' или `'?`' (уалдкарды).&]
[s3;%RU-RU &]
[s4; &]
[s5;:IsFullPath`(const char`*`): [@(0.0.255) bool]_[* IsFullPath]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 path])&]
[s2;%RU-RU Возвращает  true, если [%-*@3 path] абсолютен 
`- начинается с `'/`' на системах POSIX 
или с буквы диска в Win32.&]
[s3;%RU-RU &]
[s4; &]
[s5;:GetFileDirectory`(const char`*`): [_^String^ String]_[* GetFileDirectory]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 path])&]
[s2;%RU-RU Extracts a directory path from [%-*@3 path] `- a substring 
that ends with last path separator (`'/`' in POSIX, `'/`' or 
`'`\`\`' in Win32). Separator [*/ is] included at the end of result. 
If there is none, returns empty string.&]
[s2;%RU-RU Example (POSIX): GetFileDirectory([@3 `"/home/user/test.ext`"]) 
returns [@3 `"/home/user/`"].&]
[s0;%RU-RU &]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:GetFileFolder`(const char`*`): [_^String^ String]_[* GetFileFolder]([@(0.0.255) const]_
[@(0.0.255) char]_`*[*@3 path])&]
[s2;%RU-RU Extracts a directory path from [%-*@3 path] `- a substring 
that ends with last path separator (`'/`' in POSIX, `'/`' or 
`'`\`\`' in Win32). Separator [*/ is not] included at the end of 
result. If there is none, returns empty string.&]
[s2;%RU-RU Example (POSIX): GetFileFolder([@3 `"/home/user/test.ext`"]) 
returns [@3 `"/home/user`"].&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:GetFileTitle`(const char`*`): [_^String^ String]_[* GetFileTitle]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 path])&]
[s2;%RU-RU Gets the name of file in [%-*@3 path] without extension 
(if any).&]
[s2;%RU-RU Example (POSIX): GetFileTitle([@3 `"/home/user/test.ext`"]) 
returns [@3 `"test`"].&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:GetFileExt`(const char`*`): [_^String^ String]_[* GetFileExt]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 path])&]
[s2;%RU-RU Gets file extension, including `'.`' character. If there 
is no extension, returns empty string.&]
[s2;%RU-RU Example (POSIX): GetFileExt([@3 `"/home/user/test.ext`"]) 
returns [@3 `".ext`"].&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:GetFileName`(const char`*`): [_^String^ String]_[* GetFileName]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 path])&]
[s2;%RU-RU Возвращает the name of file (without directory).&]
[s2;%RU-RU Example (POSIX): GetFileName([@3 `"/home/user/test.ext`"]) 
returns [@3 `"test.ext`"].&]
[s3;%RU-RU &]
[s4; &]
[s5;:AppendFileName`(const String`&`,const char`*`): [_^String^ String]_[* AppendFileName
]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 path], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 filename])&]
[s2;%RU-RU Appends a [%-*@3 filename] to directory [%-*@3 path]. The 
main purpose of this operation is to maintain correct path separator. 
[%-*@3 path] is allowed but not required to end with separator.&]
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
[s2;%RU-RU If [%-*@3 path] does not have extension, appends [%-*@3 ext]. 
Возвращает result.&]
[s3;%RU-RU &]
[s4; &]
[s5;:ForceExt`(const char`*`,const char`*`): [_^String^ String]_[* ForceExt]([@(0.0.255) co
nst]_[@(0.0.255) char]_`*[*@3 path], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 ext])&]
[s2;%RU-RU Changes file extension of [%-*@3 path] to [%-*@3 ext]. If 
there is no extension, appends it. Возвращает result.&]
[s3;%RU-RU &]
[s4; &]
[s5;:GetFileOnPath`(const char`*`,const char`*`,bool`,const char`*`): [_^String^ String
]_[* GetFileOnPath]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 file], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 paths], [@(0.0.255) bool]_[*@3 current]_`=_[@(0.0.255) t
rue], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 curdir]_`=_NULL)&]
[s2;%RU-RU Attempts to find a [%-*@3 file] on semicolon separated list 
directory [%-*@3 paths]. If [%-*@3 current] is true, [%-*@3 curdir].is 
added to this list. If [%-*@3 curdir] is NULL, the real current 
directory (obtained using GetCurrentDirectory) is used instead. 
Возвращает full path of file, if found, otherwise Null.&]
[s3;%RU-RU &]
[s4; &]
[s5;:GetCurrentDirectory`(`): [_^String^ String]_[* GetCurrentDirectory]()&]
[s2;%RU-RU Возвращает the current directory.&]
[s3; &]
[s4; &]
[s5;:GetFileLength`(const char`*`): [_^int64^ int64]_[* GetFileLength]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 path])&]
[s2;%RU-RU Возвращает the length of file at [%-*@3 path ]or negative number 
if file does not exist.&]
[s3;%RU-RU &]
[s4; &]
[s5;:FileExists`(const char`*`): [@(0.0.255) bool]_[* FileExists]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 path])&]
[s2;%RU-RU Возвращает  true, если [%-*@3 path] представляет 
файл.&]
[s3;%RU-RU &]
[s4; &]
[s5;:DirectoryExists`(const char`*`): [@(0.0.255) bool]_[* DirectoryExists]([@(0.0.255) con
st]_[@(0.0.255) char]_`*[*@3 path])&]
[s2;%RU-RU Возвращает  true, если [%-*@3 path] представляет 
директорию.&]
[s3;%RU-RU &]
[s4; &]
[s5;:GetFileTime`(const char`*`): [_^FileTime^ FileTime]_[* GetFileTime]([@(0.0.255) const]_
[@(0.0.255) char]_`*[*@3 path])&]
[s2;%RU-RU Возвращает the last write time of file at [%-*@3 path]. If 
item does not exist, result is set to zero (using memset, as 
FileTime is structure in Win32).&]
[s3;%RU-RU &]
[s4; &]
[s5;:FileGetTime`(const char`*`): [_^Time^ Time]_[* FileGetTime]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 path])&]
[s2;%RU-RU Возвращает the last write time of file at [%-*@3 path] or Null 
if file does not exist.&]
[s3;%RU-RU &]
[s4; &]
[s5;:SetFileTime`(const char`*`,FileTime`): [@(0.0.255) bool]_[* SetFileTime]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 path], [_^FileTime^ FileTime]_[*@3 ft])&]
[s2;%RU-RU Устанавливаетthe last write time of file at [%-*@3 path ]to [%-*@3 ft].&]
[s3;%RU-RU &]
[s4; &]
[s5;:FileSetTime`(const char`*`,Time`): [@(0.0.255) bool]_[* FileSetTime]([@(0.0.255) const
]_[@(0.0.255) char]_`*[*@3 path], [_^Time^ Time]_[*@3 time])&]
[s2;%RU-RU Устанавливаетthe last write time of file at [%-*@3 path ]to [%-*@3 time].&]
[s3;%RU-RU &]
[s4; &]
[s5;:TimeToFileTime`(Time`): [_^FileTime^ FileTime]_[* TimeToFileTime]([_^Time^ Time]_[*@3 ti
me])&]
[s2;%RU-RU Converts Time to FileTime.&]
[s3;%RU-RU &]
[s4; &]
[s5;:FileCopy`(const char`*`,const char`*`): [@(0.0.255) bool]_[* FileCopy]([@(0.0.255) con
st]_[@(0.0.255) char]_`*[*@3 oldpath], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 newpath])
&]
[s2;%RU-RU Copies a file at [%-*@3 oldpath] to a new file at [%-*@3 newpath] 
(if already exists, overwrites it). Возвращает true on success.&]
[s3;%RU-RU &]
[s4; &]
[s5;:FileMove`(const char`*`,const char`*`): [@(0.0.255) bool]_[* FileMove]([@(0.0.255) con
st]_[@(0.0.255) char]_`*[*@3 oldpath], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 newpath])
&]
[s2;%RU-RU Moves a file at [%-*@3 oldpath] to a new file at [%-*@3 newpath] 
(if already exists, overwrites it). Возвращает true on success. 
Works on directories too.&]
[s3;%RU-RU &]
[s4; &]
[s5;:FileDelete`(const char`*`): [@(0.0.255) bool]_[* FileDelete]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 path])&]
[s2;%RU-RU Deletes file at [%-*@3 path]. Возвращает true on success.&]
[s3;%RU-RU &]
[s4; &]
[s5;:DirectoryCreate`(const char`*`,int`): [@(0.0.255) bool]_[* DirectoryCreate]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 path], [@(0.0.255) int]_[*@3 mode]_`=_[@3 0755])&]
[s6; `[POSIX`]&]
[s2;%RU-RU Создаёт new directory at [%-*@3 path] with access [%-*@3 mode].&]
[s3;%RU-RU &]
[s4; &]
[s5;:RealizeDirectory`(const String`&`,int`): [@(0.0.255) bool]_[* RealizeDirectory]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[*@3 path], [@(0.0.255) int]_[*@3 mode]_`=_[@3 0755])
&]
[s6; `[POSIX`]&]
[s2;%RU-RU Создаёт all missing directories on [%-*@3 path] with access 
[%-*@3 mode].Remember that [%-*@3 path] is a file path. To realize 
a folder path, it has be ended with a `'`\`\`'.&]
[s3;%RU-RU &]
[s4; &]
[s5;:RealizePath`(const String`&`,int`): [@(0.0.255) bool]_[* RealizePath]([@(0.0.255) cons
t]_[_^String^ String][@(0.0.255) `&]_[*@3 path], [@(0.0.255) int]_[*@3 mode]_`=_[@3 0755])&]
[s6; `[POSIX`]&]
[s2;%RU-RU То же, что и RealizeDirectory(GetFileFolder([%-*@3 path]), 
[%-*@3 mode]) `- creates all directories required before writing 
file at [%-*@3 path.]Remember that [%-*@3 path] is a file path. To 
realize a folder path, it has be ended with a `'`\`\`'.&]
[s3;%RU-RU &]
[s4; &]
[s5;:DirectoryCreate`(const char`*`): [@(0.0.255) bool]_[* DirectoryCreate]([@(0.0.255) con
st]_[@(0.0.255) char]_`*[*@3 path])&]
[s6; `[Win32`]&]
[s2;%RU-RU Создаёт new directory.&]
[s3;%RU-RU &]
[s4; &]
[s5;:RealizeDirectory`(const String`&`): [@(0.0.255) bool]_[* RealizeDirectory]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[*@3 path])&]
[s6; `[Win32`]&]
[s2;%RU-RU Создаёт all missing directories on [%-*@3 path].Remember 
that [%-*@3 path] is a file path. To realize a folder path, it 
has be ended with a `'`\`\`'.&]
[s3;%RU-RU &]
[s4; &]
[s5;:RealizePath`(const String`&`): [@(0.0.255) bool]_[* RealizePath]([@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 path])&]
[s6; `[Win32`]&]
[s2;%RU-RU То же, что и RealizeDirectory(GetFileFolder([%-*@3 path])) 
`- creates all directories required before writing file at [%-*@3 path.] 
Remember that [%-*@3 path] is a file path. To realize a folder 
path, it has be ended with a `'`\`\`'.&]
[s3;%RU-RU &]
[s4; &]
[s5;:DirectoryDelete`(const char`*`): [@(0.0.255) bool]_[* DirectoryDelete]([@(0.0.255) con
st]_[@(0.0.255) char]_`*[*@3 path])&]
[s2;%RU-RU Deletes a directory at [%-*@3 path]. Возвращает true on success.&]
[s3;%RU-RU &]
[s4; &]
[s5;:NormalizePath`(const char`*`,const char`*`): [_^String^ String]_[* NormalizePath]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 path], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 currdir])
&]
[s2;%RU-RU Converts [%-*@3 path] to normalized format. If [%-*@3 path] 
is relative, it is converted to absolute using [%-*@3 currdir] 
as reference point. All `"..`" and `".`" segments of path are 
resolved and removed from the result.&]
[s3;%RU-RU &]
[s4; &]
[s5;:NormalizePath`(const char`*`): [_^String^ String]_[* NormalizePath]([@(0.0.255) const]_
[@(0.0.255) char]_`*[*@3 path])&]
[s2;%RU-RU То же, что и NormalizePath([%-*@3 path]., GetCurrentDirectory()).&]
[s3;%RU-RU &]
[s4; &]
[s5;:PathIsEqual`(const char`*`,const char`*`): [@(0.0.255) bool]_[* PathIsEqual]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 p1], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 p2])&]
[s2;%RU-RU Возвращает  true, если [%-*@3 p1] and [%-*@3 p2] 
identify the same file or directory. Both paths can be relative 
(current directory is then used as reference point) and can contain 
segments `".`" or `"..`".&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:GetCurrentDirectoryA`(`): [_^Upp`:`:String^ String]_[* GetCurrentDirectory]()
&]
[s2;%RU-RU Возвращает current current working directory.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:ChangeCurrentDirectory`(const char`*`): [@(0.0.255) bool]_[* ChangeCurrentDir
ectory]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 path])&]
[s2;%RU-RU Changes current working directory to [%-*@3 path].&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:DeleteFolderDeep`(const char`*`,bool`): [@(0.0.255) bool]_[* DeleteFolderDeep
]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 dir], [@(0.0.255) bool]_[*@3 rdonly]_`=_[@(0.0.255) f
alse])&]
[s2;%RU-RU Removes [%-*@3 dir] and everything it contains. Возвращает 
true on success. If [%-*@3 rdonly] is true, folder is deleted regardless 
of any read`-only settings (as long as process is allowed to 
remove them).&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:FindAllPaths`(const Upp`:`:String`&`,const char`*`,Upp`:`:dword`): [_^Upp`:`:Vector^ V
ector]<[_^Upp`:`:String^ String]>_[* FindAllPaths]([@(0.0.255) const]_[_^Upp`:`:String^ S
tring][@(0.0.255) `&]_[*@3 dir], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 patterns]_`=_`"
`*`", [_^Upp`:`:dword^ dword]_[*@3 opt]_`=_FINDALLFILES)&]
[s2;%RU-RU Возвращает a Vector of all paths in [%-*@3 dir] and its sub`-directories 
that match one of [%-*@3 patterns] (see ОбразецMatchMulti) and 
are either file or folder (`".`" and `"..`" excluded) based on 
[%-*@3 opt] flags (FINDALLFILES, FINDALLFOLDERS).&]
[s3;%RU-RU &]
[s4; &]
[s5;:GetTempPath`(`): [_^String^ String]_[* GetTempPath]()&]
[s2;%RU-RU Возвращает a path to directory suitable for storing temporary 
files.&]
[s3; &]
[s4; &]
[s5;:GetTempFileName`(const char`*`): [_^String^ String]_[* GetTempFileName]([@(0.0.255) co
nst]_[@(0.0.255) char]_`*[*@3 prefix]_`=_NULL)&]
[s2;%RU-RU Возвращает a unique path of temporary file. If [%-*@3 prefix 
]is not NULL, it is prepended to the filename.&]
[s3;%RU-RU &]
[s0; ]]