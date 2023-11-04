topic "Использование DinrusIDE как Инструмент Кросс-Компиляции";
[l288;i1120;a17;O9;~~~.1408;2 $$1,0#10431211400427159095818037425705:param]
[a83;*R6 $$2,5#31310162474203024125188417583966:caption]
[H4;b83;*4 $$3,5#07864147445237544204411237157677:title]
[i288;O9;C2 $$4,6#40027414424643823182269349404212:item]
[b42;a42;ph2 $$5,5#45413000475342174754091244180557:text]
[l288;b17;a17;2 $$6,6#27521748481378242620020725143825:desc]
[l321;t246;C@5;1 $$7,7#20902679421464641399138805415013:code]
[b2503;2 $$8,0#65142375456100023862071332075487:separator]
[*@(0.0.255)2 $$9,0#83433469410354161042741608181528:base]
[t4167;C2 $$10,0#37138531426314131251341829483380:class]
[l288;a17;*1 $$11,11#70004532496200323422659154056402:requirement]
[i417;b42;a42;O9;~~~.416;2 $$12,12#10566046415157235020018451313112:tparam]
[b167;C2 $$13,13#92430459443460461911108080531343:item1]
[i288;a42;O9;C2 $$14,14#77422149456609303542238260500223:item2]
[*@2$(0.128.128)2 $$15,15#34511555403152284025741354420178:NewsDate]
[l321;*C$7;2 $$16,16#03451589433145915344929335295360:result]
[l321;b83;a83;*C$7;2 $$17,17#07531550463529505371228428965313:result`-line]
[l160;t4167;*C+117 $$18,5#88603949442205825958800053222425:package`-title]
[2 $$19,0#53580023442335529039900623488521:gap]
[t4167;C2 $$20,20#70211524482531209251820423858195:class`-nested]
[b50;2 $$21,21#03324558446220344731010354752573:Par]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_}%RU-RU 
[s2; [@3 Использование DinrusIDE как Инструмент 
Кросс`-Компиляции]&]
[s3; [@3 Содержание]&]
[s0; &]
[s3; [^topic`:`/`/DinrusIDE`/app`/CrossComp`_ru`-ru`#1^ 1. Введение]&]
[s3; [^topic`:`/`/DinrusIDE`/app`/CrossComp`_ru`-ru`#2^ 2. Создание 
пустого пакета в Projects]&]
[s3; [^topic`:`/`/DinrusIDE`/app`/CrossComp`_ru`-ru`#3^ 3. Создание 
файла макросов  для пакета <имя>]&]
[s3; [^topic`:`/`/DinrusIDE`/app`/CrossComp`_ru`-ru`#4^ 4. Запуск 
кросс`-компиляции]&]
[s3; [^topic`:`/`/DinrusIDE`/app`/CrossComp`_ru`-ru`#5^ 5. Добавление 
файлов и спейсеров]&]
[s3; [^topic`:`/`/DinrusIDE`/app`/CrossComp`_ru`-ru`#6^ 6. Открыть 
файл или Открыть Edited Files Warning]&]
[s3; [^topic`:`/`/DinrusIDE`/app`/CrossComp`_ru`-ru`#7^ 7. Встройка 
в DinrusIDE]&]
[s0; &]
[s3;:1: [@(128.0.255) 1. Введение]&]
[s5; При отключении многих фич DinrusIDE, его 
можно использовать как инструмент 
кросплатформной разработки, использующий 
заранее заготовленные внешние makefiles 
(файлы с описание процесса построения). 
Это особенно пригодно при перемещении 
приложений, разработанных изначально 
под другие ГИП (IDE) в DinrusIDE.&]
[s5; Далее следует описание того, как 
использовать DinrusIDE с такими внешними 
makefiles, при этом без необходимости 
их редактировать. Могут иметься другие 
способы выполнения таких задач; как, 
впрочем, может иметься возможность 
применить существующие средства 
построения DinrusIDE. Но целью было уменьшение 
усилий при преобразовании ради получения 
приемлемой компиляции.&]
[s5; &]
[s3;:2: [@(128.0.255) 2. Создание пустого пакета 
в Projects]&]
[s5; Запустите DinrusIDE и создайте новый 
пакет.  Определите <имя> пакета и выберите 
шаблон `'пустой`' пакет. Как только 
пакет будет создан, загрузите и отредактируй
те файл <имя>.upp из директории <имя>, 
используя DinrusIDE. Если эти строки ещё 
отсутствуют, добавьте их:&]
[s5;  &]
[ {{10000<544;>2176; [s0; mainconfig&]
[s0;      `"`"`=`"`";]}}&]
[s5; &]
[s5; Закройте и снова откройте пакет. 
Это проверка на то, находится ли пакет 
<имя> и что файл <имя>.upp распознаётся 
и имеет правильный синтаксис. Без 
указанных выше двух строк DinrusIDE пакета 
не сможет отыскать.&]
[s3;:3: [@(128.0.255) 3. Создание файла макросов 
для пакета <имя>]&]
[s0; &]
[s5; При открытом пакете <имя> в DinrusIDE создайте 
новый файл с расширением usc... Рекомендуем 
назвать его macro.usc. В этом файле макрокоманд 
будет проводиться вся работа. Создайте 
соответствующий макрос в этом файле, 
наподобии ниже показанных примеров. 
Здесь применяется язык сценариев 
ESC. Заметьте, что потребуется функция 
из стандартной библиотеки с названием 
`'Execute`'. &]
[ {{10000@(255.255.150) [s0; [* macro ]`"MakeA`" Alt`+A `{&]
[s0; -|Execute(`"c:/mingw/bin/mingw32`-make.exe `-C d:/myapps/control 
`-f d:/myapps/control/makealarm all`");&]
[s0; `}&]
[s0; &]
[s0; [* macro ]`"CopyLibs`" Alt`+0 `{&]
[s0; -|Execute (`"cp.exe d:/myapps/edrtoslib/libed8515.a d:/myapps/control/`");&]
[s0; -|Execute(`"cp d:/myapps/edrtoslib/libed8535.a d:/myapps/control/`");&]
[s0; `}]}}&]
[s5; &]
[s5; Первый макрос с именем `'MakeA`' может 
вызываться нажатием комбинации клавиш 
`'Alt`+A`' или кликом над именем макроса. 
При вызове он запускает стандартную 
функцию Execute. Параметром к этой функции 
является полный путь к приложению 
командной строки. Нужно включить 
сюда свои собственные командные строки, 
которые соответствуют вашей среде 
кросс`-разработки.&]
[s5; Пример выше относится с пакету кросс`-разраб
отки AVR с именем WINAVR. Используются 
компилятор mingw GCC и другие пакеты, 
вместе с утилитой mingw32`-make.&]
[s5; Заметьте, что определён явный makefile 
и указана явно проименованная директория. 
На то две причины: во0первых, ГИП имеет 
при выполнении команд в качестве 
`'root`' (корня) директорию `'Projects`' , а 
не директорию пакета; во`-вторых, явное 
именование директории ведёт к тому, 
что makefile не надо будет редактировать.&]
[s5; Also for some of the above `'applications; to be found the `'autoexec.bat`' 
file path and other environmental variable names have been set 
from the previous cross development application. This is not 
part of the function of DinrusIDE.&]
[s3;:4: [@(128.0.255) 4. Запуск кросс`-компиляции]&]
[s5; The `'macro`' menu item will appear sometime on the menu bar. 
You may have to close and reopen the package for it to appear 
for the first time. Click on `'macro`' and then the required 
macro name to run it. Alternatively use the quick key, Alt`+M 
in this case, if it has been defined. When selected the macro 
body will be executed which will in this case invoke the application 
make facility. Errors will be displayed in the bottom window. 
Any errors can be located by clicking the left mouse button on 
the error line as with the normal DinrusIDE operation.&]
[s3;:5: [@(128.0.255) 5. Добавление Файлов и Спейсеров]&]
[s5; Files and spacers can be added in the normal way by clicking 
the right mouse button in the files window at the left side of 
the screen. These however will have no association with the external 
makefiles.&]
[s3;:6: [@(128.0.255) 6. Open File or Open Edited Files Warning]&]
[s5; I am unsure of what will happen with open / edited files with 
the external makefiles. When the inbuilt `'build`' is executed 
any open files are updated so that the changed will be compiled. 
This does not appear to be the case when the external makefile 
is used.  It is recommended that all edited files are written 
back prior to invoking the macro.&]
[s3;:7: [@(128.0.255) 7. Встройка в DinrusIDE]&]
[s5; When more knowledge of DinrusIDE is known it may be possible 
to incorporate such external make facilities such as WINAVR into 
its standard build features. There are mechanisms to select other 
compiler builds but the extensive windows library will need to 
be uncoupled.  This however is a future project.]]