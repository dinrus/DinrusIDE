topic "Классы String и WString";
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_}%RU-RU 
[ {{10000@(113.42.0) [s0; [*@7;4 Класс String]]}}&]
[s3; &]
[s1;:String`:`:class:%- [@(0.0.255)3 class][3 _][*3 String][3 _:_][@(0.0.255)3 public][3 _][*@3;3 Mo
veable][@(0.0.255)3 <][*3 String][3 , ][_^AString^3 AString][@(0.0.255)3 <][_^String0^3 Strin
g0][@(0.0.255)3 >][3 _>_]&]
[s9; String `- это класс значения, содержащий 
массив символов (или байты). Он разработан 
для операций быстрого копирования. 
Массив символов завершается нулём. 
String предоставляет доступ к массиву 
символов без прав его изменения. String 
может содержать строку, закодированную 
8`-битно или в кодировке UTF`-8. Для строк 
UTF`-8, однако, String работает с сырыми 
8`-битными значениями `- автоматического 
преобразования не выполняется, напр., 
в операторе`[`]. Если требуется доступ 
к отдельным символам UTF`-8, лучше всего 
преобразовать строку в WString (и, возможно, 
обратно, после обработки).&]
[s9; String также часто используется для 
хранения сырых двоичных значений.&]
[s9; WString подобен String, но использует 16`-битную 
кодировку Ucs2. String и WString разделяют 
ряд общих функций через интерфейс 
AString.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Детали Конструктора]]}}&]
[s3;%- &]
[s5;:String`:`:String`(`):%- [* String]()&]
[s2; Дефолтный конструктор. Конструирует 
пустой класс [* String].&]
[s3;%- &]
[s4;%- &]
[s5;:String`:`:String`(const String`&`):%- [* String]([@(0.0.255) const]_[* String][@(0.0.255) `&
]_[*@3 s])&]
[s2; Дефолтный копи`-конструктор.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:String`:`:String`(Upp`:`:String`&`&`):%- [* String]([* String][@(0.0.255) `&`&]_
[*@3 s])&]
[s2; Пик`-конструктор.&]
[s3; &]
[s4;%- &]
[s5;:String`:`:String`(const char`*`):%- [* String]([@(0.0.255) const]_[@(0.0.255) char]_`*
[*@3 s])&]
[s2; Создаёт копию строки с нулевым окончанием 
[%-*@3 s].&]
[s3; &]
[s4;%- &]
[s5;:String`:`:String`(const char`*`,int`):%- [* String]([@(0.0.255) const]_[@(0.0.255) cha
r]_`*[*@3 s], [@(0.0.255) int]_[*@3 n])&]
[s2; Конструирует строку, равную первым 
[%-*@3 n] символам в строке [%-*@3 s]. Включая 
символы нуля.&]
[s3; &]
[s4;%- &]
[s5;:String`:`:String`(const String`&`,int`):%- [* String]([@(0.0.255) const]_[* String][@(0.0.255) `&
]_[*@3 s], [@(0.0.255) int]_[*@3 n])&]
[s2; Конструирует строку, равную первым 
[%-*@3 n] символам в String [%-*@3 s]. Включая символы 
нуля.&]
[s3; &]
[s4;%- &]
[s5;:String`:`:String`(const byte`*`,int`):%- [* String]([@(0.0.255) const]_[_^byte^ byte]_
`*[*@3 s], [@(0.0.255) int]_[*@3 n])&]
[s2; Конструирует строку, равную первым 
[%-*@3 n] байтам в строке [%-*@3 s]. Включая 
символы нуля.&]
[s3; &]
[s4;%- &]
[s5;:String`:`:String`(const char`*`,const char`*`):%- [* String]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 s], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 lim])&]
[s2; Конструирует строку, равную первым 
[%-*@3 lim] `- [%-*@3 s] символам в [%-*@3 s]. Включая 
символы нуля.&]
[s3; &]
[s4;%- &]
[s5;:String`:`:String`(int`,int`):%- [* String]([@(0.0.255) int]_[*@3 chr], 
[@(0.0.255) int]_[*@3 count])&]
[s2; Конструирует строку, состоящую из 
[%-*@3 count] символов, равную [%-*@3 chr]. [%-*@3 chr] 
может быть нулём.&]
[s3; &]
[s4;%- &]
[s5;:String`:`:String`(StringBuffer`&`):%- [* String]([_^StringBuffer^ StringBuffer][@(0.0.255) `&
]_[*@3 b])&]
[s2; Конструирует строку из [%-*@3 b]. [%-*@3 b] 
этой операцией опустошается.&]
[s3; &]
[s4;%- &]
[s5;:String`:`:String`(const Nuller`&`):%- [* String]([@(0.0.255) const]_[_^Nuller^ Nuller][@(0.0.255) `&
])&]
[s2; Конструирует пустой [* String]. Этот вариант 
позволяет присвоить строке [* Null ](что 
равносильно присваивнию пустого 
[* String]).&]
[s3;%- &]
[s4;%- &]
[s5;:String`:`:String`(const string`&`):%- [* String]([@(0.0.255) const]_[_^http`:`/`/en`.cppreference`.com`/w`/cpp`/string`/basic`_string^ s
td`::string][@(0.0.255) `&]_[*@3 s])&]
[s2; Конструктор для достижения минимальной 
совместимости с STL.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Список Публичных Членов]]}}&]
[s3;%- &]
[s5;:String`:`:Shrink`(`):%- [@(0.0.255) void]_[* Shrink]()&]
[s2; Уменьшает используемую строкой память 
до минимума (оптимизация размера).&]
[s3; &]
[s4;%- &]
[s5;:String`:`:GetCharCount`(`)const:%- [@(0.0.255) int]_[* GetCharCount]()_[@(0.0.255) con
st]&]
[s2; Возвращает число символов в String. 
Равен GetCount(), если дефолтный набор 
символов не UTF`-8, а при UTF`-8 возвращает 
число кодточек Юникода (unicode codepoints). 
Это более быстрый эквивалент ToWString().GetCount().&]
[s3;%- &]
[s4;%- &]
[s5;:String`:`:ToWString`(`)const:%- [_^WString^ WString]_[* ToWString]()_[@(0.0.255) const
]&]
[s2; Преобразует String в WString, используя 
текущий дефолтный набор символов.&]
[s3;%- &]
[s4;%- &]
[s5;:String`:`:ToString`(`)const:%- [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[* T
oString]()_[@(0.0.255) const]&]
[s2; Возвращает `*this. Является стандартным 
методом для преобразования конкретного 
типа в текст, его использует шаблон 
AsString (а также, следовательно, оператор<<).&]
[s3;%- &]
[s4;%- &]
[s5;:String`:`:GetVoid`(`):%- [@(0.0.255) static] [_^String^ String]_[* GetVoid]()&]
[s2; Возвращает особое значение Void. Это 
значение то же, что и пустая строка, 
но для него метод IsVoid возвращает true. 
Используется в особых сценариях, 
когда код`-клиент должен различать 
два пустых значения (скажем, что Void 
`"более пуст`"...). Например, LoadFile возвращает 
строку Void, если не удалось загрузить 
файл, а нормальную пустую строку, 
если он пустой.&]
[s3;%- &]
[s4;%- &]
[s5;:String`:`:IsVoid`(`)const:%- [@(0.0.255) bool]_[* IsVoid]()_[@(0.0.255) const]&]
[s2; Возвращает true, если String является 
особым значением, возвращённым от 
[* GetVoid].&]
[s3;%- &]
[s4;%- &]
[s5;:String0`:`:IsEqual`(const String0`&`)const:%- [@(0.0.255) bool]_[* IsEqual]([@(0.0.255) c
onst]_[_^String0^ String][@(0.0.255) `&]_[*@3 s])_[@(0.0.255) const]&]
[s2; Возвращает true, если строка равна 
[%-*@3 s].&]
[s3; &]
[s4;%- &]
[s5;:String0`:`:Compare`(const String0`&`)const:%- [@(0.0.255) int]_[* Compare]([@(0.0.255) c
onst]_[_^String0^ String0][@(0.0.255) `&]_[*@3 s])_[@(0.0.255) const]&]
[s2; Лексикографическое сравнение, возвращает 
`-1, если эта строка меньше, 0 `- равна, 
1 `- больше. Отдельные символы сравниваются 
как беззначные целочисленные значения.&]
[s3; &]
[s4;%- &]
[s5;:String0`:`:GetHashValue`(`)const:%- [@(0.0.255) unsigned]_[* GetHashValue]()_[@(0.0.255) c
onst]&]
[s2; Возвращает хэш`-значение данной строки.&]
[s3;%- &]
[s4;%- &]
[s5;:String0`:`:Cat`(int`):%- [@(0.0.255) void]_[* Cat]([@(0.0.255) int]_[*@3 c])&]
[s5;:String`:`:operator`+`=`(char`):%- [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_
[* operator`+`=]([@(0.0.255) char]_[*@3 c])&]
[s2; Приставляет единичный символ [%-*@3 c]. 
У этой операции константное аммортизированн
ое время (другими словами, используемое 
реализацией внутреннее пространство 
растёт экспоненциально, наподобие 
как с Vector или std`::vector). [%-*@3 c] может быть 
нулём.&]
[s3; &]
[s4;%- &]
[s5;:String0`:`:Cat`(const char`*`,int`):%- [@(0.0.255) void]_[* Cat]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 s], [@(0.0.255) int]_[*@3 len])&]
[s2;%- [%RU-RU Приставляет ][*@3 len][%RU-RU  символов 
из строки ][*@3 s] ([*@3 s ]может содержать 
символы нуля).&]
[s3; &]
[s4;%- &]
[s5;:String0`:`:Set`(int`,int`):%- [@(0.0.255) void]_[* Set]([@(0.0.255) int]_[*@3 i], 
[@(0.0.255) int]_[*@3 chr])&]
[s2; Заменяет символ в позиции [%-*@3 i] на 
[%-*@3 chr].&]
[s3; &]
[s4;%- &]
[s5;:String0`:`:Trim`(int`):%- [@(0.0.255) void]_[* Trim]([@(0.0.255) int]_[*@3 pos])&]
[s2; Устанавливает число символов (длину) 
в [%-*@3 pos].&]
[s3; &]
[s4;%- &]
[s5;:String0`:`:Begin`(`)const:%- [@(0.0.255) const]_[@(0.0.255) char]_`*[* Begin]()_[@(0.0.255) c
onst]&]
[s2; Возвращает указатель на первый символ.&]
[s3;%- &]
[s4;%- &]
[s5;:String0`:`:End`(`)const:%- [@(0.0.255) const]_[@(0.0.255) char]_`*[* End]()_[@(0.0.255) c
onst]&]
[s2; Возвращает указатель на завершающий 
нуль.&]
[s3;%- &]
[s4;%- &]
[s5;:String0`:`:Remove`(int`,int`):%- [@(0.0.255) void]_[* Remove]([@(0.0.255) int]_[*@3 pos],
 [@(0.0.255) int]_[*@3 count]_`=_[@3 1])&]
