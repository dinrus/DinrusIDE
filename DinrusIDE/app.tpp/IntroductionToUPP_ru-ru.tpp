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
[s3; Packages are centric to U`+`+.  An executable application is 
built from a package.  A package can also build into a dynamic 
link library, a static library, or a set of object files.  A 
package can be used by other packages.  A package corresponds 
to a single directory whose name is the name of the package. 
 DinrusIDE itself is a package and you can rebuild it from within 
DinrusIDE if you wish!&]
[s3; &]
[s3; An assembly can be thought of as a collection of packages but 
it is actually just a set of paths which determine where U`+`+ 
looks for the packages and source files needed when building 
one of the assembly packages.  The paths defined by the assembly 
are stored in an assembly definition file which has a .var extension 
and is stored in the U`+`+ root installation directory.  A package 
can `"appear`" in multiple assemblies.&]
[s3; &]
[s3; U`+`+ requires that packages be organized into nests.  A nest 
is actually just a directory containing a set of package directories, 
folders or files.  An assembly defines an ordered list of nests 
(paths) and the packages contained in those nests form the packages 
of the assembly.  For more detail on packages and assemblies 
see [^topic`:`/`/DinrusIDE`/app`/PackagesAssembliesAndNests`$en`-us^ `"Packages, 
Assemblies and Nests`" ]and. [^topic`:`/`/DinrusIDE`/app`/ConfiguringPackagesAssemblies`$en`-us^ `"
Creating and Configuring Assemblies and Packages`"]&]
[s0;_@(28.85.0) &]
[s0;_@(28.85.0) &]
[s2; RAD GUI designer&]
[s0; &]
[s0; &]
[s0; &]
[s2; The ESC macro language&]
[s0; &]
[s0; &]
[s2; The help system.&]
[s3; U`+`+ documentation is created with Topic`+`+.&]
[s3; &]
[s3; &]
[s0; &]
[s2; U`+`+ License&]
[s3; The U`+`+ license is BSD, allowing almost unrestricted use of 
the product and its source&]
[s3; code.  Full C`+`+ source code is provided for both DinrusIDE 
and the class framework,&]
[s3; ensuring both the future of U`+`+ and the ability to support 
and extend your &]
[s3; own applications.&]
[s0; &]
[s0; ]]