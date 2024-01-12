topic "Поддержка UHD и темы Dark";
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
[s2; [@3 Поддержка UHD`-дисплеев и темы Dark]&]
[s3; Содержание&]
[s0; &]
[s0; [^topic`:`/`/Draw`/srcdoc`/UhdAndDarkTheme`_ru`-ru`#1^ 1. Детекция 
режима ГИП]&]
[s0; [^topic`:`/`/Draw`/srcdoc`/UhdAndDarkTheme`_ru`-ru`#2^ 2. Активация 
поддержки режима Dark]&]
[s0; [^topic`:`/`/Draw`/srcdoc`/UhdAndDarkTheme`_ru`-ru`#3^ 3. Масштабирова
ние ГИП под действительный ГИП`-шрифт 
и UHD`-разрешение]&]
[s0; [^topic`:`/`/Draw`/srcdoc`/UhdAndDarkTheme`_ru`-ru`#4^ 4. Настройка 
цвета]&]
[s0; [^topic`:`/`/Draw`/srcdoc`/UhdAndDarkTheme`_ru`-ru`#5^ 5. Файлы 
Iml]&]
[s0; &]
[s3;:1: [@(128.0.255) 1. Детекция режима ГИП(GUI)]&]
[s5; Режим UHD активируется, когда ГИП`-шрифт 
больше 24`-х пикселей. Режим темы Dark 
активируется, если [* IsDark]([* SColorPaper]()),то 
есть значение grayscale дефолтного фона 
меньше 80. Заметьте, что оба режима 
в общем создают 4 комбинации: стандартное 
разрешение со светлой темой, стандартное 
разрежение с тёмной темой, UHD`-разрешение 
со светлой темой, UHD`-разрешение с 
тёмной темой.&]
[s5; Функции[*  IsUHDMode() ]и [* IsDarkTheme() ]возвращают 
соответствующий текущий статус ГИП.&]
[s3;:2: [@(128.0.255) 2. Активация поддержки режима 
Dark]&]
[s5; На некоторых платформах (Win32), приложение 
должно вызвать Ctrl`::SetDarkThemeEnabled(); при 
старте GUI`_APP`_MAIN , чтобы активировать 
тёмный режим. Это гарантирует правильное 
поведение приложения с тёмными цветами.&]
[s3;:3: [@(128.0.255) 3. Масштабирование ГИП под 
действительный ГИП`-шрифт и UHD`-разрешение]&]
[s5; Координаты U`+`+ при операциях отчерчивания 
всегда приводятся в реальных пикселах 
для целевых экранов. U`+`+ предоставляет 
различные функции для настройки метрик 
ГИП`-элементов под размер шрифта хостовой 
платформы и режим UHD. Некоторые из 
этих функций используют `'font`-ratio`', 
т.е. соотношение метрик текущего дефолтного 
ГИП`-шрифта к `'design`-font`' (который более`-менее 
определён как шрифт, в котором текст 
`"OK Cancel Exit Retry`" имеет Size(99, 13), font ratio 
при этом равно отношению размера 
текущего ГИП`-шрифта к его предопределённому
 размеру).&]
[ {{4916:5084^ [s0;b42;a42; int [* Zx](int cx);&]
[s0;b42;a42; double [* Zxf](double cx);]
::= [s5; Масштабирует значение по горизонтали 
на основе font ratio.]
::^ [s5; int [* Zy](int cy);]
::= [s5; Масштабирует значение по вертикали 
на основе font ratio.]
:: [s5; Size [* Zsz](int cx, int cy);&]
[s0;b42;a42; Size [* Zsz](Size sz);]
:: [s5; Масштабирует размер на основе font 
ratio.]
:: [s5; Font [* FontZ](int face, int height `= 0);&]
[s5; Font [* StdFontZ](int height `= 0);&]
[s5; Font [* SansSerifZ](int height `= 0);&]
[s5; Font [* SerifZ](int height `= 0);&]
[s5; Font [* MonospaceZ](int height `= 0);&]
[s5; Font [* RomanZ](int height `= 0);&]
[s5; Font [* ArialZ](int height `= 0);&]
[s0;b42;a42; Font [* CourierZ](int height `= 0);]
:: [s5; Создаёт шрифт, масштабируя его высоту 
на основе текущего font ratio. Если высота 
равна 0, она устанавливается в высоту 
текущего ГИП`-шрифта.]
:: [s0;b42;a42; int [* DPI](int a);&]
[s0;b42;a42; double [* DPI](double a);&]
[s0;b42;a42; Size [* DPI](Size sz);&]
[s0;b42;a42; Size [* DPI](int cx, int cy);]
:: [s5; Если режим UHD активирован, возвращает 
вводный аргумент, умноженный на 2, 
иначе возвращает его неизменённым.]
:: [s0;b42;a42; Image [* DPI](const Image`& a, const Image`& b);]
:: [s5; Возвращает  [* b, ]если UHD активирован, 
[* a] в обратном случае.]}}&]
[s5; Обычно используются функции [* DPI], 
если значение касается Image, и функции 
`'Z`' `- если оно касается размера текста.&]
[s3;:4: [@(128.0.255) 4. Настройка цвета]&]
[s5; Если в приложении определяются какие`-либо 
цвета, их нужно настроить под тёмную 
тему. Чаще всего это выполняется с 
помощью [^topic`:`/`/Draw`/src`/Colors`_ru`-ru^ предопределённ
ых цветов]. Иногда доступна только 
светлая цветовая тема, которую нужно 
преобразовать в тёмную тему `- это 
можно выполнить с помощью функции 
[* DarkTheme]. Альтернативно, [* AdjustIfDark] преобразует 
цвет под [* DarkTheme,] только если в данный 
момент активирован режим тёмной темы.&]
[s3;:5: [@(128.0.255) 5. Файлы Iml]&]
[s5; Файлы Iml чаще всего содержат рисунки, 
которые используются в ГИП`-интерфейсе. 
Вполне очевидно, что для 4`-х ГИП`-режимов 
эти рисунки должны быть разными.&]
[s5; .iml всегда должен иметь рисунки для 
стандартного разрешения / светлой 
темы. Эти расунки используются для 
оперделения набора иконок.&]
[s5; U`+`+ далее использует смарт`-алгоритмы, 
чтобы преобразовать такие рисунки 
под текущий ГИП`-режим. В большинстве 
случаев это работает приемлемо.&]
[s5; Разработчик должен решать,нужно 
ли предоставлять соответственные 
варианты любых рисунков для любого 
целевого режима, которые будут использованы
 вместо преобразования основной иконки. 
Такие варианты нужно помещать в том 
же файле .iml , либо в отдельном .iml`-файле.&]
[s0; Полный контроль процесса доступен 
в деталях об изображении в дизайнере 
иконок:&]
[s0; &]
[s0;= &]
[s0;= 
@@rawimage:1953&2000
(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)
&]
[s0; &]
[ {{1930:8070^ [s5; Фиксированный]
:: [s5; Рисунок никогда не масштабируется 
или затемняется, соответствуя текущему 
режиму.]
:: [s5; Фиксированные цвета]
:: [s5; Рисунок никогда не затемняется в 
соответствии с текущим режимом. Устанавлива
ется, если рисунок выглядит лучше 
в тёмном режиме, на самом деле не преобразуя 
его.]
:: [s5; Фиксированный размер]
:: [s5; Рисунок никогда не масштабируется 
под текущий режим.]
:: [s5; Вариант UHD]
:: [s5; Рисунок является вариантом для режима 
UHD.]
:: [s5; Тёмный вариант]
:: [s5; Рисунок является вариантом для темы 
Dark.]
:: [s5; Экспорт...]
:: [s5; [/ Это не относится к UHD / режиму темы 
Dark, но если отмечено, этот рисунок 
экспортируется как .ico и .png файлы. 
Это предназначено для иконки приложения 
(например, показываемой в оболочке 
хоста).]]}}&]
[s0; &]
[s5; Альтернативно, используется .iml, предназначе
ниий для оверрайдов определённого 
режима. Этот оверрайд затем используется 
как параметр в `"iml`_source`" части процесса 
включения .iml:&]
[s0; &]
[s7; #define IMAGECLASS MyImg&]
[s7; #define IMAGEFILE <MyApp/MyImg.iml>&]
[s7; #define [* IMAGEFILE`_UHD ]<MyApp/MyImgHD.iml>&]
[s7; &]
[s7; #include <Draw/iml`_source.h>&]
[s0; &]
[s5; Имена макросов для соответствующих 
ГИП`-режимов: [* MAGEFILE`_DARK], [* IMAGEFILE`_UHD], [* IMAGEFILE`_DARK`_UHD].
 Заметьте, что инструмент DinrusIDE, создающий 
код импорта .iml, автоматически добавит 
резонно проименованные файлы .iml (типа 
MyAppHD.iml, MyAppDK.iml, MyAppHDDK.iml),. как вариантные, 
автоматически.&]
[s5; Наконец, можно также отключить автоматическ
ое преобразование для файла .iml  определением
 макросов #defining FIXED`_COLORS и/или FIXED`_SIZE:&]
[s0; &]
[s7; #define IMAGECLASS MyImg&]
[s7; #define IMAGEFILE <MyApp/MyImg.iml>&]
[s7; #define [* FIXED`_COLORS]&]
[s7; #define [* FIXED`_SIZE]&]
[s7; &]
[s7; #include <Draw/iml`_source.h>&]
[s0; ]]