[s2; Удаляет [%-*@3 count] элементов, начиная 
с позиции [%-*@3 pos].&]
[s3; &]
[s4;%- &]
[s5;:String0`:`:Clear`(`):%- [@(0.0.255) void]_[* Clear]()&]
[s2; Опорожняет строку.&]
[s3;%- &]
[s4;%- &]
[s5;:String0`:`:GetCount`(`)const:%- [@(0.0.255) int]_[* GetCount]()_[@(0.0.255) const]&]
[s5;:String0`:`:GetLength`(`)const:%- [@(0.0.255) int]_[* GetLength]()_[@(0.0.255) const]&]
[s2; Возвращает число символов.&]
[s3;%- &]
[s4;%- &]
[s5;:String0`:`:GetAlloc`(`)const:%- [@(0.0.255) int]_[* GetAlloc]()_[@(0.0.255) const]&]
[s2; Возвращает число размещённых символов 
(максимальную длину строки до её наращивания
) &]
[s3;%- &]
[s4;%- &]
[s5;:String0`:`:Reserve`(int`):%- [@(0.0.255) void]_[* Reserve]([@(0.0.255) int]_[*@3 r])&]
[s2; Резервирует [%-*@3 r] символов внутреннего 
хранилища для последующих операций 
конкатенации.&]
[s3; &]
[s4;%- &]
[s5;:String`:`:operator`+`=`(const char`*`):%- [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[* operator`+`=]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s])&]
[s5;:String`:`:operator`+`=`(const String`&`):%- [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[* operator`+`=]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 s])&]
[s2; Приставляет строку с нулевым окончанием 
[%-*@3 s].&]
[s3; &]
[s4;%- &]
[s5;:String`:`:operator`=`(const String`&`):%- [_^String^ String][@(0.0.255) `&]_[* operato
r`=]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 s])&]
[s2; Присваивает другую строку [%-*@3 s]. Эта 
операция относительно быстрая и не 
зависит от длины строки.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:String`:`:operator`=`(Upp`:`:String`&`&`):%- [_^Upp`:`:String^ String][@(0.0.255) `&
]_[* operator`=]([_^Upp`:`:String^ String][@(0.0.255) `&`&]_[*@3 s])&]
[s2; Пик`-присваивание.&]
[s3; &]
[s4;%- &]
[s5;:String`:`:operator`=`(const char`*`):%- [_^String^ String][@(0.0.255) `&]_[* operator`=
]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s])&]
[s2; Присваивает строку с нулевым окончанием 
[%-*@3 s].&]
[s3; &]
[s4;%- &]
[s5;:String`:`:operator`=`(StringBuffer`&`):%- [_^String^ String][@(0.0.255) `&]_[* operato
r`=]([_^StringBuffer^ StringBuffer][@(0.0.255) `&]_[*@3 b])&]
[s2; Присваивает содержимое [* StringBuffer]. 
После операции b опорожняется.&]
[s3; &]
[s4;%- &]
[s5;:String`:`:operator`<`<`=`(const String`&`):%- [_^String^ String][@(0.0.255) `&]_[* ope
rator<<`=]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 s])&]
[s2; `"Глубокое`" присваивание. Эквивалентно 
стандартному, за которым следует 
операция сжатия [* Shrink] (другими словами, 
внутренний буфер реаллоцируется 
под точны размер источника).&]
[s3;%- &]
[s4;%- &]
[s5;:String`:`:ToStd`(`)const:%- [_^http`:`/`/en`.cppreference`.com`/w`/cpp`/string`/basic`_string^ s
td`::string]_[* ToStd]()_[@(0.0.255) const]&]
[s2; Преобразует String в std`::string.&]
[s3; &]
[s4;%- &]
[s5;:String0`:`:operator const char`*`(`)const:%- [@(0.0.255) operator]_[@(0.0.255) const
]_[@(0.0.255) char]_`*()_[@(0.0.255) const]&]
[s5;:String0`:`:operator`~`(`)const:%- [@(0.0.255) const]_[@(0.0.255) char]_`*[* operator`~
]()_[@(0.0.255) const]&]
[s2; Возвращает [* Begin]().&]
[s3;%- &]
[s4;%- &]
[s5;:String0`:`:operator`[`]`(int`)const:%- [@(0.0.255) int]_[* operator`[`]]([@(0.0.255) i
nt]_[*@3 i])_[@(0.0.255) const]&]
[s2;%- [%RU-RU Возвращает символ в позиции 
][*@3 i.]&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:String`:`:Make`(int`,Maker`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 M
aker]>_[@(0.0.255) static]_[_^Upp`:`:String^ String]_[* Make]([@(0.0.255) int]_[*@3 alloc],
 [*@4 Maker]_[*@3 m])&]
[s2; Оптимизированный статический метод 
для создания String`'ов. Создаёт внутренний 
буфер размером минимум в [%-*@3 alloc], затем 
вызывает лямбду [%-*@3 m], передавая в 
качестве параметра лямбды указатель 
char `* на внутренний буфер. Далее предполагает
ся, что лямбда заполняет этот буфер 
символами и возвращает длину строки 
(которая должна быть  <`= [%-*@3 alloc]). Для 
лучшей производительности, [%-*@3 alloc] 
должен быть константным.&]
[s0;%- &]
[s0;%- &]
[ {{10000@(113.42.0) [s0; [*@7;4 Класс WString]]}}&]
[s3; &]
[s1;:WString`:`:class:%- [@(0.0.255)3 class][3 _][*3 WString][3 _:_][@(0.0.255)3 public][3 _][*@3;3 M
oveable][@(0.0.255)3 <][*3 WString][3 , ][_^AString^3 AString][@(0.0.255)3 <][_^WString0^3 WS
tring0][@(0.0.255)3 >][3 _>_]&]
[s9; WString подобен String, но использует 16`-битную 
кодировку Ucs2. String и WString разделяют 
ряд общих функций через интерфейс 
AString.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Детали Конструктора]]}}&]
[s3; &]
[s5;:WString`:`:WString`(`):%- [* WString]()&]
[s2; Дефолтный конструктор. Constructs empty 
[* WString].&]
[s3;%- &]
[s4;%- &]
[s5;:WString`:`:WString`(const WString`&`):%- [* WString]([@(0.0.255) const]_[* WString][@(0.0.255) `&
]_[*@3 s])&]
[s2; Дефолтный копи`-конструктор.&]
[s3; &]
[s4;%- &]
[s5;:WString`:`:WString`(const wchar`*`):%- [* WString]([@(0.0.255) const]_[_^wchar^ wchar]_
`*[*@3 s])&]
[s2; Создаёт копию строки с нулевым окончанием 
[%-*@3 s].&]
[s3; &]
[s4;%- &]
[s5;:WString`:`:WString`(const char`*`):%- [* WString]([@(0.0.255) const]_[@(0.0.255) char]_
`*[*@3 s])&]
[s2; Создаёт копию строки с нулевым окончанием 
[%-*@3 s].&]
[s3; &]
[s4;%- &]
[s5;:WString`:`:WString`(const WString`&`,int`):%- [* WString]([@(0.0.255) const]_[* WStrin
g][@(0.0.255) `&]_[*@3 s], [@(0.0.255) int]_[*@3 n])&]
[s2;%- [%RU-RU Конструирует строку, равную 
первым ][*@3 n][%RU-RU  символам из ][%RU-RU* WString][%RU-RU  
][*@3 s][%RU-RU  ]([*@3 s ]может содержать символы 
нуля).&]
[s3; &]
[s4;%- &]
[s5;:WString`:`:WString`(const wchar`*`,int`):%- [* WString]([@(0.0.255) const]_[_^wchar^ w
char]_`*[*@3 s], [@(0.0.255) int]_[*@3 n])&]
[s2;%- [%RU-RU Конструирует строку, равную 
первым ][*@3 n][%RU-RU  символам из ][*@3 s ]([*@3 s 
]может содержать символы нуля).&]
[s3; &]
[s4;%- &]
[s5;:WString`:`:WString`(const wchar`*`,const wchar`*`):%- [* WString]([@(0.0.255) const]_
[_^wchar^ wchar]_`*[*@3 s], [@(0.0.255) const]_[_^wchar^ wchar]_`*[*@3 lim])&]
[s2;%- [%RU-RU Конструирует строку, равную 
первым ][*@3 lim][%RU-RU  `- ][*@3 s ][%RU-RU символам 
][*@3 s][%RU-RU  ]([*@3 s ]может содержать символы 
нуля).&]
[s3; &]
[s4;%- &]
[s5;:WString`:`:WString`(const char`*`,int`):%- [* WString]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 s], [@(0.0.255) int]_[*@3 n])&]
[s2;%- [%RU-RU Конструирует строку, равную 
первым ][*@3 n][%RU-RU  символам строки с нулевым 
окончанием ][*@3 s ]([*@3 s ]может содержать 
символы нуля).&]
[s3; &]
[s4;%- &]
[s5;:WString`:`:WString`(const char`*`,const char`*`):%- [* WString]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 s], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 lim])&]
[s2;%- [%RU-RU Конструирует строку, равную 
первым ][*@3 n][%RU-RU  символам строки с нулевым 
окончанием ][*@3 s ]([*@3 s ]может содержать 
символы нуля).&]
[s3; &]
[s4;%- &]
[s5;:WString`:`:WString`(int`,int`):%- [* WString]([@(0.0.255) int]_[*@3 chr], 
[@(0.0.255) int]_[*@3 count])&]
[s2; Конструирует строку, состоящую из 
[%-*@3 count] символов [%-*@3 chr]. [%-*@3 chr] может 
быть нулём.&]
[s3; &]
[s4;%- &]
[s5;:WString`:`:WString`(WStringBuffer`&`):%- [* WString]([_^WStringBuffer^ WStringBuffer
][@(0.0.255) `&]_[*@3 b])&]
[s2; Конструирует строку из [%-*@3 b]. [%-*@3 b] 
опустошается при этой операции.&]
[s3; &]
[s4;%- &]
[s5;:WString`:`:WString`(const Nuller`&`):%- [* WString]([@(0.0.255) const]_[_^Nuller^ Null
er][@(0.0.255) `&])&]
[s2; Конструирует пустую [* WString]. Этот вариант 
позволяет присваивать [* Null]  [* WString] (что 
равносильно присвоению пустой [* WString]).&]
[s3;%- &]
[s4;%- &]
[s5;:WString`:`:WString`(const wstring`&`):%- [* WString]([@(0.0.255) const]_std`::wstring
[@(0.0.255) `&]_[*@3 s])&]
[s2; Конструктор для минимальной совместимости 
с STL.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Список Публичных Членов]]}}&]
[s3; &]
[s5;:WString`:`:Shrink`(`):%- [@(0.0.255) void]_[* Shrink]()&]
[s2; Снижает до минимума память, используемую 
строкой (оптимизация размера).&]
[s3;%- &]
[s4;%- &]
[s5;:WString`:`:ToString`(`)const:%- [_^String^ String]_[* ToString]()_[@(0.0.255) const]&]
[s2; Преобразует [* WString ]в [* String], используя 
текущий дефолтный набор символов. 
Также служит в качестве стандартного 
преобразования текста (для шаблонов 
[* AsString] и [* operator<<]).&]
[s3;%- &]
[s4;%- &]
[s5;:WString`:`:GetVoid`(`):%- [@(0.0.255) static] [_^WString^ WString]_[* GetVoid]()&]
[s2; Возвращает особое значение Void. Это 
значение то же, что и пустая строка, 
но для него метод IsVoid возвращает true. 
Используется в особых сценариях, 
когда код`-клиент должен различать 
два пустых значения (скажем, что Void 
`"более пуст`"...). Например, LoadFile возвращает 
строку Void, если не удалось загрузить 
файл, а нормальную пустую строку, 
если он пустой.&]
[s3;%- &]
[s4;%- &]
[s5;:WString`:`:IsVoid`(`)const:%- [@(0.0.255) bool]_[* IsVoid]()_[@(0.0.255) const]&]
[s2; Возвращает true, если String является 
особым значением, возвращаемым [* GetVoid].&]
[s3;%- &]
[s4;%- &]
[s5;:WString0`:`:Begin`(`)const:%- [@(0.0.255) const]_[_^wchar^ wchar]_`*[* Begin]()_[@(0.0.255) c
onst]&]
[s2; Возвращает указатель на первый символ.&]
[s3;%- &]
[s4;%- &]
[s5;:WString0`:`:End`(`)const:%- [@(0.0.255) const]_[_^wchar^ wchar]_`*[* End]()_[@(0.0.255) c
onst]&]
[s2; Возвращает указатель на завершающий 
ноль.&]
[s3;%- &]
[s4;%- &]
[s5;:WString0`:`:GetCount`(`)const:%- [@(0.0.255) int]_[* GetCount]()_[@(0.0.255) const]&]
[s5;:WString0`:`:GetLength`(`)const:%- [@(0.0.255) int]_[* GetLength]()_[@(0.0.255) const]&]
[s2; Возвращает число символов.&]
[s3;%- &]
[s4;%- &]
[s5;:WString0`:`:GetAlloc`(`)const:%- [@(0.0.255) int]_[* GetAlloc]()_[@(0.0.255) const]&]
[s2; Возвращает число размещённых символов 
(максимальную длину строки до наращивания) 
&]
[s3;%- &]
[s4;%- &]
[s5;:WString0`:`:Cat`(int`):%- [@(0.0.255) void]_[* Cat]([@(0.0.255) int]_[*@3 c])&]
[s2; Приставляет единичный символ [%-*@3 c]. 
У операции константное амортизированное 
время (иначе говоря, внутреннее пространство
, используемое реализацией, растёт 
экпоненциально, как в случаях с Vector 
или std`::vector).&]
[s3; &]
[s4;%- &]
[s5;:WString0`:`:Cat`(const wchar`*`,int`):%- [@(0.0.255) void]_[* Cat]([@(0.0.255) const]_
[_^wchar^ wchar]_`*[*@3 s], [@(0.0.255) int]_[*@3 length])&]
[s2;%- [%RU-RU Приставляет ][*@3 len][%RU-RU  символов 
из строки ][*@3 s] ([*@3 s ]может содержать 
символы нуля).&]
[s3; &]
[s4;%- &]
[s5;:WString0`:`:GetHashValue`(`)const:%- [@(0.0.255) hash`_t]_[* GetHashValue]()_[@(0.0.255) c
onst]&]
[s2; Возвращает хэш`-значение строки.&]
[s3;%- &]
[s4;%- &]
[s5;:WString0`:`:IsEqual`(const WString0`&`)const:%- [@(0.0.255) bool]_[* IsEqual]([@(0.0.255) c
onst]_[_^WString0^ WString][@(0.0.255) `&]_[*@3 s])_[@(0.0.255) const]&]
[s2; Возвращает true, если строка равна 
[%-*@3 s].&]
[s3; &]
[s4;%- &]
[s5;:WString0`:`:Compare`(const WString0`&`)const:%- [@(0.0.255) int]_[* Compare]([@(0.0.255) c
onst]_[_^WString0^ WString][@(0.0.255) `&]_[*@3 s])_[@(0.0.255) const]&]
[s2; Лексикографическое сравнение, возвращает 
`-1, если эта строка меньше, 0 `- равна, 
1 `- больше. Отдельные символы сравниваются 
как беззначные целочисленные значения.&]
[s3; &]
[s4;%- &]
[s5;:WString0`:`:Remove`(int`,int`):%- [@(0.0.255) void]_[* Remove]([@(0.0.255) int]_[*@3 pos
], [@(0.0.255) int]_[*@3 count]_`=_[@3 1])&]
[s2; Удаляет [%-*@3 count] элементов, начиная 
с позиции [%-*@3 pos].&]
[s3; &]
[s4;%- &]
[s5;:WString0`:`:Insert`(int`,const wchar`*`,int`):%- [@(0.0.255) void]_[* Insert]([@(0.0.255) i
nt]_[*@3 pos], [@(0.0.255) const]_[_^wchar^ wchar]_`*[*@3 s], [@(0.0.255) int]_[*@3 count])&]
[s2; Вставляет первые [%-*@3 count] символов 
[%-*@3 s] в позиции [%-*@3 pos].&]
[s3; &]
[s4;%- &]
[s5;:WString0`:`:Clear`(`):%- [@(0.0.255) void]_[* Clear]()&]
[s2; Опустошает строку.&]
[s3;%- &]
[s4;%- &]
[s5;:WString0`:`:Set`(int`,int`):%- [@(0.0.255) void]_[* Set]([@(0.0.255) int]_[*@3 pos], 
[@(0.0.255) int]_[*@3 ch])&]
[s2; Заменяет символ в позиции [%-*@3 pos] на 
[%-*@3 ch].&]
[s3; &]
[s4;%- &]
[s5;:WString0`:`:Trim`(int`):%- [@(0.0.255) void]_[* Trim]([@(0.0.255) int]_[*@3 pos])&]
[s2; Устанавливает число символов (длину) 
в [%-*@3 pos].&]
[s3;%- &]
[s4;%- &]
[s5;:WString`:`:ToStd`(`)const:%- [_^http`:`/`/en`.cppreference`.com`/w`/cpp`/string`/basic`_string^ s
td`::wstring]_[* ToStd]()_[@(0.0.255) const]&]
[s2; Преобразует WString в std`::string.&]
[s0;%- &]
[s0;%- &]
[ {{10000@(113.42.0) [s0; [*@7;4 Класс AString]]}}&]
[s3; &]
[s1;:noref:%- [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@3;3 B][@(0.0.255)3 >]&]
[s1;:AString`:`:class:%- [@(0.0.255) class]_[* AString]_:_[@(0.0.255) public]_[*@3 B]&]
[s9; Класс, реализующий поведение, общее 
для String и WString.&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Список Публичных Членов]]}}&]
[s3;%- &]
[s5;:AString`:`:Clear`(`):%- [@(0.0.255) void]_[* Clear]()&]
[s2; Очищает контент строки.&]
[s3;%- &]
[s4;%- &]
[s5;:AString`:`:GetLength`(`)const:%- [@(0.0.255) int]_[* GetLength]()_[@(0.0.255) const]&]
[s2; Возвращает длину строки.&]
[s3;%- &]
[s4;%- &]
[s5;:AString`:`:IsEmpty`(`)const:%- [@(0.0.255) bool]_[* IsEmpty]()_[@(0.0.255) const]&]
[s2; Возвращает true, если длина строки 
равна нулю.&]
[s3;%- &]
[s4;%- &]
[s5;:AString`:`:End`(`)const:%- [@(0.0.255) const]_[_^AString`:`:tchar^ tchar]_`*[* End]()_
[@(0.0.255) const]&]
[s2; Возвращает указатель на конец строки.&]
[s3;%- &]
[s4;%- &]
[s5;:AString`:`:Last`(`)const:%- [@(0.0.255) const]_[_^AString`:`:tchar^ tchar]_`*[* Last](
)_[@(0.0.255) const]&]
[s2; Возвращает укзатель на последний 
символ в строке. Если строка пустая, 
возвращает указатель на завершающий 
ноль.&]
[s3;%- &]
[s4;%- &]
[s5;:AString`:`:GetIter`(int`)const:%- [@(0.0.255) const]_[_^AString`:`:tchar^ tchar]_`*[* G
etIter]([@(0.0.255) int]_[*@3 i])_[@(0.0.255) const]&]
[s2; Возвращает указатель на символьный 
индекс [%-*@3 i ]данной строки.&]
[s3; &]
[s4;%- &]
[s5;:AString`:`:operator`[`]`(int`)const:%- [@(0.0.255) int]_[* operator`[`]]([@(0.0.255) i
nt]_[*@3 i])_[@(0.0.255) const]&]
[s2; Возвращает символ с индексом [%-*@3 i].&]
[s3; &]
[s4;%- &]
[s5;:AString`:`:operator const tchar`*`(`)const:%- [@(0.0.255) operator]_[@(0.0.255) cons
t]_tchar_`*()_[@(0.0.255) const]&]
[s5;:AString`:`:operator`~`(`)const:%- [@(0.0.255) const]_[_^AString`:`:tchar^ tchar]_`*[* o
perator`~]()_[@(0.0.255) const]&]
[s5;:AString`:`:operator const void`*`(`)const:%- [@(0.0.255) operator]_[@(0.0.255) const
]_[@(0.0.255) void]_`*()_[@(0.0.255) const]&]
[s5;:AString`:`:operator const bchar`*`(`)const:%- [@(0.0.255) operator]_[@(0.0.255) cons
t]_bchar_`*()_[@(0.0.255) const]&]
[s2; То же, что и [* Begin()].&]
[s3;%- &]
[s4;%- &]
[s5;:AString`:`:Insert`(int`,int`):%- [@(0.0.255) void]_[* Insert]([@(0.0.255) int]_[*@3 pos],
 [@(0.0.255) int]_[*@3 c])&]
[s5;:AString`:`:Insert`(int`,const AString`:`:tchar`*`,int`):%- [@(0.0.255) void]_[* Inse
rt]([@(0.0.255) int]_[*@3 pos], [@(0.0.255) const]_[_^AString`:`:tchar^ tchar]_`*[*@3 s], 
[@(0.0.255) int]_[*@3 count])&]
[s5;:AString`:`:Insert`(int`,const AString`:`:String`&`):%- [@(0.0.255) void]_[* Insert](
[@(0.0.255) int]_[*@3 pos], [@(0.0.255) const]_[_^AString`:`:String^ String][@(0.0.255) `&]_
[*@3 s])&]
[s5;:AString`:`:Insert`(int`,const char`*`):%- [@(0.0.255) void]_[* Insert]([@(0.0.255) int
]_[*@3 pos], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s])&]
[s2; Вставляет элемент [%-*@3 c]/[%-*@3 s] в позиции 
[%-*@3 pos] ([%-*@3 count] раз).&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:AString`:`:TrimLast`(int`):%- [@(0.0.255) void]_[* TrimLast]([@(0.0.255) int]_[*@3 c
ount]_`=_[@3 1])&]
[s2; Удаляет [%-*@3 count] символов с конца String. 
То же, что и Remove(GetCount() `- count).&]
[s3; &]
[s4;%- &]
[s5;:AString`:`:Mid`(int`,int`)const:%- [_^AString`:`:String^ String]_[* Mid]([@(0.0.255) i
nt]_[*@3 pos], [@(0.0.255) int]_[*@3 length])_[@(0.0.255) const]&]
[s2; Возвращает подстроку, начинающуюся 
с [%-*@3 pos] и с [%-*@3 length ]символов.&]
[s3; &]
[s4;%- &]
[s5;:AString`:`:Mid`(int`)const:%- [_^AString`:`:String^ String]_[* Mid]([@(0.0.255) int]_[*@3 p
os])_[@(0.0.255) const]&]
[s2; Возвращает подстроку, начинающуюся 
с [%-*@3 pos].&]
[s3; &]
[s4;%- &]
[s5;:AString`:`:Right`(int`)const:%- [_^AString`:`:String^ String]_[* Right]([@(0.0.255) in
t]_[*@3 count])_[@(0.0.255) const]&]
[s2; Возвращает подстроку с [%-*@3 count ]символов, 
начиная с конца строки.&]
[s3; &]
[s4;%- &]
[s5;:AString`:`:Left`(int`)const:%- [_^AString`:`:String^ String]_[* Left]([@(0.0.255) int]_
[*@3 count])_[@(0.0.255) const]&]
[s2; Возвращает подстроку с [%-*@3 count ]символов, 
начиная с начала строки.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:AString`:`:Find`(int`,const Upp`:`:AString`:`:tchar`*`,int`)const:%- [@(0.0.255) i
nt]_[* Find]([@(0.0.255) int]_[*@3 len], [@(0.0.255) const]_[_^Upp`:`:AString`:`:tchar^ tch
ar]_`*[*@3 s], [@(0.0.255) int]_[*@3 from])_[@(0.0.255) const]&]
[s5;:AString`:`:Find`(const AString`:`:tchar`*`,int`)const:%- [@(0.0.255) int]_[* Find]([@(0.0.255) c
onst]_[_^AString`:`:tchar^ tchar]_`*[*@3 s], [@(0.0.255) int]_[*@3 from]_`=_[@3 0])_[@(0.0.255) c
onst]&]
[s5;:AString`:`:Find`(const AString`:`:String`&`,int`)const:%- [@(0.0.255) int]_[* Find](
[@(0.0.255) const]_[_^AString`:`:String^ String][@(0.0.255) `&]_[*@3 s], 
[@(0.0.255) int]_[*@3 from]_`=_[@3 0])_[@(0.0.255) const]&]
[s2; Возвращает первую позицию подстроки 
[%-*@3 s], большую или равную [%-*@3 from], или 
`-1, если [%-*@3 s] не обнаружена.&]
[s3; &]
[s4;%- &]
[s5;:AString`:`:FindAfter`(const AString`:`:tchar`*`,int`)const:%- [@(0.0.255) int]_[* Fi
ndAfter]([@(0.0.255) const]_[_^AString`:`:tchar^ tchar]_`*[*@3 s], 
[@(0.0.255) int]_[*@3 from]_`=_[@3 0])_[@(0.0.255) const]&]
[s5;:AString`:`:FindAfter`(const AString`:`:String`&`,int`)const:%- [@(0.0.255) int]_[* F
indAfter]([@(0.0.255) const]_[_^AString`:`:String^ String][@(0.0.255) `&]_[*@3 s], 
[@(0.0.255) int]_[*@3 from]_`=_[@3 0])_[@(0.0.255) const]&]
[s2; Подобно Find, но, если найдено, возвращает 
позицию [*/ после] найденной подстроки. 
Другими словами, если не найдено, 
то возвращает `-1, а если найдено, то 
возвращает Find([%-*@3 s][%- , ][%-*@3 from]) `+ длина 
[%-*@3 s].&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:AString`:`:ReverseFind`(int`,const Upp`:`:AString`:`:tchar`*`,int`)const:%- [@(0.0.255) i
nt]_[* ReverseFind]([@(0.0.255) int]_[*@3 len], [@(0.0.255) const]_[_^Upp`:`:AString`:`:tchar^ t
char]_`*[*@3 s], [@(0.0.255) int]_[*@3 from])_[@(0.0.255) const]&]
[s5;:Upp`:`:AString`:`:ReverseFind`(const Upp`:`:AString`:`:tchar`*`,int`)const:%- [@(0.0.255) i
nt]_[* ReverseFind]([@(0.0.255) const]_[_^Upp`:`:AString`:`:tchar^ tchar]_`*[*@3 s], 
[@(0.0.255) int]_[*@3 from])_[@(0.0.255) const]&]
[s5;:Upp`:`:AString`:`:ReverseFind`(const Upp`:`:AString`:`:String`&`,int`)const:%- [@(0.0.255) i
nt]_[* ReverseFind]([@(0.0.255) const]_[_^Upp`:`:AString`:`:String^ String][@(0.0.255) `&
]_[*@3 s], [@(0.0.255) int]_[*@3 from])_[@(0.0.255) const]&]
[s5;:Upp`:`:AString`:`:ReverseFind`(const Upp`:`:AString`:`:tchar`*`)const:%- [@(0.0.255) i
nt]_[* ReverseFind]([@(0.0.255) const]_[_^Upp`:`:AString`:`:tchar^ tchar]_`*[*@3 s])_[@(0.0.255) c
onst]&]
[s5;:Upp`:`:AString`:`:ReverseFind`(const Upp`:`:AString`:`:String`&`)const:%- [@(0.0.255) i
nt]_[* ReverseFind]([@(0.0.255) const]_[_^Upp`:`:AString`:`:String^ String][@(0.0.255) `&
]_[*@3 s])_[@(0.0.255) const]&]
[s2; Находит [*/ последнюю] позицию [%-*@3 s], 
меньшую [%-*@3 from], или `-1, если не найдено.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:AString`:`:ReverseFindAfter`(int`,const Upp`:`:AString`:`:tchar`*`,int`)const:%- [@(0.0.255) i
nt]_[* ReverseFindAfter]([@(0.0.255) int]_[*@3 len], [@(0.0.255) const]_[_^Upp`:`:AString`:`:tchar^ t
char]_`*[*@3 s], [@(0.0.255) int]_[*@3 from])_[@(0.0.255) const]&]
[s5;:Upp`:`:AString`:`:ReverseFindAfter`(const Upp`:`:AString`:`:tchar`*`,int`)const:%- [@(0.0.255) i
nt]_[* ReverseFindAfter]([@(0.0.255) const]_[_^Upp`:`:AString`:`:tchar^ tchar]_`*[*@3 s],
 [@(0.0.255) int]_[*@3 from])_[@(0.0.255) const]&]
[s5;:Upp`:`:AString`:`:ReverseFindAfter`(const Upp`:`:AString`:`:String`&`,int`)const:%- [@(0.0.255) i
nt]_[* ReverseFindAfter]([@(0.0.255) const]_[_^Upp`:`:AString`:`:String^ String][@(0.0.255) `&
]_[*@3 s], [@(0.0.255) int]_[*@3 from])_[@(0.0.255) const]&]
[s5;:Upp`:`:AString`:`:ReverseFindAfter`(const Upp`:`:AString`:`:tchar`*`)const:%- [@(0.0.255) i
nt]_[* ReverseFindAfter]([@(0.0.255) const]_[_^Upp`:`:AString`:`:tchar^ tchar]_`*[*@3 s])
_[@(0.0.255) const]&]
[s5;:Upp`:`:AString`:`:ReverseFindAfter`(const Upp`:`:AString`:`:String`&`)const:%- [@(0.0.255) i
nt]_[* ReverseFindAfter]([@(0.0.255) const]_[_^Upp`:`:AString`:`:String^ String][@(0.0.255) `&
]_[*@3 s])_[@(0.0.255) const]&]
[s2; Подобно ReverseFind,но возвращает позицию 
[*/ после] найденной подстроки. Другими 
словами, если не найдено, возвращает 
`-1, а если найдено, то возвращает Find([%-*@3 s][%- , 
][%-*@3 from]) `+ длина [%-*@3 s].&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:AString`:`:Replace`(const Upp`:`:AString`:`:tchar`*`,int`,const Upp`:`:AString`:`:tchar`*`,int`):%- [@(0.0.255) v
oid]_[* Replace]([@(0.0.255) const]_[_^Upp`:`:AString`:`:tchar^ tchar]_`*[*@3 find], 
[@(0.0.255) int]_[*@3 findlen], [@(0.0.255) const]_[_^Upp`:`:AString`:`:tchar^ tchar]_`*[*@3 r
eplace], [@(0.0.255) int]_[*@3 replacelen])&]
[s5;:AString`:`:Replace`(const AString`:`:String`&`,const AString`:`:String`&`):%- [@(0.0.255) v
oid]_[* Replace]([@(0.0.255) const]_[_^AString`:`:String^ String][@(0.0.255) `&]_[*@3 find],
 [@(0.0.255) const]_[_^AString`:`:String^ String][@(0.0.255) `&]_[*@3 replace])&]
[s5;:AString`:`:Replace`(const AString`:`:tchar`*`,const AString`:`:tchar`*`):%- [@(0.0.255) v
oid]_[* Replace]([@(0.0.255) const]_[_^AString`:`:tchar^ tchar]_`*[*@3 find], 
[@(0.0.255) const]_[_^AString`:`:tchar^ tchar]_`*[*@3 replace])&]
[s5;:AString`:`:Replace`(const AString`:`:String`&`,const AString`:`:tchar`*`):%- [@(0.0.255) v
oid]_[* Replace]([@(0.0.255) const]_[_^AString`:`:String^ String][@(0.0.255) `&]_[*@3 find],
 [@(0.0.255) const]_[_^AString`:`:tchar^ tchar]_`*[*@3 replace])&]
[s5;:AString`:`:Replace`(const AString`:`:tchar`*`,const AString`:`:String`&`):%- [@(0.0.255) v
oid]_[* Replace]([@(0.0.255) const]_[_^AString`:`:tchar^ tchar]_`*[*@3 find], 
[@(0.0.255) const]_[_^AString`:`:String^ String][@(0.0.255) `&]_[*@3 replace])&]
[s2; Заменяет подстороку [%-*@3 find] на строку 
[%-*@3 replace ]всякий раз, когда находит 
[%-*@3 find].&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:AString`:`:StartsWith`(const Upp`:`:AString`:`:tchar`*`,int`)const:%- [@(0.0.255) b
ool]_[* StartsWith]([@(0.0.255) const]_[_^Upp`:`:AString`:`:tchar^ tchar]_`*[*@3 s], 
[@(0.0.255) int]_[*@3 len])_[@(0.0.255) const]&]
[s5;:Upp`:`:AString`:`:StartsWith`(const Upp`:`:AString`:`:tchar`*`)const:%- [@(0.0.255) b
ool]_[* StartsWith]([@(0.0.255) const]_[_^Upp`:`:AString`:`:tchar^ tchar]_`*[*@3 s])_[@(0.0.255) c
onst]&]
[s5;:Upp`:`:AString`:`:StartsWith`(const Upp`:`:AString`:`:String`&`)const:%- [@(0.0.255) b
ool]_[* StartsWith]([@(0.0.255) const]_[_^Upp`:`:AString`:`:String^ String][@(0.0.255) `&
]_[*@3 s])_[@(0.0.255) const]&]
[s2; Возвращает true, если строка начинается 
с [%-*@3 s].&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:AString`:`:TrimStart`(const Upp`:`:AString`:`:tchar`*`,int`):%- [@(0.0.255) b
ool]_[* TrimStart]([@(0.0.255) const]_[_^Upp`:`:AString`:`:tchar^ tchar]_`*[*@3 s], 
[@(0.0.255) int]_[*@3 len])&]
[s5;:Upp`:`:AString`:`:TrimStart`(const Upp`:`:AString`:`:tchar`*`):%- [@(0.0.255) bool
]_[* TrimStart]([@(0.0.255) const]_[_^Upp`:`:AString`:`:tchar^ tchar]_`*[*@3 s])&]
[s5;:Upp`:`:AString`:`:TrimStart`(const Upp`:`:AString`:`:String`&`):%- [@(0.0.255) boo
l]_[* TrimStart]([@(0.0.255) const]_[_^Upp`:`:AString`:`:String^ String][@(0.0.255) `&]_[*@3 s
])&]
[s2; Если строка начинается с [%-*@3 s], обрезает 
начало, удаляя ее, и возвращает true.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:AString`:`:EndsWith`(const Upp`:`:AString`:`:tchar`*`,int`)const:%- [@(0.0.255) b
ool]_[* EndsWith]([@(0.0.255) const]_[_^Upp`:`:AString`:`:tchar^ tchar]_`*[*@3 s], 
[@(0.0.255) int]_[*@3 len])_[@(0.0.255) const]&]
[s5;:Upp`:`:AString`:`:EndsWith`(const Upp`:`:AString`:`:tchar`*`)const:%- [@(0.0.255) b
ool]_[* EndsWith]([@(0.0.255) const]_[_^Upp`:`:AString`:`:tchar^ tchar]_`*[*@3 s])_[@(0.0.255) c
onst]&]
[s5;:Upp`:`:AString`:`:EndsWith`(const Upp`:`:AString`:`:String`&`)const:%- [@(0.0.255) b
ool]_[* EndsWith]([@(0.0.255) const]_[_^Upp`:`:AString`:`:String^ String][@(0.0.255) `&]_
[*@3 s])_[@(0.0.255) const]&]
[s2; Возвращает true, если строка оканчивается 
на [%-*@3 s].&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:AString`:`:TrimEnd`(const Upp`:`:AString`:`:tchar`*`,int`):%- [@(0.0.255) b
ool]_[* TrimEnd]([@(0.0.255) const]_[_^Upp`:`:AString`:`:tchar^ tchar]_`*[*@3 s], 
[@(0.0.255) int]_[*@3 len])&]
[s5;:Upp`:`:AString`:`:TrimEnd`(const Upp`:`:AString`:`:tchar`*`):%- [@(0.0.255) bool]_
[* TrimEnd]([@(0.0.255) const]_[_^Upp`:`:AString`:`:tchar^ tchar]_`*[*@3 s])&]
[s5;:Upp`:`:AString`:`:TrimEnd`(const Upp`:`:AString`:`:String`&`):%- [@(0.0.255) bool]_
[* TrimEnd]([@(0.0.255) const]_[_^Upp`:`:AString`:`:String^ String][@(0.0.255) `&]_[*@3 s])
&]
[s2; Если строка начинается с [%-*@3 s], обрезает 
конец, удаляя ее, и возвращает true.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:AString`:`:FindFirstOf`(int`,const Upp`:`:AString`:`:tchar`*`,int`)const:%- [@(0.0.255) i
nt]_[* FindFirstOf]([@(0.0.255) int]_[*@3 len], [@(0.0.255) const]_[_^Upp`:`:AString`:`:tchar^ t
char]_`*[*@3 set], [@(0.0.255) int]_[*@3 from])_[@(0.0.255) const]&]
[s2; Возвращает первую позицию любого 
символа из [%-*@3 set], начиная с [%-*@3 from]. 
Число символов в [%-*@3 set] равно [%-*@3 len]. 
Возвращает `-1, если не найдено.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:AString`:`:FindFirstOf`(const Upp`:`:AString`:`:tchar`*`,int`)const:%- [@(0.0.255) i
nt]_[* FindFirstOf]([@(0.0.255) const]_[_^Upp`:`:AString`:`:tchar^ tchar]_`*[*@3 set], 
[@(0.0.255) int]_[*@3 from]_`=_[@3 0])_[@(0.0.255) const]&]
[s2; Возвращает первую позицию любого 
символа из [%-*@3 set] с нулевым окончанием, 
начиная с [%-*@3 from]. Возвращает `-1, если 
не найдено.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:AString`:`:FindFirstOf`(const Upp`:`:AString`:`:String`&`,int`)const:%- [@(0.0.255) i
nt]_[* FindFirstOf]([@(0.0.255) const]_[_^Upp`:`:AString`:`:String^ String][@(0.0.255) `&
]_[*@3 set], [@(0.0.255) int]_[*@3 from]_`=_[@3 0])_[@(0.0.255) const]&]
[s2; Возвращает первую позицию любого 
символа из [%-*@3 set], начиная с [%-*@3 from]. 
Возвращает `-1, если не найдено.&]
[s3; ]]