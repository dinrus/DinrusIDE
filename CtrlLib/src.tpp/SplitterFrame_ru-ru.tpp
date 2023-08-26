topic "Класс SplitterFrame (ФреймРазделитель)";
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
[ {{10000@3 [s0; [*@(229)4 Класс SplitterFrame (ФреймРазделитель)]]}}&]
[s3; &]
[s1;:SplitterFrame`:`:class:%- [@(0.0.255)3 class][3 _][*3 SplitterFrame][3 _:_][@(0.0.255)3 pub
lic][3 _][*@3;3 CtrlFrame][3 , ][@(0.0.255)3 private][3 _][*@3;3 Ctrl]&]
[s9; Этот класс работает как область рамки 
с изменяемым размером.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Список Публичных Методов]]}}&]
[s3;%- &]
[s5;:SplitterFrame`:`:Serialize`(Stream`&`):%- [@(0.0.255) virtual] 
[@(0.0.255) void]_[* Serialize]([_^Stream^ Stream][@(0.0.255) `&]_[*@3 s])&]
[s2; Сериализует действительный размер 
SplitterFrame.&]
[s3; &]
[s4;%- &]
[s5;:SplitterFrame`:`:Set`(Ctrl`&`,int`,int`):%- [_^SplitterFrame^ SplitterFrame][@(0.0.255) `&
]_[* Set]([_^Ctrl^ Ctrl][@(0.0.255) `&]_[*@3 c], [@(0.0.255) int]_[*@3 size], 
[@(0.0.255) int]_[*@3 type])&]
[s2; Устанавливает SplitterFrame, содержащий 
виджет [%-*@3 c], с начальным размером 
[%-*@3 size ](в пикселях).[%-*@3  type] определяет 
позицию `- она может быть SplitterFrame`::[* LEFT], 
SplitterFrame`::[* RIGHT], SplitterFrame`::[* TOP] или SplitterFrame`::[* BOTTOM].&]
[s3; &]
[s4;%- &]
[s5;:SplitterFrame`:`:Left`(Ctrl`&`,int`):%- [_^SplitterFrame^ SplitterFrame][@(0.0.255) `&
]_[* Left]([_^Ctrl^ Ctrl][@(0.0.255) `&]_[*@3 c], [@(0.0.255) int]_[*@3 size])&]
[s2; То же, что и Set([%-*@3 c] , [%-*@3 size], SplitterFrame`::LEFT).&]
[s3; &]
[s4;%- &]
[s5;:SplitterFrame`:`:Top`(Ctrl`&`,int`):%- [_^SplitterFrame^ SplitterFrame][@(0.0.255) `&
]_[* Top]([_^Ctrl^ Ctrl][@(0.0.255) `&]_[*@3 c], [@(0.0.255) int]_[*@3 size])&]
[s2; То же, что и Set([%-*@3 c] , [%-*@3 size], SplitterFrame`::TOP).&]
[s3; &]
[s4;%- &]
[s5;:SplitterFrame`:`:Right`(Ctrl`&`,int`):%- [_^SplitterFrame^ SplitterFrame][@(0.0.255) `&
]_[* Right]([_^Ctrl^ Ctrl][@(0.0.255) `&]_[*@3 c], [@(0.0.255) int]_[*@3 size])&]
[s2; То же, что и Set([%-*@3 c] , [%-*@3 size], SplitterFrame`::RIGHT).&]
[s3; &]
[s4;%- &]
[s5;:SplitterFrame`:`:Bottom`(Ctrl`&`,int`):%- [_^SplitterFrame^ SplitterFrame][@(0.0.255) `&
]_[* Bottom]([_^Ctrl^ Ctrl][@(0.0.255) `&]_[*@3 c], [@(0.0.255) int]_[*@3 size])&]
[s2; То же, что и Set([%-*@3 c] , [%-*@3 size], SplitterFrame`::BOTTOM).&]
[s3; &]
[s4;%- &]
[s5;:SplitterFrame`:`:MinSize`(int`):%- [_^SplitterFrame^ SplitterFrame][@(0.0.255) `&]_[* M
inSize]([@(0.0.255) int]_[*@3 sz])&]
[s2; Устанавливает минимальный размер.&]
[s3; &]
[s4;%- &]
[s5;:SplitterFrame`:`:SizeMin`(int`):%- [_^SplitterFrame^ SplitterFrame][@(0.0.255) `&]_[* S
izeMin]([@(0.0.255) int]_[*@3 sz])&]
[s2; Устанавливает минимальный размер 
следующего фрейма в списке или конечный 
вид, если его нет, `- фактически этим 
определяется максимальный размер 
SplitterFrame.&]
[s3; &]
[s4;%- &]
[s5;:SplitterFrame`:`:GetType`(`)const:%- [@(0.0.255) int]_[* GetType]()_[@(0.0.255) const]&]
[s2; Возвращает тип (в пикселях).&]
[s3; &]
[s4;%- &]
[s5;:SplitterFrame`:`:GetSize`(`)const:%- [@(0.0.255) virtual] [@(0.0.255) int]_[* GetSize](
)_[@(0.0.255) const]&]
[s2; Возвращает текущий размер.&]
[s3; &]
[s4;%- &]
[s5;:SplitterFrame`:`:SetSize`(int`):%- [@(0.0.255) void]_[* SetSize]([@(0.0.255) int]_[*@3 s
z])&]
[s2; Устанавливает размер (в пикселях).&]
[s3; &]
[s0; ]]