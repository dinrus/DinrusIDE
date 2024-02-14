topic "Класс Console";
[i448;a25;kKO9; $$1,0#37138531426314131252341829483380:structitem]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:codeitem]
[ $$0,0#00000000000000000000000000000000:Default]
[{_} 
[s4;%RU-RU &]
[s5;:BoldDisplay`(`): [_^Display^ Display][@(0.0.255) `&]_[* BoldDisplay]()&]
[s2;%RU-RU Создаёт дисплей с полужирным 
шрифтом.&]
[s3; &]
[s4; &]
[s1;:Console`:`:class: [@(0.0.255) class]_[* Console]_:_[@(0.0.255) public]_[*@3 CodeEditor]&]
[s2;%RU-RU Это класс представляет консоль.&]
[s3; &]
[s4; &]
[s5;:Console`:`:processes: [_^Upp`:`:Array^ Array]<[_^Console`:`:Slot^ Slot]>_[* processes]&]
[s2;%RU-RU Массив из номеров слотов, в каждом 
из которых происходит коммуникация 
с определённым запущенным процессом.&]
[s3; &]
[s4; &]
[s5;:Console`:`:Console`(`): [* Console]()&]
[s2;%RU-RU Конструктор.&]
[s3; &]
[s4; &]
[s5;:Console`:`:LeftDouble`(Upp`:`:Point`,Upp`:`:dword`): [@(0.0.255) virtual] 
[@(0.0.255) void]_[* LeftDouble]([_^Upp`:`:Point^ Point]_[*@3 p], [_^Upp`:`:dword^ dword])&]
[s2;%RU-RU Событие при двойном нажатии левой 
кнопки мыши знаменуется выделением 
текста (конкретного слова) в редакторе, 
над которым находится курсор.&]
[s2;%RU-RU  [%-*@3 p   ]`- Точка, в которой нажата 
левая кнопка мыши и в которой находится 
выделяемый текст.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Console`:`:RightDown`(Upp`:`:Point`,Upp`:`:dword`): [@(0.0.255) virtual] 
[@(0.0.255) void]_[* RightDown]([_^Upp`:`:Point^ Point]_[*@3 p], [_^Upp`:`:dword^ dword])&]
[s2;%RU-RU Событие нажатия правой кнопки 
мыши в редакторе знаменуется выводом 
контексного меню &]
[s2;%RU-RU  [%-*@3 p  `-   ]Точка, в которой нажата 
правая кнопка мыши, т.е. относительно 
которой будет выведено контекстное 
меню.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Console`:`:Append`(const String`&`): [@(0.0.255) virtual] [@(0.0.255) void]_[* Append](
[@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 s])&]
[s2;%RU-RU Добавляет текст в конце строки.&]
[s2;%RU-RU  [%-*@3 s    ] `-  Добавляемый текст&]
[s3;%RU-RU &]
[s1;:Console`:`:Slot`:`:struct: [@(0.0.255) struct]_[* Slot]&]
[s2;%RU-RU Защищённая внутренняя структура, 
описывающая слот с процессом, который 
выполняется в консоли.&]
[s3; &]
[s4; &]
[s5;:Console`:`:Slot`:`:Slot`(`): [* Slot]()&]
[s2;%RU-RU Конструктор.&]
[s3; &]
[s4; &]
[s5;:Console`:`:Slot`:`:process: [_^Upp`:`:One^ One]<[_^Upp`:`:AProcess^ AProcess]>_[* proc
ess]&]
[s2;%RU-RU Консольный процесс.&]
[s3; &]
[s4; &]
[s5;:Console`:`:Slot`:`:cmdline: [_^String^ String]_[* cmdline]&]
[s2;%RU-RU Командная строка.&]
[s3; &]
[s4; &]
[s5;:Console`:`:Slot`:`:output: [_^String^ String]_[* output]&]
[s2;%RU-RU Строка вывода.&]
[s3; &]
[s4; &]
[s5;:Console`:`:Slot`:`:key: [_^String^ String]_[* key]&]
[s2;%RU-RU Ключ.&]
[s3; &]
[s4; &]
[s5;:Console`:`:Slot`:`:group: [_^String^ String]_[* group]&]
[s2;%RU-RU Группа.&]
[s3; &]
[s4; &]
[s5;:Console`:`:Slot`:`:outfile: [_^Stream^ Stream]_`*[* outfile]&]
[s2;%RU-RU Поток вывода.&]
[s3; &]
[s4; &]
[s5;:Console`:`:Slot`:`:quiet: [@(0.0.255) bool]_[* quiet]&]
[s2;%RU-RU Тихое или `"громкое`" выполнение.&]
[s3; &]
[s4; &]
[s5;:Console`:`:Slot`:`:exitcode: [@(0.0.255) int]_[* exitcode]&]
[s2;%RU-RU Код выхода.&]
[s3; &]
[s4; &]
[s5;:Console`:`:Slot`:`:last`_msecs: [@(0.0.255) int]_[* last`_msecs]&]
[s2;%RU-RU Продолжительность выполнения 
в миллисекундах.&]
[s3; &]
[s4; &]
[s5;:Console`:`:Slot`:`:serial: [@(0.0.255) int]_[* serial]&]
[s2;%RU-RU Серийный номер.&]
[s3; &]
[s1;:Console`:`:Group`:`:struct: [@(0.0.255) struct]_[* Group]&]
[s2;%RU-RU Группирующая структура.&]
[s3; &]
[s4; &]
[s5;:Console`:`:Group`:`:Group`(`): [* Group]()&]
[s2;%RU-RU Конструктор.&]
[s3; &]
[s4; &]
[s5;:Console`:`:ToErrors`(const String`&`): [@(0.0.255) void]_[* ToErrors]([@(0.0.255) cons
t]_[_^String^ String][@(0.0.255) `&]_[*@3 s])&]
[s2;%RU-RU  [%-*@3 s] .&]
[s3;%RU-RU &]
[s4; &]
[s5;:Console`:`:AppendOutput`(const String`&`): [@(0.0.255) void]_[* AppendOutput]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[*@3 s])&]
[s2;%RU-RU  [%-*@3 s] .&]
[s3;%RU-RU &]
[s4; &]
[s5;:Console`:`:Flush`(`): [@(0.0.255) int]_[* Flush]()&]
[s2;%RU-RU Выводит буфер в консоль.&]
[s3; &]
[s4; &]
[s5;:Console`:`:Execute`(Upp`:`:One`<Upp`:`:AProcess`>`,const char`*`,Stream`*`,bool`): [@(0.0.255) i
nt]_[* Execute]([_^Upp`:`:One^ One]<[_^Upp`:`:AProcess^ AProcess]>_[*@3 process], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 cmdline], [_^Stream^ Stream]_`*[*@3 out]_`=_NUL
L, [@(0.0.255) bool]_[*@3 quiet]_`=_[@(0.0.255) false])&]
[s2;%RU-RU Выполняет процесс в консоли с 
названием [%-*@3 process] и командной строкой 
[%-*@3 cmdline.] [%-*@3 out ][%- может быть указан как 
поток вывода,] [%-*@3 quiet ]`-если установлен, 
то команда в консоли не видна.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Console`:`:Execute`(const char`*`,Stream`*`,const char`*`,bool`,bool`): [@(0.0.255) i
nt]_[* Execute]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 cmdline], 
[_^Stream^ Stream]_`*[*@3 out]_`=_NULL, [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 envptr]_
`=_NULL, [@(0.0.255) bool]_[*@3 quiet]_`=_[@(0.0.255) false], [@(0.0.255) bool]_[*@3 noconv
ert]_`=_[@(0.0.255) false])&]
[s2;%RU-RU  Выполняет командную строку [%-*@3 cmdline] 
[%-*@3 out] [%-*@3 envptr] [%-*@3 quiet] [%-*@3 noconvert] .&]
[s3;%RU-RU &]
[s4; &]
[s5;:Console`:`:AllocSlot`(`): [@(0.0.255) int]_[* AllocSlot]()&]
[s2;%RU-RU Размещает в памяти слот, возвращая 
его номер &]
[s3; &]
[s4; &]
[s5;:Console`:`:Run`(const char`*`,Stream`*`,const char`*`,bool`,int`,String`,int`): [@(0.0.255) b
ool]_[* Run]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 cmdline], [_^Stream^ Stream]_`*[*@3 o
ut]_`=_NULL, [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 endptr]_`=_NULL, 
[@(0.0.255) bool]_[*@3 quiet]_`=_[@(0.0.255) false], [@(0.0.255) int]_[*@3 slot]_`=_[@3 0], 
[_^String^ String]_[*@3 key]_`=_Null, [@(0.0.255) int]_[*@3 blitz`_count]_`=_[@3 1])&]
[s2;%RU-RU Создаёт и выполняет процесс [%-*@3 cmdline] 
[%-*@3 out] [%-*@3 endptr] [%-*@3 quiet] [%-*@3 slot] [%-*@3 key] [%-*@3 blitz`_count] 
.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Console`:`:Run`(Upp`:`:One`<Upp`:`:AProcess`>`,const char`*`,Stream`*`,bool`,int`,String`,int`): [@(0.0.255) b
ool]_[* Run]([_^Upp`:`:One^ One]<[_^Upp`:`:AProcess^ AProcess]>_[*@3 process], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 cmdline], [_^Stream^ Stream]_`*[*@3 out]_`=_NUL
L, [@(0.0.255) bool]_[*@3 quiet]_`=_[@(0.0.255) false], [@(0.0.255) int]_[*@3 slot]_`=_[@3 0],
 [_^String^ String]_[*@3 key]_`=_Null, [@(0.0.255) int]_[*@3 blitz`_count]_`=_[@3 1])&]
[s2;%RU-RU  Создаёт и выполняет процесс [%-*@3 process] 
[%-*@3 cmdline] [%-*@3 out] [%-*@3 quiet] [%-*@3 slot] [%-*@3 key] [%-*@3 blitz`_count] 
.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Console`:`:operator`<`<`(const String`&`): [_^Console^ Console][@(0.0.255) `&]_[* oper
ator<<]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 s])&]
[s2;%RU-RU  [%-*@3 s] .&]
[s3;%RU-RU &]
[s4; &]
[s5;:Console`:`:OnFinish`(Upp`:`:Event`<`>`): [@(0.0.255) void]_[* OnFinish]([_^Upp`:`:Event^ E
vent]<>_[*@3 cb])&]
[s2;%RU-RU Возбуждает событие по завершении 
своей работы [%-*@3 cb] .&]
[s3;%RU-RU &]
[s4; &]
[s5;:Console`:`:WrapText`(bool`): [@(0.0.255) void]_[* WrapText]([@(0.0.255) bool]_[*@3 w])&]
[s2;%RU-RU Подогнать текст под длину вывода 
(перенести на другую строку) или нет, 
в зависимости от [%-*@3 w] .&]
[s3;%RU-RU &]
[s4; &]
[s5;:Console`:`:FlushConsole`(`): [@(0.0.255) void]_[* FlushConsole]()&]
[s2;%RU-RU Сливает в консоль буфер с командой.&]
[s3; &]
[s4; &]
[s5;:Console`:`:GetSlotCount`(`)const: [@(0.0.255) int]_[* GetSlotCount]()_[@(0.0.255) cons
t]&]
[s2;%RU-RU Выводит число слотов.&]
[s3; &]
[s4; &]
[s5;:Console`:`:IsRunning`(`): [@(0.0.255) bool]_[* IsRunning]()&]
[s2;%RU-RU Проверяет, выполняется ли в данный 
момент какой`-л. процесс.&]
[s3; &]
[s4; &]
[s5;:Console`:`:IsRunning`(int`): [@(0.0.255) bool]_[* IsRunning]([@(0.0.255) int]_[*@3 slot])
&]
[s2;%RU-RU Проверяет, выполняется ли в данный 
момент какой`-л. процесс. на указанном 
слоте [%-*@3 slot] .&]
[s3;%RU-RU &]
[s4; &]
[s5;:Console`:`:Wait`(int`): [@(0.0.255) void]_[* Wait]([@(0.0.255) int]_[*@3 slot])&]
[s2;%RU-RU Ждать завершения процесса в слоте 
[%-*@3 slot] .&]
[s3;%RU-RU &]
[s4; &]
[s5;:Console`:`:Wait`(`): [@(0.0.255) bool]_[* Wait]()&]
[s2;%RU-RU Ждать завершения всех процессов&]
[s3; &]
[s4; &]
[s5;:Console`:`:Kill`(`): [@(0.0.255) void]_[* Kill]()&]
[s2;%RU-RU Потушить все выполняемые процессы.&]
[s3; &]
[s4; &]
[s5;:Console`:`:Kill`(int`): [@(0.0.255) void]_[* Kill]([@(0.0.255) int]_[*@3 slot])&]
[s2;%RU-RU Потушить процесс в слоте [%-*@3 slot] 
.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Console`:`:SetSlots`(int`): [@(0.0.255) void]_[* SetSlots]([@(0.0.255) int]_[*@3 s])&]
[s2;%RU-RU Устанавливает количество слотов 
в [%-*@3 s] .&]
[s3;%RU-RU &]
[s4; &]
[s5;:Console`:`:CheckEndGroup`(`): [@(0.0.255) void]_[* CheckEndGroup]()&]
[s2;%RU-RU Проверяет окончание работы группы 
процессов.&]
[s3; &]
[s4; &]
[s5;:Console`:`:Input`(bool`): [@(0.0.255) void]_[* Input]([@(0.0.255) bool]_[*@3 b])&]
[s2;%RU-RU Устанавливает фокус на вводе 
в консоль или снимает его, в зависимости 
от [%-*@3 b] .&]
[s3;%RU-RU &]
[s0;%RU-RU ]]