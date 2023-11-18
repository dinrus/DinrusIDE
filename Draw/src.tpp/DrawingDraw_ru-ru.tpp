topic "Класс DrawingDraw";
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
[ {{10000@(113.42.0) [s0;%- [*@7;4 Класс DrawingDraw]]}}&]
[s3;%- &]
[s1;:DrawingDraw`:`:class:%- [@(0.0.255)3 class][3 _][*3 DrawingDraw][3 _:_][@(0.0.255)3 public][3 _
][*@3;3 Draw]&]
[s9; DrawingDraw `- это Draw, способный сохранять 
операции отрисовки Draw и создавать 
значение Drawing, которое может быть 
повторно воспооизведено посредством 
Draw`::DrawDrawing.&]
[s3;%- &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Список Публичных Методов]]}}&]
[s3;%- &]
[s5;:DrawingDraw`:`:Create`(int`,int`,bool`):%- [@(0.0.255) void]_[* Create]([@(0.0.255) in
t]_[*@3 cx], [@(0.0.255) int]_[*@3 cy], [@(0.0.255) bool]_[*@3 dots]_`=_[@(0.0.255) true])&]
[s5;:DrawingDraw`:`:Create`(Size`,bool`):%- [@(0.0.255) void]_[* Create]([_^Size^ Size]_[*@3 s
z], [@(0.0.255) bool]_[*@3 dots]_`=_[@(0.0.255) true])&]
[s2; Начинает новую сессию записи. Фактически 
важен только размер итогового Drawing, 
чтобы вычислить точное ремасштабирование 
в Draw`::DrawDrawing. [%-*@3 dots ]определяет флаг 
DOTS в Draw:GetInfo(). Если в DrawingDraw уже сохранены 
какие`-либо записи, они удаляются.&]
[s3; &]
[s4; &]
[s5;:DrawingDraw`:`:GetSize`(`)const:%- [_^Size^ Size]_[* GetSize]()_[@(0.0.255) const]&]
[s2; Возвращает размер, установленный 
конструктором или Create.&]
[s3; &]
[s4; &]
[s5;:DrawingDraw`:`:GetResult`(`):%- [_^Drawing^ Drawing]_[* GetResult]()&]
[s2; Возвращает действительный результат.&]
[s3; &]
[s4; &]
[s5;:DrawingDraw`:`:operator Drawing`(`):%- [* operator_Drawing]()&]
[s2; Возвращает GetResult().&]
[s3;%- &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Конструктор  detail]]}}&]
[s3; &]
[s5;:DrawingDraw`:`:DrawingDraw`(`):%- [* DrawingDraw]()&]
[s2; Дефолтный конструктор. Create нужно 
явно вызывать у дефолтно сконструированного
 DrawingDraw.&]
[s3; &]
[s4; &]
[s5;:DrawingDraw`:`:DrawingDraw`(int`,int`,bool`):%- [* DrawingDraw]([@(0.0.255) int]_[*@3 c
x], [@(0.0.255) int]_[*@3 cy], [@(0.0.255) bool]_[*@3 dots]_`=_[@(0.0.255) true])&]
[s5;:DrawingDraw`:`:DrawingDraw`(Size`,bool`):%- [* DrawingDraw]([_^Size^ Size]_[*@3 sz], 
[@(0.0.255) bool]_[*@3 dots]_`=_[@(0.0.255) true])&]
[s2; Вызывает Create, чтобы начать сессию 
записи.&]
[s3; &]
[s0; ]]