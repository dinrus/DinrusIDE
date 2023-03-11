topic "Пакеты, Сборки и Гнёзда";
[=b133;*R+184 $$1,2#50732784410233463530219518343157:Heading1]
[a17;*@(170.42.0) $$2,3#48345324464242285505540720043115:Heading2]
[ph2 $$3,3#42410412485283044863419059360682:Enumaration]
[a83;*R6 $$4,5#31310162474203024125188417583966:caption]
[b42;a42;ph2 $$5,5#45413000475342174754091244180557:text]
[H4;b83;*4 $$6,5#07864147445237544204411237157677:title]
[b17;t246;C@5;2 $$7,7#20902679421464641399138805415013:code]
[ $$0,0#00000000000000000000000000000000:Default]
[{_}%RU-RU 
[s4; [@3 Пакеты, Сборки и Гнёзда]&]
[s6; [@3 Содержание]&]
[s0; &]
[s0; [^topic`:`/`/DinrusIDE`/app`/PackagesAssembliesAndNests`_ru`-ru`#1^2 1. 
Пакеты]&]
[s0; [^topic`:`/`/DinrusIDE`/app`/PackagesAssembliesAndNests`_ru`-ru`#2^2 2. 
Сборки]&]
[s0; [^topic`:`/`/DinrusIDE`/app`/PackagesAssembliesAndNests`_ru`-ru`#4^2 3. 
Гнёзда]&]
[s0; [^topic`:`/`/DinrusIDE`/app`/PackagesAssembliesAndNests`_ru`-ru`#5^2 4. 
Сборка U`+`+ examples]&]
[s0; [^topic`:`/`/DinrusIDE`/app`/PackagesAssembliesAndNests`_ru`-ru`#6^2 5. 
Пути включений и директивы #include]&]
[s0; [^topic`:`/`/DinrusIDE`/app`/PackagesAssembliesAndNests`_ru`-ru`#7^2 6. 
Местоположение пакетов]&]
[s0; [^topic`:`/`/DinrusIDE`/app`/PackagesAssembliesAndNests`_ru`-ru`#8^2 7. 
Имена пакетов/папок/файлов]&]
[s0; [^topic`:`/`/DinrusIDE`/app`/PackagesAssembliesAndNests`_ru`-ru`#9^2 8. 
Альтернативные механизмы путей включения]&]
[s0; [^topic`:`/`/DinrusIDE`/app`/PackagesAssembliesAndNests`_ru`-ru`#10^2 9. 
Включение файла выкладки]&]
[s0; &]
[s6;:1: [@(128.0.255) 1. Пакеты]&]
[s5; Пакеты выполняют центральную роль 
в U`+`+.  Исполнимое приложение строится 
из пакета.  Пакет также может строиться 
в библиотеку динамической компоновки. 
 Его может использовать другой пакет. 
 Пакет соответствует единичной директории, 
где имя директории является именем 
пакета. В директории пакета находится 
файл определения пакета (простой 
текстовый файл с расширением .upp), 
у которого всегда такое же имя, как 
и у директории пакета.  В файле определения 
пакета содержится список исходников, 
из которых состоит пакет, плюс информация 
о том, какого типа этот пакет, как 
его следует строить и какие иные пакеты 
он использует.  Исходники пакета, 
как правило, располагаются в директории 
пакета и в её поддиректориях, но могут 
находится и в любом другом удобном 
расположении.  Директория пакета 
должна быть в гнезде сборки или в 
подпапках гнезда (смотрите ниже пункт 
[^topic`:`/`/DinrusIDE`/app`/PackagesAssembliesAndNests`_ru`-ru`#7^ Местопол
ожение пакетов]).&]
[s5; Файл определения пакета обслуживается 
DinrusIDE и вам с ним практически не приходится 
что`-то делать вручную.  Файл определения 
пакета обновляется в тех или иных 
случаях DinrusIDE, включая :&]
[s3;l288;i150;O0;~~~160; Добавление/удаление вами 
исходников в/из пакета&]
[s3;l288;i150;O0;~~~160; Добавление/удаление вами 
пакетов, используемых этим пакетом&]
[s3;l288;i150;O0;~~~160; Изменение настороек построения 
пакета посредством [/ органайзера пакетов]&]
[s3;l288;i150;O0;~~~160; Изменение настроек построения 
пакета с помощью [/ конфигурации гравного 
пакета]&]
[s3;l288;i150;O0;~~~160; Изменение/присвоение 
кастомных этапов построения пакета&]
[s5; Более детально о конфигурировании 
пакетов и сборок описано [^topic`:`/`/DinrusIDE`/app`/ConfiguringPackagesAssemblies`_ru`-ru^ з
десь].&]
[s5; При построении пакета для каждого 
исходника пакета вызывается компилятор, 
также как и для любых исходников прямо 
или косвенно принадлежащих пакету, 
используемому [* главным пакетом.]&]
[s6;:2: [I2;@(128.0.255) 2. Сборки]&]
[s5; Под сборкой можно понимать коллекцию 
пакетов, но на самом деле это просто 
набор путей, которые определяют места 
поиска U`+`+ пакетов, необходимых для 
построения пакета (или приложения). 
 Пути сборки также определяют места 
поиска компилятором файлов, указанных 
в директивах C`+`+ #include.  В сборке также 
определяется корневая папка для размещения 
выходных файлов (.obj, .exe и т.д.), производимых 
при постройке, плюс размещение общих 
файлов. Пути, определённые в сборке, 
помещаются в файл определения сборки, 
у которого расширение .var, и который 
находится в корневой директории установки 
U`+`+. Пакет может ассоциироваться с 
несколькими сборками. Файлы, производимые 
при построении помещаются в подпапке 
корневой папки вывода, эта подпапка 
именуется по имени пакета, [* флагам 
построения] и использованному компилятору, 
например, для HelloWorld, при его построении 
компилятором MSVC`+`+ 7.1, папка вывода 
обычно называется [/ C:/upp/out/HelloWorld/MSC71.Gui.Main, 
]где [/ C:/upp/out] указывается как корневая 
папка вывода в сборке.&]
[s6;:4:~~~128; [I3;@(128.0.255) 3. Гнёзда]&]
[s5; U`+`+ требует организации пакетов по 
гнёздам.  На самом деле гнездо `- это 
просто директория, в которой находится 
ряд директорий пакетов и исходники. 
Сборка определяет упорядоченный 
список гнёзд(пути) и пакетов, которые 
находятся в них и формируют пакеты 
сборки. Пакеты, ассоциированные со 
сборкой, показаны в правой панели 
диалогового окна [/ Открыть главный 
пакет, ]а имя сборки подсвечивается 
в левой панели  Пути гнёзд, заданные 
в сборке, могут использоваться для 
настройки дополнительных путей включений 
компилятора. Подробности ищите ниже 
в [^topic`:`/`/DinrusIDE`/app`/PackagesAssembliesAndNests`_ru`-ru`#6^ Пути 
включений и директивы #include].&]
[s5; Сборка, в которой находятся все пакеты 
библиотек U`+`+, называется src, а имя 
связанного гнезда такое же (сборка 
uppsrc может содержать исходники`-прототипы 
от проекта TheIDE).&]
[s6;:5: [I4;@(128.0.255) 4. Сборка U`+`+ `'examples`']&]
[s5; Сборка, в которой находятся все примеры 
U`+`+ (такие как HelloWorld), называется `"examples`" 
и её можно найти в диалоге выбора 
главного пакета.  У неё два гнезда, 
`"examples`" и `"uppsrc`".  На платформе Windows 
пути установки гнёзд сборки examples 
могут быть такими:&]
[s5; C:/upp/examples;C:/upp/uppsrc&]
[s5; где C:`\upp `- это корневая папка установки 
U`+`+. Двоеточие служит для отделения 
путей каждого из гнёзд. Гнездо examples 
содержит все пакеты с примерами U`+`+, 
а гнездо src содержит все ключевые 
пакеты библиотек U`+`+.&]
[s5; For portability, [*_@(170.42.150) forward slashes should be used 
in all path specifications] rather than back`-slashes.  This 
also applies to #include directives.&]
[s6;:6: [I5;@(128.0.255) 5. Пути включений и директивы 
#include]&]
[s5; The nest paths defined in an assembly determine where U`+`+ 
looks for the assembly`'s packages.  These paths are also added 
to the `"include path list`" for the compilation of source files 
via the `"`-I`" (or equivalent) compiler command line option. 
 e.g. for the examples assembly above, the `-I command line setting 
(for GCC) would be &]
[s5; `-IC:/upp/examples `-IC:/upp/uppsrc&]
[s5; This means that the path names used in #include directives in 
C`+`+ source files can begin with the name of a folder/package 
that is the member of an assembly nest.  e.g. in the HelloWorld 
example hello.cpp file you will see&]
[s7; #include <CtrlLib/CtrlLib.h>&]
[s5; CtrlLib is the name of a U`+`+ library package in the uppsrc 
nest.  Angle brackets should be used in a #include directive 
when #including U`+`+ source files because this prevents the 
compiler from looking in the current directory for a folder named 
e.g. `"CtrlLib`".  i.e. with all U`+`+ supported compilers, the 
angle brackets means the search for the included file begins 
with the paths specified in the `-I directive.  When #including 
files that are members of the same folder (or sub`-folders) as 
the file doing the #include, then double quotes should be used 
instead of angle brackets.&]
[s5; On non`-Windows platforms, folder names are case sensitive so 
[*_@(141.42.150) it is recommended that the correct case always 
be used]  e.g. CtrlLib and not ctrllib.&]
[s5; See [/ Alternative`-#include`-path`-mechanisms] below for additional 
information.&]
[s6;:7: [I6;@(128.0.255) 6. Размещения пакетов]&]
[s5; A package folder does not have to be located in a top level 
nest folder.  It may be located in a sub`-folder of a nest folder 
if desired.  For example, consider the following directory structure.&]
[s5; [/ Nest1/Pkg1]&]
[s5; [/ Nest1/Project1/Pkg2]&]
[s5; [/ Nest1/Project1/Client/Pkg3]&]
[s5; [/ Nest1/Project1/Common]&]
[s5; and an assembly nest path setting of&]
[s5; [/ C:/upp/Nest1;C:/upp/uppsrc]&]
[s5; Pkg1 is located in the top level Nest1 folder. Pkg2 is located 
in the Project1 sub`-folder of Nest1.  U`+`+ searches all of 
the sub`-folders of a nest to maximum depth when looking for 
packages.&]
[s5; A source file in Pkg3 can #include a source file in Pkg2 (File2.h) 
with&]
[s7; #include <Project1/Pkg2/File2.h>&]
[s5; Double quotes can be used instead of angle brackets providing 
the Pkg3 folder does not contain a Project1 folder.&]
[s5; A source file in Pkg3 can #include a file from its own folder 
(File3.h) with either&]
[s7; #include `"File3.h`"&]
[s3; or&]
[s7; #include <Project1/Client/Pkg3/File3.h>&]
[s5; The organization of packages and #includes shown above allows 
the Project1 folder to be located in any nest.without changing 
any of the #includes.because all of the pathnames begin with 
Project1.  It also allows an assembly to switch between different 
revisions of Project1 just by changing the assembly nest path 
setting.&]
[s5; When a package is created using the Create`-new`-package dialog, 
the package name must include some path information if the package 
is not a top level folder within a nest. e.g. for the Project1/Pkg2 
package above, the package name needs to be entered as Project1/Pkg2. 
 For Pkg1, the package name can be entered as just Pkg1 because 
the package is in a top level nest folder.&]
[s5; An assembly may contain multiple projects/applications or just 
one project.  If there are multiple projects in an assembly then 
you need to consider the package/folder/file name issue described 
below.&]
[s6;:8: [I7;@(128.0.255) 7. Имена пакета/папки/файла]&]
[s5; It is usually necessary that the names of the folders and files 
that appear in the top level nest folders of an assembly be [*_@(113.42.150) unique
 across all the top level nest folders of that assembly] unless 
duplicated names are referenced using a complete path specification. 
 This means that if an assembly includes the uppsrc nest, then 
the other nests of that assembly must not contain packages or 
folders that have the same name as folders/packages in the uppsrc 
nest.  e.g. The HelloWorld examples nest cannot contain packages 
with names such as CtrlLib, RichEdit or Common because these 
are the names of uppsrc library packages.&]
[s5; Hence the names of uppsrc packages need to be avoided when choosing 
names for folders/packages that are located in a top level nest 
folder if the assembly includes the uppsrc nest.  Refer to the 
uppsrc folder in the U`+`+ installation path for the full list 
of uppsrc package/folder names.  The names of uppsrc packages 
need to be avoided as folder names within any folder that is 
specified as an include path to the compiler (see [/ alternative`-include`-path`-me
chanisms] below).&]
[s5; If a package is to be distributed to others, one way of avoiding 
a clash of package names is to locate them in a folder whose 
name is likely to be unique e.g. &]
[s5; Nest1/CZ1Soft/Pkg1.  &]
[s5; and&]
[s7; #include <CZ1Soft/Pkg1/File1.h>&]
[s5; CZ1Soft is a name that has a reasonable chance of being unique. 
 The name of the Pkg1 folder can be anything because it is not 
a top level nest folder and.is not specified as an include path 
to the compiler.&]
[s5; If source files are placed directly in a nest folder (e.g. Nest1 
above), then the names need to be unique across all nest folders 
of the assembly unless they are always accessed with either a 
complete path specification or with no path specification (in 
which case they need to be in the same folder as the including 
file). e.g.&]
[s3;l160;i96;O0; #include `"File1.h`" contains no path specification.&]
[s3;l160;i96;O0; #include `"../Pkg2/File2.h`" is a complete path 
specification.&]
[s3;l160;i96;O0; #include <Project1/Pkg2/File2.h> is an incomplete 
path specification.&]
[s5; Note `"..`" in a path specification means `"parent folder`" 
i.e. up one level.&]
[s6;:9: [I8;@(128.0.255) 8. Alternative #include path mechanisms]&]
[s5; The nest paths specified in an assembly are normally used to 
identify the directory (or directory tree) where packages can 
be found and, as explained above, the `"normal`" method of #including 
header files is that when the header file name involves an incomplete 
path, the given pathname begins with the name of a top`-level 
nest folder e.g. #include <CtrlLib/CtrlLib.h>.  CtrlLib is the 
name of a top`-level nest folder because it is located in the 
uppsrc nest.&]
[s5; Because the paths specified in an assembly are added to the 
`"include path list`" for the compiler (using `-I or /I), you 
can use this mechanism to add directories to the include path 
list, even if those directories don`'t contain any packages. 
 You need to remember that the search for packages looks in all 
sub`-folders of the assembly nest paths and also that you may 
need to avoid using folder names that are the names of uppsrc 
packages (see the [/ Package`-folder`-names] section above).  The 
order of the `-I (or /I) directives supplied to the compiler 
is the same as the order of the nest paths specified in the assembly 
and this determines the search order when the compiler looks 
for #include files.&]
[s5; e.g. suppose you have a folder, C:/SomeFolder, that contains 
a header file SomeHeader.h.  You can add SomeFolder to the assembly 
nest path like this&]
[s5; C:/upp/examples;C:/upp/uppsrc;C:/SomeFolder&]
[s3; &]
[s3; In your source files you can now write&]
[s7; #include <SomeHeader.h>&]
[s3; or&]
[s7; #include `"SomeHeader.h`"&]
[s5; Angle brackets are preferred because they mean the search begins 
in paths specified in `-I directives rather than in the folder 
containing the file doing the #include&]
[s5; .Providing the header files in the SomeFolder folder use angle 
brackets when #including uppsrc files, it will not matter if 
the SomeFolder folder contains directories that have the same 
name as uppsrc directories, because C:/SomeFolder is last in the 
assembly nest path list.&]
[s5; You can also use the [/ package`-organizer] in DinrusIDE to specify 
additional include paths.  The [/ package`-organizer] allows you 
to enter additional switches to be passed to the compiler for 
all packages, for specific packages or for specific source files. 
 This allows you to add a `-I switch (or /I) to specify an include 
path.  These switches get added to the compiler command line 
[/ after] the `-I switches for the assembly nest paths.  To see 
how the compiler is invoked when a source file is compiled, turn 
on the [/ verbose] option in the Setup menu in DinrusIDE.  You 
can use [* build flags] to restrict the additional compiler switches 
to be in effect only when your own package`'s source files are 
being compiled.  The [/^topic`:`/`/DinrusIDE`/app`/ConfiguringPackagesAssemblies`$en`-us^ p
ackage`-organizer] also allows you to specify compiler switches 
for specific files.&]
[s6;:10: [I9;@(128.0.255) 9. Layout file #includes]&]
[s5; A layout file contains a description of the GUI part of a project 
i.e. the layout of widgets etc.  e.g. the AddressBook example 
in the examples assembly uses a layout file and the AddressBook.cpp 
file has the following two lines.&]
[s7; #define LAYOUTFILE <AddressBook/AddressBook.lay>&]
[s7; #include <CtrlCore/lay.h>&]
[s5; The #define for LAYOUTFILE should use angle brackets and not 
double quotes and must also include a path specification that 
starts with a top level folder of a nest. i.e. it should not 
be written as&]
[s7; #define LAYOUTFILE `"AddressBook.lay`"&]
[s5; because the CtrlCore/lay.h file uses it to #include the layout 
file (multiple times) with&]
[s7; #include LAYOUTFILE&]
[s5; ]]