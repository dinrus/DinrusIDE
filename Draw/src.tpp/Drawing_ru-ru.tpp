topic "Класс Drawing";
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
[{_} 
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Класс Drawing]]}}&]
[s3; &]
[s1;:Drawing`:`:class: [@(0.0.255)3 class][3 _][*3 Drawing][3 _:_][@(0.0.255)3 private][3 _][*@3;3 A
ssignValueTypeNo][3 <][*3 Drawing][3 , ][@3;3 49][3 , ][_^Moveable^3 Moveable][3 <][*3 Drawing][3 >
_>_]&]
[s9;%RU-RU Этот конкретный класс, с полной 
семантикой значений (быстрой глубокой 
копией), представляет собой запись 
операций рисования Draw, которую можно 
воспроизвести повторно в любое время. 
Другими словами, Drawing `- сорт векторного 
формата. Чтобы создать Drawing, используется 
класс [^DrawingDraw^ DrawingDraw].&]
[s9;%RU-RU Drawing совместим с Value, у него набор 
фич `"rich`", `- как Value, он поддерживает 
сравнение на равенство, сериализацию, 
совместим с Null и поддерживает хэш`-значение.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Публичных Методов]]}}&]
[s3; &]
[s5;:Drawing`:`:operator bool`(`)const: [* operator_bool]()_[@(0.0.255) const]&]
[s2;%RU-RU True, если Drawing непустой.&]
[s3; &]
[s4; &]
[s5;:Drawing`:`:GetSize`(`)const: [_^Size^ Size]_[* GetSize]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает размер для Drawing `- значение, 
явно определённое в DrawingDraw, или установленно
е посредством SetSize.&]
[s3; &]
[s4; &]
[s5;:Drawing`:`:SetSize`(Size`): [@(0.0.255) void]_[* SetSize]([_^Size^ Size]_[*@3 sz])&]
[s5;:Drawing`:`:SetSize`(int`,int`): [@(0.0.255) void]_[* SetSize]([@(0.0.255) int]_[*@3 cx],
 [@(0.0.255) int]_[*@3 cy])&]
[s2;%RU-RU Устанавливает размер для Drawing.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Drawing`:`:RatioSize`(int`,int`)const: [_^Size^ Size]_[* RatioSize]([@(0.0.255) int]_[*@3 c
x], [@(0.0.255) int]_[*@3 cy])_[@(0.0.255) const]&]
[s5;:Drawing`:`:RatioSize`(Size`)const: [_^Size^ Size]_[* RatioSize]([_^Size^ Size]_[*@3 sz])
_[@(0.0.255) const]&]
[s2;%RU-RU Если либо ширина, либо высота, 
указанная в качестве параметра, равна 
нулю, то она вычисляется на основе 
размера текущего Drawing, а ненулевая 
высота или ширина сохраняет пропорции. 
Если как высота, так и ширина равны 
нулю, то возвращается размер Drawing.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Drawing`:`:Clear`(`): [@(0.0.255) void]_[* Clear]()&]
[s2;%RU-RU Устанавливает Drawing пустым.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Drawing`:`:Append`(Drawing`&`): [@(0.0.255) void]_[* Append]([_^Drawing^ Drawing][@(0.0.255) `&
]_[*@3 dw])&]
[s2;%RU-RU Приставляет в конце другой Drawing.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Drawing`:`:Serialize`(Stream`&`): [@(0.0.255) void]_[* Serialize]([_^Stream^ Stream][@(0.0.255) `&
]_[*@3 s])&]
[s2;%RU-RU Сериализует контент у Drawing потоком 
[%-*@3 s].&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Drawing`:`:IsNullInstance`(`)const: [@(0.0.255) bool]_[* IsNullInstance]()_[@(0.0.255) c
onst]&]
[s2;%RU-RU True, если Drawing пустой.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Drawing`:`:operator`=`=`(const Drawing`&`)const: [@(0.0.255) bool]_[* operator`=`=](
[@(0.0.255) const]_[_^Drawing^ Drawing][@(0.0.255) `&]_[*@3 b])_[@(0.0.255) const]&]
[s2;%RU-RU Tru, если оба Drawing`'а равны.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Drawing`:`:ToString`(`)const: [_^String^ String]_[* ToString]()_[@(0.0.255) const]&]
[s2;%RU-RU Текстовое представление для целей 
отладки.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Drawing`:`:GetHashValue`(`)const: [@(0.0.255) unsigned]_[* GetHashValue]()_[@(0.0.255) c
onst]&]
[s2;%RU-RU Хэш`-значение.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Drawing`:`:operator Value`(`)const: [* operator_Value]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает Drawing как Value `- Drawing является 
`"rich`" Value, поддерживающим оператор 
`=`=, хэширование, Null и сериализацию.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Детали конструктора]]}}&]
[s3; &]
[s5;:Drawing`:`:Drawing`(const Value`&`): [* Drawing]([@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&
]_[*@3 src])&]
[s2;%RU-RU Конструктор глубокой копии.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Drawing`:`:Drawing`(`): [* Drawing]()&]
[s2;%RU-RU Дефолтный конструктор.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Drawing`:`:Drawing`(const Nuller`&`): [* Drawing]([@(0.0.255) const]_[_^Nuller^ Nuller
][@(0.0.255) `&])&]
[s2; Конструктор Null (предоставляет Drawing 
Null).&]
[s3; &]
[s0; ]]