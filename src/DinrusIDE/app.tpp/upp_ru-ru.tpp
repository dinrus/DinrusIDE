topic "Формат файла пакета U++";
[l288;i1120;a17;O9;~~~.1408;2 $$1,0#10431211400427159095818037425705:param]
[a83;*R6 $$2,5#31310162474203024125188417583966:caption]
[H4;b83;*4 $$3,5#07864147445237544204411237157677:title]
[i288;O9;C2 $$4,6#40027414424643823182269349404212:item]
[b42;a42;ph2 $$5,5#45413000475342174754091244180557:text]
[l288;b17;a17;2 $$6,6#27521748481378242620020725143825:desc]
[l321;C@5;1 $$7,7#20902679421464641399138805415013:code]
[b2503;2 $$8,0#65142375456100023862071332075487:separator]
[*@(0.0.255)2 $$9,0#83433469410354161042741608181528:base]
[C2 $$10,0#37138531426314131251341829483380:class]
[l288;a17;*1 $$11,11#70004532496200323422659154056402:requirement]
[i417;b42;a42;O9;~~~.416;2 $$12,12#10566046415157235020018451313112:tparam]
[b167;C2 $$13,13#92430459443460461911108080531343:item1]
[i288;a42;O9;C2 $$14,14#77422149456609303542238260500223:item2]
[*@2$(0.128.128)2 $$15,15#34511555403152284025741354420178:NewsDate]
[l321;*C$7;2 $$16,16#03451589433145915344929335295360:result]
[l321;b83;a83;*C$7;2 $$17,17#07531550463529505371228428965313:result`-line]
[l160;*C+117 $$18,5#88603949442205825958800053222425:package`-title]
[2 $$19,0#53580023442335529039900623488521:gap]
[C2 $$20,20#70211524482531209251820423858195:class`-nested]
[b50;2 $$21,21#03324558446220344731010354752573:Par]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_}%RU-RU 
[s2; [@(28.85.200) Формат файла пакета U`+`+]&]
[s3; [@5 Содержание]&]
[s0;@5 &]
[s0; [^topic`:`/`/DinrusIDE`/app`/upp`_ru`-ru`#1^@5 1. Введение]&]
[s0; [^topic`:`/`/DinrusIDE`/app`/upp`_ru`-ru`#2^@5 2. Принимаемые 
флаги]&]
[s0; [^topic`:`/`/DinrusIDE`/app`/upp`_ru`-ru`#3^@5 3. Кодировка]&]
[s0; [^topic`:`/`/DinrusIDE`/app`/upp`_ru`-ru`#4^@5 4. Кастомные 
этапы построения]&]
[s0; [^topic`:`/`/DinrusIDE`/app`/upp`_ru`-ru`#5^@5 5. ][^topic`:`/`/DinrusIDE`/app`/upp`_ru`-ru`#5^@5 О
писание]&]
[s0; [^topic`:`/`/DinrusIDE`/app`/upp`_ru`-ru`#6^@5 6. ][^topic`:`/`/DinrusIDE`/app`/upp`_ru`-ru`#6^@5 Ф
айлы]&]
[s0; [^topic`:`/`/DinrusIDE`/app`/upp`_ru`-ru`#7^@5 7. Add/Remove flags]&]
[s0; [^topic`:`/`/DinrusIDE`/app`/upp`_ru`-ru`#8^@5 8. Additional includes]&]
[s0; [^topic`:`/`/DinrusIDE`/app`/upp`_ru`-ru`#9^@5 9. Libraries]&]
[s0; [^topic`:`/`/DinrusIDE`/app`/upp`_ru`-ru`#10^@5 10. Linking options]&]
[s0; [^topic`:`/`/DinrusIDE`/app`/upp`_ru`-ru`#11^@5 11. Size optimization]&]
[s0; [^topic`:`/`/DinrusIDE`/app`/upp`_ru`-ru`#12^@5 12. Speed optimization]&]
[s0; [^topic`:`/`/DinrusIDE`/app`/upp`_ru`-ru`#13^@5 13. Compiler options]&]
[s0; [^topic`:`/`/DinrusIDE`/app`/upp`_ru`-ru`#14^@5 14. Main configuration]&]
[s0; [^topic`:`/`/DinrusIDE`/app`/upp`_ru`-ru`#15^@5 15. No BLITZ]&]
[s0; [^topic`:`/`/DinrusIDE`/app`/upp`_ru`-ru`#16^@5 16. Target]&]
[s0; [^topic`:`/`/DinrusIDE`/app`/upp`_ru`-ru`#17^@5 17. Used packages]&]
[s0; &]
[s3;:1: [@(128.0.255) 1. Введение]&]
[s5; Вся информация о пакетах сохраняется 
в текстовых файлах с таким же названием, 
как у пакета.и расширением .upp. В этом 
файле сохраняется список входящих 
в пакет файлов, а также все настройки, 
которые были сделаны в Органайзере 
Пакета. В этой статье документируется 
синтаксис файла .upp.&]
[s5; Каждая секция файла .upp начинается 
с ключевого слова и заканчивается 
точкой с запятой. Распознаваемые 
ключевые слова секций следующие:&]
[s0; &]
[ {{2500:2500:2500:2500g2; [s7;l160; [2 acceptflags]&]
[s7;l160; [2 charset]&]
[s7;l160; [2 custom]&]
[s7;l160; [2 description]]
:: [s7;l160; [2 file]&]
[s7;l160; [2 flags]&]
[s7;l160; [2 include]&]
[s7;l160; [2 library]]
:: [s7;l160; [2 link]&]
[s7;l160; [2 optimize`_size]&]
[s7;l160; [2 optimize`_speed]&]
[s7;l160; [2 options]]
:: [s7;l160; [2 mainconfig]&]
[s7;l160; [2 noblitz]&]
[s7;l160; [2 target]&]
[s7;l160; [2 uses]]}}&]
[s0; &]
[s3;:2: [@(128.0.255) 2. Принимаемые флаги]&]
[s5; В каждом пакете можно указать список 
флагов, которые будут использованы 
для обеспечения особой или оптимизированной
 функциональности. Такие флаги затем 
могут передаваться с префиксом точкой 
и будут применяться только к главному 
пакету и всем пакетам, которые включают 
его в свой список среди применимых 
флагов. Это обеспечивает способ уменьшить 
число пакетов,  которые потребуется 
перестроить, если флаги изменяются.&]
[ {{1914:8086g2;h1; [s5; Синтаксис:]
:: [s5; [C@5 acceptflags <имя`_флага> `[,<имяфлага>`]`*;]]
:: [s5; Пример:]
:: [s5; [C@5 acceptflags NOGTK, GUI;]]}}&]
[s5; &]
[s3;:3: [@(128.0.255) 3. Кодировка]&]
[s5; Дефолтную кодировку для всех файлов 
в пакете можно задать с помощью ключевого 
слова [C@5 charset]. &]
[ {{1914:8086f0;g0;h1; [s5; Синтаксис:]
:: [s5; [C@5 charset <имя`_набора`_символов>;]]
:: [s5; Пример:]
:: [s5; [C@5 charset `"UTF`-8`";]]}}&]
[s5; &]
[s3;:4: [@(128.0.255) 4. Кастомные этапы построения]&]
[s5; Для каждого пакета могут вызываться 
так называемые `"кастомные этапы построения`"
, которые происходят либо при компиляции 
всех файлов с данным расширением, 
либо при компоновке, если расширение 
относится к одному из особых значений 
pre`-link или post`-link. Этот этап построения 
состоит из команды выполнить и указания 
выходных файлов, которые далее могут 
обрабатываться DinrusIDE. Например, это 
может быть применено для генерации 
исходников на ходу, либо для применения 
к окончательному исполнимому постпроцессин
говых операций. Команда и вывод могут 
содержать ряд переменных, заключённых 
в `'`$(...)`', которые заменяются при их 
оценке:&]
[ {{2833:7167<448;>640;h3; [s5;b17;a17; [C@5 FILE]]
:: [s5;b17;a17; имя текущего файла]
:: [s5;b17;a17; [C@5 TITLE]]
:: [s5;b17;a17; имя текущего файла без расширения]
:: [s5;b17;a17; [C@5 PATH]]
:: [s5;b17;a17; путь к текущему файлу]
:: [s5;b17;a17; [C@5 RELPATH]]
:: [s5;b17;a17; относительный путь к текущему 
файлу]
:: [s5;b17;a17; [C@5 FILEDIR]]
:: [s5;b17;a17; директория текущего файла]
:: [s5;b17;a17; [C@5 DIR]]
:: [s5;b17;a17; директория пакета]
:: [s5;b17;a17; [C@5 PACKAGE]]
:: [s5;b17;a17; имя пакета]
:: [s5;b17;a17; [C@5 OUTDIR]]
:: [s5;b17;a17; директори вывода пакета]
:: [s5;b17;a17; [C@5 INCLUDE]]
:: [s5;b17;a17; список всех путей включений]
:: [s5;b17;a17; [C@5 EXEPATH]]
:: [s5;b17;a17; исполнимый путь]
:: [s5;b17;a17; [C@5 EXEDIR]]
:: [s5;b17;a17; директория исполнимых]
:: [s5;b17;a17; [C@5 EXEFILE]]
:: [s5;b17;a17; имя исполнимого файла]
:: [s5;b17;a17; [C@5 EXETITLE]]
:: [s5;b17;a17; имя исполнимого файла без расширения]}}&]
[s5; &]
[ {{1914:8086h1; [s5; Синтаксис:]
:: [s5; [C@5 custom(<когда>) <расщирение>, <команда>, 
<вывод>;]]
:: [s5; Пример:]
:: [s5; [C@5 custom `".rc`", `"rc /fo`$(OUTDIR)`\`\`$(TITLE)`_rc.obj `$(PATH)`", 
`"`$(OUTDIR)`\`\`$(TITLE)`_rc.obj`";]&]
[s5; [C@5 custom(POSIX PROTECT) `"post`-link`", `"ProtectEncrypt `$(OUTPATH) 
AABBCCDDEEFF00112233445566778899`", `"`";]]}}&]
[s5; &]
[s3;:5: [@(128.0.255) 5. Описание]&]
[s5; Описание пакета, которое появляется 
в диалоговом окне выбора пакета, устанавлива
ется ключевым словом [C@5 description]. Строка 
описания также может включать порцию 
форматирования, которая определяет 
цвет и стиль имени пакета в списке 
пакетов DinrusIDE.&]
[ {{1914:8086h1; [s5; Синтаксис:]
:: [s5; [C@5 description <строка`_описания>;]&]
[s5; где&]
[s5; [C@5 <строка`_описания> `= <действительное`_описа
ние> `[`\377 `[B`] `[I`] `[<R>,<G>,<B>`]`]]&]
[s5; [C@5 B ]делает текст полужирным&]
[s5; [C@5 I ]делает текст курсивом&]
[s5; [C@5 <R>,<G>,<B> ]представляют целое в диапазоне 
от 0 до 255,&]
[s5; определяющее цвет]
:: [s5; Пример:]
:: [s5; [C@5 description `"Non`-GUI code. Streams, NTL containers, concrete 
types, Value, XML, C parsing etc...`\377B128,0,0`";]]}}&]
[s5; &]
[s3;:6: [@(128.0.255) 6. Файлы]&]
[s5; Possibly the most important part of package is the list of files 
that belong to it. Each file in the list can have set several 
options which specify how it is handled and displayed.&]
[ {{1914:8086h1; [s5; Syntax:]
:: [s5; [C@5 file <file`_name> `[<file`_options>`]`* `[,<file`_name> `[<file`_options>`]`*
`]`*;]&]
[s5; where&]
[s5; [C@5 <file`_options> `= options(<when>) <option> `| depends(<when>) 
<dependency> `| optimize`_speed `| optimize`_size `| readonly 
`| separator `| charset <charset`_name> `| tabsize <num 1`-20> 
`| font <num 0`-3> `| highlight <highlighter`_name> ]]
:: [s5; Example:]
:: [s5; [C@5 file ]&]
[s5; [C@5 -|FindInFiles.cpp optimize`_speed,]&]
[s5; [C@5 -|idefile.cpp charset `"iso8859`-1`",]&]
[s5; [C@5 -|Resources readonly separator,]&]
[s5; [C@5 -|ide.rc depends() ide.ico;]]}}&]
[s0; &]
[s5; File options have following meaning:&]
[ {{2745:7255^ [s5; [C@5 options]]
:: [s5; Additional compiler options to be used for file.]
:: [s5; [C@5 depends]]
:: [s5; Additional dependency on other file in package. Note that header 
file dependencies are resolved by build system.]
:: [s5; [C@5 optimize`_speed]]
:: [s5; In optimal build mode, this file should be optimized for speed.]
:: [s5; [C@5 optimize`_size]]
:: [s5; In optimal build mode, this file should be optimized for size, 
even if whole package is optimized for speed. Note that size 
optimization is default.]
:: [s5; [C@5 readonly]]
:: [s5; IDE setting: file should be opened in read`-only mode in editor.]
:: [s5; [C@5 charset ]]
:: [s5; IDE setting: Information about character encoding of file.]
:: [s5; [C@5 tabsize ]]
:: [s5; IDE setting: Information about tab size.]
:: [s5; [C@5 font]]
:: [s5; IDE setting: Information about font used.]
:: [s5; [C@5 highlight]]
:: [s5; IDE setting: Information about syntax highlighting overried (normal 
default depends on file extension).]}}&]
[s0; &]
[s3;:7: [@(128.0.255) 7. Add/Remove flags]&]
[s5; It is possible to automatically add or remove flags, based on 
other flags. This is can be handy for example for platform dependent 
flag switching.&]
[ {{1914:8086h1; [s5; Syntax:]
:: [s5; [C@5 flags(<when>) `[!`]<flag`_name> `[`[!`]<flag`_name>`]`*;]]
:: [s5; Example:]
:: [s5; [C@5 flags(POSIX) `"ALSA OSS`";]]}}&]
[s0; &]
[s3;:8: [@(128.0.255) 8. Additional includes]&]
[s5; If the package requires some special include paths, they can 
be added using [C@5 include] keyword.&]
[ {{1914:8086h1; [s5; Syntax:]
:: [s5; [C@5 include`[(<when>)`] <path> `[, <path>`]`*;]]
:: [s5; Example:]
:: [s5; [C@5 include ../lib;]&]
[s5; [C@5 include(POSIX) /opt/someapp/include;]]}}&]
[s0; &]
[s3;:9: [@(128.0.255) 9. Libraries]&]
[s5; If the package requires to be linked with a shared or static 
library, those can be added using [C@5 library] keyword.&]
[ {{1914:8086h1; [s5; Syntax:]
:: [s5; [C@5 library`[(<when>)`] <lib`_name> `[, <lib`_name>`]`*;]]
:: [s5; Example:]
:: [s5; [C@5 library png;]&]
[s5; [C@5 include(POSIX) jpeg;]]}}&]
[s0; &]
[s3;:10: [@(128.0.255) 10. Linking options]&]
[s5; Each package can specify special options to be passed to the 
linker command. Note that since each platform can have different 
linker, you should provide link option for each supported platform.&]
[ {{1914:8086h1; [s5; Syntax:]
:: [s5; [C@5 link`[(<when>)`] <lib`_name> `[, <lib`_name>`]`*;]]
:: [s5; Example:]
:: [s5; [C@5 link(MSC DEBUG) /nodefaultlib:msvcrt.lib;]]}}&]
[s0; &]
[s3;:11: [@(128.0.255) 11. Size optimization]&]
[s5; The [C@5 optimize`_size] keyword can be used to tell compiler 
that the entire package should be optimized for smaller size 
of resulting binary when compiled with `"Optimal`" settings. 
It takes no parameters.&]
[ {{1914:8086h1; [s5; Syntax:]
:: [s5; [C@5 optimize`_size;]]
:: [s5; Example:]
:: [s5; [C@5 optimize`_size;]]}}&]
[s0; &]
[s3;:12: [@(128.0.255) 12. Speed optimization]&]
[s5; The [C@5 optimize`_speed] keyword can be used to tell compiler 
that the entire package should be optimized for faster execution 
when compiled with `"Optimal`" settings. . It takes no parameters.&]
[ {{1914:8086h1; [s5; Syntax:]
:: [s5; [C@5 optimize`_speed;]]
:: [s5; Example:]
:: [s5; [C@5 optimize`_speed;]]}}&]
[s0; &]
[s3;:13: [@(128.0.255) 13. Compiler options]&]
[s5; Any special options necessary for the package compilation may 
be added with [C@5 options] keyword. They will be passed to the 
compiler command only when compiling given package.&]
[ {{1914:8086h1; [s5; Syntax:]
:: [s5; [C@5 options`[(<when>)`] <lib`_name> `[, <lib`_name>`]`*;]]
:: [s5; Example:]
:: [s5; [C@5 options(MSC) `"/D PSAPI`_VERSION`=1`";]]}}&]
[s0; &]
[s3;:14: [@(128.0.255) 14. Main configuration]&]
[s5; The set of flags that are available in the Main configuration 
droplist in DinrusIDE are stored using the [C@5 mainconfig] keyword. 
The name part is optional `- if the flags combination doesn`'t 
have a name it is represented as an empty string.&]
[ {{1914:8086h1; [s5; Syntax:]
:: [s5; [C@5 mainconfig <name> `= <flags> `[,<name> `= <flags>`]`*;]]
:: [s5; Example:]
:: [s5; [C@5 mainconfig `"`" `= `"GUI`", `"Without gtk`" `= `".NOGTK GUI`";]]}}&]
[s0; &]
[s3;:15: [@(128.0.255) 15. No BLITZ]&]
[s5; The [C@5 noblitz] keyword allows to prohibit BLITZ for entire 
package. It takes no parameters.&]
[ {{1914:8086h1; [s5; Syntax:]
:: [s5; [C@5 noblitz;]]
:: [s5; Example:]
:: [s5; [C@5 noblitz;]]}}&]
[s0; &]
[s3;:16: [@(128.0.255) 16. Target]&]
[s5; The section [C@5 target] can specify the name and/or path to the 
resulting executable. The value can optionally depend on the 
flags used.&]
[ {{1914:8086h1; [s5; Syntax:]
:: [s5; [C@5 target`[(<when>)`] <package`_name> `[, <package`_name>`]`*;]]
:: [s5; Example:]
:: [s5; [C@5 target calc.ocx;]&]
[s3; target(TEST) ld`-test.exe;]}}&]
[s0; &]
[s3;:17: [@(128.0.255) 17. Used packages]&]
[s5; Each package specifies a list of packages which it depends on. 
The dependency specified in [C@5 uses] section can be either conditional 
(based on flags) or unconditional.&]
[ {{1914:8086h1; [s5; Syntax:]
:: [s5; [C@5 uses`[(<when>)`] <package`_name> `[, <package`_name>`]`*;]]
:: [s5; Example:]
:: [s5; [C@5 uses CtrlCore;]&]
[s5; [C@5 uses(POSIX `| LINUX `| FREEBSD) PdfDraw;]]}}&]
[s0; ]]