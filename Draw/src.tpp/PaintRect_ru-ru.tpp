topic "Класс PaintRect";
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_}%RU-RU 
[ {{10000@(113.42.0) [s0; [*@7;4 Класс PaintRect]]}}&]
[s3;%- &]
[s1;:PaintRect`:`:class:%- [@(0.0.255) class]_[* PaintRect]_:_[@(0.0.255) private]_[*@3 Moveab
le]<[* PaintRect]>_&]
[s9; PaintRect `- простой вспомогательный класс, 
комбинирующий Value и ссылку на Display, 
обеспечивая `"визуальный контент`" 
прямоугольника. Он типа [%-*@(0.0.255) Moveable].&]
[s3;%- &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Список Публичных Методов]]}}&]
[s3;%- &]
[s5;:PaintRect`:`:Paint`(Draw`&`,const Rect`&`,Color`,Color`,dword`)const:%- [@(0.0.255) v
oid]_[* Paint]([_^Draw^ Draw][@(0.0.255) `&]_[*@3 w], [@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&
]_[*@3 r], [_^Color^ Color]_[*@3 ink]_`=_SColorText, [_^Color^ Color]_[*@3 paper]_`=_SColor
Paper, [_^dword^ dword]_[*@3 style]_`=_[@3 0])_[@(0.0.255) const]&]
[s2; Вызывает Paint содержащегося Display`'я 
для содержащегося Value.&]
[s7; [%-*C@3 w]-|Draw.&]
[s7; [%-*C@3 r]-|Целевой прямоугольник.&]
[s7; [%-*C@3 ink]-|Рекомендуемый цвет переднего 
плана (ПП).&]
[s7; [%-*C@3 paper]-|Рекомендуемый цвет фона.&]
[s7; [%-*C@3 style]-|Визуальный стиль.&]
[s3;%- &]
[s4;%- &]
[s5;:PaintRect`:`:Paint`(Draw`&`,int`,int`,int`,int`,Color`,Color`,dword`)const:%- [@(0.0.255) v
oid]_[* Paint]([_^Draw^ Draw][@(0.0.255) `&]_[*@3 w], [@(0.0.255) int]_[*@3 x], 
[@(0.0.255) int]_[*@3 y], [@(0.0.255) int]_[*@3 cx], [@(0.0.255) int]_[*@3 cy], 
[_^Color^ Color]_[*@3 ink]_`=_SColorText, [_^Color^ Color]_[*@3 paper]_`=_SColorPaper, 
[_^dword^ dword]_[*@3 style]_`=_[@3 0])_[@(0.0.255) const]&]
[s2; Вызывает содержащийся Display с содержащимся 
Value.&]
[s7; [%-*C@3 w]-|Draw.&]
[s7; [%-*C@3 x, y, cx, cy]-|Целевой прямоугольник 
(лево, верх, ширина, высота).&]
[s7; [%-*C@3 ink]-|Рекомендуемый цвет переднего 
плана (ПП).&]
[s7; [%-*C@3 paper]-|Рекомендуемый цвет фона.&]
[s7; [%-*C@3 style]-|Визуальный стиль.&]
[s3;%- &]
[s4;%- &]
[s5;:PaintRect`:`:GetStdSize`(`)const:%- [_^Size^ Size]_[* GetStdSize]()_[@(0.0.255) const]&]
[s2; Вызывает GetStdSize содержащегося Display`'я 
для содержащегося Value.&]
[s7; [*/ Возвратное значение]-|Предпочитаемый 
Size Value`'а.&]
[s3;%- &]
[s4;%- &]
[s5;:PaintRect`:`:RatioSize`(int`,int`)const:%- [_^Size^ Size]_[* RatioSize]([@(0.0.255) in
t]_[*@3 cx], [@(0.0.255) int]_[*@3 cy])_[@(0.0.255) const]&]
[s2; Вызывает RatioSize содержащегося Display`'я 
для содержащегося Value.&]
[s7; [%-*C@3 cx]-|Требуемая ширина. Если ноль, 
вычисляется, соблюдая пропорциональность 
с cy.&]
[s7; [%-*C@3 cy]-|Требуемая высота. Если ноль, 
вычисляется, соблюдая пропорциональность 
с cx.&]
[s7; [*/ Возвратное значение]-|Размер Value 
для Display`'я.&]
[s3;%- &]
[s4;%- &]
[s5;:PaintRect`:`:RatioSize`(Size`)const:%- [_^Size^ Size]_[* RatioSize]([_^Size^ Size]_[*@3 s
z])_[@(0.0.255) const]&]
[s2; Эквивалентно RatioSize(sz.cx, sz.cy).&]
[s3;%- &]
[s4;%- &]
[s5;:PaintRect`:`:SetDisplay`(const Display`&`):%- [@(0.0.255) void]_[* SetDisplay]([@(0.0.255) c
onst]_[_^Display^ Display][@(0.0.255) `&]_[*@3 d])&]
[s2; Устанавливает Display.&]
[s7; [%-*C@3 d]-|Display.&]
[s3;%- &]
[s4;%- &]
[s5;:PaintRect`:`:SetValue`(const Value`&`):%- [@(0.0.255) void]_[* SetValue]([@(0.0.255) c
onst]_[_^Value^ Value][@(0.0.255) `&]_[*@3 v])&]
[s2; Устанавливает Value.&]
[s7; [%-*C@3 v]-|Value.&]
[s3;%- &]
[s4;%- &]
[s5;:PaintRect`:`:Set`(const Display`&`,const Value`&`):%- [@(0.0.255) void]_[* Set]([@(0.0.255) c
onst]_[_^Display^ Display][@(0.0.255) `&]_[*@3 d], [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&
]_[*@3 v])&]
[s2; Устанавливает Display и Value.&]
[s7; [%-*C@3 d]-|Display.&]
[s7; [%-*C@3 v]-|Value.&]
[s3;%- &]
[s4;%- &]
[s5;:PaintRect`:`:Clear`(`):%- [@(0.0.255) void]_[* Clear]()&]
[s2; Удаляет Display `- последующие вызовы 
Paint действуют как `"no operation`", вызовы 
GetStdSize и RatioSize возвращают Size(0, 0).&]
[s3;%- &]
[s4;%- &]
[s5;:PaintRect`:`:GetValue`(`)const:%- [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[* G
etValue]()_[@(0.0.255) const]&]
[s2; Возвращает это Value.&]
[s7; [*/ Возвратное значение]-|Value.&]
[s3;%- &]
[s4;%- &]
[s5;:PaintRect`:`:GetDisplay`(`)const:%- [@(0.0.255) const]_[_^Display^ Display][@(0.0.255) `&
]_[* GetDisplay]()_[@(0.0.255) const]&]
[s2; Возвращает этот Display.&]
[s7; [*/ Возвратное значение]-|Display.&]
[s3;%- &]
[s4;%- &]
[s5;:PaintRect`:`:operator bool`(`)const:%- [* operator_bool]()_[@(0.0.255) const]&]
[s7; [*/ Возвратное значение]-|true, если Display 
установлен.&]
[s3;%- &]
[s0;* &]
[ {{10000F(128)G(128)@1 [s0; [* Конструктор  detail]]}}&]
[s3;%- &]
[s5;:PaintRect`:`:PaintRect`(`):%- [* PaintRect]()&]
[s2; Конструирует пустой PaintRect, без присваивания
 Display`'я.&]
[s3;%- &]
[s4;%- &]
[s5;:PaintRect`:`:PaintRect`(const Display`&`):%- [* PaintRect]([@(0.0.255) const]_[_^Display^ D
isplay][@(0.0.255) `&]_[*@3 display])&]
[s2; Конструирует PaintRect с указанным Display`'ем.&]
[s7; [%-*C@3 display]-|Display.&]
[s3;%- &]
[s4;%- &]
[s5;:PaintRect`:`:PaintRect`(const Display`&`,const Value`&`):%- [* PaintRect]([@(0.0.255) c
onst]_[_^Display^ Display][@(0.0.255) `&]_[*@3 display], [@(0.0.255) const]_[_^Value^ Value
][@(0.0.255) `&]_[*@3 val])&]
[s2; Конструирует PaintRect с указанными Display 
и Value.&]
[s7; [%-*C@3 display]-|Display.&]
[s7; [%-*C@3 val]-|Value.&]
[s3; &]
[s0; ]]