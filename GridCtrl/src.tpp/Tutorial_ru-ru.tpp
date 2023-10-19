topic "Как используется \"упрэлт\" т.е. \"элемент управления\", т.е. \"контрол\" \"СЕТКА\"";
[l288;i704;a17;O9;~~~.992;2 $$1,0#10431211400427159095818037425705:param]
[a83;*R6 $$2,5#31310162474203024125188417583966:caption]
[b83;*2 $$3,5#07864147445237544204411237157677:title]
[b167;a42;C2 $$4,6#40027414424643823182269349404212:item]
[b42;a42;2 $$5,5#45413000475342174754091244180557:text]
[l288;a17;2 $$6,6#27521748481378242620020725143825:desc]
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
[2 $$0,0#00000000000000000000000000000000:Default]
[{_}%EN-US 
[s0; [* 1. Основы]&]
[s0;* &]
[s0; -|Чтобы использовать элемент управления 
`*`"упрэлт`"), иначе говоря, `"контрол`" 
под названием grid (то бишь, `"сетка`"), 
добавьте к себе в проект пакет GridCtrl, 
а затем просто добавьте grid (сетку), 
как любой другой `"контрол`", в редакторе 
выкладок (Complex`-> GridCtrl); или же вручную 
поместите код, где`-либо в приложении, 
например такой:&]
[s0; &]
[s7; #include <CtrlLib/CtrlLib.h>&]
[s7; #include <GridCtrl/GridCtrl.h>&]
[s7; &]
[s7; struct App : TopWindow&]
[s7; `{&]
[s7; -|typedef App CLASSNAME;&]
[s7; &]
[s7; -|GridCtrl grid;&]
[s7; &]
[s7; -|App()&]
[s7; -|`{&]
[s7; -|-|Add(grid.SizePos());&]
[s7; -|`}&]
[s7; `};&]
[s7; &]
[s7; GUI`_APP`_MAIN&]
[s7; `{&]
[s7; -|App().Run();&]
[s7; `}&]
[s0; &]
[s0; Теперь у вас есть контрол`-сетка, 
`"spanned onto the main window`" `- `"распростёртый 
на всё главное окно`". Однако, сетка 
без колонок бессмыслена. А, чтобы 
добавить колонки, напишем:&]
[s0; &]
[s7; grid.AddColumn(`"Имя`");&]
[s7; grid.AddColumn(`"Возраст`");&]
[s7; &]
[s7; &]
[s0; А теперь добавим ко всему этому данных:&]
[s0; &]
[s7; grid.Add(`"Анна`", 21)&]
[s7;     .Add(`"Яков`", 34)&]
[s7;     .Add(`"Давид`", 15);&]
[s0; &]
[s0; Как видите, первый ряд сетки, в котором 
находятся названия колонок, отрисовывается 
иначе, чем остальные. Его часто именуют 
`"заголовочником`" (header )(как в контроле 
`"массив`"), но здесь его надо бы назвать 
`"фиксированным рядом`" (так как таких 
рядов может быть несколько). Следующие 
`"строки`" являются простыми рядами.&]
[s0; &]
[s0; Добавив данные в сетку, мы их может 
легко изменить:&]
[s0; &]
[s7; grid.Set(0, 0, `"Данила`");&]
[s0; &]
[s0; Первый аргумент у Set() `- это номер 
ряда, второй `- колонка, а последний 
`- новое устанавливаемое значение. 
Запомните, что ряд 0 является первым 
после фиксированных. Для изменения 
значения фиксированного элемента 
используется SetFixed:&]
[s0; &]
[s7; grid.SetFixed(0, 1, `"Возраст человека`");&]
[s9; &]
[s0; Если номер ряда/колонки в Set больше 
общего числа рядов/колонок, `- тогда 
сетка автоматически `"раздвигается`" 
под новый элемент[` 1].&]
[s0; &]
[s0; А если нужно изменить значение текущего 
ряда, можно не указывать первый аргумент 
к Set():&]
[s0; &]
[s7; grid.Set(0, `"Данила 1`");&]
[s7; &]
[s0; В обоих случаях применима краткая 
форма:&]
[s0; &]
[s7; grid(0, 0) `= `"Данила`";&]
[s7; grid(0) `= `"Данила 1`";&]
[s0; &]
[s0; Однако, есть две разницы:&]
[s0;i150;O0; краткая форма всегда обновляет 
внутренние данные. То есть, когда 
имеется активный контрол редактирования 
(`"edit control`") над вашим элементом, его 
значение останется неизменным: только 
низлежащее значение меняется.&]
[s0;i150;O0; краткая форма никогда не освежает 
сетку.&]
[s0; &]
[s0; Краткая форма, в целом, `"нацелена`" 
на предельно скорые обновления и 
использование в обратных вызовах 
(смотрите в следующей главе)&]
[s0; &]
[s0; А теперь сделает противоположное. 
Получим данные из сетки. Для этого 
есть простой метод Get():&]
[s0; &]
[s7; Value v0 `= grid.Get(0, 0); // получить значение 
из ряда 0 и колонки 0&]
[s7; Value v1 `= grid.Get(0);    // получить значение 
из курсорного ряда и колонки 0&]
[s7; Value v0 `= grid(0, 0);     // краткая форма случая 
1&]
[s7; Value v1 `= grid(0);        // краткая форма случая 
2&]
[s0; &]
[s0; Get всегда возвращает копию внутренних 
данных (а краткая форма возвращает 
ссылку на внутреннее значение).&]
[s0; &]
[s0; 1)Ещё не полностью реализовано в текущей 
версии, только изменение размера 
рядов. Изменение размера колонок 
будет доступно в полноте в версии 
1.0.&]
[s0; &]
[s0; [* 2. Обратные вызовы]&]
[s0;* &]
[s0; Простейшим способом добавления некой 
интерактивности в контрол`-сетку 
является применение обратных вызовов. 
Их много. Наиболее частые:&]
[s0; &]
[s0; [* WhenLeftClick] `- вызывается при нажатии 
левой кнопки мыши.&]
[s0; [* WhenLeftDouble] `- вызывается при двойном 
нажатии левой кнопки мыши.&]
[s0; [* WhenRowChange] `- вызывается, когда курсор 
в сетке меняет свою позицию.&]
[s0; [* WhenEnter] `- вызывается, когда нажата 
клавиша enter.&]
[s0; &]
[s0; Добавьте в код:&]
[s0; &]
[s7; void ShowInfo()&]
[s7; `{&]
[s7; -|PromptOK(Format(`"%s is %d years old`", grid(0), grid(1)));&]
[s7; `}&]
[s7; &]
[s7; grid.WhenLeftDouble `= THISBACK(ShowInfo);&]
[s0; &]
[s0; после запуска приложения и двойном 
клике на первом ряде, вы должны увидеть:&]
[s0; &]
[s0;= 
@@image:1449&1034
(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)
&]
[s0; А теперь замысловатый код:&]
[s0; &]
[s7; void RandomColor()&]
[s7; `{&]
[s7; -|grid.GetRow().Bg(Color(rand() % 255, rand() % 255, rand() % 
255));&]
[s7; `}&]
[s7; &]
[s7; grid.WhenRowChange `= THISBACK(RandomColor);&]
[s0;* &]
[s0; Попытайтесь изменить позицию курсора 
(используя клавиши курсора или мышь).После 
изменения каждой позиции, меняется 
фоновый цвет предыдущего активного 
ряда.&]
[s0;* &]
[s0; [* 3. Редактирование, индексы и интеграция 
с базами данных]&]
[s0;* &]
[s0; Displaying static data is very useful, but in most cases some 
of it must be changed. One way is to show another window (eg 
as a reaction on double click) and put entered data from it into 
the grid. Second way is to edit data directly in grid. GridCtrl 
support two edit modes:&]
[s0; &]
[s0; 1. Row editing.&]
[s0; -|In this mode all edit controls binded to the columns are shown. 
You can move from one edit to another by pressing tab key (or 
enter if special switch is on). &]
[s0; &]
[s0; 2. Cell editing&]
[s0; -|In this mode only one edit control is displayed. Tab moves 
the cursor to the next edit control (if available).&]
[s0;* &]
[s0; Binding edits with columns is very easy. Let`'s make our example 
to allow edit name and age. First we need to declare edit controls:&]
[s7; -|&]
[s7; EditString name;&]
[s7; EditInt age;&]
[s7; &]
[s0; Then we simply call Edit method for each column:&]
[s0; &]
[s7; grid.AddColumn(`"Name`").Edit(name);&]
[s7; grid.AddColumn(`"Age`").Edit(age);&]
[s7; &]
[s0; Now you can press Enter or click double LMB to start editing. 
By default Tab skips from one cell to another (with binded edit). 
If it is the last editing cell pressing Tab adds a new row. There 
are several ways to change editing behaviour e.g:&]
[s0; &]
[s7; grid.TabAddsRow(bool b) -|enables/disables adding new row after 
pressing tab key &]
[s7; grid.TabChangesRow(bool b)-|enables/disables changing row after 
pressing tab key &]
[s7; grid.EnterLikeTab(bool b)-|enables/disables emulation of tab 
by enter key&]
[s7; grid.OneClickEdit(bool b)-|enables/disables immediate editing 
after LMB click&]
[s0; &]
[s0; [* 4. Bulit`-in toolbar, popup menu]&]
[s0;* &]
[s0; [* 5. Properties]&]
[s0;* &]
[s0; [* 6. Others]]]