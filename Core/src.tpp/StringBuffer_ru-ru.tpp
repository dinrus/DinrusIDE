topic "Классы StringBuffer и WStringBuffer";
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
[{_} 
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Класс StringBuffer]]}}&]
[s3;%RU-RU &]
[s1;:StringBuffer`:`:class: [@(0.0.255)3 class][3 _][*3 StringBuffer][3 _:_][@(0.0.255)3 private
][3 _][*@3;3 NoCopy]&]
[s9;%RU-RU Класс, используемый при работе 
с кодом, нуждающемся в символьном 
буфкрировании, наподобие кода на 
языке Си. Поддерживает пик`-присваивание 
константное по времени (constant time pick 
assignment) в/из [* String].&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Детали Конструктора]]}}&]
[s3; &]
[s5;:StringBuffer`:`:StringBuffer`(`): [* StringBuffer]()&]
[s2;%RU-RU Конструирует пустой буфер.&]
[s3; &]
[s4; &]
[s5;:StringBuffer`:`:StringBuffer`(int`): [* StringBuffer]([@(0.0.255) int]_[*@3 len])&]
[s2;%RU-RU Конструирует буфер длиной в [%-*@3 len] 
символов.&]
[s3;%RU-RU &]
[s4; &]
[s5;:StringBuffer`:`:StringBuffer`(String`&`): [* StringBuffer]([_^String^ String][@(0.0.255) `&
]_[*@3 s])&]
[s2;%RU-RU Присваивает контент [%-*@3 s] [* StringBuffer`'у]. 
Очищает [%-*@3 s].&]
[s3;%RU-RU &]
[s4; &]
[s5;:StringBuffer`:`:`~StringBuffer`(`): [@(0.0.255) `~][* StringBuffer]()&]
[s2;%RU-RU Дефолтный деструктор .&]
[s3; &]
[s0;i448;a25;kKO9;:noref:@(0.0.255) &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Публичных Методов]]}}&]
[s3; &]
[s5;:StringBuffer`:`:Begin`(`): [@(0.0.255) char]_`*[* Begin]()&]
[s5;:Upp`:`:StringBuffer`:`:begin`(`): [@(0.0.255) char]_`*[* begin]()&]
[s2;%RU-RU Возвращает указатель на буфер 
символов. Операции по его изменению 
приводят к повреждению этого указателя. 
Заметьте, что эта операция не константна 
и она записывает завершающий ноль 
(terminating zero) в конце, `*end().&]
[s3; &]
[s4; &]
[s5;:StringBuffer`:`:End`(`): [@(0.0.255) char]_`*[* End]()&]
[s5;:Upp`:`:StringBuffer`:`:end`(`): [@(0.0.255) char]_`*[* end]()&]
[s2;%RU-RU Возвращает begin() `+ GetCount(). Операции 
по его изменению приводят к повреждению 
этого указателя. Заметьте, что эта 
операция не константна и она записывает 
завершающий ноль (terminating zero) в конце, 
`*end().&]
[s3; &]
[s4; &]
[s5;:StringBuffer`:`:Cat`(int`): [@(0.0.255) void]_[* Cat]([@(0.0.255) int]_[*@3 c])&]
[s2;%RU-RU Приставляет единичный символ 
[%-*@3 c].&]
[s3;%RU-RU &]
[s4; &]
[s5;:StringBuffer`:`:Cat`(int`,int`): [@(0.0.255) void]_[* Cat]([@(0.0.255) int]_[*@3 c], 
[@(0.0.255) int]_[*@3 count])&]
[s2;%RU-RU Приставляет [%-*@3 count] символов [%-*@3 c].&]
[s3;%RU-RU &]
[s4; &]
[s5;:StringBuffer`:`:Cat`(const char`*`): [@(0.0.255) void]_[* Cat]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 s])&]
[s5;:StringBuffer`:`:Cat`(const String`&`): [@(0.0.255) void]_[* Cat]([@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 s])&]
[s2;%RU-RU Приставляет строку [%-*@3 s].&]
[s3;%RU-RU &]
[s4; &]
[s5;:StringBuffer`:`:Cat`(const char`*`,int`): [@(0.0.255) void]_[* Cat]([@(0.0.255) const]_
[@(0.0.255) char]_`*[*@3 s], [@(0.0.255) int]_[*@3 l])&]
[s2;%RU-RU Приставляет первые [%-*@3 l] символов 
из [%-*@3 s].&]
[s3;%RU-RU &]
[s4; &]
[s5;:StringBuffer`:`:Cat`(const char`*`,const char`*`): [@(0.0.255) void]_[* Cat]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 s], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 e])&]
[s2;%RU-RU То же, что и Cat(s, e `- s).&]
[s3;%RU-RU &]
[s4; &]
[s5;:StringBuffer`:`:GetLength`(`)const: [@(0.0.255) int]_[* GetLength]()_[@(0.0.255) const
]&]
[s5;:StringBuffer`:`:GetCount`(`)const: [@(0.0.255) int]_[* GetCount]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает число символов, находящихся 
в буфере.&]
[s3; &]
[s4; &]
[s5;:StringBuffer`:`:SetLength`(int`): [@(0.0.255) void]_[* SetLength]([@(0.0.255) int]_[*@3 l
])&]
[s5;:StringBuffer`:`:SetCount`(int`): [@(0.0.255) void]_[* SetCount]([@(0.0.255) int]_[*@3 l])
&]
[s2;%RU-RU Устанавливает длину символьного 
буфера в [%-*@3 l].&]
[s3;%RU-RU &]
[s4; &]
[s5;:StringBuffer`:`:Strlen`(`): [@(0.0.255) void]_[* Strlen]()&]
[s2;%RU-RU Устанавливает длину символьного 
буфера в длину строки с нулевым окончанием, 
сохраняемой в этом буфере (до первого 
символа нуля). Применяется для преобразовани
я строк Си из системных в String.&]
[s3; &]
[s4; &]
[s5;:StringBuffer`:`:Clear`(`): [@(0.0.255) void]_[* Clear]()&]
[s2;%RU-RU Очищает содержимое строкового 
буфера.&]
[s3; &]
[s4; &]
[s5;:StringBuffer`:`:Reserve`(int`): [@(0.0.255) void]_[* Reserve]([@(0.0.255) int]_[*@3 r])&]
[s2;%RU-RU Подсказка для предварительного 
размещения внутреннего буфера для 
конкатенации следующих r символов.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:StringBuffer`:`:Shrink`(`): [@(0.0.255) void]_[* Shrink]()&]
[s2;%RU-RU Сжимает внутренний буфер для 
уменьшения потребления памяти.&]
[s3; &]
[s4; &]
[s5;:StringBuffer`:`:GetAlloc`(`)const: [@(0.0.255) int]_[* GetAlloc]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает максимальное число 
символов во внутреннем буфере.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:StringBuffer`:`:operator`*`(`): [@(0.0.255) char`&]_[* operator`*]()&]
[s2;%RU-RU Возвращает `*begin().&]
[s3; &]
[s4; &]
[s5;:Upp`:`:StringBuffer`:`:operator`[`]`(int`): [@(0.0.255) char`&]_[* operator`[`]]([@(0.0.255) i
nt]_[*@3 i])&]
[s2;%RU-RU Возвращает begin()`[[%-*@3 i]`].&]
[s3; &]
[s4; &]
[s5;:StringBuffer`:`:operator char`*`(`): [* operator_char`*]()&]
[s5;:StringBuffer`:`:operator`~`(`): [@(0.0.255) char]_`*[* operator`~]()&]
[s2;%RU-RU Возвращает begin().&]
[s3; &]
[s4; &]
[s5;:Upp`:`:StringBuffer`:`:operator byte`*`(`): [* operator_byte`*]()&]
[s2;%RU-RU Возвращает (byte `*)begin().&]
[s3; &]
[s4; &]
[s5;:Upp`:`:StringBuffer`:`:operator void`*`(`): [* operator_void`*]()&]
[s2;%RU-RU Возвращает begin().&]
[s3; &]
[s4; &]
[s5;:StringBuffer`:`:operator`=`(String`&`): [@(0.0.255) void]_[* operator`=]([_^String^ St
ring][@(0.0.255) `&]_[*@3 s])&]
[s2;%RU-RU Очищает строковый буфер и присваевает 
ему содержимое [%-*@3 s]. Очищает [%-*@3 s].&]
[s0; &]
[s0; &]
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Класс WStringBuffer]]}}&]
[s3; &]
[s1;:WStringBuffer`:`:class: [@(0.0.255)3 class][3 _][*3 WStringBuffer][3 _:_][@(0.0.255)3 priva
te][3 _][*@3;3 NoCopy]&]
[s9;%RU-RU Класс, используемый при работе 
с кодом, требующим наличия широкосимвольног
о буфера (wide character buffer), наподобии кода 
на языке Си. Поддерживает пик`-присваивание 
константное по времени (constant time pick 
assignment) в/из [* WString].&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Детали Конструктора]]}}&]
[s3; &]
[s5;:WStringBuffer`:`:WStringBuffer`(`): [* WStringBuffer]()&]
[s2;%RU-RU Конструирует пустой буфер.&]
[s3; &]
[s4; &]
[s5;:WStringBuffer`:`:WStringBuffer`(int`): [* WStringBuffer]([@(0.0.255) int]_[*@3 len])&]
[s2;%RU-RU Конструирует буфер длиной в [%-*@3 len] 
символов.&]
[s3;%RU-RU &]
[s4; &]
[s5;:WStringBuffer`:`:WStringBuffer`(WString`&`): [* WStringBuffer]([_^WString^ WString][@(0.0.255) `&
]_[*@3 s])&]
[s2;%RU-RU Присваивает контент [%-*@3 s] [* StringBuffer`'у]. 
Очищает [%-*@3 s].&]
[s3;%RU-RU &]
[s4; &]
[s5;:WStringBuffer`:`:`~WStringBuffer`(`): [@(0.0.255) `~][* WStringBuffer]()&]
[s2;%RU-RU Дефолтный деструктор.&]
[s3; &]
[s0;i448;a25;kKO9;:noref:@(0.0.255) &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Публичных Методов]]}}&]
[s3; &]
[s5;:WStringBuffer`:`:Begin`(`): [_^wchar^ wchar]_`*[* Begin]()&]
[s5;:Upp`:`:WStringBuffer`:`:begin`(`): [_^Upp`:`:wchar^ wchar]_`*[* begin]()&]
[s2;%RU-RU Возвращает указатель на буфер 
символов. Операции по его изменению 
приводят к повреждению этого указателя. 
Заметьте, что эта операция не константна 
и она записывает завершающий ноль 
(terminating zero) в конце, `*end().&]
[s3; &]
[s4; &]
[s5;:WStringBuffer`:`:End`(`): [_^wchar^ wchar]_`*[* End]()&]
[s5;:Upp`:`:WStringBuffer`:`:end`(`): [_^Upp`:`:wchar^ wchar]_`*[* end]()&]
[s2;%RU-RU Возвращает begin() `+ GetCount(). Операции 
по его изменению приводят к повреждению 
этого указателя. Заметьте, что эта 
операция не константна и она записывает 
завершающий ноль (terminating zero) в конце, 
`*end().&]
[s3; &]
[s4; &]
[s5;:WStringBuffer`:`:Cat`(int`): [@(0.0.255) void]_[* Cat]([@(0.0.255) int]_[*@3 c])&]
[s2;%RU-RU Приставляет единичный символ 
[%-*@3 c].&]
[s3;%RU-RU &]
[s4; &]
[s5;:WStringBuffer`:`:Cat`(int`,int`): [@(0.0.255) void]_[* Cat]([@(0.0.255) int]_[*@3 c], 
[@(0.0.255) int]_[*@3 count])&]
[s2;%RU-RU Приставляет [%-*@3 count] символов [%-*@3 c].&]
[s3;%RU-RU &]
[s4; &]
[s5;:WStringBuffer`:`:Cat`(const wchar`*`): [@(0.0.255) void]_[* Cat]([@(0.0.255) const]_[_^wchar^ w
char]_`*[*@3 s])&]
[s5;:WStringBuffer`:`:Cat`(const WString`&`): [@(0.0.255) void]_[* Cat]([@(0.0.255) const]_
[_^WString^ WString][@(0.0.255) `&]_[*@3 s])&]
[s5;:WStringBuffer`:`:Cat`(const char`*`): [@(0.0.255) void]_[* Cat]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 s])&]
[s2;%RU-RU Приставляет строку [%-*@3 s].&]
[s3;%RU-RU &]
[s4; &]
[s5;:WStringBuffer`:`:Cat`(const wchar`*`,int`): [@(0.0.255) void]_[* Cat]([@(0.0.255) cons
t]_[_^wchar^ wchar]_`*[*@3 s], [@(0.0.255) int]_[*@3 l])&]
[s2;%RU-RU Приставляет первые [%-*@3 l] символов 
из [%-*@3 s].&]
[s3;%RU-RU &]
[s4; &]
[s5;:WStringBuffer`:`:Cat`(const wchar`*`,const wchar`*`): [@(0.0.255) void]_[* Cat]([@(0.0.255) c
onst]_[_^wchar^ wchar]_`*[*@3 s], [@(0.0.255) const]_[_^wchar^ wchar]_`*[*@3 e])&]
[s2;%RU-RU То же, что и Cat([%-*@3 s], [%-*@3 e] `- [%-*@3 s]).&]
[s3;%RU-RU &]
[s4; &]
[s5;:WStringBuffer`:`:GetLength`(`)const: [@(0.0.255) int]_[* GetLength]()_[@(0.0.255) cons
t]&]
[s5;:WStringBuffer`:`:GetCount`(`)const: [@(0.0.255) int]_[* GetCount]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает число символов, находящихся 
в буфере.&]
[s3; &]
[s4; &]
[s5;:WStringBuffer`:`:SetLength`(int`): [@(0.0.255) void]_[* SetLength]([@(0.0.255) int]_[*@3 l
])&]
[s5;:WStringBuffer`:`:SetCount`(int`): [@(0.0.255) void]_[* SetCount]([@(0.0.255) int]_[*@3 l
])&]
[s2;%RU-RU Устанавливает длину символьного 
буфера в [%-*@3 l].&]
[s3;%RU-RU &]
[s4; &]
[s5;:WStringBuffer`:`:Strlen`(`): [@(0.0.255) void]_[* Strlen]()&]
[s2;%RU-RU Устанавливает длину символьного 
буфера в длину строки с нулевым окончанием, 
сохраняемой в этом буфере (до первого 
символа нуля). Применяется для преобразовани
я строк Си из системных в WString.&]
[s3; &]
[s4; &]
[s5;:WStringBuffer`:`:Clear`(`): [@(0.0.255) void]_[* Clear]()&]
[s2;%RU-RU Очищает содержимое строкового 
буфера.&]
[s3; &]
[s4; &]
[s5;:WStringBuffer`:`:Reserve`(int`): [@(0.0.255) void]_[* Reserve]([@(0.0.255) int]_[*@3 r])
&]
[s2;%RU-RU Предварительно размещает (преаллоцирует
) внутренний буфер (избегая изменения 
размера внутреннего буфера до [%-*@3 r] 
символов).&]
[s3;%RU-RU &]
[s4; &]
[s5;:WStringBuffer`:`:GetAlloc`(`)const: [@(0.0.255) int]_[* GetAlloc]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает максимальное число 
символов во внутреннем буфере.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:WStringBuffer`:`:operator`*`(`): [_^Upp`:`:wchar^ wchar][@(0.0.255) `&]_[* oper
ator`*]()&]
[s2;%RU-RU Возвращает `*begin().&]
[s3; &]
[s4; &]
[s5;:Upp`:`:WStringBuffer`:`:operator`[`]`(int`): [_^Upp`:`:wchar^ wchar][@(0.0.255) `&]_
[* operator`[`]]([@(0.0.255) int]_[*@3 i])&]
[s2;%RU-RU Возвращает begin()`[[%-*@3 i]`].&]
[s3; &]
[s4; &]
[s5;:WStringBuffer`:`:operator wchar`*`(`): [* operator_wchar`*]()&]
[s5;:WStringBuffer`:`:operator`~`(`): [_^wchar^ wchar]_`*[* operator`~]()&]
[s2;%RU-RU Возвращает begin().&]
[s3; &]
[s4; &]
[s5;:Upp`:`:WStringBuffer`:`:operator int16`*`(`): [* operator_int16`*]()&]
[s2;%RU-RU Возвращает (int16`*)begin().&]
[s3; &]
[s4; &]
[s5;:Upp`:`:WStringBuffer`:`:operator void`*`(`): [* operator_void`*]()&]
[s2;%RU-RU Возвращает begin().&]
[s3; &]
[s4; &]
[s5;:WStringBuffer`:`:operator`=`(WString`&`): [@(0.0.255) void]_[* operator`=]([_^WString^ W
String][@(0.0.255) `&]_[*@3 s])&]
[s2;%RU-RU Очищает строковый буфер и присваивает 
ему содержимое [%-*@3 s]. Очищает [%-*@3 s].&]
[s3; &]
[s0; ]]