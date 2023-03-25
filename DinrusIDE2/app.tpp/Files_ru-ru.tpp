topic "Типы Файлов";
[l288;i704;a17;O9;~~~.992; $$1,0#10431211400427159095818037425705:param]
[a83;*R6 $$2,5#31310162474203024125188417583966:caption]
[H4;b83;*4 $$3,5#07864147445237544204411237157677:title]
[b167;a42;C $$4,13#40027414424643823182269349404212:item]
[b42;a42;ph2 $$5,5#45413000475342174754091244180557:text]
[l288;a17; $$6,6#27521748481378242620020725143825:desc]
[l321;t246;C@5;1 $$7,7#20902679421464641399138805415013:code]
[b2503; $$8,0#65142375456100023862071332075487:separator]
[*@(0.0.255) $$9,0#83433469410354161042741608181528:base]
[t4167;C+117 $$10,0#37138531426314131251341829483380:class]
[l288;a17;*1 $$11,11#70004532496200323422659154056402:requirement]
[i416;b42;a42;O9;~~~.416; $$12,12#10566046415157235020018451313112:tparam]
[b167;C $$13,13#92430459443460461911108080531343:item1]
[a42;C $$14,14#77422149456609303542238260500223:item2]
[*@2$(0.128.128) $$15,15#34511555403152284025741354420178:NewsDate]
[l321;*C$7 $$16,16#03451589433145915344929335295360:result]
[l321;b83;a83;*C$7 $$17,17#07531550463529505371228428965313:result`-line]
[l160;t4167;*C+117 $$18,5#88603949442205825958800053222425:package`-title]
[a42;ph2 $$19,19#646FC4297C32F11C479D4FEE54310C42:text`-table]
[b83;*+117 $$20,20#1546C2CD4112BAB0A26C534D6F11ED13:subtitle]
[ $$0,0#00000000000000000000000000000000:Default]
[{_}%RU-RU 
[s2; [@3 Типы Файлов]&]
[s3; [@3 Содержание]&]
[s0;2 &]
[s0; [^topic`:`/`/DinrusIDE`/app`/Files`_ru`-ru`#1^2 1. Введение]&]
[s0; [^topic`:`/`/DinrusIDE`/app`/Files`_ru`-ru`#2^2 2. Типы файлов]&]
[s0; [2     ][^topic`:`/`/DinrusIDE`/app`/Files`_ru`-ru`#2`_1^2 2.1 Текстовые 
файлы]&]
[s0; [2     ][^topic`:`/`/DinrusIDE`/app`/Files`_ru`-ru`#2`_2^2 2.2 Особые 
файлы]&]
[s0; [2     ][^topic`:`/`/DinrusIDE`/app`/Files`_ru`-ru`#2`_3^2 2.3 Прочие 
файлы и директории]&]
[s0; [2     ][^topic`:`/`/DinrusIDE`/app`/Files`_ru`-ru`#2`_4^2 2.4 Файлы 
настроек]&]
[s0;2 &]
[s3;:1: [@(28.85.200) 1. Введение]&]
[s5; В этой статье обсуждаются связанные 
с U`+`+ файлы и типы файлов.&]
[s3;:2: [@5 2. Типы файлов]&]
[s20;:2`_1: [@(128.0.255) 2.1 Текстовые файлы]&]
[s5; Текстовые файлы имеют широкую поддержку 
в нашей интегрированной среде DinrusIDE. 
Многие из них используются в процессе 
повседневной разработки приложений 
U`+`+.&]
[s0;2 &]
[ {{2413:7587^ [s19; [* .cpp, .cc, .cxx, .c, .m, .mm]]
:: [s19; Файлы с этими расширениями распознаются 
как исходники построителями DinrusIDE 
C`+`+.]
:: [s19; [* .h, .hpp]]
:: [s19; Файлы`-заголовочники получаю автоматически
й тест на зависимости в построителях 
C`+`+. Пакеты U`+`+ намеренно используют 
[* h ]вместо  [* hpp] , когда в них содержатся 
определения шаблонных функций.]
:: [s19; [* .iccp]]
:: [s19; Этот тип файла распознаётся как исходник 
.cpp, но в отличие от простого .cpp, который 
вначале может быть помещён в библиотеку, 
а потом удалён компоновщиком, .icpp 
всегда компонуется в объектный файл. 
Это гарантирует, что файл скомпонуется 
всегда. Рациональ этого состоит в 
том, что можно поместить код инициализации 
модуля в .icpp , который будет линкован 
в исполнимый, даже если этот код не 
имеет ссылок на себя в прочих файлах.]
:: [s19; [* .java]]
:: [s19; Файлы Java]
:: [s19; [* .py]]
:: [s19; Файлы Python]
:: [s19; [* .rc]]
:: [s19; Файлы ресурсов Windows.]
:: [s19; [* .lay]]
:: [s19; Фыйлы выкладок. Содержат выкладки, 
например, диалоговых окон.]
:: [s19; [* .iml]]
:: [s19; Файлы изображений. Содержат иконки 
и прочие растровые картинки.]
:: [s19; [* .t]]
:: [s19; Файлы переводов, содержащие переводы 
строк, используемых в пакетах, на 
разные языки.]
:: [s19; [* .usc]]
:: [s19; Сценарии и иная мета`-информация. 
Содержат описания виджетов для дизайнера 
выкладок, включая описание визуального 
представления с помощью сценариев 
ESC.]
:: [s19; [* .upt]]
:: [s19; Шаблоны проекта.]
:: [s19; [* .defs]]
:: [s19; Фиксированные определения переписи 
макросов  для парсера Ассист`+`+ C`+`+.]
:: [s19; [* .png]]
:: [s19; DinrusIDE отображает рисунки, но если 
они меньше 1024x768, то их редактируют 
дизайнером иконок.]
:: [s19; [* .jpg]&]
[s19; [* .gif]&]
[s19; [* .bmp]]
:: [s19; Изображения DinrusIDE ]
:: [s19; [* .qtf]]
:: [s19; Это RTF U`+`+ (rich text format).]
:: [s19; [* .key]]
:: [s19; Файл определения клавиш, который 
можно редактировать.]
:: [s19; [* .log]]
:: [s19; Файлы журнала.]
:: [s19; [* .i]]
:: [s19; Эти файлы как правило, включаются 
некоторыми .[* cpp] файлами(файлом) для 
различных целей.]
:: [s19; [* .sch]]
:: [s19; Файлы схемы базы данных. Содержат 
определения выкладок базы данных. 
Приложения U`+`+ с БД строят сценарии 
построения схемы базы данных по этим 
файлам, а также используют их, чтобы 
определять структуры, переменные 
и константы, необходимые для взаимодействия
 с БД.]
:: [s19; [* .witz]]
:: [s19; Шаблоны Skylark HTML.]}}&]
[s0; &]
[s20;:2`_2: [@(128.0.255) 2.2 Особые файлы]&]
[s5; Есть также файлы с особым значением. 
Для правильной работы с U`+`+ и средой 
DinrusIDE имена должны быть такими, как 
в следующей таблице:&]
[s19;* &]
[ {{2353:7647^ [s19; [* import.ext]]
:: [s19; В этом файле находится описание [^topic`:`/`/DinrusIDE`/app`/importext`_ru`-ru^ и
мпортирования] исходников 3`-ей стороны, 
содержащихся в пакете.]
:: [s19; [* Copying]]
:: [s19; В этом файле содержится лицензионная 
информация о пакете.]
:: [s19; [* COPYING`-PLAIN]]
:: [s19; Содержит лицензионное пояснение.]
:: [s19; [* AUTHORS]]
:: [s19; Список авторов.]}}&]
[s0;*2 &]
[s20;:2`_3: [@(128.0.255) 2.3 Другие файлы и директории]&]
[s5; Во время путешествия по U`+`+ вам многут 
встретиться несколько типов файлов 
и директорий. Заметьте, пожалуйста, 
что некоторые из них расцениваются 
DinrusIDE особым образом и напрямую не 
редактируются. Иногда их можно редактироват
ь особым редактором, но чаще они обрабатываю
тся в секрете.&]
[s0;*2 &]
[ {{2370:7630h1;^ [s19; [* .upp]]
::= [s19; Файл определения пакета. Его имя 
соответствует имени пакета. Как правило, 
его обрабатывает органйзер пакетов 
DinrusIDE.]
::^ [s19; [* .tpp]]
::= [s19; Директории с таким расширением представляю
т собой группу Topic`+`+. В этих директориях 
находятся файлы тематик с таким же 
расширением .tpp.]
::^ [s19; [* all.i]]
::= [s19; Этот файл находится внутри директорий 
.tpp и содержит сгенерированный Topic`+`+ 
файл, предназначенный для включения 
группы тематик в C`+`+.]
::^ [s19; [* .tdx]]
::= [s19; Предварительно разобранные файлы 
.tpp для быстрого поиска.]
::^ [s19; [* init]]
::= [s19; Используются БЛИЦ для правильного 
включения файлов  icpp перед остальным 
кодом.]
::^ [s19; [* .scd]]
::= [s19; Старый формат словарей спеллинга, 
заменённый [* .udc]]}}&]
[s0; &]
[s20;:2`_4: [@(128.0.255) 2.4 Файлы настроек]&]
[s5; В этих файлах хранятся конфигурации 
DinrusIDE. Как правило, они находятся в 
директории установки DinrusIDE. Для создания 
бэкапа, их можно скопировать в другое 
`- надёжное `- место.  Их можно повтроно 
использовать между установками U`+`+ 
или делиться с друзьями.&]
[s0;*2 &]
[ {{2370:7630h1;^ [s19; [* DinrusIDE.cfg]]
::= [s19; Этот бинарный файл, берущий название 
от DinrusIDE.exe, или иного имени, с которым 
работает ИСР. В нём `- конфигурационные 
настройки, являющиеся сериализацией 
диалогового окна Настройка`->Среда.]
::^ [s19; [* .var]]
::= [s19; Текстовый файл, содержащий конфигурацию 
сборки, задаваемую в диалоговом окне 
`"Настройка сборки`".]
::^ [s19; [* .bm]]
::= [s19; Текстовый файл, содержащий настройки 
построения, связанные с определённым 
компилятором/компоновщиком (`"метод 
построения`").]
::^ [s19; [* cfg folder]]
::= [s19; В этой папке находятся кэш`-файлы 
DinrusIDE (tdx,cache,pkg`_cache) и файлы конфигурации 
(настройки по пакетам, открытые файлы, 
положения курсоров и т.д.).]}}&]
[s0; ]]