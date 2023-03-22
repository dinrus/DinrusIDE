topic "Введение в U++";
[=b133;*R5 $$1,2#50732784410233463530219518343157:Heading1]
[a17;*@(170.42.0) $$2,3#48345324464242285505540720043115:Heading2]
[2 $$3,3#42410412485283044863419059360682:Body1]
[ $$0,0#00000000000000000000000000000000:Default]
[{_}%RU-RU 
[s0; &]
[s1; [@(128.0.255) Введение в U`+`+]&]
[s0; &]
[s0; &]
[s2; Добро Пожаловать в U`+`+.&]
[s3; U`+`+ вместе представляет собой как 
фреймоворк для приложений (библиотеку 
классов для Win32 и Linux), так и полный 
инструментарий разработки, включая 
дизайнер RAD GUI, редактор исходного 
кода, редактор изображений, инструмент 
документирования кода и построитель 
проектов на C`+`+ и других языках. Также 
он предоставляет мощный `"C`-подобный`" 
язык сценариев (ESC), позволяющий конечному 
пользователю предоставлять расширяемость 
и кастомизацию собственных приложений, 
в также расширять саму DinrusIDE (the U`+`+ 
IDE)&]
[s3; &]
[s3; В этой тематике описаны ключевые 
понятия U`+`+, чтобы помочь вам начать 
работу, как можно быстрее. Если вы 
желаете испробовать U`+`+ до того, как 
продолжите чтение, можете построить 
и запустить пример HelloWorld, а также 
прочесть руководство по HelloWorld здесь[@(255.42.0)  
(fix)].&]
[s0; &]
[s0; &]
[s2; [_ DinrusIDE]&]
[s3; Перед тем как запустить DinrusIDE, сначала 
нужно выбрать пакет и сборку, с которыми 
предстоит работать, используя диалоговое 
окно `"Выбрать главный пакет`".  После 
выбора сборки и пакета, этот пакет 
откроется в DinrusIDE. Открытый пакет 
считается [* главным пакетом], а его 
имя показывается в титульной строке 
окна приложения U`+`+. В верхней панели 
слева в DinrusIDE показано имя главного 
пакета и список используемых им других 
пакетов.  В левой нижней панели приводится 
список файлов, принадлежащих пакету, 
имя которого подсвечено в верхней 
панели. Чтобы выбрать другой главный 
пакет, используется опция из меню 
Файл `"Открыть главный пакет`". Подробнее 
об использовании DinrusIDE, [^topic`:`/`/DinrusIDE`/app`/GettingStarted`_ru`-ru^ ч
итайте здесь][@(226.42.0) .]&]
[s0; &]
[s0; &]
[s2; Пакеты, сборки и гнёзда&]
[s3; Пакеты играют центральную роль в 
U`+`+. Исполнимое приложение стоится 
из пакета. Пакет также может строиться 
в библиотеку динамической компоновки 
(DLL), в статическую библиотеку или 
в набор объектных файлов. Пакет может 
использоваться другими пакетами. 
Пакет соответствует единичной директории, 
имя которой совпадает с именем пакета. 
Сама DinrusIDE является пакетом, и её можно 
перестроить по желанию из самой же 
DinrusIDE!&]
[s3; &]
[s3; О сборке можно думать, как о коллекции 
пакетов, но на самом деле `- это лишь 
набор путей, которые определяют места 
поиска U`+`+ пакетов и исходников, необходимых
 во время построения одного из пакетов 
сборки. Пути, определённые в сборке, 
сохраняются в файле определения сборки 
с расширением .var, и помещаются в корневой 
папке установки U`+`+. Пакет может `"появляться
`" в нескольких сборках.&]
[s3; &]
[s3; U`+`+ требует, чтобы у пакета была гнездовая 
организация. Гнездо на самом деле 
`- это просто директория,  в которой 
находится набор директорий пакетов, 
папки и файлы. Сборка определяет упорядоченн
ый список гнёзд (путей), а пакеты, содержащиес
я в этих гнёздах, формируют пакеты 
сборки. Более подробно о пакетах и 
сборках описано в [^topic`:`/`/DinrusIDE`/app`/PackagesAssembliesAndNests`_ru`-ru^ `"
Пакеты, Сборки и Гнёзда`"] и. [^topic`:`/`/DinrusIDE`/app`/ConfiguringPackagesAssemblies`_ru`-ru^ `"
Создание и Конфигурирование Сборок 
и Пакетов`"]&]
[s0;_@(28.85.0) &]
[s0;_@(28.85.0) &]
[s2; RAD GUI Дезайнер&]
[s0; &]
[s0; &]
[s0; &]
[s2; Язык макрокоманд ESC&]
[s0; &]
[s0; &]
[s2; Справочная система.&]
[s3; Документация для U`+`+ создаётся с 
помощью системы `"Topic`+`+`".&]
[s3; &]
[s3; &]
[s0; &]
[s2; Лицензия U`+`+&]
[s3; The U`+`+ license is BSD, allowing almost unrestricted use of 
the product and its source&]
[s3; code.  Full C`+`+ source code is provided for both DinrusIDE 
and the class framework,&]
[s3; ensuring both the future of U`+`+ and the ability to support 
and extend your &]
[s3; own applications.&]
[s0; &]
[s0; ]]