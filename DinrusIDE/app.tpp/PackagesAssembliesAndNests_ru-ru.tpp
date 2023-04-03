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
[s6; [^topic`:`/`/DinrusIDE`/app`/PackagesAssembliesAndNests`_ru`-ru`#1^ 1. 
Пакеты]&]
[s6; [^topic`:`/`/DinrusIDE`/app`/PackagesAssembliesAndNests`_ru`-ru`#2^ 2. 
Сборки]&]
[s6; [^topic`:`/`/DinrusIDE`/app`/PackagesAssembliesAndNests`_ru`-ru`#4^ 3. 
Гнёзда]&]
[s6; [^topic`:`/`/DinrusIDE`/app`/PackagesAssembliesAndNests`_ru`-ru`#5^ 4. 
Сборка U`+`+ `"examples`"]&]
[s6; [^topic`:`/`/DinrusIDE`/app`/PackagesAssembliesAndNests`_ru`-ru`#6^ 5. 
Пути включений и директивы #include]&]
[s6; [^topic`:`/`/DinrusIDE`/app`/PackagesAssembliesAndNests`_ru`-ru`#7^ 6. 
Местоположение пакетов]&]
[s6; [^topic`:`/`/DinrusIDE`/app`/PackagesAssembliesAndNests`_ru`-ru`#8^ 7. 
Имена пакетов/папок/файлов]&]
[s6; [^topic`:`/`/DinrusIDE`/app`/PackagesAssembliesAndNests`_ru`-ru`#9^ 8. 
Альтернативные механизмы путей включения]&]
[s6; [^topic`:`/`/DinrusIDE`/app`/PackagesAssembliesAndNests`_ru`-ru`#10^ 9. 
Включение файла выкладки]&]
[s0; &]
[s6;:1: [@(128.0.255) 1. Пакеты]&]
[s5; Пакеты занимают центральное место 
в U`+`+.  Исполнимое приложение строится 
из пакета.  Пакет также можно построить 
в библиотеку динамической компоновки 
(БДК). Пакет могут использовать другие 
пакеты. Каждый пакет соответствует 
одной директории, а имя директории 
совпадает с именем пакета.  А директории 
пакета располагается файл определения 
пакета (ФОП) (простой текстовый файл 
с расширением .upp), у которого всегда 
одинаковое название с директорией 
пакета.  Файл определения пакета содержит 
список исходников, составляющих пакет, 
плюс информацию о типе пакета, его 
построении и используемых им других 
пакетах.  Исходники пакета, как правило, 
находятся в директории пакета и в 
её поддиректориях, но они могут находится 
в любом удобном расположении. Директория 
пакета обязательно располагается 
в гнезде сборки или в подпапках гнезда 
(см. ниже[/ )].&]
[s5; Файл определения пакета поддерживается 
DinrusIDE, им практически не приходится 
манипулировать вручную.  ФОП обновляется 
DinrusIDE по разным случаям, среди которых 
:&]
[s3;l288;i150;O0;~~~160; Добавление/удаление вами 
исходников в/из пакета&]
[s3;l288;i150;O0;~~~160; Добавление/удаление вами 
пакетов, используемых пакетом&]
[s3;l288;i150;O0;~~~160; Изменение вами настроен 
построения пакета через [/ менеджер 
пакетов]&]
[s3;l288;i150;O0;~~~160; Изменение вами конфигурации 
построения пакета через [/ конфигурацию 
главного пакета]&]
[s3;l288;i150;O0;~~~160; Когда вы меняете/назначаете 
кастомные шаги построения для пакета&]
[s5; Более подробно о конфигурировании 
пакетов [^topic`:`/`/DinrusIDE`/app`/ConfiguringPackagesAssemblies`_ru`-ru^ о
писано здесь].&]
[s5; Когда пакет строится, для каждого 
исходника из пакета вызывается компилятор, 
и `- прямо или косвенно `- для всех пакетов, 
используемых  [* главным пакетом.]&]
[s6;:2: [I2;@(128.0.255) 2. Сборки]&]
[s5; О сборке можно думать как о коллекции 
пакетов, но на самом деле это лишь 
набор путей, определяющих места поиска 
U`+`+ пакетов, нужных для построения 
пакета (или приложения). Пути сборки 
также определяют место поиска компилятором 
файлов, указанных в директивах C`+`+ 
#include. Также сборка определяет корневую 
папку для размещения выходных файлов 
(.obj, .exe и т.д.) , производимых при построении, 
а также расположение общих файлов. 
 Определённые в сборке пути хранятся 
в файле определения сборки (ФОС), с 
расширением .var, находящемся в корневой 
директории установки U`+`+. Пакет может 
ассоциироваться с несколькими сборками. 
 Файлы, производимые при построении, 
на деле помещаются в подпапку корневой 
папки вывода, а эта подпапка именуется 
в соответствии с именем пакета, [* флагами 
построения] и используемым компилятором, 
напр., для примера HelloWorld при построении 
компилятором MSVC`+`+ 7.1, выводная папка 
как правило будет [/ C:/upp/out/HelloWorld/MSC71.Gui.Main, 
где] [/ C:/upp/out] указан как корень папки 
вывода в сборке.&]
[s6;:4:~~~128; [I3;@(128.0.255) 3. Гнёзда]&]
[s5; U`+`+ требует организации пакетов по 
гнёздам.  В реалии гнездо `- лишь директория, 
содержащая набор пакетных директорий 
и исходников.  Сборка определяет упорядоченн
ый список гнёзд (путей), а пакеты, находящиеся
 в них, формируют пакеты сборки.  Пакеты, 
связанные со сборкой, показаны в правой 
панели диалогового окна `"[/ Выбор главного 
пакета`"] , а имя сборки подсвечено 
в левой панели. Пути гнёзд, указанные 
в сборке, могут также использоваться 
для установки дополнительных путей 
включения компилятора. Подробности 
смотрите ниже в [^topic`:`/`/DinrusIDE`/app`/PackagesAssembliesAndNests`_ru`-ru`#6^  
Пути включений и директивы #include]&]
[s5; Сборка, в которой находятся все пакеты 
библиотеки U`+`+ исконно называется 
uppsrc, в DinrusIDE она переименована в src, 
и добавлена директория Projects, заменяющая 
собой MyApps.&]
[s6;:5: [I4;@(128.0.255) 4. Сборка U`+`+ `"examples`"]&]
[s5; Это сборка, в которой содержатся 
все примеры программ U`+`+ (такие как 
HelloWorld), она называется `"examples`". Её можно 
найти в диалоговом окне выбора главного 
пакета. У неё есть два гнезда, `"examples`" 
и `"uppsrc`".  На платформе Windows  путь для 
сборки гнёзд с примерами может выглядеть 
так :&]
[s5; C:/upp/examples;C:/upp/uppsrc&]
[s5; где C:`\upp `- корневая директория установки 
U`+`+. Точка с запятой служит для разделения 
настроек путей к гнёздам  Гнездо examples 
 содержит все пакеты с примерами U`+`+ 
и гнездо uppsrc, в котором находятся 
все ключевые пакеты U`+`+.&]
[s5; Для портируемости, [*_@(170.42.150) следует 
использовать прямые слэши при любых 
определениях путей] , а не обратные. 
Это также касается и деректив #include.&]
[s6;:6: [I5;@(128.0.255) 5. Пути включений и дерективы 
#include]&]
[s5; Гнездовые пути, определённые в сборке, 
определяют места поиска для U`+`+ пакетов 
сборки. Эти пути также добавляются 
в `"список путей включений`" для компилирован
ия исходников через команду компилятора 
`"`-I`" (или её эквивалентную). Например, 
для сборки examples настройка командной 
строки `-I (для GCC) будет &]
[s5; `-IC:/upp/examples `-IC:/upp/uppsrc&]
[s5; Это означает, что имена путей, используемых 
в директивах #include в исходниках на 
C`+`+, могут начинаться с названия папки/пакета
, являющегося членом гнезда сборки. 
Например, в примере HelloWorld, в файле 
hello.cpp, можно найти строку&]
[s7; #include <CtrlLib/CtrlLib.h>&]
[s5; CtrlLib `- это название пакета библиотеки 
U`+`+ в гнезде uppsrc. Угловые скобки используются
 в директивах #include при включениии 
исходников U`+`+, так как этим компилятор 
получает инструкцию не искать паку 
с названием. `"CtrlLib`" в текущей директории. 
 т.е. для всех поддерживаемых U`+`+ компиляторо
в, угловые скобки обозначают поиск 
файлов`-включений в путях, указанных 
в директиве `-I.  Когда включается файл, 
находящийся в той же папке, что и `"вызывающий
`" его,(либо в подпапках) , тогда используются 
двойные кавычки, вместо угловых скобок.&]
[s5; На платформе не`-Windows, имена папок 
отличаются в зависимости от регистра 
букв, поэтому [*_@(141.42.150) рекомендуется 
всегда использовать правильный регистр] 
, например,. CtrlLib, а не ctrllib.&]
[s5; Смотрите далее допинфо в [^topic`:`/`/DinrusIDE`/app`/PackagesAssembliesAndNests`_ru`-ru`#9^ А
льтернативные механизмы путей включения].&]
[s6;:7: [I6;@(128.0.255) 6. Размещения пакетов]&]
[s5; Папку пакета не обязательно размещать 
в гнездовой папке верхнего уровня. 
Она может быть помещена в подпапку 
гнездовой. Например, давайте взглянем 
на следующую структуру директории:&]
[s5; [/ Гнездо1/Пкт1]&]
[s5; [/ Гнездо1/Проект1/Пкт2]&]
[s5; [/ Гнездо1/Проект1/Client/Пкт3]&]
[s5; [/ Гнездо1/Проект1/Common]&]
[s5; и на гнездовой путь сборки&]
[s5; [/ C:/upp/Гнездо1;C:/upp/uppsrc]&]
[s5; Пкт1 расположен на верхнем уровне 
папки Гнездо1. Пкт2 находится в подпапке 
 Проект1  Гнездо1.  U`+`+ ищет пакеты по 
всем подпапкам гнезда, на максимум 
глубины.&]
[s5; Исходник в Пкт3 может #include`'ить исходник 
в Пкт2 (Файл2.h) через&]
[s5; #include <Проект1/Пкт2/Файл2.h>&]
[s5; Можно использовать двойные кавычки 
вместо угловых скобок, если в папке 
Пкт3 нет папки Проект1.&]
[s5; Исходник в Пкт3 может #include`'ить файл 
из его собственной папки (Файл3.h) либо 
через&]
[s7; #include `"File3.h`"&]
[s3; либо через&]
[s7; #include <Project1/Client/Pkg3/File3.h>&]
[s5; Организация пакетов и #include`'ов, показанная 
выше, позволяет поместить папку Проект1 
в любое гнездо, не меняя никаких #include`'ов, 
так как все имена путей начинаются 
с Проект1. Это даёт возможность переключатьс
я в сборке между разными ревизиями 
Проект1 путём простого изменения 
гнездового пути сборки.&]
[s5; Когда пакет создаётся через диалоговое 
окно Создать`-новый`-пакет, в имя пакета 
должна входить какая`-то информация 
о пути, если пакет не является верхнеуровнев
ой папкой внутри гнезда. напр., для 
пакета Проект1/Пкт2, нужно вводить 
имя пакета как  Проект1/Пкт2.  Для Пкт1, 
вводим имя пакета как просто Пкт1, 
так как этот пакет `- на верхнем уровне 
гнезда.&]
[s5; Сборка может содержать несколько 
проектов/приложений, либо только 
один проект. Если в ней несколько 
проектов, то нужно рассмотреть проблему 
имени пакет/папка/файл, описанную 
ниже.&]
[s6;:8: [I7;@(128.0.255) 7. Имена пакета/папки/файла]&]
[s5; Как правило, необходимо, чтобы имена 
папок и файлов в верхнеуровневых 
гнёздах сборки были [*_@(113.42.150) уникальными 
для всех верхнеуровневых папок гнезда 
этой сборки],unless duplicated names are referenced 
using a complete path specification.  This means that if an assembly 
includes the uppsrc nest, then the other nests of that assembly 
must not contain packages or folders that have the same name 
as folders/packages in the uppsrc nest.  e.g. The HelloWorld 
examples nest cannot contain packages with names such as CtrlLib, 
RichEdit or Common because these are the names of uppsrc library 
packages.&]
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
[s6;:9: [I8;@(128.0.255) 8. Альтернативный путевой 
механизм #include]&]
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
[s6;:10: [I9;@(128.0.255) 9.  #includes файла выкладки]&]
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